#!/bin/sh

idir=$1
ofile=$2

ls $idir/*.root > files.txt
sed -i.bak 's/^/file:/' files.txt
edmCopyPickMerge inputFiles_load=files.txt outputFile=$ofile
