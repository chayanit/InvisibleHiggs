import FWCore.ParameterSet.Config as cms

process = cms.Process("TTREE")

process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.threshold = ''
process.MessageLogger.cerr.FwkReport.reportEvery = 1000


# geometry & magnetic field
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")

# conditions
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')

# HLT bit filter
process.load('HLTrigger.HLTfilters.hltHighLevel_cfi')
process.hltHighLevel.TriggerResultsTag = cms.InputTag("TriggerResults","","HLT")
process.hltHighLevel.throw = cms.bool(False)
process.hltHighLevel.HLTPaths = cms.vstring(
    "HLT_DiPFJet40_PFMETnoMu65_MJJ600VBF_LeadingJets_v*",
    "HLT_DiPFJet40_PFMETnoMu65_MJJ800VBF_AllJets_v*"
)

# Ntuple producer
process.load('InvisibleHiggs/Ntuple/invHiggsTree_cfi')

# get jet corrections
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
process.invHiggsTree.jetCorrectorServiceName = cms.untracked.string("ak5CaloL2L3")
#process.ak5CaloL1Offset.useCondDB = False

# path
process.ttree = cms.Path(

# filter on HLT bit
    process.hltHighLevel

# generate TTree    
    +process.invHiggsTree

)


# TTree output file
process.TFileService = cms.Service("TFileService",
    fileName = cms.string('invHiggsTree.root')
)



### THINGS TO EDIT BELOW ###

# change Global Tag
process.GlobalTag.globaltag = 'GR_P_V32::All'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

# input files
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
    'file:/gpfs_phys/storm/cms/data/Run2011B/ZeroBiasHPF0/RECO/PromptReco-v1/000/178/104/AEC3A79D-4CF6-E011-B36A-001D09F242EF.root'
    ] );

