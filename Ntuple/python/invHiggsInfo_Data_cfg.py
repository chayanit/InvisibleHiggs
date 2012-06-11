import FWCore.ParameterSet.Config as cms

from InvisibleHiggs.Ntuple.invHiggsInfo_Master_cfg import *



### THINGS TO EDIT BELOW ###

# change Global Tag
process.GlobalTag.globaltag = 'GR_R_52_V9::All'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

# input files
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
    'file:patTuple.root'
    ] );

