#!/bin/sh

# script to run limit setting

ODIR=$1
RUN=$2

echo "combine -M Asymptotic --run=$RUN $ODIR/card*.txt"

cd $ODIR

combine -M Asymptotic --run=$RUN -m 110 card110.txt
combine -M Asymptotic --run=$RUN -m 125 card125.txt
combine -M Asymptotic --run=$RUN -m 150 card150.txt
combine -M Asymptotic --run=$RUN -m 200 card200.txt
combine -M Asymptotic --run=$RUN -m 300 card300.txt
combine -M Asymptotic --run=$RUN -m 400 card400.txt
echo

hadd -f combineMerge.root higgsCombineTest.Asymptotic.mH*.root

cd ..

echo "Done"