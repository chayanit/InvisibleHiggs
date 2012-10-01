import FWCore.ParameterSet.Config as cms
process = cms.Process("PHATJET")


#--Logger
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

#-- GlobalTag
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff') 
process.GlobalTag.globaltag = 'START53_V11::All'


#-- Input
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( ['/store/user/srimanob/InvHiggs/test/patTuple_MC.root'] );
process.source.duplicateCheckMode = cms.untracked.string('noDuplicateCheck')


#-- Study module
process.load('InvisibleHiggs.JetAnalysis.pujetanalysis_cfi')
process.MyPUJetStudy = process.PUJetStudyTemplate.clone()
process.MyPUJetStudy.leadingJets = cms.untracked.bool(False);


#-- Let's it runs
process.p = cms.Path(process.MyPUJetStudy)
