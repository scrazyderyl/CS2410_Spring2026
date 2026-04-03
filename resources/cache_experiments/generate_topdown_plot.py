#!/usr/bin/python

import sys
import os
import getopt
import glob

def getTopDown(reportRoot, target, size, index):
  fileList = glob.glob(reportRoot + "/" + target + "." + size + ".topdown")
  assert len(fileList) == 1
  fileName = fileList[0]

  f = open(fileName,"r")
  if f==0:
    return 0

  ret = 0
  while True:
    l = f.readline()
    if "tma_frontend_bound" in l:
      l = f.readline()  # Get the next line
      cols = l.split()
      ret = float(cols[index])
      break
    if len(l)==0:
      break
  f.close

  return ret 

def main():
  try:
    opts, args = getopt.getopt(sys.argv[1:],"ht:i:o:",["target=","inputDir=","outputFile="])
  except getopt.GetoptError:
    print ('generate_plot.py -t <target> -i <input directory> -o <output file>')
    sys.exit(1)
  for opt, arg in opts:
    if opt == '-h':
       print ('generate_plot.py -i <input directory> -o <output file>')
       sys.exit()
    elif opt in ("-t", "--target"):
       target = arg
    elif opt in ("-i", "--inputDir"):
       reportRoot = arg
    elif opt in ("-o", "--outputFile"):
       fileName = arg

  # Get all files for target
  files = [f for f in os.listdir(reportRoot) if f"{target}" in f and "topdown" in f.split(".")[-1]]
  # Get all sizes executed for target
  files.sort(key=lambda k: int(k.split('.')[2], 0) )
  sizes = [ f.split('.')[2] for f in files ]

  f = open(fileName,"w")
  f.write("%10s " % "#Elements")
  f.write("%20s " % "frontend_bound")
  f.write("%20s " % "retiring")
  f.write("%20s " % "backend_bound")
  f.write("%20s " % "bad_speculation")
  f.write('\n')
  for size in sizes:
    f.write("%10s " % size)
    for i in range(1, 5):
      ret = getTopDown(reportRoot, target, size, i)
      f.write("%20.2f " % ret)
    f.write('\n')
  f.close

if __name__ == "__main__":
  main()
