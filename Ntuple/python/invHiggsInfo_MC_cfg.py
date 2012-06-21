import FWCore.ParameterSet.Config as cms

from InvisibleHiggs.Ntuple.invHiggsInfo_Master_cfg import *



### THINGS TO EDIT BELOW ###

# change Global Tag
process.GlobalTag.globaltag = 'START52_V9::All'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

# input files
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
    'file:patTuple_MC.root'
#    'file:/gpfs_phys/storm/cms/user/jbrooke/ZJetsToNuNu_50_HT_100_TuneZ2Star_8TeV_madgraph/InvHiggsPAT_Summer12_v2_ZJetsToNuNu_50_HT_100/861ac8814e4c5247588bc756da882dd0/patTuple_12_1_ymh.root'
    ] );

