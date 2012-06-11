import FWCore.ParameterSet.Config as cms

process = cms.Process("TREE")

process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.threshold = ''
process.MessageLogger.cerr.FwkReport.reportEvery = 1000


## # geometry & magnetic field
## process.load('Configuration/StandardSequences/GeometryIdeal_cff')
## process.load("Configuration.StandardSequences.MagneticField_38T_cff")
## process.load("Configuration.StandardSequences.Reconstruction_cff")

# conditions
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')

# HLT filter
process.load('HLTrigger.HLTfilters.hltHighLevel_cfi')
process.hltHighLevel.TriggerResultsTag = cms.InputTag("TriggerResults","","HLT")
process.hltHighLevel.throw = cms.bool(False)
process.hltHighLevel.HLTPaths = cms.vstring(
    "HLT_DiPFJet40_PFMETnoMu65_MJJ600VBF_LeadingJets_v*",
    "HLT_DiPFJet40_PFMETnoMu65_MJJ800VBF_AllJets_v*"
)

# Ntuple producer
process.load('InvisibleHiggs/Ntuple/invHiggsAOD_cfi')

# TTree output file
process.load("CommonTools.UtilAlgos.TFileService_cfi")
process.TFileService.fileName = cms.string('invHiggsAODTree.root')

# path
process.path = cms.Path(
    process.hltHighLevel
    +process.invHiggsAOD
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

