import FWCore.ParameterSet.Config as cms

process = cms.Process("PAT")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/data/Run2012C/MET/AOD/PromptReco-v2/000/203/002/FEB6B5F2-3902-E211-8B50-003048D2BC52.root')
)
process.PFCandAssoMap = cms.EDProducer("PFCand_AssoMap",
    ConversionsCollection = cms.InputTag("allConversions"),
    NIVertexCollection = cms.InputTag("particleFlowDisplacedVertex"),
    BeamSpot = cms.InputTag("offlineBeamSpot"),
    V0KshortCollection = cms.InputTag("generalV0Candidates","Kshort"),
    V0LambdaCollection = cms.InputTag("generalV0Candidates","Lambda"),
    VertexAssClosest = cms.untracked.bool(True),
    VertexCollection = cms.InputTag("offlinePrimaryVertices"),
    UseBeamSpotCompatibility = cms.untracked.bool(True),
    PFCandidateCollection = cms.InputTag("particleFlow"),
    ignoreMissingCollection = cms.bool(True)
)


process.Tracks2Vertex = cms.EDProducer("PF_PU_AssoMap",
    VertexAssUseAbsDistance = cms.untracked.bool(False),
    VertexAssClosest = cms.untracked.bool(True),
    NIVertexCollection = cms.InputTag("particleFlowDisplacedVertex"),
    BeamSpot = cms.InputTag("offlineBeamSpot"),
    V0KshortCollection = cms.InputTag("generalV0Candidates","Kshort"),
    VertexAssOneDim = cms.untracked.bool(True),
    V0LambdaCollection = cms.InputTag("generalV0Candidates","Lambda"),
    GsfElectronCollection = cms.InputTag("gsfElectrons"),
    ConversionsCollection = cms.InputTag("allConversions"),
    VertexCollection = cms.InputTag("offlinePrimaryVertices"),
    UseBeamSpotCompatibility = cms.untracked.bool(False),
    ignoreMissingCollection = cms.bool(False),
    TrackCollection = cms.InputTag("generalTracks")
)


process.ak5CaloJetsL1FastL2L3 = cms.EDProducer("CaloJetCorrectionProducer",
    src = cms.InputTag("ak5CaloJets"),
    correctors = cms.vstring('ak5CaloL1FastL2L3')
)


process.ak5CaloJetsL1FastL2L3Residual = cms.EDProducer("CaloJetCorrectionProducer",
    src = cms.InputTag("ak5CaloJets"),
    correctors = cms.vstring('ak5CaloL1FastL2L3Residual')
)


process.ak5CaloJetsL1L2L3 = cms.EDProducer("CaloJetCorrectionProducer",
    src = cms.InputTag("ak5CaloJets"),
    correctors = cms.vstring('ak5CaloL1L2L3')
)


process.ak5CaloJetsL1L2L3Residual = cms.EDProducer("CaloJetCorrectionProducer",
    src = cms.InputTag("ak5CaloJets"),
    correctors = cms.vstring('ak5CaloL1L2L3Residual')
)


process.ak5CaloJetsL2L3 = cms.EDProducer("CaloJetCorrectionProducer",
    src = cms.InputTag("ak5CaloJets"),
    correctors = cms.vstring('ak5CaloL2L3')
)


process.ak5CaloJetsL2L3Residual = cms.EDProducer("CaloJetCorrectionProducer",
    src = cms.InputTag("ak5CaloJets"),
    correctors = cms.vstring('ak5CaloL2L3Residual')
)


process.ak5JPTJetsL1FastL2L3 = cms.EDProducer("JPTJetCorrectionProducer",
    src = cms.InputTag("JetPlusTrackZSPCorJetAntiKt5"),
    correctors = cms.vstring('ak5JPTL1FastL2L3')
)


process.ak5JPTJetsL1FastL2L3Residual = cms.EDProducer("JPTJetCorrectionProducer",
    src = cms.InputTag("JetPlusTrackZSPCorJetAntiKt5"),
    correctors = cms.vstring('ak5JPTL1FastL2L3Residual')
)


process.ak5JPTJetsL1L2L3 = cms.EDProducer("JPTJetCorrectionProducer",
    src = cms.InputTag("JetPlusTrackZSPCorJetAntiKt5"),
    correctors = cms.vstring('ak5JPTL1L2L3')
)


process.ak5JPTJetsL1L2L3Residual = cms.EDProducer("JPTJetCorrectionProducer",
    src = cms.InputTag("JetPlusTrackZSPCorJetAntiKt5"),
    correctors = cms.vstring('ak5JPTL1L2L3Residual')
)


process.ak5JPTJetsL2L3 = cms.EDProducer("JPTJetCorrectionProducer",
    src = cms.InputTag("JetPlusTrackZSPCorJetAntiKt5"),
    correctors = cms.vstring('ak5JPTL2L3')
)


process.ak5JPTJetsL2L3Residual = cms.EDProducer("JPTJetCorrectionProducer",
    src = cms.InputTag("JetPlusTrackZSPCorJetAntiKt5"),
    correctors = cms.vstring('ak5JPTL2L3Residual')
)


process.ak5JetExtender = cms.EDProducer("JetExtender",
    jets = cms.InputTag("ak5CaloJets"),
    jet2TracksAtCALO = cms.InputTag("ak5JetTracksAssociatorAtCaloFace"),
    jet2TracksAtVX = cms.InputTag("ak5JetTracksAssociatorAtVertex"),
    coneSize = cms.double(0.5)
)


process.ak5JetTracksAssociatorAtCaloFace = cms.EDProducer("JetTracksAssociatorAtCaloFace",
    trackQuality = cms.string('goodIterative'),
    tracks = cms.InputTag("generalTracks"),
    coneSize = cms.double(0.5),
    extrapolations = cms.InputTag("trackExtrapolator"),
    jets = cms.InputTag("ak5CaloJets")
)


process.ak5JetTracksAssociatorAtVertex = cms.EDProducer("JetTracksAssociatorAtVertex",
    tracks = cms.InputTag("generalTracks"),
    useAssigned = cms.bool(False),
    coneSize = cms.double(0.5),
    pvSrc = cms.InputTag("offlinePrimaryVertices"),
    jets = cms.InputTag("ak5CaloJets")
)


process.ak5PFJetsL1FastL2L3 = cms.EDProducer("PFJetCorrectionProducer",
    src = cms.InputTag("ak5PFJets"),
    correctors = cms.vstring('ak5PFL1FastL2L3')
)


process.ak5PFJetsL1FastL2L3Residual = cms.EDProducer("PFJetCorrectionProducer",
    src = cms.InputTag("ak5PFJets"),
    correctors = cms.vstring('ak5PFL1FastL2L3Residual')
)


process.ak5PFJetsL1L2L3 = cms.EDProducer("PFJetCorrectionProducer",
    src = cms.InputTag("ak5PFJets"),
    correctors = cms.vstring('ak5PFL1L2L3')
)


process.ak5PFJetsL1L2L3Residual = cms.EDProducer("PFJetCorrectionProducer",
    src = cms.InputTag("ak5PFJets"),
    correctors = cms.vstring('ak5PFL1L2L3Residual')
)


process.ak5PFJetsL2L3 = cms.EDProducer("PFJetCorrectionProducer",
    src = cms.InputTag("ak5PFJets"),
    correctors = cms.vstring('ak5PFL2L3')
)


process.ak5PFJetsL2L3Residual = cms.EDProducer("PFJetCorrectionProducer",
    src = cms.InputTag("ak5PFJets"),
    correctors = cms.vstring('ak5PFL2L3Residual')
)


process.ak5TrackJetsL2L3 = cms.EDProducer("TrackJetCorrectionProducer",
    src = cms.InputTag("ak5TrackJets"),
    correctors = cms.vstring('ak5TrackL2L3')
)


process.ak7CaloJetsL1FastL2L3 = cms.EDProducer("CaloJetCorrectionProducer",
    src = cms.InputTag("ak7CaloJets"),
    correctors = cms.vstring('ak7CaloL1FastL2L3')
)


process.ak7CaloJetsL1FastL2L3Residual = cms.EDProducer("CaloJetCorrectionProducer",
    src = cms.InputTag("ak7CaloJets"),
    correctors = cms.vstring('ak7CaloL1FastL2L3Residual')
)


process.ak7CaloJetsL1L2L3 = cms.EDProducer("CaloJetCorrectionProducer",
    src = cms.InputTag("ak7CaloJets"),
    correctors = cms.vstring('ak7CaloL1L2L3')
)


process.ak7CaloJetsL1L2L3Residual = cms.EDProducer("CaloJetCorrectionProducer",
    src = cms.InputTag("ak7CaloJets"),
    correctors = cms.vstring('ak7CaloL1L2L3Residual')
)


process.ak7CaloJetsL2L3 = cms.EDProducer("CaloJetCorrectionProducer",
    src = cms.InputTag("ak7CaloJets"),
    correctors = cms.vstring('ak7CaloL2L3')
)


process.ak7CaloJetsL2L3Residual = cms.EDProducer("CaloJetCorrectionProducer",
    src = cms.InputTag("ak7CaloJets"),
    correctors = cms.vstring('ak7CaloL2L3Residual')
)


process.ak7PFJetsL1FastL2L3 = cms.EDProducer("PFJetCorrectionProducer",
    src = cms.InputTag("ak7PFJets"),
    correctors = cms.vstring('ak7PFL1FastL2L3')
)


process.ak7PFJetsL1FastL2L3Residual = cms.EDProducer("PFJetCorrectionProducer",
    src = cms.InputTag("ak7PFJets"),
    correctors = cms.vstring('ak7PFL1FastL2L3Residual')
)


process.ak7PFJetsL1L2L3 = cms.EDProducer("PFJetCorrectionProducer",
    src = cms.InputTag("ak7PFJets"),
    correctors = cms.vstring('ak7PFL1L2L3')
)


process.ak7PFJetsL1L2L3Residual = cms.EDProducer("PFJetCorrectionProducer",
    src = cms.InputTag("ak7PFJets"),
    correctors = cms.vstring('ak7PFL1L2L3Residual')
)


process.ak7PFJetsL2L3 = cms.EDProducer("PFJetCorrectionProducer",
    src = cms.InputTag("ak7PFJets"),
    correctors = cms.vstring('ak7PFL2L3')
)


process.ak7PFJetsL2L3Residual = cms.EDProducer("PFJetCorrectionProducer",
    src = cms.InputTag("ak7PFJets"),
    correctors = cms.vstring('ak7PFL2L3Residual')
)


process.caloJetMETcorr = cms.EDProducer("CaloJetMETcorrInputProducer",
    src = cms.InputTag("ak5CaloJets"),
    type1JetPtThreshold = cms.double(20.0),
    skipEMfractionThreshold = cms.double(0.9),
    skipEM = cms.bool(True),
    jetCorrEtaMax = cms.double(9.9),
    srcMET = cms.InputTag("corMetGlobalMuons"),
    jetCorrLabel = cms.string('ak5CaloL2L3')
)


process.caloType1CorrectedMet = cms.EDProducer("CorrectedCaloMETProducer",
    applyType2Corrections = cms.bool(False),
    srcType1Corrections = cms.VInputTag(cms.InputTag("caloJetMETcorr","type1")),
    src = cms.InputTag("corMetGlobalMuons"),
    applyType1Corrections = cms.bool(True)
)


process.caloType1p2CorrectedMet = cms.EDProducer("CorrectedCaloMETProducer",
    src = cms.InputTag("corMetGlobalMuons"),
    applyType1Corrections = cms.bool(True),
    type2CorrFormula = cms.string('A + B*TMath::Exp(-C*x)'),
    srcUnclEnergySums = cms.VInputTag(cms.InputTag("caloJetMETcorr","type2"), cms.InputTag("muonCaloMETcorr")),
    srcType1Corrections = cms.VInputTag(cms.InputTag("caloJetMETcorr","type1")),
    applyType2Corrections = cms.bool(True),
    type2CorrParameter = cms.PSet(
        A = cms.double(2.0),
        C = cms.double(0.1),
        B = cms.double(1.3)
    )
)


process.cleanPatElectrons = cms.EDProducer("PATElectronCleaner",
    finalCut = cms.string(''),
    src = cms.InputTag("selectedPatElectrons"),
    checkOverlaps = cms.PSet(
        muons = cms.PSet(
            src = cms.InputTag("cleanPatMuons"),
            deltaR = cms.double(0.3),
            pairCut = cms.string(''),
            checkRecoComponents = cms.bool(False),
            algorithm = cms.string('byDeltaR'),
            preselection = cms.string(''),
            requireNoOverlaps = cms.bool(False)
        )
    ),
    preselection = cms.string('')
)


process.cleanPatJets = cms.EDProducer("PATJetCleaner",
    finalCut = cms.string(''),
    src = cms.InputTag("selectedPatJets"),
    checkOverlaps = cms.PSet(
        taus = cms.PSet(
            src = cms.InputTag("cleanPatTaus"),
            deltaR = cms.double(0.5),
            pairCut = cms.string(''),
            checkRecoComponents = cms.bool(False),
            algorithm = cms.string('byDeltaR'),
            preselection = cms.string(''),
            requireNoOverlaps = cms.bool(False)
        ),
        photons = cms.PSet(
            src = cms.InputTag("cleanPatPhotons"),
            deltaR = cms.double(0.5),
            pairCut = cms.string(''),
            checkRecoComponents = cms.bool(False),
            algorithm = cms.string('byDeltaR'),
            preselection = cms.string(''),
            requireNoOverlaps = cms.bool(False)
        ),
        electrons = cms.PSet(
            src = cms.InputTag("cleanPatElectrons"),
            deltaR = cms.double(0.5),
            pairCut = cms.string(''),
            checkRecoComponents = cms.bool(False),
            algorithm = cms.string('byDeltaR'),
            preselection = cms.string(''),
            requireNoOverlaps = cms.bool(False)
        ),
        muons = cms.PSet(
            src = cms.InputTag("cleanPatMuons"),
            deltaR = cms.double(0.5),
            pairCut = cms.string(''),
            checkRecoComponents = cms.bool(False),
            algorithm = cms.string('byDeltaR'),
            preselection = cms.string(''),
            requireNoOverlaps = cms.bool(False)
        ),
        tkIsoElectrons = cms.PSet(
            src = cms.InputTag("cleanPatElectrons"),
            deltaR = cms.double(0.3),
            pairCut = cms.string(''),
            checkRecoComponents = cms.bool(False),
            algorithm = cms.string('byDeltaR'),
            preselection = cms.string('pt > 10 && trackIso < 3'),
            requireNoOverlaps = cms.bool(False)
        )
    ),
    preselection = cms.string('')
)


process.cleanPatMuons = cms.EDProducer("PATMuonCleaner",
    finalCut = cms.string(''),
    src = cms.InputTag("selectedPatMuons"),
    checkOverlaps = cms.PSet(

    ),
    preselection = cms.string('')
)


process.cleanPatPhotons = cms.EDProducer("PATPhotonCleaner",
    finalCut = cms.string(''),
    src = cms.InputTag("selectedPatPhotons"),
    checkOverlaps = cms.PSet(
        electrons = cms.PSet(
            src = cms.InputTag("cleanPatElectrons"),
            requireNoOverlaps = cms.bool(False),
            algorithm = cms.string('bySuperClusterSeed')
        )
    ),
    preselection = cms.string('')
)


process.cleanPatTaus = cms.EDProducer("PATTauCleaner",
    finalCut = cms.string('pt > 20. & abs(eta) < 2.3'),
    src = cms.InputTag("selectedPatTaus"),
    checkOverlaps = cms.PSet(
        muons = cms.PSet(
            src = cms.InputTag("cleanPatMuons"),
            deltaR = cms.double(0.3),
            pairCut = cms.string(''),
            checkRecoComponents = cms.bool(False),
            algorithm = cms.string('byDeltaR'),
            preselection = cms.string(''),
            requireNoOverlaps = cms.bool(False)
        ),
        electrons = cms.PSet(
            src = cms.InputTag("cleanPatElectrons"),
            deltaR = cms.double(0.3),
            pairCut = cms.string(''),
            checkRecoComponents = cms.bool(False),
            algorithm = cms.string('byDeltaR'),
            preselection = cms.string(''),
            requireNoOverlaps = cms.bool(False)
        )
    ),
    preselection = cms.string('tauID("decayModeFinding") > 0.5 & tauID("byLooseCombinedIsolationDeltaBetaCorr") > 0.5 & tauID("againstMuonMedium") > 0.5 & tauID("againstElectronMedium") > 0.5')
)


process.eidCutBasedExt = cms.EDProducer("EleIdCutBasedExtProducer",
    electronQuality = cms.string('loose'),
    classbasedtightEleIDCutsV02 = cms.PSet(
        cutisohcal = cms.vdouble(10.9, 7.01, 8.75, 3.51, 7.75, 
            1.62, 11.6, 9.9, 4.97, 5.33, 
            3.18, 2.32, 0.164, 5.46, 12.0, 
            0.00604, 4.1, 0.000628),
        cutmishits = cms.vdouble(5.5, 1.5, 0.5, 1.5, 2.5, 
            0.5, 3.5, 5.5, 0.5, 0.5, 
            0.5, 0.5, 0.5, 1.5, 0.5, 
            0.5, 0.5, 0.5),
        cuthoe = cms.vdouble(0.0871, 0.0289, 0.0783, 0.0946, 0.0245, 
            0.0363, 0.0671, 0.048, 0.0614, 0.0924, 
            0.0158, 0.049, 0.0382, 0.0915, 0.0451, 
            0.0452, 0.00196, 0.0043),
        cutdeta = cms.vdouble(0.00915, 0.00302, 0.0061, 0.0135, 0.00565, 
            0.00793, 0.0102, 0.00266, 0.0106, 0.00903, 
            0.00766, 0.00723, 0.0116, 0.00203, 0.00659, 
            0.0148, 0.00555, 0.0128),
        cuteopin = cms.vdouble(0.878, 0.859, 0.874, 0.944, 0.737, 
            0.773, 0.86, 0.967, 0.917, 0.812, 
            0.915, 1.01, 0.847, 0.953, 0.979, 
            0.841, 0.771, 1.09),
        cutip = cms.vdouble(0.0239, 0.027, 0.0768, 0.0231, 0.178, 
            0.0957, 0.0102, 0.0168, 0.043, 0.0166, 
            0.0594, 0.0308, 2.1, 0.00527, 3.17, 
            4.91, 0.769, 5.9),
        cutisotk = cms.vdouble(6.53, 4.6, 6.0, 8.63, 3.11, 
            7.77, 5.42, 4.81, 4.06, 6.47, 
            2.8, 3.45, 5.29, 5.18, 15.4, 
            5.38, 4.47, 0.0347),
        cutsee = cms.vdouble(0.0131, 0.0106, 0.0115, 0.0306, 0.028, 
            0.0293, 0.0131, 0.0106, 0.0115, 0.0317, 
            0.029, 0.0289, 0.0142, 0.0106, 0.0103, 
            0.035, 0.0296, 0.0333),
        cutdphi = cms.vdouble(0.0369, 0.0307, 0.117, 0.0475, 0.0216, 
            0.117, 0.0372, 0.0246, 0.0426, 0.0612, 
            0.0142, 0.039, 0.0737, 0.0566, 0.0359, 
            0.0187, 0.012, 0.0358),
        cutisoecal = cms.vdouble(20.0, 27.2, 4.48, 13.5, 4.56, 
            3.19, 12.2, 13.1, 7.42, 7.67, 
            4.12, 4.85, 10.1, 12.4, 11.1, 
            11.0, 10.6, 13.4)
    ),
    classbasedtightEleIDCutsV03 = cms.PSet(
        cutdetain = cms.vdouble(0.00811, 0.00341, 0.00633, 0.0103, 0.00667, 
            0.01, 0.0106, 0.0145, 0.0163, 0.0076, 
            0.00259, 0.00511, 0.00941, 0.0043, 0.00857, 
            0.012, 0.0169, 0.00172, 0.00861, 0.00362, 
            0.00601, 0.00925, 0.00489, 0.00832, 0.0119, 
            0.0169, 0.000996),
        cutiso_sum = cms.vdouble(11.8, 8.31, 6.26, 6.18, 3.28, 
            4.38, 4.17, 5.4, 1.57, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0),
        cutip_gsf = cms.vdouble(0.0213, 0.0422, 0.0632, 0.0361, 0.073, 
            0.126, 0.171, 0.119, 0.0372, 0.0131, 
            0.0146, 0.0564, 0.0152, 0.0222, 0.0268, 
            0.0314, 0.0884, 0.00374, 0.00852, 0.00761, 
            0.0143, 0.0106, 0.0127, 0.0119, 0.0123, 
            0.0235, 0.00363),
        cuthoe = cms.vdouble(0.0783, 0.0387, 0.105, 0.118, 0.0227, 
            0.062, 0.13, 2.47, 0.38, 0.0888, 
            0.0503, 0.0955, 0.0741, 0.015, 0.03, 
            0.589, 1.13, 0.612, 0.0494, 0.0461, 
            0.0292, 0.0369, 0.0113, 0.0145, 0.124, 
            2.05, 0.61),
        cutfmishits = cms.vdouble(2.5, 1.5, 1.5, 1.5, 1.5, 
            0.5, 2.5, 0.5, 0.5, 2.5, 
            1.5, 0.5, 0.5, 0.5, 0.5, 
            0.5, 0.5, -0.5, 2.5, 1.5, 
            0.5, 0.5, 0.5, 0.5, 0.5, 
            0.5, 0.5),
        cutiso_sumoet = cms.vdouble(13.7, 11.6, 7.14, 9.98, 3.52, 
            4.87, 6.24, 7.96, 2.53, 11.2, 
            11.9, 7.88, 8.16, 5.58, 5.03, 
            11.4, 8.15, 5.79, 10.4, 11.1, 
            10.4, 7.47, 5.08, 5.9, 11.8, 
            14.1, 11.7),
        cutdcotdist = cms.vdouble(0.0393, 0.0256, 0.00691, 0.0394, 0.0386, 
            0.039, 0.0325, 0.0384, 0.0382, 0.0245, 
            0.000281, 5.46e-05, 0.0342, 0.0232, 0.00107, 
            0.0178, 0.0193, 0.000758, 0.000108, 0.0248, 
            0.000458, 0.0129, 0.00119, 0.0182, 4.53e-05, 
            0.0189, 0.000928),
        cutsee = cms.vdouble(0.0143, 0.0105, 0.0123, 0.0324, 0.0307, 
            0.0301, 0.0109, 0.027, 0.0292, 0.0133, 
            0.0104, 0.0116, 0.0332, 0.0296, 0.031, 
            0.00981, 0.0307, 0.072, 0.0149, 0.0105, 
            0.011, 0.0342, 0.0307, 0.0303, 0.00954, 
            0.0265, 0.0101),
        cuteseedopcor = cms.vdouble(0.784, 0.366, 0.57, 0.911, 0.298, 
            0.645, 0.51, 0.497, 0.932, 0.835, 
            0.968, 0.969, 0.923, 0.898, 0.98, 
            0.63, 0.971, 1.0, 0.515, 0.963, 
            0.986, 0.823, 0.879, 1.01, 0.931, 
            0.937, 1.05),
        cutdphiin = cms.vdouble(0.0404, 0.0499, 0.263, 0.042, 0.0484, 
            0.241, 0.242, 0.231, 0.286, 0.0552, 
            0.0338, 0.154, 0.0623, 0.0183, 0.0392, 
            0.0547, 0.0588, 0.00654, 0.042, 0.0217, 
            0.0885, 0.0445, 0.0141, 0.0234, 0.065, 
            0.0258, 0.0346),
        cutet = cms.vdouble(-100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, 13.7, 13.2, 
            13.6, 14.2, 14.1, 13.9, 12.9, 
            14.9, 17.7)
    ),
    classbasedtightEleIDCutsV00 = cms.PSet(
        deltaPhiIn = cms.vdouble(0.032, 0.016, 0.0525, 0.09, 0.025, 
            0.035, 0.065, 0.092),
        hOverE = cms.vdouble(0.05, 0.042, 0.045, 0.0, 0.055, 
            0.037, 0.05, 0.0),
        sigmaEtaEta = cms.vdouble(0.0125, 0.011, 0.01, 0.0, 0.0265, 
            0.0252, 0.026, 0.0),
        deltaEtaIn = cms.vdouble(0.0055, 0.003, 0.0065, 0.0, 0.006, 
            0.0055, 0.0075, 0.0),
        eSeedOverPin = cms.vdouble(0.24, 0.94, 0.11, 0.0, 0.32, 
            0.83, 0.0, 0.0)
    ),
    classbasedtightEleIDCutsV01 = cms.PSet(
        deltaPhiIn = cms.vdouble(0.0225, 0.0114, 0.0234, 0.039, 0.0215, 
            0.0095, 0.0148, 0.0167),
        hOverE = cms.vdouble(0.056, 0.0221, 0.037, 0.0, 0.0268, 
            0.0102, 0.0104, 0.0),
        sigmaEtaEta = cms.vdouble(0.0095, 0.0094, 0.0094, 0.0, 0.026, 
            0.0257, 0.0246, 0.0),
        deltaEtaIn = cms.vdouble(0.0043, 0.00282, 0.0036, 0.0, 0.0066, 
            0.0049, 0.0041, 0.0),
        eSeedOverPin = cms.vdouble(0.32, 0.94, 0.221, 0.0, 0.74, 
            0.89, 0.66, 0.0)
    ),
    classbasedtightEleIDCutsV06 = cms.PSet(
        cutdetain = cms.vdouble(0.0116, 0.00449, 0.00938, 0.0184, 0.00678, 
            0.0109, 0.0252, 0.0268, 0.0139),
        cutiso_sum = cms.vdouble(15.5, 12.2, 12.2, 11.7, 7.16, 
            9.71, 8.66, 11.9, 2.98),
        cutip_gsf = cms.vdouble(0.0131, 0.0586, 0.0839, 0.0366, 0.452, 
            0.204, 0.0913, 0.0802, 0.0731),
        cutip_gsfl = cms.vdouble(0.0119, 0.0527, 0.0471, 0.0212, 0.233, 
            0.267, 0.109, 0.122, 0.0479),
        cuthoe = cms.vdouble(0.215, 0.0608, 0.147, 0.369, 0.0349, 
            0.102, 0.52, 0.422, 0.404),
        cutiso_sumoetl = cms.vdouble(6.21, 6.81, 5.3, 5.39, 2.73, 
            4.73, 4.84, 3.46, 3.73),
        cutfmishits = cms.vdouble(1.5, 1.5, 1.5, 2.5, 2.5, 
            1.5, 1.5, 2.5, 0.5),
        cuthoel = cms.vdouble(0.228, 0.0836, 0.143, 0.37, 0.0392, 
            0.0979, 0.3, 0.381, 0.339),
        cutdphiin = cms.vdouble(0.0897, 0.0993, 0.295, 0.0979, 0.151, 
            0.252, 0.341, 0.308, 0.328),
        cutseel = cms.vdouble(0.0132, 0.0117, 0.0112, 0.0387, 0.0281, 
            0.0287, 0.00987, 0.0296, 0.0544),
        cutiso_sumoet = cms.vdouble(11.9, 7.81, 6.28, 8.92, 4.65, 
            5.49, 9.36, 8.84, 5.94),
        cutdcotdist = cms.vdouble(9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0),
        cutsee = cms.vdouble(0.0145, 0.0116, 0.012, 0.039, 0.0297, 
            0.0311, 0.00987, 0.0347, 0.0917),
        cuteseedopcor = cms.vdouble(0.637, 0.943, 0.742, 0.748, 0.763, 
            0.631, 0.214, 0.873, 0.473),
        cutdphiinl = cms.vdouble(0.061, 0.14, 0.286, 0.0921, 0.197, 
            0.24, 0.333, 0.303, 0.258),
        cutdetainl = cms.vdouble(0.00816, 0.00401, 0.0081, 0.019, 0.00588, 
            0.00893, 0.0171, 0.0434, 0.0143)
    ),
    classbasedtightEleIDCutsV04 = cms.PSet(
        cutdetain = cms.vdouble(0.00811, 0.00341, 0.00633, 0.0103, 0.00667, 
            0.01, 0.0106, 0.0145, 0.0163, 0.0076, 
            0.00259, 0.00511, 0.00941, 0.0043, 0.00857, 
            0.012, 0.0169, 0.00172, 0.00861, 0.00362, 
            0.00601, 0.00925, 0.00489, 0.00832, 0.0119, 
            0.0169, 0.000996),
        cutiso_sum = cms.vdouble(11.8, 8.31, 6.26, 6.18, 3.28, 
            4.38, 4.17, 5.4, 1.57, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0),
        cutip_gsf = cms.vdouble(0.0213, 0.0422, 0.0632, 0.0361, 0.073, 
            0.126, 0.171, 0.119, 0.0372, 0.0131, 
            0.0146, 0.0564, 0.0152, 0.0222, 0.0268, 
            0.0314, 0.0884, 0.00374, 0.00852, 0.00761, 
            0.0143, 0.0106, 0.0127, 0.0119, 0.0123, 
            0.0235, 0.00363),
        cuthoe = cms.vdouble(0.0783, 0.0387, 0.105, 0.118, 0.0227, 
            0.062, 0.13, 2.47, 0.38, 0.0888, 
            0.0503, 0.0955, 0.0741, 0.015, 0.03, 
            0.589, 1.13, 0.612, 0.0494, 0.0461, 
            0.0292, 0.0369, 0.0113, 0.0145, 0.124, 
            2.05, 0.61),
        cutfmishits = cms.vdouble(2.5, 1.5, 1.5, 1.5, 1.5, 
            0.5, 2.5, 0.5, 0.5, 2.5, 
            1.5, 0.5, 0.5, 0.5, 0.5, 
            0.5, 0.5, -0.5, 2.5, 1.5, 
            0.5, 0.5, 0.5, 0.5, 0.5, 
            0.5, 0.5),
        cutiso_sumoet = cms.vdouble(13.7, 11.6, 7.14, 9.98, 3.52, 
            4.87, 6.24, 7.96, 2.53, 11.2, 
            11.9, 7.88, 8.16, 5.58, 5.03, 
            11.4, 8.15, 5.79, 10.4, 11.1, 
            10.4, 7.47, 5.08, 5.9, 11.8, 
            14.1, 11.7),
        cutdcotdist = cms.vdouble(0.0393, 0.0256, 0.00691, 0.0394, 0.0386, 
            0.039, 0.0325, 0.0384, 0.0382, 0.0245, 
            0.000281, 5.46e-05, 0.0342, 0.0232, 0.00107, 
            0.0178, 0.0193, 0.000758, 0.000108, 0.0248, 
            0.000458, 0.0129, 0.00119, 0.0182, 4.53e-05, 
            0.0189, 0.000928),
        cutsee = cms.vdouble(0.0143, 0.0105, 0.0123, 0.0324, 0.0307, 
            0.0301, 0.0109, 0.027, 0.0292, 0.0133, 
            0.0104, 0.0116, 0.0332, 0.0296, 0.031, 
            0.00981, 0.0307, 0.072, 0.0149, 0.0105, 
            0.011, 0.0342, 0.0307, 0.0303, 0.00954, 
            0.0265, 0.0101),
        cuteseedopcor = cms.vdouble(0.784, 0.366, 0.57, 0.911, 0.298, 
            0.645, 0.51, 0.497, 0.932, 0.835, 
            0.968, 0.969, 0.923, 0.898, 0.98, 
            0.63, 0.971, 1.0, 0.515, 0.963, 
            0.986, 0.823, 0.879, 1.01, 0.931, 
            0.937, 1.05),
        cutdphiin = cms.vdouble(0.0404, 0.0499, 0.263, 0.042, 0.0484, 
            0.241, 0.242, 0.231, 0.286, 0.0552, 
            0.0338, 0.154, 0.0623, 0.0183, 0.0392, 
            0.0547, 0.0588, 0.00654, 0.042, 0.0217, 
            0.0885, 0.0445, 0.0141, 0.0234, 0.065, 
            0.0258, 0.0346),
        cutet = cms.vdouble(-100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, 13.7, 13.2, 
            13.6, 14.2, 14.1, 13.9, 12.9, 
            14.9, 17.7)
    ),
    electronIDType = cms.string('robust'),
    robusttightEleIDCutsV04 = cms.PSet(
        barrel = cms.vdouble(0.0201, 0.0102, 0.0211, 0.00606, -1, 
            -1, 2.34, 3.24, 4.51, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.00253, 0.0291, 0.022, 0.0032, -1, 
            -1, 0.826, 2.7, 0.255, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    electronVersion = cms.string(''),
    robusttightEleIDCutsV00 = cms.PSet(
        barrel = cms.vdouble(0.015, 0.0092, 0.02, 0.0025, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.018, 0.025, 0.02, 0.004, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robusttightEleIDCutsV01 = cms.PSet(
        barrel = cms.vdouble(0.01, 0.0099, 0.025, 0.004, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.01, 0.028, 0.02, 0.0066, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robusttightEleIDCutsV02 = cms.PSet(
        barrel = cms.vdouble(0.0201, 0.0102, 0.0211, 0.00606, -1, 
            -1, 2.34, 3.24, 4.51, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.00253, 0.0291, 0.022, 0.0032, -1, 
            -1, 0.826, 2.7, 0.255, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robusttightEleIDCutsV03 = cms.PSet(
        barrel = cms.vdouble(0.0201, 0.0102, 0.0211, 0.00606, -1, 
            -1, 2.34, 3.24, 4.51, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.00253, 0.0291, 0.022, 0.0032, -1, 
            -1, 0.826, 2.7, 0.255, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    reducedEndcapRecHitCollection = cms.InputTag("ecalRecHit","EcalRecHitsEE"),
    verticesCollection = cms.InputTag("offlinePrimaryVerticesWithBS"),
    classbasedlooseEleIDCuts = cms.PSet(
        cutdetain = cms.vdouble(0.0137, 0.00678, 0.0241, 0.0187, 0.0161, 
            0.0224, 0.0252, 0.0308, 0.0273),
        cutiso_sum = cms.vdouble(33.0, 17.0, 17.9, 18.8, 8.55, 
            12.5, 17.6, 18.5, 2.98),
        cutip_gsf = cms.vdouble(0.0551, 0.0765, 0.143, 0.0874, 0.594, 
            0.37, 0.0913, 1.15, 0.231),
        cutip_gsfl = cms.vdouble(0.0186, 0.0759, 0.138, 0.0473, 0.62, 
            0.304, 0.109, 0.775, 0.0479),
        cuthoe = cms.vdouble(0.247, 0.137, 0.147, 0.371, 0.0588, 
            0.147, 0.52, 0.452, 0.404),
        cutiso_sumoetl = cms.vdouble(11.3, 9.05, 9.07, 9.94, 5.25, 
            6.15, 10.7, 10.8, 4.4),
        cutfmishits = cms.vdouble(4.5, 1.5, 1.5, 2.5, 2.5, 
            1.5, 4.5, 3.5, 3.5),
        cuthoel = cms.vdouble(0.236, 0.126, 0.147, 0.375, 0.0392, 
            0.145, 0.365, 0.383, 0.384),
        cutdphiin = cms.vdouble(0.0897, 0.262, 0.353, 0.116, 0.357, 
            0.319, 0.342, 0.404, 0.336),
        cutseel = cms.vdouble(0.0164, 0.0118, 0.015, 0.0523, 0.0326, 
            0.0456, 0.0185, 0.0589, 0.0544),
        cutiso_sumoet = cms.vdouble(34.5, 12.7, 12.1, 19.9, 6.35, 
            8.85, 14.0, 10.5, 9.74),
        cutdcotdist = cms.vdouble(9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0),
        cutsee = cms.vdouble(0.0176, 0.0125, 0.0181, 0.0415, 0.0364, 
            0.0418, 0.0146, 0.0678, 0.133),
        cuteseedopcor = cms.vdouble(0.63, 0.82, 0.401, 0.718, 0.4, 
            0.458, 0.15, 0.664, 0.373),
        cutdphiinl = cms.vdouble(0.0747, 0.25, 0.356, 0.0956, 0.347, 
            0.326, 0.333, 0.647, 0.289),
        cutdetainl = cms.vdouble(0.0124, 0.00503, 0.0257, 0.0228, 0.0118, 
            0.0178, 0.0188, 0.14, 0.024)
    ),
    reducedBarrelRecHitCollection = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
    robusthighenergyEleIDCutsV04 = cms.PSet(
        barrel = cms.vdouble(0.05, 9999, 0.09, 0.005, 0.94, 
            0.83, 7.5, 2, 0.03, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.05, 0.03, 0.09, 0.007, -1, 
            -1, 15, 2.5, 0.03, 2.5, 
            0, 0.5, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robusthighenergyEleIDCutsV01 = cms.PSet(
        barrel = cms.vdouble(0.05, 9999, 0.09, 0.005, 0.94, 
            0.83, 9999.0, 9999.0, 0, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.05, 0.0275, 0.09, 0.007, -1, 
            -1, 9999.0, 9999.0, 0, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robusthighenergyEleIDCutsV00 = cms.PSet(
        barrel = cms.vdouble(0.05, 0.011, 0.09, 0.005, -1, 
            -1, 9999.0, 9999.0, 0, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.1, 0.0275, 0.09, 0.007, -1, 
            -1, 9999.0, 9999.0, 0, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robusthighenergyEleIDCutsV03 = cms.PSet(
        barrel = cms.vdouble(0.05, 9999, 0.09, 0.005, 0.94, 
            0.83, 7.5, 2, 0.03, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.05, 0.03, 0.09, 0.007, -1, 
            -1, 15, 2.5, 0.03, 2.5, 
            0, 0.5, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robusthighenergyEleIDCutsV02 = cms.PSet(
        barrel = cms.vdouble(0.05, 9999, 0.09, 0.005, 0.94, 
            0.83, 7.5, 2, 0.03, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.05, 0.03, 0.09, 0.007, -1, 
            -1, 15, 2.5, 0.03, 2.5, 
            0, 0.5, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    classbasedlooseEleIDCutsV00 = cms.PSet(
        deltaPhiIn = cms.vdouble(0.05, 0.025, 0.053, 0.09, 0.07, 
            0.03, 0.092, 0.092),
        hOverE = cms.vdouble(0.115, 0.1, 0.055, 0.0, 0.145, 
            0.12, 0.15, 0.0),
        sigmaEtaEta = cms.vdouble(0.014, 0.012, 0.0115, 0.0, 0.0275, 
            0.0265, 0.0265, 0.0),
        deltaEtaIn = cms.vdouble(0.009, 0.0045, 0.0085, 0.0, 0.0105, 
            0.0068, 0.01, 0.0),
        eSeedOverPin = cms.vdouble(0.11, 0.91, 0.11, 0.0, 0.0, 
            0.85, 0.0, 0.0)
    ),
    classbasedlooseEleIDCutsV01 = cms.PSet(
        deltaPhiIn = cms.vdouble(0.053, 0.0189, 0.059, 0.099, 0.0278, 
            0.0157, 0.042, 0.08),
        hOverE = cms.vdouble(0.076, 0.033, 0.07, 0.0, 0.083, 
            0.0148, 0.033, 0.0),
        sigmaEtaEta = cms.vdouble(0.0101, 0.0095, 0.0097, 0.0, 0.0271, 
            0.0267, 0.0259, 0.0),
        deltaEtaIn = cms.vdouble(0.0078, 0.00259, 0.0062, 0.0, 0.0078, 
            0.0061, 0.0061, 0.0),
        eSeedOverPin = cms.vdouble(0.3, 0.92, 0.211, 0.0, 0.42, 
            0.88, 0.68, 0.0)
    ),
    classbasedlooseEleIDCutsV02 = cms.PSet(
        cutisohcal = cms.vdouble(13.5, 9.93, 7.56, 14.8, 8.1, 
            10.8, 42.7, 20.1, 9.11, 10.4, 
            6.89, 5.59, 8.53, 9.59, 24.2, 
            2.78, 8.67, 0.288),
        cutmishits = cms.vdouble(5.5, 1.5, 5.5, 2.5, 2.5, 
            2.5, 3.5, 5.5, 0.5, 1.5, 
            2.5, 0.5, 1.5, 1.5, 0.5, 
            0.5, 0.5, 0.5),
        cuthoe = cms.vdouble(0.0887, 0.0934, 0.0949, 0.0986, 0.0431, 
            0.0878, 0.097, 0.0509, 0.098, 0.0991, 
            0.0321, 0.0928, 0.0663, 0.0717, 0.0966, 
            0.0758, 0.0149, 0.0131),
        cutdeta = cms.vdouble(0.00958, 0.00406, 0.0122, 0.0137, 0.00837, 
            0.0127, 0.011, 0.00336, 0.00977, 0.015, 
            0.00675, 0.0109, 0.014, 0.00508, 0.0109, 
            0.0146, 0.00506, 0.0127),
        cuteopin = cms.vdouble(0.878, 0.802, 0.814, 0.942, 0.735, 
            0.774, 0.829, 0.909, 0.829, 0.813, 
            0.86, 0.897, 0.817, 0.831, 0.818, 
            0.861, 0.787, 0.789),
        cutip = cms.vdouble(0.0246, 0.076, 0.0966, 0.0885, 0.441, 
            0.205, 0.0292, 0.0293, 0.0619, 0.0251, 
            0.159, 0.0815, 7.29, 0.0106, 5.76, 
            6.89, 1.27, 5.89),
        cutisotk = cms.vdouble(24.3, 8.45, 14.4, 27.8, 6.02, 
            10.5, 14.1, 10.2, 14.5, 19.1, 
            6.1, 14.1, 8.59, 8.33, 8.3, 
            8.93, 8.6, 16.0),
        cutsee = cms.vdouble(0.0172, 0.0115, 0.0143, 0.0344, 0.0295, 
            0.0304, 0.0145, 0.0108, 0.0128, 0.0347, 
            0.0307, 0.0316, 0.018, 0.011, 0.0132, 
            0.0349, 0.031, 0.0327),
        cutdphi = cms.vdouble(0.0372, 0.114, 0.118, 0.0488, 0.117, 
            0.119, 0.0606, 0.0548, 0.117, 0.07, 
            0.0355, 0.117, 0.088, 0.045, 0.118, 
            0.0919, 0.0236, 0.0515),
        cutisoecal = cms.vdouble(33.4, 28.1, 7.32, 27.4, 7.33, 
            21.7, 93.8, 102.0, 12.1, 26.0, 
            8.91, 10.0, 16.1, 31.3, 16.9, 
            15.4, 13.3, 37.7)
    ),
    classbasedlooseEleIDCutsV03 = cms.PSet(
        cutdetain = cms.vdouble(0.00989, 0.00484, 0.0146, 0.0146, 0.00902, 
            0.0172, 0.0137, 0.0477, 0.0275, 0.00967, 
            0.00377, 0.00924, 0.013, 0.00666, 0.0123, 
            0.0125, 0.0228, 0.0112, 0.0106, 0.0038, 
            0.00897, 0.0139, 0.00667, 0.0122, 0.0122, 
            0.0193, 0.00239),
        cutiso_sum = cms.vdouble(31.5, 10.3, 8.8, 11.0, 6.13, 
            6.94, 7.52, 9.0, 3.5, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0),
        cutip_gsf = cms.vdouble(0.0431, 0.0767, 0.139, 0.101, 0.149, 
            0.154, 0.932, 0.15, 0.124, 0.0238, 
            0.0467, 0.0759, 0.0369, 0.147, 0.0986, 
            0.0626, 0.195, 0.116, 0.0122, 0.0125, 
            0.0693, 0.0162, 0.089, 0.0673, 0.0467, 
            0.0651, 0.0221),
        cuthoe = cms.vdouble(0.166, 0.0771, 0.144, 0.37, 0.0497, 
            0.139, 0.401, 2.68, 0.516, 0.234, 
            0.0556, 0.144, 0.368, 0.031, 0.12, 
            0.602, 2.01, 1.05, 0.104, 0.063, 
            0.0565, 0.38, 0.0192, 0.0294, 0.537, 
            4.65, 1.87),
        cutfmishits = cms.vdouble(4.5, 1.5, 1.5, 2.5, 2.5, 
            1.5, 2.5, 2.5, 1.5, 2.5, 
            1.5, 1.5, 1.5, 1.5, 0.5, 
            2.5, 2.5, 0.5, 2.5, 1.5, 
            0.5, 1.5, 1.5, 0.5, 2.5, 
            0.5, 0.5),
        cutiso_sumoet = cms.vdouble(28.9, 15.3, 12.0, 18.3, 7.17, 
            9.42, 11.0, 9.81, 3.94, 22.7, 
            15.9, 12.3, 17.0, 7.58, 8.89, 
            15.2, 12.7, 6.17, 20.8, 21.2, 
            17.2, 15.5, 9.37, 10.6, 19.8, 
            22.1, 15.6),
        cutdcotdist = cms.vdouble(0.0393, 0.0392, 0.0397, 0.0394, 0.0393, 
            0.039, 0.0378, 0.0388, 0.0382, 0.0385, 
            0.0167, 0.00325, 0.0394, 0.0387, 0.0388, 
            0.0227, 0.0258, 0.0127, 0.0298, 0.03, 
            0.00946, 0.039, 0.0231, 0.0278, 0.00162, 
            0.0367, 0.0199),
        cutsee = cms.vdouble(0.0175, 0.0127, 0.0177, 0.0373, 0.0314, 
            0.0329, 0.0157, 0.0409, 0.14, 0.0169, 
            0.0106, 0.0142, 0.0363, 0.0322, 0.0354, 
            0.0117, 0.0372, 28.2, 0.0171, 0.0113, 
            0.014, 0.0403, 0.0323, 0.0411, 0.0104, 
            0.0436, 0.0114),
        cuteseedopcor = cms.vdouble(0.78, 0.302, 0.483, 0.904, 0.168, 
            0.645, 0.108, 0.284, 0.324, 0.591, 
            0.286, 0.488, 0.813, 0.791, 0.672, 
            0.398, 0.834, 0.878, 0.515, 0.937, 
            0.806, 0.816, 0.85, 0.507, 0.367, 
            0.83, 0.648),
        cutdphiin = cms.vdouble(0.041, 0.275, 0.365, 0.047, 0.273, 
            0.296, 0.329, 0.465, 0.627, 0.0581, 
            0.0954, 0.327, 0.0702, 0.0582, 0.279, 
            0.117, 0.318, 0.246, 0.0821, 0.052, 
            0.292, 0.116, 0.0435, 0.312, 0.118, 
            0.296, 0.0459),
        cutet = cms.vdouble(-100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, 12.0, 12.0, 
            12.0, 12.0, 12.0, 12.0, 12.0, 
            12.0, 12.5)
    ),
    classbasedlooseEleIDCutsV04 = cms.PSet(
        cutdetain = cms.vdouble(0.00989, 0.00484, 0.0146, 0.0146, 0.00902, 
            0.0172, 0.0137, 0.0477, 0.0275, 0.00967, 
            0.00377, 0.00924, 0.013, 0.00666, 0.0123, 
            0.0125, 0.0228, 0.0112, 0.0106, 0.0038, 
            0.00897, 0.0139, 0.00667, 0.0122, 0.0122, 
            0.0193, 0.00239),
        cutiso_sum = cms.vdouble(31.5, 10.3, 8.8, 11.0, 6.13, 
            6.94, 7.52, 9.0, 3.5, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0),
        cutip_gsf = cms.vdouble(0.0431, 0.0767, 0.139, 0.101, 0.149, 
            0.154, 0.932, 0.15, 0.124, 0.0238, 
            0.0467, 0.0759, 0.0369, 0.147, 0.0986, 
            0.0626, 0.195, 0.116, 0.0122, 0.0125, 
            0.0693, 0.0162, 0.089, 0.0673, 0.0467, 
            0.0651, 0.0221),
        cuthoe = cms.vdouble(0.166, 0.0771, 0.144, 0.37, 0.0497, 
            0.139, 0.401, 2.68, 0.516, 0.234, 
            0.0556, 0.144, 0.368, 0.031, 0.12, 
            0.602, 2.01, 1.05, 0.104, 0.063, 
            0.0565, 0.38, 0.0192, 0.0294, 0.537, 
            4.65, 1.87),
        cutfmishits = cms.vdouble(4.5, 1.5, 1.5, 2.5, 2.5, 
            1.5, 2.5, 2.5, 1.5, 2.5, 
            1.5, 1.5, 1.5, 1.5, 0.5, 
            2.5, 2.5, 0.5, 2.5, 1.5, 
            0.5, 1.5, 1.5, 0.5, 2.5, 
            0.5, 0.5),
        cutiso_sumoet = cms.vdouble(28.9, 15.3, 12.0, 18.3, 7.17, 
            9.42, 11.0, 9.81, 3.94, 22.7, 
            15.9, 12.3, 17.0, 7.58, 8.89, 
            15.2, 12.7, 6.17, 20.8, 21.2, 
            17.2, 15.5, 9.37, 10.6, 19.8, 
            22.1, 15.6),
        cutdcotdist = cms.vdouble(0.0393, 0.0392, 0.0397, 0.0394, 0.0393, 
            0.039, 0.0378, 0.0388, 0.0382, 0.0385, 
            0.0167, 0.00325, 0.0394, 0.0387, 0.0388, 
            0.0227, 0.0258, 0.0127, 0.0298, 0.03, 
            0.00946, 0.039, 0.0231, 0.0278, 0.00162, 
            0.0367, 0.0199),
        cutsee = cms.vdouble(0.0175, 0.0127, 0.0177, 0.0373, 0.0314, 
            0.0329, 0.0157, 0.0409, 0.14, 0.0169, 
            0.0106, 0.0142, 0.0363, 0.0322, 0.0354, 
            0.0117, 0.0372, 28.2, 0.0171, 0.0113, 
            0.014, 0.0403, 0.0323, 0.0411, 0.0104, 
            0.0436, 0.0114),
        cuteseedopcor = cms.vdouble(0.78, 0.302, 0.483, 0.904, 0.168, 
            0.645, 0.108, 0.284, 0.324, 0.591, 
            0.286, 0.488, 0.813, 0.791, 0.672, 
            0.398, 0.834, 0.878, 0.515, 0.937, 
            0.806, 0.816, 0.85, 0.507, 0.367, 
            0.83, 0.648),
        cutdphiin = cms.vdouble(0.041, 0.275, 0.365, 0.047, 0.273, 
            0.296, 0.329, 0.465, 0.627, 0.0581, 
            0.0954, 0.327, 0.0702, 0.0582, 0.279, 
            0.117, 0.318, 0.246, 0.0821, 0.052, 
            0.292, 0.116, 0.0435, 0.312, 0.118, 
            0.296, 0.0459),
        cutet = cms.vdouble(-100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, 12.0, 12.0, 
            12.0, 12.0, 12.0, 12.0, 12.0, 
            12.0, 12.5)
    ),
    classbasedlooseEleIDCutsV06 = cms.PSet(
        cutdetain = cms.vdouble(0.0137, 0.00678, 0.0241, 0.0187, 0.0161, 
            0.0224, 0.0252, 0.0308, 0.0273),
        cutiso_sum = cms.vdouble(33.0, 17.0, 17.9, 18.8, 8.55, 
            12.5, 17.6, 18.5, 2.98),
        cutip_gsf = cms.vdouble(0.0551, 0.0765, 0.143, 0.0874, 0.594, 
            0.37, 0.0913, 1.15, 0.231),
        cutip_gsfl = cms.vdouble(0.0186, 0.0759, 0.138, 0.0473, 0.62, 
            0.304, 0.109, 0.775, 0.0479),
        cuthoe = cms.vdouble(0.247, 0.137, 0.147, 0.371, 0.0588, 
            0.147, 0.52, 0.452, 0.404),
        cutiso_sumoetl = cms.vdouble(11.3, 9.05, 9.07, 9.94, 5.25, 
            6.15, 10.7, 10.8, 4.4),
        cutfmishits = cms.vdouble(4.5, 1.5, 1.5, 2.5, 2.5, 
            1.5, 4.5, 3.5, 3.5),
        cuthoel = cms.vdouble(0.236, 0.126, 0.147, 0.375, 0.0392, 
            0.145, 0.365, 0.383, 0.384),
        cutdphiin = cms.vdouble(0.0897, 0.262, 0.353, 0.116, 0.357, 
            0.319, 0.342, 0.404, 0.336),
        cutseel = cms.vdouble(0.0164, 0.0118, 0.015, 0.0523, 0.0326, 
            0.0456, 0.0185, 0.0589, 0.0544),
        cutiso_sumoet = cms.vdouble(34.5, 12.7, 12.1, 19.9, 6.35, 
            8.85, 14.0, 10.5, 9.74),
        cutdcotdist = cms.vdouble(9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0),
        cutsee = cms.vdouble(0.0176, 0.0125, 0.0181, 0.0415, 0.0364, 
            0.0418, 0.0146, 0.0678, 0.133),
        cuteseedopcor = cms.vdouble(0.63, 0.82, 0.401, 0.718, 0.4, 
            0.458, 0.15, 0.664, 0.373),
        cutdphiinl = cms.vdouble(0.0747, 0.25, 0.356, 0.0956, 0.347, 
            0.326, 0.333, 0.647, 0.289),
        cutdetainl = cms.vdouble(0.0124, 0.00503, 0.0257, 0.0228, 0.0118, 
            0.0178, 0.0188, 0.14, 0.024)
    ),
    src = cms.InputTag("gsfElectrons"),
    robusttightEleIDCuts = cms.PSet(
        barrel = cms.vdouble(0.0201, 0.0102, 0.0211, 0.00606, -1, 
            -1, 2.34, 3.24, 4.51, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.00253, 0.0291, 0.022, 0.0032, -1, 
            -1, 0.826, 2.7, 0.255, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    classbasedtightEleIDCuts = cms.PSet(
        cutdetain = cms.vdouble(0.0116, 0.00449, 0.00938, 0.0184, 0.00678, 
            0.0109, 0.0252, 0.0268, 0.0139),
        cutiso_sum = cms.vdouble(15.5, 12.2, 12.2, 11.7, 7.16, 
            9.71, 8.66, 11.9, 2.98),
        cutip_gsf = cms.vdouble(0.0131, 0.0586, 0.0839, 0.0366, 0.452, 
            0.204, 0.0913, 0.0802, 0.0731),
        cutip_gsfl = cms.vdouble(0.0119, 0.0527, 0.0471, 0.0212, 0.233, 
            0.267, 0.109, 0.122, 0.0479),
        cuthoe = cms.vdouble(0.215, 0.0608, 0.147, 0.369, 0.0349, 
            0.102, 0.52, 0.422, 0.404),
        cutiso_sumoetl = cms.vdouble(6.21, 6.81, 5.3, 5.39, 2.73, 
            4.73, 4.84, 3.46, 3.73),
        cutfmishits = cms.vdouble(1.5, 1.5, 1.5, 2.5, 2.5, 
            1.5, 1.5, 2.5, 0.5),
        cuthoel = cms.vdouble(0.228, 0.0836, 0.143, 0.37, 0.0392, 
            0.0979, 0.3, 0.381, 0.339),
        cutdphiin = cms.vdouble(0.0897, 0.0993, 0.295, 0.0979, 0.151, 
            0.252, 0.341, 0.308, 0.328),
        cutseel = cms.vdouble(0.0132, 0.0117, 0.0112, 0.0387, 0.0281, 
            0.0287, 0.00987, 0.0296, 0.0544),
        cutiso_sumoet = cms.vdouble(11.9, 7.81, 6.28, 8.92, 4.65, 
            5.49, 9.36, 8.84, 5.94),
        cutdcotdist = cms.vdouble(9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0),
        cutsee = cms.vdouble(0.0145, 0.0116, 0.012, 0.039, 0.0297, 
            0.0311, 0.00987, 0.0347, 0.0917),
        cuteseedopcor = cms.vdouble(0.637, 0.943, 0.742, 0.748, 0.763, 
            0.631, 0.214, 0.873, 0.473),
        cutdphiinl = cms.vdouble(0.061, 0.14, 0.286, 0.0921, 0.197, 
            0.24, 0.333, 0.303, 0.258),
        cutdetainl = cms.vdouble(0.00816, 0.00401, 0.0081, 0.019, 0.00588, 
            0.00893, 0.0171, 0.0434, 0.0143)
    ),
    algorithm = cms.string('eIDCB'),
    robusthighenergyEleIDCuts = cms.PSet(
        barrel = cms.vdouble(0.05, 9999, 0.09, 0.005, 0.94, 
            0.83, 7.5, 2, 0.03, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.05, 0.03, 0.09, 0.007, -1, 
            -1, 15, 2.5, 0.03, 2.5, 
            0, 0.5, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robustlooseEleIDCuts = cms.PSet(
        barrel = cms.vdouble(0.05, 0.0103, 0.8, 0.00688, -1, 
            -1, 7.33, 4.68, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.0389, 0.0307, 0.7, 0.00944, -1, 
            -1, 7.76, 3.09, 2.23, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robustlooseEleIDCutsV02 = cms.PSet(
        barrel = cms.vdouble(0.05, 0.0103, 0.8, 0.00688, -1, 
            -1, 7.33, 4.68, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.0389, 0.0307, 0.7, 0.00944, -1, 
            -1, 7.76, 3.09, 2.23, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robustlooseEleIDCutsV03 = cms.PSet(
        barrel = cms.vdouble(0.05, 0.0103, 0.8, 0.00688, -1, 
            -1, 7.33, 4.68, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.0389, 0.0307, 0.7, 0.00944, -1, 
            -1, 7.76, 3.09, 2.23, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robustlooseEleIDCutsV00 = cms.PSet(
        barrel = cms.vdouble(0.115, 0.014, 0.09, 0.009, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.15, 0.0275, 0.092, 0.0105, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robustlooseEleIDCutsV01 = cms.PSet(
        barrel = cms.vdouble(0.075, 0.0132, 0.058, 0.0077, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.083, 0.027, 0.042, 0.01, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robustlooseEleIDCutsV04 = cms.PSet(
        barrel = cms.vdouble(0.05, 0.0103, 0.8, 0.00688, -1, 
            -1, 7.33, 4.68, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.0389, 0.0307, 0.7, 0.00944, -1, 
            -1, 7.76, 3.09, 2.23, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    additionalCategories = cms.bool(True),
    etBinning = cms.bool(True)
)


process.eidLoose = cms.EDProducer("EleIdCutBasedExtProducer",
    electronQuality = cms.string('loose'),
    classbasedtightEleIDCutsV02 = cms.PSet(
        cutisohcal = cms.vdouble(10.9, 7.01, 8.75, 3.51, 7.75, 
            1.62, 11.6, 9.9, 4.97, 5.33, 
            3.18, 2.32, 0.164, 5.46, 12.0, 
            0.00604, 4.1, 0.000628),
        cutmishits = cms.vdouble(5.5, 1.5, 0.5, 1.5, 2.5, 
            0.5, 3.5, 5.5, 0.5, 0.5, 
            0.5, 0.5, 0.5, 1.5, 0.5, 
            0.5, 0.5, 0.5),
        cuthoe = cms.vdouble(0.0871, 0.0289, 0.0783, 0.0946, 0.0245, 
            0.0363, 0.0671, 0.048, 0.0614, 0.0924, 
            0.0158, 0.049, 0.0382, 0.0915, 0.0451, 
            0.0452, 0.00196, 0.0043),
        cutdeta = cms.vdouble(0.00915, 0.00302, 0.0061, 0.0135, 0.00565, 
            0.00793, 0.0102, 0.00266, 0.0106, 0.00903, 
            0.00766, 0.00723, 0.0116, 0.00203, 0.00659, 
            0.0148, 0.00555, 0.0128),
        cuteopin = cms.vdouble(0.878, 0.859, 0.874, 0.944, 0.737, 
            0.773, 0.86, 0.967, 0.917, 0.812, 
            0.915, 1.01, 0.847, 0.953, 0.979, 
            0.841, 0.771, 1.09),
        cutip = cms.vdouble(0.0239, 0.027, 0.0768, 0.0231, 0.178, 
            0.0957, 0.0102, 0.0168, 0.043, 0.0166, 
            0.0594, 0.0308, 2.1, 0.00527, 3.17, 
            4.91, 0.769, 5.9),
        cutisotk = cms.vdouble(6.53, 4.6, 6.0, 8.63, 3.11, 
            7.77, 5.42, 4.81, 4.06, 6.47, 
            2.8, 3.45, 5.29, 5.18, 15.4, 
            5.38, 4.47, 0.0347),
        cutsee = cms.vdouble(0.0131, 0.0106, 0.0115, 0.0306, 0.028, 
            0.0293, 0.0131, 0.0106, 0.0115, 0.0317, 
            0.029, 0.0289, 0.0142, 0.0106, 0.0103, 
            0.035, 0.0296, 0.0333),
        cutdphi = cms.vdouble(0.0369, 0.0307, 0.117, 0.0475, 0.0216, 
            0.117, 0.0372, 0.0246, 0.0426, 0.0612, 
            0.0142, 0.039, 0.0737, 0.0566, 0.0359, 
            0.0187, 0.012, 0.0358),
        cutisoecal = cms.vdouble(20.0, 27.2, 4.48, 13.5, 4.56, 
            3.19, 12.2, 13.1, 7.42, 7.67, 
            4.12, 4.85, 10.1, 12.4, 11.1, 
            11.0, 10.6, 13.4)
    ),
    classbasedtightEleIDCutsV03 = cms.PSet(
        cutdetain = cms.vdouble(0.00811, 0.00341, 0.00633, 0.0103, 0.00667, 
            0.01, 0.0106, 0.0145, 0.0163, 0.0076, 
            0.00259, 0.00511, 0.00941, 0.0043, 0.00857, 
            0.012, 0.0169, 0.00172, 0.00861, 0.00362, 
            0.00601, 0.00925, 0.00489, 0.00832, 0.0119, 
            0.0169, 0.000996),
        cutiso_sum = cms.vdouble(11.8, 8.31, 6.26, 6.18, 3.28, 
            4.38, 4.17, 5.4, 1.57, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0),
        cutip_gsf = cms.vdouble(0.0213, 0.0422, 0.0632, 0.0361, 0.073, 
            0.126, 0.171, 0.119, 0.0372, 0.0131, 
            0.0146, 0.0564, 0.0152, 0.0222, 0.0268, 
            0.0314, 0.0884, 0.00374, 0.00852, 0.00761, 
            0.0143, 0.0106, 0.0127, 0.0119, 0.0123, 
            0.0235, 0.00363),
        cuthoe = cms.vdouble(0.0783, 0.0387, 0.105, 0.118, 0.0227, 
            0.062, 0.13, 2.47, 0.38, 0.0888, 
            0.0503, 0.0955, 0.0741, 0.015, 0.03, 
            0.589, 1.13, 0.612, 0.0494, 0.0461, 
            0.0292, 0.0369, 0.0113, 0.0145, 0.124, 
            2.05, 0.61),
        cutfmishits = cms.vdouble(2.5, 1.5, 1.5, 1.5, 1.5, 
            0.5, 2.5, 0.5, 0.5, 2.5, 
            1.5, 0.5, 0.5, 0.5, 0.5, 
            0.5, 0.5, -0.5, 2.5, 1.5, 
            0.5, 0.5, 0.5, 0.5, 0.5, 
            0.5, 0.5),
        cutiso_sumoet = cms.vdouble(13.7, 11.6, 7.14, 9.98, 3.52, 
            4.87, 6.24, 7.96, 2.53, 11.2, 
            11.9, 7.88, 8.16, 5.58, 5.03, 
            11.4, 8.15, 5.79, 10.4, 11.1, 
            10.4, 7.47, 5.08, 5.9, 11.8, 
            14.1, 11.7),
        cutdcotdist = cms.vdouble(0.0393, 0.0256, 0.00691, 0.0394, 0.0386, 
            0.039, 0.0325, 0.0384, 0.0382, 0.0245, 
            0.000281, 5.46e-05, 0.0342, 0.0232, 0.00107, 
            0.0178, 0.0193, 0.000758, 0.000108, 0.0248, 
            0.000458, 0.0129, 0.00119, 0.0182, 4.53e-05, 
            0.0189, 0.000928),
        cutsee = cms.vdouble(0.0143, 0.0105, 0.0123, 0.0324, 0.0307, 
            0.0301, 0.0109, 0.027, 0.0292, 0.0133, 
            0.0104, 0.0116, 0.0332, 0.0296, 0.031, 
            0.00981, 0.0307, 0.072, 0.0149, 0.0105, 
            0.011, 0.0342, 0.0307, 0.0303, 0.00954, 
            0.0265, 0.0101),
        cuteseedopcor = cms.vdouble(0.784, 0.366, 0.57, 0.911, 0.298, 
            0.645, 0.51, 0.497, 0.932, 0.835, 
            0.968, 0.969, 0.923, 0.898, 0.98, 
            0.63, 0.971, 1.0, 0.515, 0.963, 
            0.986, 0.823, 0.879, 1.01, 0.931, 
            0.937, 1.05),
        cutdphiin = cms.vdouble(0.0404, 0.0499, 0.263, 0.042, 0.0484, 
            0.241, 0.242, 0.231, 0.286, 0.0552, 
            0.0338, 0.154, 0.0623, 0.0183, 0.0392, 
            0.0547, 0.0588, 0.00654, 0.042, 0.0217, 
            0.0885, 0.0445, 0.0141, 0.0234, 0.065, 
            0.0258, 0.0346),
        cutet = cms.vdouble(-100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, 13.7, 13.2, 
            13.6, 14.2, 14.1, 13.9, 12.9, 
            14.9, 17.7)
    ),
    classbasedtightEleIDCutsV00 = cms.PSet(
        deltaPhiIn = cms.vdouble(0.032, 0.016, 0.0525, 0.09, 0.025, 
            0.035, 0.065, 0.092),
        hOverE = cms.vdouble(0.05, 0.042, 0.045, 0.0, 0.055, 
            0.037, 0.05, 0.0),
        sigmaEtaEta = cms.vdouble(0.0125, 0.011, 0.01, 0.0, 0.0265, 
            0.0252, 0.026, 0.0),
        deltaEtaIn = cms.vdouble(0.0055, 0.003, 0.0065, 0.0, 0.006, 
            0.0055, 0.0075, 0.0),
        eSeedOverPin = cms.vdouble(0.24, 0.94, 0.11, 0.0, 0.32, 
            0.83, 0.0, 0.0)
    ),
    classbasedtightEleIDCutsV01 = cms.PSet(
        deltaPhiIn = cms.vdouble(0.0225, 0.0114, 0.0234, 0.039, 0.0215, 
            0.0095, 0.0148, 0.0167),
        hOverE = cms.vdouble(0.056, 0.0221, 0.037, 0.0, 0.0268, 
            0.0102, 0.0104, 0.0),
        sigmaEtaEta = cms.vdouble(0.0095, 0.0094, 0.0094, 0.0, 0.026, 
            0.0257, 0.0246, 0.0),
        deltaEtaIn = cms.vdouble(0.0043, 0.00282, 0.0036, 0.0, 0.0066, 
            0.0049, 0.0041, 0.0),
        eSeedOverPin = cms.vdouble(0.32, 0.94, 0.221, 0.0, 0.74, 
            0.89, 0.66, 0.0)
    ),
    classbasedtightEleIDCutsV06 = cms.PSet(
        cutdetain = cms.vdouble(0.0116, 0.00449, 0.00938, 0.0184, 0.00678, 
            0.0109, 0.0252, 0.0268, 0.0139),
        cutiso_sum = cms.vdouble(15.5, 12.2, 12.2, 11.7, 7.16, 
            9.71, 8.66, 11.9, 2.98),
        cutip_gsf = cms.vdouble(0.0131, 0.0586, 0.0839, 0.0366, 0.452, 
            0.204, 0.0913, 0.0802, 0.0731),
        cutip_gsfl = cms.vdouble(0.0119, 0.0527, 0.0471, 0.0212, 0.233, 
            0.267, 0.109, 0.122, 0.0479),
        cuthoe = cms.vdouble(0.215, 0.0608, 0.147, 0.369, 0.0349, 
            0.102, 0.52, 0.422, 0.404),
        cutiso_sumoetl = cms.vdouble(6.21, 6.81, 5.3, 5.39, 2.73, 
            4.73, 4.84, 3.46, 3.73),
        cutfmishits = cms.vdouble(1.5, 1.5, 1.5, 2.5, 2.5, 
            1.5, 1.5, 2.5, 0.5),
        cuthoel = cms.vdouble(0.228, 0.0836, 0.143, 0.37, 0.0392, 
            0.0979, 0.3, 0.381, 0.339),
        cutdphiin = cms.vdouble(0.0897, 0.0993, 0.295, 0.0979, 0.151, 
            0.252, 0.341, 0.308, 0.328),
        cutseel = cms.vdouble(0.0132, 0.0117, 0.0112, 0.0387, 0.0281, 
            0.0287, 0.00987, 0.0296, 0.0544),
        cutiso_sumoet = cms.vdouble(11.9, 7.81, 6.28, 8.92, 4.65, 
            5.49, 9.36, 8.84, 5.94),
        cutdcotdist = cms.vdouble(9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0),
        cutsee = cms.vdouble(0.0145, 0.0116, 0.012, 0.039, 0.0297, 
            0.0311, 0.00987, 0.0347, 0.0917),
        cuteseedopcor = cms.vdouble(0.637, 0.943, 0.742, 0.748, 0.763, 
            0.631, 0.214, 0.873, 0.473),
        cutdphiinl = cms.vdouble(0.061, 0.14, 0.286, 0.0921, 0.197, 
            0.24, 0.333, 0.303, 0.258),
        cutdetainl = cms.vdouble(0.00816, 0.00401, 0.0081, 0.019, 0.00588, 
            0.00893, 0.0171, 0.0434, 0.0143)
    ),
    classbasedtightEleIDCutsV04 = cms.PSet(
        cutdetain = cms.vdouble(0.00811, 0.00341, 0.00633, 0.0103, 0.00667, 
            0.01, 0.0106, 0.0145, 0.0163, 0.0076, 
            0.00259, 0.00511, 0.00941, 0.0043, 0.00857, 
            0.012, 0.0169, 0.00172, 0.00861, 0.00362, 
            0.00601, 0.00925, 0.00489, 0.00832, 0.0119, 
            0.0169, 0.000996),
        cutiso_sum = cms.vdouble(11.8, 8.31, 6.26, 6.18, 3.28, 
            4.38, 4.17, 5.4, 1.57, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0),
        cutip_gsf = cms.vdouble(0.0213, 0.0422, 0.0632, 0.0361, 0.073, 
            0.126, 0.171, 0.119, 0.0372, 0.0131, 
            0.0146, 0.0564, 0.0152, 0.0222, 0.0268, 
            0.0314, 0.0884, 0.00374, 0.00852, 0.00761, 
            0.0143, 0.0106, 0.0127, 0.0119, 0.0123, 
            0.0235, 0.00363),
        cuthoe = cms.vdouble(0.0783, 0.0387, 0.105, 0.118, 0.0227, 
            0.062, 0.13, 2.47, 0.38, 0.0888, 
            0.0503, 0.0955, 0.0741, 0.015, 0.03, 
            0.589, 1.13, 0.612, 0.0494, 0.0461, 
            0.0292, 0.0369, 0.0113, 0.0145, 0.124, 
            2.05, 0.61),
        cutfmishits = cms.vdouble(2.5, 1.5, 1.5, 1.5, 1.5, 
            0.5, 2.5, 0.5, 0.5, 2.5, 
            1.5, 0.5, 0.5, 0.5, 0.5, 
            0.5, 0.5, -0.5, 2.5, 1.5, 
            0.5, 0.5, 0.5, 0.5, 0.5, 
            0.5, 0.5),
        cutiso_sumoet = cms.vdouble(13.7, 11.6, 7.14, 9.98, 3.52, 
            4.87, 6.24, 7.96, 2.53, 11.2, 
            11.9, 7.88, 8.16, 5.58, 5.03, 
            11.4, 8.15, 5.79, 10.4, 11.1, 
            10.4, 7.47, 5.08, 5.9, 11.8, 
            14.1, 11.7),
        cutdcotdist = cms.vdouble(0.0393, 0.0256, 0.00691, 0.0394, 0.0386, 
            0.039, 0.0325, 0.0384, 0.0382, 0.0245, 
            0.000281, 5.46e-05, 0.0342, 0.0232, 0.00107, 
            0.0178, 0.0193, 0.000758, 0.000108, 0.0248, 
            0.000458, 0.0129, 0.00119, 0.0182, 4.53e-05, 
            0.0189, 0.000928),
        cutsee = cms.vdouble(0.0143, 0.0105, 0.0123, 0.0324, 0.0307, 
            0.0301, 0.0109, 0.027, 0.0292, 0.0133, 
            0.0104, 0.0116, 0.0332, 0.0296, 0.031, 
            0.00981, 0.0307, 0.072, 0.0149, 0.0105, 
            0.011, 0.0342, 0.0307, 0.0303, 0.00954, 
            0.0265, 0.0101),
        cuteseedopcor = cms.vdouble(0.784, 0.366, 0.57, 0.911, 0.298, 
            0.645, 0.51, 0.497, 0.932, 0.835, 
            0.968, 0.969, 0.923, 0.898, 0.98, 
            0.63, 0.971, 1.0, 0.515, 0.963, 
            0.986, 0.823, 0.879, 1.01, 0.931, 
            0.937, 1.05),
        cutdphiin = cms.vdouble(0.0404, 0.0499, 0.263, 0.042, 0.0484, 
            0.241, 0.242, 0.231, 0.286, 0.0552, 
            0.0338, 0.154, 0.0623, 0.0183, 0.0392, 
            0.0547, 0.0588, 0.00654, 0.042, 0.0217, 
            0.0885, 0.0445, 0.0141, 0.0234, 0.065, 
            0.0258, 0.0346),
        cutet = cms.vdouble(-100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, 13.7, 13.2, 
            13.6, 14.2, 14.1, 13.9, 12.9, 
            14.9, 17.7)
    ),
    electronIDType = cms.string('classbased'),
    robusttightEleIDCutsV04 = cms.PSet(
        barrel = cms.vdouble(0.0201, 0.0102, 0.0211, 0.00606, -1, 
            -1, 2.34, 3.24, 4.51, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.00253, 0.0291, 0.022, 0.0032, -1, 
            -1, 0.826, 2.7, 0.255, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    electronVersion = cms.string(''),
    robusttightEleIDCutsV00 = cms.PSet(
        barrel = cms.vdouble(0.015, 0.0092, 0.02, 0.0025, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.018, 0.025, 0.02, 0.004, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robusttightEleIDCutsV01 = cms.PSet(
        barrel = cms.vdouble(0.01, 0.0099, 0.025, 0.004, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.01, 0.028, 0.02, 0.0066, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    reducedBarrelRecHitCollection = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
    robusttightEleIDCutsV03 = cms.PSet(
        barrel = cms.vdouble(0.0201, 0.0102, 0.0211, 0.00606, -1, 
            -1, 2.34, 3.24, 4.51, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.00253, 0.0291, 0.022, 0.0032, -1, 
            -1, 0.826, 2.7, 0.255, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    reducedEndcapRecHitCollection = cms.InputTag("ecalRecHit","EcalRecHitsEE"),
    verticesCollection = cms.InputTag("offlinePrimaryVerticesWithBS"),
    classbasedlooseEleIDCuts = cms.PSet(
        cutdetain = cms.vdouble(0.0137, 0.00678, 0.0241, 0.0187, 0.0161, 
            0.0224, 0.0252, 0.0308, 0.0273),
        cutiso_sum = cms.vdouble(33.0, 17.0, 17.9, 18.8, 8.55, 
            12.5, 17.6, 18.5, 2.98),
        cutip_gsf = cms.vdouble(0.0551, 0.0765, 0.143, 0.0874, 0.594, 
            0.37, 0.0913, 1.15, 0.231),
        cutip_gsfl = cms.vdouble(0.0186, 0.0759, 0.138, 0.0473, 0.62, 
            0.304, 0.109, 0.775, 0.0479),
        cuthoe = cms.vdouble(0.247, 0.137, 0.147, 0.371, 0.0588, 
            0.147, 0.52, 0.452, 0.404),
        cutiso_sumoetl = cms.vdouble(11.3, 9.05, 9.07, 9.94, 5.25, 
            6.15, 10.7, 10.8, 4.4),
        cutfmishits = cms.vdouble(4.5, 1.5, 1.5, 2.5, 2.5, 
            1.5, 4.5, 3.5, 3.5),
        cuthoel = cms.vdouble(0.236, 0.126, 0.147, 0.375, 0.0392, 
            0.145, 0.365, 0.383, 0.384),
        cutdphiin = cms.vdouble(0.0897, 0.262, 0.353, 0.116, 0.357, 
            0.319, 0.342, 0.404, 0.336),
        cutseel = cms.vdouble(0.0164, 0.0118, 0.015, 0.0523, 0.0326, 
            0.0456, 0.0185, 0.0589, 0.0544),
        cutiso_sumoet = cms.vdouble(34.5, 12.7, 12.1, 19.9, 6.35, 
            8.85, 14.0, 10.5, 9.74),
        cutdcotdist = cms.vdouble(9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0),
        cutsee = cms.vdouble(0.0176, 0.0125, 0.0181, 0.0415, 0.0364, 
            0.0418, 0.0146, 0.0678, 0.133),
        cuteseedopcor = cms.vdouble(0.63, 0.82, 0.401, 0.718, 0.4, 
            0.458, 0.15, 0.664, 0.373),
        cutdphiinl = cms.vdouble(0.0747, 0.25, 0.356, 0.0956, 0.347, 
            0.326, 0.333, 0.647, 0.289),
        cutdetainl = cms.vdouble(0.0124, 0.00503, 0.0257, 0.0228, 0.0118, 
            0.0178, 0.0188, 0.14, 0.024)
    ),
    robusttightEleIDCutsV02 = cms.PSet(
        barrel = cms.vdouble(0.0201, 0.0102, 0.0211, 0.00606, -1, 
            -1, 2.34, 3.24, 4.51, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.00253, 0.0291, 0.022, 0.0032, -1, 
            -1, 0.826, 2.7, 0.255, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robusthighenergyEleIDCutsV04 = cms.PSet(
        barrel = cms.vdouble(0.05, 9999, 0.09, 0.005, 0.94, 
            0.83, 7.5, 2, 0.03, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.05, 0.03, 0.09, 0.007, -1, 
            -1, 15, 2.5, 0.03, 2.5, 
            0, 0.5, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robusthighenergyEleIDCutsV01 = cms.PSet(
        barrel = cms.vdouble(0.05, 9999, 0.09, 0.005, 0.94, 
            0.83, 9999.0, 9999.0, 0, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.05, 0.0275, 0.09, 0.007, -1, 
            -1, 9999.0, 9999.0, 0, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robusthighenergyEleIDCutsV00 = cms.PSet(
        barrel = cms.vdouble(0.05, 0.011, 0.09, 0.005, -1, 
            -1, 9999.0, 9999.0, 0, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.1, 0.0275, 0.09, 0.007, -1, 
            -1, 9999.0, 9999.0, 0, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robusthighenergyEleIDCutsV03 = cms.PSet(
        barrel = cms.vdouble(0.05, 9999, 0.09, 0.005, 0.94, 
            0.83, 7.5, 2, 0.03, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.05, 0.03, 0.09, 0.007, -1, 
            -1, 15, 2.5, 0.03, 2.5, 
            0, 0.5, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robusthighenergyEleIDCutsV02 = cms.PSet(
        barrel = cms.vdouble(0.05, 9999, 0.09, 0.005, 0.94, 
            0.83, 7.5, 2, 0.03, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.05, 0.03, 0.09, 0.007, -1, 
            -1, 15, 2.5, 0.03, 2.5, 
            0, 0.5, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    classbasedlooseEleIDCutsV00 = cms.PSet(
        deltaPhiIn = cms.vdouble(0.05, 0.025, 0.053, 0.09, 0.07, 
            0.03, 0.092, 0.092),
        hOverE = cms.vdouble(0.115, 0.1, 0.055, 0.0, 0.145, 
            0.12, 0.15, 0.0),
        sigmaEtaEta = cms.vdouble(0.014, 0.012, 0.0115, 0.0, 0.0275, 
            0.0265, 0.0265, 0.0),
        deltaEtaIn = cms.vdouble(0.009, 0.0045, 0.0085, 0.0, 0.0105, 
            0.0068, 0.01, 0.0),
        eSeedOverPin = cms.vdouble(0.11, 0.91, 0.11, 0.0, 0.0, 
            0.85, 0.0, 0.0)
    ),
    classbasedlooseEleIDCutsV01 = cms.PSet(
        deltaPhiIn = cms.vdouble(0.053, 0.0189, 0.059, 0.099, 0.0278, 
            0.0157, 0.042, 0.08),
        hOverE = cms.vdouble(0.076, 0.033, 0.07, 0.0, 0.083, 
            0.0148, 0.033, 0.0),
        sigmaEtaEta = cms.vdouble(0.0101, 0.0095, 0.0097, 0.0, 0.0271, 
            0.0267, 0.0259, 0.0),
        deltaEtaIn = cms.vdouble(0.0078, 0.00259, 0.0062, 0.0, 0.0078, 
            0.0061, 0.0061, 0.0),
        eSeedOverPin = cms.vdouble(0.3, 0.92, 0.211, 0.0, 0.42, 
            0.88, 0.68, 0.0)
    ),
    classbasedlooseEleIDCutsV02 = cms.PSet(
        cutisohcal = cms.vdouble(13.5, 9.93, 7.56, 14.8, 8.1, 
            10.8, 42.7, 20.1, 9.11, 10.4, 
            6.89, 5.59, 8.53, 9.59, 24.2, 
            2.78, 8.67, 0.288),
        cutmishits = cms.vdouble(5.5, 1.5, 5.5, 2.5, 2.5, 
            2.5, 3.5, 5.5, 0.5, 1.5, 
            2.5, 0.5, 1.5, 1.5, 0.5, 
            0.5, 0.5, 0.5),
        cuthoe = cms.vdouble(0.0887, 0.0934, 0.0949, 0.0986, 0.0431, 
            0.0878, 0.097, 0.0509, 0.098, 0.0991, 
            0.0321, 0.0928, 0.0663, 0.0717, 0.0966, 
            0.0758, 0.0149, 0.0131),
        cutdeta = cms.vdouble(0.00958, 0.00406, 0.0122, 0.0137, 0.00837, 
            0.0127, 0.011, 0.00336, 0.00977, 0.015, 
            0.00675, 0.0109, 0.014, 0.00508, 0.0109, 
            0.0146, 0.00506, 0.0127),
        cuteopin = cms.vdouble(0.878, 0.802, 0.814, 0.942, 0.735, 
            0.774, 0.829, 0.909, 0.829, 0.813, 
            0.86, 0.897, 0.817, 0.831, 0.818, 
            0.861, 0.787, 0.789),
        cutip = cms.vdouble(0.0246, 0.076, 0.0966, 0.0885, 0.441, 
            0.205, 0.0292, 0.0293, 0.0619, 0.0251, 
            0.159, 0.0815, 7.29, 0.0106, 5.76, 
            6.89, 1.27, 5.89),
        cutisotk = cms.vdouble(24.3, 8.45, 14.4, 27.8, 6.02, 
            10.5, 14.1, 10.2, 14.5, 19.1, 
            6.1, 14.1, 8.59, 8.33, 8.3, 
            8.93, 8.6, 16.0),
        cutsee = cms.vdouble(0.0172, 0.0115, 0.0143, 0.0344, 0.0295, 
            0.0304, 0.0145, 0.0108, 0.0128, 0.0347, 
            0.0307, 0.0316, 0.018, 0.011, 0.0132, 
            0.0349, 0.031, 0.0327),
        cutdphi = cms.vdouble(0.0372, 0.114, 0.118, 0.0488, 0.117, 
            0.119, 0.0606, 0.0548, 0.117, 0.07, 
            0.0355, 0.117, 0.088, 0.045, 0.118, 
            0.0919, 0.0236, 0.0515),
        cutisoecal = cms.vdouble(33.4, 28.1, 7.32, 27.4, 7.33, 
            21.7, 93.8, 102.0, 12.1, 26.0, 
            8.91, 10.0, 16.1, 31.3, 16.9, 
            15.4, 13.3, 37.7)
    ),
    classbasedlooseEleIDCutsV03 = cms.PSet(
        cutdetain = cms.vdouble(0.00989, 0.00484, 0.0146, 0.0146, 0.00902, 
            0.0172, 0.0137, 0.0477, 0.0275, 0.00967, 
            0.00377, 0.00924, 0.013, 0.00666, 0.0123, 
            0.0125, 0.0228, 0.0112, 0.0106, 0.0038, 
            0.00897, 0.0139, 0.00667, 0.0122, 0.0122, 
            0.0193, 0.00239),
        cutiso_sum = cms.vdouble(31.5, 10.3, 8.8, 11.0, 6.13, 
            6.94, 7.52, 9.0, 3.5, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0),
        cutip_gsf = cms.vdouble(0.0431, 0.0767, 0.139, 0.101, 0.149, 
            0.154, 0.932, 0.15, 0.124, 0.0238, 
            0.0467, 0.0759, 0.0369, 0.147, 0.0986, 
            0.0626, 0.195, 0.116, 0.0122, 0.0125, 
            0.0693, 0.0162, 0.089, 0.0673, 0.0467, 
            0.0651, 0.0221),
        cuthoe = cms.vdouble(0.166, 0.0771, 0.144, 0.37, 0.0497, 
            0.139, 0.401, 2.68, 0.516, 0.234, 
            0.0556, 0.144, 0.368, 0.031, 0.12, 
            0.602, 2.01, 1.05, 0.104, 0.063, 
            0.0565, 0.38, 0.0192, 0.0294, 0.537, 
            4.65, 1.87),
        cutfmishits = cms.vdouble(4.5, 1.5, 1.5, 2.5, 2.5, 
            1.5, 2.5, 2.5, 1.5, 2.5, 
            1.5, 1.5, 1.5, 1.5, 0.5, 
            2.5, 2.5, 0.5, 2.5, 1.5, 
            0.5, 1.5, 1.5, 0.5, 2.5, 
            0.5, 0.5),
        cutiso_sumoet = cms.vdouble(28.9, 15.3, 12.0, 18.3, 7.17, 
            9.42, 11.0, 9.81, 3.94, 22.7, 
            15.9, 12.3, 17.0, 7.58, 8.89, 
            15.2, 12.7, 6.17, 20.8, 21.2, 
            17.2, 15.5, 9.37, 10.6, 19.8, 
            22.1, 15.6),
        cutdcotdist = cms.vdouble(0.0393, 0.0392, 0.0397, 0.0394, 0.0393, 
            0.039, 0.0378, 0.0388, 0.0382, 0.0385, 
            0.0167, 0.00325, 0.0394, 0.0387, 0.0388, 
            0.0227, 0.0258, 0.0127, 0.0298, 0.03, 
            0.00946, 0.039, 0.0231, 0.0278, 0.00162, 
            0.0367, 0.0199),
        cutsee = cms.vdouble(0.0175, 0.0127, 0.0177, 0.0373, 0.0314, 
            0.0329, 0.0157, 0.0409, 0.14, 0.0169, 
            0.0106, 0.0142, 0.0363, 0.0322, 0.0354, 
            0.0117, 0.0372, 28.2, 0.0171, 0.0113, 
            0.014, 0.0403, 0.0323, 0.0411, 0.0104, 
            0.0436, 0.0114),
        cuteseedopcor = cms.vdouble(0.78, 0.302, 0.483, 0.904, 0.168, 
            0.645, 0.108, 0.284, 0.324, 0.591, 
            0.286, 0.488, 0.813, 0.791, 0.672, 
            0.398, 0.834, 0.878, 0.515, 0.937, 
            0.806, 0.816, 0.85, 0.507, 0.367, 
            0.83, 0.648),
        cutdphiin = cms.vdouble(0.041, 0.275, 0.365, 0.047, 0.273, 
            0.296, 0.329, 0.465, 0.627, 0.0581, 
            0.0954, 0.327, 0.0702, 0.0582, 0.279, 
            0.117, 0.318, 0.246, 0.0821, 0.052, 
            0.292, 0.116, 0.0435, 0.312, 0.118, 
            0.296, 0.0459),
        cutet = cms.vdouble(-100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, 12.0, 12.0, 
            12.0, 12.0, 12.0, 12.0, 12.0, 
            12.0, 12.5)
    ),
    classbasedlooseEleIDCutsV04 = cms.PSet(
        cutdetain = cms.vdouble(0.00989, 0.00484, 0.0146, 0.0146, 0.00902, 
            0.0172, 0.0137, 0.0477, 0.0275, 0.00967, 
            0.00377, 0.00924, 0.013, 0.00666, 0.0123, 
            0.0125, 0.0228, 0.0112, 0.0106, 0.0038, 
            0.00897, 0.0139, 0.00667, 0.0122, 0.0122, 
            0.0193, 0.00239),
        cutiso_sum = cms.vdouble(31.5, 10.3, 8.8, 11.0, 6.13, 
            6.94, 7.52, 9.0, 3.5, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0),
        cutip_gsf = cms.vdouble(0.0431, 0.0767, 0.139, 0.101, 0.149, 
            0.154, 0.932, 0.15, 0.124, 0.0238, 
            0.0467, 0.0759, 0.0369, 0.147, 0.0986, 
            0.0626, 0.195, 0.116, 0.0122, 0.0125, 
            0.0693, 0.0162, 0.089, 0.0673, 0.0467, 
            0.0651, 0.0221),
        cuthoe = cms.vdouble(0.166, 0.0771, 0.144, 0.37, 0.0497, 
            0.139, 0.401, 2.68, 0.516, 0.234, 
            0.0556, 0.144, 0.368, 0.031, 0.12, 
            0.602, 2.01, 1.05, 0.104, 0.063, 
            0.0565, 0.38, 0.0192, 0.0294, 0.537, 
            4.65, 1.87),
        cutfmishits = cms.vdouble(4.5, 1.5, 1.5, 2.5, 2.5, 
            1.5, 2.5, 2.5, 1.5, 2.5, 
            1.5, 1.5, 1.5, 1.5, 0.5, 
            2.5, 2.5, 0.5, 2.5, 1.5, 
            0.5, 1.5, 1.5, 0.5, 2.5, 
            0.5, 0.5),
        cutiso_sumoet = cms.vdouble(28.9, 15.3, 12.0, 18.3, 7.17, 
            9.42, 11.0, 9.81, 3.94, 22.7, 
            15.9, 12.3, 17.0, 7.58, 8.89, 
            15.2, 12.7, 6.17, 20.8, 21.2, 
            17.2, 15.5, 9.37, 10.6, 19.8, 
            22.1, 15.6),
        cutdcotdist = cms.vdouble(0.0393, 0.0392, 0.0397, 0.0394, 0.0393, 
            0.039, 0.0378, 0.0388, 0.0382, 0.0385, 
            0.0167, 0.00325, 0.0394, 0.0387, 0.0388, 
            0.0227, 0.0258, 0.0127, 0.0298, 0.03, 
            0.00946, 0.039, 0.0231, 0.0278, 0.00162, 
            0.0367, 0.0199),
        cutsee = cms.vdouble(0.0175, 0.0127, 0.0177, 0.0373, 0.0314, 
            0.0329, 0.0157, 0.0409, 0.14, 0.0169, 
            0.0106, 0.0142, 0.0363, 0.0322, 0.0354, 
            0.0117, 0.0372, 28.2, 0.0171, 0.0113, 
            0.014, 0.0403, 0.0323, 0.0411, 0.0104, 
            0.0436, 0.0114),
        cuteseedopcor = cms.vdouble(0.78, 0.302, 0.483, 0.904, 0.168, 
            0.645, 0.108, 0.284, 0.324, 0.591, 
            0.286, 0.488, 0.813, 0.791, 0.672, 
            0.398, 0.834, 0.878, 0.515, 0.937, 
            0.806, 0.816, 0.85, 0.507, 0.367, 
            0.83, 0.648),
        cutdphiin = cms.vdouble(0.041, 0.275, 0.365, 0.047, 0.273, 
            0.296, 0.329, 0.465, 0.627, 0.0581, 
            0.0954, 0.327, 0.0702, 0.0582, 0.279, 
            0.117, 0.318, 0.246, 0.0821, 0.052, 
            0.292, 0.116, 0.0435, 0.312, 0.118, 
            0.296, 0.0459),
        cutet = cms.vdouble(-100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, 12.0, 12.0, 
            12.0, 12.0, 12.0, 12.0, 12.0, 
            12.0, 12.5)
    ),
    classbasedlooseEleIDCutsV06 = cms.PSet(
        cutdetain = cms.vdouble(0.0137, 0.00678, 0.0241, 0.0187, 0.0161, 
            0.0224, 0.0252, 0.0308, 0.0273),
        cutiso_sum = cms.vdouble(33.0, 17.0, 17.9, 18.8, 8.55, 
            12.5, 17.6, 18.5, 2.98),
        cutip_gsf = cms.vdouble(0.0551, 0.0765, 0.143, 0.0874, 0.594, 
            0.37, 0.0913, 1.15, 0.231),
        cutip_gsfl = cms.vdouble(0.0186, 0.0759, 0.138, 0.0473, 0.62, 
            0.304, 0.109, 0.775, 0.0479),
        cuthoe = cms.vdouble(0.247, 0.137, 0.147, 0.371, 0.0588, 
            0.147, 0.52, 0.452, 0.404),
        cutiso_sumoetl = cms.vdouble(11.3, 9.05, 9.07, 9.94, 5.25, 
            6.15, 10.7, 10.8, 4.4),
        cutfmishits = cms.vdouble(4.5, 1.5, 1.5, 2.5, 2.5, 
            1.5, 4.5, 3.5, 3.5),
        cuthoel = cms.vdouble(0.236, 0.126, 0.147, 0.375, 0.0392, 
            0.145, 0.365, 0.383, 0.384),
        cutdphiin = cms.vdouble(0.0897, 0.262, 0.353, 0.116, 0.357, 
            0.319, 0.342, 0.404, 0.336),
        cutseel = cms.vdouble(0.0164, 0.0118, 0.015, 0.0523, 0.0326, 
            0.0456, 0.0185, 0.0589, 0.0544),
        cutiso_sumoet = cms.vdouble(34.5, 12.7, 12.1, 19.9, 6.35, 
            8.85, 14.0, 10.5, 9.74),
        cutdcotdist = cms.vdouble(9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0),
        cutsee = cms.vdouble(0.0176, 0.0125, 0.0181, 0.0415, 0.0364, 
            0.0418, 0.0146, 0.0678, 0.133),
        cuteseedopcor = cms.vdouble(0.63, 0.82, 0.401, 0.718, 0.4, 
            0.458, 0.15, 0.664, 0.373),
        cutdphiinl = cms.vdouble(0.0747, 0.25, 0.356, 0.0956, 0.347, 
            0.326, 0.333, 0.647, 0.289),
        cutdetainl = cms.vdouble(0.0124, 0.00503, 0.0257, 0.0228, 0.0118, 
            0.0178, 0.0188, 0.14, 0.024)
    ),
    src = cms.InputTag("gsfElectrons"),
    robusttightEleIDCuts = cms.PSet(
        barrel = cms.vdouble(0.0201, 0.0102, 0.0211, 0.00606, -1, 
            -1, 2.34, 3.24, 4.51, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.00253, 0.0291, 0.022, 0.0032, -1, 
            -1, 0.826, 2.7, 0.255, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    classbasedtightEleIDCuts = cms.PSet(
        cutdetain = cms.vdouble(0.0116, 0.00449, 0.00938, 0.0184, 0.00678, 
            0.0109, 0.0252, 0.0268, 0.0139),
        cutiso_sum = cms.vdouble(15.5, 12.2, 12.2, 11.7, 7.16, 
            9.71, 8.66, 11.9, 2.98),
        cutip_gsf = cms.vdouble(0.0131, 0.0586, 0.0839, 0.0366, 0.452, 
            0.204, 0.0913, 0.0802, 0.0731),
        cutip_gsfl = cms.vdouble(0.0119, 0.0527, 0.0471, 0.0212, 0.233, 
            0.267, 0.109, 0.122, 0.0479),
        cuthoe = cms.vdouble(0.215, 0.0608, 0.147, 0.369, 0.0349, 
            0.102, 0.52, 0.422, 0.404),
        cutiso_sumoetl = cms.vdouble(6.21, 6.81, 5.3, 5.39, 2.73, 
            4.73, 4.84, 3.46, 3.73),
        cutfmishits = cms.vdouble(1.5, 1.5, 1.5, 2.5, 2.5, 
            1.5, 1.5, 2.5, 0.5),
        cuthoel = cms.vdouble(0.228, 0.0836, 0.143, 0.37, 0.0392, 
            0.0979, 0.3, 0.381, 0.339),
        cutdphiin = cms.vdouble(0.0897, 0.0993, 0.295, 0.0979, 0.151, 
            0.252, 0.341, 0.308, 0.328),
        cutseel = cms.vdouble(0.0132, 0.0117, 0.0112, 0.0387, 0.0281, 
            0.0287, 0.00987, 0.0296, 0.0544),
        cutiso_sumoet = cms.vdouble(11.9, 7.81, 6.28, 8.92, 4.65, 
            5.49, 9.36, 8.84, 5.94),
        cutdcotdist = cms.vdouble(9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0),
        cutsee = cms.vdouble(0.0145, 0.0116, 0.012, 0.039, 0.0297, 
            0.0311, 0.00987, 0.0347, 0.0917),
        cuteseedopcor = cms.vdouble(0.637, 0.943, 0.742, 0.748, 0.763, 
            0.631, 0.214, 0.873, 0.473),
        cutdphiinl = cms.vdouble(0.061, 0.14, 0.286, 0.0921, 0.197, 
            0.24, 0.333, 0.303, 0.258),
        cutdetainl = cms.vdouble(0.00816, 0.00401, 0.0081, 0.019, 0.00588, 
            0.00893, 0.0171, 0.0434, 0.0143)
    ),
    algorithm = cms.string('eIDCB'),
    robusthighenergyEleIDCuts = cms.PSet(
        barrel = cms.vdouble(0.05, 9999, 0.09, 0.005, 0.94, 
            0.83, 7.5, 2, 0.03, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.05, 0.03, 0.09, 0.007, -1, 
            -1, 15, 2.5, 0.03, 2.5, 
            0, 0.5, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robustlooseEleIDCuts = cms.PSet(
        barrel = cms.vdouble(0.05, 0.0103, 0.8, 0.00688, -1, 
            -1, 7.33, 4.68, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.0389, 0.0307, 0.7, 0.00944, -1, 
            -1, 7.76, 3.09, 2.23, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robustlooseEleIDCutsV02 = cms.PSet(
        barrel = cms.vdouble(0.05, 0.0103, 0.8, 0.00688, -1, 
            -1, 7.33, 4.68, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.0389, 0.0307, 0.7, 0.00944, -1, 
            -1, 7.76, 3.09, 2.23, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robustlooseEleIDCutsV03 = cms.PSet(
        barrel = cms.vdouble(0.05, 0.0103, 0.8, 0.00688, -1, 
            -1, 7.33, 4.68, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.0389, 0.0307, 0.7, 0.00944, -1, 
            -1, 7.76, 3.09, 2.23, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robustlooseEleIDCutsV00 = cms.PSet(
        barrel = cms.vdouble(0.115, 0.014, 0.09, 0.009, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.15, 0.0275, 0.092, 0.0105, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robustlooseEleIDCutsV01 = cms.PSet(
        barrel = cms.vdouble(0.075, 0.0132, 0.058, 0.0077, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.083, 0.027, 0.042, 0.01, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robustlooseEleIDCutsV04 = cms.PSet(
        barrel = cms.vdouble(0.05, 0.0103, 0.8, 0.00688, -1, 
            -1, 7.33, 4.68, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.0389, 0.0307, 0.7, 0.00944, -1, 
            -1, 7.76, 3.09, 2.23, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    additionalCategories = cms.bool(True),
    etBinning = cms.bool(True)
)


process.eidRobustHighEnergy = cms.EDProducer("EleIdCutBasedExtProducer",
    electronQuality = cms.string('highenergy'),
    classbasedtightEleIDCutsV02 = cms.PSet(
        cutisohcal = cms.vdouble(10.9, 7.01, 8.75, 3.51, 7.75, 
            1.62, 11.6, 9.9, 4.97, 5.33, 
            3.18, 2.32, 0.164, 5.46, 12.0, 
            0.00604, 4.1, 0.000628),
        cutmishits = cms.vdouble(5.5, 1.5, 0.5, 1.5, 2.5, 
            0.5, 3.5, 5.5, 0.5, 0.5, 
            0.5, 0.5, 0.5, 1.5, 0.5, 
            0.5, 0.5, 0.5),
        cuthoe = cms.vdouble(0.0871, 0.0289, 0.0783, 0.0946, 0.0245, 
            0.0363, 0.0671, 0.048, 0.0614, 0.0924, 
            0.0158, 0.049, 0.0382, 0.0915, 0.0451, 
            0.0452, 0.00196, 0.0043),
        cutdeta = cms.vdouble(0.00915, 0.00302, 0.0061, 0.0135, 0.00565, 
            0.00793, 0.0102, 0.00266, 0.0106, 0.00903, 
            0.00766, 0.00723, 0.0116, 0.00203, 0.00659, 
            0.0148, 0.00555, 0.0128),
        cuteopin = cms.vdouble(0.878, 0.859, 0.874, 0.944, 0.737, 
            0.773, 0.86, 0.967, 0.917, 0.812, 
            0.915, 1.01, 0.847, 0.953, 0.979, 
            0.841, 0.771, 1.09),
        cutip = cms.vdouble(0.0239, 0.027, 0.0768, 0.0231, 0.178, 
            0.0957, 0.0102, 0.0168, 0.043, 0.0166, 
            0.0594, 0.0308, 2.1, 0.00527, 3.17, 
            4.91, 0.769, 5.9),
        cutisotk = cms.vdouble(6.53, 4.6, 6.0, 8.63, 3.11, 
            7.77, 5.42, 4.81, 4.06, 6.47, 
            2.8, 3.45, 5.29, 5.18, 15.4, 
            5.38, 4.47, 0.0347),
        cutsee = cms.vdouble(0.0131, 0.0106, 0.0115, 0.0306, 0.028, 
            0.0293, 0.0131, 0.0106, 0.0115, 0.0317, 
            0.029, 0.0289, 0.0142, 0.0106, 0.0103, 
            0.035, 0.0296, 0.0333),
        cutdphi = cms.vdouble(0.0369, 0.0307, 0.117, 0.0475, 0.0216, 
            0.117, 0.0372, 0.0246, 0.0426, 0.0612, 
            0.0142, 0.039, 0.0737, 0.0566, 0.0359, 
            0.0187, 0.012, 0.0358),
        cutisoecal = cms.vdouble(20.0, 27.2, 4.48, 13.5, 4.56, 
            3.19, 12.2, 13.1, 7.42, 7.67, 
            4.12, 4.85, 10.1, 12.4, 11.1, 
            11.0, 10.6, 13.4)
    ),
    classbasedtightEleIDCutsV03 = cms.PSet(
        cutdetain = cms.vdouble(0.00811, 0.00341, 0.00633, 0.0103, 0.00667, 
            0.01, 0.0106, 0.0145, 0.0163, 0.0076, 
            0.00259, 0.00511, 0.00941, 0.0043, 0.00857, 
            0.012, 0.0169, 0.00172, 0.00861, 0.00362, 
            0.00601, 0.00925, 0.00489, 0.00832, 0.0119, 
            0.0169, 0.000996),
        cutiso_sum = cms.vdouble(11.8, 8.31, 6.26, 6.18, 3.28, 
            4.38, 4.17, 5.4, 1.57, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0),
        cutip_gsf = cms.vdouble(0.0213, 0.0422, 0.0632, 0.0361, 0.073, 
            0.126, 0.171, 0.119, 0.0372, 0.0131, 
            0.0146, 0.0564, 0.0152, 0.0222, 0.0268, 
            0.0314, 0.0884, 0.00374, 0.00852, 0.00761, 
            0.0143, 0.0106, 0.0127, 0.0119, 0.0123, 
            0.0235, 0.00363),
        cuthoe = cms.vdouble(0.0783, 0.0387, 0.105, 0.118, 0.0227, 
            0.062, 0.13, 2.47, 0.38, 0.0888, 
            0.0503, 0.0955, 0.0741, 0.015, 0.03, 
            0.589, 1.13, 0.612, 0.0494, 0.0461, 
            0.0292, 0.0369, 0.0113, 0.0145, 0.124, 
            2.05, 0.61),
        cutfmishits = cms.vdouble(2.5, 1.5, 1.5, 1.5, 1.5, 
            0.5, 2.5, 0.5, 0.5, 2.5, 
            1.5, 0.5, 0.5, 0.5, 0.5, 
            0.5, 0.5, -0.5, 2.5, 1.5, 
            0.5, 0.5, 0.5, 0.5, 0.5, 
            0.5, 0.5),
        cutiso_sumoet = cms.vdouble(13.7, 11.6, 7.14, 9.98, 3.52, 
            4.87, 6.24, 7.96, 2.53, 11.2, 
            11.9, 7.88, 8.16, 5.58, 5.03, 
            11.4, 8.15, 5.79, 10.4, 11.1, 
            10.4, 7.47, 5.08, 5.9, 11.8, 
            14.1, 11.7),
        cutdcotdist = cms.vdouble(0.0393, 0.0256, 0.00691, 0.0394, 0.0386, 
            0.039, 0.0325, 0.0384, 0.0382, 0.0245, 
            0.000281, 5.46e-05, 0.0342, 0.0232, 0.00107, 
            0.0178, 0.0193, 0.000758, 0.000108, 0.0248, 
            0.000458, 0.0129, 0.00119, 0.0182, 4.53e-05, 
            0.0189, 0.000928),
        cutsee = cms.vdouble(0.0143, 0.0105, 0.0123, 0.0324, 0.0307, 
            0.0301, 0.0109, 0.027, 0.0292, 0.0133, 
            0.0104, 0.0116, 0.0332, 0.0296, 0.031, 
            0.00981, 0.0307, 0.072, 0.0149, 0.0105, 
            0.011, 0.0342, 0.0307, 0.0303, 0.00954, 
            0.0265, 0.0101),
        cuteseedopcor = cms.vdouble(0.784, 0.366, 0.57, 0.911, 0.298, 
            0.645, 0.51, 0.497, 0.932, 0.835, 
            0.968, 0.969, 0.923, 0.898, 0.98, 
            0.63, 0.971, 1.0, 0.515, 0.963, 
            0.986, 0.823, 0.879, 1.01, 0.931, 
            0.937, 1.05),
        cutdphiin = cms.vdouble(0.0404, 0.0499, 0.263, 0.042, 0.0484, 
            0.241, 0.242, 0.231, 0.286, 0.0552, 
            0.0338, 0.154, 0.0623, 0.0183, 0.0392, 
            0.0547, 0.0588, 0.00654, 0.042, 0.0217, 
            0.0885, 0.0445, 0.0141, 0.0234, 0.065, 
            0.0258, 0.0346),
        cutet = cms.vdouble(-100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, 13.7, 13.2, 
            13.6, 14.2, 14.1, 13.9, 12.9, 
            14.9, 17.7)
    ),
    classbasedtightEleIDCutsV00 = cms.PSet(
        deltaPhiIn = cms.vdouble(0.032, 0.016, 0.0525, 0.09, 0.025, 
            0.035, 0.065, 0.092),
        hOverE = cms.vdouble(0.05, 0.042, 0.045, 0.0, 0.055, 
            0.037, 0.05, 0.0),
        sigmaEtaEta = cms.vdouble(0.0125, 0.011, 0.01, 0.0, 0.0265, 
            0.0252, 0.026, 0.0),
        deltaEtaIn = cms.vdouble(0.0055, 0.003, 0.0065, 0.0, 0.006, 
            0.0055, 0.0075, 0.0),
        eSeedOverPin = cms.vdouble(0.24, 0.94, 0.11, 0.0, 0.32, 
            0.83, 0.0, 0.0)
    ),
    classbasedtightEleIDCutsV01 = cms.PSet(
        deltaPhiIn = cms.vdouble(0.0225, 0.0114, 0.0234, 0.039, 0.0215, 
            0.0095, 0.0148, 0.0167),
        hOverE = cms.vdouble(0.056, 0.0221, 0.037, 0.0, 0.0268, 
            0.0102, 0.0104, 0.0),
        sigmaEtaEta = cms.vdouble(0.0095, 0.0094, 0.0094, 0.0, 0.026, 
            0.0257, 0.0246, 0.0),
        deltaEtaIn = cms.vdouble(0.0043, 0.00282, 0.0036, 0.0, 0.0066, 
            0.0049, 0.0041, 0.0),
        eSeedOverPin = cms.vdouble(0.32, 0.94, 0.221, 0.0, 0.74, 
            0.89, 0.66, 0.0)
    ),
    classbasedtightEleIDCutsV06 = cms.PSet(
        cutdetain = cms.vdouble(0.0116, 0.00449, 0.00938, 0.0184, 0.00678, 
            0.0109, 0.0252, 0.0268, 0.0139),
        cutiso_sum = cms.vdouble(15.5, 12.2, 12.2, 11.7, 7.16, 
            9.71, 8.66, 11.9, 2.98),
        cutip_gsf = cms.vdouble(0.0131, 0.0586, 0.0839, 0.0366, 0.452, 
            0.204, 0.0913, 0.0802, 0.0731),
        cutip_gsfl = cms.vdouble(0.0119, 0.0527, 0.0471, 0.0212, 0.233, 
            0.267, 0.109, 0.122, 0.0479),
        cuthoe = cms.vdouble(0.215, 0.0608, 0.147, 0.369, 0.0349, 
            0.102, 0.52, 0.422, 0.404),
        cutiso_sumoetl = cms.vdouble(6.21, 6.81, 5.3, 5.39, 2.73, 
            4.73, 4.84, 3.46, 3.73),
        cutfmishits = cms.vdouble(1.5, 1.5, 1.5, 2.5, 2.5, 
            1.5, 1.5, 2.5, 0.5),
        cuthoel = cms.vdouble(0.228, 0.0836, 0.143, 0.37, 0.0392, 
            0.0979, 0.3, 0.381, 0.339),
        cutdphiin = cms.vdouble(0.0897, 0.0993, 0.295, 0.0979, 0.151, 
            0.252, 0.341, 0.308, 0.328),
        cutseel = cms.vdouble(0.0132, 0.0117, 0.0112, 0.0387, 0.0281, 
            0.0287, 0.00987, 0.0296, 0.0544),
        cutiso_sumoet = cms.vdouble(11.9, 7.81, 6.28, 8.92, 4.65, 
            5.49, 9.36, 8.84, 5.94),
        cutdcotdist = cms.vdouble(9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0),
        cutsee = cms.vdouble(0.0145, 0.0116, 0.012, 0.039, 0.0297, 
            0.0311, 0.00987, 0.0347, 0.0917),
        cuteseedopcor = cms.vdouble(0.637, 0.943, 0.742, 0.748, 0.763, 
            0.631, 0.214, 0.873, 0.473),
        cutdphiinl = cms.vdouble(0.061, 0.14, 0.286, 0.0921, 0.197, 
            0.24, 0.333, 0.303, 0.258),
        cutdetainl = cms.vdouble(0.00816, 0.00401, 0.0081, 0.019, 0.00588, 
            0.00893, 0.0171, 0.0434, 0.0143)
    ),
    classbasedtightEleIDCutsV04 = cms.PSet(
        cutdetain = cms.vdouble(0.00811, 0.00341, 0.00633, 0.0103, 0.00667, 
            0.01, 0.0106, 0.0145, 0.0163, 0.0076, 
            0.00259, 0.00511, 0.00941, 0.0043, 0.00857, 
            0.012, 0.0169, 0.00172, 0.00861, 0.00362, 
            0.00601, 0.00925, 0.00489, 0.00832, 0.0119, 
            0.0169, 0.000996),
        cutiso_sum = cms.vdouble(11.8, 8.31, 6.26, 6.18, 3.28, 
            4.38, 4.17, 5.4, 1.57, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0),
        cutip_gsf = cms.vdouble(0.0213, 0.0422, 0.0632, 0.0361, 0.073, 
            0.126, 0.171, 0.119, 0.0372, 0.0131, 
            0.0146, 0.0564, 0.0152, 0.0222, 0.0268, 
            0.0314, 0.0884, 0.00374, 0.00852, 0.00761, 
            0.0143, 0.0106, 0.0127, 0.0119, 0.0123, 
            0.0235, 0.00363),
        cuthoe = cms.vdouble(0.0783, 0.0387, 0.105, 0.118, 0.0227, 
            0.062, 0.13, 2.47, 0.38, 0.0888, 
            0.0503, 0.0955, 0.0741, 0.015, 0.03, 
            0.589, 1.13, 0.612, 0.0494, 0.0461, 
            0.0292, 0.0369, 0.0113, 0.0145, 0.124, 
            2.05, 0.61),
        cutfmishits = cms.vdouble(2.5, 1.5, 1.5, 1.5, 1.5, 
            0.5, 2.5, 0.5, 0.5, 2.5, 
            1.5, 0.5, 0.5, 0.5, 0.5, 
            0.5, 0.5, -0.5, 2.5, 1.5, 
            0.5, 0.5, 0.5, 0.5, 0.5, 
            0.5, 0.5),
        cutiso_sumoet = cms.vdouble(13.7, 11.6, 7.14, 9.98, 3.52, 
            4.87, 6.24, 7.96, 2.53, 11.2, 
            11.9, 7.88, 8.16, 5.58, 5.03, 
            11.4, 8.15, 5.79, 10.4, 11.1, 
            10.4, 7.47, 5.08, 5.9, 11.8, 
            14.1, 11.7),
        cutdcotdist = cms.vdouble(0.0393, 0.0256, 0.00691, 0.0394, 0.0386, 
            0.039, 0.0325, 0.0384, 0.0382, 0.0245, 
            0.000281, 5.46e-05, 0.0342, 0.0232, 0.00107, 
            0.0178, 0.0193, 0.000758, 0.000108, 0.0248, 
            0.000458, 0.0129, 0.00119, 0.0182, 4.53e-05, 
            0.0189, 0.000928),
        cutsee = cms.vdouble(0.0143, 0.0105, 0.0123, 0.0324, 0.0307, 
            0.0301, 0.0109, 0.027, 0.0292, 0.0133, 
            0.0104, 0.0116, 0.0332, 0.0296, 0.031, 
            0.00981, 0.0307, 0.072, 0.0149, 0.0105, 
            0.011, 0.0342, 0.0307, 0.0303, 0.00954, 
            0.0265, 0.0101),
        cuteseedopcor = cms.vdouble(0.784, 0.366, 0.57, 0.911, 0.298, 
            0.645, 0.51, 0.497, 0.932, 0.835, 
            0.968, 0.969, 0.923, 0.898, 0.98, 
            0.63, 0.971, 1.0, 0.515, 0.963, 
            0.986, 0.823, 0.879, 1.01, 0.931, 
            0.937, 1.05),
        cutdphiin = cms.vdouble(0.0404, 0.0499, 0.263, 0.042, 0.0484, 
            0.241, 0.242, 0.231, 0.286, 0.0552, 
            0.0338, 0.154, 0.0623, 0.0183, 0.0392, 
            0.0547, 0.0588, 0.00654, 0.042, 0.0217, 
            0.0885, 0.0445, 0.0141, 0.0234, 0.065, 
            0.0258, 0.0346),
        cutet = cms.vdouble(-100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, 13.7, 13.2, 
            13.6, 14.2, 14.1, 13.9, 12.9, 
            14.9, 17.7)
    ),
    electronIDType = cms.string('robust'),
    robusttightEleIDCutsV04 = cms.PSet(
        barrel = cms.vdouble(0.0201, 0.0102, 0.0211, 0.00606, -1, 
            -1, 2.34, 3.24, 4.51, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.00253, 0.0291, 0.022, 0.0032, -1, 
            -1, 0.826, 2.7, 0.255, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    electronVersion = cms.string(''),
    robusttightEleIDCutsV00 = cms.PSet(
        barrel = cms.vdouble(0.015, 0.0092, 0.02, 0.0025, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.018, 0.025, 0.02, 0.004, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robusttightEleIDCutsV01 = cms.PSet(
        barrel = cms.vdouble(0.01, 0.0099, 0.025, 0.004, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.01, 0.028, 0.02, 0.0066, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    reducedBarrelRecHitCollection = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
    robusttightEleIDCutsV03 = cms.PSet(
        barrel = cms.vdouble(0.0201, 0.0102, 0.0211, 0.00606, -1, 
            -1, 2.34, 3.24, 4.51, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.00253, 0.0291, 0.022, 0.0032, -1, 
            -1, 0.826, 2.7, 0.255, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    reducedEndcapRecHitCollection = cms.InputTag("ecalRecHit","EcalRecHitsEE"),
    verticesCollection = cms.InputTag("offlinePrimaryVerticesWithBS"),
    classbasedlooseEleIDCuts = cms.PSet(
        cutdetain = cms.vdouble(0.0137, 0.00678, 0.0241, 0.0187, 0.0161, 
            0.0224, 0.0252, 0.0308, 0.0273),
        cutiso_sum = cms.vdouble(33.0, 17.0, 17.9, 18.8, 8.55, 
            12.5, 17.6, 18.5, 2.98),
        cutip_gsf = cms.vdouble(0.0551, 0.0765, 0.143, 0.0874, 0.594, 
            0.37, 0.0913, 1.15, 0.231),
        cutip_gsfl = cms.vdouble(0.0186, 0.0759, 0.138, 0.0473, 0.62, 
            0.304, 0.109, 0.775, 0.0479),
        cuthoe = cms.vdouble(0.247, 0.137, 0.147, 0.371, 0.0588, 
            0.147, 0.52, 0.452, 0.404),
        cutiso_sumoetl = cms.vdouble(11.3, 9.05, 9.07, 9.94, 5.25, 
            6.15, 10.7, 10.8, 4.4),
        cutfmishits = cms.vdouble(4.5, 1.5, 1.5, 2.5, 2.5, 
            1.5, 4.5, 3.5, 3.5),
        cuthoel = cms.vdouble(0.236, 0.126, 0.147, 0.375, 0.0392, 
            0.145, 0.365, 0.383, 0.384),
        cutdphiin = cms.vdouble(0.0897, 0.262, 0.353, 0.116, 0.357, 
            0.319, 0.342, 0.404, 0.336),
        cutseel = cms.vdouble(0.0164, 0.0118, 0.015, 0.0523, 0.0326, 
            0.0456, 0.0185, 0.0589, 0.0544),
        cutiso_sumoet = cms.vdouble(34.5, 12.7, 12.1, 19.9, 6.35, 
            8.85, 14.0, 10.5, 9.74),
        cutdcotdist = cms.vdouble(9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0),
        cutsee = cms.vdouble(0.0176, 0.0125, 0.0181, 0.0415, 0.0364, 
            0.0418, 0.0146, 0.0678, 0.133),
        cuteseedopcor = cms.vdouble(0.63, 0.82, 0.401, 0.718, 0.4, 
            0.458, 0.15, 0.664, 0.373),
        cutdphiinl = cms.vdouble(0.0747, 0.25, 0.356, 0.0956, 0.347, 
            0.326, 0.333, 0.647, 0.289),
        cutdetainl = cms.vdouble(0.0124, 0.00503, 0.0257, 0.0228, 0.0118, 
            0.0178, 0.0188, 0.14, 0.024)
    ),
    robusttightEleIDCutsV02 = cms.PSet(
        barrel = cms.vdouble(0.0201, 0.0102, 0.0211, 0.00606, -1, 
            -1, 2.34, 3.24, 4.51, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.00253, 0.0291, 0.022, 0.0032, -1, 
            -1, 0.826, 2.7, 0.255, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robusthighenergyEleIDCutsV04 = cms.PSet(
        barrel = cms.vdouble(0.05, 9999, 0.09, 0.005, 0.94, 
            0.83, 7.5, 2, 0.03, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.05, 0.03, 0.09, 0.007, -1, 
            -1, 15, 2.5, 0.03, 2.5, 
            0, 0.5, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robusthighenergyEleIDCutsV01 = cms.PSet(
        barrel = cms.vdouble(0.05, 9999, 0.09, 0.005, 0.94, 
            0.83, 9999.0, 9999.0, 0, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.05, 0.0275, 0.09, 0.007, -1, 
            -1, 9999.0, 9999.0, 0, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robusthighenergyEleIDCutsV00 = cms.PSet(
        barrel = cms.vdouble(0.05, 0.011, 0.09, 0.005, -1, 
            -1, 9999.0, 9999.0, 0, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.1, 0.0275, 0.09, 0.007, -1, 
            -1, 9999.0, 9999.0, 0, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robusthighenergyEleIDCutsV03 = cms.PSet(
        barrel = cms.vdouble(0.05, 9999, 0.09, 0.005, 0.94, 
            0.83, 7.5, 2, 0.03, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.05, 0.03, 0.09, 0.007, -1, 
            -1, 15, 2.5, 0.03, 2.5, 
            0, 0.5, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robusthighenergyEleIDCutsV02 = cms.PSet(
        barrel = cms.vdouble(0.05, 9999, 0.09, 0.005, 0.94, 
            0.83, 7.5, 2, 0.03, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.05, 0.03, 0.09, 0.007, -1, 
            -1, 15, 2.5, 0.03, 2.5, 
            0, 0.5, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    classbasedlooseEleIDCutsV00 = cms.PSet(
        deltaPhiIn = cms.vdouble(0.05, 0.025, 0.053, 0.09, 0.07, 
            0.03, 0.092, 0.092),
        hOverE = cms.vdouble(0.115, 0.1, 0.055, 0.0, 0.145, 
            0.12, 0.15, 0.0),
        sigmaEtaEta = cms.vdouble(0.014, 0.012, 0.0115, 0.0, 0.0275, 
            0.0265, 0.0265, 0.0),
        deltaEtaIn = cms.vdouble(0.009, 0.0045, 0.0085, 0.0, 0.0105, 
            0.0068, 0.01, 0.0),
        eSeedOverPin = cms.vdouble(0.11, 0.91, 0.11, 0.0, 0.0, 
            0.85, 0.0, 0.0)
    ),
    classbasedlooseEleIDCutsV01 = cms.PSet(
        deltaPhiIn = cms.vdouble(0.053, 0.0189, 0.059, 0.099, 0.0278, 
            0.0157, 0.042, 0.08),
        hOverE = cms.vdouble(0.076, 0.033, 0.07, 0.0, 0.083, 
            0.0148, 0.033, 0.0),
        sigmaEtaEta = cms.vdouble(0.0101, 0.0095, 0.0097, 0.0, 0.0271, 
            0.0267, 0.0259, 0.0),
        deltaEtaIn = cms.vdouble(0.0078, 0.00259, 0.0062, 0.0, 0.0078, 
            0.0061, 0.0061, 0.0),
        eSeedOverPin = cms.vdouble(0.3, 0.92, 0.211, 0.0, 0.42, 
            0.88, 0.68, 0.0)
    ),
    classbasedlooseEleIDCutsV02 = cms.PSet(
        cutisohcal = cms.vdouble(13.5, 9.93, 7.56, 14.8, 8.1, 
            10.8, 42.7, 20.1, 9.11, 10.4, 
            6.89, 5.59, 8.53, 9.59, 24.2, 
            2.78, 8.67, 0.288),
        cutmishits = cms.vdouble(5.5, 1.5, 5.5, 2.5, 2.5, 
            2.5, 3.5, 5.5, 0.5, 1.5, 
            2.5, 0.5, 1.5, 1.5, 0.5, 
            0.5, 0.5, 0.5),
        cuthoe = cms.vdouble(0.0887, 0.0934, 0.0949, 0.0986, 0.0431, 
            0.0878, 0.097, 0.0509, 0.098, 0.0991, 
            0.0321, 0.0928, 0.0663, 0.0717, 0.0966, 
            0.0758, 0.0149, 0.0131),
        cutdeta = cms.vdouble(0.00958, 0.00406, 0.0122, 0.0137, 0.00837, 
            0.0127, 0.011, 0.00336, 0.00977, 0.015, 
            0.00675, 0.0109, 0.014, 0.00508, 0.0109, 
            0.0146, 0.00506, 0.0127),
        cuteopin = cms.vdouble(0.878, 0.802, 0.814, 0.942, 0.735, 
            0.774, 0.829, 0.909, 0.829, 0.813, 
            0.86, 0.897, 0.817, 0.831, 0.818, 
            0.861, 0.787, 0.789),
        cutip = cms.vdouble(0.0246, 0.076, 0.0966, 0.0885, 0.441, 
            0.205, 0.0292, 0.0293, 0.0619, 0.0251, 
            0.159, 0.0815, 7.29, 0.0106, 5.76, 
            6.89, 1.27, 5.89),
        cutisotk = cms.vdouble(24.3, 8.45, 14.4, 27.8, 6.02, 
            10.5, 14.1, 10.2, 14.5, 19.1, 
            6.1, 14.1, 8.59, 8.33, 8.3, 
            8.93, 8.6, 16.0),
        cutsee = cms.vdouble(0.0172, 0.0115, 0.0143, 0.0344, 0.0295, 
            0.0304, 0.0145, 0.0108, 0.0128, 0.0347, 
            0.0307, 0.0316, 0.018, 0.011, 0.0132, 
            0.0349, 0.031, 0.0327),
        cutdphi = cms.vdouble(0.0372, 0.114, 0.118, 0.0488, 0.117, 
            0.119, 0.0606, 0.0548, 0.117, 0.07, 
            0.0355, 0.117, 0.088, 0.045, 0.118, 
            0.0919, 0.0236, 0.0515),
        cutisoecal = cms.vdouble(33.4, 28.1, 7.32, 27.4, 7.33, 
            21.7, 93.8, 102.0, 12.1, 26.0, 
            8.91, 10.0, 16.1, 31.3, 16.9, 
            15.4, 13.3, 37.7)
    ),
    classbasedlooseEleIDCutsV03 = cms.PSet(
        cutdetain = cms.vdouble(0.00989, 0.00484, 0.0146, 0.0146, 0.00902, 
            0.0172, 0.0137, 0.0477, 0.0275, 0.00967, 
            0.00377, 0.00924, 0.013, 0.00666, 0.0123, 
            0.0125, 0.0228, 0.0112, 0.0106, 0.0038, 
            0.00897, 0.0139, 0.00667, 0.0122, 0.0122, 
            0.0193, 0.00239),
        cutiso_sum = cms.vdouble(31.5, 10.3, 8.8, 11.0, 6.13, 
            6.94, 7.52, 9.0, 3.5, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0),
        cutip_gsf = cms.vdouble(0.0431, 0.0767, 0.139, 0.101, 0.149, 
            0.154, 0.932, 0.15, 0.124, 0.0238, 
            0.0467, 0.0759, 0.0369, 0.147, 0.0986, 
            0.0626, 0.195, 0.116, 0.0122, 0.0125, 
            0.0693, 0.0162, 0.089, 0.0673, 0.0467, 
            0.0651, 0.0221),
        cuthoe = cms.vdouble(0.166, 0.0771, 0.144, 0.37, 0.0497, 
            0.139, 0.401, 2.68, 0.516, 0.234, 
            0.0556, 0.144, 0.368, 0.031, 0.12, 
            0.602, 2.01, 1.05, 0.104, 0.063, 
            0.0565, 0.38, 0.0192, 0.0294, 0.537, 
            4.65, 1.87),
        cutfmishits = cms.vdouble(4.5, 1.5, 1.5, 2.5, 2.5, 
            1.5, 2.5, 2.5, 1.5, 2.5, 
            1.5, 1.5, 1.5, 1.5, 0.5, 
            2.5, 2.5, 0.5, 2.5, 1.5, 
            0.5, 1.5, 1.5, 0.5, 2.5, 
            0.5, 0.5),
        cutiso_sumoet = cms.vdouble(28.9, 15.3, 12.0, 18.3, 7.17, 
            9.42, 11.0, 9.81, 3.94, 22.7, 
            15.9, 12.3, 17.0, 7.58, 8.89, 
            15.2, 12.7, 6.17, 20.8, 21.2, 
            17.2, 15.5, 9.37, 10.6, 19.8, 
            22.1, 15.6),
        cutdcotdist = cms.vdouble(0.0393, 0.0392, 0.0397, 0.0394, 0.0393, 
            0.039, 0.0378, 0.0388, 0.0382, 0.0385, 
            0.0167, 0.00325, 0.0394, 0.0387, 0.0388, 
            0.0227, 0.0258, 0.0127, 0.0298, 0.03, 
            0.00946, 0.039, 0.0231, 0.0278, 0.00162, 
            0.0367, 0.0199),
        cutsee = cms.vdouble(0.0175, 0.0127, 0.0177, 0.0373, 0.0314, 
            0.0329, 0.0157, 0.0409, 0.14, 0.0169, 
            0.0106, 0.0142, 0.0363, 0.0322, 0.0354, 
            0.0117, 0.0372, 28.2, 0.0171, 0.0113, 
            0.014, 0.0403, 0.0323, 0.0411, 0.0104, 
            0.0436, 0.0114),
        cuteseedopcor = cms.vdouble(0.78, 0.302, 0.483, 0.904, 0.168, 
            0.645, 0.108, 0.284, 0.324, 0.591, 
            0.286, 0.488, 0.813, 0.791, 0.672, 
            0.398, 0.834, 0.878, 0.515, 0.937, 
            0.806, 0.816, 0.85, 0.507, 0.367, 
            0.83, 0.648),
        cutdphiin = cms.vdouble(0.041, 0.275, 0.365, 0.047, 0.273, 
            0.296, 0.329, 0.465, 0.627, 0.0581, 
            0.0954, 0.327, 0.0702, 0.0582, 0.279, 
            0.117, 0.318, 0.246, 0.0821, 0.052, 
            0.292, 0.116, 0.0435, 0.312, 0.118, 
            0.296, 0.0459),
        cutet = cms.vdouble(-100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, 12.0, 12.0, 
            12.0, 12.0, 12.0, 12.0, 12.0, 
            12.0, 12.5)
    ),
    classbasedlooseEleIDCutsV04 = cms.PSet(
        cutdetain = cms.vdouble(0.00989, 0.00484, 0.0146, 0.0146, 0.00902, 
            0.0172, 0.0137, 0.0477, 0.0275, 0.00967, 
            0.00377, 0.00924, 0.013, 0.00666, 0.0123, 
            0.0125, 0.0228, 0.0112, 0.0106, 0.0038, 
            0.00897, 0.0139, 0.00667, 0.0122, 0.0122, 
            0.0193, 0.00239),
        cutiso_sum = cms.vdouble(31.5, 10.3, 8.8, 11.0, 6.13, 
            6.94, 7.52, 9.0, 3.5, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0),
        cutip_gsf = cms.vdouble(0.0431, 0.0767, 0.139, 0.101, 0.149, 
            0.154, 0.932, 0.15, 0.124, 0.0238, 
            0.0467, 0.0759, 0.0369, 0.147, 0.0986, 
            0.0626, 0.195, 0.116, 0.0122, 0.0125, 
            0.0693, 0.0162, 0.089, 0.0673, 0.0467, 
            0.0651, 0.0221),
        cuthoe = cms.vdouble(0.166, 0.0771, 0.144, 0.37, 0.0497, 
            0.139, 0.401, 2.68, 0.516, 0.234, 
            0.0556, 0.144, 0.368, 0.031, 0.12, 
            0.602, 2.01, 1.05, 0.104, 0.063, 
            0.0565, 0.38, 0.0192, 0.0294, 0.537, 
            4.65, 1.87),
        cutfmishits = cms.vdouble(4.5, 1.5, 1.5, 2.5, 2.5, 
            1.5, 2.5, 2.5, 1.5, 2.5, 
            1.5, 1.5, 1.5, 1.5, 0.5, 
            2.5, 2.5, 0.5, 2.5, 1.5, 
            0.5, 1.5, 1.5, 0.5, 2.5, 
            0.5, 0.5),
        cutiso_sumoet = cms.vdouble(28.9, 15.3, 12.0, 18.3, 7.17, 
            9.42, 11.0, 9.81, 3.94, 22.7, 
            15.9, 12.3, 17.0, 7.58, 8.89, 
            15.2, 12.7, 6.17, 20.8, 21.2, 
            17.2, 15.5, 9.37, 10.6, 19.8, 
            22.1, 15.6),
        cutdcotdist = cms.vdouble(0.0393, 0.0392, 0.0397, 0.0394, 0.0393, 
            0.039, 0.0378, 0.0388, 0.0382, 0.0385, 
            0.0167, 0.00325, 0.0394, 0.0387, 0.0388, 
            0.0227, 0.0258, 0.0127, 0.0298, 0.03, 
            0.00946, 0.039, 0.0231, 0.0278, 0.00162, 
            0.0367, 0.0199),
        cutsee = cms.vdouble(0.0175, 0.0127, 0.0177, 0.0373, 0.0314, 
            0.0329, 0.0157, 0.0409, 0.14, 0.0169, 
            0.0106, 0.0142, 0.0363, 0.0322, 0.0354, 
            0.0117, 0.0372, 28.2, 0.0171, 0.0113, 
            0.014, 0.0403, 0.0323, 0.0411, 0.0104, 
            0.0436, 0.0114),
        cuteseedopcor = cms.vdouble(0.78, 0.302, 0.483, 0.904, 0.168, 
            0.645, 0.108, 0.284, 0.324, 0.591, 
            0.286, 0.488, 0.813, 0.791, 0.672, 
            0.398, 0.834, 0.878, 0.515, 0.937, 
            0.806, 0.816, 0.85, 0.507, 0.367, 
            0.83, 0.648),
        cutdphiin = cms.vdouble(0.041, 0.275, 0.365, 0.047, 0.273, 
            0.296, 0.329, 0.465, 0.627, 0.0581, 
            0.0954, 0.327, 0.0702, 0.0582, 0.279, 
            0.117, 0.318, 0.246, 0.0821, 0.052, 
            0.292, 0.116, 0.0435, 0.312, 0.118, 
            0.296, 0.0459),
        cutet = cms.vdouble(-100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, 12.0, 12.0, 
            12.0, 12.0, 12.0, 12.0, 12.0, 
            12.0, 12.5)
    ),
    classbasedlooseEleIDCutsV06 = cms.PSet(
        cutdetain = cms.vdouble(0.0137, 0.00678, 0.0241, 0.0187, 0.0161, 
            0.0224, 0.0252, 0.0308, 0.0273),
        cutiso_sum = cms.vdouble(33.0, 17.0, 17.9, 18.8, 8.55, 
            12.5, 17.6, 18.5, 2.98),
        cutip_gsf = cms.vdouble(0.0551, 0.0765, 0.143, 0.0874, 0.594, 
            0.37, 0.0913, 1.15, 0.231),
        cutip_gsfl = cms.vdouble(0.0186, 0.0759, 0.138, 0.0473, 0.62, 
            0.304, 0.109, 0.775, 0.0479),
        cuthoe = cms.vdouble(0.247, 0.137, 0.147, 0.371, 0.0588, 
            0.147, 0.52, 0.452, 0.404),
        cutiso_sumoetl = cms.vdouble(11.3, 9.05, 9.07, 9.94, 5.25, 
            6.15, 10.7, 10.8, 4.4),
        cutfmishits = cms.vdouble(4.5, 1.5, 1.5, 2.5, 2.5, 
            1.5, 4.5, 3.5, 3.5),
        cuthoel = cms.vdouble(0.236, 0.126, 0.147, 0.375, 0.0392, 
            0.145, 0.365, 0.383, 0.384),
        cutdphiin = cms.vdouble(0.0897, 0.262, 0.353, 0.116, 0.357, 
            0.319, 0.342, 0.404, 0.336),
        cutseel = cms.vdouble(0.0164, 0.0118, 0.015, 0.0523, 0.0326, 
            0.0456, 0.0185, 0.0589, 0.0544),
        cutiso_sumoet = cms.vdouble(34.5, 12.7, 12.1, 19.9, 6.35, 
            8.85, 14.0, 10.5, 9.74),
        cutdcotdist = cms.vdouble(9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0),
        cutsee = cms.vdouble(0.0176, 0.0125, 0.0181, 0.0415, 0.0364, 
            0.0418, 0.0146, 0.0678, 0.133),
        cuteseedopcor = cms.vdouble(0.63, 0.82, 0.401, 0.718, 0.4, 
            0.458, 0.15, 0.664, 0.373),
        cutdphiinl = cms.vdouble(0.0747, 0.25, 0.356, 0.0956, 0.347, 
            0.326, 0.333, 0.647, 0.289),
        cutdetainl = cms.vdouble(0.0124, 0.00503, 0.0257, 0.0228, 0.0118, 
            0.0178, 0.0188, 0.14, 0.024)
    ),
    src = cms.InputTag("gsfElectrons"),
    robusttightEleIDCuts = cms.PSet(
        barrel = cms.vdouble(0.0201, 0.0102, 0.0211, 0.00606, -1, 
            -1, 2.34, 3.24, 4.51, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.00253, 0.0291, 0.022, 0.0032, -1, 
            -1, 0.826, 2.7, 0.255, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    classbasedtightEleIDCuts = cms.PSet(
        cutdetain = cms.vdouble(0.0116, 0.00449, 0.00938, 0.0184, 0.00678, 
            0.0109, 0.0252, 0.0268, 0.0139),
        cutiso_sum = cms.vdouble(15.5, 12.2, 12.2, 11.7, 7.16, 
            9.71, 8.66, 11.9, 2.98),
        cutip_gsf = cms.vdouble(0.0131, 0.0586, 0.0839, 0.0366, 0.452, 
            0.204, 0.0913, 0.0802, 0.0731),
        cutip_gsfl = cms.vdouble(0.0119, 0.0527, 0.0471, 0.0212, 0.233, 
            0.267, 0.109, 0.122, 0.0479),
        cuthoe = cms.vdouble(0.215, 0.0608, 0.147, 0.369, 0.0349, 
            0.102, 0.52, 0.422, 0.404),
        cutiso_sumoetl = cms.vdouble(6.21, 6.81, 5.3, 5.39, 2.73, 
            4.73, 4.84, 3.46, 3.73),
        cutfmishits = cms.vdouble(1.5, 1.5, 1.5, 2.5, 2.5, 
            1.5, 1.5, 2.5, 0.5),
        cuthoel = cms.vdouble(0.228, 0.0836, 0.143, 0.37, 0.0392, 
            0.0979, 0.3, 0.381, 0.339),
        cutdphiin = cms.vdouble(0.0897, 0.0993, 0.295, 0.0979, 0.151, 
            0.252, 0.341, 0.308, 0.328),
        cutseel = cms.vdouble(0.0132, 0.0117, 0.0112, 0.0387, 0.0281, 
            0.0287, 0.00987, 0.0296, 0.0544),
        cutiso_sumoet = cms.vdouble(11.9, 7.81, 6.28, 8.92, 4.65, 
            5.49, 9.36, 8.84, 5.94),
        cutdcotdist = cms.vdouble(9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0),
        cutsee = cms.vdouble(0.0145, 0.0116, 0.012, 0.039, 0.0297, 
            0.0311, 0.00987, 0.0347, 0.0917),
        cuteseedopcor = cms.vdouble(0.637, 0.943, 0.742, 0.748, 0.763, 
            0.631, 0.214, 0.873, 0.473),
        cutdphiinl = cms.vdouble(0.061, 0.14, 0.286, 0.0921, 0.197, 
            0.24, 0.333, 0.303, 0.258),
        cutdetainl = cms.vdouble(0.00816, 0.00401, 0.0081, 0.019, 0.00588, 
            0.00893, 0.0171, 0.0434, 0.0143)
    ),
    algorithm = cms.string('eIDCB'),
    robusthighenergyEleIDCuts = cms.PSet(
        barrel = cms.vdouble(0.05, 9999, 0.09, 0.005, 0.94, 
            0.83, 7.5, 2, 0.03, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.05, 0.03, 0.09, 0.007, -1, 
            -1, 15, 2.5, 0.03, 2.5, 
            0, 0.5, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robustlooseEleIDCuts = cms.PSet(
        barrel = cms.vdouble(0.05, 0.0103, 0.8, 0.00688, -1, 
            -1, 7.33, 4.68, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.0389, 0.0307, 0.7, 0.00944, -1, 
            -1, 7.76, 3.09, 2.23, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robustlooseEleIDCutsV02 = cms.PSet(
        barrel = cms.vdouble(0.05, 0.0103, 0.8, 0.00688, -1, 
            -1, 7.33, 4.68, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.0389, 0.0307, 0.7, 0.00944, -1, 
            -1, 7.76, 3.09, 2.23, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robustlooseEleIDCutsV03 = cms.PSet(
        barrel = cms.vdouble(0.05, 0.0103, 0.8, 0.00688, -1, 
            -1, 7.33, 4.68, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.0389, 0.0307, 0.7, 0.00944, -1, 
            -1, 7.76, 3.09, 2.23, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robustlooseEleIDCutsV00 = cms.PSet(
        barrel = cms.vdouble(0.115, 0.014, 0.09, 0.009, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.15, 0.0275, 0.092, 0.0105, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robustlooseEleIDCutsV01 = cms.PSet(
        barrel = cms.vdouble(0.075, 0.0132, 0.058, 0.0077, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.083, 0.027, 0.042, 0.01, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robustlooseEleIDCutsV04 = cms.PSet(
        barrel = cms.vdouble(0.05, 0.0103, 0.8, 0.00688, -1, 
            -1, 7.33, 4.68, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.0389, 0.0307, 0.7, 0.00944, -1, 
            -1, 7.76, 3.09, 2.23, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    additionalCategories = cms.bool(True),
    etBinning = cms.bool(True)
)


process.eidRobustLoose = cms.EDProducer("EleIdCutBasedExtProducer",
    electronQuality = cms.string('loose'),
    classbasedtightEleIDCutsV02 = cms.PSet(
        cutisohcal = cms.vdouble(10.9, 7.01, 8.75, 3.51, 7.75, 
            1.62, 11.6, 9.9, 4.97, 5.33, 
            3.18, 2.32, 0.164, 5.46, 12.0, 
            0.00604, 4.1, 0.000628),
        cutmishits = cms.vdouble(5.5, 1.5, 0.5, 1.5, 2.5, 
            0.5, 3.5, 5.5, 0.5, 0.5, 
            0.5, 0.5, 0.5, 1.5, 0.5, 
            0.5, 0.5, 0.5),
        cuthoe = cms.vdouble(0.0871, 0.0289, 0.0783, 0.0946, 0.0245, 
            0.0363, 0.0671, 0.048, 0.0614, 0.0924, 
            0.0158, 0.049, 0.0382, 0.0915, 0.0451, 
            0.0452, 0.00196, 0.0043),
        cutdeta = cms.vdouble(0.00915, 0.00302, 0.0061, 0.0135, 0.00565, 
            0.00793, 0.0102, 0.00266, 0.0106, 0.00903, 
            0.00766, 0.00723, 0.0116, 0.00203, 0.00659, 
            0.0148, 0.00555, 0.0128),
        cuteopin = cms.vdouble(0.878, 0.859, 0.874, 0.944, 0.737, 
            0.773, 0.86, 0.967, 0.917, 0.812, 
            0.915, 1.01, 0.847, 0.953, 0.979, 
            0.841, 0.771, 1.09),
        cutip = cms.vdouble(0.0239, 0.027, 0.0768, 0.0231, 0.178, 
            0.0957, 0.0102, 0.0168, 0.043, 0.0166, 
            0.0594, 0.0308, 2.1, 0.00527, 3.17, 
            4.91, 0.769, 5.9),
        cutisotk = cms.vdouble(6.53, 4.6, 6.0, 8.63, 3.11, 
            7.77, 5.42, 4.81, 4.06, 6.47, 
            2.8, 3.45, 5.29, 5.18, 15.4, 
            5.38, 4.47, 0.0347),
        cutsee = cms.vdouble(0.0131, 0.0106, 0.0115, 0.0306, 0.028, 
            0.0293, 0.0131, 0.0106, 0.0115, 0.0317, 
            0.029, 0.0289, 0.0142, 0.0106, 0.0103, 
            0.035, 0.0296, 0.0333),
        cutdphi = cms.vdouble(0.0369, 0.0307, 0.117, 0.0475, 0.0216, 
            0.117, 0.0372, 0.0246, 0.0426, 0.0612, 
            0.0142, 0.039, 0.0737, 0.0566, 0.0359, 
            0.0187, 0.012, 0.0358),
        cutisoecal = cms.vdouble(20.0, 27.2, 4.48, 13.5, 4.56, 
            3.19, 12.2, 13.1, 7.42, 7.67, 
            4.12, 4.85, 10.1, 12.4, 11.1, 
            11.0, 10.6, 13.4)
    ),
    classbasedtightEleIDCutsV03 = cms.PSet(
        cutdetain = cms.vdouble(0.00811, 0.00341, 0.00633, 0.0103, 0.00667, 
            0.01, 0.0106, 0.0145, 0.0163, 0.0076, 
            0.00259, 0.00511, 0.00941, 0.0043, 0.00857, 
            0.012, 0.0169, 0.00172, 0.00861, 0.00362, 
            0.00601, 0.00925, 0.00489, 0.00832, 0.0119, 
            0.0169, 0.000996),
        cutiso_sum = cms.vdouble(11.8, 8.31, 6.26, 6.18, 3.28, 
            4.38, 4.17, 5.4, 1.57, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0),
        cutip_gsf = cms.vdouble(0.0213, 0.0422, 0.0632, 0.0361, 0.073, 
            0.126, 0.171, 0.119, 0.0372, 0.0131, 
            0.0146, 0.0564, 0.0152, 0.0222, 0.0268, 
            0.0314, 0.0884, 0.00374, 0.00852, 0.00761, 
            0.0143, 0.0106, 0.0127, 0.0119, 0.0123, 
            0.0235, 0.00363),
        cuthoe = cms.vdouble(0.0783, 0.0387, 0.105, 0.118, 0.0227, 
            0.062, 0.13, 2.47, 0.38, 0.0888, 
            0.0503, 0.0955, 0.0741, 0.015, 0.03, 
            0.589, 1.13, 0.612, 0.0494, 0.0461, 
            0.0292, 0.0369, 0.0113, 0.0145, 0.124, 
            2.05, 0.61),
        cutfmishits = cms.vdouble(2.5, 1.5, 1.5, 1.5, 1.5, 
            0.5, 2.5, 0.5, 0.5, 2.5, 
            1.5, 0.5, 0.5, 0.5, 0.5, 
            0.5, 0.5, -0.5, 2.5, 1.5, 
            0.5, 0.5, 0.5, 0.5, 0.5, 
            0.5, 0.5),
        cutiso_sumoet = cms.vdouble(13.7, 11.6, 7.14, 9.98, 3.52, 
            4.87, 6.24, 7.96, 2.53, 11.2, 
            11.9, 7.88, 8.16, 5.58, 5.03, 
            11.4, 8.15, 5.79, 10.4, 11.1, 
            10.4, 7.47, 5.08, 5.9, 11.8, 
            14.1, 11.7),
        cutdcotdist = cms.vdouble(0.0393, 0.0256, 0.00691, 0.0394, 0.0386, 
            0.039, 0.0325, 0.0384, 0.0382, 0.0245, 
            0.000281, 5.46e-05, 0.0342, 0.0232, 0.00107, 
            0.0178, 0.0193, 0.000758, 0.000108, 0.0248, 
            0.000458, 0.0129, 0.00119, 0.0182, 4.53e-05, 
            0.0189, 0.000928),
        cutsee = cms.vdouble(0.0143, 0.0105, 0.0123, 0.0324, 0.0307, 
            0.0301, 0.0109, 0.027, 0.0292, 0.0133, 
            0.0104, 0.0116, 0.0332, 0.0296, 0.031, 
            0.00981, 0.0307, 0.072, 0.0149, 0.0105, 
            0.011, 0.0342, 0.0307, 0.0303, 0.00954, 
            0.0265, 0.0101),
        cuteseedopcor = cms.vdouble(0.784, 0.366, 0.57, 0.911, 0.298, 
            0.645, 0.51, 0.497, 0.932, 0.835, 
            0.968, 0.969, 0.923, 0.898, 0.98, 
            0.63, 0.971, 1.0, 0.515, 0.963, 
            0.986, 0.823, 0.879, 1.01, 0.931, 
            0.937, 1.05),
        cutdphiin = cms.vdouble(0.0404, 0.0499, 0.263, 0.042, 0.0484, 
            0.241, 0.242, 0.231, 0.286, 0.0552, 
            0.0338, 0.154, 0.0623, 0.0183, 0.0392, 
            0.0547, 0.0588, 0.00654, 0.042, 0.0217, 
            0.0885, 0.0445, 0.0141, 0.0234, 0.065, 
            0.0258, 0.0346),
        cutet = cms.vdouble(-100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, 13.7, 13.2, 
            13.6, 14.2, 14.1, 13.9, 12.9, 
            14.9, 17.7)
    ),
    classbasedtightEleIDCutsV00 = cms.PSet(
        deltaPhiIn = cms.vdouble(0.032, 0.016, 0.0525, 0.09, 0.025, 
            0.035, 0.065, 0.092),
        hOverE = cms.vdouble(0.05, 0.042, 0.045, 0.0, 0.055, 
            0.037, 0.05, 0.0),
        sigmaEtaEta = cms.vdouble(0.0125, 0.011, 0.01, 0.0, 0.0265, 
            0.0252, 0.026, 0.0),
        deltaEtaIn = cms.vdouble(0.0055, 0.003, 0.0065, 0.0, 0.006, 
            0.0055, 0.0075, 0.0),
        eSeedOverPin = cms.vdouble(0.24, 0.94, 0.11, 0.0, 0.32, 
            0.83, 0.0, 0.0)
    ),
    classbasedtightEleIDCutsV01 = cms.PSet(
        deltaPhiIn = cms.vdouble(0.0225, 0.0114, 0.0234, 0.039, 0.0215, 
            0.0095, 0.0148, 0.0167),
        hOverE = cms.vdouble(0.056, 0.0221, 0.037, 0.0, 0.0268, 
            0.0102, 0.0104, 0.0),
        sigmaEtaEta = cms.vdouble(0.0095, 0.0094, 0.0094, 0.0, 0.026, 
            0.0257, 0.0246, 0.0),
        deltaEtaIn = cms.vdouble(0.0043, 0.00282, 0.0036, 0.0, 0.0066, 
            0.0049, 0.0041, 0.0),
        eSeedOverPin = cms.vdouble(0.32, 0.94, 0.221, 0.0, 0.74, 
            0.89, 0.66, 0.0)
    ),
    classbasedtightEleIDCutsV06 = cms.PSet(
        cutdetain = cms.vdouble(0.0116, 0.00449, 0.00938, 0.0184, 0.00678, 
            0.0109, 0.0252, 0.0268, 0.0139),
        cutiso_sum = cms.vdouble(15.5, 12.2, 12.2, 11.7, 7.16, 
            9.71, 8.66, 11.9, 2.98),
        cutip_gsf = cms.vdouble(0.0131, 0.0586, 0.0839, 0.0366, 0.452, 
            0.204, 0.0913, 0.0802, 0.0731),
        cutip_gsfl = cms.vdouble(0.0119, 0.0527, 0.0471, 0.0212, 0.233, 
            0.267, 0.109, 0.122, 0.0479),
        cuthoe = cms.vdouble(0.215, 0.0608, 0.147, 0.369, 0.0349, 
            0.102, 0.52, 0.422, 0.404),
        cutiso_sumoetl = cms.vdouble(6.21, 6.81, 5.3, 5.39, 2.73, 
            4.73, 4.84, 3.46, 3.73),
        cutfmishits = cms.vdouble(1.5, 1.5, 1.5, 2.5, 2.5, 
            1.5, 1.5, 2.5, 0.5),
        cuthoel = cms.vdouble(0.228, 0.0836, 0.143, 0.37, 0.0392, 
            0.0979, 0.3, 0.381, 0.339),
        cutdphiin = cms.vdouble(0.0897, 0.0993, 0.295, 0.0979, 0.151, 
            0.252, 0.341, 0.308, 0.328),
        cutseel = cms.vdouble(0.0132, 0.0117, 0.0112, 0.0387, 0.0281, 
            0.0287, 0.00987, 0.0296, 0.0544),
        cutiso_sumoet = cms.vdouble(11.9, 7.81, 6.28, 8.92, 4.65, 
            5.49, 9.36, 8.84, 5.94),
        cutdcotdist = cms.vdouble(9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0),
        cutsee = cms.vdouble(0.0145, 0.0116, 0.012, 0.039, 0.0297, 
            0.0311, 0.00987, 0.0347, 0.0917),
        cuteseedopcor = cms.vdouble(0.637, 0.943, 0.742, 0.748, 0.763, 
            0.631, 0.214, 0.873, 0.473),
        cutdphiinl = cms.vdouble(0.061, 0.14, 0.286, 0.0921, 0.197, 
            0.24, 0.333, 0.303, 0.258),
        cutdetainl = cms.vdouble(0.00816, 0.00401, 0.0081, 0.019, 0.00588, 
            0.00893, 0.0171, 0.0434, 0.0143)
    ),
    classbasedtightEleIDCutsV04 = cms.PSet(
        cutdetain = cms.vdouble(0.00811, 0.00341, 0.00633, 0.0103, 0.00667, 
            0.01, 0.0106, 0.0145, 0.0163, 0.0076, 
            0.00259, 0.00511, 0.00941, 0.0043, 0.00857, 
            0.012, 0.0169, 0.00172, 0.00861, 0.00362, 
            0.00601, 0.00925, 0.00489, 0.00832, 0.0119, 
            0.0169, 0.000996),
        cutiso_sum = cms.vdouble(11.8, 8.31, 6.26, 6.18, 3.28, 
            4.38, 4.17, 5.4, 1.57, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0),
        cutip_gsf = cms.vdouble(0.0213, 0.0422, 0.0632, 0.0361, 0.073, 
            0.126, 0.171, 0.119, 0.0372, 0.0131, 
            0.0146, 0.0564, 0.0152, 0.0222, 0.0268, 
            0.0314, 0.0884, 0.00374, 0.00852, 0.00761, 
            0.0143, 0.0106, 0.0127, 0.0119, 0.0123, 
            0.0235, 0.00363),
        cuthoe = cms.vdouble(0.0783, 0.0387, 0.105, 0.118, 0.0227, 
            0.062, 0.13, 2.47, 0.38, 0.0888, 
            0.0503, 0.0955, 0.0741, 0.015, 0.03, 
            0.589, 1.13, 0.612, 0.0494, 0.0461, 
            0.0292, 0.0369, 0.0113, 0.0145, 0.124, 
            2.05, 0.61),
        cutfmishits = cms.vdouble(2.5, 1.5, 1.5, 1.5, 1.5, 
            0.5, 2.5, 0.5, 0.5, 2.5, 
            1.5, 0.5, 0.5, 0.5, 0.5, 
            0.5, 0.5, -0.5, 2.5, 1.5, 
            0.5, 0.5, 0.5, 0.5, 0.5, 
            0.5, 0.5),
        cutiso_sumoet = cms.vdouble(13.7, 11.6, 7.14, 9.98, 3.52, 
            4.87, 6.24, 7.96, 2.53, 11.2, 
            11.9, 7.88, 8.16, 5.58, 5.03, 
            11.4, 8.15, 5.79, 10.4, 11.1, 
            10.4, 7.47, 5.08, 5.9, 11.8, 
            14.1, 11.7),
        cutdcotdist = cms.vdouble(0.0393, 0.0256, 0.00691, 0.0394, 0.0386, 
            0.039, 0.0325, 0.0384, 0.0382, 0.0245, 
            0.000281, 5.46e-05, 0.0342, 0.0232, 0.00107, 
            0.0178, 0.0193, 0.000758, 0.000108, 0.0248, 
            0.000458, 0.0129, 0.00119, 0.0182, 4.53e-05, 
            0.0189, 0.000928),
        cutsee = cms.vdouble(0.0143, 0.0105, 0.0123, 0.0324, 0.0307, 
            0.0301, 0.0109, 0.027, 0.0292, 0.0133, 
            0.0104, 0.0116, 0.0332, 0.0296, 0.031, 
            0.00981, 0.0307, 0.072, 0.0149, 0.0105, 
            0.011, 0.0342, 0.0307, 0.0303, 0.00954, 
            0.0265, 0.0101),
        cuteseedopcor = cms.vdouble(0.784, 0.366, 0.57, 0.911, 0.298, 
            0.645, 0.51, 0.497, 0.932, 0.835, 
            0.968, 0.969, 0.923, 0.898, 0.98, 
            0.63, 0.971, 1.0, 0.515, 0.963, 
            0.986, 0.823, 0.879, 1.01, 0.931, 
            0.937, 1.05),
        cutdphiin = cms.vdouble(0.0404, 0.0499, 0.263, 0.042, 0.0484, 
            0.241, 0.242, 0.231, 0.286, 0.0552, 
            0.0338, 0.154, 0.0623, 0.0183, 0.0392, 
            0.0547, 0.0588, 0.00654, 0.042, 0.0217, 
            0.0885, 0.0445, 0.0141, 0.0234, 0.065, 
            0.0258, 0.0346),
        cutet = cms.vdouble(-100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, 13.7, 13.2, 
            13.6, 14.2, 14.1, 13.9, 12.9, 
            14.9, 17.7)
    ),
    electronIDType = cms.string('robust'),
    robusttightEleIDCutsV04 = cms.PSet(
        barrel = cms.vdouble(0.0201, 0.0102, 0.0211, 0.00606, -1, 
            -1, 2.34, 3.24, 4.51, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.00253, 0.0291, 0.022, 0.0032, -1, 
            -1, 0.826, 2.7, 0.255, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    electronVersion = cms.string(''),
    robusttightEleIDCutsV00 = cms.PSet(
        barrel = cms.vdouble(0.015, 0.0092, 0.02, 0.0025, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.018, 0.025, 0.02, 0.004, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robusttightEleIDCutsV01 = cms.PSet(
        barrel = cms.vdouble(0.01, 0.0099, 0.025, 0.004, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.01, 0.028, 0.02, 0.0066, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    reducedBarrelRecHitCollection = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
    robusttightEleIDCutsV03 = cms.PSet(
        barrel = cms.vdouble(0.0201, 0.0102, 0.0211, 0.00606, -1, 
            -1, 2.34, 3.24, 4.51, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.00253, 0.0291, 0.022, 0.0032, -1, 
            -1, 0.826, 2.7, 0.255, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    reducedEndcapRecHitCollection = cms.InputTag("ecalRecHit","EcalRecHitsEE"),
    verticesCollection = cms.InputTag("offlinePrimaryVerticesWithBS"),
    classbasedlooseEleIDCuts = cms.PSet(
        cutdetain = cms.vdouble(0.0137, 0.00678, 0.0241, 0.0187, 0.0161, 
            0.0224, 0.0252, 0.0308, 0.0273),
        cutiso_sum = cms.vdouble(33.0, 17.0, 17.9, 18.8, 8.55, 
            12.5, 17.6, 18.5, 2.98),
        cutip_gsf = cms.vdouble(0.0551, 0.0765, 0.143, 0.0874, 0.594, 
            0.37, 0.0913, 1.15, 0.231),
        cutip_gsfl = cms.vdouble(0.0186, 0.0759, 0.138, 0.0473, 0.62, 
            0.304, 0.109, 0.775, 0.0479),
        cuthoe = cms.vdouble(0.247, 0.137, 0.147, 0.371, 0.0588, 
            0.147, 0.52, 0.452, 0.404),
        cutiso_sumoetl = cms.vdouble(11.3, 9.05, 9.07, 9.94, 5.25, 
            6.15, 10.7, 10.8, 4.4),
        cutfmishits = cms.vdouble(4.5, 1.5, 1.5, 2.5, 2.5, 
            1.5, 4.5, 3.5, 3.5),
        cuthoel = cms.vdouble(0.236, 0.126, 0.147, 0.375, 0.0392, 
            0.145, 0.365, 0.383, 0.384),
        cutdphiin = cms.vdouble(0.0897, 0.262, 0.353, 0.116, 0.357, 
            0.319, 0.342, 0.404, 0.336),
        cutseel = cms.vdouble(0.0164, 0.0118, 0.015, 0.0523, 0.0326, 
            0.0456, 0.0185, 0.0589, 0.0544),
        cutiso_sumoet = cms.vdouble(34.5, 12.7, 12.1, 19.9, 6.35, 
            8.85, 14.0, 10.5, 9.74),
        cutdcotdist = cms.vdouble(9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0),
        cutsee = cms.vdouble(0.0176, 0.0125, 0.0181, 0.0415, 0.0364, 
            0.0418, 0.0146, 0.0678, 0.133),
        cuteseedopcor = cms.vdouble(0.63, 0.82, 0.401, 0.718, 0.4, 
            0.458, 0.15, 0.664, 0.373),
        cutdphiinl = cms.vdouble(0.0747, 0.25, 0.356, 0.0956, 0.347, 
            0.326, 0.333, 0.647, 0.289),
        cutdetainl = cms.vdouble(0.0124, 0.00503, 0.0257, 0.0228, 0.0118, 
            0.0178, 0.0188, 0.14, 0.024)
    ),
    robusttightEleIDCutsV02 = cms.PSet(
        barrel = cms.vdouble(0.0201, 0.0102, 0.0211, 0.00606, -1, 
            -1, 2.34, 3.24, 4.51, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.00253, 0.0291, 0.022, 0.0032, -1, 
            -1, 0.826, 2.7, 0.255, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robusthighenergyEleIDCutsV04 = cms.PSet(
        barrel = cms.vdouble(0.05, 9999, 0.09, 0.005, 0.94, 
            0.83, 7.5, 2, 0.03, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.05, 0.03, 0.09, 0.007, -1, 
            -1, 15, 2.5, 0.03, 2.5, 
            0, 0.5, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robusthighenergyEleIDCutsV01 = cms.PSet(
        barrel = cms.vdouble(0.05, 9999, 0.09, 0.005, 0.94, 
            0.83, 9999.0, 9999.0, 0, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.05, 0.0275, 0.09, 0.007, -1, 
            -1, 9999.0, 9999.0, 0, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robusthighenergyEleIDCutsV00 = cms.PSet(
        barrel = cms.vdouble(0.05, 0.011, 0.09, 0.005, -1, 
            -1, 9999.0, 9999.0, 0, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.1, 0.0275, 0.09, 0.007, -1, 
            -1, 9999.0, 9999.0, 0, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robusthighenergyEleIDCutsV03 = cms.PSet(
        barrel = cms.vdouble(0.05, 9999, 0.09, 0.005, 0.94, 
            0.83, 7.5, 2, 0.03, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.05, 0.03, 0.09, 0.007, -1, 
            -1, 15, 2.5, 0.03, 2.5, 
            0, 0.5, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robusthighenergyEleIDCutsV02 = cms.PSet(
        barrel = cms.vdouble(0.05, 9999, 0.09, 0.005, 0.94, 
            0.83, 7.5, 2, 0.03, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.05, 0.03, 0.09, 0.007, -1, 
            -1, 15, 2.5, 0.03, 2.5, 
            0, 0.5, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    classbasedlooseEleIDCutsV00 = cms.PSet(
        deltaPhiIn = cms.vdouble(0.05, 0.025, 0.053, 0.09, 0.07, 
            0.03, 0.092, 0.092),
        hOverE = cms.vdouble(0.115, 0.1, 0.055, 0.0, 0.145, 
            0.12, 0.15, 0.0),
        sigmaEtaEta = cms.vdouble(0.014, 0.012, 0.0115, 0.0, 0.0275, 
            0.0265, 0.0265, 0.0),
        deltaEtaIn = cms.vdouble(0.009, 0.0045, 0.0085, 0.0, 0.0105, 
            0.0068, 0.01, 0.0),
        eSeedOverPin = cms.vdouble(0.11, 0.91, 0.11, 0.0, 0.0, 
            0.85, 0.0, 0.0)
    ),
    classbasedlooseEleIDCutsV01 = cms.PSet(
        deltaPhiIn = cms.vdouble(0.053, 0.0189, 0.059, 0.099, 0.0278, 
            0.0157, 0.042, 0.08),
        hOverE = cms.vdouble(0.076, 0.033, 0.07, 0.0, 0.083, 
            0.0148, 0.033, 0.0),
        sigmaEtaEta = cms.vdouble(0.0101, 0.0095, 0.0097, 0.0, 0.0271, 
            0.0267, 0.0259, 0.0),
        deltaEtaIn = cms.vdouble(0.0078, 0.00259, 0.0062, 0.0, 0.0078, 
            0.0061, 0.0061, 0.0),
        eSeedOverPin = cms.vdouble(0.3, 0.92, 0.211, 0.0, 0.42, 
            0.88, 0.68, 0.0)
    ),
    classbasedlooseEleIDCutsV02 = cms.PSet(
        cutisohcal = cms.vdouble(13.5, 9.93, 7.56, 14.8, 8.1, 
            10.8, 42.7, 20.1, 9.11, 10.4, 
            6.89, 5.59, 8.53, 9.59, 24.2, 
            2.78, 8.67, 0.288),
        cutmishits = cms.vdouble(5.5, 1.5, 5.5, 2.5, 2.5, 
            2.5, 3.5, 5.5, 0.5, 1.5, 
            2.5, 0.5, 1.5, 1.5, 0.5, 
            0.5, 0.5, 0.5),
        cuthoe = cms.vdouble(0.0887, 0.0934, 0.0949, 0.0986, 0.0431, 
            0.0878, 0.097, 0.0509, 0.098, 0.0991, 
            0.0321, 0.0928, 0.0663, 0.0717, 0.0966, 
            0.0758, 0.0149, 0.0131),
        cutdeta = cms.vdouble(0.00958, 0.00406, 0.0122, 0.0137, 0.00837, 
            0.0127, 0.011, 0.00336, 0.00977, 0.015, 
            0.00675, 0.0109, 0.014, 0.00508, 0.0109, 
            0.0146, 0.00506, 0.0127),
        cuteopin = cms.vdouble(0.878, 0.802, 0.814, 0.942, 0.735, 
            0.774, 0.829, 0.909, 0.829, 0.813, 
            0.86, 0.897, 0.817, 0.831, 0.818, 
            0.861, 0.787, 0.789),
        cutip = cms.vdouble(0.0246, 0.076, 0.0966, 0.0885, 0.441, 
            0.205, 0.0292, 0.0293, 0.0619, 0.0251, 
            0.159, 0.0815, 7.29, 0.0106, 5.76, 
            6.89, 1.27, 5.89),
        cutisotk = cms.vdouble(24.3, 8.45, 14.4, 27.8, 6.02, 
            10.5, 14.1, 10.2, 14.5, 19.1, 
            6.1, 14.1, 8.59, 8.33, 8.3, 
            8.93, 8.6, 16.0),
        cutsee = cms.vdouble(0.0172, 0.0115, 0.0143, 0.0344, 0.0295, 
            0.0304, 0.0145, 0.0108, 0.0128, 0.0347, 
            0.0307, 0.0316, 0.018, 0.011, 0.0132, 
            0.0349, 0.031, 0.0327),
        cutdphi = cms.vdouble(0.0372, 0.114, 0.118, 0.0488, 0.117, 
            0.119, 0.0606, 0.0548, 0.117, 0.07, 
            0.0355, 0.117, 0.088, 0.045, 0.118, 
            0.0919, 0.0236, 0.0515),
        cutisoecal = cms.vdouble(33.4, 28.1, 7.32, 27.4, 7.33, 
            21.7, 93.8, 102.0, 12.1, 26.0, 
            8.91, 10.0, 16.1, 31.3, 16.9, 
            15.4, 13.3, 37.7)
    ),
    classbasedlooseEleIDCutsV03 = cms.PSet(
        cutdetain = cms.vdouble(0.00989, 0.00484, 0.0146, 0.0146, 0.00902, 
            0.0172, 0.0137, 0.0477, 0.0275, 0.00967, 
            0.00377, 0.00924, 0.013, 0.00666, 0.0123, 
            0.0125, 0.0228, 0.0112, 0.0106, 0.0038, 
            0.00897, 0.0139, 0.00667, 0.0122, 0.0122, 
            0.0193, 0.00239),
        cutiso_sum = cms.vdouble(31.5, 10.3, 8.8, 11.0, 6.13, 
            6.94, 7.52, 9.0, 3.5, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0),
        cutip_gsf = cms.vdouble(0.0431, 0.0767, 0.139, 0.101, 0.149, 
            0.154, 0.932, 0.15, 0.124, 0.0238, 
            0.0467, 0.0759, 0.0369, 0.147, 0.0986, 
            0.0626, 0.195, 0.116, 0.0122, 0.0125, 
            0.0693, 0.0162, 0.089, 0.0673, 0.0467, 
            0.0651, 0.0221),
        cuthoe = cms.vdouble(0.166, 0.0771, 0.144, 0.37, 0.0497, 
            0.139, 0.401, 2.68, 0.516, 0.234, 
            0.0556, 0.144, 0.368, 0.031, 0.12, 
            0.602, 2.01, 1.05, 0.104, 0.063, 
            0.0565, 0.38, 0.0192, 0.0294, 0.537, 
            4.65, 1.87),
        cutfmishits = cms.vdouble(4.5, 1.5, 1.5, 2.5, 2.5, 
            1.5, 2.5, 2.5, 1.5, 2.5, 
            1.5, 1.5, 1.5, 1.5, 0.5, 
            2.5, 2.5, 0.5, 2.5, 1.5, 
            0.5, 1.5, 1.5, 0.5, 2.5, 
            0.5, 0.5),
        cutiso_sumoet = cms.vdouble(28.9, 15.3, 12.0, 18.3, 7.17, 
            9.42, 11.0, 9.81, 3.94, 22.7, 
            15.9, 12.3, 17.0, 7.58, 8.89, 
            15.2, 12.7, 6.17, 20.8, 21.2, 
            17.2, 15.5, 9.37, 10.6, 19.8, 
            22.1, 15.6),
        cutdcotdist = cms.vdouble(0.0393, 0.0392, 0.0397, 0.0394, 0.0393, 
            0.039, 0.0378, 0.0388, 0.0382, 0.0385, 
            0.0167, 0.00325, 0.0394, 0.0387, 0.0388, 
            0.0227, 0.0258, 0.0127, 0.0298, 0.03, 
            0.00946, 0.039, 0.0231, 0.0278, 0.00162, 
            0.0367, 0.0199),
        cutsee = cms.vdouble(0.0175, 0.0127, 0.0177, 0.0373, 0.0314, 
            0.0329, 0.0157, 0.0409, 0.14, 0.0169, 
            0.0106, 0.0142, 0.0363, 0.0322, 0.0354, 
            0.0117, 0.0372, 28.2, 0.0171, 0.0113, 
            0.014, 0.0403, 0.0323, 0.0411, 0.0104, 
            0.0436, 0.0114),
        cuteseedopcor = cms.vdouble(0.78, 0.302, 0.483, 0.904, 0.168, 
            0.645, 0.108, 0.284, 0.324, 0.591, 
            0.286, 0.488, 0.813, 0.791, 0.672, 
            0.398, 0.834, 0.878, 0.515, 0.937, 
            0.806, 0.816, 0.85, 0.507, 0.367, 
            0.83, 0.648),
        cutdphiin = cms.vdouble(0.041, 0.275, 0.365, 0.047, 0.273, 
            0.296, 0.329, 0.465, 0.627, 0.0581, 
            0.0954, 0.327, 0.0702, 0.0582, 0.279, 
            0.117, 0.318, 0.246, 0.0821, 0.052, 
            0.292, 0.116, 0.0435, 0.312, 0.118, 
            0.296, 0.0459),
        cutet = cms.vdouble(-100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, 12.0, 12.0, 
            12.0, 12.0, 12.0, 12.0, 12.0, 
            12.0, 12.5)
    ),
    classbasedlooseEleIDCutsV04 = cms.PSet(
        cutdetain = cms.vdouble(0.00989, 0.00484, 0.0146, 0.0146, 0.00902, 
            0.0172, 0.0137, 0.0477, 0.0275, 0.00967, 
            0.00377, 0.00924, 0.013, 0.00666, 0.0123, 
            0.0125, 0.0228, 0.0112, 0.0106, 0.0038, 
            0.00897, 0.0139, 0.00667, 0.0122, 0.0122, 
            0.0193, 0.00239),
        cutiso_sum = cms.vdouble(31.5, 10.3, 8.8, 11.0, 6.13, 
            6.94, 7.52, 9.0, 3.5, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0),
        cutip_gsf = cms.vdouble(0.0431, 0.0767, 0.139, 0.101, 0.149, 
            0.154, 0.932, 0.15, 0.124, 0.0238, 
            0.0467, 0.0759, 0.0369, 0.147, 0.0986, 
            0.0626, 0.195, 0.116, 0.0122, 0.0125, 
            0.0693, 0.0162, 0.089, 0.0673, 0.0467, 
            0.0651, 0.0221),
        cuthoe = cms.vdouble(0.166, 0.0771, 0.144, 0.37, 0.0497, 
            0.139, 0.401, 2.68, 0.516, 0.234, 
            0.0556, 0.144, 0.368, 0.031, 0.12, 
            0.602, 2.01, 1.05, 0.104, 0.063, 
            0.0565, 0.38, 0.0192, 0.0294, 0.537, 
            4.65, 1.87),
        cutfmishits = cms.vdouble(4.5, 1.5, 1.5, 2.5, 2.5, 
            1.5, 2.5, 2.5, 1.5, 2.5, 
            1.5, 1.5, 1.5, 1.5, 0.5, 
            2.5, 2.5, 0.5, 2.5, 1.5, 
            0.5, 1.5, 1.5, 0.5, 2.5, 
            0.5, 0.5),
        cutiso_sumoet = cms.vdouble(28.9, 15.3, 12.0, 18.3, 7.17, 
            9.42, 11.0, 9.81, 3.94, 22.7, 
            15.9, 12.3, 17.0, 7.58, 8.89, 
            15.2, 12.7, 6.17, 20.8, 21.2, 
            17.2, 15.5, 9.37, 10.6, 19.8, 
            22.1, 15.6),
        cutdcotdist = cms.vdouble(0.0393, 0.0392, 0.0397, 0.0394, 0.0393, 
            0.039, 0.0378, 0.0388, 0.0382, 0.0385, 
            0.0167, 0.00325, 0.0394, 0.0387, 0.0388, 
            0.0227, 0.0258, 0.0127, 0.0298, 0.03, 
            0.00946, 0.039, 0.0231, 0.0278, 0.00162, 
            0.0367, 0.0199),
        cutsee = cms.vdouble(0.0175, 0.0127, 0.0177, 0.0373, 0.0314, 
            0.0329, 0.0157, 0.0409, 0.14, 0.0169, 
            0.0106, 0.0142, 0.0363, 0.0322, 0.0354, 
            0.0117, 0.0372, 28.2, 0.0171, 0.0113, 
            0.014, 0.0403, 0.0323, 0.0411, 0.0104, 
            0.0436, 0.0114),
        cuteseedopcor = cms.vdouble(0.78, 0.302, 0.483, 0.904, 0.168, 
            0.645, 0.108, 0.284, 0.324, 0.591, 
            0.286, 0.488, 0.813, 0.791, 0.672, 
            0.398, 0.834, 0.878, 0.515, 0.937, 
            0.806, 0.816, 0.85, 0.507, 0.367, 
            0.83, 0.648),
        cutdphiin = cms.vdouble(0.041, 0.275, 0.365, 0.047, 0.273, 
            0.296, 0.329, 0.465, 0.627, 0.0581, 
            0.0954, 0.327, 0.0702, 0.0582, 0.279, 
            0.117, 0.318, 0.246, 0.0821, 0.052, 
            0.292, 0.116, 0.0435, 0.312, 0.118, 
            0.296, 0.0459),
        cutet = cms.vdouble(-100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, 12.0, 12.0, 
            12.0, 12.0, 12.0, 12.0, 12.0, 
            12.0, 12.5)
    ),
    classbasedlooseEleIDCutsV06 = cms.PSet(
        cutdetain = cms.vdouble(0.0137, 0.00678, 0.0241, 0.0187, 0.0161, 
            0.0224, 0.0252, 0.0308, 0.0273),
        cutiso_sum = cms.vdouble(33.0, 17.0, 17.9, 18.8, 8.55, 
            12.5, 17.6, 18.5, 2.98),
        cutip_gsf = cms.vdouble(0.0551, 0.0765, 0.143, 0.0874, 0.594, 
            0.37, 0.0913, 1.15, 0.231),
        cutip_gsfl = cms.vdouble(0.0186, 0.0759, 0.138, 0.0473, 0.62, 
            0.304, 0.109, 0.775, 0.0479),
        cuthoe = cms.vdouble(0.247, 0.137, 0.147, 0.371, 0.0588, 
            0.147, 0.52, 0.452, 0.404),
        cutiso_sumoetl = cms.vdouble(11.3, 9.05, 9.07, 9.94, 5.25, 
            6.15, 10.7, 10.8, 4.4),
        cutfmishits = cms.vdouble(4.5, 1.5, 1.5, 2.5, 2.5, 
            1.5, 4.5, 3.5, 3.5),
        cuthoel = cms.vdouble(0.236, 0.126, 0.147, 0.375, 0.0392, 
            0.145, 0.365, 0.383, 0.384),
        cutdphiin = cms.vdouble(0.0897, 0.262, 0.353, 0.116, 0.357, 
            0.319, 0.342, 0.404, 0.336),
        cutseel = cms.vdouble(0.0164, 0.0118, 0.015, 0.0523, 0.0326, 
            0.0456, 0.0185, 0.0589, 0.0544),
        cutiso_sumoet = cms.vdouble(34.5, 12.7, 12.1, 19.9, 6.35, 
            8.85, 14.0, 10.5, 9.74),
        cutdcotdist = cms.vdouble(9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0),
        cutsee = cms.vdouble(0.0176, 0.0125, 0.0181, 0.0415, 0.0364, 
            0.0418, 0.0146, 0.0678, 0.133),
        cuteseedopcor = cms.vdouble(0.63, 0.82, 0.401, 0.718, 0.4, 
            0.458, 0.15, 0.664, 0.373),
        cutdphiinl = cms.vdouble(0.0747, 0.25, 0.356, 0.0956, 0.347, 
            0.326, 0.333, 0.647, 0.289),
        cutdetainl = cms.vdouble(0.0124, 0.00503, 0.0257, 0.0228, 0.0118, 
            0.0178, 0.0188, 0.14, 0.024)
    ),
    src = cms.InputTag("gsfElectrons"),
    robusttightEleIDCuts = cms.PSet(
        barrel = cms.vdouble(0.0201, 0.0102, 0.0211, 0.00606, -1, 
            -1, 2.34, 3.24, 4.51, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.00253, 0.0291, 0.022, 0.0032, -1, 
            -1, 0.826, 2.7, 0.255, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    classbasedtightEleIDCuts = cms.PSet(
        cutdetain = cms.vdouble(0.0116, 0.00449, 0.00938, 0.0184, 0.00678, 
            0.0109, 0.0252, 0.0268, 0.0139),
        cutiso_sum = cms.vdouble(15.5, 12.2, 12.2, 11.7, 7.16, 
            9.71, 8.66, 11.9, 2.98),
        cutip_gsf = cms.vdouble(0.0131, 0.0586, 0.0839, 0.0366, 0.452, 
            0.204, 0.0913, 0.0802, 0.0731),
        cutip_gsfl = cms.vdouble(0.0119, 0.0527, 0.0471, 0.0212, 0.233, 
            0.267, 0.109, 0.122, 0.0479),
        cuthoe = cms.vdouble(0.215, 0.0608, 0.147, 0.369, 0.0349, 
            0.102, 0.52, 0.422, 0.404),
        cutiso_sumoetl = cms.vdouble(6.21, 6.81, 5.3, 5.39, 2.73, 
            4.73, 4.84, 3.46, 3.73),
        cutfmishits = cms.vdouble(1.5, 1.5, 1.5, 2.5, 2.5, 
            1.5, 1.5, 2.5, 0.5),
        cuthoel = cms.vdouble(0.228, 0.0836, 0.143, 0.37, 0.0392, 
            0.0979, 0.3, 0.381, 0.339),
        cutdphiin = cms.vdouble(0.0897, 0.0993, 0.295, 0.0979, 0.151, 
            0.252, 0.341, 0.308, 0.328),
        cutseel = cms.vdouble(0.0132, 0.0117, 0.0112, 0.0387, 0.0281, 
            0.0287, 0.00987, 0.0296, 0.0544),
        cutiso_sumoet = cms.vdouble(11.9, 7.81, 6.28, 8.92, 4.65, 
            5.49, 9.36, 8.84, 5.94),
        cutdcotdist = cms.vdouble(9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0),
        cutsee = cms.vdouble(0.0145, 0.0116, 0.012, 0.039, 0.0297, 
            0.0311, 0.00987, 0.0347, 0.0917),
        cuteseedopcor = cms.vdouble(0.637, 0.943, 0.742, 0.748, 0.763, 
            0.631, 0.214, 0.873, 0.473),
        cutdphiinl = cms.vdouble(0.061, 0.14, 0.286, 0.0921, 0.197, 
            0.24, 0.333, 0.303, 0.258),
        cutdetainl = cms.vdouble(0.00816, 0.00401, 0.0081, 0.019, 0.00588, 
            0.00893, 0.0171, 0.0434, 0.0143)
    ),
    algorithm = cms.string('eIDCB'),
    robusthighenergyEleIDCuts = cms.PSet(
        barrel = cms.vdouble(0.05, 9999, 0.09, 0.005, 0.94, 
            0.83, 7.5, 2, 0.03, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.05, 0.03, 0.09, 0.007, -1, 
            -1, 15, 2.5, 0.03, 2.5, 
            0, 0.5, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robustlooseEleIDCuts = cms.PSet(
        barrel = cms.vdouble(0.05, 0.0103, 0.8, 0.00688, -1, 
            -1, 7.33, 4.68, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.0389, 0.0307, 0.7, 0.00944, -1, 
            -1, 7.76, 3.09, 2.23, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robustlooseEleIDCutsV02 = cms.PSet(
        barrel = cms.vdouble(0.05, 0.0103, 0.8, 0.00688, -1, 
            -1, 7.33, 4.68, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.0389, 0.0307, 0.7, 0.00944, -1, 
            -1, 7.76, 3.09, 2.23, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robustlooseEleIDCutsV03 = cms.PSet(
        barrel = cms.vdouble(0.05, 0.0103, 0.8, 0.00688, -1, 
            -1, 7.33, 4.68, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.0389, 0.0307, 0.7, 0.00944, -1, 
            -1, 7.76, 3.09, 2.23, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robustlooseEleIDCutsV00 = cms.PSet(
        barrel = cms.vdouble(0.115, 0.014, 0.09, 0.009, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.15, 0.0275, 0.092, 0.0105, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robustlooseEleIDCutsV01 = cms.PSet(
        barrel = cms.vdouble(0.075, 0.0132, 0.058, 0.0077, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.083, 0.027, 0.042, 0.01, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robustlooseEleIDCutsV04 = cms.PSet(
        barrel = cms.vdouble(0.05, 0.0103, 0.8, 0.00688, -1, 
            -1, 7.33, 4.68, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.0389, 0.0307, 0.7, 0.00944, -1, 
            -1, 7.76, 3.09, 2.23, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    additionalCategories = cms.bool(True),
    etBinning = cms.bool(True)
)


process.eidRobustTight = cms.EDProducer("EleIdCutBasedExtProducer",
    electronQuality = cms.string('tight'),
    classbasedtightEleIDCutsV02 = cms.PSet(
        cutisohcal = cms.vdouble(10.9, 7.01, 8.75, 3.51, 7.75, 
            1.62, 11.6, 9.9, 4.97, 5.33, 
            3.18, 2.32, 0.164, 5.46, 12.0, 
            0.00604, 4.1, 0.000628),
        cutmishits = cms.vdouble(5.5, 1.5, 0.5, 1.5, 2.5, 
            0.5, 3.5, 5.5, 0.5, 0.5, 
            0.5, 0.5, 0.5, 1.5, 0.5, 
            0.5, 0.5, 0.5),
        cuthoe = cms.vdouble(0.0871, 0.0289, 0.0783, 0.0946, 0.0245, 
            0.0363, 0.0671, 0.048, 0.0614, 0.0924, 
            0.0158, 0.049, 0.0382, 0.0915, 0.0451, 
            0.0452, 0.00196, 0.0043),
        cutdeta = cms.vdouble(0.00915, 0.00302, 0.0061, 0.0135, 0.00565, 
            0.00793, 0.0102, 0.00266, 0.0106, 0.00903, 
            0.00766, 0.00723, 0.0116, 0.00203, 0.00659, 
            0.0148, 0.00555, 0.0128),
        cuteopin = cms.vdouble(0.878, 0.859, 0.874, 0.944, 0.737, 
            0.773, 0.86, 0.967, 0.917, 0.812, 
            0.915, 1.01, 0.847, 0.953, 0.979, 
            0.841, 0.771, 1.09),
        cutip = cms.vdouble(0.0239, 0.027, 0.0768, 0.0231, 0.178, 
            0.0957, 0.0102, 0.0168, 0.043, 0.0166, 
            0.0594, 0.0308, 2.1, 0.00527, 3.17, 
            4.91, 0.769, 5.9),
        cutisotk = cms.vdouble(6.53, 4.6, 6.0, 8.63, 3.11, 
            7.77, 5.42, 4.81, 4.06, 6.47, 
            2.8, 3.45, 5.29, 5.18, 15.4, 
            5.38, 4.47, 0.0347),
        cutsee = cms.vdouble(0.0131, 0.0106, 0.0115, 0.0306, 0.028, 
            0.0293, 0.0131, 0.0106, 0.0115, 0.0317, 
            0.029, 0.0289, 0.0142, 0.0106, 0.0103, 
            0.035, 0.0296, 0.0333),
        cutdphi = cms.vdouble(0.0369, 0.0307, 0.117, 0.0475, 0.0216, 
            0.117, 0.0372, 0.0246, 0.0426, 0.0612, 
            0.0142, 0.039, 0.0737, 0.0566, 0.0359, 
            0.0187, 0.012, 0.0358),
        cutisoecal = cms.vdouble(20.0, 27.2, 4.48, 13.5, 4.56, 
            3.19, 12.2, 13.1, 7.42, 7.67, 
            4.12, 4.85, 10.1, 12.4, 11.1, 
            11.0, 10.6, 13.4)
    ),
    classbasedtightEleIDCutsV03 = cms.PSet(
        cutdetain = cms.vdouble(0.00811, 0.00341, 0.00633, 0.0103, 0.00667, 
            0.01, 0.0106, 0.0145, 0.0163, 0.0076, 
            0.00259, 0.00511, 0.00941, 0.0043, 0.00857, 
            0.012, 0.0169, 0.00172, 0.00861, 0.00362, 
            0.00601, 0.00925, 0.00489, 0.00832, 0.0119, 
            0.0169, 0.000996),
        cutiso_sum = cms.vdouble(11.8, 8.31, 6.26, 6.18, 3.28, 
            4.38, 4.17, 5.4, 1.57, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0),
        cutip_gsf = cms.vdouble(0.0213, 0.0422, 0.0632, 0.0361, 0.073, 
            0.126, 0.171, 0.119, 0.0372, 0.0131, 
            0.0146, 0.0564, 0.0152, 0.0222, 0.0268, 
            0.0314, 0.0884, 0.00374, 0.00852, 0.00761, 
            0.0143, 0.0106, 0.0127, 0.0119, 0.0123, 
            0.0235, 0.00363),
        cuthoe = cms.vdouble(0.0783, 0.0387, 0.105, 0.118, 0.0227, 
            0.062, 0.13, 2.47, 0.38, 0.0888, 
            0.0503, 0.0955, 0.0741, 0.015, 0.03, 
            0.589, 1.13, 0.612, 0.0494, 0.0461, 
            0.0292, 0.0369, 0.0113, 0.0145, 0.124, 
            2.05, 0.61),
        cutfmishits = cms.vdouble(2.5, 1.5, 1.5, 1.5, 1.5, 
            0.5, 2.5, 0.5, 0.5, 2.5, 
            1.5, 0.5, 0.5, 0.5, 0.5, 
            0.5, 0.5, -0.5, 2.5, 1.5, 
            0.5, 0.5, 0.5, 0.5, 0.5, 
            0.5, 0.5),
        cutiso_sumoet = cms.vdouble(13.7, 11.6, 7.14, 9.98, 3.52, 
            4.87, 6.24, 7.96, 2.53, 11.2, 
            11.9, 7.88, 8.16, 5.58, 5.03, 
            11.4, 8.15, 5.79, 10.4, 11.1, 
            10.4, 7.47, 5.08, 5.9, 11.8, 
            14.1, 11.7),
        cutdcotdist = cms.vdouble(0.0393, 0.0256, 0.00691, 0.0394, 0.0386, 
            0.039, 0.0325, 0.0384, 0.0382, 0.0245, 
            0.000281, 5.46e-05, 0.0342, 0.0232, 0.00107, 
            0.0178, 0.0193, 0.000758, 0.000108, 0.0248, 
            0.000458, 0.0129, 0.00119, 0.0182, 4.53e-05, 
            0.0189, 0.000928),
        cutsee = cms.vdouble(0.0143, 0.0105, 0.0123, 0.0324, 0.0307, 
            0.0301, 0.0109, 0.027, 0.0292, 0.0133, 
            0.0104, 0.0116, 0.0332, 0.0296, 0.031, 
            0.00981, 0.0307, 0.072, 0.0149, 0.0105, 
            0.011, 0.0342, 0.0307, 0.0303, 0.00954, 
            0.0265, 0.0101),
        cuteseedopcor = cms.vdouble(0.784, 0.366, 0.57, 0.911, 0.298, 
            0.645, 0.51, 0.497, 0.932, 0.835, 
            0.968, 0.969, 0.923, 0.898, 0.98, 
            0.63, 0.971, 1.0, 0.515, 0.963, 
            0.986, 0.823, 0.879, 1.01, 0.931, 
            0.937, 1.05),
        cutdphiin = cms.vdouble(0.0404, 0.0499, 0.263, 0.042, 0.0484, 
            0.241, 0.242, 0.231, 0.286, 0.0552, 
            0.0338, 0.154, 0.0623, 0.0183, 0.0392, 
            0.0547, 0.0588, 0.00654, 0.042, 0.0217, 
            0.0885, 0.0445, 0.0141, 0.0234, 0.065, 
            0.0258, 0.0346),
        cutet = cms.vdouble(-100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, 13.7, 13.2, 
            13.6, 14.2, 14.1, 13.9, 12.9, 
            14.9, 17.7)
    ),
    classbasedtightEleIDCutsV00 = cms.PSet(
        deltaPhiIn = cms.vdouble(0.032, 0.016, 0.0525, 0.09, 0.025, 
            0.035, 0.065, 0.092),
        hOverE = cms.vdouble(0.05, 0.042, 0.045, 0.0, 0.055, 
            0.037, 0.05, 0.0),
        sigmaEtaEta = cms.vdouble(0.0125, 0.011, 0.01, 0.0, 0.0265, 
            0.0252, 0.026, 0.0),
        deltaEtaIn = cms.vdouble(0.0055, 0.003, 0.0065, 0.0, 0.006, 
            0.0055, 0.0075, 0.0),
        eSeedOverPin = cms.vdouble(0.24, 0.94, 0.11, 0.0, 0.32, 
            0.83, 0.0, 0.0)
    ),
    classbasedtightEleIDCutsV01 = cms.PSet(
        deltaPhiIn = cms.vdouble(0.0225, 0.0114, 0.0234, 0.039, 0.0215, 
            0.0095, 0.0148, 0.0167),
        hOverE = cms.vdouble(0.056, 0.0221, 0.037, 0.0, 0.0268, 
            0.0102, 0.0104, 0.0),
        sigmaEtaEta = cms.vdouble(0.0095, 0.0094, 0.0094, 0.0, 0.026, 
            0.0257, 0.0246, 0.0),
        deltaEtaIn = cms.vdouble(0.0043, 0.00282, 0.0036, 0.0, 0.0066, 
            0.0049, 0.0041, 0.0),
        eSeedOverPin = cms.vdouble(0.32, 0.94, 0.221, 0.0, 0.74, 
            0.89, 0.66, 0.0)
    ),
    classbasedtightEleIDCutsV06 = cms.PSet(
        cutdetain = cms.vdouble(0.0116, 0.00449, 0.00938, 0.0184, 0.00678, 
            0.0109, 0.0252, 0.0268, 0.0139),
        cutiso_sum = cms.vdouble(15.5, 12.2, 12.2, 11.7, 7.16, 
            9.71, 8.66, 11.9, 2.98),
        cutip_gsf = cms.vdouble(0.0131, 0.0586, 0.0839, 0.0366, 0.452, 
            0.204, 0.0913, 0.0802, 0.0731),
        cutip_gsfl = cms.vdouble(0.0119, 0.0527, 0.0471, 0.0212, 0.233, 
            0.267, 0.109, 0.122, 0.0479),
        cuthoe = cms.vdouble(0.215, 0.0608, 0.147, 0.369, 0.0349, 
            0.102, 0.52, 0.422, 0.404),
        cutiso_sumoetl = cms.vdouble(6.21, 6.81, 5.3, 5.39, 2.73, 
            4.73, 4.84, 3.46, 3.73),
        cutfmishits = cms.vdouble(1.5, 1.5, 1.5, 2.5, 2.5, 
            1.5, 1.5, 2.5, 0.5),
        cuthoel = cms.vdouble(0.228, 0.0836, 0.143, 0.37, 0.0392, 
            0.0979, 0.3, 0.381, 0.339),
        cutdphiin = cms.vdouble(0.0897, 0.0993, 0.295, 0.0979, 0.151, 
            0.252, 0.341, 0.308, 0.328),
        cutseel = cms.vdouble(0.0132, 0.0117, 0.0112, 0.0387, 0.0281, 
            0.0287, 0.00987, 0.0296, 0.0544),
        cutiso_sumoet = cms.vdouble(11.9, 7.81, 6.28, 8.92, 4.65, 
            5.49, 9.36, 8.84, 5.94),
        cutdcotdist = cms.vdouble(9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0),
        cutsee = cms.vdouble(0.0145, 0.0116, 0.012, 0.039, 0.0297, 
            0.0311, 0.00987, 0.0347, 0.0917),
        cuteseedopcor = cms.vdouble(0.637, 0.943, 0.742, 0.748, 0.763, 
            0.631, 0.214, 0.873, 0.473),
        cutdphiinl = cms.vdouble(0.061, 0.14, 0.286, 0.0921, 0.197, 
            0.24, 0.333, 0.303, 0.258),
        cutdetainl = cms.vdouble(0.00816, 0.00401, 0.0081, 0.019, 0.00588, 
            0.00893, 0.0171, 0.0434, 0.0143)
    ),
    classbasedtightEleIDCutsV04 = cms.PSet(
        cutdetain = cms.vdouble(0.00811, 0.00341, 0.00633, 0.0103, 0.00667, 
            0.01, 0.0106, 0.0145, 0.0163, 0.0076, 
            0.00259, 0.00511, 0.00941, 0.0043, 0.00857, 
            0.012, 0.0169, 0.00172, 0.00861, 0.00362, 
            0.00601, 0.00925, 0.00489, 0.00832, 0.0119, 
            0.0169, 0.000996),
        cutiso_sum = cms.vdouble(11.8, 8.31, 6.26, 6.18, 3.28, 
            4.38, 4.17, 5.4, 1.57, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0),
        cutip_gsf = cms.vdouble(0.0213, 0.0422, 0.0632, 0.0361, 0.073, 
            0.126, 0.171, 0.119, 0.0372, 0.0131, 
            0.0146, 0.0564, 0.0152, 0.0222, 0.0268, 
            0.0314, 0.0884, 0.00374, 0.00852, 0.00761, 
            0.0143, 0.0106, 0.0127, 0.0119, 0.0123, 
            0.0235, 0.00363),
        cuthoe = cms.vdouble(0.0783, 0.0387, 0.105, 0.118, 0.0227, 
            0.062, 0.13, 2.47, 0.38, 0.0888, 
            0.0503, 0.0955, 0.0741, 0.015, 0.03, 
            0.589, 1.13, 0.612, 0.0494, 0.0461, 
            0.0292, 0.0369, 0.0113, 0.0145, 0.124, 
            2.05, 0.61),
        cutfmishits = cms.vdouble(2.5, 1.5, 1.5, 1.5, 1.5, 
            0.5, 2.5, 0.5, 0.5, 2.5, 
            1.5, 0.5, 0.5, 0.5, 0.5, 
            0.5, 0.5, -0.5, 2.5, 1.5, 
            0.5, 0.5, 0.5, 0.5, 0.5, 
            0.5, 0.5),
        cutiso_sumoet = cms.vdouble(13.7, 11.6, 7.14, 9.98, 3.52, 
            4.87, 6.24, 7.96, 2.53, 11.2, 
            11.9, 7.88, 8.16, 5.58, 5.03, 
            11.4, 8.15, 5.79, 10.4, 11.1, 
            10.4, 7.47, 5.08, 5.9, 11.8, 
            14.1, 11.7),
        cutdcotdist = cms.vdouble(0.0393, 0.0256, 0.00691, 0.0394, 0.0386, 
            0.039, 0.0325, 0.0384, 0.0382, 0.0245, 
            0.000281, 5.46e-05, 0.0342, 0.0232, 0.00107, 
            0.0178, 0.0193, 0.000758, 0.000108, 0.0248, 
            0.000458, 0.0129, 0.00119, 0.0182, 4.53e-05, 
            0.0189, 0.000928),
        cutsee = cms.vdouble(0.0143, 0.0105, 0.0123, 0.0324, 0.0307, 
            0.0301, 0.0109, 0.027, 0.0292, 0.0133, 
            0.0104, 0.0116, 0.0332, 0.0296, 0.031, 
            0.00981, 0.0307, 0.072, 0.0149, 0.0105, 
            0.011, 0.0342, 0.0307, 0.0303, 0.00954, 
            0.0265, 0.0101),
        cuteseedopcor = cms.vdouble(0.784, 0.366, 0.57, 0.911, 0.298, 
            0.645, 0.51, 0.497, 0.932, 0.835, 
            0.968, 0.969, 0.923, 0.898, 0.98, 
            0.63, 0.971, 1.0, 0.515, 0.963, 
            0.986, 0.823, 0.879, 1.01, 0.931, 
            0.937, 1.05),
        cutdphiin = cms.vdouble(0.0404, 0.0499, 0.263, 0.042, 0.0484, 
            0.241, 0.242, 0.231, 0.286, 0.0552, 
            0.0338, 0.154, 0.0623, 0.0183, 0.0392, 
            0.0547, 0.0588, 0.00654, 0.042, 0.0217, 
            0.0885, 0.0445, 0.0141, 0.0234, 0.065, 
            0.0258, 0.0346),
        cutet = cms.vdouble(-100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, 13.7, 13.2, 
            13.6, 14.2, 14.1, 13.9, 12.9, 
            14.9, 17.7)
    ),
    electronIDType = cms.string('robust'),
    robusttightEleIDCutsV04 = cms.PSet(
        barrel = cms.vdouble(0.0201, 0.0102, 0.0211, 0.00606, -1, 
            -1, 2.34, 3.24, 4.51, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.00253, 0.0291, 0.022, 0.0032, -1, 
            -1, 0.826, 2.7, 0.255, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    electronVersion = cms.string(''),
    robusttightEleIDCutsV00 = cms.PSet(
        barrel = cms.vdouble(0.015, 0.0092, 0.02, 0.0025, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.018, 0.025, 0.02, 0.004, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robusttightEleIDCutsV01 = cms.PSet(
        barrel = cms.vdouble(0.01, 0.0099, 0.025, 0.004, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.01, 0.028, 0.02, 0.0066, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    reducedBarrelRecHitCollection = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
    robusttightEleIDCutsV03 = cms.PSet(
        barrel = cms.vdouble(0.0201, 0.0102, 0.0211, 0.00606, -1, 
            -1, 2.34, 3.24, 4.51, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.00253, 0.0291, 0.022, 0.0032, -1, 
            -1, 0.826, 2.7, 0.255, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    reducedEndcapRecHitCollection = cms.InputTag("ecalRecHit","EcalRecHitsEE"),
    verticesCollection = cms.InputTag("offlinePrimaryVerticesWithBS"),
    classbasedlooseEleIDCuts = cms.PSet(
        cutdetain = cms.vdouble(0.0137, 0.00678, 0.0241, 0.0187, 0.0161, 
            0.0224, 0.0252, 0.0308, 0.0273),
        cutiso_sum = cms.vdouble(33.0, 17.0, 17.9, 18.8, 8.55, 
            12.5, 17.6, 18.5, 2.98),
        cutip_gsf = cms.vdouble(0.0551, 0.0765, 0.143, 0.0874, 0.594, 
            0.37, 0.0913, 1.15, 0.231),
        cutip_gsfl = cms.vdouble(0.0186, 0.0759, 0.138, 0.0473, 0.62, 
            0.304, 0.109, 0.775, 0.0479),
        cuthoe = cms.vdouble(0.247, 0.137, 0.147, 0.371, 0.0588, 
            0.147, 0.52, 0.452, 0.404),
        cutiso_sumoetl = cms.vdouble(11.3, 9.05, 9.07, 9.94, 5.25, 
            6.15, 10.7, 10.8, 4.4),
        cutfmishits = cms.vdouble(4.5, 1.5, 1.5, 2.5, 2.5, 
            1.5, 4.5, 3.5, 3.5),
        cuthoel = cms.vdouble(0.236, 0.126, 0.147, 0.375, 0.0392, 
            0.145, 0.365, 0.383, 0.384),
        cutdphiin = cms.vdouble(0.0897, 0.262, 0.353, 0.116, 0.357, 
            0.319, 0.342, 0.404, 0.336),
        cutseel = cms.vdouble(0.0164, 0.0118, 0.015, 0.0523, 0.0326, 
            0.0456, 0.0185, 0.0589, 0.0544),
        cutiso_sumoet = cms.vdouble(34.5, 12.7, 12.1, 19.9, 6.35, 
            8.85, 14.0, 10.5, 9.74),
        cutdcotdist = cms.vdouble(9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0),
        cutsee = cms.vdouble(0.0176, 0.0125, 0.0181, 0.0415, 0.0364, 
            0.0418, 0.0146, 0.0678, 0.133),
        cuteseedopcor = cms.vdouble(0.63, 0.82, 0.401, 0.718, 0.4, 
            0.458, 0.15, 0.664, 0.373),
        cutdphiinl = cms.vdouble(0.0747, 0.25, 0.356, 0.0956, 0.347, 
            0.326, 0.333, 0.647, 0.289),
        cutdetainl = cms.vdouble(0.0124, 0.00503, 0.0257, 0.0228, 0.0118, 
            0.0178, 0.0188, 0.14, 0.024)
    ),
    robusttightEleIDCutsV02 = cms.PSet(
        barrel = cms.vdouble(0.0201, 0.0102, 0.0211, 0.00606, -1, 
            -1, 2.34, 3.24, 4.51, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.00253, 0.0291, 0.022, 0.0032, -1, 
            -1, 0.826, 2.7, 0.255, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robusthighenergyEleIDCutsV04 = cms.PSet(
        barrel = cms.vdouble(0.05, 9999, 0.09, 0.005, 0.94, 
            0.83, 7.5, 2, 0.03, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.05, 0.03, 0.09, 0.007, -1, 
            -1, 15, 2.5, 0.03, 2.5, 
            0, 0.5, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robusthighenergyEleIDCutsV01 = cms.PSet(
        barrel = cms.vdouble(0.05, 9999, 0.09, 0.005, 0.94, 
            0.83, 9999.0, 9999.0, 0, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.05, 0.0275, 0.09, 0.007, -1, 
            -1, 9999.0, 9999.0, 0, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robusthighenergyEleIDCutsV00 = cms.PSet(
        barrel = cms.vdouble(0.05, 0.011, 0.09, 0.005, -1, 
            -1, 9999.0, 9999.0, 0, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.1, 0.0275, 0.09, 0.007, -1, 
            -1, 9999.0, 9999.0, 0, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robusthighenergyEleIDCutsV03 = cms.PSet(
        barrel = cms.vdouble(0.05, 9999, 0.09, 0.005, 0.94, 
            0.83, 7.5, 2, 0.03, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.05, 0.03, 0.09, 0.007, -1, 
            -1, 15, 2.5, 0.03, 2.5, 
            0, 0.5, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robusthighenergyEleIDCutsV02 = cms.PSet(
        barrel = cms.vdouble(0.05, 9999, 0.09, 0.005, 0.94, 
            0.83, 7.5, 2, 0.03, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.05, 0.03, 0.09, 0.007, -1, 
            -1, 15, 2.5, 0.03, 2.5, 
            0, 0.5, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    classbasedlooseEleIDCutsV00 = cms.PSet(
        deltaPhiIn = cms.vdouble(0.05, 0.025, 0.053, 0.09, 0.07, 
            0.03, 0.092, 0.092),
        hOverE = cms.vdouble(0.115, 0.1, 0.055, 0.0, 0.145, 
            0.12, 0.15, 0.0),
        sigmaEtaEta = cms.vdouble(0.014, 0.012, 0.0115, 0.0, 0.0275, 
            0.0265, 0.0265, 0.0),
        deltaEtaIn = cms.vdouble(0.009, 0.0045, 0.0085, 0.0, 0.0105, 
            0.0068, 0.01, 0.0),
        eSeedOverPin = cms.vdouble(0.11, 0.91, 0.11, 0.0, 0.0, 
            0.85, 0.0, 0.0)
    ),
    classbasedlooseEleIDCutsV01 = cms.PSet(
        deltaPhiIn = cms.vdouble(0.053, 0.0189, 0.059, 0.099, 0.0278, 
            0.0157, 0.042, 0.08),
        hOverE = cms.vdouble(0.076, 0.033, 0.07, 0.0, 0.083, 
            0.0148, 0.033, 0.0),
        sigmaEtaEta = cms.vdouble(0.0101, 0.0095, 0.0097, 0.0, 0.0271, 
            0.0267, 0.0259, 0.0),
        deltaEtaIn = cms.vdouble(0.0078, 0.00259, 0.0062, 0.0, 0.0078, 
            0.0061, 0.0061, 0.0),
        eSeedOverPin = cms.vdouble(0.3, 0.92, 0.211, 0.0, 0.42, 
            0.88, 0.68, 0.0)
    ),
    classbasedlooseEleIDCutsV02 = cms.PSet(
        cutisohcal = cms.vdouble(13.5, 9.93, 7.56, 14.8, 8.1, 
            10.8, 42.7, 20.1, 9.11, 10.4, 
            6.89, 5.59, 8.53, 9.59, 24.2, 
            2.78, 8.67, 0.288),
        cutmishits = cms.vdouble(5.5, 1.5, 5.5, 2.5, 2.5, 
            2.5, 3.5, 5.5, 0.5, 1.5, 
            2.5, 0.5, 1.5, 1.5, 0.5, 
            0.5, 0.5, 0.5),
        cuthoe = cms.vdouble(0.0887, 0.0934, 0.0949, 0.0986, 0.0431, 
            0.0878, 0.097, 0.0509, 0.098, 0.0991, 
            0.0321, 0.0928, 0.0663, 0.0717, 0.0966, 
            0.0758, 0.0149, 0.0131),
        cutdeta = cms.vdouble(0.00958, 0.00406, 0.0122, 0.0137, 0.00837, 
            0.0127, 0.011, 0.00336, 0.00977, 0.015, 
            0.00675, 0.0109, 0.014, 0.00508, 0.0109, 
            0.0146, 0.00506, 0.0127),
        cuteopin = cms.vdouble(0.878, 0.802, 0.814, 0.942, 0.735, 
            0.774, 0.829, 0.909, 0.829, 0.813, 
            0.86, 0.897, 0.817, 0.831, 0.818, 
            0.861, 0.787, 0.789),
        cutip = cms.vdouble(0.0246, 0.076, 0.0966, 0.0885, 0.441, 
            0.205, 0.0292, 0.0293, 0.0619, 0.0251, 
            0.159, 0.0815, 7.29, 0.0106, 5.76, 
            6.89, 1.27, 5.89),
        cutisotk = cms.vdouble(24.3, 8.45, 14.4, 27.8, 6.02, 
            10.5, 14.1, 10.2, 14.5, 19.1, 
            6.1, 14.1, 8.59, 8.33, 8.3, 
            8.93, 8.6, 16.0),
        cutsee = cms.vdouble(0.0172, 0.0115, 0.0143, 0.0344, 0.0295, 
            0.0304, 0.0145, 0.0108, 0.0128, 0.0347, 
            0.0307, 0.0316, 0.018, 0.011, 0.0132, 
            0.0349, 0.031, 0.0327),
        cutdphi = cms.vdouble(0.0372, 0.114, 0.118, 0.0488, 0.117, 
            0.119, 0.0606, 0.0548, 0.117, 0.07, 
            0.0355, 0.117, 0.088, 0.045, 0.118, 
            0.0919, 0.0236, 0.0515),
        cutisoecal = cms.vdouble(33.4, 28.1, 7.32, 27.4, 7.33, 
            21.7, 93.8, 102.0, 12.1, 26.0, 
            8.91, 10.0, 16.1, 31.3, 16.9, 
            15.4, 13.3, 37.7)
    ),
    classbasedlooseEleIDCutsV03 = cms.PSet(
        cutdetain = cms.vdouble(0.00989, 0.00484, 0.0146, 0.0146, 0.00902, 
            0.0172, 0.0137, 0.0477, 0.0275, 0.00967, 
            0.00377, 0.00924, 0.013, 0.00666, 0.0123, 
            0.0125, 0.0228, 0.0112, 0.0106, 0.0038, 
            0.00897, 0.0139, 0.00667, 0.0122, 0.0122, 
            0.0193, 0.00239),
        cutiso_sum = cms.vdouble(31.5, 10.3, 8.8, 11.0, 6.13, 
            6.94, 7.52, 9.0, 3.5, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0),
        cutip_gsf = cms.vdouble(0.0431, 0.0767, 0.139, 0.101, 0.149, 
            0.154, 0.932, 0.15, 0.124, 0.0238, 
            0.0467, 0.0759, 0.0369, 0.147, 0.0986, 
            0.0626, 0.195, 0.116, 0.0122, 0.0125, 
            0.0693, 0.0162, 0.089, 0.0673, 0.0467, 
            0.0651, 0.0221),
        cuthoe = cms.vdouble(0.166, 0.0771, 0.144, 0.37, 0.0497, 
            0.139, 0.401, 2.68, 0.516, 0.234, 
            0.0556, 0.144, 0.368, 0.031, 0.12, 
            0.602, 2.01, 1.05, 0.104, 0.063, 
            0.0565, 0.38, 0.0192, 0.0294, 0.537, 
            4.65, 1.87),
        cutfmishits = cms.vdouble(4.5, 1.5, 1.5, 2.5, 2.5, 
            1.5, 2.5, 2.5, 1.5, 2.5, 
            1.5, 1.5, 1.5, 1.5, 0.5, 
            2.5, 2.5, 0.5, 2.5, 1.5, 
            0.5, 1.5, 1.5, 0.5, 2.5, 
            0.5, 0.5),
        cutiso_sumoet = cms.vdouble(28.9, 15.3, 12.0, 18.3, 7.17, 
            9.42, 11.0, 9.81, 3.94, 22.7, 
            15.9, 12.3, 17.0, 7.58, 8.89, 
            15.2, 12.7, 6.17, 20.8, 21.2, 
            17.2, 15.5, 9.37, 10.6, 19.8, 
            22.1, 15.6),
        cutdcotdist = cms.vdouble(0.0393, 0.0392, 0.0397, 0.0394, 0.0393, 
            0.039, 0.0378, 0.0388, 0.0382, 0.0385, 
            0.0167, 0.00325, 0.0394, 0.0387, 0.0388, 
            0.0227, 0.0258, 0.0127, 0.0298, 0.03, 
            0.00946, 0.039, 0.0231, 0.0278, 0.00162, 
            0.0367, 0.0199),
        cutsee = cms.vdouble(0.0175, 0.0127, 0.0177, 0.0373, 0.0314, 
            0.0329, 0.0157, 0.0409, 0.14, 0.0169, 
            0.0106, 0.0142, 0.0363, 0.0322, 0.0354, 
            0.0117, 0.0372, 28.2, 0.0171, 0.0113, 
            0.014, 0.0403, 0.0323, 0.0411, 0.0104, 
            0.0436, 0.0114),
        cuteseedopcor = cms.vdouble(0.78, 0.302, 0.483, 0.904, 0.168, 
            0.645, 0.108, 0.284, 0.324, 0.591, 
            0.286, 0.488, 0.813, 0.791, 0.672, 
            0.398, 0.834, 0.878, 0.515, 0.937, 
            0.806, 0.816, 0.85, 0.507, 0.367, 
            0.83, 0.648),
        cutdphiin = cms.vdouble(0.041, 0.275, 0.365, 0.047, 0.273, 
            0.296, 0.329, 0.465, 0.627, 0.0581, 
            0.0954, 0.327, 0.0702, 0.0582, 0.279, 
            0.117, 0.318, 0.246, 0.0821, 0.052, 
            0.292, 0.116, 0.0435, 0.312, 0.118, 
            0.296, 0.0459),
        cutet = cms.vdouble(-100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, 12.0, 12.0, 
            12.0, 12.0, 12.0, 12.0, 12.0, 
            12.0, 12.5)
    ),
    classbasedlooseEleIDCutsV04 = cms.PSet(
        cutdetain = cms.vdouble(0.00989, 0.00484, 0.0146, 0.0146, 0.00902, 
            0.0172, 0.0137, 0.0477, 0.0275, 0.00967, 
            0.00377, 0.00924, 0.013, 0.00666, 0.0123, 
            0.0125, 0.0228, 0.0112, 0.0106, 0.0038, 
            0.00897, 0.0139, 0.00667, 0.0122, 0.0122, 
            0.0193, 0.00239),
        cutiso_sum = cms.vdouble(31.5, 10.3, 8.8, 11.0, 6.13, 
            6.94, 7.52, 9.0, 3.5, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0),
        cutip_gsf = cms.vdouble(0.0431, 0.0767, 0.139, 0.101, 0.149, 
            0.154, 0.932, 0.15, 0.124, 0.0238, 
            0.0467, 0.0759, 0.0369, 0.147, 0.0986, 
            0.0626, 0.195, 0.116, 0.0122, 0.0125, 
            0.0693, 0.0162, 0.089, 0.0673, 0.0467, 
            0.0651, 0.0221),
        cuthoe = cms.vdouble(0.166, 0.0771, 0.144, 0.37, 0.0497, 
            0.139, 0.401, 2.68, 0.516, 0.234, 
            0.0556, 0.144, 0.368, 0.031, 0.12, 
            0.602, 2.01, 1.05, 0.104, 0.063, 
            0.0565, 0.38, 0.0192, 0.0294, 0.537, 
            4.65, 1.87),
        cutfmishits = cms.vdouble(4.5, 1.5, 1.5, 2.5, 2.5, 
            1.5, 2.5, 2.5, 1.5, 2.5, 
            1.5, 1.5, 1.5, 1.5, 0.5, 
            2.5, 2.5, 0.5, 2.5, 1.5, 
            0.5, 1.5, 1.5, 0.5, 2.5, 
            0.5, 0.5),
        cutiso_sumoet = cms.vdouble(28.9, 15.3, 12.0, 18.3, 7.17, 
            9.42, 11.0, 9.81, 3.94, 22.7, 
            15.9, 12.3, 17.0, 7.58, 8.89, 
            15.2, 12.7, 6.17, 20.8, 21.2, 
            17.2, 15.5, 9.37, 10.6, 19.8, 
            22.1, 15.6),
        cutdcotdist = cms.vdouble(0.0393, 0.0392, 0.0397, 0.0394, 0.0393, 
            0.039, 0.0378, 0.0388, 0.0382, 0.0385, 
            0.0167, 0.00325, 0.0394, 0.0387, 0.0388, 
            0.0227, 0.0258, 0.0127, 0.0298, 0.03, 
            0.00946, 0.039, 0.0231, 0.0278, 0.00162, 
            0.0367, 0.0199),
        cutsee = cms.vdouble(0.0175, 0.0127, 0.0177, 0.0373, 0.0314, 
            0.0329, 0.0157, 0.0409, 0.14, 0.0169, 
            0.0106, 0.0142, 0.0363, 0.0322, 0.0354, 
            0.0117, 0.0372, 28.2, 0.0171, 0.0113, 
            0.014, 0.0403, 0.0323, 0.0411, 0.0104, 
            0.0436, 0.0114),
        cuteseedopcor = cms.vdouble(0.78, 0.302, 0.483, 0.904, 0.168, 
            0.645, 0.108, 0.284, 0.324, 0.591, 
            0.286, 0.488, 0.813, 0.791, 0.672, 
            0.398, 0.834, 0.878, 0.515, 0.937, 
            0.806, 0.816, 0.85, 0.507, 0.367, 
            0.83, 0.648),
        cutdphiin = cms.vdouble(0.041, 0.275, 0.365, 0.047, 0.273, 
            0.296, 0.329, 0.465, 0.627, 0.0581, 
            0.0954, 0.327, 0.0702, 0.0582, 0.279, 
            0.117, 0.318, 0.246, 0.0821, 0.052, 
            0.292, 0.116, 0.0435, 0.312, 0.118, 
            0.296, 0.0459),
        cutet = cms.vdouble(-100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, 12.0, 12.0, 
            12.0, 12.0, 12.0, 12.0, 12.0, 
            12.0, 12.5)
    ),
    classbasedlooseEleIDCutsV06 = cms.PSet(
        cutdetain = cms.vdouble(0.0137, 0.00678, 0.0241, 0.0187, 0.0161, 
            0.0224, 0.0252, 0.0308, 0.0273),
        cutiso_sum = cms.vdouble(33.0, 17.0, 17.9, 18.8, 8.55, 
            12.5, 17.6, 18.5, 2.98),
        cutip_gsf = cms.vdouble(0.0551, 0.0765, 0.143, 0.0874, 0.594, 
            0.37, 0.0913, 1.15, 0.231),
        cutip_gsfl = cms.vdouble(0.0186, 0.0759, 0.138, 0.0473, 0.62, 
            0.304, 0.109, 0.775, 0.0479),
        cuthoe = cms.vdouble(0.247, 0.137, 0.147, 0.371, 0.0588, 
            0.147, 0.52, 0.452, 0.404),
        cutiso_sumoetl = cms.vdouble(11.3, 9.05, 9.07, 9.94, 5.25, 
            6.15, 10.7, 10.8, 4.4),
        cutfmishits = cms.vdouble(4.5, 1.5, 1.5, 2.5, 2.5, 
            1.5, 4.5, 3.5, 3.5),
        cuthoel = cms.vdouble(0.236, 0.126, 0.147, 0.375, 0.0392, 
            0.145, 0.365, 0.383, 0.384),
        cutdphiin = cms.vdouble(0.0897, 0.262, 0.353, 0.116, 0.357, 
            0.319, 0.342, 0.404, 0.336),
        cutseel = cms.vdouble(0.0164, 0.0118, 0.015, 0.0523, 0.0326, 
            0.0456, 0.0185, 0.0589, 0.0544),
        cutiso_sumoet = cms.vdouble(34.5, 12.7, 12.1, 19.9, 6.35, 
            8.85, 14.0, 10.5, 9.74),
        cutdcotdist = cms.vdouble(9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0),
        cutsee = cms.vdouble(0.0176, 0.0125, 0.0181, 0.0415, 0.0364, 
            0.0418, 0.0146, 0.0678, 0.133),
        cuteseedopcor = cms.vdouble(0.63, 0.82, 0.401, 0.718, 0.4, 
            0.458, 0.15, 0.664, 0.373),
        cutdphiinl = cms.vdouble(0.0747, 0.25, 0.356, 0.0956, 0.347, 
            0.326, 0.333, 0.647, 0.289),
        cutdetainl = cms.vdouble(0.0124, 0.00503, 0.0257, 0.0228, 0.0118, 
            0.0178, 0.0188, 0.14, 0.024)
    ),
    src = cms.InputTag("gsfElectrons"),
    robusttightEleIDCuts = cms.PSet(
        barrel = cms.vdouble(0.0201, 0.0102, 0.0211, 0.00606, -1, 
            -1, 2.34, 3.24, 4.51, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.00253, 0.0291, 0.022, 0.0032, -1, 
            -1, 0.826, 2.7, 0.255, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    classbasedtightEleIDCuts = cms.PSet(
        cutdetain = cms.vdouble(0.0116, 0.00449, 0.00938, 0.0184, 0.00678, 
            0.0109, 0.0252, 0.0268, 0.0139),
        cutiso_sum = cms.vdouble(15.5, 12.2, 12.2, 11.7, 7.16, 
            9.71, 8.66, 11.9, 2.98),
        cutip_gsf = cms.vdouble(0.0131, 0.0586, 0.0839, 0.0366, 0.452, 
            0.204, 0.0913, 0.0802, 0.0731),
        cutip_gsfl = cms.vdouble(0.0119, 0.0527, 0.0471, 0.0212, 0.233, 
            0.267, 0.109, 0.122, 0.0479),
        cuthoe = cms.vdouble(0.215, 0.0608, 0.147, 0.369, 0.0349, 
            0.102, 0.52, 0.422, 0.404),
        cutiso_sumoetl = cms.vdouble(6.21, 6.81, 5.3, 5.39, 2.73, 
            4.73, 4.84, 3.46, 3.73),
        cutfmishits = cms.vdouble(1.5, 1.5, 1.5, 2.5, 2.5, 
            1.5, 1.5, 2.5, 0.5),
        cuthoel = cms.vdouble(0.228, 0.0836, 0.143, 0.37, 0.0392, 
            0.0979, 0.3, 0.381, 0.339),
        cutdphiin = cms.vdouble(0.0897, 0.0993, 0.295, 0.0979, 0.151, 
            0.252, 0.341, 0.308, 0.328),
        cutseel = cms.vdouble(0.0132, 0.0117, 0.0112, 0.0387, 0.0281, 
            0.0287, 0.00987, 0.0296, 0.0544),
        cutiso_sumoet = cms.vdouble(11.9, 7.81, 6.28, 8.92, 4.65, 
            5.49, 9.36, 8.84, 5.94),
        cutdcotdist = cms.vdouble(9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0),
        cutsee = cms.vdouble(0.0145, 0.0116, 0.012, 0.039, 0.0297, 
            0.0311, 0.00987, 0.0347, 0.0917),
        cuteseedopcor = cms.vdouble(0.637, 0.943, 0.742, 0.748, 0.763, 
            0.631, 0.214, 0.873, 0.473),
        cutdphiinl = cms.vdouble(0.061, 0.14, 0.286, 0.0921, 0.197, 
            0.24, 0.333, 0.303, 0.258),
        cutdetainl = cms.vdouble(0.00816, 0.00401, 0.0081, 0.019, 0.00588, 
            0.00893, 0.0171, 0.0434, 0.0143)
    ),
    algorithm = cms.string('eIDCB'),
    robusthighenergyEleIDCuts = cms.PSet(
        barrel = cms.vdouble(0.05, 9999, 0.09, 0.005, 0.94, 
            0.83, 7.5, 2, 0.03, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.05, 0.03, 0.09, 0.007, -1, 
            -1, 15, 2.5, 0.03, 2.5, 
            0, 0.5, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robustlooseEleIDCuts = cms.PSet(
        barrel = cms.vdouble(0.05, 0.0103, 0.8, 0.00688, -1, 
            -1, 7.33, 4.68, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.0389, 0.0307, 0.7, 0.00944, -1, 
            -1, 7.76, 3.09, 2.23, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robustlooseEleIDCutsV02 = cms.PSet(
        barrel = cms.vdouble(0.05, 0.0103, 0.8, 0.00688, -1, 
            -1, 7.33, 4.68, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.0389, 0.0307, 0.7, 0.00944, -1, 
            -1, 7.76, 3.09, 2.23, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robustlooseEleIDCutsV03 = cms.PSet(
        barrel = cms.vdouble(0.05, 0.0103, 0.8, 0.00688, -1, 
            -1, 7.33, 4.68, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.0389, 0.0307, 0.7, 0.00944, -1, 
            -1, 7.76, 3.09, 2.23, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robustlooseEleIDCutsV00 = cms.PSet(
        barrel = cms.vdouble(0.115, 0.014, 0.09, 0.009, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.15, 0.0275, 0.092, 0.0105, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robustlooseEleIDCutsV01 = cms.PSet(
        barrel = cms.vdouble(0.075, 0.0132, 0.058, 0.0077, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.083, 0.027, 0.042, 0.01, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robustlooseEleIDCutsV04 = cms.PSet(
        barrel = cms.vdouble(0.05, 0.0103, 0.8, 0.00688, -1, 
            -1, 7.33, 4.68, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.0389, 0.0307, 0.7, 0.00944, -1, 
            -1, 7.76, 3.09, 2.23, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    additionalCategories = cms.bool(True),
    etBinning = cms.bool(True)
)


process.eidTight = cms.EDProducer("EleIdCutBasedExtProducer",
    electronQuality = cms.string('tight'),
    classbasedtightEleIDCutsV02 = cms.PSet(
        cutisohcal = cms.vdouble(10.9, 7.01, 8.75, 3.51, 7.75, 
            1.62, 11.6, 9.9, 4.97, 5.33, 
            3.18, 2.32, 0.164, 5.46, 12.0, 
            0.00604, 4.1, 0.000628),
        cutmishits = cms.vdouble(5.5, 1.5, 0.5, 1.5, 2.5, 
            0.5, 3.5, 5.5, 0.5, 0.5, 
            0.5, 0.5, 0.5, 1.5, 0.5, 
            0.5, 0.5, 0.5),
        cuthoe = cms.vdouble(0.0871, 0.0289, 0.0783, 0.0946, 0.0245, 
            0.0363, 0.0671, 0.048, 0.0614, 0.0924, 
            0.0158, 0.049, 0.0382, 0.0915, 0.0451, 
            0.0452, 0.00196, 0.0043),
        cutdeta = cms.vdouble(0.00915, 0.00302, 0.0061, 0.0135, 0.00565, 
            0.00793, 0.0102, 0.00266, 0.0106, 0.00903, 
            0.00766, 0.00723, 0.0116, 0.00203, 0.00659, 
            0.0148, 0.00555, 0.0128),
        cuteopin = cms.vdouble(0.878, 0.859, 0.874, 0.944, 0.737, 
            0.773, 0.86, 0.967, 0.917, 0.812, 
            0.915, 1.01, 0.847, 0.953, 0.979, 
            0.841, 0.771, 1.09),
        cutip = cms.vdouble(0.0239, 0.027, 0.0768, 0.0231, 0.178, 
            0.0957, 0.0102, 0.0168, 0.043, 0.0166, 
            0.0594, 0.0308, 2.1, 0.00527, 3.17, 
            4.91, 0.769, 5.9),
        cutisotk = cms.vdouble(6.53, 4.6, 6.0, 8.63, 3.11, 
            7.77, 5.42, 4.81, 4.06, 6.47, 
            2.8, 3.45, 5.29, 5.18, 15.4, 
            5.38, 4.47, 0.0347),
        cutsee = cms.vdouble(0.0131, 0.0106, 0.0115, 0.0306, 0.028, 
            0.0293, 0.0131, 0.0106, 0.0115, 0.0317, 
            0.029, 0.0289, 0.0142, 0.0106, 0.0103, 
            0.035, 0.0296, 0.0333),
        cutdphi = cms.vdouble(0.0369, 0.0307, 0.117, 0.0475, 0.0216, 
            0.117, 0.0372, 0.0246, 0.0426, 0.0612, 
            0.0142, 0.039, 0.0737, 0.0566, 0.0359, 
            0.0187, 0.012, 0.0358),
        cutisoecal = cms.vdouble(20.0, 27.2, 4.48, 13.5, 4.56, 
            3.19, 12.2, 13.1, 7.42, 7.67, 
            4.12, 4.85, 10.1, 12.4, 11.1, 
            11.0, 10.6, 13.4)
    ),
    classbasedtightEleIDCutsV03 = cms.PSet(
        cutdetain = cms.vdouble(0.00811, 0.00341, 0.00633, 0.0103, 0.00667, 
            0.01, 0.0106, 0.0145, 0.0163, 0.0076, 
            0.00259, 0.00511, 0.00941, 0.0043, 0.00857, 
            0.012, 0.0169, 0.00172, 0.00861, 0.00362, 
            0.00601, 0.00925, 0.00489, 0.00832, 0.0119, 
            0.0169, 0.000996),
        cutiso_sum = cms.vdouble(11.8, 8.31, 6.26, 6.18, 3.28, 
            4.38, 4.17, 5.4, 1.57, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0),
        cutip_gsf = cms.vdouble(0.0213, 0.0422, 0.0632, 0.0361, 0.073, 
            0.126, 0.171, 0.119, 0.0372, 0.0131, 
            0.0146, 0.0564, 0.0152, 0.0222, 0.0268, 
            0.0314, 0.0884, 0.00374, 0.00852, 0.00761, 
            0.0143, 0.0106, 0.0127, 0.0119, 0.0123, 
            0.0235, 0.00363),
        cuthoe = cms.vdouble(0.0783, 0.0387, 0.105, 0.118, 0.0227, 
            0.062, 0.13, 2.47, 0.38, 0.0888, 
            0.0503, 0.0955, 0.0741, 0.015, 0.03, 
            0.589, 1.13, 0.612, 0.0494, 0.0461, 
            0.0292, 0.0369, 0.0113, 0.0145, 0.124, 
            2.05, 0.61),
        cutfmishits = cms.vdouble(2.5, 1.5, 1.5, 1.5, 1.5, 
            0.5, 2.5, 0.5, 0.5, 2.5, 
            1.5, 0.5, 0.5, 0.5, 0.5, 
            0.5, 0.5, -0.5, 2.5, 1.5, 
            0.5, 0.5, 0.5, 0.5, 0.5, 
            0.5, 0.5),
        cutiso_sumoet = cms.vdouble(13.7, 11.6, 7.14, 9.98, 3.52, 
            4.87, 6.24, 7.96, 2.53, 11.2, 
            11.9, 7.88, 8.16, 5.58, 5.03, 
            11.4, 8.15, 5.79, 10.4, 11.1, 
            10.4, 7.47, 5.08, 5.9, 11.8, 
            14.1, 11.7),
        cutdcotdist = cms.vdouble(0.0393, 0.0256, 0.00691, 0.0394, 0.0386, 
            0.039, 0.0325, 0.0384, 0.0382, 0.0245, 
            0.000281, 5.46e-05, 0.0342, 0.0232, 0.00107, 
            0.0178, 0.0193, 0.000758, 0.000108, 0.0248, 
            0.000458, 0.0129, 0.00119, 0.0182, 4.53e-05, 
            0.0189, 0.000928),
        cutsee = cms.vdouble(0.0143, 0.0105, 0.0123, 0.0324, 0.0307, 
            0.0301, 0.0109, 0.027, 0.0292, 0.0133, 
            0.0104, 0.0116, 0.0332, 0.0296, 0.031, 
            0.00981, 0.0307, 0.072, 0.0149, 0.0105, 
            0.011, 0.0342, 0.0307, 0.0303, 0.00954, 
            0.0265, 0.0101),
        cuteseedopcor = cms.vdouble(0.784, 0.366, 0.57, 0.911, 0.298, 
            0.645, 0.51, 0.497, 0.932, 0.835, 
            0.968, 0.969, 0.923, 0.898, 0.98, 
            0.63, 0.971, 1.0, 0.515, 0.963, 
            0.986, 0.823, 0.879, 1.01, 0.931, 
            0.937, 1.05),
        cutdphiin = cms.vdouble(0.0404, 0.0499, 0.263, 0.042, 0.0484, 
            0.241, 0.242, 0.231, 0.286, 0.0552, 
            0.0338, 0.154, 0.0623, 0.0183, 0.0392, 
            0.0547, 0.0588, 0.00654, 0.042, 0.0217, 
            0.0885, 0.0445, 0.0141, 0.0234, 0.065, 
            0.0258, 0.0346),
        cutet = cms.vdouble(-100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, 13.7, 13.2, 
            13.6, 14.2, 14.1, 13.9, 12.9, 
            14.9, 17.7)
    ),
    classbasedtightEleIDCutsV00 = cms.PSet(
        deltaPhiIn = cms.vdouble(0.032, 0.016, 0.0525, 0.09, 0.025, 
            0.035, 0.065, 0.092),
        hOverE = cms.vdouble(0.05, 0.042, 0.045, 0.0, 0.055, 
            0.037, 0.05, 0.0),
        sigmaEtaEta = cms.vdouble(0.0125, 0.011, 0.01, 0.0, 0.0265, 
            0.0252, 0.026, 0.0),
        deltaEtaIn = cms.vdouble(0.0055, 0.003, 0.0065, 0.0, 0.006, 
            0.0055, 0.0075, 0.0),
        eSeedOverPin = cms.vdouble(0.24, 0.94, 0.11, 0.0, 0.32, 
            0.83, 0.0, 0.0)
    ),
    classbasedtightEleIDCutsV01 = cms.PSet(
        deltaPhiIn = cms.vdouble(0.0225, 0.0114, 0.0234, 0.039, 0.0215, 
            0.0095, 0.0148, 0.0167),
        hOverE = cms.vdouble(0.056, 0.0221, 0.037, 0.0, 0.0268, 
            0.0102, 0.0104, 0.0),
        sigmaEtaEta = cms.vdouble(0.0095, 0.0094, 0.0094, 0.0, 0.026, 
            0.0257, 0.0246, 0.0),
        deltaEtaIn = cms.vdouble(0.0043, 0.00282, 0.0036, 0.0, 0.0066, 
            0.0049, 0.0041, 0.0),
        eSeedOverPin = cms.vdouble(0.32, 0.94, 0.221, 0.0, 0.74, 
            0.89, 0.66, 0.0)
    ),
    classbasedtightEleIDCutsV06 = cms.PSet(
        cutdetain = cms.vdouble(0.0116, 0.00449, 0.00938, 0.0184, 0.00678, 
            0.0109, 0.0252, 0.0268, 0.0139),
        cutiso_sum = cms.vdouble(15.5, 12.2, 12.2, 11.7, 7.16, 
            9.71, 8.66, 11.9, 2.98),
        cutip_gsf = cms.vdouble(0.0131, 0.0586, 0.0839, 0.0366, 0.452, 
            0.204, 0.0913, 0.0802, 0.0731),
        cutip_gsfl = cms.vdouble(0.0119, 0.0527, 0.0471, 0.0212, 0.233, 
            0.267, 0.109, 0.122, 0.0479),
        cuthoe = cms.vdouble(0.215, 0.0608, 0.147, 0.369, 0.0349, 
            0.102, 0.52, 0.422, 0.404),
        cutiso_sumoetl = cms.vdouble(6.21, 6.81, 5.3, 5.39, 2.73, 
            4.73, 4.84, 3.46, 3.73),
        cutfmishits = cms.vdouble(1.5, 1.5, 1.5, 2.5, 2.5, 
            1.5, 1.5, 2.5, 0.5),
        cuthoel = cms.vdouble(0.228, 0.0836, 0.143, 0.37, 0.0392, 
            0.0979, 0.3, 0.381, 0.339),
        cutdphiin = cms.vdouble(0.0897, 0.0993, 0.295, 0.0979, 0.151, 
            0.252, 0.341, 0.308, 0.328),
        cutseel = cms.vdouble(0.0132, 0.0117, 0.0112, 0.0387, 0.0281, 
            0.0287, 0.00987, 0.0296, 0.0544),
        cutiso_sumoet = cms.vdouble(11.9, 7.81, 6.28, 8.92, 4.65, 
            5.49, 9.36, 8.84, 5.94),
        cutdcotdist = cms.vdouble(9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0),
        cutsee = cms.vdouble(0.0145, 0.0116, 0.012, 0.039, 0.0297, 
            0.0311, 0.00987, 0.0347, 0.0917),
        cuteseedopcor = cms.vdouble(0.637, 0.943, 0.742, 0.748, 0.763, 
            0.631, 0.214, 0.873, 0.473),
        cutdphiinl = cms.vdouble(0.061, 0.14, 0.286, 0.0921, 0.197, 
            0.24, 0.333, 0.303, 0.258),
        cutdetainl = cms.vdouble(0.00816, 0.00401, 0.0081, 0.019, 0.00588, 
            0.00893, 0.0171, 0.0434, 0.0143)
    ),
    classbasedtightEleIDCutsV04 = cms.PSet(
        cutdetain = cms.vdouble(0.00811, 0.00341, 0.00633, 0.0103, 0.00667, 
            0.01, 0.0106, 0.0145, 0.0163, 0.0076, 
            0.00259, 0.00511, 0.00941, 0.0043, 0.00857, 
            0.012, 0.0169, 0.00172, 0.00861, 0.00362, 
            0.00601, 0.00925, 0.00489, 0.00832, 0.0119, 
            0.0169, 0.000996),
        cutiso_sum = cms.vdouble(11.8, 8.31, 6.26, 6.18, 3.28, 
            4.38, 4.17, 5.4, 1.57, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0),
        cutip_gsf = cms.vdouble(0.0213, 0.0422, 0.0632, 0.0361, 0.073, 
            0.126, 0.171, 0.119, 0.0372, 0.0131, 
            0.0146, 0.0564, 0.0152, 0.0222, 0.0268, 
            0.0314, 0.0884, 0.00374, 0.00852, 0.00761, 
            0.0143, 0.0106, 0.0127, 0.0119, 0.0123, 
            0.0235, 0.00363),
        cuthoe = cms.vdouble(0.0783, 0.0387, 0.105, 0.118, 0.0227, 
            0.062, 0.13, 2.47, 0.38, 0.0888, 
            0.0503, 0.0955, 0.0741, 0.015, 0.03, 
            0.589, 1.13, 0.612, 0.0494, 0.0461, 
            0.0292, 0.0369, 0.0113, 0.0145, 0.124, 
            2.05, 0.61),
        cutfmishits = cms.vdouble(2.5, 1.5, 1.5, 1.5, 1.5, 
            0.5, 2.5, 0.5, 0.5, 2.5, 
            1.5, 0.5, 0.5, 0.5, 0.5, 
            0.5, 0.5, -0.5, 2.5, 1.5, 
            0.5, 0.5, 0.5, 0.5, 0.5, 
            0.5, 0.5),
        cutiso_sumoet = cms.vdouble(13.7, 11.6, 7.14, 9.98, 3.52, 
            4.87, 6.24, 7.96, 2.53, 11.2, 
            11.9, 7.88, 8.16, 5.58, 5.03, 
            11.4, 8.15, 5.79, 10.4, 11.1, 
            10.4, 7.47, 5.08, 5.9, 11.8, 
            14.1, 11.7),
        cutdcotdist = cms.vdouble(0.0393, 0.0256, 0.00691, 0.0394, 0.0386, 
            0.039, 0.0325, 0.0384, 0.0382, 0.0245, 
            0.000281, 5.46e-05, 0.0342, 0.0232, 0.00107, 
            0.0178, 0.0193, 0.000758, 0.000108, 0.0248, 
            0.000458, 0.0129, 0.00119, 0.0182, 4.53e-05, 
            0.0189, 0.000928),
        cutsee = cms.vdouble(0.0143, 0.0105, 0.0123, 0.0324, 0.0307, 
            0.0301, 0.0109, 0.027, 0.0292, 0.0133, 
            0.0104, 0.0116, 0.0332, 0.0296, 0.031, 
            0.00981, 0.0307, 0.072, 0.0149, 0.0105, 
            0.011, 0.0342, 0.0307, 0.0303, 0.00954, 
            0.0265, 0.0101),
        cuteseedopcor = cms.vdouble(0.784, 0.366, 0.57, 0.911, 0.298, 
            0.645, 0.51, 0.497, 0.932, 0.835, 
            0.968, 0.969, 0.923, 0.898, 0.98, 
            0.63, 0.971, 1.0, 0.515, 0.963, 
            0.986, 0.823, 0.879, 1.01, 0.931, 
            0.937, 1.05),
        cutdphiin = cms.vdouble(0.0404, 0.0499, 0.263, 0.042, 0.0484, 
            0.241, 0.242, 0.231, 0.286, 0.0552, 
            0.0338, 0.154, 0.0623, 0.0183, 0.0392, 
            0.0547, 0.0588, 0.00654, 0.042, 0.0217, 
            0.0885, 0.0445, 0.0141, 0.0234, 0.065, 
            0.0258, 0.0346),
        cutet = cms.vdouble(-100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, 13.7, 13.2, 
            13.6, 14.2, 14.1, 13.9, 12.9, 
            14.9, 17.7)
    ),
    electronIDType = cms.string('classbased'),
    robusttightEleIDCutsV04 = cms.PSet(
        barrel = cms.vdouble(0.0201, 0.0102, 0.0211, 0.00606, -1, 
            -1, 2.34, 3.24, 4.51, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.00253, 0.0291, 0.022, 0.0032, -1, 
            -1, 0.826, 2.7, 0.255, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    electronVersion = cms.string(''),
    robusttightEleIDCutsV00 = cms.PSet(
        barrel = cms.vdouble(0.015, 0.0092, 0.02, 0.0025, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.018, 0.025, 0.02, 0.004, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robusttightEleIDCutsV01 = cms.PSet(
        barrel = cms.vdouble(0.01, 0.0099, 0.025, 0.004, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.01, 0.028, 0.02, 0.0066, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    reducedBarrelRecHitCollection = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
    robusttightEleIDCutsV03 = cms.PSet(
        barrel = cms.vdouble(0.0201, 0.0102, 0.0211, 0.00606, -1, 
            -1, 2.34, 3.24, 4.51, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.00253, 0.0291, 0.022, 0.0032, -1, 
            -1, 0.826, 2.7, 0.255, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    reducedEndcapRecHitCollection = cms.InputTag("ecalRecHit","EcalRecHitsEE"),
    verticesCollection = cms.InputTag("offlinePrimaryVerticesWithBS"),
    classbasedlooseEleIDCuts = cms.PSet(
        cutdetain = cms.vdouble(0.0137, 0.00678, 0.0241, 0.0187, 0.0161, 
            0.0224, 0.0252, 0.0308, 0.0273),
        cutiso_sum = cms.vdouble(33.0, 17.0, 17.9, 18.8, 8.55, 
            12.5, 17.6, 18.5, 2.98),
        cutip_gsf = cms.vdouble(0.0551, 0.0765, 0.143, 0.0874, 0.594, 
            0.37, 0.0913, 1.15, 0.231),
        cutip_gsfl = cms.vdouble(0.0186, 0.0759, 0.138, 0.0473, 0.62, 
            0.304, 0.109, 0.775, 0.0479),
        cuthoe = cms.vdouble(0.247, 0.137, 0.147, 0.371, 0.0588, 
            0.147, 0.52, 0.452, 0.404),
        cutiso_sumoetl = cms.vdouble(11.3, 9.05, 9.07, 9.94, 5.25, 
            6.15, 10.7, 10.8, 4.4),
        cutfmishits = cms.vdouble(4.5, 1.5, 1.5, 2.5, 2.5, 
            1.5, 4.5, 3.5, 3.5),
        cuthoel = cms.vdouble(0.236, 0.126, 0.147, 0.375, 0.0392, 
            0.145, 0.365, 0.383, 0.384),
        cutdphiin = cms.vdouble(0.0897, 0.262, 0.353, 0.116, 0.357, 
            0.319, 0.342, 0.404, 0.336),
        cutseel = cms.vdouble(0.0164, 0.0118, 0.015, 0.0523, 0.0326, 
            0.0456, 0.0185, 0.0589, 0.0544),
        cutiso_sumoet = cms.vdouble(34.5, 12.7, 12.1, 19.9, 6.35, 
            8.85, 14.0, 10.5, 9.74),
        cutdcotdist = cms.vdouble(9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0),
        cutsee = cms.vdouble(0.0176, 0.0125, 0.0181, 0.0415, 0.0364, 
            0.0418, 0.0146, 0.0678, 0.133),
        cuteseedopcor = cms.vdouble(0.63, 0.82, 0.401, 0.718, 0.4, 
            0.458, 0.15, 0.664, 0.373),
        cutdphiinl = cms.vdouble(0.0747, 0.25, 0.356, 0.0956, 0.347, 
            0.326, 0.333, 0.647, 0.289),
        cutdetainl = cms.vdouble(0.0124, 0.00503, 0.0257, 0.0228, 0.0118, 
            0.0178, 0.0188, 0.14, 0.024)
    ),
    robusttightEleIDCutsV02 = cms.PSet(
        barrel = cms.vdouble(0.0201, 0.0102, 0.0211, 0.00606, -1, 
            -1, 2.34, 3.24, 4.51, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.00253, 0.0291, 0.022, 0.0032, -1, 
            -1, 0.826, 2.7, 0.255, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robusthighenergyEleIDCutsV04 = cms.PSet(
        barrel = cms.vdouble(0.05, 9999, 0.09, 0.005, 0.94, 
            0.83, 7.5, 2, 0.03, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.05, 0.03, 0.09, 0.007, -1, 
            -1, 15, 2.5, 0.03, 2.5, 
            0, 0.5, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robusthighenergyEleIDCutsV01 = cms.PSet(
        barrel = cms.vdouble(0.05, 9999, 0.09, 0.005, 0.94, 
            0.83, 9999.0, 9999.0, 0, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.05, 0.0275, 0.09, 0.007, -1, 
            -1, 9999.0, 9999.0, 0, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robusthighenergyEleIDCutsV00 = cms.PSet(
        barrel = cms.vdouble(0.05, 0.011, 0.09, 0.005, -1, 
            -1, 9999.0, 9999.0, 0, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.1, 0.0275, 0.09, 0.007, -1, 
            -1, 9999.0, 9999.0, 0, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robusthighenergyEleIDCutsV03 = cms.PSet(
        barrel = cms.vdouble(0.05, 9999, 0.09, 0.005, 0.94, 
            0.83, 7.5, 2, 0.03, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.05, 0.03, 0.09, 0.007, -1, 
            -1, 15, 2.5, 0.03, 2.5, 
            0, 0.5, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robusthighenergyEleIDCutsV02 = cms.PSet(
        barrel = cms.vdouble(0.05, 9999, 0.09, 0.005, 0.94, 
            0.83, 7.5, 2, 0.03, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.05, 0.03, 0.09, 0.007, -1, 
            -1, 15, 2.5, 0.03, 2.5, 
            0, 0.5, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    classbasedlooseEleIDCutsV00 = cms.PSet(
        deltaPhiIn = cms.vdouble(0.05, 0.025, 0.053, 0.09, 0.07, 
            0.03, 0.092, 0.092),
        hOverE = cms.vdouble(0.115, 0.1, 0.055, 0.0, 0.145, 
            0.12, 0.15, 0.0),
        sigmaEtaEta = cms.vdouble(0.014, 0.012, 0.0115, 0.0, 0.0275, 
            0.0265, 0.0265, 0.0),
        deltaEtaIn = cms.vdouble(0.009, 0.0045, 0.0085, 0.0, 0.0105, 
            0.0068, 0.01, 0.0),
        eSeedOverPin = cms.vdouble(0.11, 0.91, 0.11, 0.0, 0.0, 
            0.85, 0.0, 0.0)
    ),
    classbasedlooseEleIDCutsV01 = cms.PSet(
        deltaPhiIn = cms.vdouble(0.053, 0.0189, 0.059, 0.099, 0.0278, 
            0.0157, 0.042, 0.08),
        hOverE = cms.vdouble(0.076, 0.033, 0.07, 0.0, 0.083, 
            0.0148, 0.033, 0.0),
        sigmaEtaEta = cms.vdouble(0.0101, 0.0095, 0.0097, 0.0, 0.0271, 
            0.0267, 0.0259, 0.0),
        deltaEtaIn = cms.vdouble(0.0078, 0.00259, 0.0062, 0.0, 0.0078, 
            0.0061, 0.0061, 0.0),
        eSeedOverPin = cms.vdouble(0.3, 0.92, 0.211, 0.0, 0.42, 
            0.88, 0.68, 0.0)
    ),
    classbasedlooseEleIDCutsV02 = cms.PSet(
        cutisohcal = cms.vdouble(13.5, 9.93, 7.56, 14.8, 8.1, 
            10.8, 42.7, 20.1, 9.11, 10.4, 
            6.89, 5.59, 8.53, 9.59, 24.2, 
            2.78, 8.67, 0.288),
        cutmishits = cms.vdouble(5.5, 1.5, 5.5, 2.5, 2.5, 
            2.5, 3.5, 5.5, 0.5, 1.5, 
            2.5, 0.5, 1.5, 1.5, 0.5, 
            0.5, 0.5, 0.5),
        cuthoe = cms.vdouble(0.0887, 0.0934, 0.0949, 0.0986, 0.0431, 
            0.0878, 0.097, 0.0509, 0.098, 0.0991, 
            0.0321, 0.0928, 0.0663, 0.0717, 0.0966, 
            0.0758, 0.0149, 0.0131),
        cutdeta = cms.vdouble(0.00958, 0.00406, 0.0122, 0.0137, 0.00837, 
            0.0127, 0.011, 0.00336, 0.00977, 0.015, 
            0.00675, 0.0109, 0.014, 0.00508, 0.0109, 
            0.0146, 0.00506, 0.0127),
        cuteopin = cms.vdouble(0.878, 0.802, 0.814, 0.942, 0.735, 
            0.774, 0.829, 0.909, 0.829, 0.813, 
            0.86, 0.897, 0.817, 0.831, 0.818, 
            0.861, 0.787, 0.789),
        cutip = cms.vdouble(0.0246, 0.076, 0.0966, 0.0885, 0.441, 
            0.205, 0.0292, 0.0293, 0.0619, 0.0251, 
            0.159, 0.0815, 7.29, 0.0106, 5.76, 
            6.89, 1.27, 5.89),
        cutisotk = cms.vdouble(24.3, 8.45, 14.4, 27.8, 6.02, 
            10.5, 14.1, 10.2, 14.5, 19.1, 
            6.1, 14.1, 8.59, 8.33, 8.3, 
            8.93, 8.6, 16.0),
        cutsee = cms.vdouble(0.0172, 0.0115, 0.0143, 0.0344, 0.0295, 
            0.0304, 0.0145, 0.0108, 0.0128, 0.0347, 
            0.0307, 0.0316, 0.018, 0.011, 0.0132, 
            0.0349, 0.031, 0.0327),
        cutdphi = cms.vdouble(0.0372, 0.114, 0.118, 0.0488, 0.117, 
            0.119, 0.0606, 0.0548, 0.117, 0.07, 
            0.0355, 0.117, 0.088, 0.045, 0.118, 
            0.0919, 0.0236, 0.0515),
        cutisoecal = cms.vdouble(33.4, 28.1, 7.32, 27.4, 7.33, 
            21.7, 93.8, 102.0, 12.1, 26.0, 
            8.91, 10.0, 16.1, 31.3, 16.9, 
            15.4, 13.3, 37.7)
    ),
    classbasedlooseEleIDCutsV03 = cms.PSet(
        cutdetain = cms.vdouble(0.00989, 0.00484, 0.0146, 0.0146, 0.00902, 
            0.0172, 0.0137, 0.0477, 0.0275, 0.00967, 
            0.00377, 0.00924, 0.013, 0.00666, 0.0123, 
            0.0125, 0.0228, 0.0112, 0.0106, 0.0038, 
            0.00897, 0.0139, 0.00667, 0.0122, 0.0122, 
            0.0193, 0.00239),
        cutiso_sum = cms.vdouble(31.5, 10.3, 8.8, 11.0, 6.13, 
            6.94, 7.52, 9.0, 3.5, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0),
        cutip_gsf = cms.vdouble(0.0431, 0.0767, 0.139, 0.101, 0.149, 
            0.154, 0.932, 0.15, 0.124, 0.0238, 
            0.0467, 0.0759, 0.0369, 0.147, 0.0986, 
            0.0626, 0.195, 0.116, 0.0122, 0.0125, 
            0.0693, 0.0162, 0.089, 0.0673, 0.0467, 
            0.0651, 0.0221),
        cuthoe = cms.vdouble(0.166, 0.0771, 0.144, 0.37, 0.0497, 
            0.139, 0.401, 2.68, 0.516, 0.234, 
            0.0556, 0.144, 0.368, 0.031, 0.12, 
            0.602, 2.01, 1.05, 0.104, 0.063, 
            0.0565, 0.38, 0.0192, 0.0294, 0.537, 
            4.65, 1.87),
        cutfmishits = cms.vdouble(4.5, 1.5, 1.5, 2.5, 2.5, 
            1.5, 2.5, 2.5, 1.5, 2.5, 
            1.5, 1.5, 1.5, 1.5, 0.5, 
            2.5, 2.5, 0.5, 2.5, 1.5, 
            0.5, 1.5, 1.5, 0.5, 2.5, 
            0.5, 0.5),
        cutiso_sumoet = cms.vdouble(28.9, 15.3, 12.0, 18.3, 7.17, 
            9.42, 11.0, 9.81, 3.94, 22.7, 
            15.9, 12.3, 17.0, 7.58, 8.89, 
            15.2, 12.7, 6.17, 20.8, 21.2, 
            17.2, 15.5, 9.37, 10.6, 19.8, 
            22.1, 15.6),
        cutdcotdist = cms.vdouble(0.0393, 0.0392, 0.0397, 0.0394, 0.0393, 
            0.039, 0.0378, 0.0388, 0.0382, 0.0385, 
            0.0167, 0.00325, 0.0394, 0.0387, 0.0388, 
            0.0227, 0.0258, 0.0127, 0.0298, 0.03, 
            0.00946, 0.039, 0.0231, 0.0278, 0.00162, 
            0.0367, 0.0199),
        cutsee = cms.vdouble(0.0175, 0.0127, 0.0177, 0.0373, 0.0314, 
            0.0329, 0.0157, 0.0409, 0.14, 0.0169, 
            0.0106, 0.0142, 0.0363, 0.0322, 0.0354, 
            0.0117, 0.0372, 28.2, 0.0171, 0.0113, 
            0.014, 0.0403, 0.0323, 0.0411, 0.0104, 
            0.0436, 0.0114),
        cuteseedopcor = cms.vdouble(0.78, 0.302, 0.483, 0.904, 0.168, 
            0.645, 0.108, 0.284, 0.324, 0.591, 
            0.286, 0.488, 0.813, 0.791, 0.672, 
            0.398, 0.834, 0.878, 0.515, 0.937, 
            0.806, 0.816, 0.85, 0.507, 0.367, 
            0.83, 0.648),
        cutdphiin = cms.vdouble(0.041, 0.275, 0.365, 0.047, 0.273, 
            0.296, 0.329, 0.465, 0.627, 0.0581, 
            0.0954, 0.327, 0.0702, 0.0582, 0.279, 
            0.117, 0.318, 0.246, 0.0821, 0.052, 
            0.292, 0.116, 0.0435, 0.312, 0.118, 
            0.296, 0.0459),
        cutet = cms.vdouble(-100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, 12.0, 12.0, 
            12.0, 12.0, 12.0, 12.0, 12.0, 
            12.0, 12.5)
    ),
    classbasedlooseEleIDCutsV04 = cms.PSet(
        cutdetain = cms.vdouble(0.00989, 0.00484, 0.0146, 0.0146, 0.00902, 
            0.0172, 0.0137, 0.0477, 0.0275, 0.00967, 
            0.00377, 0.00924, 0.013, 0.00666, 0.0123, 
            0.0125, 0.0228, 0.0112, 0.0106, 0.0038, 
            0.00897, 0.0139, 0.00667, 0.0122, 0.0122, 
            0.0193, 0.00239),
        cutiso_sum = cms.vdouble(31.5, 10.3, 8.8, 11.0, 6.13, 
            6.94, 7.52, 9.0, 3.5, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0, 100000.0, 100000.0, 100000.0, 
            100000.0, 100000.0),
        cutip_gsf = cms.vdouble(0.0431, 0.0767, 0.139, 0.101, 0.149, 
            0.154, 0.932, 0.15, 0.124, 0.0238, 
            0.0467, 0.0759, 0.0369, 0.147, 0.0986, 
            0.0626, 0.195, 0.116, 0.0122, 0.0125, 
            0.0693, 0.0162, 0.089, 0.0673, 0.0467, 
            0.0651, 0.0221),
        cuthoe = cms.vdouble(0.166, 0.0771, 0.144, 0.37, 0.0497, 
            0.139, 0.401, 2.68, 0.516, 0.234, 
            0.0556, 0.144, 0.368, 0.031, 0.12, 
            0.602, 2.01, 1.05, 0.104, 0.063, 
            0.0565, 0.38, 0.0192, 0.0294, 0.537, 
            4.65, 1.87),
        cutfmishits = cms.vdouble(4.5, 1.5, 1.5, 2.5, 2.5, 
            1.5, 2.5, 2.5, 1.5, 2.5, 
            1.5, 1.5, 1.5, 1.5, 0.5, 
            2.5, 2.5, 0.5, 2.5, 1.5, 
            0.5, 1.5, 1.5, 0.5, 2.5, 
            0.5, 0.5),
        cutiso_sumoet = cms.vdouble(28.9, 15.3, 12.0, 18.3, 7.17, 
            9.42, 11.0, 9.81, 3.94, 22.7, 
            15.9, 12.3, 17.0, 7.58, 8.89, 
            15.2, 12.7, 6.17, 20.8, 21.2, 
            17.2, 15.5, 9.37, 10.6, 19.8, 
            22.1, 15.6),
        cutdcotdist = cms.vdouble(0.0393, 0.0392, 0.0397, 0.0394, 0.0393, 
            0.039, 0.0378, 0.0388, 0.0382, 0.0385, 
            0.0167, 0.00325, 0.0394, 0.0387, 0.0388, 
            0.0227, 0.0258, 0.0127, 0.0298, 0.03, 
            0.00946, 0.039, 0.0231, 0.0278, 0.00162, 
            0.0367, 0.0199),
        cutsee = cms.vdouble(0.0175, 0.0127, 0.0177, 0.0373, 0.0314, 
            0.0329, 0.0157, 0.0409, 0.14, 0.0169, 
            0.0106, 0.0142, 0.0363, 0.0322, 0.0354, 
            0.0117, 0.0372, 28.2, 0.0171, 0.0113, 
            0.014, 0.0403, 0.0323, 0.0411, 0.0104, 
            0.0436, 0.0114),
        cuteseedopcor = cms.vdouble(0.78, 0.302, 0.483, 0.904, 0.168, 
            0.645, 0.108, 0.284, 0.324, 0.591, 
            0.286, 0.488, 0.813, 0.791, 0.672, 
            0.398, 0.834, 0.878, 0.515, 0.937, 
            0.806, 0.816, 0.85, 0.507, 0.367, 
            0.83, 0.648),
        cutdphiin = cms.vdouble(0.041, 0.275, 0.365, 0.047, 0.273, 
            0.296, 0.329, 0.465, 0.627, 0.0581, 
            0.0954, 0.327, 0.0702, 0.0582, 0.279, 
            0.117, 0.318, 0.246, 0.0821, 0.052, 
            0.292, 0.116, 0.0435, 0.312, 0.118, 
            0.296, 0.0459),
        cutet = cms.vdouble(-100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, -100000.0, -100000.0, 
            -100000.0, -100000.0, -100000.0, 12.0, 12.0, 
            12.0, 12.0, 12.0, 12.0, 12.0, 
            12.0, 12.5)
    ),
    classbasedlooseEleIDCutsV06 = cms.PSet(
        cutdetain = cms.vdouble(0.0137, 0.00678, 0.0241, 0.0187, 0.0161, 
            0.0224, 0.0252, 0.0308, 0.0273),
        cutiso_sum = cms.vdouble(33.0, 17.0, 17.9, 18.8, 8.55, 
            12.5, 17.6, 18.5, 2.98),
        cutip_gsf = cms.vdouble(0.0551, 0.0765, 0.143, 0.0874, 0.594, 
            0.37, 0.0913, 1.15, 0.231),
        cutip_gsfl = cms.vdouble(0.0186, 0.0759, 0.138, 0.0473, 0.62, 
            0.304, 0.109, 0.775, 0.0479),
        cuthoe = cms.vdouble(0.247, 0.137, 0.147, 0.371, 0.0588, 
            0.147, 0.52, 0.452, 0.404),
        cutiso_sumoetl = cms.vdouble(11.3, 9.05, 9.07, 9.94, 5.25, 
            6.15, 10.7, 10.8, 4.4),
        cutfmishits = cms.vdouble(4.5, 1.5, 1.5, 2.5, 2.5, 
            1.5, 4.5, 3.5, 3.5),
        cuthoel = cms.vdouble(0.236, 0.126, 0.147, 0.375, 0.0392, 
            0.145, 0.365, 0.383, 0.384),
        cutdphiin = cms.vdouble(0.0897, 0.262, 0.353, 0.116, 0.357, 
            0.319, 0.342, 0.404, 0.336),
        cutseel = cms.vdouble(0.0164, 0.0118, 0.015, 0.0523, 0.0326, 
            0.0456, 0.0185, 0.0589, 0.0544),
        cutiso_sumoet = cms.vdouble(34.5, 12.7, 12.1, 19.9, 6.35, 
            8.85, 14.0, 10.5, 9.74),
        cutdcotdist = cms.vdouble(9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0),
        cutsee = cms.vdouble(0.0176, 0.0125, 0.0181, 0.0415, 0.0364, 
            0.0418, 0.0146, 0.0678, 0.133),
        cuteseedopcor = cms.vdouble(0.63, 0.82, 0.401, 0.718, 0.4, 
            0.458, 0.15, 0.664, 0.373),
        cutdphiinl = cms.vdouble(0.0747, 0.25, 0.356, 0.0956, 0.347, 
            0.326, 0.333, 0.647, 0.289),
        cutdetainl = cms.vdouble(0.0124, 0.00503, 0.0257, 0.0228, 0.0118, 
            0.0178, 0.0188, 0.14, 0.024)
    ),
    src = cms.InputTag("gsfElectrons"),
    robusttightEleIDCuts = cms.PSet(
        barrel = cms.vdouble(0.0201, 0.0102, 0.0211, 0.00606, -1, 
            -1, 2.34, 3.24, 4.51, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.00253, 0.0291, 0.022, 0.0032, -1, 
            -1, 0.826, 2.7, 0.255, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    classbasedtightEleIDCuts = cms.PSet(
        cutdetain = cms.vdouble(0.0116, 0.00449, 0.00938, 0.0184, 0.00678, 
            0.0109, 0.0252, 0.0268, 0.0139),
        cutiso_sum = cms.vdouble(15.5, 12.2, 12.2, 11.7, 7.16, 
            9.71, 8.66, 11.9, 2.98),
        cutip_gsf = cms.vdouble(0.0131, 0.0586, 0.0839, 0.0366, 0.452, 
            0.204, 0.0913, 0.0802, 0.0731),
        cutip_gsfl = cms.vdouble(0.0119, 0.0527, 0.0471, 0.0212, 0.233, 
            0.267, 0.109, 0.122, 0.0479),
        cuthoe = cms.vdouble(0.215, 0.0608, 0.147, 0.369, 0.0349, 
            0.102, 0.52, 0.422, 0.404),
        cutiso_sumoetl = cms.vdouble(6.21, 6.81, 5.3, 5.39, 2.73, 
            4.73, 4.84, 3.46, 3.73),
        cutfmishits = cms.vdouble(1.5, 1.5, 1.5, 2.5, 2.5, 
            1.5, 1.5, 2.5, 0.5),
        cuthoel = cms.vdouble(0.228, 0.0836, 0.143, 0.37, 0.0392, 
            0.0979, 0.3, 0.381, 0.339),
        cutdphiin = cms.vdouble(0.0897, 0.0993, 0.295, 0.0979, 0.151, 
            0.252, 0.341, 0.308, 0.328),
        cutseel = cms.vdouble(0.0132, 0.0117, 0.0112, 0.0387, 0.0281, 
            0.0287, 0.00987, 0.0296, 0.0544),
        cutiso_sumoet = cms.vdouble(11.9, 7.81, 6.28, 8.92, 4.65, 
            5.49, 9.36, 8.84, 5.94),
        cutdcotdist = cms.vdouble(9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0),
        cutsee = cms.vdouble(0.0145, 0.0116, 0.012, 0.039, 0.0297, 
            0.0311, 0.00987, 0.0347, 0.0917),
        cuteseedopcor = cms.vdouble(0.637, 0.943, 0.742, 0.748, 0.763, 
            0.631, 0.214, 0.873, 0.473),
        cutdphiinl = cms.vdouble(0.061, 0.14, 0.286, 0.0921, 0.197, 
            0.24, 0.333, 0.303, 0.258),
        cutdetainl = cms.vdouble(0.00816, 0.00401, 0.0081, 0.019, 0.00588, 
            0.00893, 0.0171, 0.0434, 0.0143)
    ),
    algorithm = cms.string('eIDCB'),
    robusthighenergyEleIDCuts = cms.PSet(
        barrel = cms.vdouble(0.05, 9999, 0.09, 0.005, 0.94, 
            0.83, 7.5, 2, 0.03, 9999.0, 
            0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.05, 0.03, 0.09, 0.007, -1, 
            -1, 15, 2.5, 0.03, 2.5, 
            0, 0.5, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robustlooseEleIDCuts = cms.PSet(
        barrel = cms.vdouble(0.05, 0.0103, 0.8, 0.00688, -1, 
            -1, 7.33, 4.68, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.0389, 0.0307, 0.7, 0.00944, -1, 
            -1, 7.76, 3.09, 2.23, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robustlooseEleIDCutsV02 = cms.PSet(
        barrel = cms.vdouble(0.05, 0.0103, 0.8, 0.00688, -1, 
            -1, 7.33, 4.68, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.0389, 0.0307, 0.7, 0.00944, -1, 
            -1, 7.76, 3.09, 2.23, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robustlooseEleIDCutsV03 = cms.PSet(
        barrel = cms.vdouble(0.05, 0.0103, 0.8, 0.00688, -1, 
            -1, 7.33, 4.68, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.0389, 0.0307, 0.7, 0.00944, -1, 
            -1, 7.76, 3.09, 2.23, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robustlooseEleIDCutsV00 = cms.PSet(
        barrel = cms.vdouble(0.115, 0.014, 0.09, 0.009, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.15, 0.0275, 0.092, 0.0105, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robustlooseEleIDCutsV01 = cms.PSet(
        barrel = cms.vdouble(0.075, 0.0132, 0.058, 0.0077, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.083, 0.027, 0.042, 0.01, -1, 
            -1, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    robustlooseEleIDCutsV04 = cms.PSet(
        barrel = cms.vdouble(0.05, 0.0103, 0.8, 0.00688, -1, 
            -1, 7.33, 4.68, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0),
        endcap = cms.vdouble(0.0389, 0.0307, 0.7, 0.00944, -1, 
            -1, 7.76, 3.09, 2.23, 9999.0, 
            9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 
            9999.0, 9999.0, 9999.0, 0.0, -9999.0, 
            9999.0, 9999.0, 9999, -1, 0, 
            0)
    ),
    additionalCategories = cms.bool(True),
    etBinning = cms.bool(True)
)


process.eleIsoDepositEcalFromHits = cms.EDProducer("CandIsoDepositProducer",
    src = cms.InputTag("gsfElectrons"),
    MultipleDepositsFlag = cms.bool(False),
    trackType = cms.string('candidate'),
    ExtractorPSet = cms.PSet(
        isolationVariable = cms.string('et'),
        tryBoth = cms.bool(True),
        intStrip = cms.double(0.0),
        ComponentName = cms.string('EgammaRecHitExtractor'),
        endcapEcalHits = cms.InputTag("reducedEcalRecHitsEE"),
        recHitFlagsToBeExcluded = cms.vstring('kFaultyHardware', 
            'kPoorCalib', 
            'kTowerRecovered', 
            'kDead'),
        intRadius = cms.double(0.0),
        severityLevelCut = cms.int32(4),
        energyMin = cms.double(0.095),
        extRadius = cms.double(0.6),
        subtractSuperClusterEnergy = cms.bool(False),
        vetoClustered = cms.bool(False),
        etMin = cms.double(0.0),
        DepositLabel = cms.untracked.string(''),
        barrelEcalHits = cms.InputTag("reducedEcalRecHitsEB")
    )
)


process.eleIsoDepositHcalFromTowers = cms.EDProducer("CandIsoDepositProducer",
    src = cms.InputTag("gsfElectrons"),
    MultipleDepositsFlag = cms.bool(False),
    trackType = cms.string('candidate'),
    ExtractorPSet = cms.PSet(
        caloTowers = cms.InputTag("towerMaker"),
        ComponentName = cms.string('EgammaTowerExtractor'),
        hcalDepth = cms.int32(-1),
        intRadius = cms.double(0.0),
        extRadius = cms.double(0.6),
        DepositLabel = cms.untracked.string(''),
        etMin = cms.double(-999.0)
    )
)


process.eleIsoDepositTk = cms.EDProducer("CandIsoDepositProducer",
    src = cms.InputTag("gsfElectrons"),
    MultipleDepositsFlag = cms.bool(False),
    trackType = cms.string('candidate'),
    ExtractorPSet = cms.PSet(
        Diff_z = cms.double(0.2),
        dzOption = cms.string('vz'),
        BeamSpotLabel = cms.InputTag("offlineBeamSpot"),
        ComponentName = cms.string('EgammaTrackExtractor'),
        DR_Max = cms.double(1.0),
        Diff_r = cms.double(9999.0),
        Chi2Prob_Min = cms.double(-1.0),
        DR_Veto = cms.double(0.0),
        NHits_Min = cms.uint32(0),
        Chi2Ndof_Max = cms.double(1e+64),
        Pt_Min = cms.double(-1.0),
        DepositLabel = cms.untracked.string(''),
        BeamlineOption = cms.string('BeamSpotFromEvent'),
        inputTrackCollection = cms.InputTag("generalTracks")
    )
)


process.eleIsoFromDepsEcalFromHitsByCrystal = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("eleIsoDepositEcalFromHits"),
        deltaR = cms.double(0.4),
        weight = cms.string('1'),
        vetos = cms.vstring('NumCrystalVeto(3.0)', 
            'NumCrystalEtaPhiVeto(1.5,9999.0)', 
            'EcalBarrel:AbsThresholdFromTransverse(0.095)', 
            'EcalEndcaps:AbsThreshold(0.110)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.eleIsoFromDepsHcalFromTowers = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("eleIsoDepositHcalFromTowers"),
        deltaR = cms.double(0.4),
        weight = cms.string('1'),
        vetos = cms.vstring('0.15'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.eleIsoFromDepsTk = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("eleIsoDepositTk"),
        deltaR = cms.double(0.3),
        weight = cms.string('1'),
        vetos = cms.vstring('RectangularEtaPhiVeto(-0.015,0.015,-0.5,0.5)', 
            'Threshold(0.7)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.electronMatch = cms.EDProducer("MCMatcher",
    src = cms.InputTag("gsfElectrons"),
    maxDPtRel = cms.double(0.5),
    mcPdgId = cms.vint32(11),
    mcStatus = cms.vint32(1),
    resolveByMatchQuality = cms.bool(False),
    maxDeltaR = cms.double(0.5),
    checkCharge = cms.bool(True),
    resolveAmbiguities = cms.bool(True),
    matched = cms.InputTag("genParticles")
)


process.gamIsoDepositEcalFromHits = cms.EDProducer("CandIsoDepositProducer",
    src = cms.InputTag("photons"),
    MultipleDepositsFlag = cms.bool(False),
    trackType = cms.string('candidate'),
    ExtractorPSet = cms.PSet(
        isolationVariable = cms.string('et'),
        tryBoth = cms.bool(True),
        intStrip = cms.double(0.0),
        ComponentName = cms.string('EgammaRecHitExtractor'),
        endcapEcalHits = cms.InputTag("reducedEcalRecHitsEE"),
        recHitFlagsToBeExcluded = cms.vstring('kFaultyHardware', 
            'kPoorCalib', 
            'kTowerRecovered', 
            'kDead'),
        intRadius = cms.double(0.0),
        severityLevelCut = cms.int32(4),
        energyMin = cms.double(0.095),
        extRadius = cms.double(0.6),
        subtractSuperClusterEnergy = cms.bool(False),
        vetoClustered = cms.bool(False),
        detector = cms.string('Ecal'),
        etMin = cms.double(0.0),
        DepositLabel = cms.untracked.string(''),
        barrelEcalHits = cms.InputTag("reducedEcalRecHitsEB")
    )
)


process.gamIsoDepositHcalFromTowers = cms.EDProducer("CandIsoDepositProducer",
    src = cms.InputTag("photons"),
    MultipleDepositsFlag = cms.bool(False),
    trackType = cms.string('candidate'),
    ExtractorPSet = cms.PSet(
        caloTowers = cms.InputTag("towerMaker"),
        ComponentName = cms.string('EgammaTowerExtractor'),
        hcalDepth = cms.int32(-1),
        intRadius = cms.double(0.0),
        extRadius = cms.double(0.6),
        DepositLabel = cms.untracked.string(''),
        etMin = cms.double(-999.0)
    )
)


process.gamIsoDepositTk = cms.EDProducer("CandIsoDepositProducer",
    src = cms.InputTag("photons"),
    MultipleDepositsFlag = cms.bool(False),
    trackType = cms.string('candidate'),
    ExtractorPSet = cms.PSet(
        Diff_z = cms.double(0.2),
        dzOption = cms.string('vz'),
        BeamSpotLabel = cms.InputTag("offlineBeamSpot"),
        ComponentName = cms.string('EgammaTrackExtractor'),
        DR_Max = cms.double(1.0),
        Diff_r = cms.double(9999.0),
        Chi2Prob_Min = cms.double(-1.0),
        DR_Veto = cms.double(0.0),
        NHits_Min = cms.uint32(0),
        Chi2Ndof_Max = cms.double(1e+64),
        Pt_Min = cms.double(-1.0),
        DepositLabel = cms.untracked.string(''),
        BeamlineOption = cms.string('BeamSpotFromEvent'),
        inputTrackCollection = cms.InputTag("generalTracks")
    )
)


process.gamIsoFromDepsEcalFromHits = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("gamIsoDepositEcalFromHits"),
        deltaR = cms.double(0.4),
        weight = cms.string('1'),
        vetos = cms.vstring('EcalBarrel:0.045', 
            'EcalBarrel:RectangularEtaPhiVeto(-0.02,0.02,-0.5,0.5)', 
            'EcalBarrel:AbsThresholdFromTransverse(0.095)', 
            'EcalEndcaps:0.070', 
            'EcalEndcaps:RectangularEtaPhiVeto(-0.02,0.02,-0.5,0.5)', 
            'EcalEndcaps:AbsThreshold(0.110)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.gamIsoFromDepsHcalFromTowers = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("gamIsoDepositHcalFromTowers"),
        deltaR = cms.double(0.4),
        weight = cms.string('1'),
        vetos = cms.vstring('0.15'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.gamIsoFromDepsTk = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(cms.PSet(
        src = cms.InputTag("gamIsoDepositTk"),
        deltaR = cms.double(0.3),
        weight = cms.string('1'),
        vetos = cms.vstring('RectangularEtaPhiVeto(-0.015,0.015,-0.5,0.5)', 
            'Threshold(1.0)'),
        skipDefaultVeto = cms.bool(True),
        mode = cms.string('sum')
    ))
)


process.ic5CaloJetsL1FastL2L3 = cms.EDProducer("CaloJetCorrectionProducer",
    src = cms.InputTag("iterativeCone5CaloJets"),
    correctors = cms.vstring('ic5CaloL1FastL2L3')
)


process.ic5CaloJetsL1FastL2L3Residual = cms.EDProducer("CaloJetCorrectionProducer",
    src = cms.InputTag("iterativeCone5CaloJets"),
    correctors = cms.vstring('ic5CaloL1FastL2L3Residual')
)


process.ic5CaloJetsL1L2L3 = cms.EDProducer("CaloJetCorrectionProducer",
    src = cms.InputTag("iterativeCone5CaloJets"),
    correctors = cms.vstring('ic5CaloL1L2L3')
)


process.ic5CaloJetsL1L2L3Residual = cms.EDProducer("CaloJetCorrectionProducer",
    src = cms.InputTag("iterativeCone5CaloJets"),
    correctors = cms.vstring('ic5CaloL1L2L3Residual')
)


process.ic5CaloJetsL2L3 = cms.EDProducer("CaloJetCorrectionProducer",
    src = cms.InputTag("iterativeCone5CaloJets"),
    correctors = cms.vstring('ic5CaloL2L3')
)


process.ic5CaloJetsL2L3Residual = cms.EDProducer("CaloJetCorrectionProducer",
    src = cms.InputTag("iterativeCone5CaloJets"),
    correctors = cms.vstring('ic5CaloL2L3Residual')
)


process.ic5PFJetsL1FastL2L3 = cms.EDProducer("PFJetCorrectionProducer",
    src = cms.InputTag("iterativeCone5PFJets"),
    correctors = cms.vstring('ic5PFL1FastL2L3')
)


process.ic5PFJetsL1FastL2L3Residual = cms.EDProducer("PFJetCorrectionProducer",
    src = cms.InputTag("iterativeCone5PFJets"),
    correctors = cms.vstring('ic5PFL1FastL2L3Residual')
)


process.ic5PFJetsL1L2L3 = cms.EDProducer("PFJetCorrectionProducer",
    src = cms.InputTag("iterativeCone5PFJets"),
    correctors = cms.vstring('ic5PFL1L2L3')
)


process.ic5PFJetsL1L2L3Residual = cms.EDProducer("PFJetCorrectionProducer",
    src = cms.InputTag("iterativeCone5PFJets"),
    correctors = cms.vstring('ic5PFL1L2L3Residual')
)


process.ic5PFJetsL2L3 = cms.EDProducer("PFJetCorrectionProducer",
    src = cms.InputTag("iterativeCone5PFJets"),
    correctors = cms.vstring('ic5PFL2L3')
)


process.ic5PFJetsL2L3Residual = cms.EDProducer("PFJetCorrectionProducer",
    src = cms.InputTag("iterativeCone5PFJets"),
    correctors = cms.vstring('ic5PFL2L3Residual')
)


process.jetTracksAssociatorAtVertex = cms.EDProducer("JetTracksAssociatorAtVertex",
    jets = cms.InputTag("ak5PFJets"),
    tracks = cms.InputTag("generalTracks"),
    useAssigned = cms.bool(False),
    coneSize = cms.double(0.5),
    pvSrc = cms.InputTag("offlinePrimaryVertices")
)


process.kt4CaloJetsL1FastL2L3 = cms.EDProducer("CaloJetCorrectionProducer",
    src = cms.InputTag("kt4CaloJets"),
    correctors = cms.vstring('kt4CaloL1FastL2L3')
)


process.kt4CaloJetsL1FastL2L3Residual = cms.EDProducer("CaloJetCorrectionProducer",
    src = cms.InputTag("kt4CaloJets"),
    correctors = cms.vstring('kt4CaloL1FastL2L3Residual')
)


process.kt4CaloJetsL1L2L3 = cms.EDProducer("CaloJetCorrectionProducer",
    src = cms.InputTag("kt4CaloJets"),
    correctors = cms.vstring('kt4CaloL1L2L3')
)


process.kt4CaloJetsL1L2L3Residual = cms.EDProducer("CaloJetCorrectionProducer",
    src = cms.InputTag("kt4CaloJets"),
    correctors = cms.vstring('kt4CaloL1L2L3Residual')
)


process.kt4CaloJetsL2L3 = cms.EDProducer("CaloJetCorrectionProducer",
    src = cms.InputTag("kt4CaloJets"),
    correctors = cms.vstring('kt4CaloL2L3')
)


process.kt4CaloJetsL2L3Residual = cms.EDProducer("CaloJetCorrectionProducer",
    src = cms.InputTag("kt4CaloJets"),
    correctors = cms.vstring('kt4CaloL2L3Residual')
)


process.kt4PFJetsL1FastL2L3 = cms.EDProducer("PFJetCorrectionProducer",
    src = cms.InputTag("kt4PFJets"),
    correctors = cms.vstring('kt4PFL1FastL2L3')
)


process.kt4PFJetsL1FastL2L3Residual = cms.EDProducer("PFJetCorrectionProducer",
    src = cms.InputTag("kt4PFJets"),
    correctors = cms.vstring('kt4PFL1FastL2L3Residual')
)


process.kt4PFJetsL1L2L3 = cms.EDProducer("PFJetCorrectionProducer",
    src = cms.InputTag("kt4PFJets"),
    correctors = cms.vstring('kt4PFL1L2L3')
)


process.kt4PFJetsL1L2L3Residual = cms.EDProducer("PFJetCorrectionProducer",
    src = cms.InputTag("kt4PFJets"),
    correctors = cms.vstring('kt4PFL1L2L3Residual')
)


process.kt4PFJetsL2L3 = cms.EDProducer("PFJetCorrectionProducer",
    src = cms.InputTag("kt4PFJets"),
    correctors = cms.vstring('kt4PFL2L3')
)


process.kt4PFJetsL2L3Residual = cms.EDProducer("PFJetCorrectionProducer",
    src = cms.InputTag("kt4PFJets"),
    correctors = cms.vstring('kt4PFL2L3Residual')
)


process.kt6CaloJetsL1FastL2L3 = cms.EDProducer("CaloJetCorrectionProducer",
    src = cms.InputTag("kt6CaloJets"),
    correctors = cms.vstring('kt6CaloL1FastL2L3')
)


process.kt6CaloJetsL1FastL2L3Residual = cms.EDProducer("CaloJetCorrectionProducer",
    src = cms.InputTag("kt6CaloJets"),
    correctors = cms.vstring('kt6CaloL1FastL2L3Residual')
)


process.kt6CaloJetsL1L2L3 = cms.EDProducer("CaloJetCorrectionProducer",
    src = cms.InputTag("kt6CaloJets"),
    correctors = cms.vstring('kt6CaloL1L2L3')
)


process.kt6CaloJetsL1L2L3Residual = cms.EDProducer("CaloJetCorrectionProducer",
    src = cms.InputTag("kt6CaloJets"),
    correctors = cms.vstring('kt6CaloL1L2L3Residual')
)


process.kt6CaloJetsL2L3 = cms.EDProducer("CaloJetCorrectionProducer",
    src = cms.InputTag("kt6CaloJets"),
    correctors = cms.vstring('kt6CaloL2L3')
)


process.kt6CaloJetsL2L3Residual = cms.EDProducer("CaloJetCorrectionProducer",
    src = cms.InputTag("kt6CaloJets"),
    correctors = cms.vstring('kt6CaloL2L3Residual')
)


process.kt6PFJetsL1FastL2L3 = cms.EDProducer("PFJetCorrectionProducer",
    src = cms.InputTag("kt6PFJets"),
    correctors = cms.vstring('kt6PFL1FastL2L3')
)


process.kt6PFJetsL1FastL2L3Residual = cms.EDProducer("PFJetCorrectionProducer",
    src = cms.InputTag("kt6PFJets"),
    correctors = cms.vstring('kt6PFL1FastL2L3Residual')
)


process.kt6PFJetsL1L2L3 = cms.EDProducer("PFJetCorrectionProducer",
    src = cms.InputTag("kt6PFJets"),
    correctors = cms.vstring('kt6PFL1L2L3')
)


process.kt6PFJetsL1L2L3Residual = cms.EDProducer("PFJetCorrectionProducer",
    src = cms.InputTag("kt6PFJets"),
    correctors = cms.vstring('kt6PFL1L2L3Residual')
)


process.kt6PFJetsL2L3 = cms.EDProducer("PFJetCorrectionProducer",
    src = cms.InputTag("kt6PFJets"),
    correctors = cms.vstring('kt6PFL2L3')
)


process.kt6PFJetsL2L3Residual = cms.EDProducer("PFJetCorrectionProducer",
    src = cms.InputTag("kt6PFJets"),
    correctors = cms.vstring('kt6PFL2L3Residual')
)


process.muonCaloMETcorr = cms.EDProducer("MuonMETcorrInputProducer",
    srcMuonCorrections = cms.InputTag("muonMETValueMapProducer","muCorrData"),
    src = cms.InputTag("muons")
)


process.muonMatch = cms.EDProducer("MCMatcher",
    src = cms.InputTag("muons"),
    maxDPtRel = cms.double(0.5),
    mcPdgId = cms.vint32(13),
    mcStatus = cms.vint32(1),
    resolveByMatchQuality = cms.bool(False),
    maxDeltaR = cms.double(0.5),
    checkCharge = cms.bool(True),
    resolveAmbiguities = cms.bool(True),
    matched = cms.InputTag("genParticles")
)


process.particleFlowDisplacedVertex = cms.EDProducer("PFDisplacedVertexProducer",
    verbose = cms.untracked.bool(False),
    primaryVertexCut = cms.double(2.5),
    avfParameters = cms.PSet(
        sigmacut = cms.double(6.0),
        ratio = cms.double(0.25),
        Tini = cms.double(256.0)
    ),
    minAdaptWeight = cms.double(0.5),
    switchOff2TrackVertex = cms.untracked.bool(True),
    vertexIdentifierParameters = cms.PSet(
        logPrimSec_min = cms.double(0.0),
        pt_kink_min = cms.double(3.0),
        masses = cms.vdouble(0.05, 0.485, 0.515, 0.48, 0.52, 
            1.107, 1.125, 0.2),
        angles = cms.vdouble(15, 15),
        pt_min = cms.double(0.5),
        bIdentifyVertices = cms.bool(True),
        looper_eta_max = cms.double(0.1)
    ),
    mainVertexLabel = cms.InputTag("offlinePrimaryVertices"),
    vertexCandidatesLabel = cms.InputTag("particleFlowDisplacedVertexCandidate"),
    transvSize = cms.double(1.0),
    offlineBeamSpotLabel = cms.InputTag("offlineBeamSpot"),
    tracksSelectorParameters = cms.PSet(
        nChi2_max = cms.double(5.0),
        bSelectTracks = cms.bool(True),
        nOuterHits_max = cms.int32(9),
        dxy_min = cms.double(0.2),
        nHits_min = cms.int32(6),
        nChi2_min = cms.double(0.5),
        pt_min = cms.double(0.2),
        quality = cms.string('HighPurity')
    ),
    debug = cms.untracked.bool(False),
    tobCut = cms.double(100),
    longSize = cms.double(5),
    tecCut = cms.double(220)
)


process.patElectrons = cms.EDProducer("PATElectronProducer",
    embedHighLevelSelection = cms.bool(True),
    embedRecHits = cms.bool(True),
    embedGsfElectronCore = cms.bool(True),
    electronSource = cms.InputTag("gsfElectrons"),
    resolutions = cms.PSet(

    ),
    pfElectronSource = cms.InputTag("particleFlow"),
    userIsolation = cms.PSet(

    ),
    embedSuperCluster = cms.bool(True),
    embedSeedCluster = cms.bool(True),
    embedPFCandidate = cms.bool(True),
    pfCandidateMap = cms.InputTag("particleFlow","electrons"),
    addElectronID = cms.bool(True),
    efficiencies = cms.PSet(

    ),
    reducedBarrelRecHitCollection = cms.InputTag("reducedEcalRecHitsEB"),
    embedGsfTrack = cms.bool(True),
    useParticleFlow = cms.bool(False),
    userData = cms.PSet(
        userCands = cms.PSet(
            src = cms.VInputTag("")
        ),
        userInts = cms.PSet(
            src = cms.VInputTag("")
        ),
        userFloats = cms.PSet(
            src = cms.VInputTag("")
        ),
        userClasses = cms.PSet(
            src = cms.VInputTag("")
        ),
        userFunctionLabels = cms.vstring(),
        userFunctions = cms.vstring()
    ),
    embedTrack = cms.bool(True),
    addEfficiencies = cms.bool(False),
    usePV = cms.bool(True),
    reducedEndcapRecHitCollection = cms.InputTag("reducedEcalRecHitsEE"),
    pvSrc = cms.InputTag("offlinePrimaryVertices"),
    electronIDSources = cms.PSet(
        eidTight = cms.InputTag("eidTight"),
        eidLoose = cms.InputTag("eidLoose"),
        eidRobustTight = cms.InputTag("eidRobustTight"),
        eidRobustHighEnergy = cms.InputTag("eidRobustHighEnergy"),
        eidRobustLoose = cms.InputTag("eidRobustLoose")
    ),
    genParticleMatch = cms.InputTag(""),
    beamLineSrc = cms.InputTag("offlineBeamSpot"),
    addGenMatch = cms.bool(False),
    addResolutions = cms.bool(False),
    isoDeposits = cms.PSet(

    ),
    embedGenMatch = cms.bool(False)
)


process.patHemispheres = cms.EDProducer("PATHemisphereProducer",
    patJets = cms.InputTag("cleanLayer1Jets"),
    maxTauEta = cms.double(-1),
    maxPhotonEta = cms.double(5),
    minMuonEt = cms.double(7),
    patMuons = cms.InputTag("cleanLayer1Muons"),
    seedMethod = cms.int32(3),
    patElectrons = cms.InputTag("cleanLayer1Electrons"),
    patMets = cms.InputTag("layer1METs"),
    maxMuonEta = cms.double(5),
    minTauEt = cms.double(1000000),
    minPhotonEt = cms.double(200000),
    minElectronEt = cms.double(7),
    patPhotons = cms.InputTag("cleanLayer1Photons"),
    combinationMethod = cms.int32(3),
    maxJetEta = cms.double(5),
    maxElectronEta = cms.double(5),
    minJetEt = cms.double(30),
    patTaus = cms.InputTag("cleanLayer1Taus")
)


process.patJetCharge = cms.EDProducer("JetChargeProducer",
    var = cms.string('Pt'),
    src = cms.InputTag("jetTracksAssociatorAtVertex"),
    exp = cms.double(1.0)
)


process.patJetCorrFactors = cms.EDProducer("JetCorrFactorsProducer",
    src = cms.InputTag("ak5PFJets"),
    emf = cms.bool(False),
    primaryVertices = cms.InputTag("offlinePrimaryVertices"),
    levels = cms.vstring('L1FastJet', 
        'L2Relative', 
        'L3Absolute', 
        'L2L3Residual'),
    useNPV = cms.bool(True),
    rho = cms.InputTag("kt6PFJets","rho"),
    useRho = cms.bool(True),
    payload = cms.string('AK5PF'),
    flavorType = cms.string('J')
)


process.patJetFlavourAssociation = cms.EDProducer("JetFlavourIdentifier",
    srcByReference = cms.InputTag("patJetPartonAssociation"),
    physicsDefinition = cms.bool(False)
)


process.patJetGenJetMatch = cms.EDProducer("GenJetMatcher",
    src = cms.InputTag("ak5PFJets"),
    maxDPtRel = cms.double(3.0),
    mcPdgId = cms.vint32(),
    mcStatus = cms.vint32(),
    resolveByMatchQuality = cms.bool(False),
    maxDeltaR = cms.double(0.4),
    checkCharge = cms.bool(False),
    resolveAmbiguities = cms.bool(True),
    matched = cms.InputTag("ak5GenJets")
)


process.patJetPartonAssociation = cms.EDProducer("JetPartonMatcher",
    jets = cms.InputTag("ak5PFJets"),
    coneSizeToAssociate = cms.double(0.3),
    partons = cms.InputTag("patJetPartons")
)


process.patJetPartonMatch = cms.EDProducer("MCMatcher",
    src = cms.InputTag("ak5PFJets"),
    maxDPtRel = cms.double(3.0),
    mcPdgId = cms.vint32(1, 2, 3, 4, 5, 
        21),
    mcStatus = cms.vint32(3),
    resolveByMatchQuality = cms.bool(False),
    maxDeltaR = cms.double(0.4),
    checkCharge = cms.bool(False),
    resolveAmbiguities = cms.bool(True),
    matched = cms.InputTag("genParticles")
)


process.patJetPartons = cms.EDProducer("PartonSelector",
    src = cms.InputTag("genParticles"),
    withLeptons = cms.bool(False)
)


process.patJets = cms.EDProducer("PATJetProducer",
    addJetCharge = cms.bool(True),
    addGenJetMatch = cms.bool(False),
    embedPFCandidates = cms.bool(True),
    embedGenJetMatch = cms.bool(True),
    addAssociatedTracks = cms.bool(True),
    partonJetSource = cms.InputTag("NOT_IMPLEMENTED"),
    addGenPartonMatch = cms.bool(False),
    JetPartonMapSource = cms.InputTag(""),
    resolutions = cms.PSet(

    ),
    genPartonMatch = cms.InputTag(""),
    addTagInfos = cms.bool(False),
    addPartonJetMatch = cms.bool(False),
    embedGenPartonMatch = cms.bool(False),
    efficiencies = cms.PSet(

    ),
    genJetMatch = cms.InputTag(""),
    userData = cms.PSet(
        userCands = cms.PSet(
            src = cms.VInputTag("")
        ),
        userInts = cms.PSet(
            src = cms.VInputTag("")
        ),
        userFloats = cms.PSet(
            src = cms.VInputTag("")
        ),
        userClasses = cms.PSet(
            src = cms.VInputTag("")
        ),
        userFunctionLabels = cms.vstring(),
        userFunctions = cms.vstring()
    ),
    jetSource = cms.InputTag("ak5PFJets"),
    addEfficiencies = cms.bool(False),
    jetCorrFactorsSource = cms.VInputTag(cms.InputTag("patJetCorrFactors")),
    trackAssociationSource = cms.InputTag("jetTracksAssociatorAtVertex"),
    tagInfoSources = cms.VInputTag(),
    discriminatorSources = cms.VInputTag(cms.InputTag("combinedSecondaryVertexBJetTags"), cms.InputTag("combinedSecondaryVertexMVABJetTags"), cms.InputTag("jetBProbabilityBJetTags"), cms.InputTag("jetProbabilityBJetTags"), cms.InputTag("simpleSecondaryVertexHighEffBJetTags"), 
        cms.InputTag("simpleSecondaryVertexHighPurBJetTags"), cms.InputTag("softElectronByPtBJetTags"), cms.InputTag("softElectronByIP3dBJetTags"), cms.InputTag("softMuonBJetTags"), cms.InputTag("softMuonByPtBJetTags"), 
        cms.InputTag("softMuonByIP3dBJetTags"), cms.InputTag("trackCountingHighEffBJetTags"), cms.InputTag("trackCountingHighPurBJetTags")),
    addBTagInfo = cms.bool(False),
    embedCaloTowers = cms.bool(True),
    addResolutions = cms.bool(False),
    getJetMCFlavour = cms.bool(False),
    addDiscriminators = cms.bool(True),
    jetChargeSource = cms.InputTag("patJetCharge"),
    addJetCorrFactors = cms.bool(True),
    jetIDMap = cms.InputTag("ak5JetID"),
    addJetID = cms.bool(False)
)


process.patMETs = cms.EDProducer("PATMETProducer",
    metSource = cms.InputTag("pfType1CorrectedMet"),
    userData = cms.PSet(
        userCands = cms.PSet(
            src = cms.VInputTag("")
        ),
        userInts = cms.PSet(
            src = cms.VInputTag("")
        ),
        userFloats = cms.PSet(
            src = cms.VInputTag("")
        ),
        userClasses = cms.PSet(
            src = cms.VInputTag("")
        ),
        userFunctionLabels = cms.vstring(),
        userFunctions = cms.vstring()
    ),
    addResolutions = cms.bool(False),
    addEfficiencies = cms.bool(False),
    genMETSource = cms.InputTag(""),
    efficiencies = cms.PSet(

    ),
    addGenMET = cms.bool(False),
    addMuonCorrections = cms.bool(False),
    muonSource = cms.InputTag("muons"),
    resolutions = cms.PSet(

    )
)


process.patMETsPF = cms.EDProducer("PATMETProducer",
    metSource = cms.InputTag("pfType1CorrectedMet"),
    userData = cms.PSet(
        userCands = cms.PSet(
            src = cms.VInputTag("")
        ),
        userInts = cms.PSet(
            src = cms.VInputTag("")
        ),
        userFloats = cms.PSet(
            src = cms.VInputTag("")
        ),
        userClasses = cms.PSet(
            src = cms.VInputTag("")
        ),
        userFunctionLabels = cms.vstring(),
        userFunctions = cms.vstring()
    ),
    addResolutions = cms.bool(False),
    muonSource = cms.InputTag("muons"),
    addEfficiencies = cms.bool(False),
    genMETSource = cms.InputTag(""),
    efficiencies = cms.PSet(

    ),
    addGenMET = cms.bool(False),
    addMuonCorrections = cms.bool(False),
    resolutions = cms.PSet(

    )
)


process.patMHTs = cms.EDProducer("PATMHTProducer",
    verbose = cms.double(0.0),
    muonEtaMax = cms.double(2.5),
    jetTag = cms.untracked.InputTag("allLayer1Jets"),
    eleEtaMax = cms.double(3.0),
    noHF = cms.bool(False),
    muonTag = cms.untracked.InputTag("allLayer1Muons"),
    CaloTowerTag = cms.InputTag("towerMaker"),
    elePhiUncertaintyParameter0 = cms.double(0.01),
    uncertaintyScaleFactor = cms.double(1.0),
    muonPtMin = cms.double(10.0),
    eleEtUncertaintyParameter0 = cms.double(0.01),
    useHO = cms.bool(False),
    jetEtUncertaintyParameter2 = cms.double(0.033),
    jetEtUncertaintyParameter1 = cms.double(1.25),
    jetEMfracMax = cms.double(0.9),
    jetPhiUncertaintyParameter2 = cms.double(0.023),
    jetPhiUncertaintyParameter0 = cms.double(4.75),
    jetPhiUncertaintyParameter1 = cms.double(-0.426),
    tauTag = cms.untracked.InputTag("allLayer1Taus"),
    jetEtUncertaintyParameter0 = cms.double(5.6),
    electronTag = cms.untracked.InputTag("allLayer1Electrons"),
    jetEtaMax = cms.double(5.0),
    elePtMin = cms.double(10.0),
    jetPtMin = cms.double(20.0),
    muonEtUncertaintyParameter0 = cms.double(0.01),
    photonTag = cms.untracked.InputTag("allLayer1Photons"),
    muonPhiUncertaintyParameter0 = cms.double(0.01),
    controlledUncertainty = cms.bool(True),
    towerEtThreshold = cms.double(0.5)
)


process.patMuons = cms.EDProducer("PATMuonProducer",
    embedTpfmsMuon = cms.bool(True),
    embedHighLevelSelection = cms.bool(True),
    embedCaloMETMuonCorrs = cms.bool(True),
    caloMETMuonCorrs = cms.InputTag("muonMETValueMapProducer","muCorrData"),
    resolutions = cms.PSet(

    ),
    embedDytMuon = cms.bool(True),
    userIsolation = cms.PSet(

    ),
    embedPFCandidate = cms.bool(True),
    pfMuonSource = cms.InputTag("particleFlow"),
    efficiencies = cms.PSet(

    ),
    embedStandAloneMuon = cms.bool(True),
    useParticleFlow = cms.bool(False),
    userData = cms.PSet(
        userCands = cms.PSet(
            src = cms.VInputTag("")
        ),
        userInts = cms.PSet(
            src = cms.VInputTag("")
        ),
        userFloats = cms.PSet(
            src = cms.VInputTag("")
        ),
        userClasses = cms.PSet(
            src = cms.VInputTag("")
        ),
        userFunctionLabels = cms.vstring(),
        userFunctions = cms.vstring()
    ),
    embedTrack = cms.bool(False),
    addEfficiencies = cms.bool(False),
    usePV = cms.bool(True),
    embedTcMETMuonCorrs = cms.bool(True),
    pvSrc = cms.InputTag("offlinePrimaryVertices"),
    embedMuonBestTrack = cms.bool(True),
    muonSource = cms.InputTag("muons"),
    embedCombinedMuon = cms.bool(True),
    genParticleMatch = cms.InputTag(""),
    beamLineSrc = cms.InputTag("offlineBeamSpot"),
    addGenMatch = cms.bool(False),
    addResolutions = cms.bool(False),
    isoDeposits = cms.PSet(

    ),
    embedGenMatch = cms.bool(False),
    tcMETMuonCorrs = cms.InputTag("muonTCMETValueMapProducer","muCorrData"),
    embedPickyMuon = cms.bool(True)
)


process.patPhotons = cms.EDProducer("PATPhotonProducer",
    userData = cms.PSet(
        userCands = cms.PSet(
            src = cms.VInputTag("")
        ),
        userInts = cms.PSet(
            src = cms.VInputTag("")
        ),
        userFloats = cms.PSet(
            src = cms.VInputTag("")
        ),
        userClasses = cms.PSet(
            src = cms.VInputTag("")
        ),
        userFunctionLabels = cms.vstring(),
        userFunctions = cms.vstring()
    ),
    addGenMatch = cms.bool(False),
    addResolutions = cms.bool(False),
    addEfficiencies = cms.bool(False),
    photonIDSources = cms.PSet(
        PhotonCutBasedIDTight = cms.InputTag("PhotonIDProd","PhotonCutBasedIDTight"),
        PhotonCutBasedIDLoose = cms.InputTag("PhotonIDProd","PhotonCutBasedIDLoose")
    ),
    isoDeposits = cms.PSet(

    ),
    efficiencies = cms.PSet(

    ),
    embedSuperCluster = cms.bool(True),
    embedGenMatch = cms.bool(False),
    resolutions = cms.PSet(

    ),
    addPhotonID = cms.bool(True),
    photonSource = cms.InputTag("photons"),
    userIsolation = cms.PSet(

    ),
    genParticleMatch = cms.InputTag("")
)


process.patTaus = cms.EDProducer("PATTauProducer",
    tauIDSources = cms.PSet(
        byLooseCombinedIsolationDeltaBetaCorr = cms.InputTag("hpsPFTauDiscriminationByLooseCombinedIsolationDBSumPtCorr"),
        againstMuonMedium = cms.InputTag("hpsPFTauDiscriminationByMediumMuonRejection"),
        byVLooseCombinedIsolationDeltaBetaCorr = cms.InputTag("hpsPFTauDiscriminationByVLooseCombinedIsolationDBSumPtCorr"),
        decayModeFinding = cms.InputTag("hpsPFTauDiscriminationByDecayModeFinding"),
        againstElectronMVA = cms.InputTag("hpsPFTauDiscriminationByMVAElectronRejection"),
        againstMuonLoose = cms.InputTag("hpsPFTauDiscriminationByLooseMuonRejection"),
        againstElectronTight = cms.InputTag("hpsPFTauDiscriminationByTightElectronRejection"),
        byMediumCombinedIsolationDeltaBetaCorr = cms.InputTag("hpsPFTauDiscriminationByMediumCombinedIsolationDBSumPtCorr"),
        againstMuonTight = cms.InputTag("hpsPFTauDiscriminationByTightMuonRejection"),
        againstElectronMedium = cms.InputTag("hpsPFTauDiscriminationByMediumElectronRejection"),
        byTightCombinedIsolationDeltaBetaCorr = cms.InputTag("hpsPFTauDiscriminationByTightCombinedIsolationDBSumPtCorr"),
        againstElectronLoose = cms.InputTag("hpsPFTauDiscriminationByLooseElectronRejection")
    ),
    addGenJetMatch = cms.bool(False),
    embedGenJetMatch = cms.bool(False),
    embedLeadTrack = cms.bool(False),
    embedLeadPFCand = cms.bool(False),
    embedSignalPFChargedHadrCands = cms.bool(False),
    addTauJetCorrFactors = cms.bool(False),
    resolutions = cms.PSet(

    ),
    userIsolation = cms.PSet(
        pfAllParticles = cms.PSet(
            threshold = cms.double(0.0),
            src = cms.InputTag("tauIsoDepositPFCandidates"),
            deltaR = cms.double(0.5)
        ),
        pfNeutralHadron = cms.PSet(
            threshold = cms.double(0.0),
            src = cms.InputTag("tauIsoDepositPFNeutralHadrons"),
            deltaR = cms.double(0.5)
        ),
        pfChargedHadron = cms.PSet(
            threshold = cms.double(0.0),
            src = cms.InputTag("tauIsoDepositPFChargedHadrons"),
            deltaR = cms.double(0.5)
        ),
        pfGamma = cms.PSet(
            threshold = cms.double(0.0),
            src = cms.InputTag("tauIsoDepositPFGammas"),
            deltaR = cms.double(0.5)
        )
    ),
    embedIsolationPFGammaCands = cms.bool(False),
    embedSignalPFGammaCands = cms.bool(False),
    efficiencies = cms.PSet(

    ),
    genJetMatch = cms.InputTag(""),
    embedIsolationPFCands = cms.bool(False),
    userData = cms.PSet(
        userCands = cms.PSet(
            src = cms.VInputTag("")
        ),
        userInts = cms.PSet(
            src = cms.VInputTag("")
        ),
        userFloats = cms.PSet(
            src = cms.VInputTag("")
        ),
        userClasses = cms.PSet(
            src = cms.VInputTag("")
        ),
        userFunctionLabels = cms.vstring(),
        userFunctions = cms.vstring()
    ),
    embedSignalPFCands = cms.bool(False),
    addEfficiencies = cms.bool(False),
    embedSignalTracks = cms.bool(False),
    tauSource = cms.InputTag("hpsPFTauProducer"),
    tauJetCorrFactorsSource = cms.VInputTag(cms.InputTag("patTauJetCorrFactors")),
    embedIsolationPFNeutralHadrCands = cms.bool(False),
    addTauID = cms.bool(True),
    genParticleMatch = cms.InputTag(""),
    addGenMatch = cms.bool(False),
    addResolutions = cms.bool(False),
    embedIsolationPFChargedHadrCands = cms.bool(False),
    embedIsolationTracks = cms.bool(False),
    embedSignalPFNeutralHadrCands = cms.bool(False),
    isoDeposits = cms.PSet(
        pfAllParticles = cms.InputTag("tauIsoDepositPFCandidates"),
        pfNeutralHadron = cms.InputTag("tauIsoDepositPFNeutralHadrons"),
        pfChargedHadron = cms.InputTag("tauIsoDepositPFChargedHadrons"),
        pfGamma = cms.InputTag("tauIsoDepositPFGammas")
    ),
    embedLeadPFChargedHadrCand = cms.bool(False),
    embedGenMatch = cms.bool(False),
    embedLeadPFNeutralCand = cms.bool(False)
)


process.pfCandMETcorr = cms.EDProducer("PFCandMETcorrInputProducer",
    src = cms.InputTag("pfCandsNotInJet")
)


process.pfCandidateToVertexAssociation = cms.EDProducer("PFCand_AssoMap",
    ConversionsCollection = cms.InputTag("allConversions"),
    NIVertexCollection = cms.InputTag("particleFlowDisplacedVertex"),
    BeamSpot = cms.InputTag("offlineBeamSpot"),
    V0KshortCollection = cms.InputTag("generalV0Candidates","Kshort"),
    V0LambdaCollection = cms.InputTag("generalV0Candidates","Lambda"),
    VertexTrackAssociationMap = cms.InputTag("trackToVertexAssociation"),
    VertexAssClosest = cms.untracked.bool(True),
    VertexCollection = cms.InputTag("offlinePrimaryVertices"),
    UseBeamSpotCompatibility = cms.untracked.bool(True),
    PFCandidateCollection = cms.InputTag("particleFlow"),
    ignoreMissingCollection = cms.bool(True)
)


process.pfCandsNotInJet = cms.EDProducer("TPPFJetsOnPFCandidates",
    bottomCollection = cms.InputTag("particleFlow"),
    enable = cms.bool(True),
    topCollection = cms.InputTag("ak5PFJets"),
    name = cms.untracked.string('noJet'),
    verbose = cms.untracked.bool(False)
)


process.pfJetMETcorr = cms.EDProducer("PFJetMETcorrInputProducer",
    src = cms.InputTag("ak5PFJets"),
    type1JetPtThreshold = cms.double(10.0),
    skipEMfractionThreshold = cms.double(0.9),
    skipEM = cms.bool(True),
    offsetCorrLabel = cms.string('AK5PFL1FastJet'),
    skipMuons = cms.bool(True),
    skipMuonSelection = cms.string('isGlobalMuon | isStandAloneMuon'),
    jetCorrEtaMax = cms.double(9.9),
    jetCorrLabel = cms.string('AK5PFCombinedCorrector')
)


process.pfMETcorrType0 = cms.EDProducer("Type0PFMETcorrInputProducer",
    srcPFCandidateToVertexAssociations = cms.InputTag("pfCandidateToVertexAssociation"),
    correction = cms.PSet(
        formula = cms.string('-([0] + [1]*x)*(1.0 + TMath::Erf(-[2]*TMath::Power(x, [3])))'),
        par3 = cms.double(0.909209),
        par2 = cms.double(0.0303531),
        par1 = cms.double(-0.703151),
        par0 = cms.double(0.0)
    ),
    srcHardScatterVertex = cms.InputTag("selectedPrimaryVertexHighestPtTrackSumForPFMEtCorrType0"),
    minDz = cms.double(0.2)
)


process.pfNoJet = cms.EDProducer("TPPFJetsOnPFCandidates",
    bottomCollection = cms.InputTag("pfNoElectron"),
    enable = cms.bool(True),
    topCollection = cms.InputTag("pfJets"),
    name = cms.untracked.string('noJet'),
    verbose = cms.untracked.bool(False)
)


process.pfNoPileUp = cms.EDProducer("TPPFCandidatesOnPFCandidates",
    bottomCollection = cms.InputTag("particleFlowTmp"),
    enable = cms.bool(True),
    topCollection = cms.InputTag("pfPileUp"),
    name = cms.untracked.string('pileUpOnPFCandidates'),
    verbose = cms.untracked.bool(False)
)


process.pfNoPileUpIso = cms.EDProducer("TPPFCandidatesOnPFCandidates",
    bottomCollection = cms.InputTag("particleFlow"),
    enable = cms.bool(True),
    topCollection = cms.InputTag("pfPileUpIso"),
    name = cms.untracked.string('pileUpOnPFCandidates'),
    verbose = cms.untracked.bool(False)
)


process.pfPileUp = cms.EDProducer("PFPileUp",
    PFCandidates = cms.InputTag("particleFlowTmp"),
    Enable = cms.bool(True),
    checkClosestZVertex = cms.bool(True),
    verbose = cms.untracked.bool(False),
    Vertices = cms.InputTag("offlinePrimaryVertices")
)


process.pfPileUpIso = cms.EDProducer("PFPileUp",
    checkClosestZVertex = cms.bool(True),
    Enable = cms.bool(True),
    PFCandidates = cms.InputTag("particleFlow"),
    verbose = cms.untracked.bool(False),
    Vertices = cms.InputTag("offlinePrimaryVertices")
)


process.pfType1CorrectedMet = cms.EDProducer("CorrectedPFMETProducer",
    src = cms.InputTag("pfMet"),
    applyType1Corrections = cms.bool(True),
    srcType1Corrections = cms.VInputTag(cms.InputTag("pfMETcorrType0"), cms.InputTag("pfJetMETcorr","type1")),
    type0Rsoft = cms.double(0.6),
    applyType2Corrections = cms.bool(False),
    srcCHSSums = cms.VInputTag(cms.InputTag("pfchsMETcorr","type0")),
    applyType0Corrections = cms.bool(False)
)


process.pfType1p2CorrectedMet = cms.EDProducer("CorrectedPFMETProducer",
    src = cms.InputTag("pfMet"),
    applyType1Corrections = cms.bool(True),
    type2CorrFormula = cms.string('A'),
    srcUnclEnergySums = cms.VInputTag(cms.InputTag("pfJetMETcorr","type2"), cms.InputTag("pfJetMETcorr","offset"), cms.InputTag("pfCandMETcorr")),
    srcType1Corrections = cms.VInputTag(cms.InputTag("pfJetMETcorr","type1")),
    type0Rsoft = cms.double(0.6),
    applyType2Corrections = cms.bool(True),
    srcCHSSums = cms.VInputTag(cms.InputTag("pfchsMETcorr","type0")),
    applyType0Corrections = cms.bool(False),
    type2CorrParameter = cms.PSet(
        A = cms.double(1.4)
    )
)


process.pfchsMETcorr = cms.EDProducer("PFchsMETcorrInputProducer",
    src = cms.InputTag("offlinePrimaryVertices"),
    goodVtxNdof = cms.uint32(4),
    goodVtxZ = cms.double(24)
)


process.photonMatch = cms.EDProducer("MCMatcher",
    src = cms.InputTag("photons"),
    maxDPtRel = cms.double(1.0),
    mcPdgId = cms.vint32(22),
    mcStatus = cms.vint32(1),
    resolveByMatchQuality = cms.bool(False),
    maxDeltaR = cms.double(0.2),
    checkCharge = cms.bool(True),
    resolveAmbiguities = cms.bool(True),
    matched = cms.InputTag("genParticles")
)


process.pileupJetIdProducer = cms.EDProducer("PileupJetIdProducer",
    algos = cms.VPSet(cms.PSet(
        tmvaVariables = cms.vstring('frac01', 
            'frac02', 
            'frac03', 
            'frac04', 
            'frac05', 
            'nvtx', 
            'beta', 
            'betaStar'),
        tmvaMethod = cms.string('BDT_simpleNoVtxCat'),
        cutBased = cms.bool(False),
        tmvaWeights = cms.string('CMGTools/External/data/TMVAClassification_5x_BDT_simpleNoVtxCat.weights.xml'),
        tmvaSpectators = cms.vstring('jetPt', 
            'jetEta'),
        label = cms.string('simple'),
        version = cms.int32(-1),
        JetIdParams = cms.PSet(
            Pt2030_Tight = cms.vdouble(-0.33, -0.3, -0.15, -0.49),
            Pt2030_Loose = cms.vdouble(-0.8, -0.73, -0.68, -0.77),
            Pt3050_Medium = cms.vdouble(-0.4, -0.54, -0.5, -0.65),
            Pt1020_Tight = cms.vdouble(-0.84, -0.8, -0.75, -0.82),
            Pt2030_Medium = cms.vdouble(-0.4, -0.54, -0.5, -0.65),
            Pt010_Tight = cms.vdouble(-0.84, -0.8, -0.75, -0.82),
            Pt1020_Loose = cms.vdouble(-0.95, -0.96, -0.94, -0.95),
            Pt010_Medium = cms.vdouble(-0.85, -0.93, -0.91, -0.92),
            Pt1020_Medium = cms.vdouble(-0.85, -0.93, -0.91, -0.92),
            Pt010_Loose = cms.vdouble(-0.95, -0.96, -0.94, -0.95),
            Pt3050_Loose = cms.vdouble(-0.8, -0.73, -0.68, -0.77),
            Pt3050_Tight = cms.vdouble(-0.33, -0.3, -0.15, -0.49)
        ),
        impactParTkThreshold = cms.double(1.0)
    ), 
        cms.PSet(
            tmvaVariables = cms.vstring('frac01', 
                'frac02', 
                'frac03', 
                'frac04', 
                'frac05', 
                'dR2Mean', 
                'nvtx', 
                'nNeutrals', 
                'beta', 
                'betaStar', 
                'dZ', 
                'nCharged'),
            tmvaMethod = cms.string('BDT_fullPlusRMS'),
            cutBased = cms.bool(False),
            tmvaWeights = cms.string('CMGTools/External/data/TMVAClassification_5x_BDT_fullPlusRMS.weights.xml'),
            tmvaSpectators = cms.vstring('jetPt', 
                'jetEta'),
            label = cms.string('full'),
            version = cms.int32(-1),
            JetIdParams = cms.PSet(
                Pt2030_Tight = cms.vdouble(-0.38, -0.32, -0.14, -0.48),
                Pt2030_Loose = cms.vdouble(-0.8, -0.74, -0.68, -0.77),
                Pt3050_Medium = cms.vdouble(-0.4, -0.49, -0.5, -0.65),
                Pt1020_Tight = cms.vdouble(-0.83, -0.81, -0.74, -0.81),
                Pt2030_Medium = cms.vdouble(-0.4, -0.49, -0.5, -0.65),
                Pt010_Tight = cms.vdouble(-0.83, -0.81, -0.74, -0.81),
                Pt1020_Loose = cms.vdouble(-0.95, -0.96, -0.94, -0.95),
                Pt010_Medium = cms.vdouble(-0.83, -0.92, -0.9, -0.92),
                Pt1020_Medium = cms.vdouble(-0.83, -0.92, -0.9, -0.92),
                Pt010_Loose = cms.vdouble(-0.95, -0.96, -0.94, -0.95),
                Pt3050_Loose = cms.vdouble(-0.8, -0.74, -0.68, -0.77),
                Pt3050_Tight = cms.vdouble(-0.38, -0.32, -0.14, -0.48)
            ),
            impactParTkThreshold = cms.double(1.0)
        ), 
        cms.PSet(
            cutBased = cms.bool(True),
            JetIdParams = cms.PSet(
                Pt010_RMSLoose = cms.vdouble(0.08, 0.07, 0.05, 0.07),
                Pt3050_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
                Pt3050_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
                Pt010_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
                Pt1020_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
                Pt010_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
                Pt1020_RMSLoose = cms.vdouble(0.08, 0.07, 0.05, 0.07),
                Pt1020_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
                Pt2030_RMSTight = cms.vdouble(0.05, 0.07, 0.03, 0.045),
                Pt3050_RMSTight = cms.vdouble(0.05, 0.06, 0.03, 0.04),
                Pt1020_RMSTight = cms.vdouble(0.06, 0.07, 0.04, 0.05),
                Pt3050_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
                Pt3050_RMSLoose = cms.vdouble(0.07, 0.06, 0.04, 0.05),
                Pt2030_RMSLoose = cms.vdouble(0.08, 0.07, 0.04, 0.055),
                Pt010_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
                Pt2030_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
                Pt1020_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
                Pt2030_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
                Pt2030_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
                Pt2030_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
                Pt010_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
                Pt3050_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
                Pt1020_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
                Pt010_RMSTight = cms.vdouble(0.06, 0.07, 0.04, 0.05)
            ),
            impactParTkThreshold = cms.double(1.0),
            label = cms.string('cutbased')
        )),
    inputIsCorrected = cms.bool(True),
    vertexes = cms.InputTag("offlinePrimaryVertices"),
    produceJetIds = cms.bool(True),
    jec = cms.string('AK5PF'),
    applyJec = cms.bool(False),
    jetids = cms.InputTag(""),
    rho = cms.InputTag("kt6PFJets","rho"),
    jets = cms.InputTag("selectedPatJetsPFlow"),
    runMvas = cms.bool(True)
)


process.puJetId = cms.EDProducer("PileupJetIdProducer",
    algos = cms.VPSet(cms.PSet(
        cutBased = cms.bool(True),
        JetIdParams = cms.PSet(
            Pt010_RMSLoose = cms.vdouble(0.08, 0.07, 0.05, 0.07),
            Pt3050_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
            Pt3050_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
            Pt010_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
            Pt1020_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
            Pt010_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
            Pt1020_RMSLoose = cms.vdouble(0.08, 0.07, 0.05, 0.07),
            Pt1020_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
            Pt2030_RMSTight = cms.vdouble(0.05, 0.07, 0.03, 0.045),
            Pt3050_RMSTight = cms.vdouble(0.05, 0.06, 0.03, 0.04),
            Pt1020_RMSTight = cms.vdouble(0.06, 0.07, 0.04, 0.05),
            Pt3050_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
            Pt3050_RMSLoose = cms.vdouble(0.07, 0.06, 0.04, 0.05),
            Pt2030_RMSLoose = cms.vdouble(0.08, 0.07, 0.04, 0.055),
            Pt010_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
            Pt2030_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
            Pt1020_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
            Pt2030_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
            Pt2030_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
            Pt2030_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
            Pt010_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
            Pt3050_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
            Pt1020_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
            Pt010_RMSTight = cms.vdouble(0.06, 0.07, 0.04, 0.05)
        ),
        impactParTkThreshold = cms.double(1.0),
        label = cms.string('cutbased')
    )),
    inputIsCorrected = cms.bool(True),
    vertexes = cms.InputTag("offlinePrimaryVertices"),
    produceJetIds = cms.bool(True),
    jec = cms.string('AK5PF'),
    applyJec = cms.bool(False),
    jetids = cms.InputTag(""),
    rho = cms.InputTag("kt6PFJets","rho"),
    jets = cms.InputTag("goodPatJets"),
    runMvas = cms.bool(False)
)


process.puJetIdChs = cms.EDProducer("PileupJetIdProducer",
    algos = cms.VPSet(cms.PSet(
        cutBased = cms.bool(True),
        JetIdParams = cms.PSet(
            Pt010_RMSLoose = cms.vdouble(0.08, 0.07, 0.05, 0.07),
            Pt3050_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
            Pt3050_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
            Pt010_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
            Pt1020_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
            Pt010_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
            Pt1020_RMSLoose = cms.vdouble(0.08, 0.07, 0.05, 0.07),
            Pt1020_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
            Pt2030_RMSTight = cms.vdouble(0.05, 0.07, 0.03, 0.045),
            Pt3050_RMSTight = cms.vdouble(0.05, 0.06, 0.03, 0.04),
            Pt1020_RMSTight = cms.vdouble(0.06, 0.07, 0.04, 0.05),
            Pt3050_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
            Pt3050_RMSLoose = cms.vdouble(0.07, 0.06, 0.04, 0.05),
            Pt2030_RMSLoose = cms.vdouble(0.08, 0.07, 0.04, 0.055),
            Pt010_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
            Pt2030_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
            Pt1020_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
            Pt2030_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
            Pt2030_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
            Pt2030_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
            Pt010_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
            Pt3050_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
            Pt1020_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
            Pt010_RMSTight = cms.vdouble(0.06, 0.07, 0.04, 0.05)
        ),
        impactParTkThreshold = cms.double(1.0),
        label = cms.string('cutbased')
    )),
    inputIsCorrected = cms.bool(True),
    vertexes = cms.InputTag("offlinePrimaryVertices"),
    produceJetIds = cms.bool(True),
    jec = cms.string('AK5PF'),
    applyJec = cms.bool(False),
    jetids = cms.InputTag(""),
    rho = cms.InputTag("kt6PFJets","rho"),
    jets = cms.InputTag("selectedPatJets"),
    runMvas = cms.bool(False)
)


process.puJetMva = cms.EDProducer("PileupJetIdProducer",
    algos = cms.VPSet(cms.PSet(
        tmvaVariables = cms.vstring('frac01', 
            'frac02', 
            'frac03', 
            'frac04', 
            'frac05', 
            'nvtx', 
            'beta', 
            'betaStar'),
        tmvaMethod = cms.string('BDT_simpleNoVtxCat'),
        cutBased = cms.bool(False),
        tmvaWeights = cms.string('CMGTools/External/data/TMVAClassification_5x_BDT_simpleNoVtxCat.weights.xml'),
        tmvaSpectators = cms.vstring('jetPt', 
            'jetEta'),
        label = cms.string('simple'),
        version = cms.int32(-1),
        JetIdParams = cms.PSet(
            Pt2030_Tight = cms.vdouble(-0.33, -0.3, -0.15, -0.49),
            Pt2030_Loose = cms.vdouble(-0.8, -0.73, -0.68, -0.77),
            Pt3050_Medium = cms.vdouble(-0.4, -0.54, -0.5, -0.65),
            Pt1020_Tight = cms.vdouble(-0.84, -0.8, -0.75, -0.82),
            Pt2030_Medium = cms.vdouble(-0.4, -0.54, -0.5, -0.65),
            Pt010_Tight = cms.vdouble(-0.84, -0.8, -0.75, -0.82),
            Pt1020_Loose = cms.vdouble(-0.95, -0.96, -0.94, -0.95),
            Pt010_Medium = cms.vdouble(-0.85, -0.93, -0.91, -0.92),
            Pt1020_Medium = cms.vdouble(-0.85, -0.93, -0.91, -0.92),
            Pt010_Loose = cms.vdouble(-0.95, -0.96, -0.94, -0.95),
            Pt3050_Loose = cms.vdouble(-0.8, -0.73, -0.68, -0.77),
            Pt3050_Tight = cms.vdouble(-0.33, -0.3, -0.15, -0.49)
        ),
        impactParTkThreshold = cms.double(1.0)
    ), 
        cms.PSet(
            tmvaVariables = cms.vstring('frac01', 
                'frac02', 
                'frac03', 
                'frac04', 
                'frac05', 
                'dR2Mean', 
                'nvtx', 
                'nNeutrals', 
                'beta', 
                'betaStar', 
                'dZ', 
                'nCharged'),
            tmvaMethod = cms.string('BDT_fullPlusRMS'),
            cutBased = cms.bool(False),
            tmvaWeights = cms.string('CMGTools/External/data/TMVAClassification_5x_BDT_fullPlusRMS.weights.xml'),
            tmvaSpectators = cms.vstring('jetPt', 
                'jetEta'),
            label = cms.string('full'),
            version = cms.int32(-1),
            JetIdParams = cms.PSet(
                Pt2030_Tight = cms.vdouble(-0.38, -0.32, -0.14, -0.48),
                Pt2030_Loose = cms.vdouble(-0.8, -0.74, -0.68, -0.77),
                Pt3050_Medium = cms.vdouble(-0.4, -0.49, -0.5, -0.65),
                Pt1020_Tight = cms.vdouble(-0.83, -0.81, -0.74, -0.81),
                Pt2030_Medium = cms.vdouble(-0.4, -0.49, -0.5, -0.65),
                Pt010_Tight = cms.vdouble(-0.83, -0.81, -0.74, -0.81),
                Pt1020_Loose = cms.vdouble(-0.95, -0.96, -0.94, -0.95),
                Pt010_Medium = cms.vdouble(-0.83, -0.92, -0.9, -0.92),
                Pt1020_Medium = cms.vdouble(-0.83, -0.92, -0.9, -0.92),
                Pt010_Loose = cms.vdouble(-0.95, -0.96, -0.94, -0.95),
                Pt3050_Loose = cms.vdouble(-0.8, -0.74, -0.68, -0.77),
                Pt3050_Tight = cms.vdouble(-0.38, -0.32, -0.14, -0.48)
            ),
            impactParTkThreshold = cms.double(1.0)
        ), 
        cms.PSet(
            cutBased = cms.bool(True),
            JetIdParams = cms.PSet(
                Pt010_RMSLoose = cms.vdouble(0.08, 0.07, 0.05, 0.07),
                Pt3050_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
                Pt3050_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
                Pt010_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
                Pt1020_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
                Pt010_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
                Pt1020_RMSLoose = cms.vdouble(0.08, 0.07, 0.05, 0.07),
                Pt1020_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
                Pt2030_RMSTight = cms.vdouble(0.05, 0.07, 0.03, 0.045),
                Pt3050_RMSTight = cms.vdouble(0.05, 0.06, 0.03, 0.04),
                Pt1020_RMSTight = cms.vdouble(0.06, 0.07, 0.04, 0.05),
                Pt3050_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
                Pt3050_RMSLoose = cms.vdouble(0.07, 0.06, 0.04, 0.05),
                Pt2030_RMSLoose = cms.vdouble(0.08, 0.07, 0.04, 0.055),
                Pt010_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
                Pt2030_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
                Pt1020_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
                Pt2030_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
                Pt2030_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
                Pt2030_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
                Pt010_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
                Pt3050_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
                Pt1020_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
                Pt010_RMSTight = cms.vdouble(0.06, 0.07, 0.04, 0.05)
            ),
            impactParTkThreshold = cms.double(1.0),
            label = cms.string('cutbased')
        )),
    inputIsCorrected = cms.bool(True),
    vertexes = cms.InputTag("offlinePrimaryVertices"),
    produceJetIds = cms.bool(False),
    jec = cms.string('AK5PF'),
    applyJec = cms.bool(False),
    jetids = cms.InputTag("puJetId"),
    rho = cms.InputTag("kt6PFJets","rho"),
    jets = cms.InputTag("goodPatJets"),
    runMvas = cms.bool(True)
)


process.puJetMvaChs = cms.EDProducer("PileupJetIdProducer",
    algos = cms.VPSet(cms.PSet(
        tmvaVariables = cms.vstring('frac01', 
            'frac02', 
            'frac03', 
            'frac04', 
            'frac05', 
            'nvtx', 
            'beta', 
            'betaStar'),
        tmvaMethod = cms.string('BDT_chsSimpleNoVtxCat'),
        cutBased = cms.bool(False),
        tmvaWeights = cms.string('CMGTools/External/data/TMVAClassification_5x_BDT_chsSimpleNoVtxCat.weights.xml'),
        tmvaSpectators = cms.vstring('jetPt', 
            'jetEta'),
        label = cms.string('simple'),
        version = cms.int32(-1),
        JetIdParams = cms.PSet(
            Pt2030_Tight = cms.vdouble(-0.47, -0.06, -0.23, -0.47),
            Pt2030_Loose = cms.vdouble(-0.89, -0.75, -0.72, -0.75),
            Pt3050_Medium = cms.vdouble(-0.59, -0.65, -0.56, -0.68),
            Pt1020_Tight = cms.vdouble(-0.6, -0.74, -0.78, -0.81),
            Pt2030_Medium = cms.vdouble(-0.59, -0.65, -0.56, -0.68),
            Pt010_Tight = cms.vdouble(-0.6, -0.74, -0.78, -0.81),
            Pt1020_Loose = cms.vdouble(-0.98, -0.96, -0.94, -0.94),
            Pt010_Medium = cms.vdouble(-0.95, -0.94, -0.92, -0.91),
            Pt1020_Medium = cms.vdouble(-0.95, -0.94, -0.92, -0.91),
            Pt010_Loose = cms.vdouble(-0.98, -0.96, -0.94, -0.94),
            Pt3050_Loose = cms.vdouble(-0.89, -0.75, -0.72, -0.75),
            Pt3050_Tight = cms.vdouble(-0.47, -0.06, -0.23, -0.47)
        ),
        impactParTkThreshold = cms.double(1.0)
    ), 
        cms.PSet(
            tmvaVariables = cms.vstring('frac01', 
                'frac02', 
                'frac03', 
                'frac04', 
                'frac05', 
                'dR2Mean', 
                'nvtx', 
                'nNeutrals', 
                'beta', 
                'betaStar', 
                'dZ', 
                'nCharged'),
            tmvaMethod = cms.string('BDT_chsFullPlusRMS'),
            cutBased = cms.bool(False),
            tmvaWeights = cms.string('CMGTools/External/data/TMVAClassification_5x_BDT_chsFullPlusRMS.weights.xml'),
            tmvaSpectators = cms.vstring('jetPt', 
                'jetEta'),
            label = cms.string('full'),
            version = cms.int32(-1),
            JetIdParams = cms.PSet(
                Pt2030_Tight = cms.vdouble(0.41, -0.1, -0.2, -0.45),
                Pt2030_Loose = cms.vdouble(-0.89, -0.77, -0.69, -0.75),
                Pt3050_Medium = cms.vdouble(-0.58, -0.65, -0.57, -0.67),
                Pt1020_Tight = cms.vdouble(-0.59, -0.75, -0.78, -0.8),
                Pt2030_Medium = cms.vdouble(-0.58, -0.65, -0.57, -0.67),
                Pt010_Tight = cms.vdouble(-0.59, -0.75, -0.78, -0.8),
                Pt1020_Loose = cms.vdouble(-0.98, -0.95, -0.94, -0.94),
                Pt010_Medium = cms.vdouble(-0.94, -0.91, -0.91, -0.92),
                Pt1020_Medium = cms.vdouble(-0.94, -0.91, -0.91, -0.92),
                Pt010_Loose = cms.vdouble(-0.98, -0.95, -0.94, -0.94),
                Pt3050_Loose = cms.vdouble(-0.89, -0.77, -0.69, -0.57),
                Pt3050_Tight = cms.vdouble(0.41, -0.1, -0.2, -0.45)
            ),
            impactParTkThreshold = cms.double(1.0)
        ), 
        cms.PSet(
            cutBased = cms.bool(True),
            JetIdParams = cms.PSet(
                Pt010_RMSLoose = cms.vdouble(0.08, 0.07, 0.05, 0.07),
                Pt3050_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
                Pt3050_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
                Pt010_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
                Pt1020_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
                Pt010_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
                Pt1020_RMSLoose = cms.vdouble(0.08, 0.07, 0.05, 0.07),
                Pt1020_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
                Pt2030_RMSTight = cms.vdouble(0.05, 0.07, 0.03, 0.045),
                Pt3050_RMSTight = cms.vdouble(0.05, 0.06, 0.03, 0.04),
                Pt1020_RMSTight = cms.vdouble(0.06, 0.07, 0.04, 0.05),
                Pt3050_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
                Pt3050_RMSLoose = cms.vdouble(0.07, 0.06, 0.04, 0.05),
                Pt2030_RMSLoose = cms.vdouble(0.08, 0.07, 0.04, 0.055),
                Pt010_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
                Pt2030_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
                Pt1020_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
                Pt2030_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
                Pt2030_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
                Pt2030_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
                Pt010_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
                Pt3050_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
                Pt1020_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
                Pt010_RMSTight = cms.vdouble(0.06, 0.07, 0.04, 0.05)
            ),
            impactParTkThreshold = cms.double(1.0),
            label = cms.string('cutbased')
        )),
    inputIsCorrected = cms.bool(True),
    vertexes = cms.InputTag("offlinePrimaryVertices"),
    produceJetIds = cms.bool(False),
    jec = cms.string('AK5PF'),
    applyJec = cms.bool(False),
    jetids = cms.InputTag("puJetIdChs"),
    rho = cms.InputTag("kt6PFJets","rho"),
    jets = cms.InputTag("selectedPatJets"),
    runMvas = cms.bool(True)
)


process.secondaryVertexNegativeTagInfos = cms.EDProducer("SecondaryVertexProducer",
    trackSelection = cms.PSet(
        totalHitsMin = cms.uint32(8),
        jetDeltaRMax = cms.double(0.3),
        qualityClass = cms.string('highPurity'),
        pixelHitsMin = cms.uint32(2),
        sip3dSigMin = cms.double(-99999.9),
        sip3dSigMax = cms.double(99999.9),
        maxDistToAxis = cms.double(0.2),
        sip2dValMax = cms.double(99999.9),
        maxDecayLen = cms.double(99999.9),
        ptMin = cms.double(1.0),
        sip2dSigMax = cms.double(99999.9),
        sip2dSigMin = cms.double(-99999.9),
        sip3dValMax = cms.double(99999.9),
        sip3dValMin = cms.double(-99999.9),
        sip2dValMin = cms.double(-99999.9),
        normChi2Max = cms.double(99999.9)
    ),
    vertexSelection = cms.PSet(
        sortCriterium = cms.string('dist3dError')
    ),
    vertexCuts = cms.PSet(
        distSig3dMax = cms.double(99999.9),
        fracPV = cms.double(0.65),
        distVal2dMax = cms.double(-0.01),
        useTrackWeights = cms.bool(True),
        maxDeltaRToJetAxis = cms.double(-0.5),
        v0Filter = cms.PSet(
            k0sMassWindow = cms.double(0.05)
        ),
        distSig2dMin = cms.double(-99999.9),
        multiplicityMin = cms.uint32(2),
        massMax = cms.double(6.5),
        distSig2dMax = cms.double(-3.0),
        distVal3dMax = cms.double(99999.9),
        minimumTrackWeight = cms.double(0.5),
        distVal3dMin = cms.double(-99999.9),
        distVal2dMin = cms.double(-2.5),
        distSig3dMin = cms.double(-99999.9)
    ),
    vertexReco = cms.PSet(
        seccut = cms.double(6.0),
        primcut = cms.double(1.8),
        smoothing = cms.bool(False),
        weightthreshold = cms.double(0.001),
        minweight = cms.double(0.5),
        finder = cms.string('avr')
    ),
    extSVDeltaRToJet = cms.double(0.3),
    beamSpotTag = cms.InputTag("offlineBeamSpot"),
    constraint = cms.string('BeamSpot'),
    useExternalSV = cms.bool(False),
    trackIPTagInfos = cms.InputTag("impactParameterTagInfos"),
    minimumTrackWeight = cms.double(0.5),
    usePVError = cms.bool(True),
    trackSort = cms.string('sip3dSig'),
    extSVCollection = cms.InputTag("secondaryVertices")
)


process.secondaryVertexTagInfos = cms.EDProducer("SecondaryVertexProducer",
    trackSelection = cms.PSet(
        totalHitsMin = cms.uint32(8),
        jetDeltaRMax = cms.double(0.3),
        qualityClass = cms.string('highPurity'),
        pixelHitsMin = cms.uint32(2),
        sip3dSigMin = cms.double(-99999.9),
        sip3dSigMax = cms.double(99999.9),
        maxDistToAxis = cms.double(0.2),
        sip2dValMax = cms.double(99999.9),
        maxDecayLen = cms.double(99999.9),
        ptMin = cms.double(1.0),
        sip2dSigMax = cms.double(99999.9),
        sip2dSigMin = cms.double(-99999.9),
        sip3dValMax = cms.double(99999.9),
        sip3dValMin = cms.double(-99999.9),
        sip2dValMin = cms.double(-99999.9),
        normChi2Max = cms.double(99999.9)
    ),
    vertexSelection = cms.PSet(
        sortCriterium = cms.string('dist3dError')
    ),
    vertexCuts = cms.PSet(
        distSig3dMax = cms.double(99999.9),
        fracPV = cms.double(0.65),
        distVal2dMax = cms.double(2.5),
        useTrackWeights = cms.bool(True),
        maxDeltaRToJetAxis = cms.double(0.5),
        v0Filter = cms.PSet(
            k0sMassWindow = cms.double(0.05)
        ),
        distSig2dMin = cms.double(3.0),
        multiplicityMin = cms.uint32(2),
        massMax = cms.double(6.5),
        distSig2dMax = cms.double(99999.9),
        distVal3dMax = cms.double(99999.9),
        minimumTrackWeight = cms.double(0.5),
        distVal3dMin = cms.double(-99999.9),
        distVal2dMin = cms.double(0.01),
        distSig3dMin = cms.double(-99999.9)
    ),
    vertexReco = cms.PSet(
        seccut = cms.double(6.0),
        primcut = cms.double(1.8),
        smoothing = cms.bool(False),
        weightthreshold = cms.double(0.001),
        minweight = cms.double(0.5),
        finder = cms.string('avr')
    ),
    extSVDeltaRToJet = cms.double(0.3),
    beamSpotTag = cms.InputTag("offlineBeamSpot"),
    constraint = cms.string('BeamSpot'),
    useExternalSV = cms.bool(False),
    trackIPTagInfos = cms.InputTag("impactParameterTagInfos"),
    minimumTrackWeight = cms.double(0.5),
    usePVError = cms.bool(True),
    trackSort = cms.string('sip3dSig'),
    extSVCollection = cms.InputTag("secondaryVertices")
)


process.simpleSecondaryVertexNegativeBJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('simpleSecondaryVertex'),
    tagInfos = cms.VInputTag(cms.InputTag("secondaryVertexNegativeTagInfos"))
)


process.tauGenJetMatch = cms.EDProducer("GenJetMatcher",
    src = cms.InputTag("hpsPFTauProducer"),
    maxDPtRel = cms.double(3.0),
    mcPdgId = cms.vint32(),
    mcStatus = cms.vint32(),
    resolveByMatchQuality = cms.bool(False),
    maxDeltaR = cms.double(0.1),
    checkCharge = cms.bool(False),
    resolveAmbiguities = cms.bool(True),
    matched = cms.InputTag("tauGenJetsSelectorAllHadrons")
)


process.tauGenJets = cms.EDProducer("TauGenJetProducer",
    includeNeutrinos = cms.bool(False),
    GenParticles = cms.InputTag("genParticles"),
    verbose = cms.untracked.bool(False)
)


process.tauIsoDepositPFCandidates = cms.EDProducer("CandIsoDepositProducer",
    src = cms.InputTag("hpsPFTauProducer"),
    MultipleDepositsFlag = cms.bool(False),
    trackType = cms.string('candidate'),
    ExtractorPSet = cms.PSet(
        Diff_z = cms.double(10000.0),
        ComponentName = cms.string('PFTauExtractor'),
        DR_Max = cms.double(1.0),
        Diff_r = cms.double(10000.0),
        dRvetoPFTauSignalConeConstituents = cms.double(0.01),
        tauSource = cms.InputTag("hpsPFTauProducer"),
        DR_Veto = cms.double(0.0),
        DepositLabel = cms.untracked.string(''),
        candidateSource = cms.InputTag("particleFlow"),
        dRmatchPFTau = cms.double(0.1)
    )
)


process.tauIsoDepositPFChargedHadrons = cms.EDProducer("CandIsoDepositProducer",
    src = cms.InputTag("hpsPFTauProducer"),
    MultipleDepositsFlag = cms.bool(False),
    trackType = cms.string('candidate'),
    ExtractorPSet = cms.PSet(
        Diff_z = cms.double(0.2),
        ComponentName = cms.string('PFTauExtractor'),
        DR_Max = cms.double(1.0),
        Diff_r = cms.double(0.1),
        dRvetoPFTauSignalConeConstituents = cms.double(0.01),
        tauSource = cms.InputTag("hpsPFTauProducer"),
        DR_Veto = cms.double(0.0),
        DepositLabel = cms.untracked.string(''),
        candidateSource = cms.InputTag("pfAllChargedHadrons"),
        dRmatchPFTau = cms.double(0.1)
    )
)


process.tauIsoDepositPFGammas = cms.EDProducer("CandIsoDepositProducer",
    src = cms.InputTag("hpsPFTauProducer"),
    MultipleDepositsFlag = cms.bool(False),
    trackType = cms.string('candidate'),
    ExtractorPSet = cms.PSet(
        Diff_z = cms.double(10000.0),
        ComponentName = cms.string('PFTauExtractor'),
        DR_Max = cms.double(1.0),
        Diff_r = cms.double(10000.0),
        dRvetoPFTauSignalConeConstituents = cms.double(0.01),
        tauSource = cms.InputTag("hpsPFTauProducer"),
        DR_Veto = cms.double(0.0),
        DepositLabel = cms.untracked.string(''),
        candidateSource = cms.InputTag("pfAllPhotons"),
        dRmatchPFTau = cms.double(0.1)
    )
)


process.tauIsoDepositPFNeutralHadrons = cms.EDProducer("CandIsoDepositProducer",
    src = cms.InputTag("hpsPFTauProducer"),
    MultipleDepositsFlag = cms.bool(False),
    trackType = cms.string('candidate'),
    ExtractorPSet = cms.PSet(
        Diff_z = cms.double(10000.0),
        ComponentName = cms.string('PFTauExtractor'),
        DR_Max = cms.double(1.0),
        Diff_r = cms.double(10000.0),
        dRvetoPFTauSignalConeConstituents = cms.double(0.01),
        tauSource = cms.InputTag("hpsPFTauProducer"),
        DR_Veto = cms.double(0.0),
        DepositLabel = cms.untracked.string(''),
        candidateSource = cms.InputTag("pfAllNeutralHadrons"),
        dRmatchPFTau = cms.double(0.1)
    )
)


process.tauMatch = cms.EDProducer("MCMatcher",
    src = cms.InputTag("hpsPFTauProducer"),
    maxDPtRel = cms.double(999.9),
    mcPdgId = cms.vint32(15),
    mcStatus = cms.vint32(2),
    resolveByMatchQuality = cms.bool(False),
    maxDeltaR = cms.double(999.9),
    checkCharge = cms.bool(True),
    resolveAmbiguities = cms.bool(True),
    matched = cms.InputTag("genParticles")
)


process.trackToVertexAssociation = cms.EDProducer("PF_PU_AssoMap",
    VertexAssUseAbsDistance = cms.untracked.bool(False),
    VertexAssClosest = cms.untracked.bool(True),
    NIVertexCollection = cms.InputTag("particleFlowDisplacedVertex"),
    BeamSpot = cms.InputTag("offlineBeamSpot"),
    V0KshortCollection = cms.InputTag("generalV0Candidates","Kshort"),
    VertexAssOneDim = cms.untracked.bool(True),
    V0LambdaCollection = cms.InputTag("generalV0Candidates","Lambda"),
    GsfElectronCollection = cms.InputTag("gsfElectrons"),
    ConversionsCollection = cms.InputTag("allConversions"),
    VertexCollection = cms.InputTag("offlinePrimaryVertices"),
    UseBeamSpotCompatibility = cms.untracked.bool(False),
    ignoreMissingCollection = cms.bool(False),
    TrackCollection = cms.InputTag("generalTracks")
)


process.CSCBasedHaloFilter = cms.EDFilter("CSCHaloFilter",
    ExpectedBX = cms.int32(3),
    MinOuterMomentumTheta = cms.double(0.1),
    FilterCSCTight = cms.bool(False),
    CollisionMuonLabel = cms.InputTag("muons"),
    MatchingDPhiThreshold = cms.double(0.18),
    MinNumberOfOutOfTimeDigis = cms.untracked.int32(1),
    MaxOuterMomentumTheta = cms.double(3.0),
    OuterRMax = cms.double(99999.0),
    ALCTDigiLabel = cms.InputTag("muonCSCDigis","MuonCSCALCTDigi"),
    SACosmicMuonLabel = cms.InputTag("cosmicMuons"),
    NormChi2 = cms.double(8.0),
    FilterDigiLevel = cms.bool(True),
    FilterTriggerLevel = cms.bool(True),
    TrackAssociatorParameters = cms.PSet(
        muonMaxDistanceSigmaX = cms.double(0.0),
        muonMaxDistanceSigmaY = cms.double(0.0),
        CSCSegmentCollectionLabel = cms.InputTag("cscSegments"),
        dRHcal = cms.double(9999.0),
        dRPreshowerPreselection = cms.double(0.2),
        CaloTowerCollectionLabel = cms.InputTag("towerMaker"),
        useEcal = cms.bool(True),
        dREcal = cms.double(9999.0),
        dREcalPreselection = cms.double(0.05),
        HORecHitCollectionLabel = cms.InputTag("horeco"),
        dRMuon = cms.double(9999.0),
        propagateAllDirections = cms.bool(True),
        muonMaxDistanceX = cms.double(5.0),
        muonMaxDistanceY = cms.double(5.0),
        useHO = cms.bool(True),
        trajectoryUncertaintyTolerance = cms.double(-1.0),
        usePreshower = cms.bool(False),
        DTRecSegment4DCollectionLabel = cms.InputTag("dt4DSegments"),
        EERecHitCollectionLabel = cms.InputTag("ecalRecHit","EcalRecHitsEE"),
        dRHcalPreselection = cms.double(0.2),
        useMuon = cms.bool(True),
        useCalo = cms.bool(False),
        accountForTrajectoryChangeCalo = cms.bool(False),
        EBRecHitCollectionLabel = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
        dRMuonPreselection = cms.double(0.2),
        truthMatch = cms.bool(False),
        HBHERecHitCollectionLabel = cms.InputTag("hbhereco"),
        useHcal = cms.bool(True)
    ),
    Deta = cms.double(0.1),
    FilterRecoLevel = cms.bool(True),
    CSCSegmentLabel = cms.InputTag("cscSegments"),
    InnerRMax = cms.double(99999.0),
    MaxDROverDz = cms.double(0.13),
    L1MuGMTReadoutLabel = cms.InputTag("gtDigis"),
    MinNumberOfHaloTracks = cms.untracked.int32(1),
    OuterRMin = cms.double(0.0),
    InnerRMin = cms.double(0.0),
    FilterCSCLoose = cms.bool(False),
    BeamHaloSummaryLabel = cms.InputTag("BeamHaloSummary"),
    MatchingDEtaThreshold = cms.double(0.4),
    MatchingDWireThreshold = cms.int32(5),
    MinNumberOfHaloTriggers = cms.untracked.int32(1),
    Dphi = cms.double(1.0),
    CSCRecHitLabel = cms.InputTag("csc2DRecHits"),
    CSCHaloDataLabel = cms.InputTag("CSCHaloData")
)


process.CSCHaloFilterDigiAndRecoLevel = cms.EDFilter("CSCHaloFilter",
    ExpectedBX = cms.int32(3),
    MinOuterMomentumTheta = cms.double(0.1),
    MaxDROverDz = cms.double(0.13),
    CollisionMuonLabel = cms.InputTag("muons"),
    MatchingDPhiThreshold = cms.double(0.18),
    MinNumberOfOutOfTimeDigis = cms.untracked.int32(1),
    MaxOuterMomentumTheta = cms.double(3.0),
    OuterRMax = cms.double(99999.0),
    ALCTDigiLabel = cms.InputTag("muonCSCDigis","MuonCSCALCTDigi"),
    SACosmicMuonLabel = cms.InputTag("cosmicMuons"),
    NormChi2 = cms.double(8.0),
    FilterDigiLevel = cms.bool(True),
    FilterTriggerLevel = cms.bool(False),
    TrackAssociatorParameters = cms.PSet(
        muonMaxDistanceSigmaX = cms.double(0.0),
        muonMaxDistanceSigmaY = cms.double(0.0),
        CSCSegmentCollectionLabel = cms.InputTag("cscSegments"),
        dRHcal = cms.double(9999.0),
        dRPreshowerPreselection = cms.double(0.2),
        CaloTowerCollectionLabel = cms.InputTag("towerMaker"),
        useEcal = cms.bool(True),
        dREcal = cms.double(9999.0),
        dREcalPreselection = cms.double(0.05),
        HORecHitCollectionLabel = cms.InputTag("horeco"),
        dRMuon = cms.double(9999.0),
        propagateAllDirections = cms.bool(True),
        muonMaxDistanceX = cms.double(5.0),
        muonMaxDistanceY = cms.double(5.0),
        useHO = cms.bool(True),
        trajectoryUncertaintyTolerance = cms.double(-1.0),
        usePreshower = cms.bool(False),
        DTRecSegment4DCollectionLabel = cms.InputTag("dt4DSegments"),
        EERecHitCollectionLabel = cms.InputTag("ecalRecHit","EcalRecHitsEE"),
        dRHcalPreselection = cms.double(0.2),
        useMuon = cms.bool(True),
        useCalo = cms.bool(False),
        accountForTrajectoryChangeCalo = cms.bool(False),
        EBRecHitCollectionLabel = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
        dRMuonPreselection = cms.double(0.2),
        truthMatch = cms.bool(False),
        HBHERecHitCollectionLabel = cms.InputTag("hbhereco"),
        useHcal = cms.bool(True)
    ),
    Deta = cms.double(0.1),
    FilterRecoLevel = cms.bool(True),
    CSCSegmentLabel = cms.InputTag("cscSegments"),
    InnerRMax = cms.double(99999.0),
    FilterCSCTight = cms.bool(False),
    L1MuGMTReadoutLabel = cms.InputTag("gtDigis"),
    MinNumberOfHaloTracks = cms.untracked.int32(1),
    OuterRMin = cms.double(0.0),
    InnerRMin = cms.double(0.0),
    FilterCSCLoose = cms.bool(False),
    BeamHaloSummaryLabel = cms.InputTag("BeamHaloSummary"),
    MatchingDEtaThreshold = cms.double(0.4),
    MatchingDWireThreshold = cms.int32(5),
    MinNumberOfHaloTriggers = cms.untracked.int32(1),
    Dphi = cms.double(1.0),
    CSCRecHitLabel = cms.InputTag("csc2DRecHits"),
    CSCHaloDataLabel = cms.InputTag("CSCHaloData")
)


process.CSCHaloFilterDigiAndTriggerLevel = cms.EDFilter("CSCHaloFilter",
    ExpectedBX = cms.int32(3),
    MinOuterMomentumTheta = cms.double(0.1),
    MaxDROverDz = cms.double(0.13),
    CollisionMuonLabel = cms.InputTag("muons"),
    MatchingDPhiThreshold = cms.double(0.18),
    MinNumberOfOutOfTimeDigis = cms.untracked.int32(1),
    MaxOuterMomentumTheta = cms.double(3.0),
    OuterRMax = cms.double(99999.0),
    ALCTDigiLabel = cms.InputTag("muonCSCDigis","MuonCSCALCTDigi"),
    SACosmicMuonLabel = cms.InputTag("cosmicMuons"),
    NormChi2 = cms.double(8.0),
    FilterDigiLevel = cms.bool(True),
    FilterTriggerLevel = cms.bool(True),
    TrackAssociatorParameters = cms.PSet(
        muonMaxDistanceSigmaX = cms.double(0.0),
        muonMaxDistanceSigmaY = cms.double(0.0),
        CSCSegmentCollectionLabel = cms.InputTag("cscSegments"),
        dRHcal = cms.double(9999.0),
        dRPreshowerPreselection = cms.double(0.2),
        CaloTowerCollectionLabel = cms.InputTag("towerMaker"),
        useEcal = cms.bool(True),
        dREcal = cms.double(9999.0),
        dREcalPreselection = cms.double(0.05),
        HORecHitCollectionLabel = cms.InputTag("horeco"),
        dRMuon = cms.double(9999.0),
        propagateAllDirections = cms.bool(True),
        muonMaxDistanceX = cms.double(5.0),
        muonMaxDistanceY = cms.double(5.0),
        useHO = cms.bool(True),
        trajectoryUncertaintyTolerance = cms.double(-1.0),
        usePreshower = cms.bool(False),
        DTRecSegment4DCollectionLabel = cms.InputTag("dt4DSegments"),
        EERecHitCollectionLabel = cms.InputTag("ecalRecHit","EcalRecHitsEE"),
        dRHcalPreselection = cms.double(0.2),
        useMuon = cms.bool(True),
        useCalo = cms.bool(False),
        accountForTrajectoryChangeCalo = cms.bool(False),
        EBRecHitCollectionLabel = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
        dRMuonPreselection = cms.double(0.2),
        truthMatch = cms.bool(False),
        HBHERecHitCollectionLabel = cms.InputTag("hbhereco"),
        useHcal = cms.bool(True)
    ),
    Deta = cms.double(0.1),
    FilterRecoLevel = cms.bool(False),
    CSCSegmentLabel = cms.InputTag("cscSegments"),
    InnerRMax = cms.double(99999.0),
    FilterCSCTight = cms.bool(False),
    L1MuGMTReadoutLabel = cms.InputTag("gtDigis"),
    MinNumberOfHaloTracks = cms.untracked.int32(1),
    OuterRMin = cms.double(0.0),
    InnerRMin = cms.double(0.0),
    FilterCSCLoose = cms.bool(False),
    BeamHaloSummaryLabel = cms.InputTag("BeamHaloSummary"),
    MatchingDEtaThreshold = cms.double(0.4),
    MatchingDWireThreshold = cms.int32(5),
    MinNumberOfHaloTriggers = cms.untracked.int32(1),
    Dphi = cms.double(1.0),
    CSCRecHitLabel = cms.InputTag("csc2DRecHits"),
    CSCHaloDataLabel = cms.InputTag("CSCHaloData")
)


process.CSCHaloFilterDigiLevel = cms.EDFilter("CSCHaloFilter",
    ExpectedBX = cms.int32(3),
    MinOuterMomentumTheta = cms.double(0.1),
    MaxDROverDz = cms.double(0.13),
    CollisionMuonLabel = cms.InputTag("muons"),
    MatchingDPhiThreshold = cms.double(0.18),
    MinNumberOfOutOfTimeDigis = cms.untracked.int32(1),
    MaxOuterMomentumTheta = cms.double(3.0),
    OuterRMax = cms.double(99999.0),
    ALCTDigiLabel = cms.InputTag("muonCSCDigis","MuonCSCALCTDigi"),
    SACosmicMuonLabel = cms.InputTag("cosmicMuons"),
    NormChi2 = cms.double(8.0),
    FilterDigiLevel = cms.bool(True),
    FilterTriggerLevel = cms.bool(False),
    TrackAssociatorParameters = cms.PSet(
        muonMaxDistanceSigmaX = cms.double(0.0),
        muonMaxDistanceSigmaY = cms.double(0.0),
        CSCSegmentCollectionLabel = cms.InputTag("cscSegments"),
        dRHcal = cms.double(9999.0),
        dRPreshowerPreselection = cms.double(0.2),
        CaloTowerCollectionLabel = cms.InputTag("towerMaker"),
        useEcal = cms.bool(True),
        dREcal = cms.double(9999.0),
        dREcalPreselection = cms.double(0.05),
        HORecHitCollectionLabel = cms.InputTag("horeco"),
        dRMuon = cms.double(9999.0),
        propagateAllDirections = cms.bool(True),
        muonMaxDistanceX = cms.double(5.0),
        muonMaxDistanceY = cms.double(5.0),
        useHO = cms.bool(True),
        trajectoryUncertaintyTolerance = cms.double(-1.0),
        usePreshower = cms.bool(False),
        DTRecSegment4DCollectionLabel = cms.InputTag("dt4DSegments"),
        EERecHitCollectionLabel = cms.InputTag("ecalRecHit","EcalRecHitsEE"),
        dRHcalPreselection = cms.double(0.2),
        useMuon = cms.bool(True),
        useCalo = cms.bool(False),
        accountForTrajectoryChangeCalo = cms.bool(False),
        EBRecHitCollectionLabel = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
        dRMuonPreselection = cms.double(0.2),
        truthMatch = cms.bool(False),
        HBHERecHitCollectionLabel = cms.InputTag("hbhereco"),
        useHcal = cms.bool(True)
    ),
    Deta = cms.double(0.1),
    FilterRecoLevel = cms.bool(False),
    CSCSegmentLabel = cms.InputTag("cscSegments"),
    InnerRMax = cms.double(99999.0),
    FilterCSCTight = cms.bool(False),
    L1MuGMTReadoutLabel = cms.InputTag("gtDigis"),
    MinNumberOfHaloTracks = cms.untracked.int32(1),
    OuterRMin = cms.double(0.0),
    InnerRMin = cms.double(0.0),
    FilterCSCLoose = cms.bool(False),
    BeamHaloSummaryLabel = cms.InputTag("BeamHaloSummary"),
    MatchingDEtaThreshold = cms.double(0.4),
    MatchingDWireThreshold = cms.int32(5),
    MinNumberOfHaloTriggers = cms.untracked.int32(1),
    Dphi = cms.double(1.0),
    CSCRecHitLabel = cms.InputTag("csc2DRecHits"),
    CSCHaloDataLabel = cms.InputTag("CSCHaloData")
)


process.CSCHaloFilterRecoAndDigiAndTriggerLevel = cms.EDFilter("CSCHaloFilter",
    ExpectedBX = cms.int32(3),
    MinOuterMomentumTheta = cms.double(0.1),
    MaxDROverDz = cms.double(0.13),
    CollisionMuonLabel = cms.InputTag("muons"),
    MatchingDPhiThreshold = cms.double(0.18),
    MinNumberOfOutOfTimeDigis = cms.untracked.int32(1),
    MaxOuterMomentumTheta = cms.double(3.0),
    OuterRMax = cms.double(99999.0),
    ALCTDigiLabel = cms.InputTag("muonCSCDigis","MuonCSCALCTDigi"),
    SACosmicMuonLabel = cms.InputTag("cosmicMuons"),
    NormChi2 = cms.double(8.0),
    FilterDigiLevel = cms.bool(True),
    FilterTriggerLevel = cms.bool(True),
    TrackAssociatorParameters = cms.PSet(
        muonMaxDistanceSigmaX = cms.double(0.0),
        muonMaxDistanceSigmaY = cms.double(0.0),
        CSCSegmentCollectionLabel = cms.InputTag("cscSegments"),
        dRHcal = cms.double(9999.0),
        dRPreshowerPreselection = cms.double(0.2),
        CaloTowerCollectionLabel = cms.InputTag("towerMaker"),
        useEcal = cms.bool(True),
        dREcal = cms.double(9999.0),
        dREcalPreselection = cms.double(0.05),
        HORecHitCollectionLabel = cms.InputTag("horeco"),
        dRMuon = cms.double(9999.0),
        propagateAllDirections = cms.bool(True),
        muonMaxDistanceX = cms.double(5.0),
        muonMaxDistanceY = cms.double(5.0),
        useHO = cms.bool(True),
        trajectoryUncertaintyTolerance = cms.double(-1.0),
        usePreshower = cms.bool(False),
        DTRecSegment4DCollectionLabel = cms.InputTag("dt4DSegments"),
        EERecHitCollectionLabel = cms.InputTag("ecalRecHit","EcalRecHitsEE"),
        dRHcalPreselection = cms.double(0.2),
        useMuon = cms.bool(True),
        useCalo = cms.bool(False),
        accountForTrajectoryChangeCalo = cms.bool(False),
        EBRecHitCollectionLabel = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
        dRMuonPreselection = cms.double(0.2),
        truthMatch = cms.bool(False),
        HBHERecHitCollectionLabel = cms.InputTag("hbhereco"),
        useHcal = cms.bool(True)
    ),
    Deta = cms.double(0.1),
    FilterRecoLevel = cms.bool(True),
    CSCSegmentLabel = cms.InputTag("cscSegments"),
    InnerRMax = cms.double(99999.0),
    FilterCSCTight = cms.bool(False),
    L1MuGMTReadoutLabel = cms.InputTag("gtDigis"),
    MinNumberOfHaloTracks = cms.untracked.int32(1),
    OuterRMin = cms.double(0.0),
    InnerRMin = cms.double(0.0),
    FilterCSCLoose = cms.bool(False),
    BeamHaloSummaryLabel = cms.InputTag("BeamHaloSummary"),
    MatchingDEtaThreshold = cms.double(0.4),
    MatchingDWireThreshold = cms.int32(5),
    MinNumberOfHaloTriggers = cms.untracked.int32(1),
    Dphi = cms.double(1.0),
    CSCRecHitLabel = cms.InputTag("csc2DRecHits"),
    CSCHaloDataLabel = cms.InputTag("CSCHaloData")
)


process.CSCHaloFilterRecoAndTriggerLevel = cms.EDFilter("CSCHaloFilter",
    ExpectedBX = cms.int32(3),
    MinOuterMomentumTheta = cms.double(0.1),
    MaxDROverDz = cms.double(0.13),
    CollisionMuonLabel = cms.InputTag("muons"),
    MatchingDPhiThreshold = cms.double(0.18),
    MinNumberOfOutOfTimeDigis = cms.untracked.int32(1),
    MaxOuterMomentumTheta = cms.double(3.0),
    OuterRMax = cms.double(99999.0),
    ALCTDigiLabel = cms.InputTag("muonCSCDigis","MuonCSCALCTDigi"),
    SACosmicMuonLabel = cms.InputTag("cosmicMuons"),
    NormChi2 = cms.double(8.0),
    FilterDigiLevel = cms.bool(False),
    FilterTriggerLevel = cms.bool(True),
    TrackAssociatorParameters = cms.PSet(
        muonMaxDistanceSigmaX = cms.double(0.0),
        muonMaxDistanceSigmaY = cms.double(0.0),
        CSCSegmentCollectionLabel = cms.InputTag("cscSegments"),
        dRHcal = cms.double(9999.0),
        dRPreshowerPreselection = cms.double(0.2),
        CaloTowerCollectionLabel = cms.InputTag("towerMaker"),
        useEcal = cms.bool(True),
        dREcal = cms.double(9999.0),
        dREcalPreselection = cms.double(0.05),
        HORecHitCollectionLabel = cms.InputTag("horeco"),
        dRMuon = cms.double(9999.0),
        propagateAllDirections = cms.bool(True),
        muonMaxDistanceX = cms.double(5.0),
        muonMaxDistanceY = cms.double(5.0),
        useHO = cms.bool(True),
        trajectoryUncertaintyTolerance = cms.double(-1.0),
        usePreshower = cms.bool(False),
        DTRecSegment4DCollectionLabel = cms.InputTag("dt4DSegments"),
        EERecHitCollectionLabel = cms.InputTag("ecalRecHit","EcalRecHitsEE"),
        dRHcalPreselection = cms.double(0.2),
        useMuon = cms.bool(True),
        useCalo = cms.bool(False),
        accountForTrajectoryChangeCalo = cms.bool(False),
        EBRecHitCollectionLabel = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
        dRMuonPreselection = cms.double(0.2),
        truthMatch = cms.bool(False),
        HBHERecHitCollectionLabel = cms.InputTag("hbhereco"),
        useHcal = cms.bool(True)
    ),
    Deta = cms.double(0.1),
    FilterRecoLevel = cms.bool(True),
    CSCSegmentLabel = cms.InputTag("cscSegments"),
    InnerRMax = cms.double(99999.0),
    FilterCSCTight = cms.bool(False),
    L1MuGMTReadoutLabel = cms.InputTag("gtDigis"),
    MinNumberOfHaloTracks = cms.untracked.int32(1),
    OuterRMin = cms.double(0.0),
    InnerRMin = cms.double(0.0),
    FilterCSCLoose = cms.bool(False),
    BeamHaloSummaryLabel = cms.InputTag("BeamHaloSummary"),
    MatchingDEtaThreshold = cms.double(0.4),
    MatchingDWireThreshold = cms.int32(5),
    MinNumberOfHaloTriggers = cms.untracked.int32(1),
    Dphi = cms.double(1.0),
    CSCRecHitLabel = cms.InputTag("csc2DRecHits"),
    CSCHaloDataLabel = cms.InputTag("CSCHaloData")
)


process.CSCHaloFilterRecoLevel = cms.EDFilter("CSCHaloFilter",
    ExpectedBX = cms.int32(3),
    MinOuterMomentumTheta = cms.double(0.1),
    MaxDROverDz = cms.double(0.13),
    CollisionMuonLabel = cms.InputTag("muons"),
    MatchingDPhiThreshold = cms.double(0.18),
    MinNumberOfOutOfTimeDigis = cms.untracked.int32(1),
    MaxOuterMomentumTheta = cms.double(3.0),
    OuterRMax = cms.double(99999.0),
    ALCTDigiLabel = cms.InputTag("muonCSCDigis","MuonCSCALCTDigi"),
    SACosmicMuonLabel = cms.InputTag("cosmicMuons"),
    NormChi2 = cms.double(8.0),
    FilterDigiLevel = cms.bool(False),
    FilterTriggerLevel = cms.bool(False),
    TrackAssociatorParameters = cms.PSet(
        muonMaxDistanceSigmaX = cms.double(0.0),
        muonMaxDistanceSigmaY = cms.double(0.0),
        CSCSegmentCollectionLabel = cms.InputTag("cscSegments"),
        dRHcal = cms.double(9999.0),
        dRPreshowerPreselection = cms.double(0.2),
        CaloTowerCollectionLabel = cms.InputTag("towerMaker"),
        useEcal = cms.bool(True),
        dREcal = cms.double(9999.0),
        dREcalPreselection = cms.double(0.05),
        HORecHitCollectionLabel = cms.InputTag("horeco"),
        dRMuon = cms.double(9999.0),
        propagateAllDirections = cms.bool(True),
        muonMaxDistanceX = cms.double(5.0),
        muonMaxDistanceY = cms.double(5.0),
        useHO = cms.bool(True),
        trajectoryUncertaintyTolerance = cms.double(-1.0),
        usePreshower = cms.bool(False),
        DTRecSegment4DCollectionLabel = cms.InputTag("dt4DSegments"),
        EERecHitCollectionLabel = cms.InputTag("ecalRecHit","EcalRecHitsEE"),
        dRHcalPreselection = cms.double(0.2),
        useMuon = cms.bool(True),
        useCalo = cms.bool(False),
        accountForTrajectoryChangeCalo = cms.bool(False),
        EBRecHitCollectionLabel = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
        dRMuonPreselection = cms.double(0.2),
        truthMatch = cms.bool(False),
        HBHERecHitCollectionLabel = cms.InputTag("hbhereco"),
        useHcal = cms.bool(True)
    ),
    Deta = cms.double(0.1),
    FilterRecoLevel = cms.bool(True),
    CSCSegmentLabel = cms.InputTag("cscSegments"),
    InnerRMax = cms.double(99999.0),
    FilterCSCTight = cms.bool(False),
    L1MuGMTReadoutLabel = cms.InputTag("gtDigis"),
    MinNumberOfHaloTracks = cms.untracked.int32(1),
    OuterRMin = cms.double(0.0),
    InnerRMin = cms.double(0.0),
    FilterCSCLoose = cms.bool(False),
    BeamHaloSummaryLabel = cms.InputTag("BeamHaloSummary"),
    MatchingDEtaThreshold = cms.double(0.4),
    MatchingDWireThreshold = cms.int32(5),
    MinNumberOfHaloTriggers = cms.untracked.int32(1),
    Dphi = cms.double(1.0),
    CSCRecHitLabel = cms.InputTag("csc2DRecHits"),
    CSCHaloDataLabel = cms.InputTag("CSCHaloData")
)


process.CSCHaloFilterTriggerLevel = cms.EDFilter("CSCHaloFilter",
    ExpectedBX = cms.int32(3),
    MinOuterMomentumTheta = cms.double(0.1),
    MaxDROverDz = cms.double(0.13),
    CollisionMuonLabel = cms.InputTag("muons"),
    MatchingDPhiThreshold = cms.double(0.18),
    MinNumberOfOutOfTimeDigis = cms.untracked.int32(1),
    MaxOuterMomentumTheta = cms.double(3.0),
    OuterRMax = cms.double(99999.0),
    ALCTDigiLabel = cms.InputTag("muonCSCDigis","MuonCSCALCTDigi"),
    SACosmicMuonLabel = cms.InputTag("cosmicMuons"),
    NormChi2 = cms.double(8.0),
    FilterDigiLevel = cms.bool(False),
    FilterTriggerLevel = cms.bool(True),
    TrackAssociatorParameters = cms.PSet(
        muonMaxDistanceSigmaX = cms.double(0.0),
        muonMaxDistanceSigmaY = cms.double(0.0),
        CSCSegmentCollectionLabel = cms.InputTag("cscSegments"),
        dRHcal = cms.double(9999.0),
        dRPreshowerPreselection = cms.double(0.2),
        CaloTowerCollectionLabel = cms.InputTag("towerMaker"),
        useEcal = cms.bool(True),
        dREcal = cms.double(9999.0),
        dREcalPreselection = cms.double(0.05),
        HORecHitCollectionLabel = cms.InputTag("horeco"),
        dRMuon = cms.double(9999.0),
        propagateAllDirections = cms.bool(True),
        muonMaxDistanceX = cms.double(5.0),
        muonMaxDistanceY = cms.double(5.0),
        useHO = cms.bool(True),
        trajectoryUncertaintyTolerance = cms.double(-1.0),
        usePreshower = cms.bool(False),
        DTRecSegment4DCollectionLabel = cms.InputTag("dt4DSegments"),
        EERecHitCollectionLabel = cms.InputTag("ecalRecHit","EcalRecHitsEE"),
        dRHcalPreselection = cms.double(0.2),
        useMuon = cms.bool(True),
        useCalo = cms.bool(False),
        accountForTrajectoryChangeCalo = cms.bool(False),
        EBRecHitCollectionLabel = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
        dRMuonPreselection = cms.double(0.2),
        truthMatch = cms.bool(False),
        HBHERecHitCollectionLabel = cms.InputTag("hbhereco"),
        useHcal = cms.bool(True)
    ),
    Deta = cms.double(0.1),
    FilterRecoLevel = cms.bool(False),
    CSCSegmentLabel = cms.InputTag("cscSegments"),
    InnerRMax = cms.double(99999.0),
    FilterCSCTight = cms.bool(False),
    L1MuGMTReadoutLabel = cms.InputTag("gtDigis"),
    MinNumberOfHaloTracks = cms.untracked.int32(1),
    OuterRMin = cms.double(0.0),
    InnerRMin = cms.double(0.0),
    FilterCSCLoose = cms.bool(False),
    BeamHaloSummaryLabel = cms.InputTag("BeamHaloSummary"),
    MatchingDEtaThreshold = cms.double(0.4),
    MatchingDWireThreshold = cms.int32(5),
    MinNumberOfHaloTriggers = cms.untracked.int32(1),
    Dphi = cms.double(1.0),
    CSCRecHitLabel = cms.InputTag("csc2DRecHits"),
    CSCHaloDataLabel = cms.InputTag("CSCHaloData")
)


process.CSCLooseHaloFilter = cms.EDFilter("CSCHaloFilter",
    ExpectedBX = cms.int32(3),
    MinOuterMomentumTheta = cms.double(0.1),
    MaxDROverDz = cms.double(0.13),
    CollisionMuonLabel = cms.InputTag("muons"),
    MatchingDPhiThreshold = cms.double(0.18),
    MinNumberOfOutOfTimeDigis = cms.untracked.int32(1),
    MaxOuterMomentumTheta = cms.double(3.0),
    OuterRMax = cms.double(99999.0),
    ALCTDigiLabel = cms.InputTag("muonCSCDigis","MuonCSCALCTDigi"),
    SACosmicMuonLabel = cms.InputTag("cosmicMuons"),
    NormChi2 = cms.double(8.0),
    FilterDigiLevel = cms.bool(True),
    FilterTriggerLevel = cms.bool(True),
    TrackAssociatorParameters = cms.PSet(
        muonMaxDistanceSigmaX = cms.double(0.0),
        muonMaxDistanceSigmaY = cms.double(0.0),
        CSCSegmentCollectionLabel = cms.InputTag("cscSegments"),
        dRHcal = cms.double(9999.0),
        dRPreshowerPreselection = cms.double(0.2),
        CaloTowerCollectionLabel = cms.InputTag("towerMaker"),
        useEcal = cms.bool(True),
        dREcal = cms.double(9999.0),
        dREcalPreselection = cms.double(0.05),
        HORecHitCollectionLabel = cms.InputTag("horeco"),
        dRMuon = cms.double(9999.0),
        propagateAllDirections = cms.bool(True),
        muonMaxDistanceX = cms.double(5.0),
        muonMaxDistanceY = cms.double(5.0),
        useHO = cms.bool(True),
        trajectoryUncertaintyTolerance = cms.double(-1.0),
        usePreshower = cms.bool(False),
        DTRecSegment4DCollectionLabel = cms.InputTag("dt4DSegments"),
        EERecHitCollectionLabel = cms.InputTag("ecalRecHit","EcalRecHitsEE"),
        dRHcalPreselection = cms.double(0.2),
        useMuon = cms.bool(True),
        useCalo = cms.bool(False),
        accountForTrajectoryChangeCalo = cms.bool(False),
        EBRecHitCollectionLabel = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
        dRMuonPreselection = cms.double(0.2),
        truthMatch = cms.bool(False),
        HBHERecHitCollectionLabel = cms.InputTag("hbhereco"),
        useHcal = cms.bool(True)
    ),
    Deta = cms.double(0.1),
    FilterRecoLevel = cms.bool(True),
    CSCSegmentLabel = cms.InputTag("cscSegments"),
    InnerRMax = cms.double(99999.0),
    FilterCSCTight = cms.bool(False),
    L1MuGMTReadoutLabel = cms.InputTag("gtDigis"),
    MinNumberOfHaloTracks = cms.untracked.int32(1),
    OuterRMin = cms.double(0.0),
    InnerRMin = cms.double(0.0),
    FilterCSCLoose = cms.bool(True),
    BeamHaloSummaryLabel = cms.InputTag("BeamHaloSummary"),
    MatchingDEtaThreshold = cms.double(0.4),
    MatchingDWireThreshold = cms.int32(5),
    MinNumberOfHaloTriggers = cms.untracked.int32(1),
    Dphi = cms.double(1.0),
    CSCRecHitLabel = cms.InputTag("csc2DRecHits"),
    CSCHaloDataLabel = cms.InputTag("CSCHaloData")
)


process.CSCTightHaloFilter = cms.EDFilter("CSCHaloFilter",
    ExpectedBX = cms.int32(3),
    MinOuterMomentumTheta = cms.double(0.1),
    MaxDROverDz = cms.double(0.13),
    CollisionMuonLabel = cms.InputTag("muons"),
    MatchingDPhiThreshold = cms.double(0.18),
    MinNumberOfOutOfTimeDigis = cms.untracked.int32(1),
    MaxOuterMomentumTheta = cms.double(3.0),
    OuterRMax = cms.double(99999.0),
    ALCTDigiLabel = cms.InputTag("muonCSCDigis","MuonCSCALCTDigi"),
    SACosmicMuonLabel = cms.InputTag("cosmicMuons"),
    NormChi2 = cms.double(8.0),
    FilterDigiLevel = cms.bool(True),
    FilterTriggerLevel = cms.bool(True),
    TrackAssociatorParameters = cms.PSet(
        muonMaxDistanceSigmaX = cms.double(0.0),
        muonMaxDistanceSigmaY = cms.double(0.0),
        CSCSegmentCollectionLabel = cms.InputTag("cscSegments"),
        dRHcal = cms.double(9999.0),
        dRPreshowerPreselection = cms.double(0.2),
        CaloTowerCollectionLabel = cms.InputTag("towerMaker"),
        useEcal = cms.bool(True),
        dREcal = cms.double(9999.0),
        dREcalPreselection = cms.double(0.05),
        HORecHitCollectionLabel = cms.InputTag("horeco"),
        dRMuon = cms.double(9999.0),
        propagateAllDirections = cms.bool(True),
        muonMaxDistanceX = cms.double(5.0),
        muonMaxDistanceY = cms.double(5.0),
        useHO = cms.bool(True),
        trajectoryUncertaintyTolerance = cms.double(-1.0),
        usePreshower = cms.bool(False),
        DTRecSegment4DCollectionLabel = cms.InputTag("dt4DSegments"),
        EERecHitCollectionLabel = cms.InputTag("ecalRecHit","EcalRecHitsEE"),
        dRHcalPreselection = cms.double(0.2),
        useMuon = cms.bool(True),
        useCalo = cms.bool(False),
        accountForTrajectoryChangeCalo = cms.bool(False),
        EBRecHitCollectionLabel = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
        dRMuonPreselection = cms.double(0.2),
        truthMatch = cms.bool(False),
        HBHERecHitCollectionLabel = cms.InputTag("hbhereco"),
        useHcal = cms.bool(True)
    ),
    Deta = cms.double(0.1),
    FilterRecoLevel = cms.bool(True),
    CSCSegmentLabel = cms.InputTag("cscSegments"),
    InnerRMax = cms.double(99999.0),
    FilterCSCTight = cms.bool(True),
    L1MuGMTReadoutLabel = cms.InputTag("gtDigis"),
    MinNumberOfHaloTracks = cms.untracked.int32(1),
    OuterRMin = cms.double(0.0),
    InnerRMin = cms.double(0.0),
    FilterCSCLoose = cms.bool(False),
    BeamHaloSummaryLabel = cms.InputTag("BeamHaloSummary"),
    MatchingDEtaThreshold = cms.double(0.4),
    MatchingDWireThreshold = cms.int32(5),
    MinNumberOfHaloTriggers = cms.untracked.int32(1),
    Dphi = cms.double(1.0),
    CSCRecHitLabel = cms.InputTag("csc2DRecHits"),
    CSCHaloDataLabel = cms.InputTag("CSCHaloData")
)


process.EcalDeadCellTriggerPrimitiveFilter = cms.EDFilter("EcalDeadCellTriggerPrimitiveFilter",
    maskedEcalChannelStatusThreshold = cms.int32(1),
    ebReducedRecHitCollection = cms.InputTag("reducedEcalRecHitsEB"),
    etValToBeFlagged = cms.double(63.75),
    eeReducedRecHitCollection = cms.InputTag("reducedEcalRecHitsEE"),
    profileRootName = cms.untracked.string('deadCellFilterProfile.root'),
    doEEfilter = cms.untracked.bool(True),
    makeProfileRoot = cms.untracked.bool(False),
    taggingMode = cms.bool(False),
    debug = cms.bool(False),
    tpDigiCollection = cms.InputTag("ecalTPSkimNA"),
    verbose = cms.int32(1)
)


process.HBHENoiseFilter = cms.EDFilter("HBHENoiseFilter",
    IgnoreTS4TS5ifJetInLowBVRegion = cms.bool(False),
    jetlabel = cms.InputTag("ak5PFJets"),
    maxNHF = cms.double(0.9),
    minHPDHits = cms.int32(17),
    maxjetindex = cms.int32(0),
    minIsolatedNoiseSumE = cms.double(50.0),
    minHighEHitTime = cms.double(-9999.0),
    minHPDNoOtherHits = cms.int32(10),
    useTS4TS5 = cms.bool(True),
    noiselabel = cms.InputTag("hcalnoise"),
    minZeros = cms.int32(10),
    minNumIsolatedNoiseChannels = cms.int32(10),
    maxRatio = cms.double(999.0),
    maxHighEHitTime = cms.double(9999.0),
    maxRBXEMF = cms.double(-999.0),
    minRBXHits = cms.int32(999),
    minIsolatedNoiseSumEt = cms.double(25.0),
    minRatio = cms.double(-999.0)
)


process.countPatElectrons = cms.EDFilter("PATCandViewCountFilter",
    maxNumber = cms.uint32(999999),
    src = cms.InputTag("cleanPatElectrons"),
    minNumber = cms.uint32(0)
)


process.countPatJets = cms.EDFilter("PATCandViewCountFilter",
    maxNumber = cms.uint32(999999),
    src = cms.InputTag("cleanPatJets"),
    minNumber = cms.uint32(0)
)


process.countPatLeptons = cms.EDFilter("PATLeptonCountFilter",
    maxNumber = cms.uint32(999999),
    countElectrons = cms.bool(True),
    muonSource = cms.InputTag("cleanPatMuons"),
    minNumber = cms.uint32(0),
    electronSource = cms.InputTag("cleanPatElectrons"),
    tauSource = cms.InputTag("cleanPatTaus"),
    countTaus = cms.bool(False),
    countMuons = cms.bool(True)
)


process.countPatMuons = cms.EDFilter("PATCandViewCountFilter",
    maxNumber = cms.uint32(999999),
    src = cms.InputTag("cleanPatMuons"),
    minNumber = cms.uint32(0)
)


process.countPatPhotons = cms.EDFilter("PATCandViewCountFilter",
    maxNumber = cms.uint32(999999),
    src = cms.InputTag("cleanPatPhotons"),
    minNumber = cms.uint32(0)
)


process.countPatTaus = cms.EDFilter("PATCandViewCountFilter",
    maxNumber = cms.uint32(999999),
    src = cms.InputTag("cleanPatTaus"),
    minNumber = cms.uint32(0)
)


process.ecalLaserCorrFilter = cms.EDFilter("EcalLaserCorrFilter",
    EBEnegyMIN = cms.double(10.0),
    EBLaserMAX = cms.double(3.0),
    EELaserMIN = cms.double(0.3),
    EELaserMAX = cms.double(8.0),
    EEEnegyMIN = cms.double(10.0),
    EBRecHitSource = cms.InputTag("reducedEcalRecHitsEB"),
    EBLaserMIN = cms.double(0.3),
    EERecHitSource = cms.InputTag("reducedEcalRecHitsEE"),
    taggingMode = cms.bool(False),
    Debug = cms.bool(False)
)


process.eeBadScFilter = cms.EDFilter("EEBadScFilter",
    SCsize = cms.int32(5),
    badscEE = cms.vint32(-1023023, 1048098, -1078063),
    EtminSC = cms.double(1000.0),
    EminHit = cms.double(1000.0),
    EERecHitSource = cms.InputTag("reducedEcalRecHitsEE"),
    taggingMode = cms.bool(False),
    debug = cms.bool(False),
    nBadHitsSC = cms.int32(2)
)


process.goodPatJets = cms.EDFilter("PFJetIDSelectionFunctorFilter",
    filter = cms.bool(True),
    src = cms.InputTag("selectedPatJets"),
    filterParams = cms.PSet(
        version = cms.string('FIRSTDATA'),
        quality = cms.string('LOOSE')
    )
)


process.goodVertices = cms.EDFilter("VertexSelector",
    filter = cms.bool(False),
    src = cms.InputTag("offlinePrimaryVertices"),
    cut = cms.string('!isFake && ndof > 4 && abs(z) <= 24 && position.rho < 2')
)


process.hcalLaserEventFilter = cms.EDFilter("HcalLaserEventFilter",
    vetoByHBHEOccupancy = cms.untracked.bool(True),
    maxerrormessage = cms.untracked.int32(5),
    forceUseRecHitCollection = cms.untracked.bool(False),
    forceUseHcalNoiseSummary = cms.untracked.bool(False),
    hbheInputLabel = cms.untracked.InputTag("hbhereco"),
    minOccupiedHBHE = cms.untracked.uint32(4000),
    taggingMode = cms.bool(False),
    debug = cms.untracked.bool(False),
    vetoByRunEventNumber = cms.untracked.bool(False),
    reverseFilter = cms.untracked.bool(False),
    BadRunEventNumbers = (cms.untracked.vuint32(160957, 146483131, 160957, 146483132, 160957, 367078426, 163289, 120704451, 163289, 120704452, 163332, 300924904, 163587, 5705088, 163588, 86700074, 163659, 269761831, 163659, 379050220, 165415, 696548170, 165415, 696548171, 165617, 295894671, 165617, 295894672, 165993, 120876169, 165993, 120876170, 166011, 58123616, 166011, 58123617, 166380, 833988349, 166380, 833988350, 166380, 874735805, 166380, 874735806, 166380, 915050480, 166380, 1037024294, 166512, 1222721981, 166512, 1222721982, 166563, 299342294, 166563, 299431306, 166563, 299431307, 166563, 299645965, 166699, 908134746, 166699, 908134747, 167281, 115904577, 167282, 286707493, 167282, 286707494, 167282, 286766119, 167282, 286766120, 167284, 44118160, 167284, 44118161, 167551, 365086623, 167551, 365086624, 167674, 59067344, 167674, 59067345, 167675, 227610655, 167675, 227610656, 167754, 73011356, 167754, 73011357, 167807, 1202030368, 167807, 1202030369, 167898, 568063754, 167898, 568063755, 167898, 718530727, 167969, 3462839, 167969, 3462840, 167969, 9442755, 167969, 9442756, 167969, 11435992, 167969, 11435993, 170255, 83361834, 170255, 83361835, 170304, 57541359, 170304, 57541360, 170854, 291050200, 170854, 291050201, 170854, 329611102, 170854, 329611103, 170899, 39787119, 170899, 39787120, 171091, 9021397, 171091, 9021398, 171091, 97261559, 171091, 97261560, 171156, 369378017, 171156, 369378018, 171897, 353709470, 172033, 412685841, 172033, 412685842, 172033, 885328695, 172033, 982705197, 172033, 982705198, 172163, 530358965, 172389, 45660388, 172389, 45660389, 172411, 173742880, 172411, 173742881, 172478, 53762243, 172478, 53762244, 172478, 54053290, 172478, 54092625, 172478, 54092626, 172478, 54092948, 172478, 98093904, 172478, 98093905, 172485, 424192588, 172485, 424192589, 172791, 966404647, 172802, 464891113, 172802, 464891114, 172802, 464892883, 172802, 464892884, 172819, 81201593, 172822, 1074244840, 172822, 2836941609, 172868, 393947631, 172868, 393947632, 172868, 1421063049, 172868, 1421063050, 172868, 1421076216, 172868, 1421076217, 172868, 2012432054, 172868, 2012432055, 172868, 2137890207, 172868, 2137890208, 173198, 741435194, 173198, 741435195, 173198, 1009198868, 173198, 1009198869, 173226, 781573, 173226, 781574, 173241, 746837625, 173241, 746837626, 173380, 21324672, 173380, 21324673, 173659, 128113809, 173659, 128113810, 173662, 10511470, 173662, 10511471, 173692)+cms.untracked.vuint32(755803939, 173692, 2597438478, 173692, 2597438479, 174809, 777532, 174809, 777533, 175560, 2368923, 175560, 2368924, 175560, 7580776, 175834, 105072341, 175834, 105072342, 175866, 343429213, 175866, 343429214, 175875, 182390835, 175875, 182390836, 175888, 49192628, 175888, 49192629, 175888, 128999776, 175888, 128999777, 175973, 122366434, 175973, 122366435, 175976, 80421989, 175976, 80421990, 175990, 6376426, 175990, 6376427, 175990, 75007084, 175990, 75007085, 175990, 146437701, 175990, 146437702, 176161, 15560079, 176161, 15560080, 176202, 119772380, 176202, 119772381, 176202, 324604001, 176202, 324604002, 176309, 233512658, 176309, 233512659, 176309, 935495115, 176309, 935495116, 176309, 1331935829, 176309, 1331935830, 176309, 2496631352, 176697, 403510, 176697, 403511, 176701, 73573187, 176701, 73573188, 176702, 11693970, 176702, 11693971, 176702, 67569367, 176702, 67569368, 176801, 410530622, 176929, 460082240, 176929, 460082241, 176954, 138469, 176954, 138470, 177053, 327815438, 177053, 327815439, 177074, 154911610, 177074, 154911611, 177140, 785923000, 177317, 72936561, 177317, 72936562, 177317, 73219012, 177317, 73219013, 177449, 275466422, 177449, 275466423, 177452, 226991391, 177452, 226991392, 177509, 99081581, 177509, 99081582, 177509, 314204437, 177509, 314204438, 177509, 314319381, 177509, 314319382, 177515, 291757022, 177515, 291757023, 177515, 1103056195, 177515, 1103056196, 177515, 1534353246, 177515, 1534353247, 177718, 890704274, 177718, 890704275, 177719, 294071879, 177719, 294071880, 177730, 1850737398, 177730, 1850737399, 177730, 2007600403, 177730, 2007600404, 177730, 2563818242, 177730, 2563818243, 177790, 507968788, 177790, 507968789, 177790, 772640382, 177790, 772640383, 177791, 89470582, 177791, 89470583, 177875, 647616276, 177875, 647616277, 178041, 783372, 178041, 783394, 178041, 783395, 178041, 784044, 178041, 784045, 178041, 784499, 178041, 784500, 178041, 784551, 178041, 784552, 178041, 786438, 178041, 786439, 178041, 786770, 178041, 786771, 178041, 787142, 178041, 787143, 178041, 787202, 178100, 898633273, 178100, 1566052885, 178100, 1566052886, 178116, 453801141, 178116, 453801142, 178424, 630485076, 178424, 630485077, 178667, 494184, 178667, 494185, 178667, 51504048, 178667, 51504049, 178825, 149780, 178825, 149781, 178866, 410333501, 178866, 410333502)+cms.untracked.vuint32(178866, 651783943, 178866, 651783944, 178871, 236100751, 178970, 660540406, 178970, 660540407, 178985, 85355292, 178985, 85355293, 179547, 21999275, 179547, 21999276, 179563, 143108913, 179563, 143108914, 179563, 391201547, 179563, 391201548, 180163, 7578238, 180163, 7578239, 180222, 6076323, 180222, 24642472, 180241, 500046589, 180241, 500562971, 180241, 500562972, 180250, 371542986, 180250, 371542987, 180275, 10578469, 180275, 10578470, 180275, 10583104, 180275, 10583105)),
    hcalNoiseSummaryLabel = cms.untracked.InputTag("hcalnoise")
)


process.hltHighLevel = cms.EDFilter("HLTHighLevel",
    eventSetupPathsKey = cms.string(''),
    TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
    HLTPaths = cms.vstring('HLT_IsoMu24_v*', 
        'HLT_Mu40_v*'),
    throw = cms.bool(False),
    andOr = cms.bool(True)
)


process.noscraping = cms.EDFilter("FilterOutScraping",
    debugOn = cms.untracked.bool(False),
    thresh = cms.untracked.double(0.25),
    numtrack = cms.untracked.uint32(10),
    applyfilter = cms.untracked.bool(True)
)


process.pfAllChargedHadrons = cms.EDFilter("PdgIdPFCandidateSelector",
    pdgId = cms.vint32(211, -211, 321, -321, 999211, 
        2212, -2212),
    src = cms.InputTag("pfNoPileUpIso")
)


process.pfAllChargedParticles = cms.EDFilter("PdgIdPFCandidateSelector",
    pdgId = cms.vint32(211, -211, 321, -321, 999211, 
        2212, -2212, 11, -11, 13, 
        -13),
    src = cms.InputTag("pfNoPileUpIso")
)


process.pfAllElectrons = cms.EDFilter("PdgIdPFCandidateSelector",
    pdgId = cms.vint32(11, -11),
    src = cms.InputTag("pfNoMuon")
)


process.pfAllMuons = cms.EDFilter("PdgIdPFCandidateSelector",
    pdgId = cms.vint32(-13, 13),
    src = cms.InputTag("pfNoPileUp")
)


process.pfAllNeutralHadrons = cms.EDFilter("PdgIdPFCandidateSelector",
    pdgId = cms.vint32(111, 130, 310, 2112),
    src = cms.InputTag("pfNoPileUpIso")
)


process.pfAllNeutralHadronsAndPhotons = cms.EDFilter("PdgIdPFCandidateSelector",
    pdgId = cms.vint32(22, 111, 130, 310, 2112),
    src = cms.InputTag("pfNoPileUpIso")
)


process.pfAllPhotons = cms.EDFilter("PdgIdPFCandidateSelector",
    pdgId = cms.vint32(22),
    src = cms.InputTag("pfNoPileUpIso")
)


process.pfPileUpAllChargedParticles = cms.EDFilter("PdgIdPFCandidateSelector",
    pdgId = cms.vint32(211, -211, 321, -321, 999211, 
        2212, -2212, 11, -11, 13, 
        -13),
    src = cms.InputTag("pfPileUpIso")
)


process.primaryVertexFilter = cms.EDFilter("VertexSelector",
    filter = cms.bool(True),
    src = cms.InputTag("offlinePrimaryVertices"),
    cut = cms.string('!isFake && ndof > 4 && abs(z) <= 24 && position.Rho <= 2')
)


process.selectedPatElectrons = cms.EDFilter("PATElectronSelector",
    src = cms.InputTag("patElectrons"),
    cut = cms.string('pt>10. && abs(eta)<2.5')
)


process.selectedPatJets = cms.EDFilter("PATJetSelector",
    src = cms.InputTag("patJets"),
    cut = cms.string('pt>10. && abs(eta)<5.')
)


process.selectedPatMuons = cms.EDFilter("PATMuonSelector",
    src = cms.InputTag("patMuons"),
    cut = cms.string('isGlobalMuon && pt>10. && abs(eta)<2.5')
)


process.selectedPatPhotons = cms.EDFilter("PATPhotonSelector",
    src = cms.InputTag("patPhotons"),
    cut = cms.string('')
)


process.selectedPatTaus = cms.EDFilter("PATTauSelector",
    src = cms.InputTag("patTaus"),
    cut = cms.string('')
)


process.selectedPrimaryVertexHighestPtTrackSumForPFMEtCorrType0 = cms.EDFilter("PATSingleVertexSelector",
    filter = cms.bool(False),
    vertices = cms.InputTag("selectedVerticesForPFMEtCorrType0"),
    mode = cms.string('firstVertex')
)


process.selectedVerticesForPFMEtCorrType0 = cms.EDFilter("VertexSelector",
    filter = cms.bool(False),
    src = cms.InputTag("offlinePrimaryVertices"),
    cut = cms.string('isValid & ndof >= 4 & chi2 > 0 & tracksSize > 0 & abs(z) < 24 & abs(position.Rho) < 2.')
)


process.tauGenJetsSelectorAllHadrons = cms.EDFilter("TauGenJetDecayModeSelector",
    filter = cms.bool(False),
    src = cms.InputTag("tauGenJets"),
    select = cms.vstring('oneProng0Pi0', 
        'oneProng1Pi0', 
        'oneProng2Pi0', 
        'oneProngOther', 
        'threeProng0Pi0', 
        'threeProng1Pi0', 
        'threeProngOther', 
        'rare')
)


process.trackingFailureFilter = cms.EDFilter("TrackingFailureFilter",
    JetSource = cms.InputTag("ak5PFJets"),
    MinSumPtOverHT = cms.double(0.1),
    TrackSource = cms.InputTag("generalTracks"),
    DxyTrVtxMax = cms.double(0.2),
    taggingMode = cms.bool(False),
    debug = cms.bool(False),
    DzTrVtxMax = cms.double(1),
    VertexSource = cms.InputTag("goodVertices")
)


process.cleanPatCandidateSummary = cms.EDAnalyzer("CandidateSummaryTable",
    logName = cms.untracked.string('cleanPatCandidates|PATSummaryTables'),
    candidates = cms.VInputTag(cms.InputTag("cleanPatElectrons"), cms.InputTag("cleanPatMuons"), cms.InputTag("cleanPatTaus"), cms.InputTag("cleanPatPhotons"), cms.InputTag("cleanPatJets"))
)


process.patCandidateSummary = cms.EDAnalyzer("CandidateSummaryTable",
    logName = cms.untracked.string('patCandidates|PATSummaryTables'),
    candidates = cms.VInputTag(cms.InputTag("patElectrons"), cms.InputTag("patMuons"), cms.InputTag("patTaus"), cms.InputTag("patPhotons"), cms.InputTag("patJets"), 
        cms.InputTag("patMETs"), cms.InputTag("patMETsPF"))
)


process.selectedPatCandidateSummary = cms.EDAnalyzer("CandidateSummaryTable",
    logName = cms.untracked.string('selectedPatCanddiates|PATSummaryTables'),
    candidates = cms.VInputTag(cms.InputTag("selectedPatElectrons"), cms.InputTag("selectedPatMuons"), cms.InputTag("selectedPatTaus"), cms.InputTag("selectedPatPhotons"), cms.InputTag("selectedPatJets"))
)


process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('patTuple_Data.root'),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('p')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_selectedPatJets*_*_*', 
        'drop patJets_selectedPatJets*_*_*', 
        'drop *_selectedPatJets_pfCandidates_*', 
        'drop *_*PF_caloTowers_*', 
        'drop *_*JPT_pfCandidates_*', 
        'drop *_*Calo_pfCandidates_*', 
        'keep *_cleanPatPhotons*_*_*', 
        'keep *_cleanPatElectrons*_*_*', 
        'keep *_cleanPatMuons*_*_*', 
        'keep *_cleanPatTaus*_*_*', 
        'keep *_cleanPatJets*_*_*', 
        'keep *_patMETs*_*_*', 
        'keep *_cleanPatHemispheres*_*_*', 
        'keep *_cleanPatPFParticles*_*_*', 
        'keep *_cleanPatTrackCands*_*_*', 
        'drop *_selectedPatJets*_genJets_*', 
        'drop *_selectedPatJets*_genJets_*', 
        'drop *_selectedPatJets_tagInfos_*', 
        'keep *_selectedPatJets_pfCandidates_*', 
        'drop *_selectedPatJets_caloTowers_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep *_hltTriggerSummaryAOD_*_*', 
        'keep *_goodPatJets_*_*', 
        'keep *_puJetId_*_*', 
        'keep *_puJetMva_*_*', 
        'keep *_offlineBeamSpot_*_*', 
        'keep *_offlinePrimaryVertices*_*_*', 
        'keep *_goodOfflinePrimaryVertices*_*_*')
)


process.pfSortByTypeSequence = cms.Sequence(process.pfAllNeutralHadrons+process.pfAllChargedHadrons+process.pfAllPhotons+process.pfAllChargedParticles+process.pfPileUpAllChargedParticles+process.pfAllNeutralHadronsAndPhotons)


process.patElectronTrackIsolation = cms.Sequence(process.eleIsoDepositTk+process.eleIsoFromDepsTk)


process.patPhotonTrackIsolation = cms.Sequence(process.gamIsoDepositTk+process.gamIsoFromDepsTk)


process.patJetFlavourId = cms.Sequence(process.patJetPartons+process.patJetPartonAssociation+process.patJetFlavourAssociation)


process.patPhotonEcalIsolation = cms.Sequence(process.gamIsoDepositEcalFromHits+process.gamIsoFromDepsEcalFromHits)


process.ak5JTA = cms.Sequence(process.ak5JetTracksAssociatorAtVertex+process.ak5JetTracksAssociatorAtCaloFace+process.ak5JetExtender)


process.patPFTauIsolation = cms.Sequence(process.tauIsoDepositPFCandidates+process.tauIsoDepositPFChargedHadrons+process.tauIsoDepositPFNeutralHadrons+process.tauIsoDepositPFGammas)


process.patElectronHcalIsolation = cms.Sequence(process.eleIsoDepositHcalFromTowers+process.eleIsoFromDepsHcalFromTowers)


process.makePatElectrons = cms.Sequence(process.electronMatch+process.patElectrons)


process.patElectronId = cms.Sequence(process.eidRobustHighEnergy)


process.makePatPhotons = cms.Sequence(process.photonMatch+process.patPhotons)


process.produceCaloMETCorrections = cms.Sequence(process.caloJetMETcorr+process.muonCaloMETcorr+process.caloType1CorrectedMet+process.caloType1p2CorrectedMet)


process.CSCHaloFilterDigiOrRecoLevel = cms.Sequence(process.CSCHaloFilterDigiLevel+process.CSCHaloFilterRecoLevel)


process.puJetIdSqeuenceChs = cms.Sequence(process.puJetIdChs+process.puJetMvaChs)


process.cleanPatCandidates = cms.Sequence(process.cleanPatMuons+process.cleanPatElectrons+process.cleanPatPhotons+process.cleanPatTaus+process.cleanPatJets+process.cleanPatCandidateSummary)


process.producePFMETCorrections = cms.Sequence(process.pfCandsNotInJet+process.pfJetMETcorr+process.pfCandMETcorr+process.pfchsMETcorr+process.pfType1CorrectedMet+process.pfType1p2CorrectedMet)


process.patJetCorrections = cms.Sequence(process.patJetCorrFactors)


process.pfNoPileUpIsoSequence = cms.Sequence(process.pfPileUpIso+process.pfNoPileUpIso)


process.CSCHaloFilterRecoOrTriggerLevel = cms.Sequence(process.CSCHaloFilterTriggerLevel+process.CSCHaloFilterRecoLevel)


process.makePatMHTs = cms.Sequence(process.patMHTs)


process.CSCHaloFilterDigiOrTriggerLevel = cms.Sequence(process.CSCHaloFilterDigiLevel+process.CSCHaloFilterTriggerLevel)


process.CSCHaloFilter_DigiAndReco_Or_DigiAndTrigger_Or_RecoAndTrigger = cms.Sequence(process.CSCHaloFilterRecoAndTriggerLevel+process.CSCHaloFilterDigiAndTriggerLevel+process.CSCHaloFilterDigiAndRecoLevel)


process.puJetIdSqeuence = cms.Sequence(process.puJetId+process.puJetMva)


process.eIdSequence = cms.Sequence(process.eidRobustLoose+process.eidRobustTight+process.eidRobustHighEnergy+process.eidLoose+process.eidTight)


process.patElectronEcalIsolation = cms.Sequence(process.eleIsoDepositEcalFromHits+process.eleIsoFromDepsEcalFromHitsByCrystal)


process.selectedPatCandidates = cms.Sequence(process.selectedPatElectrons+process.selectedPatMuons+process.selectedPatTaus+process.selectedPatPhotons+process.selectedPatJets+process.selectedPatCandidateSummary)


process.CSCHaloFilterDigiOrRecoOrTriggerLevel = cms.Sequence(process.CSCHaloFilterDigiLevel+process.CSCHaloFilterRecoLevel+process.CSCHaloFilterTriggerLevel)


process.countPatCandidates = cms.Sequence(process.countPatElectrons+process.countPatMuons+process.countPatTaus+process.countPatLeptons+process.countPatPhotons+process.countPatJets)


process.type0PFMEtCorrectionPFCandToVertexAssociation = cms.Sequence(process.selectedVerticesForPFMEtCorrType0+process.selectedPrimaryVertexHighestPtTrackSumForPFMEtCorrType0+process.particleFlowDisplacedVertex+process.trackToVertexAssociation+process.pfCandidateToVertexAssociation)


process.patPhotonHcalIsolation = cms.Sequence(process.gamIsoDepositHcalFromTowers+process.gamIsoFromDepsHcalFromTowers)


process.makePatMuons = cms.Sequence(process.muonMatch+process.patMuons)


process.patMETCorrections = cms.Sequence(process.producePFMETCorrections)


process.patElectronIsolation = cms.Sequence(process.patElectronTrackIsolation+process.patElectronEcalIsolation+process.patElectronHcalIsolation)


process.patJetMETCorrections = cms.Sequence(process.patJetCorrections)


process.makePatJets = cms.Sequence(process.patJetCorrections+process.patJetCharge+process.patJetPartonMatch+process.patJetGenJetMatch+process.patJetFlavourId+process.patJets)


process.makePatMETs = cms.Sequence(process.patMETCorrections+process.patMETs)


process.type0PFMEtCorrection = cms.Sequence(process.type0PFMEtCorrectionPFCandToVertexAssociation+process.pfMETcorrType0)


process.patPFCandidateIsoDepositSelection = cms.Sequence(process.pfNoPileUpIsoSequence+process.pfSortByTypeSequence)


process.makePatTaus = cms.Sequence(process.patPFCandidateIsoDepositSelection+process.patPFTauIsolation+process.tauMatch+process.tauGenJets+process.tauGenJetsSelectorAllHadrons+process.tauGenJetMatch+process.patTaus)


process.patPhotonIsolation = cms.Sequence(process.patPhotonTrackIsolation+process.patPhotonEcalIsolation+process.patPhotonHcalIsolation)


process.patCandidates = cms.Sequence(process.makePatElectrons+process.makePatMuons+process.makePatTaus+process.makePatPhotons+process.makePatJets+process.makePatMETs+process.patCandidateSummary)


process.patDefaultSequence = cms.Sequence(process.patElectrons+process.patMuons+process.patPFCandidateIsoDepositSelection+process.patPFTauIsolation+process.patTaus+process.patPhotons+process.patJetCorrections+process.jetTracksAssociatorAtVertex+process.patJetCharge+process.patJets+process.patMETCorrections+process.patMETs+process.patMETsPF+process.patCandidateSummary+process.selectedPatCandidates+process.cleanPatCandidates+process.countPatCandidates)


process.p = cms.Path(process.hltHighLevel+process.noscraping+process.primaryVertexFilter+process.HBHENoiseFilter+process.CSCTightHaloFilter+process.hcalLaserEventFilter+process.EcalDeadCellTriggerPrimitiveFilter+process.eeBadScFilter+process.ecalLaserCorrFilter+process.goodVertices+process.trackingFailureFilter+process.type0PFMEtCorrection+process.patDefaultSequence+process.goodPatJets+process.puJetIdSqeuence)


process.outpath = cms.EndPath(process.out)


process.MessageLogger = cms.Service("MessageLogger",
    suppressInfo = cms.untracked.vstring(),
    debugs = cms.untracked.PSet(
        placeholder = cms.untracked.bool(True)
    ),
    suppressDebug = cms.untracked.vstring(),
    cout = cms.untracked.PSet(
        placeholder = cms.untracked.bool(True)
    ),
    cerr_stats = cms.untracked.PSet(
        threshold = cms.untracked.string('WARNING'),
        output = cms.untracked.string('cerr'),
        optionalPSet = cms.untracked.bool(True)
    ),
    warnings = cms.untracked.PSet(
        placeholder = cms.untracked.bool(True)
    ),
    default = cms.untracked.PSet(

    ),
    statistics = cms.untracked.vstring('cerr_stats'),
    cerr = cms.untracked.PSet(
        INFO = cms.untracked.PSet(
            limit = cms.untracked.int32(0)
        ),
        noTimeStamps = cms.untracked.bool(False),
        FwkReport = cms.untracked.PSet(
            reportEvery = cms.untracked.int32(2000),
            limit = cms.untracked.int32(10000000)
        ),
        default = cms.untracked.PSet(
            limit = cms.untracked.int32(10000000)
        ),
        Root_NoDictionary = cms.untracked.PSet(
            optionalPSet = cms.untracked.bool(True),
            limit = cms.untracked.int32(0)
        ),
        threshold = cms.untracked.string('INFO'),
        FwkJob = cms.untracked.PSet(
            optionalPSet = cms.untracked.bool(True),
            limit = cms.untracked.int32(0)
        ),
        FwkSummary = cms.untracked.PSet(
            reportEvery = cms.untracked.int32(1),
            optionalPSet = cms.untracked.bool(True),
            limit = cms.untracked.int32(10000000)
        ),
        optionalPSet = cms.untracked.bool(True)
    ),
    FrameworkJobReport = cms.untracked.PSet(
        default = cms.untracked.PSet(
            limit = cms.untracked.int32(0)
        ),
        optionalPSet = cms.untracked.bool(True),
        FwkJob = cms.untracked.PSet(
            optionalPSet = cms.untracked.bool(True),
            limit = cms.untracked.int32(10000000)
        )
    ),
    suppressWarning = cms.untracked.vstring(),
    errors = cms.untracked.PSet(
        placeholder = cms.untracked.bool(True)
    ),
    destinations = cms.untracked.vstring('warnings', 
        'errors', 
        'infos', 
        'debugs', 
        'cout', 
        'cerr'),
    debugModules = cms.untracked.vstring(),
    infos = cms.untracked.PSet(
        optionalPSet = cms.untracked.bool(True),
        Root_NoDictionary = cms.untracked.PSet(
            optionalPSet = cms.untracked.bool(True),
            limit = cms.untracked.int32(0)
        ),
        placeholder = cms.untracked.bool(True)
    ),
    categories = cms.untracked.vstring('FwkJob', 
        'FwkReport', 
        'FwkSummary', 
        'Root_NoDictionary'),
    fwkJobReports = cms.untracked.vstring('FrameworkJobReport')
)


process.AK5PFCombinedCorrector = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('AK5PFL1FastJet', 
        'AK5PFL2Relative', 
        'AK5PFL3Absolute', 
        'AK5PFL2L3Residual')
)


process.AK5PFL1FastJet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("kt6PFJets","rho"),
    algorithm = cms.string('AK5PF'),
    level = cms.string('L1FastJet')
)


process.AK5PFL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK5PF'),
    level = cms.string('L1Offset')
)


process.AK5PFL2L3Residual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5PF'),
    level = cms.string('L2L3Residual')
)


process.AK5PFL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5PF'),
    level = cms.string('L2Relative')
)


process.AK5PFL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5PF'),
    level = cms.string('L3Absolute')
)


process.CSCGeometryESModule = cms.ESProducer("CSCGeometryESModule",
    appendToDataLabel = cms.string(''),
    useDDD = cms.bool(True),
    debugV = cms.untracked.bool(False),
    useGangedStripsInME1a = cms.bool(True),
    alignmentsLabel = cms.string(''),
    useOnlyWiresInME1a = cms.bool(False),
    useRealWireGeometry = cms.bool(True),
    useCentreTIOffsets = cms.bool(False),
    applyAlignment = cms.bool(True)
)


process.CaloGeometryBuilder = cms.ESProducer("CaloGeometryBuilder",
    SelectedCalos = cms.vstring('HCAL', 
        'ZDC', 
        'CASTOR', 
        'EcalBarrel', 
        'EcalEndcap', 
        'EcalPreshower', 
        'TOWER')
)


process.CaloTopologyBuilder = cms.ESProducer("CaloTopologyBuilder")


process.CaloTowerHardcodeGeometryEP = cms.ESProducer("CaloTowerHardcodeGeometryEP")


process.CastorDbProducer = cms.ESProducer("CastorDbProducer")


process.CastorHardcodeGeometryEP = cms.ESProducer("CastorHardcodeGeometryEP")


process.DTGeometryESModule = cms.ESProducer("DTGeometryESModule",
    appendToDataLabel = cms.string(''),
    fromDDD = cms.bool(True),
    applyAlignment = cms.bool(True),
    alignmentsLabel = cms.string('')
)


process.EcalBarrelGeometryEP = cms.ESProducer("EcalBarrelGeometryEP",
    applyAlignment = cms.bool(False)
)


process.EcalElectronicsMappingBuilder = cms.ESProducer("EcalElectronicsMappingBuilder")


process.EcalEndcapGeometryEP = cms.ESProducer("EcalEndcapGeometryEP",
    applyAlignment = cms.bool(False)
)


process.EcalLaserCorrectionService = cms.ESProducer("EcalLaserCorrectionService")


process.EcalPreshowerGeometryEP = cms.ESProducer("EcalPreshowerGeometryEP",
    applyAlignment = cms.bool(False)
)


process.EcalTrigTowerConstituentsMapBuilder = cms.ESProducer("EcalTrigTowerConstituentsMapBuilder",
    MapFile = cms.untracked.string('Geometry/EcalMapping/data/EndCap_TTMap.txt')
)


process.GlobalTrackingGeometryESProducer = cms.ESProducer("GlobalTrackingGeometryESProducer")


process.HcalHardcodeGeometryEP = cms.ESProducer("HcalHardcodeGeometryEP")


process.HcalTopologyIdealEP = cms.ESProducer("HcalTopologyIdealEP")


process.MuonDetLayerGeometryESProducer = cms.ESProducer("MuonDetLayerGeometryESProducer")


process.MuonNumberingInitialization = cms.ESProducer("MuonNumberingInitialization")


process.ParametrizedMagneticFieldProducer = cms.ESProducer("ParametrizedMagneticFieldProducer",
    version = cms.string('OAE_1103l_071212'),
    parameters = cms.PSet(
        BValue = cms.string('3_8T')
    ),
    label = cms.untracked.string('parametrizedField')
)


process.RPCGeometryESModule = cms.ESProducer("RPCGeometryESModule",
    useDDD = cms.untracked.bool(True),
    compatibiltyWith11 = cms.untracked.bool(True)
)


process.SiStripRecHitMatcherESProducer = cms.ESProducer("SiStripRecHitMatcherESProducer",
    ComponentName = cms.string('StandardMatcher'),
    NSigmaInside = cms.double(3.0)
)


process.SteppingHelixPropagatorAlong = cms.ESProducer("SteppingHelixPropagatorESProducer",
    endcapShiftInZNeg = cms.double(0.0),
    PropagationDirection = cms.string('alongMomentum'),
    useMatVolumes = cms.bool(True),
    useTuningForL2Speed = cms.bool(False),
    useIsYokeFlag = cms.bool(True),
    NoErrorPropagation = cms.bool(False),
    SetVBFPointer = cms.bool(False),
    AssumeNoMaterial = cms.bool(False),
    returnTangentPlane = cms.bool(True),
    useInTeslaFromMagField = cms.bool(False),
    VBFName = cms.string('VolumeBasedMagneticField'),
    useEndcapShiftsInZ = cms.bool(False),
    sendLogWarning = cms.bool(False),
    ComponentName = cms.string('SteppingHelixPropagatorAlong'),
    debug = cms.bool(False),
    ApplyRadX0Correction = cms.bool(True),
    useMagVolumes = cms.bool(True),
    endcapShiftInZPos = cms.double(0.0)
)


process.StripCPEfromTrackAngleESProducer = cms.ESProducer("StripCPEESProducer",
    ComponentName = cms.string('StripCPEfromTrackAngle')
)


process.TrackerDigiGeometryESModule = cms.ESProducer("TrackerDigiGeometryESModule",
    appendToDataLabel = cms.string(''),
    fromDDD = cms.bool(True),
    applyAlignment = cms.bool(True),
    alignmentsLabel = cms.string('')
)


process.TrackerGeometricDetESModule = cms.ESProducer("TrackerGeometricDetESModule",
    fromDDD = cms.bool(True)
)


process.TrackerRecoGeometryESProducer = cms.ESProducer("TrackerRecoGeometryESProducer")


process.TransientTrackBuilderESProducer = cms.ESProducer("TransientTrackBuilderESProducer",
    ComponentName = cms.string('TransientTrackBuilder')
)


process.VolumeBasedMagneticFieldESProducer = cms.ESProducer("VolumeBasedMagneticFieldESProducer",
    scalingVolumes = cms.vint32(14100, 14200, 17600, 17800, 17900, 
        18100, 18300, 18400, 18600, 23100, 
        23300, 23400, 23600, 23800, 23900, 
        24100, 28600, 28800, 28900, 29100, 
        29300, 29400, 29600, 28609, 28809, 
        28909, 29109, 29309, 29409, 29609, 
        28610, 28810, 28910, 29110, 29310, 
        29410, 29610, 28611, 28811, 28911, 
        29111, 29311, 29411, 29611),
    scalingFactors = cms.vdouble(1, 1, 0.994, 1.004, 1.004, 
        1.005, 1.004, 1.004, 0.994, 0.965, 
        0.958, 0.958, 0.953, 0.958, 0.958, 
        0.965, 0.918, 0.924, 0.924, 0.906, 
        0.924, 0.924, 0.918, 0.991, 0.998, 
        0.998, 0.978, 0.998, 0.998, 0.991, 
        0.991, 0.998, 0.998, 0.978, 0.998, 
        0.998, 0.991, 0.991, 0.998, 0.998, 
        0.978, 0.998, 0.998, 0.991),
    overrideMasterSector = cms.bool(False),
    useParametrizedTrackerField = cms.bool(True),
    label = cms.untracked.string(''),
    version = cms.string('grid_1103l_090322_3_8t'),
    debugBuilder = cms.untracked.bool(False),
    paramLabel = cms.string('parametrizedField'),
    geometryVersion = cms.int32(90322),
    cacheLastVolume = cms.untracked.bool(True)
)


process.ZdcHardcodeGeometryEP = cms.ESProducer("ZdcHardcodeGeometryEP")


process.ak5CaloL1FastL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5CaloL1Fastjet', 
        'ak5CaloL2Relative', 
        'ak5CaloL3Absolute')
)


process.ak5CaloL1FastL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5CaloL1Fastjet', 
        'ak5CaloL2Relative', 
        'ak5CaloL3Absolute', 
        'ak5CaloL6SLB')
)


process.ak5CaloL1FastL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5CaloL1Fastjet', 
        'ak5CaloL2Relative', 
        'ak5CaloL3Absolute', 
        'ak5CaloResidual')
)


process.ak5CaloL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("kt6CaloJets","rho"),
    algorithm = cms.string('AK5Calo'),
    level = cms.string('L1FastJet')
)


process.ak5CaloL1L2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5CaloL1Offset', 
        'ak5CaloL2Relative', 
        'ak5CaloL3Absolute')
)


process.ak5CaloL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5CaloL1Offset', 
        'ak5CaloL2Relative', 
        'ak5CaloL3Absolute', 
        'ak5CaloResidual')
)


process.ak5CaloL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK5Calo'),
    level = cms.string('L1Offset')
)


process.ak5CaloL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5CaloL2Relative', 
        'ak5CaloL3Absolute')
)


process.ak5CaloL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5CaloL2Relative', 
        'ak5CaloL3Absolute', 
        'ak5CaloL6SLB')
)


process.ak5CaloL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5CaloL2Relative', 
        'ak5CaloL3Absolute', 
        'ak5CaloResidual')
)


process.ak5CaloL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5Calo'),
    level = cms.string('L2Relative')
)


process.ak5CaloL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5Calo'),
    level = cms.string('L3Absolute')
)


process.ak5CaloL6SLB = cms.ESProducer("L6SLBCorrectionESProducer",
    srcBTagInfoElectron = cms.InputTag("ak5CaloJetsSoftElectronTagInfos"),
    srcBTagInfoMuon = cms.InputTag("ak5CaloJetsSoftMuonTagInfos"),
    addMuonToJet = cms.bool(True),
    algorithm = cms.string(''),
    level = cms.string('L6SLB')
)


process.ak5CaloResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5Calo'),
    level = cms.string('L2L3Residual')
)


process.ak5JPTL1FastL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5JPTL1Fastjet', 
        'ak5JPTL2Relative', 
        'ak5JPTL3Absolute')
)


process.ak5JPTL1FastL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5JPTL1Fastjet', 
        'ak5JPTL2Relative', 
        'ak5JPTL3Absolute', 
        'ak5JPTResidual')
)


process.ak5JPTL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("kt6CaloJets","rho"),
    algorithm = cms.string('AK5Calo'),
    level = cms.string('L1FastJet')
)


process.ak5JPTL1L2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5L1JPTOffset', 
        'ak5JPTL2Relative', 
        'ak5JPTL3Absolute')
)


process.ak5JPTL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5L1JPTOffset', 
        'ak5JPTL2Relative', 
        'ak5JPTL3Absolute', 
        'ak5JPTResidual')
)


process.ak5JPTL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK5JPT'),
    level = cms.string('L1Offset')
)


process.ak5JPTL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5L1JPTOffset', 
        'ak5JPTL2Relative', 
        'ak5JPTL3Absolute')
)


process.ak5JPTL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5L1JPTOffset', 
        'ak5JPTL2Relative', 
        'ak5JPTL3Absolute', 
        'ak5JPTResidual')
)


process.ak5JPTL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5JPT'),
    level = cms.string('L2Relative')
)


process.ak5JPTL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5JPT'),
    level = cms.string('L3Absolute')
)


process.ak5JPTResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5JPT'),
    level = cms.string('L2L3Residual')
)


process.ak5L1JPTOffset = cms.ESProducer("L1JPTOffsetCorrectionESProducer",
    offsetService = cms.string('ak5CaloL1Offset'),
    algorithm = cms.string('AK5JPT'),
    level = cms.string('L1JPTOffset')
)


process.ak5PFL1FastL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5PFL1Fastjet', 
        'ak5PFL2Relative', 
        'ak5PFL3Absolute')
)


process.ak5PFL1FastL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5PFL1Fastjet', 
        'ak5PFL2Relative', 
        'ak5PFL3Absolute', 
        'ak5PFL6SLB')
)


process.ak5PFL1FastL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5PFL1Fastjet', 
        'ak5PFL2Relative', 
        'ak5PFL3Absolute', 
        'ak5PFResidual')
)


process.ak5PFL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("kt6PFJets","rho"),
    algorithm = cms.string('AK5PF'),
    level = cms.string('L1FastJet')
)


process.ak5PFL1L2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5PFL1Offset', 
        'ak5PFL2Relative', 
        'ak5PFL3Absolute')
)


process.ak5PFL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5PFL1Offset', 
        'ak5PFL2Relative', 
        'ak5PFL3Absolute', 
        'ak5PFResidual')
)


process.ak5PFL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK5PF'),
    level = cms.string('L1Offset')
)


process.ak5PFL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5PFL2Relative', 
        'ak5PFL3Absolute')
)


process.ak5PFL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5PFL2Relative', 
        'ak5PFL3Absolute', 
        'ak5PFL6SLB')
)


process.ak5PFL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5PFL2Relative', 
        'ak5PFL3Absolute', 
        'ak5PFResidual')
)


process.ak5PFL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5PF'),
    level = cms.string('L2Relative')
)


process.ak5PFL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5PF'),
    level = cms.string('L3Absolute')
)


process.ak5PFL6SLB = cms.ESProducer("L6SLBCorrectionESProducer",
    srcBTagInfoElectron = cms.InputTag("ak5PFJetsSoftElectronTagInfos"),
    srcBTagInfoMuon = cms.InputTag("ak5PFJetsSoftMuonTagInfos"),
    addMuonToJet = cms.bool(False),
    algorithm = cms.string(''),
    level = cms.string('L6SLB')
)


process.ak5PFResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5PF'),
    level = cms.string('L2L3Residual')
)


process.ak5TrackL1FastL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5CaloL1Fastjet', 
        'ak5TrackL2Relative', 
        'ak5TrackL3Absolute')
)


process.ak5TrackL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5TrackL2Relative', 
        'ak5TrackL3Absolute')
)


process.ak5TrackL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5TRK'),
    level = cms.string('L2Relative')
)


process.ak5TrackL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5TRK'),
    level = cms.string('L3Absolute')
)


process.ak7CaloL1FastL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5CaloL1Fastjet', 
        'ak7CaloL2Relative', 
        'ak7CaloL3Absolute')
)


process.ak7CaloL1FastL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak7CaloL1Offset', 
        'ak7CaloL2Relative', 
        'ak7CaloL3Absolute', 
        'ak7CaloL6SLB')
)


process.ak7CaloL1FastL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak7CaloL1Fastjet', 
        'ak7CaloL2Relative', 
        'ak7CaloL3Absolute', 
        'ak7CaloResidual')
)


process.ak7CaloL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("kt6CaloJets","rho"),
    algorithm = cms.string('AK5Calo'),
    level = cms.string('L1FastJet')
)


process.ak7CaloL1L2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak7CaloL1Offset', 
        'ak7CaloL2Relative', 
        'ak7CaloL3Absolute')
)


process.ak7CaloL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak7CaloL1Offset', 
        'ak7CaloL2Relative', 
        'ak7CaloL3Absolute', 
        'ak7CaloResidual')
)


process.ak7CaloL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK5Calo'),
    level = cms.string('L1Offset')
)


process.ak7CaloL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak7CaloL2Relative', 
        'ak7CaloL3Absolute')
)


process.ak7CaloL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak7CaloL2Relative', 
        'ak7CaloL3Absolute', 
        'ak7CaloL6SLB')
)


process.ak7CaloL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak7CaloL2Relative', 
        'ak7CaloL3Absolute', 
        'ak7CaloResidual')
)


process.ak7CaloL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK7Calo'),
    level = cms.string('L2Relative')
)


process.ak7CaloL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK7Calo'),
    level = cms.string('L3Absolute')
)


process.ak7CaloL6SLB = cms.ESProducer("L6SLBCorrectionESProducer",
    srcBTagInfoElectron = cms.InputTag("ak7CaloJetsSoftElectronTagInfos"),
    srcBTagInfoMuon = cms.InputTag("ak7CaloJetsSoftMuonTagInfos"),
    addMuonToJet = cms.bool(True),
    algorithm = cms.string(''),
    level = cms.string('L6SLB')
)


process.ak7CaloResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5Calo'),
    level = cms.string('L2L3Residual')
)


process.ak7JPTL1FastL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak7JPTL1Fastjet', 
        'ak7L1JPTOffset', 
        'ak7JPTL2Relative', 
        'ak7JPTL3Absolute', 
        'ak7JPTResidual')
)


process.ak7JPTL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("kt6CaloJets","rho"),
    algorithm = cms.string('AK5Calo'),
    level = cms.string('L1FastJet')
)


process.ak7JPTL1L2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak7JPTL1Offset', 
        'ak7L1JPTOffset', 
        'ak7JPTL2Relative', 
        'ak7JPTL3Absolute')
)


process.ak7JPTL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak7JPTL1Offset', 
        'ak7L1JPTOffset', 
        'ak7JPTL2Relative', 
        'ak7JPTL3Absolute', 
        'ak7JPTResidual')
)


process.ak7JPTL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK5Calo'),
    level = cms.string('L1Offset')
)


process.ak7JPTL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak7L1JPTOffset', 
        'ak7JPTL2Relative', 
        'ak7JPTL3Absolute')
)


process.ak7L1JPTOffset = cms.ESProducer("L1JPTOffsetCorrectionESProducer",
    offsetService = cms.string('ak5CaloL1Offset'),
    algorithm = cms.string('AK5JPT'),
    level = cms.string('L1JPTOffset')
)


process.ak7PFL1FastL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5PFL1Fastjet', 
        'ak7PFL2Relative', 
        'ak7PFL3Absolute')
)


process.ak7PFL1FastL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5PFL1Fastjet', 
        'ak7PFL2Relative', 
        'ak7PFL3Absolute', 
        'ak7PFL6SLB')
)


process.ak7PFL1FastL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak7PFL1Fastjet', 
        'ak7PFL2Relative', 
        'ak7PFL3Absolute', 
        'ak7PFResidual')
)


process.ak7PFL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("kt6PFJets","rho"),
    algorithm = cms.string('AK5PF'),
    level = cms.string('L1FastJet')
)


process.ak7PFL1L2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak7PFL1Offset', 
        'ak7PFL2Relative', 
        'ak7PFL3Absolute')
)


process.ak7PFL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak7PFL1Offset', 
        'ak7PFL2Relative', 
        'ak7PFL3Absolute', 
        'ak7PFResidual')
)


process.ak7PFL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK5PF'),
    level = cms.string('L1Offset')
)


process.ak7PFL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak7PFL2Relative', 
        'ak7PFL3Absolute')
)


process.ak7PFL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak7PFL2Relative', 
        'ak7PFL3Absolute', 
        'ak7PFL6SLB')
)


process.ak7PFL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak7PFL2Relative', 
        'ak7PFL3Absolute', 
        'ak7PFResidual')
)


process.ak7PFL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK7PF'),
    level = cms.string('L2Relative')
)


process.ak7PFL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK7PF'),
    level = cms.string('L3Absolute')
)


process.ak7PFL6SLB = cms.ESProducer("L6SLBCorrectionESProducer",
    srcBTagInfoElectron = cms.InputTag("ak7PFJetsSoftElectronTagInfos"),
    srcBTagInfoMuon = cms.InputTag("ak7PFJetsSoftMuonTagInfos"),
    addMuonToJet = cms.bool(False),
    algorithm = cms.string(''),
    level = cms.string('L6SLB')
)


process.ak7PFResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5PF'),
    level = cms.string('L2L3Residual')
)


process.fakeForIdealAlignment = cms.ESProducer("FakeAlignmentProducer",
    appendToDataLabel = cms.string('fakeForIdeal')
)


process.hcal_db_producer = cms.ESProducer("HcalDbProducer",
    file = cms.untracked.string(''),
    dump = cms.untracked.vstring('')
)


process.ic5CaloL1FastL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5CaloL1Fastjet', 
        'ic5CaloL2Relative', 
        'ic5CaloL3Absolute')
)


process.ic5CaloL1FastL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ic5CaloL1Offset', 
        'ic5CaloL2Relative', 
        'ic5CaloL3Absolute', 
        'ic5CaloL6SLB')
)


process.ic5CaloL1FastL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ic5CaloL1Fastjet', 
        'ic5CaloL2Relative', 
        'ic5CaloL3Absolute', 
        'ic5CaloResidual')
)


process.ic5CaloL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("kt6CaloJets","rho"),
    algorithm = cms.string('AK5Calo'),
    level = cms.string('L1FastJet')
)


process.ic5CaloL1L2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ic5CaloL1Offset', 
        'ic5CaloL2Relative', 
        'ic5CaloL3Absolute')
)


process.ic5CaloL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ic5CaloL1Offset', 
        'ic5CaloL2Relative', 
        'ic5CaloL3Absolute', 
        'ic5CaloResidual')
)


process.ic5CaloL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK5Calo'),
    level = cms.string('L1Offset')
)


process.ic5CaloL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ic5CaloL2Relative', 
        'ic5CaloL3Absolute')
)


process.ic5CaloL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ic5CaloL2Relative', 
        'ic5CaloL3Absolute', 
        'ic5CaloL6SLB')
)


process.ic5CaloL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ic5CaloL2Relative', 
        'ic5CaloL3Absolute', 
        'ic5CaloResidual')
)


process.ic5CaloL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('IC5Calo'),
    level = cms.string('L2Relative')
)


process.ic5CaloL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('IC5Calo'),
    level = cms.string('L3Absolute')
)


process.ic5CaloL6SLB = cms.ESProducer("L6SLBCorrectionESProducer",
    srcBTagInfoElectron = cms.InputTag("ic5CaloJetsSoftElectronTagInfos"),
    srcBTagInfoMuon = cms.InputTag("ic5CaloJetsSoftMuonTagInfos"),
    addMuonToJet = cms.bool(True),
    algorithm = cms.string(''),
    level = cms.string('L6SLB')
)


process.ic5CaloResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5Calo'),
    level = cms.string('L2L3Residual')
)


process.ic5PFL1FastL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5PFL1Fastjet', 
        'ic5PFL2Relative', 
        'ic5PFL3Absolute')
)


process.ic5PFL1FastL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5PFL1Fastjet', 
        'ic5PFL2Relative', 
        'ic5PFL3Absolute', 
        'ic5PFL6SLB')
)


process.ic5PFL1FastL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ic5PFL1Fastjet', 
        'ic5PFL2Relative', 
        'ic5PFL3Absolute', 
        'ic5PFResidual')
)


process.ic5PFL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("kt6PFJets","rho"),
    algorithm = cms.string('AK5PF'),
    level = cms.string('L1FastJet')
)


process.ic5PFL1L2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ic5PFL1Offset', 
        'ic5PFL2Relative', 
        'ic5PFL3Absolute')
)


process.ic5PFL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ic5PFL1Offset', 
        'ic5PFL2Relative', 
        'ic5PFL3Absolute', 
        'ic5PFResidual')
)


process.ic5PFL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK5PF'),
    level = cms.string('L1Offset')
)


process.ic5PFL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ic5PFL2Relative', 
        'ic5PFL3Absolute')
)


process.ic5PFL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ic5PFL2Relative', 
        'ic5PFL3Absolute', 
        'ic5PFL6SLB')
)


process.ic5PFL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ic5PFL2Relative', 
        'ic5PFL3Absolute', 
        'ic5PFResidual')
)


process.ic5PFL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('IC5PF'),
    level = cms.string('L2Relative')
)


process.ic5PFL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('IC5PF'),
    level = cms.string('L3Absolute')
)


process.ic5PFL6SLB = cms.ESProducer("L6SLBCorrectionESProducer",
    srcBTagInfoElectron = cms.InputTag("ic5PFJetsSoftElectronTagInfos"),
    srcBTagInfoMuon = cms.InputTag("ic5PFJetsSoftMuonTagInfos"),
    addMuonToJet = cms.bool(False),
    algorithm = cms.string(''),
    level = cms.string('L6SLB')
)


process.ic5PFResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5PF'),
    level = cms.string('L2L3Residual')
)


process.idealForDigiCSCGeometry = cms.ESProducer("CSCGeometryESModule",
    appendToDataLabel = cms.string('idealForDigi'),
    useDDD = cms.bool(True),
    debugV = cms.untracked.bool(False),
    useGangedStripsInME1a = cms.bool(True),
    alignmentsLabel = cms.string('fakeForIdeal'),
    useOnlyWiresInME1a = cms.bool(False),
    useRealWireGeometry = cms.bool(True),
    useCentreTIOffsets = cms.bool(False),
    applyAlignment = cms.bool(False)
)


process.idealForDigiDTGeometry = cms.ESProducer("DTGeometryESModule",
    appendToDataLabel = cms.string('idealForDigi'),
    fromDDD = cms.bool(True),
    applyAlignment = cms.bool(False),
    alignmentsLabel = cms.string('fakeForIdeal')
)


process.idealForDigiTrackerGeometry = cms.ESProducer("TrackerDigiGeometryESModule",
    appendToDataLabel = cms.string('idealForDigi'),
    fromDDD = cms.bool(True),
    applyAlignment = cms.bool(False),
    alignmentsLabel = cms.string('fakeForIdeal')
)


process.kt4CaloL1FastL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5CaloL1Fastjet', 
        'kt4CaloL2Relative', 
        'kt4CaloL3Absolute')
)


process.kt4CaloL1FastL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt4CaloL1Offset', 
        'kt4CaloL2Relative', 
        'kt4CaloL3Absolute', 
        'kt4CaloL6SLB')
)


process.kt4CaloL1FastL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt4CaloL1Fastjet', 
        'kt4CaloL2Relative', 
        'kt4CaloL3Absolute', 
        'kt4CaloResidual')
)


process.kt4CaloL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("kt6CaloJets","rho"),
    algorithm = cms.string('AK5Calo'),
    level = cms.string('L1FastJet')
)


process.kt4CaloL1L2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt4CaloL1Offset', 
        'kt4CaloL2Relative', 
        'kt4CaloL3Absolute')
)


process.kt4CaloL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt4CaloL1Offset', 
        'kt4CaloL2Relative', 
        'kt4CaloL3Absolute', 
        'kt4CaloResidual')
)


process.kt4CaloL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK5Calo'),
    level = cms.string('L1Offset')
)


process.kt4CaloL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt4CaloL2Relative', 
        'kt4CaloL3Absolute')
)


process.kt4CaloL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt4CaloL2Relative', 
        'kt4CaloL3Absolute', 
        'kt4CaloL6SLB')
)


process.kt4CaloL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt4CaloL2Relative', 
        'kt4CaloL3Absolute', 
        'kt4CaloResidual')
)


process.kt4CaloL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('KT4Calo'),
    level = cms.string('L2Relative')
)


process.kt4CaloL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('KT4Calo'),
    level = cms.string('L3Absolute')
)


process.kt4CaloL6SLB = cms.ESProducer("L6SLBCorrectionESProducer",
    srcBTagInfoElectron = cms.InputTag("kt4CaloJetsSoftElectronTagInfos"),
    srcBTagInfoMuon = cms.InputTag("kt4CaloJetsSoftMuonTagInfos"),
    addMuonToJet = cms.bool(True),
    algorithm = cms.string(''),
    level = cms.string('L6SLB')
)


process.kt4CaloResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5Calo'),
    level = cms.string('L2L3Residual')
)


process.kt4PFL1FastL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5PFL1Fastjet', 
        'kt4PFL2Relative', 
        'kt4PFL3Absolute')
)


process.kt4PFL1FastL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5PFL1Fastjet', 
        'kt4PFL2Relative', 
        'kt4PFL3Absolute', 
        'kt4PFL6SLB')
)


process.kt4PFL1FastL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt4PFL1Fastjet', 
        'kt4PFL2Relative', 
        'kt4PFL3Absolute', 
        'kt4PFResidual')
)


process.kt4PFL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("kt6PFJets","rho"),
    algorithm = cms.string('AK5PF'),
    level = cms.string('L1FastJet')
)


process.kt4PFL1L2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt4PFL1Offset', 
        'kt4PFL2Relative', 
        'kt4PFL3Absolute')
)


process.kt4PFL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt4PFL1Offset', 
        'kt4PFL2Relative', 
        'kt4PFL3Absolute', 
        'kt4PFResidual')
)


process.kt4PFL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK5PF'),
    level = cms.string('L1Offset')
)


process.kt4PFL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt4PFL2Relative', 
        'kt4PFL3Absolute')
)


process.kt4PFL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt4PFL2Relative', 
        'kt4PFL3Absolute', 
        'kt4PFL6SLB')
)


process.kt4PFL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt4PFL2Relative', 
        'kt4PFL3Absolute', 
        'kt4PFResidual')
)


process.kt4PFL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('KT4PF'),
    level = cms.string('L2Relative')
)


process.kt4PFL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('KT4PF'),
    level = cms.string('L3Absolute')
)


process.kt4PFL6SLB = cms.ESProducer("L6SLBCorrectionESProducer",
    srcBTagInfoElectron = cms.InputTag("kt4PFJetsSoftElectronTagInfos"),
    srcBTagInfoMuon = cms.InputTag("kt4PFJetsSoftMuonTagInfos"),
    addMuonToJet = cms.bool(False),
    algorithm = cms.string(''),
    level = cms.string('L6SLB')
)


process.kt4PFResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5PF'),
    level = cms.string('L2L3Residual')
)


process.kt6CaloL1FastL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5CaloL1Fastjet', 
        'kt6CaloL2Relative', 
        'kt6CaloL3Absolute')
)


process.kt6CaloL1FastL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt6CaloL1Offset', 
        'kt6CaloL2Relative', 
        'kt6CaloL3Absolute', 
        'kt6CaloL6SLB')
)


process.kt6CaloL1FastL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt6CaloL1Fastjet', 
        'kt6CaloL2Relative', 
        'kt6CaloL3Absolute', 
        'kt6CaloResidual')
)


process.kt6CaloL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("kt6CaloJets","rho"),
    algorithm = cms.string('AK5Calo'),
    level = cms.string('L1FastJet')
)


process.kt6CaloL1L2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt6CaloL1Offset', 
        'kt6CaloL2Relative', 
        'kt6CaloL3Absolute')
)


process.kt6CaloL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt6CaloL1Offset', 
        'kt6CaloL2Relative', 
        'kt6CaloL3Absolute', 
        'kt6CaloResidual')
)


process.kt6CaloL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK5Calo'),
    level = cms.string('L1Offset')
)


process.kt6CaloL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt6CaloL2Relative', 
        'kt6CaloL3Absolute')
)


process.kt6CaloL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt6CaloL2Relative', 
        'kt6CaloL3Absolute', 
        'kt6CaloL6SLB')
)


process.kt6CaloL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt6CaloL2Relative', 
        'kt6CaloL3Absolute', 
        'kt6CaloResidual')
)


process.kt6CaloL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('KT6Calo'),
    level = cms.string('L2Relative')
)


process.kt6CaloL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('KT6Calo'),
    level = cms.string('L3Absolute')
)


process.kt6CaloL6SLB = cms.ESProducer("L6SLBCorrectionESProducer",
    srcBTagInfoElectron = cms.InputTag("kt6CaloJetsSoftElectronTagInfos"),
    srcBTagInfoMuon = cms.InputTag("kt6CaloJetsSoftMuonTagInfos"),
    addMuonToJet = cms.bool(True),
    algorithm = cms.string(''),
    level = cms.string('L6SLB')
)


process.kt6CaloResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5Calo'),
    level = cms.string('L2L3Residual')
)


process.kt6PFL1FastL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5PFL1Fastjet', 
        'kt6PFL2Relative', 
        'kt6PFL3Absolute')
)


process.kt6PFL1FastL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('ak5PFL1Fastjet', 
        'kt6PFL2Relative', 
        'kt6PFL3Absolute', 
        'kt6PFL6SLB')
)


process.kt6PFL1FastL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt6PFL1Fastjet', 
        'kt6PFL2Relative', 
        'kt6PFL3Absolute', 
        'kt6PFResidual')
)


process.kt6PFL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("kt6PFJets","rho"),
    algorithm = cms.string('AK5PF'),
    level = cms.string('L1FastJet')
)


process.kt6PFL1L2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt6PFL1Offset', 
        'kt6PFL2Relative', 
        'kt6PFL3Absolute')
)


process.kt6PFL1L2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt6PFL1Offset', 
        'kt6PFL2Relative', 
        'kt6PFL3Absolute', 
        'kt6PFResidual')
)


process.kt6PFL1Offset = cms.ESProducer("L1OffsetCorrectionESProducer",
    minVtxNdof = cms.int32(4),
    vertexCollection = cms.string('offlinePrimaryVertices'),
    algorithm = cms.string('AK5PF'),
    level = cms.string('L1Offset')
)


process.kt6PFL2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt6PFL2Relative', 
        'kt6PFL3Absolute')
)


process.kt6PFL2L3L6 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt6PFL2Relative', 
        'kt6PFL3Absolute', 
        'kt6PFL6SLB')
)


process.kt6PFL2L3Residual = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring('kt6PFL2Relative', 
        'kt6PFL3Absolute', 
        'kt6PFResidual')
)


process.kt6PFL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('KT6PF'),
    level = cms.string('L2Relative')
)


process.kt6PFL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('KT6PF'),
    level = cms.string('L3Absolute')
)


process.kt6PFL6SLB = cms.ESProducer("L6SLBCorrectionESProducer",
    srcBTagInfoElectron = cms.InputTag("kt6PFJetsSoftElectronTagInfos"),
    srcBTagInfoMuon = cms.InputTag("kt6PFJetsSoftMuonTagInfos"),
    addMuonToJet = cms.bool(False),
    algorithm = cms.string(''),
    level = cms.string('L6SLB')
)


process.kt6PFResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5PF'),
    level = cms.string('L2L3Residual')
)


process.siPixelQualityESProducer = cms.ESProducer("SiPixelQualityESProducer",
    ListOfRecordToMerge = cms.VPSet(cms.PSet(
        record = cms.string('SiPixelQualityFromDbRcd'),
        tag = cms.string('')
    ), 
        cms.PSet(
            record = cms.string('SiPixelDetVOffRcd'),
            tag = cms.string('')
        ))
)


process.siStripGainESProducer = cms.ESProducer("SiStripGainESProducer",
    printDebug = cms.untracked.bool(False),
    appendToDataLabel = cms.string(''),
    APVGain = cms.VPSet(cms.PSet(
        Record = cms.string('SiStripApvGainRcd'),
        NormalizationFactor = cms.untracked.double(1.0),
        Label = cms.untracked.string('')
    ), 
        cms.PSet(
            Record = cms.string('SiStripApvGain2Rcd'),
            NormalizationFactor = cms.untracked.double(1.0),
            Label = cms.untracked.string('')
        )),
    AutomaticNormalization = cms.bool(False)
)


process.siStripLorentzAngleDepESProducer = cms.ESProducer("SiStripLorentzAngleDepESProducer",
    LatencyRecord = cms.PSet(
        record = cms.string('SiStripLatencyRcd'),
        label = cms.untracked.string('')
    ),
    LorentzAngleDeconvMode = cms.PSet(
        record = cms.string('SiStripLorentzAngleRcd'),
        label = cms.untracked.string('deconvolution')
    ),
    LorentzAnglePeakMode = cms.PSet(
        record = cms.string('SiStripLorentzAngleRcd'),
        label = cms.untracked.string('peak')
    )
)


process.siStripQualityESProducer = cms.ESProducer("SiStripQualityESProducer",
    appendToDataLabel = cms.string(''),
    PrintDebugOutput = cms.bool(False),
    ThresholdForReducedGranularity = cms.double(0.3),
    UseEmptyRunInfo = cms.bool(False),
    ReduceGranularity = cms.bool(False),
    ListOfRecordToMerge = cms.VPSet(cms.PSet(
        record = cms.string('SiStripDetVOffRcd'),
        tag = cms.string('')
    ), 
        cms.PSet(
            record = cms.string('SiStripDetCablingRcd'),
            tag = cms.string('')
        ), 
        cms.PSet(
            record = cms.string('RunInfoRcd'),
            tag = cms.string('')
        ), 
        cms.PSet(
            record = cms.string('SiStripBadChannelRcd'),
            tag = cms.string('')
        ), 
        cms.PSet(
            record = cms.string('SiStripBadFiberRcd'),
            tag = cms.string('')
        ), 
        cms.PSet(
            record = cms.string('SiStripBadModuleRcd'),
            tag = cms.string('')
        ), 
        cms.PSet(
            record = cms.string('SiStripBadStripRcd'),
            tag = cms.string('')
        ))
)


process.simpleSecondaryVertex2Trk = cms.ESProducer("SimpleSecondaryVertexESProducer",
    minTracks = cms.uint32(2),
    unBoost = cms.bool(False),
    useSignificance = cms.bool(True),
    use3d = cms.bool(True)
)


process.sistripconn = cms.ESProducer("SiStripConnectivity")


process.BTagRecord = cms.ESSource("EmptyESSource",
    iovIsRunNotTime = cms.bool(True),
    recordName = cms.string('JetTagComputerRecord'),
    firstValid = cms.vuint32(1)
)


process.GlobalTag = cms.ESSource("PoolDBESSource",
    DBParameters = cms.PSet(
        authenticationPath = cms.untracked.string(''),
        enableReadOnlySessionOnUpdateConnection = cms.untracked.bool(False),
        idleConnectionCleanupPeriod = cms.untracked.int32(10),
        messageLevel = cms.untracked.int32(0),
        enablePoolAutomaticCleanUp = cms.untracked.bool(False),
        enableConnectionSharing = cms.untracked.bool(True),
        connectionRetrialTimeOut = cms.untracked.int32(60),
        connectionTimeOut = cms.untracked.int32(60),
        authenticationSystem = cms.untracked.int32(0),
        connectionRetrialPeriod = cms.untracked.int32(10)
    ),
    BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService'),
    toGet = cms.VPSet(),
    connect = cms.string('frontier://FrontierProd/CMS_COND_31X_GLOBALTAG'),
    globaltag = cms.string('GR_P_V41_AN2::All')
)


process.XMLIdealGeometryESSource = cms.ESSource("XMLIdealGeometryESSource",
    geomXMLFiles = cms.vstring('Geometry/CMSCommonData/data/materials.xml', 
        'Geometry/CMSCommonData/data/rotations.xml', 
        'Geometry/CMSCommonData/data/normal/cmsextent.xml', 
        'Geometry/CMSCommonData/data/cms.xml', 
        'Geometry/CMSCommonData/data/cmsMother.xml', 
        'Geometry/CMSCommonData/data/cmsTracker.xml', 
        'Geometry/CMSCommonData/data/caloBase.xml', 
        'Geometry/CMSCommonData/data/cmsCalo.xml', 
        'Geometry/CMSCommonData/data/muonBase.xml', 
        'Geometry/CMSCommonData/data/cmsMuon.xml', 
        'Geometry/CMSCommonData/data/mgnt.xml', 
        'Geometry/CMSCommonData/data/beampipe.xml', 
        'Geometry/CMSCommonData/data/cmsBeam.xml', 
        'Geometry/CMSCommonData/data/muonMB.xml', 
        'Geometry/CMSCommonData/data/muonMagnet.xml', 
        'Geometry/TrackerCommonData/data/pixfwdMaterials.xml', 
        'Geometry/TrackerCommonData/data/pixfwdCommon.xml', 
        'Geometry/TrackerCommonData/data/pixfwdPlaq.xml', 
        'Geometry/TrackerCommonData/data/pixfwdPlaq1x2.xml', 
        'Geometry/TrackerCommonData/data/pixfwdPlaq1x5.xml', 
        'Geometry/TrackerCommonData/data/pixfwdPlaq2x3.xml', 
        'Geometry/TrackerCommonData/data/pixfwdPlaq2x4.xml', 
        'Geometry/TrackerCommonData/data/pixfwdPlaq2x5.xml', 
        'Geometry/TrackerCommonData/data/pixfwdPanelBase.xml', 
        'Geometry/TrackerCommonData/data/pixfwdPanel.xml', 
        'Geometry/TrackerCommonData/data/pixfwdBlade.xml', 
        'Geometry/TrackerCommonData/data/pixfwdNipple.xml', 
        'Geometry/TrackerCommonData/data/pixfwdDisk.xml', 
        'Geometry/TrackerCommonData/data/pixfwdCylinder.xml', 
        'Geometry/TrackerCommonData/data/pixfwd.xml', 
        'Geometry/TrackerCommonData/data/pixbarmaterial.xml', 
        'Geometry/TrackerCommonData/data/pixbarladder.xml', 
        'Geometry/TrackerCommonData/data/pixbarladderfull.xml', 
        'Geometry/TrackerCommonData/data/pixbarladderhalf.xml', 
        'Geometry/TrackerCommonData/data/pixbarlayer.xml', 
        'Geometry/TrackerCommonData/data/pixbarlayer0.xml', 
        'Geometry/TrackerCommonData/data/pixbarlayer1.xml', 
        'Geometry/TrackerCommonData/data/pixbarlayer2.xml', 
        'Geometry/TrackerCommonData/data/pixbar.xml', 
        'Geometry/TrackerCommonData/data/tibtidcommonmaterial.xml', 
        'Geometry/TrackerCommonData/data/tibmaterial.xml', 
        'Geometry/TrackerCommonData/data/tibmodpar.xml', 
        'Geometry/TrackerCommonData/data/tibmodule0.xml', 
        'Geometry/TrackerCommonData/data/tibmodule0a.xml', 
        'Geometry/TrackerCommonData/data/tibmodule0b.xml', 
        'Geometry/TrackerCommonData/data/tibmodule2.xml', 
        'Geometry/TrackerCommonData/data/tibstringpar.xml', 
        'Geometry/TrackerCommonData/data/tibstring0ll.xml', 
        'Geometry/TrackerCommonData/data/tibstring0lr.xml', 
        'Geometry/TrackerCommonData/data/tibstring0ul.xml', 
        'Geometry/TrackerCommonData/data/tibstring0ur.xml', 
        'Geometry/TrackerCommonData/data/tibstring0.xml', 
        'Geometry/TrackerCommonData/data/tibstring1ll.xml', 
        'Geometry/TrackerCommonData/data/tibstring1lr.xml', 
        'Geometry/TrackerCommonData/data/tibstring1ul.xml', 
        'Geometry/TrackerCommonData/data/tibstring1ur.xml', 
        'Geometry/TrackerCommonData/data/tibstring1.xml', 
        'Geometry/TrackerCommonData/data/tibstring2ll.xml', 
        'Geometry/TrackerCommonData/data/tibstring2lr.xml', 
        'Geometry/TrackerCommonData/data/tibstring2ul.xml', 
        'Geometry/TrackerCommonData/data/tibstring2ur.xml', 
        'Geometry/TrackerCommonData/data/tibstring2.xml', 
        'Geometry/TrackerCommonData/data/tibstring3ll.xml', 
        'Geometry/TrackerCommonData/data/tibstring3lr.xml', 
        'Geometry/TrackerCommonData/data/tibstring3ul.xml', 
        'Geometry/TrackerCommonData/data/tibstring3ur.xml', 
        'Geometry/TrackerCommonData/data/tibstring3.xml', 
        'Geometry/TrackerCommonData/data/tiblayerpar.xml', 
        'Geometry/TrackerCommonData/data/tiblayer0.xml', 
        'Geometry/TrackerCommonData/data/tiblayer1.xml', 
        'Geometry/TrackerCommonData/data/tiblayer2.xml', 
        'Geometry/TrackerCommonData/data/tiblayer3.xml', 
        'Geometry/TrackerCommonData/data/tib.xml', 
        'Geometry/TrackerCommonData/data/tidmaterial.xml', 
        'Geometry/TrackerCommonData/data/tidmodpar.xml', 
        'Geometry/TrackerCommonData/data/tidmodule0.xml', 
        'Geometry/TrackerCommonData/data/tidmodule0r.xml', 
        'Geometry/TrackerCommonData/data/tidmodule0l.xml', 
        'Geometry/TrackerCommonData/data/tidmodule1.xml', 
        'Geometry/TrackerCommonData/data/tidmodule1r.xml', 
        'Geometry/TrackerCommonData/data/tidmodule1l.xml', 
        'Geometry/TrackerCommonData/data/tidmodule2.xml', 
        'Geometry/TrackerCommonData/data/tidringpar.xml', 
        'Geometry/TrackerCommonData/data/tidring0.xml', 
        'Geometry/TrackerCommonData/data/tidring0f.xml', 
        'Geometry/TrackerCommonData/data/tidring0b.xml', 
        'Geometry/TrackerCommonData/data/tidring1.xml', 
        'Geometry/TrackerCommonData/data/tidring1f.xml', 
        'Geometry/TrackerCommonData/data/tidring1b.xml', 
        'Geometry/TrackerCommonData/data/tidring2.xml', 
        'Geometry/TrackerCommonData/data/tid.xml', 
        'Geometry/TrackerCommonData/data/tidf.xml', 
        'Geometry/TrackerCommonData/data/tidb.xml', 
        'Geometry/TrackerCommonData/data/tibtidservices.xml', 
        'Geometry/TrackerCommonData/data/tibtidservicesf.xml', 
        'Geometry/TrackerCommonData/data/tibtidservicesb.xml', 
        'Geometry/TrackerCommonData/data/tobmaterial.xml', 
        'Geometry/TrackerCommonData/data/tobmodpar.xml', 
        'Geometry/TrackerCommonData/data/tobmodule0.xml', 
        'Geometry/TrackerCommonData/data/tobmodule2.xml', 
        'Geometry/TrackerCommonData/data/tobmodule4.xml', 
        'Geometry/TrackerCommonData/data/tobrodpar.xml', 
        'Geometry/TrackerCommonData/data/tobrod0c.xml', 
        'Geometry/TrackerCommonData/data/tobrod0l.xml', 
        'Geometry/TrackerCommonData/data/tobrod0h.xml', 
        'Geometry/TrackerCommonData/data/tobrod0.xml', 
        'Geometry/TrackerCommonData/data/tobrod1l.xml', 
        'Geometry/TrackerCommonData/data/tobrod1h.xml', 
        'Geometry/TrackerCommonData/data/tobrod1.xml', 
        'Geometry/TrackerCommonData/data/tobrod2c.xml', 
        'Geometry/TrackerCommonData/data/tobrod2l.xml', 
        'Geometry/TrackerCommonData/data/tobrod2h.xml', 
        'Geometry/TrackerCommonData/data/tobrod2.xml', 
        'Geometry/TrackerCommonData/data/tobrod3l.xml', 
        'Geometry/TrackerCommonData/data/tobrod3h.xml', 
        'Geometry/TrackerCommonData/data/tobrod3.xml', 
        'Geometry/TrackerCommonData/data/tobrod4c.xml', 
        'Geometry/TrackerCommonData/data/tobrod4l.xml', 
        'Geometry/TrackerCommonData/data/tobrod4h.xml', 
        'Geometry/TrackerCommonData/data/tobrod4.xml', 
        'Geometry/TrackerCommonData/data/tobrod5l.xml', 
        'Geometry/TrackerCommonData/data/tobrod5h.xml', 
        'Geometry/TrackerCommonData/data/tobrod5.xml', 
        'Geometry/TrackerCommonData/data/tob.xml', 
        'Geometry/TrackerCommonData/data/tecmaterial.xml', 
        'Geometry/TrackerCommonData/data/tecmodpar.xml', 
        'Geometry/TrackerCommonData/data/tecmodule0.xml', 
        'Geometry/TrackerCommonData/data/tecmodule0r.xml', 
        'Geometry/TrackerCommonData/data/tecmodule0s.xml', 
        'Geometry/TrackerCommonData/data/tecmodule1.xml', 
        'Geometry/TrackerCommonData/data/tecmodule1r.xml', 
        'Geometry/TrackerCommonData/data/tecmodule1s.xml', 
        'Geometry/TrackerCommonData/data/tecmodule2.xml', 
        'Geometry/TrackerCommonData/data/tecmodule3.xml', 
        'Geometry/TrackerCommonData/data/tecmodule4.xml', 
        'Geometry/TrackerCommonData/data/tecmodule4r.xml', 
        'Geometry/TrackerCommonData/data/tecmodule4s.xml', 
        'Geometry/TrackerCommonData/data/tecmodule5.xml', 
        'Geometry/TrackerCommonData/data/tecmodule6.xml', 
        'Geometry/TrackerCommonData/data/tecpetpar.xml', 
        'Geometry/TrackerCommonData/data/tecring0.xml', 
        'Geometry/TrackerCommonData/data/tecring1.xml', 
        'Geometry/TrackerCommonData/data/tecring2.xml', 
        'Geometry/TrackerCommonData/data/tecring3.xml', 
        'Geometry/TrackerCommonData/data/tecring4.xml', 
        'Geometry/TrackerCommonData/data/tecring5.xml', 
        'Geometry/TrackerCommonData/data/tecring6.xml', 
        'Geometry/TrackerCommonData/data/tecring0f.xml', 
        'Geometry/TrackerCommonData/data/tecring1f.xml', 
        'Geometry/TrackerCommonData/data/tecring2f.xml', 
        'Geometry/TrackerCommonData/data/tecring3f.xml', 
        'Geometry/TrackerCommonData/data/tecring4f.xml', 
        'Geometry/TrackerCommonData/data/tecring5f.xml', 
        'Geometry/TrackerCommonData/data/tecring6f.xml', 
        'Geometry/TrackerCommonData/data/tecring0b.xml', 
        'Geometry/TrackerCommonData/data/tecring1b.xml', 
        'Geometry/TrackerCommonData/data/tecring2b.xml', 
        'Geometry/TrackerCommonData/data/tecring3b.xml', 
        'Geometry/TrackerCommonData/data/tecring4b.xml', 
        'Geometry/TrackerCommonData/data/tecring5b.xml', 
        'Geometry/TrackerCommonData/data/tecring6b.xml', 
        'Geometry/TrackerCommonData/data/tecpetalf.xml', 
        'Geometry/TrackerCommonData/data/tecpetalb.xml', 
        'Geometry/TrackerCommonData/data/tecpetal0.xml', 
        'Geometry/TrackerCommonData/data/tecpetal0f.xml', 
        'Geometry/TrackerCommonData/data/tecpetal0b.xml', 
        'Geometry/TrackerCommonData/data/tecpetal3.xml', 
        'Geometry/TrackerCommonData/data/tecpetal3f.xml', 
        'Geometry/TrackerCommonData/data/tecpetal3b.xml', 
        'Geometry/TrackerCommonData/data/tecpetal6f.xml', 
        'Geometry/TrackerCommonData/data/tecpetal6b.xml', 
        'Geometry/TrackerCommonData/data/tecpetal8f.xml', 
        'Geometry/TrackerCommonData/data/tecpetal8b.xml', 
        'Geometry/TrackerCommonData/data/tecwheel.xml', 
        'Geometry/TrackerCommonData/data/tecwheela.xml', 
        'Geometry/TrackerCommonData/data/tecwheelb.xml', 
        'Geometry/TrackerCommonData/data/tecwheelc.xml', 
        'Geometry/TrackerCommonData/data/tecwheeld.xml', 
        'Geometry/TrackerCommonData/data/tecwheel6.xml', 
        'Geometry/TrackerCommonData/data/tecservices.xml', 
        'Geometry/TrackerCommonData/data/tecbackplate.xml', 
        'Geometry/TrackerCommonData/data/tec.xml', 
        'Geometry/TrackerCommonData/data/trackermaterial.xml', 
        'Geometry/TrackerCommonData/data/tracker.xml', 
        'Geometry/TrackerCommonData/data/trackerpixbar.xml', 
        'Geometry/TrackerCommonData/data/trackerpixfwd.xml', 
        'Geometry/TrackerCommonData/data/trackertibtidservices.xml', 
        'Geometry/TrackerCommonData/data/trackertib.xml', 
        'Geometry/TrackerCommonData/data/trackertid.xml', 
        'Geometry/TrackerCommonData/data/trackertob.xml', 
        'Geometry/TrackerCommonData/data/trackertec.xml', 
        'Geometry/TrackerCommonData/data/trackerbulkhead.xml', 
        'Geometry/TrackerCommonData/data/trackerother.xml', 
        'Geometry/EcalCommonData/data/eregalgo.xml', 
        'Geometry/EcalCommonData/data/ebalgo.xml', 
        'Geometry/EcalCommonData/data/ebcon.xml', 
        'Geometry/EcalCommonData/data/ebrot.xml', 
        'Geometry/EcalCommonData/data/eecon.xml', 
        'Geometry/EcalCommonData/data/eefixed.xml', 
        'Geometry/EcalCommonData/data/eehier.xml', 
        'Geometry/EcalCommonData/data/eealgo.xml', 
        'Geometry/EcalCommonData/data/escon.xml', 
        'Geometry/EcalCommonData/data/esalgo.xml', 
        'Geometry/EcalCommonData/data/eeF.xml', 
        'Geometry/EcalCommonData/data/eeB.xml', 
        'Geometry/HcalCommonData/data/hcalrotations.xml', 
        'Geometry/HcalCommonData/data/hcalalgo.xml', 
        'Geometry/HcalCommonData/data/hcalbarrelalgo.xml', 
        'Geometry/HcalCommonData/data/hcalendcapalgo.xml', 
        'Geometry/HcalCommonData/data/hcalouteralgo.xml', 
        'Geometry/HcalCommonData/data/hcalforwardalgo.xml', 
        'Geometry/HcalCommonData/data/average/hcalforwardmaterial.xml', 
        'Geometry/MuonCommonData/data/mbCommon.xml', 
        'Geometry/MuonCommonData/data/mb1.xml', 
        'Geometry/MuonCommonData/data/mb2.xml', 
        'Geometry/MuonCommonData/data/mb3.xml', 
        'Geometry/MuonCommonData/data/mb4.xml', 
        'Geometry/MuonCommonData/data/muonYoke.xml', 
        'Geometry/MuonCommonData/data/mf.xml', 
        'Geometry/ForwardCommonData/data/forward.xml', 
        'Geometry/ForwardCommonData/data/bundle/forwardshield.xml', 
        'Geometry/ForwardCommonData/data/brmrotations.xml', 
        'Geometry/ForwardCommonData/data/brm.xml', 
        'Geometry/ForwardCommonData/data/totemMaterials.xml', 
        'Geometry/ForwardCommonData/data/totemRotations.xml', 
        'Geometry/ForwardCommonData/data/totemt1.xml', 
        'Geometry/ForwardCommonData/data/totemt2.xml', 
        'Geometry/ForwardCommonData/data/ionpump.xml', 
        'Geometry/MuonCommonData/data/muonNumbering.xml', 
        'Geometry/TrackerCommonData/data/trackerStructureTopology.xml', 
        'Geometry/TrackerSimData/data/trackersens.xml', 
        'Geometry/TrackerRecoData/data/trackerRecoMaterial.xml', 
        'Geometry/EcalSimData/data/ecalsens.xml', 
        'Geometry/HcalCommonData/data/hcalsenspmf.xml', 
        'Geometry/HcalSimData/data/hf.xml', 
        'Geometry/HcalSimData/data/hfpmt.xml', 
        'Geometry/HcalSimData/data/hffibrebundle.xml', 
        'Geometry/HcalSimData/data/CaloUtil.xml', 
        'Geometry/MuonSimData/data/muonSens.xml', 
        'Geometry/DTGeometryBuilder/data/dtSpecsFilter.xml', 
        'Geometry/CSCGeometryBuilder/data/cscSpecsFilter.xml', 
        'Geometry/CSCGeometryBuilder/data/cscSpecs.xml', 
        'Geometry/RPCGeometryBuilder/data/RPCSpecs.xml', 
        'Geometry/ForwardCommonData/data/brmsens.xml', 
        'Geometry/HcalSimData/data/HcalProdCuts.xml', 
        'Geometry/EcalSimData/data/EcalProdCuts.xml', 
        'Geometry/EcalSimData/data/ESProdCuts.xml', 
        'Geometry/TrackerSimData/data/trackerProdCuts.xml', 
        'Geometry/TrackerSimData/data/trackerProdCutsBEAM.xml', 
        'Geometry/MuonSimData/data/muonProdCuts.xml', 
        'Geometry/ForwardSimData/data/ForwardShieldProdCuts.xml', 
        'Geometry/CMSCommonData/data/FieldParameters.xml'),
    rootNodeName = cms.string('cms:OCMS')
)


process.eegeom = cms.ESSource("EmptyESSource",
    iovIsRunNotTime = cms.bool(True),
    recordName = cms.string('EcalMappingRcd'),
    firstValid = cms.vuint32(1)
)


process.es_hardcode = cms.ESSource("HcalHardcodeCalibrations",
    toGet = cms.untracked.vstring('GainWidths')
)


process.magfield = cms.ESSource("XMLIdealGeometryESSource",
    geomXMLFiles = cms.vstring('Geometry/CMSCommonData/data/normal/cmsextent.xml', 
        'Geometry/CMSCommonData/data/cms.xml', 
        'Geometry/CMSCommonData/data/cmsMagneticField.xml', 
        'MagneticField/GeomBuilder/data/MagneticFieldVolumes_1103l.xml', 
        'MagneticField/GeomBuilder/data/MagneticFieldParameters_07_2pi.xml', 
        'Geometry/CMSCommonData/data/materials.xml'),
    rootNodeName = cms.string('cmsMagneticField:MAGF')
)


process.prefer("magfield")

process.CondDBSetup = cms.PSet(
    DBParameters = cms.PSet(
        authenticationPath = cms.untracked.string(''),
        enableReadOnlySessionOnUpdateConnection = cms.untracked.bool(False),
        idleConnectionCleanupPeriod = cms.untracked.int32(10),
        messageLevel = cms.untracked.int32(0),
        enablePoolAutomaticCleanUp = cms.untracked.bool(False),
        enableConnectionSharing = cms.untracked.bool(True),
        connectionRetrialTimeOut = cms.untracked.int32(60),
        connectionTimeOut = cms.untracked.int32(60),
        authenticationSystem = cms.untracked.int32(0),
        connectionRetrialPeriod = cms.untracked.int32(10)
    )
)

process.TrackAssociatorParameterBlock = cms.PSet(
    TrackAssociatorParameters = cms.PSet(
        muonMaxDistanceSigmaX = cms.double(0.0),
        muonMaxDistanceSigmaY = cms.double(0.0),
        CSCSegmentCollectionLabel = cms.InputTag("cscSegments"),
        dRHcal = cms.double(9999.0),
        dRPreshowerPreselection = cms.double(0.2),
        CaloTowerCollectionLabel = cms.InputTag("towerMaker"),
        useEcal = cms.bool(True),
        dREcal = cms.double(9999.0),
        dREcalPreselection = cms.double(0.05),
        HORecHitCollectionLabel = cms.InputTag("horeco"),
        dRMuon = cms.double(9999.0),
        propagateAllDirections = cms.bool(True),
        muonMaxDistanceX = cms.double(5.0),
        muonMaxDistanceY = cms.double(5.0),
        useHO = cms.bool(True),
        trajectoryUncertaintyTolerance = cms.double(-1.0),
        usePreshower = cms.bool(False),
        DTRecSegment4DCollectionLabel = cms.InputTag("dt4DSegments"),
        EERecHitCollectionLabel = cms.InputTag("ecalRecHit","EcalRecHitsEE"),
        dRHcalPreselection = cms.double(0.2),
        useMuon = cms.bool(True),
        useCalo = cms.bool(False),
        accountForTrajectoryChangeCalo = cms.bool(False),
        EBRecHitCollectionLabel = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
        dRMuonPreselection = cms.double(0.2),
        truthMatch = cms.bool(False),
        HBHERecHitCollectionLabel = cms.InputTag("hbhereco"),
        useHcal = cms.bool(True)
    )
)

process.TrackAssociatorParameters = cms.PSet(
    muonMaxDistanceSigmaX = cms.double(0.0),
    muonMaxDistanceSigmaY = cms.double(0.0),
    CSCSegmentCollectionLabel = cms.InputTag("cscSegments"),
    dRHcal = cms.double(9999.0),
    dREcal = cms.double(9999.0),
    CaloTowerCollectionLabel = cms.InputTag("towerMaker"),
    useEcal = cms.bool(True),
    dREcalPreselection = cms.double(0.05),
    HORecHitCollectionLabel = cms.InputTag("horeco"),
    dRMuon = cms.double(9999.0),
    propagateAllDirections = cms.bool(True),
    muonMaxDistanceX = cms.double(5.0),
    muonMaxDistanceY = cms.double(5.0),
    useHO = cms.bool(True),
    trajectoryUncertaintyTolerance = cms.double(-1.0),
    usePreshower = cms.bool(False),
    DTRecSegment4DCollectionLabel = cms.InputTag("dt4DSegments"),
    EERecHitCollectionLabel = cms.InputTag("ecalRecHit","EcalRecHitsEE"),
    dRHcalPreselection = cms.double(0.2),
    useMuon = cms.bool(True),
    useCalo = cms.bool(False),
    accountForTrajectoryChangeCalo = cms.bool(False),
    EBRecHitCollectionLabel = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
    dRMuonPreselection = cms.double(0.2),
    truthMatch = cms.bool(False),
    HBHERecHitCollectionLabel = cms.InputTag("hbhereco"),
    useHcal = cms.bool(True)
)

process.cutbased = cms.PSet(
    cutBased = cms.bool(True),
    JetIdParams = cms.PSet(
        Pt010_RMSLoose = cms.vdouble(0.08, 0.07, 0.05, 0.07),
        Pt3050_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
        Pt3050_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
        Pt010_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
        Pt1020_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
        Pt010_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
        Pt1020_RMSLoose = cms.vdouble(0.08, 0.07, 0.05, 0.07),
        Pt1020_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
        Pt2030_RMSTight = cms.vdouble(0.05, 0.07, 0.03, 0.045),
        Pt3050_RMSTight = cms.vdouble(0.05, 0.06, 0.03, 0.04),
        Pt1020_RMSTight = cms.vdouble(0.06, 0.07, 0.04, 0.05),
        Pt3050_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
        Pt3050_RMSLoose = cms.vdouble(0.07, 0.06, 0.04, 0.05),
        Pt2030_RMSLoose = cms.vdouble(0.08, 0.07, 0.04, 0.055),
        Pt010_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
        Pt2030_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
        Pt1020_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
        Pt2030_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
        Pt2030_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
        Pt2030_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
        Pt010_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
        Pt3050_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
        Pt1020_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
        Pt010_RMSTight = cms.vdouble(0.06, 0.07, 0.04, 0.05)
    ),
    impactParTkThreshold = cms.double(1.0),
    label = cms.string('cutbased')
)

process.fieldScaling = cms.PSet(
    scalingVolumes = cms.vint32(14100, 14200, 17600, 17800, 17900, 
        18100, 18300, 18400, 18600, 23100, 
        23300, 23400, 23600, 23800, 23900, 
        24100, 28600, 28800, 28900, 29100, 
        29300, 29400, 29600, 28609, 28809, 
        28909, 29109, 29309, 29409, 29609, 
        28610, 28810, 28910, 29110, 29310, 
        29410, 29610, 28611, 28811, 28911, 
        29111, 29311, 29411, 29611),
    scalingFactors = cms.vdouble(1, 1, 0.994, 1.004, 1.004, 
        1.005, 1.004, 1.004, 0.994, 0.965, 
        0.958, 0.958, 0.953, 0.958, 0.958, 
        0.965, 0.918, 0.924, 0.924, 0.906, 
        0.924, 0.924, 0.918, 0.991, 0.998, 
        0.998, 0.978, 0.998, 0.998, 0.991, 
        0.991, 0.998, 0.998, 0.978, 0.998, 
        0.998, 0.991, 0.991, 0.998, 0.998, 
        0.978, 0.998, 0.998, 0.991)
)

process.j2tParametersCALO = cms.PSet(
    trackQuality = cms.string('goodIterative'),
    tracks = cms.InputTag("generalTracks"),
    coneSize = cms.double(0.5),
    extrapolations = cms.InputTag("trackExtrapolator")
)

process.j2tParametersVX = cms.PSet(
    tracks = cms.InputTag("generalTracks"),
    useAssigned = cms.bool(False),
    coneSize = cms.double(0.5),
    pvSrc = cms.InputTag("offlinePrimaryVertices")
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

process.vertexCutsBlock = cms.PSet(
    vertexCuts = cms.PSet(
        distSig3dMax = cms.double(99999.9),
        fracPV = cms.double(0.65),
        distVal2dMax = cms.double(2.5),
        useTrackWeights = cms.bool(True),
        maxDeltaRToJetAxis = cms.double(0.5),
        v0Filter = cms.PSet(
            k0sMassWindow = cms.double(0.05)
        ),
        distSig2dMin = cms.double(3.0),
        multiplicityMin = cms.uint32(2),
        massMax = cms.double(6.5),
        distSig2dMax = cms.double(99999.9),
        distVal3dMax = cms.double(99999.9),
        minimumTrackWeight = cms.double(0.5),
        distVal3dMin = cms.double(-99999.9),
        distVal2dMin = cms.double(0.01),
        distSig3dMin = cms.double(-99999.9)
    )
)

process.vertexRecoBlock = cms.PSet(
    vertexReco = cms.PSet(
        seccut = cms.double(6.0),
        primcut = cms.double(1.8),
        smoothing = cms.bool(False),
        weightthreshold = cms.double(0.001),
        minweight = cms.double(0.5),
        finder = cms.string('avr')
    )
)

process.vertexSelectionBlock = cms.PSet(
    vertexSelection = cms.PSet(
        sortCriterium = cms.string('dist3dError')
    )
)

process.vertexTrackSelectionBlock = cms.PSet(
    trackSelection = cms.PSet(
        totalHitsMin = cms.uint32(8),
        jetDeltaRMax = cms.double(0.3),
        qualityClass = cms.string('highPurity'),
        pixelHitsMin = cms.uint32(2),
        sip3dSigMin = cms.double(-99999.9),
        sip3dSigMax = cms.double(99999.9),
        maxDistToAxis = cms.double(0.2),
        sip2dValMax = cms.double(99999.9),
        maxDecayLen = cms.double(99999.9),
        ptMin = cms.double(1.0),
        sip2dSigMax = cms.double(99999.9),
        sip2dSigMin = cms.double(-99999.9),
        sip3dValMax = cms.double(99999.9),
        sip3dValMin = cms.double(-99999.9),
        sip2dValMin = cms.double(-99999.9),
        normChi2Max = cms.double(99999.9)
    )
)

process.chsalgos = cms.VPSet(cms.PSet(
    tmvaVariables = cms.vstring('frac01', 
        'frac02', 
        'frac03', 
        'frac04', 
        'frac05', 
        'nvtx', 
        'beta', 
        'betaStar'),
    tmvaMethod = cms.string('BDT_chsSimpleNoVtxCat'),
    cutBased = cms.bool(False),
    tmvaWeights = cms.string('CMGTools/External/data/TMVAClassification_5x_BDT_chsSimpleNoVtxCat.weights.xml'),
    tmvaSpectators = cms.vstring('jetPt', 
        'jetEta'),
    label = cms.string('simple'),
    version = cms.int32(-1),
    JetIdParams = cms.PSet(
        Pt2030_Tight = cms.vdouble(-0.47, -0.06, -0.23, -0.47),
        Pt2030_Loose = cms.vdouble(-0.89, -0.75, -0.72, -0.75),
        Pt3050_Medium = cms.vdouble(-0.59, -0.65, -0.56, -0.68),
        Pt1020_Tight = cms.vdouble(-0.6, -0.74, -0.78, -0.81),
        Pt2030_Medium = cms.vdouble(-0.59, -0.65, -0.56, -0.68),
        Pt010_Tight = cms.vdouble(-0.6, -0.74, -0.78, -0.81),
        Pt1020_Loose = cms.vdouble(-0.98, -0.96, -0.94, -0.94),
        Pt010_Medium = cms.vdouble(-0.95, -0.94, -0.92, -0.91),
        Pt1020_Medium = cms.vdouble(-0.95, -0.94, -0.92, -0.91),
        Pt010_Loose = cms.vdouble(-0.98, -0.96, -0.94, -0.94),
        Pt3050_Loose = cms.vdouble(-0.89, -0.75, -0.72, -0.75),
        Pt3050_Tight = cms.vdouble(-0.47, -0.06, -0.23, -0.47)
    ),
    impactParTkThreshold = cms.double(1.0)
), 
    cms.PSet(
        tmvaVariables = cms.vstring('frac01', 
            'frac02', 
            'frac03', 
            'frac04', 
            'frac05', 
            'dR2Mean', 
            'nvtx', 
            'nNeutrals', 
            'beta', 
            'betaStar', 
            'dZ', 
            'nCharged'),
        tmvaMethod = cms.string('BDT_chsFullPlusRMS'),
        cutBased = cms.bool(False),
        tmvaWeights = cms.string('CMGTools/External/data/TMVAClassification_5x_BDT_chsFullPlusRMS.weights.xml'),
        tmvaSpectators = cms.vstring('jetPt', 
            'jetEta'),
        label = cms.string('full'),
        version = cms.int32(-1),
        JetIdParams = cms.PSet(
            Pt2030_Tight = cms.vdouble(0.41, -0.1, -0.2, -0.45),
            Pt2030_Loose = cms.vdouble(-0.89, -0.77, -0.69, -0.75),
            Pt3050_Medium = cms.vdouble(-0.58, -0.65, -0.57, -0.67),
            Pt1020_Tight = cms.vdouble(-0.59, -0.75, -0.78, -0.8),
            Pt2030_Medium = cms.vdouble(-0.58, -0.65, -0.57, -0.67),
            Pt010_Tight = cms.vdouble(-0.59, -0.75, -0.78, -0.8),
            Pt1020_Loose = cms.vdouble(-0.98, -0.95, -0.94, -0.94),
            Pt010_Medium = cms.vdouble(-0.94, -0.91, -0.91, -0.92),
            Pt1020_Medium = cms.vdouble(-0.94, -0.91, -0.91, -0.92),
            Pt010_Loose = cms.vdouble(-0.98, -0.95, -0.94, -0.94),
            Pt3050_Loose = cms.vdouble(-0.89, -0.77, -0.69, -0.57),
            Pt3050_Tight = cms.vdouble(0.41, -0.1, -0.2, -0.45)
        ),
        impactParTkThreshold = cms.double(1.0)
    ), 
    cms.PSet(
        cutBased = cms.bool(True),
        JetIdParams = cms.PSet(
            Pt010_RMSLoose = cms.vdouble(0.08, 0.07, 0.05, 0.07),
            Pt3050_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
            Pt3050_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
            Pt010_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
            Pt1020_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
            Pt010_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
            Pt1020_RMSLoose = cms.vdouble(0.08, 0.07, 0.05, 0.07),
            Pt1020_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
            Pt2030_RMSTight = cms.vdouble(0.05, 0.07, 0.03, 0.045),
            Pt3050_RMSTight = cms.vdouble(0.05, 0.06, 0.03, 0.04),
            Pt1020_RMSTight = cms.vdouble(0.06, 0.07, 0.04, 0.05),
            Pt3050_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
            Pt3050_RMSLoose = cms.vdouble(0.07, 0.06, 0.04, 0.05),
            Pt2030_RMSLoose = cms.vdouble(0.08, 0.07, 0.04, 0.055),
            Pt010_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
            Pt2030_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
            Pt1020_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
            Pt2030_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
            Pt2030_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
            Pt2030_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
            Pt010_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
            Pt3050_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
            Pt1020_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
            Pt010_RMSTight = cms.vdouble(0.06, 0.07, 0.04, 0.05)
        ),
        impactParTkThreshold = cms.double(1.0),
        label = cms.string('cutbased')
    ))

process.chsalgos_4x = cms.VPSet(cms.PSet(
    tmvaVariables = cms.vstring('frac01', 
        'frac02', 
        'frac03', 
        'frac04', 
        'frac05', 
        'beta', 
        'betaStar'),
    tmvaMethod = cms.string('PuJetIdMinMVA'),
    cutBased = cms.bool(False),
    tmvaWeights = cms.string('CMGTools/External/data/TMVAClassification_PuJetIdMinMVA.weights.xml'),
    tmvaSpectators = cms.vstring('nvtx', 
        'jetPt', 
        'jetEta'),
    label = cms.string('simple'),
    version = cms.int32(-1),
    JetIdParams = cms.PSet(
        Pt2030_Tight = cms.vdouble(-0.2, 0.0, 0.0, 0.0),
        Pt2030_Loose = cms.vdouble(-0.4, -0.85, -0.7, -0.6),
        Pt3050_Medium = cms.vdouble(0.1, -0.4, -0.5, -0.45),
        Pt1020_Tight = cms.vdouble(-0.5, -0.2, -0.83, -0.7),
        Pt2030_Medium = cms.vdouble(0.1, -0.4, -0.5, -0.45),
        Pt010_Tight = cms.vdouble(-0.5, -0.2, -0.83, -0.7),
        Pt1020_Loose = cms.vdouble(-0.9, -0.9, -0.94, -0.9),
        Pt010_Medium = cms.vdouble(-0.73, -0.89, -0.89, -0.83),
        Pt1020_Medium = cms.vdouble(-0.73, -0.89, -0.89, -0.83),
        Pt010_Loose = cms.vdouble(-0.9, -0.9, -0.94, -0.9),
        Pt3050_Loose = cms.vdouble(-0.4, -0.85, -0.7, -0.6),
        Pt3050_Tight = cms.vdouble(-0.2, 0.0, 0.0, 0.0)
    ),
    impactParTkThreshold = cms.double(1.0)
), 
    cms.PSet(
        tmvaVariables = cms.vstring('frac01', 
            'frac02', 
            'frac03', 
            'frac04', 
            'frac05', 
            'nvtx', 
            'nNeutrals', 
            'beta', 
            'betaStar', 
            'dZ', 
            'nCharged'),
        tmvaMethod = cms.string('PuJetIdOptMVA'),
        cutBased = cms.bool(False),
        tmvaWeights = cms.string('CMGTools/External/data/TMVAClassification_PuJetIdOptMVA.weights.xml'),
        tmvaSpectators = cms.vstring('jetPt', 
            'jetEta'),
        label = cms.string('full'),
        version = cms.int32(-1),
        JetIdParams = cms.PSet(
            Pt2030_Tight = cms.vdouble(-0.2, 0.0, 0.0, 0.0),
            Pt2030_Loose = cms.vdouble(-0.4, -0.85, -0.7, -0.6),
            Pt3050_Medium = cms.vdouble(0.1, -0.4, -0.4, -0.45),
            Pt1020_Tight = cms.vdouble(-0.5, -0.2, -0.83, -0.7),
            Pt2030_Medium = cms.vdouble(0.1, -0.4, -0.4, -0.45),
            Pt010_Tight = cms.vdouble(-0.5, -0.2, -0.83, -0.7),
            Pt1020_Loose = cms.vdouble(-0.9, -0.9, -0.9, -0.9),
            Pt010_Medium = cms.vdouble(-0.73, -0.89, -0.89, -0.83),
            Pt1020_Medium = cms.vdouble(-0.73, -0.89, -0.89, -0.83),
            Pt010_Loose = cms.vdouble(-0.9, -0.9, -0.9, -0.9),
            Pt3050_Loose = cms.vdouble(-0.4, -0.85, -0.7, -0.6),
            Pt3050_Tight = cms.vdouble(-0.2, 0.0, 0.0, 0.0)
        ),
        impactParTkThreshold = cms.double(1.0)
    ), 
    cms.PSet(
        cutBased = cms.bool(True),
        JetIdParams = cms.PSet(
            Pt010_RMSLoose = cms.vdouble(0.08, 0.07, 0.05, 0.07),
            Pt3050_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
            Pt3050_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
            Pt010_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
            Pt1020_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
            Pt010_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
            Pt1020_RMSLoose = cms.vdouble(0.08, 0.07, 0.05, 0.07),
            Pt1020_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
            Pt2030_RMSTight = cms.vdouble(0.05, 0.07, 0.03, 0.045),
            Pt3050_RMSTight = cms.vdouble(0.05, 0.06, 0.03, 0.04),
            Pt1020_RMSTight = cms.vdouble(0.06, 0.07, 0.04, 0.05),
            Pt3050_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
            Pt3050_RMSLoose = cms.vdouble(0.07, 0.06, 0.04, 0.05),
            Pt2030_RMSLoose = cms.vdouble(0.08, 0.07, 0.04, 0.055),
            Pt010_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
            Pt2030_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
            Pt1020_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
            Pt2030_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
            Pt2030_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
            Pt2030_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
            Pt010_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
            Pt3050_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
            Pt1020_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
            Pt010_RMSTight = cms.vdouble(0.06, 0.07, 0.04, 0.05)
        ),
        impactParTkThreshold = cms.double(1.0),
        label = cms.string('cutbased')
    ))

process.chsalgos_5x = cms.VPSet(cms.PSet(
    tmvaVariables = cms.vstring('frac01', 
        'frac02', 
        'frac03', 
        'frac04', 
        'frac05', 
        'nvtx', 
        'beta', 
        'betaStar'),
    tmvaMethod = cms.string('BDT_chsSimpleNoVtxCat'),
    cutBased = cms.bool(False),
    tmvaWeights = cms.string('CMGTools/External/data/TMVAClassification_5x_BDT_chsSimpleNoVtxCat.weights.xml'),
    tmvaSpectators = cms.vstring('jetPt', 
        'jetEta'),
    label = cms.string('simple'),
    version = cms.int32(-1),
    JetIdParams = cms.PSet(
        Pt2030_Tight = cms.vdouble(-0.47, -0.06, -0.23, -0.47),
        Pt2030_Loose = cms.vdouble(-0.89, -0.75, -0.72, -0.75),
        Pt3050_Medium = cms.vdouble(-0.59, -0.65, -0.56, -0.68),
        Pt1020_Tight = cms.vdouble(-0.6, -0.74, -0.78, -0.81),
        Pt2030_Medium = cms.vdouble(-0.59, -0.65, -0.56, -0.68),
        Pt010_Tight = cms.vdouble(-0.6, -0.74, -0.78, -0.81),
        Pt1020_Loose = cms.vdouble(-0.98, -0.96, -0.94, -0.94),
        Pt010_Medium = cms.vdouble(-0.95, -0.94, -0.92, -0.91),
        Pt1020_Medium = cms.vdouble(-0.95, -0.94, -0.92, -0.91),
        Pt010_Loose = cms.vdouble(-0.98, -0.96, -0.94, -0.94),
        Pt3050_Loose = cms.vdouble(-0.89, -0.75, -0.72, -0.75),
        Pt3050_Tight = cms.vdouble(-0.47, -0.06, -0.23, -0.47)
    ),
    impactParTkThreshold = cms.double(1.0)
), 
    cms.PSet(
        tmvaVariables = cms.vstring('frac01', 
            'frac02', 
            'frac03', 
            'frac04', 
            'frac05', 
            'dR2Mean', 
            'nvtx', 
            'nNeutrals', 
            'beta', 
            'betaStar', 
            'dZ', 
            'nCharged'),
        tmvaMethod = cms.string('BDT_chsFullPlusRMS'),
        cutBased = cms.bool(False),
        tmvaWeights = cms.string('CMGTools/External/data/TMVAClassification_5x_BDT_chsFullPlusRMS.weights.xml'),
        tmvaSpectators = cms.vstring('jetPt', 
            'jetEta'),
        label = cms.string('full'),
        version = cms.int32(-1),
        JetIdParams = cms.PSet(
            Pt2030_Tight = cms.vdouble(0.41, -0.1, -0.2, -0.45),
            Pt2030_Loose = cms.vdouble(-0.89, -0.77, -0.69, -0.75),
            Pt3050_Medium = cms.vdouble(-0.58, -0.65, -0.57, -0.67),
            Pt1020_Tight = cms.vdouble(-0.59, -0.75, -0.78, -0.8),
            Pt2030_Medium = cms.vdouble(-0.58, -0.65, -0.57, -0.67),
            Pt010_Tight = cms.vdouble(-0.59, -0.75, -0.78, -0.8),
            Pt1020_Loose = cms.vdouble(-0.98, -0.95, -0.94, -0.94),
            Pt010_Medium = cms.vdouble(-0.94, -0.91, -0.91, -0.92),
            Pt1020_Medium = cms.vdouble(-0.94, -0.91, -0.91, -0.92),
            Pt010_Loose = cms.vdouble(-0.98, -0.95, -0.94, -0.94),
            Pt3050_Loose = cms.vdouble(-0.89, -0.77, -0.69, -0.57),
            Pt3050_Tight = cms.vdouble(0.41, -0.1, -0.2, -0.45)
        ),
        impactParTkThreshold = cms.double(1.0)
    ), 
    cms.PSet(
        cutBased = cms.bool(True),
        JetIdParams = cms.PSet(
            Pt010_RMSLoose = cms.vdouble(0.08, 0.07, 0.05, 0.07),
            Pt3050_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
            Pt3050_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
            Pt010_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
            Pt1020_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
            Pt010_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
            Pt1020_RMSLoose = cms.vdouble(0.08, 0.07, 0.05, 0.07),
            Pt1020_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
            Pt2030_RMSTight = cms.vdouble(0.05, 0.07, 0.03, 0.045),
            Pt3050_RMSTight = cms.vdouble(0.05, 0.06, 0.03, 0.04),
            Pt1020_RMSTight = cms.vdouble(0.06, 0.07, 0.04, 0.05),
            Pt3050_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
            Pt3050_RMSLoose = cms.vdouble(0.07, 0.06, 0.04, 0.05),
            Pt2030_RMSLoose = cms.vdouble(0.08, 0.07, 0.04, 0.055),
            Pt010_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
            Pt2030_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
            Pt1020_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
            Pt2030_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
            Pt2030_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
            Pt2030_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
            Pt010_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
            Pt3050_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
            Pt1020_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
            Pt010_RMSTight = cms.vdouble(0.06, 0.07, 0.04, 0.05)
        ),
        impactParTkThreshold = cms.double(1.0),
        label = cms.string('cutbased')
    ))

process.stdalgos = cms.VPSet(cms.PSet(
    tmvaVariables = cms.vstring('frac01', 
        'frac02', 
        'frac03', 
        'frac04', 
        'frac05', 
        'nvtx', 
        'beta', 
        'betaStar'),
    tmvaMethod = cms.string('BDT_simpleNoVtxCat'),
    cutBased = cms.bool(False),
    tmvaWeights = cms.string('CMGTools/External/data/TMVAClassification_5x_BDT_simpleNoVtxCat.weights.xml'),
    tmvaSpectators = cms.vstring('jetPt', 
        'jetEta'),
    label = cms.string('simple'),
    version = cms.int32(-1),
    JetIdParams = cms.PSet(
        Pt2030_Tight = cms.vdouble(-0.33, -0.3, -0.15, -0.49),
        Pt2030_Loose = cms.vdouble(-0.8, -0.73, -0.68, -0.77),
        Pt3050_Medium = cms.vdouble(-0.4, -0.54, -0.5, -0.65),
        Pt1020_Tight = cms.vdouble(-0.84, -0.8, -0.75, -0.82),
        Pt2030_Medium = cms.vdouble(-0.4, -0.54, -0.5, -0.65),
        Pt010_Tight = cms.vdouble(-0.84, -0.8, -0.75, -0.82),
        Pt1020_Loose = cms.vdouble(-0.95, -0.96, -0.94, -0.95),
        Pt010_Medium = cms.vdouble(-0.85, -0.93, -0.91, -0.92),
        Pt1020_Medium = cms.vdouble(-0.85, -0.93, -0.91, -0.92),
        Pt010_Loose = cms.vdouble(-0.95, -0.96, -0.94, -0.95),
        Pt3050_Loose = cms.vdouble(-0.8, -0.73, -0.68, -0.77),
        Pt3050_Tight = cms.vdouble(-0.33, -0.3, -0.15, -0.49)
    ),
    impactParTkThreshold = cms.double(1.0)
), 
    cms.PSet(
        tmvaVariables = cms.vstring('frac01', 
            'frac02', 
            'frac03', 
            'frac04', 
            'frac05', 
            'dR2Mean', 
            'nvtx', 
            'nNeutrals', 
            'beta', 
            'betaStar', 
            'dZ', 
            'nCharged'),
        tmvaMethod = cms.string('BDT_fullPlusRMS'),
        cutBased = cms.bool(False),
        tmvaWeights = cms.string('CMGTools/External/data/TMVAClassification_5x_BDT_fullPlusRMS.weights.xml'),
        tmvaSpectators = cms.vstring('jetPt', 
            'jetEta'),
        label = cms.string('full'),
        version = cms.int32(-1),
        JetIdParams = cms.PSet(
            Pt2030_Tight = cms.vdouble(-0.38, -0.32, -0.14, -0.48),
            Pt2030_Loose = cms.vdouble(-0.8, -0.74, -0.68, -0.77),
            Pt3050_Medium = cms.vdouble(-0.4, -0.49, -0.5, -0.65),
            Pt1020_Tight = cms.vdouble(-0.83, -0.81, -0.74, -0.81),
            Pt2030_Medium = cms.vdouble(-0.4, -0.49, -0.5, -0.65),
            Pt010_Tight = cms.vdouble(-0.83, -0.81, -0.74, -0.81),
            Pt1020_Loose = cms.vdouble(-0.95, -0.96, -0.94, -0.95),
            Pt010_Medium = cms.vdouble(-0.83, -0.92, -0.9, -0.92),
            Pt1020_Medium = cms.vdouble(-0.83, -0.92, -0.9, -0.92),
            Pt010_Loose = cms.vdouble(-0.95, -0.96, -0.94, -0.95),
            Pt3050_Loose = cms.vdouble(-0.8, -0.74, -0.68, -0.77),
            Pt3050_Tight = cms.vdouble(-0.38, -0.32, -0.14, -0.48)
        ),
        impactParTkThreshold = cms.double(1.0)
    ), 
    cms.PSet(
        cutBased = cms.bool(True),
        JetIdParams = cms.PSet(
            Pt010_RMSLoose = cms.vdouble(0.08, 0.07, 0.05, 0.07),
            Pt3050_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
            Pt3050_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
            Pt010_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
            Pt1020_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
            Pt010_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
            Pt1020_RMSLoose = cms.vdouble(0.08, 0.07, 0.05, 0.07),
            Pt1020_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
            Pt2030_RMSTight = cms.vdouble(0.05, 0.07, 0.03, 0.045),
            Pt3050_RMSTight = cms.vdouble(0.05, 0.06, 0.03, 0.04),
            Pt1020_RMSTight = cms.vdouble(0.06, 0.07, 0.04, 0.05),
            Pt3050_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
            Pt3050_RMSLoose = cms.vdouble(0.07, 0.06, 0.04, 0.05),
            Pt2030_RMSLoose = cms.vdouble(0.08, 0.07, 0.04, 0.055),
            Pt010_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
            Pt2030_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
            Pt1020_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
            Pt2030_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
            Pt2030_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
            Pt2030_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
            Pt010_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
            Pt3050_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
            Pt1020_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
            Pt010_RMSTight = cms.vdouble(0.06, 0.07, 0.04, 0.05)
        ),
        impactParTkThreshold = cms.double(1.0),
        label = cms.string('cutbased')
    ))

process.stdalgos_4x = cms.VPSet(cms.PSet(
    tmvaVariables = cms.vstring('frac01', 
        'frac02', 
        'frac03', 
        'frac04', 
        'frac05', 
        'beta', 
        'betaStar'),
    tmvaMethod = cms.string('PuJetIdMinMVA'),
    cutBased = cms.bool(False),
    tmvaWeights = cms.string('CMGTools/External/data/TMVAClassification_PuJetIdMinMVA.weights.xml'),
    tmvaSpectators = cms.vstring('nvtx', 
        'jetPt', 
        'jetEta'),
    label = cms.string('simple'),
    version = cms.int32(-1),
    JetIdParams = cms.PSet(
        Pt2030_Tight = cms.vdouble(-0.2, 0.0, 0.0, 0.0),
        Pt2030_Loose = cms.vdouble(-0.4, -0.85, -0.7, -0.6),
        Pt3050_Medium = cms.vdouble(0.1, -0.4, -0.5, -0.45),
        Pt1020_Tight = cms.vdouble(-0.5, -0.2, -0.83, -0.7),
        Pt2030_Medium = cms.vdouble(0.1, -0.4, -0.5, -0.45),
        Pt010_Tight = cms.vdouble(-0.5, -0.2, -0.83, -0.7),
        Pt1020_Loose = cms.vdouble(-0.9, -0.9, -0.94, -0.9),
        Pt010_Medium = cms.vdouble(-0.73, -0.89, -0.89, -0.83),
        Pt1020_Medium = cms.vdouble(-0.73, -0.89, -0.89, -0.83),
        Pt010_Loose = cms.vdouble(-0.9, -0.9, -0.94, -0.9),
        Pt3050_Loose = cms.vdouble(-0.4, -0.85, -0.7, -0.6),
        Pt3050_Tight = cms.vdouble(-0.2, 0.0, 0.0, 0.0)
    ),
    impactParTkThreshold = cms.double(1.0)
), 
    cms.PSet(
        tmvaVariables = cms.vstring('frac01', 
            'frac02', 
            'frac03', 
            'frac04', 
            'frac05', 
            'nvtx', 
            'nNeutrals', 
            'beta', 
            'betaStar', 
            'dZ', 
            'nCharged'),
        tmvaMethod = cms.string('PuJetIdOptMVA'),
        cutBased = cms.bool(False),
        tmvaWeights = cms.string('CMGTools/External/data/TMVAClassification_PuJetIdOptMVA.weights.xml'),
        tmvaSpectators = cms.vstring('jetPt', 
            'jetEta'),
        label = cms.string('full'),
        version = cms.int32(-1),
        JetIdParams = cms.PSet(
            Pt2030_Tight = cms.vdouble(-0.2, 0.0, 0.0, 0.0),
            Pt2030_Loose = cms.vdouble(-0.4, -0.85, -0.7, -0.6),
            Pt3050_Medium = cms.vdouble(0.1, -0.4, -0.4, -0.45),
            Pt1020_Tight = cms.vdouble(-0.5, -0.2, -0.83, -0.7),
            Pt2030_Medium = cms.vdouble(0.1, -0.4, -0.4, -0.45),
            Pt010_Tight = cms.vdouble(-0.5, -0.2, -0.83, -0.7),
            Pt1020_Loose = cms.vdouble(-0.9, -0.9, -0.9, -0.9),
            Pt010_Medium = cms.vdouble(-0.73, -0.89, -0.89, -0.83),
            Pt1020_Medium = cms.vdouble(-0.73, -0.89, -0.89, -0.83),
            Pt010_Loose = cms.vdouble(-0.9, -0.9, -0.9, -0.9),
            Pt3050_Loose = cms.vdouble(-0.4, -0.85, -0.7, -0.6),
            Pt3050_Tight = cms.vdouble(-0.2, 0.0, 0.0, 0.0)
        ),
        impactParTkThreshold = cms.double(1.0)
    ), 
    cms.PSet(
        cutBased = cms.bool(True),
        JetIdParams = cms.PSet(
            Pt010_RMSLoose = cms.vdouble(0.08, 0.07, 0.05, 0.07),
            Pt3050_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
            Pt3050_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
            Pt010_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
            Pt1020_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
            Pt010_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
            Pt1020_RMSLoose = cms.vdouble(0.08, 0.07, 0.05, 0.07),
            Pt1020_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
            Pt2030_RMSTight = cms.vdouble(0.05, 0.07, 0.03, 0.045),
            Pt3050_RMSTight = cms.vdouble(0.05, 0.06, 0.03, 0.04),
            Pt1020_RMSTight = cms.vdouble(0.06, 0.07, 0.04, 0.05),
            Pt3050_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
            Pt3050_RMSLoose = cms.vdouble(0.07, 0.06, 0.04, 0.05),
            Pt2030_RMSLoose = cms.vdouble(0.08, 0.07, 0.04, 0.055),
            Pt010_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
            Pt2030_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
            Pt1020_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
            Pt2030_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
            Pt2030_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
            Pt2030_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
            Pt010_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
            Pt3050_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
            Pt1020_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
            Pt010_RMSTight = cms.vdouble(0.06, 0.07, 0.04, 0.05)
        ),
        impactParTkThreshold = cms.double(1.0),
        label = cms.string('cutbased')
    ))

process.stdalgos_5x = cms.VPSet(cms.PSet(
    tmvaVariables = cms.vstring('frac01', 
        'frac02', 
        'frac03', 
        'frac04', 
        'frac05', 
        'nvtx', 
        'beta', 
        'betaStar'),
    tmvaMethod = cms.string('BDT_simpleNoVtxCat'),
    cutBased = cms.bool(False),
    tmvaWeights = cms.string('CMGTools/External/data/TMVAClassification_5x_BDT_simpleNoVtxCat.weights.xml'),
    tmvaSpectators = cms.vstring('jetPt', 
        'jetEta'),
    label = cms.string('simple'),
    version = cms.int32(-1),
    JetIdParams = cms.PSet(
        Pt2030_Tight = cms.vdouble(-0.33, -0.3, -0.15, -0.49),
        Pt2030_Loose = cms.vdouble(-0.8, -0.73, -0.68, -0.77),
        Pt3050_Medium = cms.vdouble(-0.4, -0.54, -0.5, -0.65),
        Pt1020_Tight = cms.vdouble(-0.84, -0.8, -0.75, -0.82),
        Pt2030_Medium = cms.vdouble(-0.4, -0.54, -0.5, -0.65),
        Pt010_Tight = cms.vdouble(-0.84, -0.8, -0.75, -0.82),
        Pt1020_Loose = cms.vdouble(-0.95, -0.96, -0.94, -0.95),
        Pt010_Medium = cms.vdouble(-0.85, -0.93, -0.91, -0.92),
        Pt1020_Medium = cms.vdouble(-0.85, -0.93, -0.91, -0.92),
        Pt010_Loose = cms.vdouble(-0.95, -0.96, -0.94, -0.95),
        Pt3050_Loose = cms.vdouble(-0.8, -0.73, -0.68, -0.77),
        Pt3050_Tight = cms.vdouble(-0.33, -0.3, -0.15, -0.49)
    ),
    impactParTkThreshold = cms.double(1.0)
), 
    cms.PSet(
        tmvaVariables = cms.vstring('frac01', 
            'frac02', 
            'frac03', 
            'frac04', 
            'frac05', 
            'dR2Mean', 
            'nvtx', 
            'nNeutrals', 
            'beta', 
            'betaStar', 
            'dZ', 
            'nCharged'),
        tmvaMethod = cms.string('BDT_fullPlusRMS'),
        cutBased = cms.bool(False),
        tmvaWeights = cms.string('CMGTools/External/data/TMVAClassification_5x_BDT_fullPlusRMS.weights.xml'),
        tmvaSpectators = cms.vstring('jetPt', 
            'jetEta'),
        label = cms.string('full'),
        version = cms.int32(-1),
        JetIdParams = cms.PSet(
            Pt2030_Tight = cms.vdouble(-0.38, -0.32, -0.14, -0.48),
            Pt2030_Loose = cms.vdouble(-0.8, -0.74, -0.68, -0.77),
            Pt3050_Medium = cms.vdouble(-0.4, -0.49, -0.5, -0.65),
            Pt1020_Tight = cms.vdouble(-0.83, -0.81, -0.74, -0.81),
            Pt2030_Medium = cms.vdouble(-0.4, -0.49, -0.5, -0.65),
            Pt010_Tight = cms.vdouble(-0.83, -0.81, -0.74, -0.81),
            Pt1020_Loose = cms.vdouble(-0.95, -0.96, -0.94, -0.95),
            Pt010_Medium = cms.vdouble(-0.83, -0.92, -0.9, -0.92),
            Pt1020_Medium = cms.vdouble(-0.83, -0.92, -0.9, -0.92),
            Pt010_Loose = cms.vdouble(-0.95, -0.96, -0.94, -0.95),
            Pt3050_Loose = cms.vdouble(-0.8, -0.74, -0.68, -0.77),
            Pt3050_Tight = cms.vdouble(-0.38, -0.32, -0.14, -0.48)
        ),
        impactParTkThreshold = cms.double(1.0)
    ), 
    cms.PSet(
        cutBased = cms.bool(True),
        JetIdParams = cms.PSet(
            Pt010_RMSLoose = cms.vdouble(0.08, 0.07, 0.05, 0.07),
            Pt3050_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
            Pt3050_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
            Pt010_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
            Pt1020_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
            Pt010_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
            Pt1020_RMSLoose = cms.vdouble(0.08, 0.07, 0.05, 0.07),
            Pt1020_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
            Pt2030_RMSTight = cms.vdouble(0.05, 0.07, 0.03, 0.045),
            Pt3050_RMSTight = cms.vdouble(0.05, 0.06, 0.03, 0.04),
            Pt1020_RMSTight = cms.vdouble(0.06, 0.07, 0.04, 0.05),
            Pt3050_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
            Pt3050_RMSLoose = cms.vdouble(0.07, 0.06, 0.04, 0.05),
            Pt2030_RMSLoose = cms.vdouble(0.08, 0.07, 0.04, 0.055),
            Pt010_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
            Pt2030_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
            Pt1020_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
            Pt2030_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
            Pt2030_BetaStarTight = cms.vdouble(0.15, 0.15, 999.0, 999.0),
            Pt2030_RMSMedium = cms.vdouble(0.06, 0.03, 0.03, 0.04),
            Pt010_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
            Pt3050_BetaStarLoose = cms.vdouble(0.2, 0.3, 999.0, 999.0),
            Pt1020_BetaStarMedium = cms.vdouble(0.2, 999.0, 999.0, 999.0),
            Pt010_RMSTight = cms.vdouble(0.06, 0.07, 0.04, 0.05)
        ),
        impactParTkThreshold = cms.double(1.0),
        label = cms.string('cutbased')
    ))

