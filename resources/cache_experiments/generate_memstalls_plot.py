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

  l1bound = 0
  l2bound = 0
  l3bound = 0
  drambound = 0
  storebound = 0

  fileList = glob.glob(reportRoot + "/" + target + "." + size + ".memstalls")
  assert len(fileList) == 1
  fileName = fileList[0]

  f = open(fileName,"r")
  if f==0:
    return

  while True:
    l = f.readline()
    if "tma_l1_bound" in l:
      cols = l.split("#")
      cols = cols[1].split("%")
      l1bound = float(cols[0])
    if "tma_l2_bound" in l:
      cols = l.split("#")
      cols = cols[1].split("%")
      l2bound = float(cols[0])
    if "tma_l3_bound" in l:
      cols = l.split("#")
      cols = cols[1].split("%")
      l3bound = float(cols[0])
    if "tma_dram_bound" in l:
      cols = l.split("#")
      cols = cols[1].split("%")
      drambound = float(cols[0])
    if "tma_store_bound" in l:
      cols = l.split("#")
      cols = cols[1].split("%")
      storebound = float(cols[0])
    if len(l)==0:
      break
  f.close

  # print ("target = %s, size = %s, l3misses = %f, l2misses = %f, l1misses=%f " % (target, size, l3misses, l2misses, l1misses))
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
  f.write("%10s %10s %10s %10s %10s" % ("L1Bound", "L2Bound", "L3Bound", "DRAMBound", "StoreBound"))
  f.write('\n')
  for size in sizes:
    f.write("%10s " % size)
    getPercentage(reportRoot, target, size)
    f.write("%10.2f %10.2f %10.2f %10.2f %10.2f" % (l1bound, l2bound, l3bound, drambound, storebound))
    f.write('\n')
  f.close

if __name__ == "__main__":
  main()
