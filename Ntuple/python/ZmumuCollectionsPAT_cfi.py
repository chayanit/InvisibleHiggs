import FWCore.ParameterSet.Config as cms

isQCD = False

isolationCutString = cms.string("")
if isQCD:
#    isolationCutString = "(isolationR03().sumPt+isolationR03().emEt+isolationR03().hadEt)/pt> 0.1"
    isolationCutString = "(pfIsolationR04().sumChargedHadronPt+max(0.,pfIsolationR04().sumNeutralHadronEt+pfIsolationR04().sumPhotonEt-0.5*pfIsolationR04().sumPUPt))/pt> 0.12"
else:
#    isolationCutString = "(isolationR03().sumPt+isolationR03().emEt+isolationR03().hadEt)/pt< 0.3"
    isolationCutString = "(pfIsolationR04().sumChargedHadronPt+max(0.,pfIsolationR04().sumNeutralHadronEt+pfIsolationR04().sumPhotonEt-0.5*pfIsolationR04().sumPUPt))/pt< 0.12"

selectMuons = cms.EDFilter("PATMuonSelector",
    src = cms.InputTag("cleanPatMuons"),
    cut = cms.string("pt>20 && isGlobalMuon && isPFMuon && abs(eta)<2.1"
                     " && globalTrack().normalizedChi2<10"
                     " && globalTrack().hitPattern().numberOfValidMuonHits>0"
                     " && globalTrack().hitPattern().numberOfValidPixelHits>0"
                     " && numberOfMatchedStations>1"
                     " && globalTrack().hitPattern().trackerLayersWithMeasurement>5"
                     " && " + isolationCutString
                     )
)

ZToMM = cms.EDProducer("NamedCandViewShallowCloneCombiner",
    cut = cms.string('60 < mass < 120'),
    name = cms.string('ZToMM'),
    roles = cms.vstring('muon1', 'muon2'),
    decay = cms.string('selectMuons@+ selectMuons@-'),
    checkCharge = cms.bool(True)
)

bestZmumu = cms.EDFilter("LargestPtCandViewSelector",
    maxNumber = cms.uint32(1),
    src = cms.InputTag("ZToMM")
)

ZPath = cms.Sequence(selectMuons*ZToMM*bestZmumu)
