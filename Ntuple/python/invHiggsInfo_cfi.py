import FWCore.ParameterSet.Config as cms

invHiggsInfo = cms.EDAnalyzer(
    "InvHiggsInfoProducer",

    # MC
    genEvtTag      = cms.untracked.InputTag("generator","","SIM"),
    genParticleTag = cms.untracked.InputTag("genParticles","","SIM"),
    mcPYTHIA       = cms.untracked.bool(True),

    # trigger
    hltResultsTag = cms.untracked.InputTag("TriggerResults","","HLT"),
    hltPath1Name  = cms.untracked.string("HLT_DiPFJet40_PFMETnoMu65_MJJ600VBF_LeadingJets_v"),
    hltPath2Name  = cms.untracked.string("HLT_DiPFJet40_PFMETnoMu65_MJJ800VBF_AllJets_v"),

    # jets
    jetTag      = cms.untracked.InputTag("goodPatJets"),
    puJetMvaTag = cms.untracked.InputTag("puJetMva", "fullDiscriminant"),
    puJetIdTag  = cms.untracked.InputTag("puJetMva", "fullId"),
    useLeadingJets = cms.untracked.bool(True),

    # leptons
    muonTag     = cms.untracked.InputTag("selectMuons"), #cleanPatMuons
    electronTag = cms.untracked.InputTag("selectElectrons"), #cleanPatElectrons

    # met
    metResultsTag = cms.untracked.InputTag("TriggerResults","","PAT"),
    metTag = cms.untracked.InputTag("patMETs"),
    mhtTag = cms.untracked.InputTag("patMHTs"),

    # Z and W candidates
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
