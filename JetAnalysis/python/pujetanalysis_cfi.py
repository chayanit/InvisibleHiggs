import FWCore.ParameterSet.Config as cms

PUJetStudyTemplate = cms.EDAnalyzer(
    'InvHiggsPUJetStudy',
    trigEvent   = cms.untracked.InputTag("hltTriggerSummaryAOD","","HLT"),
    trigResults = cms.untracked.InputTag("TriggerResults","","HLT"),

    #
    jetTag = cms.untracked.InputTag("goodPatJets"),
    puJetMvaTag = cms.untracked.InputTag("puJetMva", "fullDiscriminant"),
    puJetIdTag = cms.untracked.InputTag("puJetMva", "fullId"),
    metTag = cms.untracked.InputTag("patMETs"),

    #
    muonTag = cms.untracked.InputTag("cleanPatMuons"),
    electronTag = cms.untracked.InputTag("cleanPatElectrons"),

    #
    leadingJets = cms.untracked.bool(True)
)
