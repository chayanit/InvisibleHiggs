import FWCore.ParameterSet.Config as cms

invHiggsInfo = cms.EDAnalyzer(
    "InvHiggsInfoProducer",

    # control
    useLeadingJets = cms.untracked.bool(True),
    
    # trigger
    hltResultsTag = cms.untracked.InputTag("TriggerResults","","HLT"),
    hltPath1Name = cms.untracked.string("HLT_DiPFJet40_PFMETnoMu65_MJJ600VBF_LeadingJets_v"),
    hltPath2Name = cms.untracked.string("HLT_DiPFJet40_PFMETnoMu65_MJJ800VBF_AllJets_v"),

    # met filters
    metResultsTag = cms.untracked.InputTag("TriggerResults","","PAT"),

    # MC
    mcTag = cms.untracked.InputTag("generator"),
    
    # tags
    jetTag = cms.untracked.InputTag("goodPatJets"),
    puJetMvaTag = cms.untracked.InputTag("puJetMva", "fullDiscriminant"),
    puJetIdTag = cms.untracked.InputTag("puJetMva", "fullId"),
    muonTag = cms.untracked.InputTag("selectMuons"), #cleanPatMuons
    electronTag = cms.untracked.InputTag("selectElectrons"), #cleanPatElectrons
    metTag = cms.untracked.InputTag("patMETs"),
    mhtTag = cms.untracked.InputTag("patMHTs"),
    zMuTag = cms.untracked.InputTag("bestZMuMu"),
    zElTag = cms.untracked.InputTag("bestZEE"),
    wMuTag = cms.untracked.InputTag("bestWMuNu"),
    wElTag = cms.untracked.InputTag("bestWENu"),

    # PU re-weighting
    puMCFile = cms.untracked.string(""),
    puDataFile = cms.untracked.string(""),
    puMCHist = cms.untracked.string(""),
    puDataHist = cms.untracked.string("")
    
)
