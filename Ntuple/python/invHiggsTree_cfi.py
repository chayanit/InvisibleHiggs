import FWCore.ParameterSet.Config as cms

invHiggsTree = cms.EDAnalyzer(
    "InvHiggsTreeProducer",

    # control
    usePAT = cms.untracked.bool(False),
    
    # trigger
    l1BitsTag = cms.untracked.InputTag("gtDigis"),
    l1JetsTag = cms.untracked.string("l1extraParticles"),
    hltResultsTag = cms.untracked.InputTag("TriggerResults","","HLT"),
    hltEventTag = cms.untracked.InputTag("hltTriggerSummaryAOD","","HLT"),
    # Remove '*' from name, so that ntuple Producer can search for strings properly in C++
    hltPathName = cms.untracked.string("HLT_DiPFJet40_PFMETnoMu65_MJJ800VBF_AllJets_v"),
    hltL3Tag = cms.untracked.InputTag("","","HLT"),

    # MC
    mcTag = cms.untracked.InputTag("generator"),
    
    # jets
    jetCorrectorServiceName = cms.untracked.string("ak5CaloJetsL1L2L3Residual"),
    caloJetTag = cms.untracked.InputTag("ak5CaloJets"),
    caloJetIDTag = cms.untracked.InputTag("ak5JetID"),
    pfJetTag = cms.untracked.InputTag("ak5PFJets"),

    # muons
    muonTag = cms.untracked.InputTag("muons"),

    # electrons
    electronTag = cms.untracked.InputTag("gsfElectrons"),

    # MET
    metTag = cms.untracked.InputTag("met"),

    # MHT
#    mhtTag = cms.untracked.InputTag("patPFMht"),

    # vertices
    vertexTag = cms.untracked.InputTag("offlinePrimaryVertices")

)
