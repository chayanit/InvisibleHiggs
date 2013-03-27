#!/bin/sh

IDIR=/storage/phjjb/invisibleHiggs/InvHiggsInfo_v9
ODIR=InvHiggsInfo_v9
DATASETS=InvisibleHiggs/Analysis/data/datasets_v9.txt
DATASETS_W=InvisibleHiggs/Analysis/data/datasets_W_v9.txt
DATASETS_Z=InvisibleHiggs/Analysis/data/datasets_Z_v9.txt
DATASETS_QCD=InvisibleHiggs/Analysis/data/datasets_QCD_v9.txt
LUMI=19576

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

# set limits
echo "limits -o $ODIR $LUMI"
limits -o $ODIR -l $LUMI

# tar everything up
echo "tar -zcvf $ODIR.tgz $ODIR/"
tar -zcvf $ODIR.tgz $ODIR/
