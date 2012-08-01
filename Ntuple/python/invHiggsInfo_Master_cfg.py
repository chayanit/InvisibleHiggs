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

# Ntuple producer
process.load('InvisibleHiggs/Ntuple/invHiggsInfo_cfi')

# TTree output file
process.load("CommonTools.UtilAlgos.TFileService_cfi")
process.TFileService.fileName = cms.string('invHiggsInfo.root')

# path
process.path = cms.Path(
    process.invHiggsInfo
)



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

