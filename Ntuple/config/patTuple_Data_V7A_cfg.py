import FWCore.ParameterSet.Config as cms


###--------------------------------------------------------------
### load PAT, because it insists on defining the process for you
from PhysicsTools.PatAlgos.patTemplate_cfg import *


###--------------------------------------------------------------
### Logger
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport = cms.untracked.PSet(
    reportEvery = cms.untracked.int32(2000),
    limit = cms.untracked.int32(10000000)
)


###--------------------------------------------------------------
### basic filters
# trigger filter
process.load('HLTrigger.HLTfilters.hltHighLevel_cfi')
process.hltHighLevel.TriggerResultsTag = cms.InputTag("TriggerResults","","HLT")
process.hltHighLevel.throw = cms.bool(False)
#process.hltHighLevel.HLTPaths = cms.vstring("HLT_Mu17_Mu8_v*")              #DoubleMu
#process.hltHighLevel.HLTPaths = cms.vstring("HLT_IsoMu24_v*","HLT_Mu40_v*") #SingleMu
process.hltHighLevel.HLTPaths = cms.vstring(                                #MET
    "HLT_DiPFJet40_PFMETnoMu65_MJJ600VBF_LeadingJets_v*",
    "HLT_DiPFJet40_PFMETnoMu65_MJJ800VBF_AllJets_v*"
)


# track quality filter
process.noscraping = cms.EDFilter(
    "FilterOutScraping",
    applyfilter = cms.untracked.bool(True),
    debugOn = cms.untracked.bool(False),
    numtrack = cms.untracked.uint32(10),
    thresh = cms.untracked.double(0.25)
    )

# require a good vertex
process.primaryVertexFilter = cms.EDFilter(
    "VertexSelector",
    src = cms.InputTag("offlinePrimaryVertices"),
    cut = cms.string("!isFake && ndof > 4 && abs(z) <= 24 && position.Rho <= 2"),
    filter = cms.bool(True)
    )


###--------------------------------------------------------------
### MET Filters
# HCAL noise filter
process.load('CommonTools.RecoAlgos.HBHENoiseFilter_cfi')

# Beam halo filter
process.load('RecoMET.METAnalyzers.CSCHaloFilter_cfi')

# HCAL laser filter
process.load('RecoMET.METFilters.hcalLaserEventFilter_cfi')

# ECAL dead cells filter
process.load('RecoMET.METFilters.EcalDeadCellTriggerPrimitiveFilter_cfi')

# ECAL bad supercluster filter
process.load('RecoMET.METFilters.eeBadScFilter_cfi')

# The ECAL laser correction filter
process.load('RecoMET.METFilters.ecalLaserCorrFilter_cfi')

# tracking failure filter
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
process.load('RecoMET.METFilters.trackingFailureFilter_cfi')

# good vertices for tracking failure filter
process.goodVertices = cms.EDFilter(
  "VertexSelector",
  filter = cms.bool(False),
  src = cms.InputTag("offlinePrimaryVertices"),
  cut = cms.string("!isFake && ndof > 4 && abs(z) <= 24 && position.rho < 2")
)
#process.trackingFailureFilter.JetSource = cms.InputTag('ak5PFJets')
#process.trackingFailureFilter.VertexSource = cms.InputTag('goodVertices4TFF')


###--------------------------------------------------------------
### customise PAT
from PhysicsTools.PatAlgos.tools.coreTools import *
removeMCMatching(process, ['All'])
runOnData(process)

# add the PF MET
from PhysicsTools.PatAlgos.tools.metTools import *
addPfMET(process, 'PF')

# switch to PF jets
from PhysicsTools.PatAlgos.tools.jetTools import *
switchJetCollection(process, 
                    cms.InputTag('ak5PFJets'),   
                    doJTA            = True,            
                    doBTagging       = False,            
                    jetCorrLabel     = ('AK5PF', ['L1FastJet', 'L2Relative', 'L3Absolute', 'L2L3Residual']),
                    doType1MET       = True,            
                    genJetCollection = cms.InputTag("ak5GenJets"),
                    doJetID          = False,
                    jetIdLabel       = "ak5"
                    )

# jet selection
process.selectedPatJets.cut = cms.string("pt>10. && abs(eta)<5.")

# apply type 0 MET corrections based on PFCandidate
process.load("JetMETCorrections.Type1MET.pfMETCorrectionType0_cfi")
process.pfType1CorrectedMet.applyType0Corrections = cms.bool(False)
process.pfType1CorrectedMet.srcType1Corrections = cms.VInputTag(
    cms.InputTag('pfMETcorrType0'),
    cms.InputTag('pfJetMETcorr', 'type1')        
)


# remove overlaps ???
#process.cleanPatJets.finalCut = "!hasOverlaps('electrons') && !hasOverlaps('muons')"

# apply loose PF jet ID
from PhysicsTools.SelectorUtils.pfJetIDSelector_cfi import pfJetIDSelector
process.goodPatJets = cms.EDFilter("PFJetIDSelectionFunctorFilter",
                                   filterParams = pfJetIDSelector.clone(),
                                   src = cms.InputTag("selectedPatJets"),
                                   filter = cms.bool(True)
                                   )

