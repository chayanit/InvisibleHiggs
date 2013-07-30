#!/bin/sh

VER=v11
FILEVER=V11d
LUMI=19576

IDIR=/storage/phjjb/invisibleHiggs/Ntuple$FILEVER/Central
ODIR=Analysis$FILEVER

echo "Deleting and recreating $ODIR"
if [ -d "$ODIR" ]; then
    rm -r $ODIR
fi
mkdir $ODIR

DATASETS=$CMSSW_BASE/src/InvisibleHiggs/Analysis/data/datasets_$VER.txt
DATASETS_W=$CMSSW_BASE/src/InvisibleHiggs/Analysis/data/datasets_W_$VER.txt
DATASETS_Z=$CMSSW_BASE/src/InvisibleHiggs/Analysis/data/datasets_Z_$VER.txt
DATASETS_QCD=$CMSSW_BASE/src/InvisibleHiggs/Analysis/data/datasets_QCD_$VER.txt


# create lepton weight TTree
echo "writeLeptonWeights -i $IDIR -f $DATASETS"
#writeLeptonWeights -i $IDIR -f $DATASETS > $ODIR/leptonWeights.log

# MC numbers & plots
echo "efficiency -i $IDIR -o $ODIR -f $DATASETS -l $LUMI"
efficiency -i $IDIR -o $ODIR -f $DATASETS -l $LUMI > $ODIR/efficiency.log

echo "nMinusOne -i $IDIR -o $ODIR -f $DATASETS -l $LUMI"
nMinusOne -i $IDIR -o $ODIR -f $DATASETS -l $LUMI > $ODIR/nMinusOne.log
echo ""

# MC vs data
echo "controlPlots -i $IDIR -o $ODIR -f $DATASETS -l $LUMI"
controlPlots -i $IDIR -o $ODIR -f $DATASETS -l $LUMI > $ODIR/controlPlots.log
echo ""

# BG estimates
echo "wBackground -i $IDIR -o $ODIR -f $DATASETS_W -l $LUMI"
wBackground -i $IDIR -o $ODIR -f $DATASETS_W -l $LUMI > $ODIR/wBackground.log
echo ""

echo "wTauBackground -i $IDIR -o $ODIR -f $DATASETS_W -l $LUMI"
wBackground -i $IDIR -o $ODIR -f $DATASETS_W -l $LUMI > $ODIR/wTauBackground.log
echo ""

echo "zBackground -i $IDIR -o $ODIR -f $DATASETS_Z -l $LUMI"
zBackground -i $IDIR -o $ODIR -f $DATASETS_Z -l $LUMI > $ODIR/zBackground.log
echo ""

echo "qcdBackground -i $IDIR -o $ODIR -f $DATASETS_QCD -l $LUMI"
#qcdBackground -i $IDIR -o $ODIR -f $DATASETS_QCD -l $LUMI > $ODIR/qcdBackground.log
echo ""

echo "qcdBackground2 -i $IDIR -o $ODIR -f $DATASETS_QCD -l $LUMI"
#qcdBackground2 -i $IDIR -o $ODIR -f $DATASETS_QCD -l $LUMI > $ODIR/qcdBackground2.log
echo ""

echo "qcdBackground3 -i $IDIR -o $ODIR -f $DATASETS_QCD -l $LUMI"
qcdBackground3 -i $IDIR -o $ODIR -f $DATASETS_QCD -l $LUMI > $ODIR/qcdBackground3.log
echo ""

echo "summary -o $ODIR $LUMI"
summary -q 3 -o $ODIR -l $LUMI > $ODIR/summary.log
echo ""

# combine tool limits
echo "combine -M Asymptotic $ODIR/card.txt"
cd $ODIR
combine -M Asymptotic --run=blind -m 110 card110.txt
combine -M Asymptotic --run=blind -m 125 card125.txt
combine -M Asymptotic --run=blind -m 150 card150.txt
combine -M Asymptotic --run=blind -m 200 card200.txt
combine -M Asymptotic --run=blind -m 300 card300.txt
combine -M Asymptotic --run=blind -m 400 card400.txt
hadd -f combineMerge.root higgsCombineTest.Asymptotic.mH*.root
cd ..

# make plots
echo "limitPlots -o $ODIR $LUMI"
limitPlots -o $ODIR -l $LUMI > $ODIR/limits.log
echo ""

# tar everything up
echo "tar -zcvf $ODIR.tgz $ODIR/"
tar -zcvf $ODIR.tgz $ODIR/
