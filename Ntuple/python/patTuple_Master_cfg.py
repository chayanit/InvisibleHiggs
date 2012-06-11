import FWCore.ParameterSet.Config as cms

### load PAT, because it insists on defining the process for you
from PhysicsTools.PatAlgos.patTemplate_cfg import *

### basic filters
# trigger filter
process.load('HLTrigger.HLTfilters.hltHighLevel_cfi')
process.hltHighLevel.TriggerResultsTag = cms.InputTag("TriggerResults","","HLT")
process.hltHighLevel.throw = cms.bool(False)
process.hltHighLevel.HLTPaths = cms.vstring(
    "HLT_DiPFJet40_PFMETnoMu65_MJJ600VBF_LeadingJets_v*",
    "HLT_DiPFJet40_PFMETnoMu65_MJJ800VBF_AllJets_v*"
)

# track quality filter
process.noscraping = cms.EDFilter("FilterOutScraping",
                                  applyfilter = cms.untracked.bool(True),
                                  debugOn = cms.untracked.bool(True),
                                  numtrack = cms.untracked.uint32(10),
                                  thresh = cms.untracked.double(0.25)
                                  )

# require a good vertex
process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
                                           vertexCollection = cms.InputTag('offlinePrimaryVertices'),
                                           minimumNDOF = cms.uint32(4) ,
                                           maxAbsZ = cms.double(24), 
                                           maxd0 = cms.double(2) 
                                           )


### MET Filters ###
# Beam halo filter
process.load('RecoMET.METAnalyzers.CSCHaloFilter_cfi')

# HCAL noise filter
process.load('CommonTools.RecoAlgos.HBHENoiseFilter_cfi')

# HCAL laser filter
process.load('RecoMET.METFilters.hcalLaserEventFilter_cfi')
process.hcalLaserEventFilter.vetoByRunEventNumber = cms.untracked.bool(False)
process.hcalLaserEventFilter.vetoByHBHEOccupancy = cms.untracked.bool(True)

# ECAL dead cells filter
process.load('RecoMET.METFilters.EcalDeadCellTriggerPrimitiveFilter_cfi')

# tracking failure filter
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
process.load('RecoMET.METFilters.trackingFailureFilter_cfi')

process.goodVertices4TFF = cms.EDFilter(
  "VertexSelector",
  filter = cms.bool(False),
  src = cms.InputTag("offlinePrimaryVertices"),
  cut = cms.string("!isFake && ndof > 4 && abs(z) <= 24 && position.rho < 2")
)
process.trackingFailureFilter.JetSource = cms.InputTag('ak5PFJets')
process.trackingFailureFilter.VertexSource = cms.InputTag('goodVertices4TFF')


### get JEC from SQLite file
process.load("InvisibleHiggs.Ntuple.JEC_SQLite_cff")



### customise PAT
from PhysicsTools.PatAlgos.tools.coreTools import *
from PhysicsTools.PatAlgos.tools.pfTools import *

# use PF MET
switchToPFMET(process, input=cms.InputTag('pfMet'))

# use PF2PAT jets
switchJetCollection(process, 
                    cms.InputTag('ak5PFJets'),   
                    doJTA            = True,            
                    doBTagging       = False,            
                    jetCorrLabel     = ('AK5PF', ['L1FastJet', 'L2Relative', 'L3Absolute', 'L2L3Residual']),  
                    doType1MET       = False,            
                    genJetCollection = cms.InputTag("ak5GenJets"),
                    doJetID      = False,
                    jetIdLabel   = "ak5"
#                    btagInfo = ['impactParameterTagInfos','secondaryVertexTagInfos']
#                    btagdiscriminators=['simpleSecondaryVertexHighEffBJetTags','simpleSecondaryVertexHighPurBJetTags']
                    )

## Trigger matching
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



### the path 
process.p = cms.Path(

# trigger filter
    process.hltHighLevel

# basic filters
    +process.noscraping
    +process.primaryVertexFilter

# MET filters
    +process.HBHENoiseFilter 
    +process.CSCTightHaloFilter
    +process.hcalLaserEventFilter 
    +process.EcalDeadCellTriggerPrimitiveFilter
    +process.eeBadScFilter
    +process.goodVertices4TFF
    +process.trackingFailureFilter

# generate PAT
    +process.patDefaultSequence
    )


## ------------------------------------------------------
#  In addition you usually want to change the following
#  parameters:
## ------------------------------------------------------
#
#   process.GlobalTag.globaltag =  ...    ##  (according to https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions)
#                                         ##
#   process.source.fileNames =  ...       ##  (e.g. 'file:AOD.root')
#                                         ##
process.maxEvents.input = 100
#                                         ##
#   process.out.outputCommands = [ ... ]  ##  (e.g. taken from PhysicsTools/PatAlgos/python/patEventContent_cff.py)
#                                         ##
process.out.fileName = 'patTuple.root'
#                                         ##
#   process.options.wantSummary = False   ##  (to suppress the long output at the end of the job)

