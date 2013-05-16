#!/bin/sh

VER=v11
LUMI=19576

IDIR=/storage/phjjb/invisibleHiggs/InvHiggsInfo_$VER/Central
ODIR=InvHiggsInfo_$VER

echo "Deleting and recreating $ODIR"
if [ -d "$ODIR" ]; then
    rm -r $ODIR
fi
mkdir $ODIR

DATASETS=$CMSSW_BASE/src/InvisibleHiggs/Analysis/data/datasets_$VER.txt
DATASETS_W=$CMSSW_BASE/src/InvisibleHiggs/Analysis/data/datasets_W_$VER.txt
DATASETS_Z=$CMSSW_BASE/src/InvisibleHiggs/Analysis/data/datasets_Z_$VER.txt
DATASETS_QCD=$CMSSW_BASE/src/InvisibleHiggs/Analysis/data/datasets_QCD_$VER.txt

# MC numbers & plots
echo "efficiency -i $IDIR -o $ODIR -f $DATASETS -l $LUMI"
efficiency -i $IDIR -o $ODIR -f $DATASETS -l $LUMI > $ODIR/efficiency.log

echo "nMinusOne -i $IDIR -o $ODIR -f $DATASETS -l $LUMI"
echo "Not working yet!"
echo ""
nMinusOne -i $IDIR -o $ODIR -f $DATASETS -l $LUMI > $ODIR/nMinusOne.log

# MC vs data
echo "controlPlots -i $IDIR -o $ODIR -f $DATASETS -l $LUMI"
echo "Not working yet !"
echo ""
#controlPlots -i $IDIR -o $ODIR -f $DATASETS -l $LUMI > $ODIR/controlPlots.log

# BG estimates
echo "wBackground -i $IDIR -o $ODIR -f $DATASETS_W -l $LUMI"
wBackground -i $IDIR -o $ODIR -f $DATASETS_W -l $LUMI > $ODIR/wBackground.log

echo "zBackground -i $IDIR -o $ODIR -f $DATASETS_Z -l $LUMI"
zBackground -i $IDIR -o $ODIR -f $DATASETS_Z -l $LUMI > $ODIR/zBackground.log

echo "qcdBackground -i $IDIR -o $ODIR -f $DATASETS_QCD -l $LUMI"
qcdBackground -i $IDIR -o $ODIR -f $DATASETS_QCD -l $LUMI > $ODIR/qcdBackground.log

echo "summary -o $ODIR $LUMI"
summary -o $ODIR -l $LUMI > $ODIR/summary.log

# set limits
echo "limits -o $ODIR $LUMI"
limits -o $ODIR -l $LUMI > $ODIR/limits.log

# tar everything up
echo "tar -zcvf $ODIR.tgz $ODIR/"
tar -zcvf $ODIR.tgz $ODIR/
