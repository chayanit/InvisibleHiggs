import FWCore.ParameterSet.Config as cms

process = cms.Process("TREE")

process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.threshold = ''
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
#process.MessageLogger.cerr.INFO.limit = cms.untracked.int32(100)
#process.MessageLogger.suppressInfo = cms.untracked.vstring("SiStripDetInfoFileReader:  (NoModuleName)")

## # geometry & magnetic field
## process.load('Configuration/StandardSequences/GeometryIdeal_cff')
## process.load("Configuration.StandardSequences.MagneticField_38T_cff")
## process.load("Configuration.StandardSequences.Reconstruction_cff")

# conditions
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')

# Physics Object Candidates
process.load('InvisibleHiggs/Ntuple/PhysicsObjectCandidates_cff')

# Z and W candidates
process.load('InvisibleHiggs/Ntuple/WCandidates_cff')
process.load('InvisibleHiggs/Ntuple/ZCandidates_cff')

# TTree output file
process.load("CommonTools.UtilAlgos.TFileService_cfi")
process.TFileService.fileName = cms.string('invHiggsInfo.root')

# Fix PUJetID
#process.load("CMGTools.External.pujetidsequence_cff")
#process.puJetMva.jets = cms.InputTag("smearedGoodPatJetsResUp")
#process.puJetId.jets  = cms.InputTag("smearedGoodPatJetsResUp")

# Ntuple producer
process.load('InvisibleHiggs/Ntuple/invHiggsInfo_cfi')
#process.invHiggsInfo.puJetMvaTag = cms.untracked.InputTag("puJetMva", "fullDiscriminant","TREE")
#process.invHiggsInfo.puJetIdTag  = cms.untracked.InputTag("puJetMva", "fullId","TREE")

# path
process.p0   = cms.Path(#process.puJetIdSqeuence *
                        process.PhysicsObjectSequence
                        * process.WSequence
                        * process.ZSequence
                        )
process.path = cms.EndPath(process.invHiggsInfo)


### THINGS TO EDIT BELOW ###

# change Global Tag
process.GlobalTag.globaltag = 'GR_P_V42_AN3::All'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

# input files
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
    'file:/localhome/srimanob/SUSY/CMSSW/MC-Production/slc5_amd64_gcc462/Higgs/invisibleHiggs/PUJetID/case-1/CMSSW_5_3_7_patch4/src/InvisibleHiggs/Ntuple/test/test/patTuple.root'
    ] );
