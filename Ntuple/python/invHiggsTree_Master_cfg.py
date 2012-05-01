import FWCore.ParameterSet.Config as cms

process = cms.Process("TREE")

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

# get jet corrections (inc re-running L1Fast jet)
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
process.load('RecoJets.Configuration.RecoPFJets_cff')
process.kt6PFJets.doRhoFastjet = True
process.ak5PFJetsL1FastL2L3   = cms.EDProducer('PFJetCorrectionProducer',
    src         = cms.InputTag('ak5PFJets'),
    correctors  = cms.vstring('ak5PFL1FastL2L3')
)
process.ak5CaloJetsL1FastL2L3   = cms.EDProducer('CaloJetCorrectionProducer',
    src         = cms.InputTag('ak5CaloJets'),
    correctors  = cms.vstring('ak5CaloL1FastL2L3')
)
process.ak5CaloJets.doAreaFastjet = True
process.ak5PFJets.doAreaFastjet = True


# path
process.tree = cms.Path(

# filter on HLT bit
    process.hltHighLevel

# jet corrections
    +process.kt6PFJets
    +process.ak5PFJets
    +process.ak5PFJetsL1FastL2L3
    +process.ak5CaloJets
    +process.ak5CaloJetsL1FastL2L3

# generate TTree    
    +process.invHiggsTree

)


# TTree output file
process.TFileService = cms.Service("TFileService",
    fileName = cms.string('invHiggsTree.root')
)



### THINGS TO EDIT BELOW ###

# change Global Tag
process.GlobalTag.globaltag = 'START44_V6::All'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

# input files
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
    'file:/gpfs_phys/storm/cms/mc/Fall11/VBF_HToZZTo4Nu_M-120_7TeV-pythia6/GEN-SIM-RAW-HLTDEBUG-RECO/E7TeV_Ave32_50ns-v1/0000/04074BAA-0459-E111-91A3-003048D46300.root'
    ] );

