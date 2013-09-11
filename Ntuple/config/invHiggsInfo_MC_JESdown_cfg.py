import FWCore.ParameterSet.Config as cms

from InvisibleHiggs.Ntuple.invHiggsInfo_Master_cfg import *

# Jet/MET uncertainty
process.invHiggsInfo.jetTag      = cms.untracked.InputTag("shiftedGoodPatJetsEnDownForCorrMEt")
process.invHiggsInfo.metTag      = cms.untracked.InputTag("patType1CorrectedPFMetJetEnDown")
process.invHiggsInfo.puJetMvaTag = cms.untracked.InputTag("puJetMvaEnDown", "fullDiscriminant")
process.invHiggsInfo.puJetIdTag  = cms.untracked.InputTag("puJetMvaEnDown", "fullId")
 
# PU re-weighting
process.invHiggsInfo.puMCFile    = cms.untracked.string("PUHistS10.root")
process.invHiggsInfo.puDataFile  = cms.untracked.string("PUHistRun2012All_forV9.root")
process.invHiggsInfo.puMCHist    = cms.untracked.string("pileup")
process.invHiggsInfo.puDataHist  = cms.untracked.string("pileup")
process.invHiggsInfo.mcPYTHIA    = cms.untracked.bool(True)

process.invHiggsInfo.trigCorrFile   = cms.untracked.string("DataMCWeight_53X_v1.root")

process.TFileService.fileName = cms.string('invHiggsInfo_MC.root')


### THINGS TO EDIT BELOW ###

# change Global Tag
process.GlobalTag.globaltag = 'START53_V7G::All'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

# input files
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
        'file:patTuple.root'
    ] );

