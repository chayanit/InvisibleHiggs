from PhysicsTools.PatAlgos.patTemplate_cfg import *

## ------------------------------------------------------
#  NOTE: you can use a bunch of core tools of PAT to
#  taylor your PAT configuration; for a few examples
#  uncomment the lines below
## ------------------------------------------------------
#from PhysicsTools.PatAlgos.tools.coreTools import *

## remove MC matching from the default sequence
# removeMCMatching(process, ['Muons'])
# runOnData(process)

## remove certain objects from the default sequence
# removeAllPATObjectsBut(process, ['Muons'])
# removeSpecificPATObjects(process, ['Electrons', 'Muons', 'Taus'])

# trigger filter
process.load('HLTrigger.HLTfilters.hltHighLevel_cfi')
process.hltHighLevel.TriggerResultsTag = cms.InputTag("TriggerResults","","HLT")
process.hltHighLevel.throw = cms.bool(False)
process.hltHighLevel.HLTPaths = cms.vstring(
    "HLT_DiPFJet40_PFMETnoMu65_MJJ600VBF_LeadingJets_v*",
    "HLT_DiPFJet40_PFMETnoMu65_MJJ800VBF_AllJets_v*"
)

# Jet corrections (inc L1 Fast jet)
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
process.load('RecoJets.Configuration.RecoPFJets_cff')
process.kt6PFJets.doRhoFastjet = True
process.ak5PFJetsL1FastL2L3   = cms.EDProducer('PFJetCorrectionProducer',
    src         = cms.InputTag('ak5PFJets'),
    correctors  = cms.vstring('ak5PFL1FastL2L3')
)
process.ak5CaloJetsL1FastL2L3   = cms.EDProducer('CaloJetCorrectionProducer',
    src         = cms.InputTag('ak5CaloJets'),
    correctors  = cms.vstring('ak5CaloL1FastL2L3')
)
process.ak5CaloJets.doAreaFastJet = True
process.ak5PFJets.doAreaFastjet = True


## let it run
process.p = cms.Path(

# trigger filter
    process.hltHighLevel
    
# jet corrections
    +process.kt6PFJets
    +process.ak5PFJets
    +process.ak5PFJetsL1FastL2L3
    +process.ak5CaloJets
    +process.ak5CaloJetsL1FastL2L3

# generate PAT
    +process.patDefaultSequence
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
process.out.fileName = 'invHiggs_patTuple.root'
#                                         ##
#   process.options.wantSummary = False   ##  (to suppress the long output at the end of the job)

