import FWCore.ParameterSet.Config as cms

invHiggsInfo = cms.EDAnalyzer(
    "InvHiggsInfoProducer",

    # control
    useLeadingJets = cms.untracked.bool(True),
    
    # trigger
    hltResultsTag = cms.untracked.InputTag("TriggerResults","","HLT"),

    # MC
    mcTag = cms.untracked.InputTag("generator"),
    
    # tags
    jetTag = cms.untracked.InputTag("cleanPatJets"),
    muonTag = cms.untracked.InputTag("cleanPatMuons"),
    electronTag = cms.untracked.InputTag("cleanPatElectrons"),
    metTag = cms.untracked.InputTag("patMETs"),
    mhtTag = cms.untracked.InputTag("patMHTs"),

    # PU re-weighting
    puMCFile = cms.untracked.string(""),
    puDataFile = cms.untracked.string(""),
    puMCHist = cms.untracked.string(""),
    puDataHist = cms.untracked.string("")

)
