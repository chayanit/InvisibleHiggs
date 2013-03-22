#!/bin/sh

IDIR=/storage/phjjb/invisibleHiggs/InvHiggsInfo_v9
ODIR=InvHiggsInfo_v9
DATASETS=InvisibleHiggs/Analysis/data/datasets_v9.txt
DATASETS_W=InvisibleHiggs/Analysis/data/datasets_W_v9.txt
LUMI=19560

echo "controlPlots -i $IDIR -o $ODIR -f $DATASETS -l $LUMI"
controlPlots -i $IDIR -o $ODIR -f $DATASETS -l $LUMI

echo "efficiency -i $IDIR -o $ODIR -f $DATASETS -l $LUMI"
efficiency -i $IDIR -o $ODIR -f $DATASETS -l $LUMI

echo "nMinusOne -i $IDIR -o $ODIR -f $DATASETS -l $LUMI"
nMinusOne -i $IDIR -o $ODIR -f $DATASETS -l $LUMI

echo "wBackground -i $IDIR -o $ODIR -f $DATASETS_W -l $LUMI"
wBackground -i $IDIR -o $ODIR -f $DATASETS_W -l $LUMI

echo "tar -zcvf $ODIR.tgz $ODIR/"
tar -zcvf $ODIR.tgz $ODIR/
