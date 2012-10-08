#!/bin/sh

# this script merges all the EDM files in a folder (first argument)
# and stores output in a specified location (2nd argument)

idir=$1
ofile=$2

ls $idir/*.root > files.txt
sed -i.bak 's/^/file:/' files.txt
edmCopyPickMerge inputFiles_load=files.txt outputFile=$ofile
