#!/usr/bin/python

import sys
import os
import getopt
import glob

def getMetric(reportRoot, prefix, policy, size, metric):
  fileList = glob.glob(reportRoot + "/" + prefix + "." + policy + "." + size + ".prof")
  assert len(fileList) == 1
  fileName = fileList[0]

  f = open(fileName,"r")
  if f==0:
    return 0

  ret = 0
  while True:
    l = f.readline()
    if l.find(metric) != -1:
      cols = l.split()
      ret = cols[-1].replace("%","").replace("GB/s", "")
      if ret.find("MB/s") != -1:
        ret = ret.replace("MB/s", "")
        ret = float(ret) / 1000
      break
    if len(l)==0:
      break
  f.close
  return float(ret)

def getTime(reportRoot, prefix, policy, size, comp):
  fileList = glob.glob(reportRoot + "/" + prefix + "." + policy + "." + size + ".perf")
  assert len(fileList) == 1
  fileName = fileList[0]

  f = open(fileName,"r")
  if f==0:
    return 0

  time = 0
  while True:
    l = f.readline()
    if l.find(comp) != -1:
      cols = l.split(" ")
      time = float(cols[2].replace("\n","").replace(" ", ""))
      break
    if len(l)==0:
      break
  f.close
  return time

def main():
  try:
    opts, args = getopt.getopt(sys.argv[1:],"hi:o:t:p:",["inputDir=","outputFile=","targetName="])
  except getopt.GetoptError:
    print ('generate_plot.py -i <input directory> -o <output file> -t <target name> -p <policy>')
    sys.exit(1)
  for opt, arg in opts:
    if opt == '-h':
       print ('generate_plot.py -i <input directory> -o <output file> -t <target name> -p <policy>')
       sys.exit()
    elif opt in ("-i", "--inputDir"):
       reportRoot = arg
    elif opt in ("-o", "--outputFile"):
       fileName = arg
    elif opt in ("-t", "--targetName"):
       targetName = arg
    elif opt in ("-p", "--policy"):
       policy = arg

  # Get all files for target
  files = [f for f in os.listdir(reportRoot) if f"{targetName}" in f and f"{policy}" in f.split(".")[-3] and f"prof" in f.split(".")[-1]]
  # Get all sizes executed for target
  files.sort(key=lambda k: int(k.split('.')[2], 0) )
  sizes = [ f.split('.')[2] for f in files ]

  metrics = ["global_hit_rate", "l2_tex_read_hit_rate", "dram_read_throughput", "flop_count_sp", "gflops_per_second", "achieved_occupancy", "sm_efficiency", "warp_execution_efficiency", "eligible_warps_per_cycle", "shared_load_transactions", "gld_transactions", "pct_shared", "pct_l1", "pct_l2", "pct_dram"]

  f = open(fileName,"w")
  f.write("%15s " % "BlockSizes")
  for metric in metrics:
    formatString = "%" + str(len(metric)) + "s "
    f.write(formatString % metric)
  f.write('\n')
  for size in sizes:
    if targetName.find("vec") != -1:
      f.write("%15d " % int(size))
    else:
      f.write("%15d " % (int(size) * int(size)))

    # Precalculate memory profile metrics
    shared_load_transactions = getMetric(reportRoot, targetName, policy, size, "shared_load_transactions")
    gld_transactions = getMetric(reportRoot, targetName, policy, size, "gld_transactions")
    l1_hit_rate = getMetric(reportRoot, targetName, policy, size, "global_hit_rate") / 100.0
    l2_hit_rate = getMetric(reportRoot, targetName, policy, size, "l2_tex_read_hit_rate") / 100.0

    # Bytes per transaction (Pascal-specific)
    SHARED_BYTES_PER_TXN = 128   # 32 banks × 4 bytes (assumes no bank conflicts)
    GLOBAL_BYTES_PER_TXN = 32    # one L1/L2/DRAM sector

    shared_bytes = shared_load_transactions * SHARED_BYTES_PER_TXN
    gld_bytes = gld_transactions * GLOBAL_BYTES_PER_TXN

    l1_hit_bytes = gld_bytes * l1_hit_rate
    l1_miss_bytes = gld_bytes * (1 - l1_hit_rate)
    l2_hit_bytes = l1_miss_bytes * l2_hit_rate
    dram_bytes = l1_miss_bytes * (1 - l2_hit_rate)

    total_bytes = shared_bytes + gld_bytes

    pct_shared = 100 * shared_bytes / total_bytes
    pct_l1     = 100 * l1_hit_bytes / total_bytes
    pct_l2     = 100 * l2_hit_bytes / total_bytes
    pct_dram   = 100 * dram_bytes / total_bytes

    for metric in metrics:
      if metric == "gflops_per_second":
        flop_count_sp = getMetric(reportRoot, targetName, policy, size, "flop_count_sp")
        time = getTime(reportRoot, targetName, policy, str(size), "Compute")
        value = flop_count_sp / time / 1e9
        formatString = "%" + str(len(metric)) + ".2f "
        f.write(formatString % value)
      elif metric == "pct_shared":
        formatString = "%" + str(len(metric)) + ".2f "
        f.write(formatString % pct_shared)
      elif metric == "pct_l1":
        formatString = "%" + str(len(metric)) + ".2f "
        f.write(formatString % pct_l1)
      elif metric == "pct_l2":
        formatString = "%" + str(len(metric)) + ".2f "
        f.write(formatString % pct_l2)
      elif metric == "pct_dram":
        formatString = "%" + str(len(metric)) + ".2f "
        f.write(formatString % pct_dram)
      else:
        value = getMetric(reportRoot, targetName, policy, size, metric)
        formatString = "%" + str(len(metric)) + ".2f "
        f.write(formatString % value)
    f.write('\n')
  f.close

if __name__ == "__main__":
  main()
