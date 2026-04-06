#!/usr/bin/python

import sys
import os
import getopt
import glob

def getIPC(reportRoot, target, size):
  fileList = glob.glob(reportRoot + "/" + target + "." + size + ".ipc")
  assert len(fileList) == 1
  fileName = fileList[0]

  f = open(fileName,"r")
  if f==0:
    return 0

  rate = 0
  while True:
    l = f.readline()
    if "insn per cycle" in l:
      cols = l.split('#')
      cols = cols[1].split()
      ipc = float(cols[0])
      break
    if len(l)==0:
      ipc = 0
      break
  f.close

  return ipc

def main():
  try:
    opts, args = getopt.getopt(sys.argv[1:],"hi:o:",["inputDir=","outputFile="])
  except getopt.GetoptError:
    print ('generate_plot.py -i <input directory> -o <output file>')
    sys.exit(1)
  for opt, arg in opts:
    if opt == '-h':
       print ('generate_plot.py -i <input directory> -o <output file>')
       sys.exit()
    elif opt in ("-i", "--inputDir"):
       reportRoot = arg
    elif opt in ("-o", "--outputFile"):
       fileName = arg

  targets = ["array.bin", "array_nodata.bin", "array_optimized.bin",
             "linked-list.bin", "linked-list_nodata.bin", "linked-list_optimized.bin"]

  # Get all files for array.bin
  files = [f for f in os.listdir(reportRoot) if f"array.bin" in f and "ipc" in f.split(".")[-1]]
  # Get all sizes executed for array.bin
  files.sort(key=lambda k: int(k.split('.')[2], 0) )
  sizes = [ f.split('.')[2] for f in files ]

  f = open(fileName,"w")
  f.write("%10s " % "#Elements")
  for target in targets:
    f.write("%10s " % target)
  f.write('\n')
  for size in sizes:
    f.write("%10s " % size)
    for target in targets:
      ipc = getIPC(reportRoot, target, size)
      f.write("%10.2f " % ipc)
    f.write('\n')
  f.close

if __name__ == "__main__":
  main()