# load the PU JetID sequence
process.load("CMGTools.External.pujetidsequence_cff")
process.puJetMva.jets = cms.InputTag("goodPatJets")
process.puJetId.jets = cms.InputTag("goodPatJets")


###--------------------------------------------------------------
### Leptons selection
process.selectedPatMuons.cut = cms.string("isGlobalMuon && pt>10. && abs(eta)<2.5")
# && isPFMuon && globalTrack.normalizedChi2<10. && globalTrack.hitPattern.numberOfValidMuonHits>0 && numberOfMatchedStations>1 && db<0.2")

process.selectedPatElectrons.cut = cms.string("pt>10. && abs(eta)<2.5")


###--------------------------------------------------------------
### MET


###--------------------------------------------------------------
### Tau
# we're not interested in taus
#removeSpecificPATObjects( process, ['Taus'] )
#process.patDefaultSequence.remove( process.patTaus )


###--------------------------------------------------------------
### Trigger matching
## from PhysicsTools.PatAlgos.tools.trigTools import *

## process.metTriggerMatch = cms.EDProducer(
##     "PATTriggerMatcherDRLessByR"                    # match by DeltaR only, best match by DeltaR
##     , src     = cms.InputTag( 'patMETs' )
##     , matched = cms.InputTag( 'patTrigger' )          # default producer label as defined in PhysicsTools/PatAlgos/python/triggerLayer1/triggerProducer_cfi.py
##     , matchedCuts = cms.string( 'path( "HLT_MET100_v*" )' )
##     , maxDPtRel = cms.double( 0.5 )
##     , maxDeltaR = cms.double( 0.5 )
##     , resolveAmbiguities    = cms.bool( True )        # only one match per trigger object
##     , resolveByMatchQuality = cms.bool( True )        # take best match found per reco object: by DeltaR here (s. above)
## )
## process.jetTriggerMatch = cms.EDProducer(
##     "PATTriggerMatcherDRLessByR"                    # match by DeltaR only, best match by DeltaR
##     , src     = cms.InputTag( 'patJets' )
##     , matched = cms.InputTag( 'patTrigger' )          # default producer label as defined in PhysicsTools/PatAlgos/python/triggerLayer1/triggerProducer_cfi.py
##     , matchedCuts = cms.string( 'path( "HLT_Jet30_v*" )' )
##     , maxDPtRel = cms.double( 0.5 )
##     , maxDeltaR = cms.double( 0.5 )
##     , resolveAmbiguities    = cms.bool( True )        # only one match per trigger object
##     , resolveByMatchQuality = cms.bool( True )        # take best match found per reco object: by DeltaR here (s. above)
## )

## switchOnTriggerMatchEmbedding( process, [ 'metTriggerMatch', 'jetTriggerMatch' ] )


###--------------------------------------------------------------
### the path 
process.p = cms.Path(

# trigger filter
    process.hltHighLevel *

# basic filters
    process.noscraping *
    process.primaryVertexFilter *

# MET filters
    process.HBHENoiseFilter  *
    process.CSCTightHaloFilter *
    process.hcalLaserEventFilter  *
    process.EcalDeadCellTriggerPrimitiveFilter *
    process.eeBadScFilter *
    process.ecalLaserCorrFilter *
    process.goodVertices *
    process.trackingFailureFilter *

    process.type0PFMEtCorrection *
    
# generate PAT
    process.patDefaultSequence *
    process.goodPatJets *
    process.puJetIdSqeuence 
)

###--------------------------------------------------------------
### additional parameter
# Global tag
#process.GlobalTag.globaltag = 'GR_R_53_V14::All'
process.GlobalTag.globaltag = 'GR_P_V41_AN2::All'

# Sources
process.source.fileNames =  ['/store/data/Run2012C/MET/AOD/PromptReco-v2/000/203/002/FEB6B5F2-3902-E211-8B50-003048D2BC52.root']
process.maxEvents.input = -1

# JSON (interactive run only)
#import FWCore.PythonUtilities.LumiList as LumiList
#import FWCore.ParameterSet.Types as CfgTypes
#process.source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
#JSONfile = 'Cert_190456-203853_8TeV_PromptReco_Collisions12_JSON_v2.txt'
#myLumis = LumiList.LumiList(filename = JSONfile).getCMSSWString().split(',')
#process.source.lumisToProcess.extend(myLumis)

# output, adjust the event content
process.out.outputCommands += [
    # trigger results
    'keep edmTriggerResults_*_*_*'
    ,'keep *_hltTriggerSummaryAOD_*_*'
    # good jets
    ,'keep *_goodPatJets_*_*'
    # PU jet ID
    ,'keep *_puJetId_*_*'
    ,'keep *_puJetMva_*_*'
    # vertices
    ,'keep *_offlineBeamSpot_*_*'
    ,'keep *_offlinePrimaryVertices*_*_*'
    ,'keep *_goodOfflinePrimaryVertices*_*_*'
    ]
process.out.fileName = 'patTuple_Data.root'

#
#process.options.wantSummary = False   ##  (to suppress the long output at the end of the job)


###---------------------------------------------------------------
### Dump
#file = open('InvHiggs_Data_MET_V7A_cfg.py','w')
#file.write(str(process.dumpPython()))
#file.close()
