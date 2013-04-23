#!/bin/sh

VER=v10
LUMI=19576

IDIR=/storage/phjjb/invisibleHiggs/InvHiggsInfo_$VER
ODIR=InvHiggsInfo_$VER

DATASETS=$CMSSW_BASE/src/InvisibleHiggs/Analysis/data/datasets_$VER.txt
DATASETS_W=$CMSSW_BASE/src/InvisibleHiggs/Analysis/data/datasets_W_$VER.txt
DATASETS_Z=$CMSSW_BASE/src/InvisibleHiggs/Analysis/data/datasets_Z_$VER.txt
DATASETS_QCD=$CMSSW_BASE/src/InvisibleHiggs/Analysis/data/datasets_QCD_$VER.txt

# MC numbers & plots
echo "efficiency -i $IDIR -o $ODIR -f $DATASETS -l $LUMI"
efficiency -i $IDIR -o $ODIR -f $DATASETS -l $LUMI

echo "nMinusOne -i $IDIR -o $ODIR -f $DATASETS -l $LUMI"
nMinusOne -i $IDIR -o $ODIR -f $DATASETS -l $LUMI

# MC vs data
echo "controlPlots -i $IDIR -o $ODIR -f $DATASETS -l $LUMI"
controlPlots -i $IDIR -o $ODIR -f $DATASETS -l $LUMI

# BG estimates
echo "wBackground -i $IDIR -o $ODIR -f $DATASETS_W -l $LUMI"
wBackground -i $IDIR -o $ODIR -f $DATASETS_W -l $LUMI

echo "zBackground -i $IDIR -o $ODIR -f $DATASETS_Z -l $LUMI"
zBackground -i $IDIR -o $ODIR -f $DATASETS_Z -l $LUMI

echo "qcdBackground -i $IDIR -o $ODIR -f $DATASETS_QCD -l $LUMI"
qcdBackground -i $IDIR -o $ODIR -f $DATASETS_QCD -l $LUMI

echo "summary -o $ODIR $LUMI"
summary -o $ODIR -l $LUMI

# set limits
echo "limits -o $ODIR $LUMI"
limits -o $ODIR -l $LUMI

# tar everything up
echo "tar -zcvf $ODIR.tgz $ODIR/"
tar -zcvf $ODIR.tgz $ODIR/
