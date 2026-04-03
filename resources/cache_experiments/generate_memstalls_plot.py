#!/usr/bin/python

import sys
import os
import getopt
import glob

l1missrate = 0
l2missrate = 0
l3missrate = 0

def getPercentage(reportRoot, target, size):
  global l1bound
  global l2bound
  global l3bound
  global drambound
  global storebound

  fileList = glob.glob(reportRoot + "/" + target + "." + size + ".memstalls")
  assert len(fileList) == 1
  fileName = fileList[0]

  f = open(fileName,"r")
  if f==0:
    return

  while True:
    l = f.readline()
    if "cycles:u" in l:
      cols = l.split()
      cycles = float(cols[0].replace(",", ""))
    if "cycle_activity.stalls_mem_any:u" in l:
      cols = l.split()
      stalls_mem_any = float(cols[0].replace(",", ""))
    if "cycle_activity.stalls_l1d_miss:u" in l:
      cols = l.split()
      stalls_l1d_miss = float(cols[0].replace(",", ""))
    if "cycle_activity.stalls_l2_miss:u" in l:
      cols = l.split()
      stalls_l2_miss = float(cols[0].replace(",", ""))
    if "cycle_activity.stalls_l3_miss:u" in l:
      cols = l.split()
      stalls_l3_miss = float(cols[0].replace(",", ""))
    if len(l)==0:
      break
  f.close

  l1bound = 100 * (stalls_mem_any - stalls_l1d_miss) / cycles
  l2bound = 100 * (stalls_l1d_miss - stalls_l2_miss) / cycles
  l3bound = 100 * (stalls_l2_miss - stalls_l3_miss) / cycles
  drambound = 100 * stalls_l3_miss / cycles

  # print ("target = %s, size = %s, cycles = %f" % (target, size, cycles))

  return

def main():
  try:
    opts, args = getopt.getopt(sys.argv[1:],"hi:o:t:",["inputDir=","outputFile=","targetName-"])
  except getopt.GetoptError:
    print ('generate_cachestats_plot.py -i <input directory> -o <output file> -t <target name>')
    sys.exit(1)
  for opt, arg in opts:
    if opt == '-h':
      print ('generate_cachestats_plot.py -i <input directory> -o <output file> -t <target name>')
      sys.exit()
    elif opt in ("-i", "--inputDir"):
      reportRoot = arg
    elif opt in ("-o", "--outputFile"):
      fileName = arg
    elif opt in ("-t", "--targetName"):
      target = arg

  # Get all files for target
  files = [f for f in os.listdir(reportRoot) if f"{target}" in f and "memstalls" in f.split(".")[-1]]
  # Get all sizes executed for target
  files.sort(key=lambda k: int(k.split('.')[2], 0) )
  sizes = [ f.split('.')[2] for f in files ]

  f = open(fileName,"w")
  f.write("%10s " % "#Elements")
  f.write("%10s %10s %10s %10s" % ("L1Bound", "L2Bound", "L3Bound", "DRAMBound"))
  f.write('\n')
  for size in sizes:
    f.write("%10s " % size)
    getPercentage(reportRoot, target, size)
    f.write("%10.2f %10.2f %10.2f %10.2f" % (l1bound, l2bound, l3bound, drambound))
    f.write('\n')
  f.close

if __name__ == "__main__":
  main()
