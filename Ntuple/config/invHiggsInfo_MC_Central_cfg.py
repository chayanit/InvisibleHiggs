import FWCore.ParameterSet.Config as cms

from InvisibleHiggs.Ntuple.invHiggsInfo_Master_cfg import *

# Jet/MET uncertainty
process.invHiggsInfo.jetTag      = cms.untracked.InputTag("smearedGoodPatJetsWithGaussian")
process.invHiggsInfo.metTag      = cms.untracked.InputTag("patType1CorrectedPFMetWithGaussian")
process.invHiggsInfo.puJetMvaTag = cms.untracked.InputTag("puJetMvaSmeared", "fullDiscriminant")
process.invHiggsInfo.puJetIdTag  = cms.untracked.InputTag("puJetMvaSmeared", "fullId")
 
# PU re-weighting
process.invHiggsInfo.puMCFile    = cms.untracked.string("PUHistS10.root")
process.invHiggsInfo.puDataFile  = cms.untracked.string("PUHistRun2012All_forV9.root")
process.invHiggsInfo.puMCHist    = cms.untracked.string("pileup")
process.invHiggsInfo.puDataHist  = cms.untracked.string("pileup")
process.invHiggsInfo.mcPYTHIA    = cms.untracked.bool(True)

process.invHiggsInfo.trigCorrFile   = cms.untracked.string("DataMCWeight_53X_v1.root")

process.invHiggsInfo.leptCorrFile   = cms.untracked.string("../data/leptonWeights.root")

process.TFileService.fileName = cms.string('invHiggsInfo_MC.root')


### THINGS TO EDIT BELOW ###

# change Global Tag
process.GlobalTag.globaltag = 'START53_V7G::All'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

# input files
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
        # 'file:patTuple.root'
        'file:./WJets/patTuple_50_1_j4e.root',
        'file:./WJets/patTuple_51_1_kpE.root',
        'file:./WJets/patTuple_52_1_svD.root',
        'file:./WJets/patTuple_53_1_hfc.root',
        'file:./WJets/patTuple_54_1_mT3.root',
        'file:./WJets/patTuple_55_1_HTT.root',
        'file:./WJets/patTuple_56_1_xYR.root',
        'file:./WJets/patTuple_57_1_h61.root',
        'file:./WJets/patTuple_58_1_1tn.root',
        'file:./WJets/patTuple_59_1_iYE.root',
        'file:./WJets/patTuple_5_1_67b.root'
    ] );

