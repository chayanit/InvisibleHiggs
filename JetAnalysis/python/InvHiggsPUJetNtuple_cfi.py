import FWCore.ParameterSet.Config as cms

InvHiggsPUJetNtupleTemplate = cms.EDAnalyzer(
    'InvHiggsPUJetNtuple',
    METResultsLabel    = cms.InputTag("TriggerResults","","PAT"),
    VertexLabel        = cms.InputTag("offlinePrimaryVertices"), 
    METLabel           = cms.InputTag("pfMet"),
    JetLabel           = cms.InputTag("ak5PFJet"),
    puJetMvaTag        = cms.InputTag("puJetMva:fullDiscriminant"),
    puJetIdTag         = cms.InputTag("puJetMva:fullId"),
    MuonLabel          = cms.InputTag("muons"),
    LooseMuonLabel     = cms.InputTag("muons"),
    ElectronLabel      = cms.InputTag("electrons"),
    LooseElectronLabel = cms.InputTag("electrons"),
    TauLabel           = cms.InputTag("taus"),
    PhotonLabel        = cms.InputTag("photons"),
)
