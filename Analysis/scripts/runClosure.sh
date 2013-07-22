#!/bin/sh

VER=v11
FILEVER=v11d
LUMI=19576

IDIR_Central=/afs/cern.ch/work/c/chayanit/public/forInvHiggs/NtupleV11d/Central/
ODIR_Central=/afs/cern.ch/work/c/chayanit/private/VBFInvHiggs_PATv11/CMSSW_5_3_7_patch5/src/InvisibleHiggs/Analysis/output/WithCJV/ClosureEWK
 
echo "Deleting and recreating $ODIR_Central"
if [ -d "$ODIR_Central" ]; then
    rm -r $ODIR_Central
fi
mkdir $ODIR_Central

DATASETS=$CMSSW_BASE/src/InvisibleHiggs/Analysis/data/datasets_$VER.txt
DATASETS_W=$CMSSW_BASE/src/InvisibleHiggs/Analysis/data/datasets_W_$VER.txt
DATASETS_Z=$CMSSW_BASE/src/InvisibleHiggs/Analysis/data/datasets_Z_$VER.txt


echo "zBackground_A -i $IDIR_Central -o $ODIR_Central -f $DATASETS_Z -l $LUMI"
zBackground_A -i $IDIR_Central -o $ODIR_Central -f $DATASETS_Z -l $LUMI > $ODIR_Central/zBackground_A.log
echo ""

echo "zBackground_B -i $IDIR_Central -o $ODIR_Central -f $DATASETS_W -l $LUMI"
zBackground_B -i $IDIR_Central -o $ODIR_Central -f $DATASETS_W -l $LUMI > $ODIR_Central/zBackground_B.log
echo ""

echo "zBackground_C -i $IDIR_Central -o $ODIR_Central -f $DATASETS_W -l $LUMI"
zBackground_C -i $IDIR_Central -o $ODIR_Central -f $DATASETS_W -l $LUMI > $ODIR_Central/zBackground_C.log
echo ""

echo "wBackground_A -i $IDIR_Central -o $ODIR_Central -f $DATASETS_W -l $LUMI"
wBackground_A -i $IDIR_Central -o $ODIR_Central -f $DATASETS_W -l $LUMI > $ODIR_Central/wBackground_A.log
echo ""

