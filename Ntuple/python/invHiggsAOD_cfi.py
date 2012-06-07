import FWCore.ParameterSet.Config as cms

invHiggsAOD = cms.EDAnalyzer(
    "InvHiggsAODProducer",

    # control
    useMC = cms.untracked.bool(False),
    
    # trigger
    l1BitsTag = cms.untracked.InputTag("gtDigis"),
    l1JetsTag = cms.untracked.string("l1extraParticles"),
    hltResultsTag = cms.untracked.InputTag("TriggerResults","","HLT"),
    hltEventTag = cms.untracked.InputTag("hltTriggerSummaryAOD","","HLT"),
    # Remove '*' from name, so that ntuple Producer can search for strings properly in C++
    hltPath1Name = cms.untracked.string("HLT_DiPFJet40_PFMETnoMu65_MJJ600VBF_LeadingJets_v"),
    hltPath2Name = cms.untracked.string("HLT_DiPFJet40_PFMETnoMu65_MJJ800VBF_AllJets_v"),
    hltL3Tag = cms.untracked.InputTag("","","HLT"),

    # MC
    mcTag = cms.untracked.InputTag("generator"),
    
    # jets
    jetCorrectorServiceName = cms.untracked.string(""),   # for corrections on the fly
    caloJetTag = cms.untracked.InputTag("ak5CaloJets"),
    caloJetIDTag = cms.untracked.InputTag("ak5JetID"),   # AOD
    pfJetTag = cms.untracked.InputTag("ak5PFJets"),

    # muons
    muonTag = cms.untracked.InputTag("muons"),

    # electrons
    electronTag = cms.untracked.InputTag("gsfElectrons"),

    # energy sums
    caloMETTag = cms.untracked.InputTag("met"),
    pfMETTag = cms.untracked.InputTag("pfMet"),
    pfMHTTag = cms.untracked.InputTag("pfMht"),  # only in PAT

    # vertices
    vertexTag = cms.untracked.InputTag("offlinePrimaryVertices"),

    # PU re-weighting
    puMCFile = cms.untracked.string(""),
    puDataFile = cms.untracked.string(""),
    puMCHist = cms.untracked.string(""),
    puDataHist = cms.untracked.string("")

)
