#!/usr/bin/python

import sys
import os
import getopt
import glob

l1missrate = 0
l2missrate = 0
l3missrate = 0

def getPercentage(reportRoot, target, size, extension):
  global l1missrate
  global l2missrate
  global l3missrate

  loads = 0
  l1hits = 0
  l1misses = 0
  l2hits = 0
  l2misses = 0
  l3hits = 0
  l3misses = 0

  fileList = glob.glob(reportRoot + "/" + target + "." + size + "." + extension)
  assert len(fileList) == 1
  fileName = fileList[0]

  f = open(fileName,"r")
  if f==0:
    return

  while True:
    l = f.readline()
    if "mem_inst_retired.all_loads" in l:
      cols = l.split()
      loads = float(cols[0].replace(",", ""))
    if "mem_load_retired.l1_hit" in l:
      cols = l.split()
      l1hits = float(cols[0].replace(",", ""))
    if "mem_load_retired.l1_miss" in l:
      cols = l.split()
      l1misses = float(cols[0].replace(",", ""))
    if "mem_load_retired.l2_hit" in l:
      cols = l.split()
      l2hits = float(cols[0].replace(",", ""))
    if "mem_load_retired.l2_miss" in l:
      cols = l.split()
      l2misses = float(cols[0].replace(",", ""))
    if "mem_load_retired.l3_hit" in l:
      cols = l.split()
      l3hits = float(cols[0].replace(",", ""))
    if "mem_load_retired.l3_miss" in l:
      cols = l.split()
      l3misses = float(cols[0].replace(",", ""))
    if len(l)==0:
      break
  f.close

  l3missrate = l3misses / (l3misses + l3hits) * 100 if l3misses + l3hits > 0 else 0
  l2missrate = l2misses / (l2misses + l2hits) * 100 if l2misses + l2hits > 0 else 0
  # Sometimes CPUs coalesce two load instructions to the same cache line as
  # one load access.  So dividing by the total load instructions is more accurate.
  l1missrate = l1misses / loads * 100 if loads > 0 else 0
  # print ("target = %s, size = %s, l3misses = %f, l2misses = %f, l1misses=%f " % (target, size, l3misses, l2misses, l1misses))
  return

def main():
  try:
    opts, args = getopt.getopt(sys.argv[1:],"hi:o:t:e:",["inputDir=","outputFile=","targetName=","extension="])
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
    elif opt in ("-e", "--extension"):
      extension = arg

  # Get all files for target
  files = [f for f in os.listdir(reportRoot) if f"{target}" in f and f"{extension}" in f.split(".")[-1]]
  # Get all sizes executed for target
  files.sort(key=lambda k: int(k.split('.')[2], 0) )
  sizes = [ f.split('.')[2] for f in files ]

  f = open(fileName,"w")
  f.write("%10s " % "#Elements")
  f.write("%10s %10s %10s" % ("L1MissRate", "L2MissRate", "L3MissRate"))
  f.write('\n')
  for size in sizes:
    f.write("%10s " % size)
    getPercentage(reportRoot, target, size, extension)
    f.write("%10.2f %10.2f %10.2f" % (l1missrate, l2missrate, l3missrate))
    f.write('\n')
  f.close

if __name__ == "__main__":
  main()
