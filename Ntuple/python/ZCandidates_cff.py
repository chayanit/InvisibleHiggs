import FWCore.ParameterSet.Config as cms

# Electrons/Muons are selected from PhysicsObjectCandidates_cff.py

# Z->Mu+ Mu-
zToMuMu = cms.EDProducer("NamedCandViewShallowCloneCombiner",
    cut = cms.string('60 < mass < 120'),
    name = cms.string('zToMuMu'),
    roles = cms.vstring('muon1', 'muon2'),
    decay = cms.string('selectMuons@+ selectMuons@-'),
    checkCharge = cms.bool(True)
)

bestZMuMu = cms.EDFilter("LargestPtCandViewSelector",
    maxNumber = cms.uint32(1),
    src = cms.InputTag("zToMuMu")
)

# Z->E+ E-
zToEE = cms.EDProducer("NamedCandViewShallowCloneCombiner",
    cut = cms.string('60 < mass < 120'),
    name = cms.string('zToEE'),
    roles = cms.vstring('electron1', 'electron2'),
    decay = cms.string('selectLooseElectrons@+ selectLooseElectrons@-'),
    checkCharge = cms.bool(True)
)

bestZEE = cms.EDFilter("LargestPtCandViewSelector",
    maxNumber = cms.uint32(1),
    src = cms.InputTag("zToEE")
)

ZSequence = cms.Sequence(zToMuMu
                         * bestZMuMu
                         * zToEE
                         * bestZEE
                         )
