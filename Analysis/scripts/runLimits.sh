#!/bin/sh

# script to run limit setting

ODIR=$1
RUN=$2

echo "combine -M Asymptotic --run=$RUN $ODIR/vbfhinv_*_8TeV.txt"

cd $ODIR

combine -M Asymptotic --run=$RUN -m 110 vbfhinv_110_8TeV.txt
combine -M Asymptotic --run=$RUN -m 125 vbfhinv_125_8TeV.txt
combine -M Asymptotic --run=$RUN -m 150 vbfhinv_150_8TeV.txt
combine -M Asymptotic --run=$RUN -m 200 vbfhinv_200_8TeV.txt
combine -M Asymptotic --run=$RUN -m 300 vbfhinv_300_8TeV.txt
combine -M Asymptotic --run=$RUN -m 400 vbfhinv_400_8TeV.txt
echo

hadd -f combineMerge.root higgsCombineTest.Asymptotic.mH*.root

cd ..

echo "Done"
