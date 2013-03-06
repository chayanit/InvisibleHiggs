import FWCore.ParameterSet.Config as cms

from InvisibleHiggs.Ntuple.invHiggsInfo_Master_cfg import *

process.invHiggsInfo.mcPYTHIA   = cms.untracked.bool(False)

process.TFileService.fileName = cms.string('invHiggsInfo_Data.root')

### THINGS TO EDIT BELOW ###

# change Global Tag
process.GlobalTag.globaltag = 'GR_R_53_V14::All'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

# input files
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
    'file:/uscms_data/d3/chayanit/VplusJetsAnalysis2012/CMSSW_5_2_5/src/InvisibleHiggs/Ntuple/python/testroot/patTuple_DoubleMuNoTrig.root'
    ] );

