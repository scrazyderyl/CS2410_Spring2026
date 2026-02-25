#!/usr/bin/python

import sys
import getopt
import glob

def getIPC(reportRoot, target, size):
  fileList = glob.glob(reportRoot + "/" + target + "." + size + ".stat")
  assert len(fileList) == 1
  fileName = fileList[0]

  f = open(fileName,"r")
  if f==0:
    return 0

  ipc = 0
  while True:
    l = f.readline()
    if "insn per cycle" in l:
      cols = l.split('#')
      cols = cols[1].split()
      ipc = float(cols[0])
      break
    if len(l)==0:
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

  targets = ["nosort", "sort"]
  sizes = ["10", "100", "1000", "2000", "3000", "4000", "5000", "6000", "7000", "8000", "9000", "10000", "20000", "30000", "40000", "50000", "60000", "70000", "80000", "90000", "100000"]

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
