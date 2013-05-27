import FWCore.ParameterSet.Config as cms

# Electrons/Muons are selected from PhysicsObjectCandidates_cff.py

# W->Mu Nu
## Note: the 'mt()' method doesn't compute the transverse mass correctly, so we have to do it by hand.
wToMuNu = cms.EDProducer("CandViewShallowCloneCombiner",
    decay = cms.string("selectMuons patMETs"),    
    cut = cms.string('sqrt(2*daughter(0).pt*daughter(1).pt*(1-cos(daughter(0).phi-daughter(1).phi)))>0'),
    checkCharge = cms.bool(False),
)

bestWMuNu = cms.EDFilter("LargestPtCandViewSelector",
    maxNumber = cms.uint32(5),
    src = cms.InputTag("wToMuNu")
)

# W->Electron Nu
## Note: the 'mt()' method doesn't compute the transverse mass correctly, so we have to do it by hand.
wToENu = cms.EDProducer("CandViewShallowCloneCombiner",
    decay = cms.string("selectElectrons patMETs"),
    cut = cms.string('sqrt(2*daughter(0).pt*daughter(1).pt*(1-cos(daughter(0).phi-daughter(1).phi)))>0'),
    checkCharge = cms.bool(False),
)

bestWENu = cms.EDFilter("LargestPtCandViewSelector",
    maxNumber = cms.uint32(5),
    src = cms.InputTag("wToENu")
)

# Veto
muonFilter = cms.EDFilter("PATCandViewCountFilter",
    minNumber = cms.uint32(0),
    maxNumber = cms.uint32(5),
    src = cms.InputTag("selectMuons")                     
)

electronFilter = cms.EDFilter("PATCandViewCountFilter",
    minNumber = cms.uint32(0),
    maxNumber = cms.uint32(5),
    src = cms.InputTag("selectElectrons")                     
)

# Sequence
WSequence = cms.Sequence(wToMuNu 
                         * bestWMuNu
                         * wToENu
                         * bestWENu 
                         )

VetoSequence = cms.Sequence(electronFilter*muonFilter)

WSequence = cms.Sequence(WSequence*VetoSequence)
