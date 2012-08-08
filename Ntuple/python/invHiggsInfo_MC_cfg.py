import FWCore.ParameterSet.Config as cms

from InvisibleHiggs.Ntuple.invHiggsInfo_Master_cfg import *

# PU re-weighting
process.invHiggsInfo.puMCFile = cms.untracked.string("PUHistS7.root")
process.invHiggsInfo.puDataFile = cms.untracked.string("PUHistRun2012A.root")
process.invHiggsInfo.puMCHist = cms.untracked.string("pileup")
process.invHiggsInfo.puDataHist = cms.untracked.string("pileup")


### THINGS TO EDIT BELOW ###

# change Global Tag
process.GlobalTag.globaltag = 'START52_V9::All'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

# input files
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
#    'file:patTuple_MC.root'
    'file:/gpfs_phys/storm/cms/user/jbrooke/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/InvHiggsPAT_Summer12_v5_WJetsToLNu/dc0b67fb76a75169bf58598357984118/patTuple_820_2_9fC.root'
    ] );

