## import skeleton process
from PhysicsTools.PatAlgos.patTemplate_cfg import *


## ------------------------------------------------------
#  NOTE: you can use a bunch of core tools of PAT to
#  taylor your PAT configuration; for a few examples
#  uncomment the lines below
## ------------------------------------------------------
from PhysicsTools.PatAlgos.tools.coreTools import *

## remove MC matching from the default sequence
# removeMCMatching(process)
# runOnData(process)

## remove certain objects from the default sequence
# removeAllPATObjectsBut(process, ['Muons'])
# removeSpecificPATObjects(process, ['Electrons', 'Muons', 'Taus'])


# L1FastJet hack
## Geometry and Detector Conditions (needed for a few patTuple production steps)
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
##-------------------- Import the JEC services -----------------------
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
##-------------------- Import the Jet RECO modules -----------------------
process.load('RecoJets.Configuration.RecoPFJets_cff')
##-------------------- Turn-on the FastJet density calculation -----------------------
process.kt6PFJets.doRhoFastjet = True
##-------------------- Turn-on the FastJet jet area calculation for your favorite algorithm -----------------------
process.ak5PFJets.doAreaFastjet = True

## let it run
process.p = cms.Path(
    process.kt6PFJets *
    process.ak5PFJets *
    process.patDefaultSequence
    )

## Trigger matching
from PhysicsTools.PatAlgos.tools.trigTools import *

process.metTriggerMatch = cms.EDProducer(
    "PATTriggerMatcherDRLessByR"                    # match by DeltaR only, best match by DeltaR
    , src     = cms.InputTag( 'patMETs' )
    , matched = cms.InputTag( 'patTrigger' )          # default producer label as defined in PhysicsTools/PatAlgos/python/triggerLayer1/triggerProducer_cfi.py
    , matchedCuts = cms.string( 'path( "HLT_MET100_v*" )' )
    , maxDPtRel = cms.double( 0.5 )
    , maxDeltaR = cms.double( 0.5 )
    , resolveAmbiguities    = cms.bool( True )        # only one match per trigger object
    , resolveByMatchQuality = cms.bool( True )        # take best match found per reco object: by DeltaR here (s. above)
)
process.jetTriggerMatch = cms.EDProducer(
    "PATTriggerMatcherDRLessByR"                    # match by DeltaR only, best match by DeltaR
    , src     = cms.InputTag( 'patJets' )
    , matched = cms.InputTag( 'patTrigger' )          # default producer label as defined in PhysicsTools/PatAlgos/python/triggerLayer1/triggerProducer_cfi.py
    , matchedCuts = cms.string( 'path( "HLT_Jet30_v*" )' )
    , maxDPtRel = cms.double( 0.5 )
    , maxDeltaR = cms.double( 0.5 )
    , resolveAmbiguities    = cms.bool( True )        # only one match per trigger object
    , resolveByMatchQuality = cms.bool( True )        # take best match found per reco object: by DeltaR here (s. above)
)

switchOnTriggerMatchEmbedding( process, [ 'metTriggerMatch', 'jetTriggerMatch' ] )

## ------------------------------------------------------
#  In addition you usually want to change the following
#  parameters:
## ------------------------------------------------------
#
process.GlobalTag.globaltag = cms.string( 'START44_V13::All')
#                                       
process.source.fileNames = [
    'file:/gpfs_phys/storm/cms/mc/Fall11/VBF_HToZZTo4Nu_M-120_7TeV-pythia6/GEN-SIM-RAW-HLTDEBUG-RECO/E7TeV_Ave32_50ns-v1/0000/F855B69B-0559-E111-B875-003048D45F80.root'
]
#                                         
process.maxEvents.input = 100
#                                         
#   process.out.outputCommands = [ ... ]  ##  (e.g. taken from PhysicsTools/PatAlgos/python/patEventContent_cff.py)
#                                         
#   process.out.fileName = ...            ##  (e.g. 'myTuple.root')
#                                         
#   process.options.wantSummary = True    ##  (to suppress the long output at the end of the job)    
