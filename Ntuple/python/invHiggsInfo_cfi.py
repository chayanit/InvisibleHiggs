import FWCore.ParameterSet.Config as cms

invHiggsInfo = cms.EDAnalyzer(
    "InvHiggsInfoProducer",

    # MC
    genEvtTag      = cms.untracked.InputTag("generator","","SIM"),
    genParticleTag = cms.untracked.InputTag("genParticles","","SIM"),
    mcPYTHIA       = cms.untracked.bool(True),

    # trigger
    L1ExtraEtMissMET = cms.untracked.InputTag("l1extraParticles","MET"),
    L1ExtraEtMissMHT = cms.untracked.InputTag("l1extraParticles","MHT"),
    hltResultsTag = cms.untracked.InputTag("TriggerResults","","HLT"),
    hltPath1Name  = cms.untracked.string("HLT_DiPFJet40_PFMETnoMu65_MJJ600VBF_LeadingJets_v"),
    hltPath2Name  = cms.untracked.string("HLT_DiPFJet40_PFMETnoMu65_MJJ800VBF_AllJets_v"),
    hltPath3Name  = cms.untracked.string("HLT_DiJet35_MJJ700_AllJets_DEta3p5_VBF_v"),
    hltPath4Name  = cms.untracked.string("HLT_DiJet30_MJJ700_AllJets_DEta3p5_VBF_v"),
    
    # jets 
    jetTag      = cms.untracked.InputTag("goodPatJets"),
    puJetMvaTag = cms.untracked.InputTag("puJetMva", "fullDiscriminant"),
    puJetIdTag  = cms.untracked.InputTag("puJetMva", "fullId"),
    useLeadingJets = cms.untracked.bool(True),

    # leptons
    muonTag     = cms.untracked.InputTag("selectMuons"), #cleanPatMuons
    looseMuonTag     = cms.untracked.InputTag("selectLooseMuons"), #cleanPatMuons

    electronTag = cms.untracked.InputTag("selectElectrons"), #cleanPatElectrons
    #looseElectronTag = cms.untracked.InputTag("selectLooseElectrons"), #cleanPatElectrons
    looseElectronTag = cms.untracked.InputTag("selectVetoElectrons"), #cleanPatElectrons

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
    puDataHist = cms.untracked.string(""),

    # trigger MC re-weighting
    trigCorrFile = cms.untracked.string("")
    
)
