 #!/bin/sh

# script to run the whole analysis

VER=v11f
FILEVER=V11h
LUMI=19494
RUN=both     # use either blind or both
LEP=central  # turn on lepton ID correction

IDIR=/media/disk-1/chayanit/NtupleV11h/Central/
ODIR=/afs/cern.ch/work/c/chayanit/private/VBFInvHiggs_PATv11NoNu/CMSSW_5_3_7_patch5/src/InvisibleHiggs/Analysis/output/Central/

echo "Deleting and recreating $ODIR"
if [ -d "$ODIR" ]; then
    rm -r $ODIR
fi
mkdir $ODIR

DATASETS=$CMSSW_BASE/src/InvisibleHiggs/Analysis/data/datasets_$VER.txt
DATASETS_scaled=$CMSSW_BASE/src/InvisibleHiggs/Analysis/data/datasets_$VER_scaled.txt
DATASETS_W=$CMSSW_BASE/src/InvisibleHiggs/Analysis/data/datasets_W_$VER.txt
DATASETS_WTau=$CMSSW_BASE/src/InvisibleHiggs/Analysis/data/datasets_WTau_$VER.txt
DATASETS_Z=$CMSSW_BASE/src/InvisibleHiggs/Analysis/data/datasets_Z_$VER.txt
DATASETS_QCD=$CMSSW_BASE/src/InvisibleHiggs/Analysis/data/datasets_QCD_$VER.txt


# MC numbers & plots
echo "efficiency -i $IDIR -o $ODIR -f $DATASETS -l $LUMI -c $LEP"
efficiency -i $IDIR -o $ODIR -f $DATASETS -l $LUMI -c $LEP > $ODIR/efficiency.log

echo "nMinusOne -i $IDIR -o $ODIR -f $DATASETS -l $LUMI -c $LEP"
nMinusOne -i $IDIR -o $ODIR -f $DATASETS -l $LUMI -c $LEP > $ODIR/nMinusOne.log
echo ""

# control plots
echo "controlPlots -i $IDIR -o $ODIR -f $DATASETS_scaled -l $LUMI"
controlPlots -i $IDIR -o $ODIR -f $DATASETS_scaled -l $LUMI > $ODIR/controlPlots.log
echo ""

# BG estimates
echo "wBackground -i $IDIR -o $ODIR -f $DATASETS_W -l $LUMI -c $LEP"
wBackground -i $IDIR -o $ODIR -f $DATASETS_W -l $LUMI -c $LEP > $ODIR/wBackground.log
echo ""

echo "wTauBackground -i $IDIR -o $ODIR -f $DATASETS_WTau -l $LUMI -c $LEP"
wTauBackground -i $IDIR -o $ODIR -f $DATASETS_WTau -l $LUMI -c $LEP > $ODIR/wTauBackground.log
echo ""

echo "zBackground -i $IDIR -o $ODIR -f $DATASETS_Z -l $LUMI -c $LEP"
zBackground -i $IDIR -o $ODIR -f $DATASETS_Z -l $LUMI -c $LEP > $ODIR/zBackground.log
echo ""
 
echo "qcdBackground3 -i $IDIR -o $ODIR -f $DATASETS_QCD -l $LUMI -c $LEP"
qcdBackground3 -i $IDIR -o $ODIR -f $DATASETS_QCD -l $LUMI -c $LEP > $ODIR/qcdBackground3.log
echo ""
 
echo "search -i $IDIR -o $ODIR -f $DATASETS -l $LUMI"
search -i $IDIR -o $ODIR -f $DATASETS -l $LUMI > $ODIR/search.log
echo ""

echo "summary -o $ODIR $LUMI"
summary -q 3 -o $ODIR -l $LUMI > $ODIR/summary.log
echo ""

## combine tool limits
./runLimits.sh $ODIR $RUN

# make plots
echo "limitPlots -o $ODIR $LUMI"
limitPlots -o $ODIR -l $LUMI > $ODIR/limits.log
echo ""

# tar everything up
echo "tar -zcvf $ODIR.tgz $ODIR/"
tar -zcvf $ODIR.tgz $ODIR/
