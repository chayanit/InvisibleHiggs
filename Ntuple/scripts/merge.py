#!/usr/bin/env python

import sys
import os
import string
import re
from subprocess import *

from optparse import OptionParser

# get options
parser = OptionParser()
parser.add_option("-i","--indir",
                  action="store",
                  default=os.getcwd(),
                  dest="indir",
                  help="Input directory")
parser.add_option("-o","--outdir",
                  action="store",
                  default=os.getcwd(),
                  dest="outdir",
                  help="Output directory")
parser.add_option("-d","--outfile",
                  action="store",
                  default="",
                  dest="dataset",
                  help="Dataset to merge")

(options,args)=parser.parse_args()

# last unclaimed argument is used as input file
for arg in args:
    options.dataset=arg

# create the command with output file
command="hadd -f "+options.outdir+"/"+options.dataset+".root "

# get the input files
files=os.listdir(options.indir+"/"+options.dataset)

#print files

for file in files:
    if (re.match(".+root\Z", file)):
        command+=options.indir+"/"+options.dataset+"/"+file+" "

print command

cp = Popen(command, shell=True)  # execute command
cp.wait()  # is this necessary?
