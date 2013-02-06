import FWCore.ParameterSet.Config as cms

######################
# Muons
# https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideMuonId
######################
# Definition bases on PhysicsTools/PatAlgos/plugins/PATObjectSelector.cc 

# Tight muons
isolationCutString = cms.string("")
isolationCutString = "(pfIsolationR04().sumChargedHadronPt+max(0.,pfIsolationR04().sumNeutralHadronEt+pfIsolationR04().sumPhotonEt-0.5*pfIsolationR04().sumPUPt))/pt< 0.12"

selectMuons = cms.EDFilter(
    "PATMuonSelector",
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

# Loose muons
selectLooseMuons = cms.EDFilter(
    "PATMuonSelector",
    src = cms.InputTag("cleanPatMuons"),
    cut = cms.string("pt>10 && (isGlobalMuon || isTrackerMuon) && isPFMuon && abs(eta)<2.1")
)

######################
# Electrons
# https://twiki.cern.ch/twiki/bin/view/CMS/EgammaIDRecipes
######################
# Definition bases on InvisibleHiggs/Ntuple/plugins/ElectronIdSelector.cc

# Tight electrons
selectElectrons = cms.EDProducer("InvHiggsPATElectronIdSelector",
    src = cms.InputTag( "cleanPatElectrons" ),
    idLabel = cms.string("tight")
)

# Loose electrons
selectLooseElectrons = cms.EDProducer("InvHiggsPATElectronIdSelector",
    src = cms.InputTag( "cleanPatElectrons" ),
    idLabel = cms.string("loose")
)

# Veto electrons
selectVetoElectrons = cms.EDProducer("InvHiggsPATElectronIdSelector",
    src = cms.InputTag( "cleanPatElectrons" ),
    idLabel = cms.string("veto")
)

# Photon: We are now using

# Tau: We are now using

PhysicsObjectSequence = cms.Sequence(selectMuons
                                     * selectLooseMuons
                                     * selectElectrons
                                     * selectLooseElectrons
				     * selectVetoElectrons
                                    )
