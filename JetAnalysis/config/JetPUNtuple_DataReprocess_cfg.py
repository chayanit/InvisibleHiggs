import FWCore.ParameterSet.Config as cms

from InvisibleHiggs.Ntuple.invHiggsInfo_Master_cfg import *

process.invHiggsInfo.mcPYTHIA   = cms.untracked.bool(False)

process.TFileService.fileName = cms.string('JetPUNtuple.root')

### THINGS TO EDIT BELOW ###

# change Global Tag
process.GlobalTag.globaltag = 'GR_R_53_V14::All'  #ReProcessData
#process.GlobalTag.globaltag = 'GR_P_V41_AN2::All'  #PromptReco
#process.GlobalTag.globaltag = 'START53_V7A::All'  #MC

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

# input files
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
    'file:/afs/cern.ch/user/s/srimanob/public/ForInvHiggs/Samples/patTuple_DataPrompt_V8.root'
    ] );

process.load('InvisibleHiggs.JetAnalysis.InvHiggsPUJetNtuple_cfi')
process.MyNtuple = process.InvHiggsPUJetNtupleTemplate.clone()
process.MyNtuple.METResultsLabel      = cms.InputTag("TriggerResults","","PAT")
process.MyNtuple.VertexLabel          = cms.InputTag("offlinePrimaryVertices")
process.MyNtuple.METLabel             = cms.InputTag("patMETsPF")
process.MyNtuple.JetLabel             = cms.InputTag("goodPatJets")
process.MyNtuple.puJetMvaTag          = cms.InputTag("puJetMva:fullDiscriminant")
process.MyNtuple.puJetIdTag           = cms.InputTag("puJetMva:fullId")
process.MyNtuple.MuonLabel            = cms.InputTag("selectMuons")
process.MyNtuple.LooseMuonLabel       = cms.InputTag("selectLooseMuons")
process.MyNtuple.ElectronLabel        = cms.InputTag("selectElectrons")
process.MyNtuple.LooseElectronLabel   = cms.InputTag("selectLooseElectrons")
process.MyNtuple.PhotonLabel          = cms.InputTag("cleanPatPhotons")
process.MyNtuple.TauLabel             = cms.InputTag("cleanPatTaus")

process.path.replace(process.invHiggsInfo,process.MyNtuple)
