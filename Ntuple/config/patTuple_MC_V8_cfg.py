import FWCore.ParameterSet.Config as cms


###--------------------------------------------------------------
### Load PAT, because it insists on defining the process for you
from PhysicsTools.PatAlgos.patTemplate_cfg import *


###--------------------------------------------------------------
### Set these parameters
iRunOnData = False
iHLTFilter = "MET"  #MET, SingleMu, DoubleMu, SingleElectron, DoubleElectron
iDump      = False

# Input + GlobalTag
if iRunOnData == True:
    process.source.fileNames =  ['/store/data/Run2012C/MET/AOD/PromptReco-v2/000/202/972/02DE5FC9-7501-E211-96BF-001D09F244DE.root']

    #process.GlobalTag.globaltag = 'GR_R_53_V14::All'  #For reprocessing data
    process.GlobalTag.globaltag = 'GR_P_V41_AN2::All' #Use for prompt data, start from 2012C_v2
else:
    process.source.fileNames =  ['/store/relval/CMSSW_5_3_4_cand1-START53_V10/RelValTTbar/GEN-SIM-RECO/v1/0000/D670FBA4-53F5-E111-9587-001A928116D6.root']
    process.GlobalTag.globaltag = 'START53_V7A::All'  #MC

process.maxEvents.input = -1

# JSON (interactive run only)
#import FWCore.PythonUtilities.LumiList as LumiList
#import FWCore.ParameterSet.Types as CfgTypes
#process.source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
#JSONfile = 'Cert_190456-203853_8TeV_PromptReco_Collisions12_JSON_v2.txt'
#myLumis = LumiList.LumiList(filename = JSONfile).getCMSSWString().split(',')
#process.source.lumisToProcess.extend(myLumis)


###--------------------------------------------------------------
### Logger
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport = cms.untracked.PSet(
    reportEvery = cms.untracked.int32(2000),
    limit = cms.untracked.int32(10000000)
)


###--------------------------------------------------------------
### Basic filters
# Trigger filter
process.load('HLTrigger.HLTfilters.hltHighLevel_cfi')
process.hltHighLevel.TriggerResultsTag = cms.InputTag("TriggerResults","","HLT")
process.hltHighLevel.throw = cms.bool(False)  # Tolerate if triggers not available
process.hltHighLevel.andOr = cms.bool(True)   # True = OR, False = AND
if (iHLTFilter.find("DoubleMu")==0):
    process.hltHighLevel.HLTPaths = cms.vstring("HLT_Mu17_Mu8_v*")
elif (iHLTFilter.find("SingleMu")==0):
    process.hltHighLevel.HLTPaths = cms.vstring("HLT_IsoMu24_eta2p1_v*","HLT_Mu40_eta2p1_v*")
elif (iHLTFilter.find("DoubleElectron")==0):
    process.hltHighLevel.HLTPaths = cms.vstring("HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*")
elif (iHLTFilter.find("SingleElectron")==0):
    process.hltHighLevel.HLTPaths = cms.vstring("HLT_Ele27_WP80_v*")
else:
    process.hltHighLevel.HLTPaths = cms.vstring(
        "HLT_DiPFJet40_PFMETnoMu65_MJJ600VBF_LeadingJets_v*",
        "HLT_DiPFJet40_PFMETnoMu65_MJJ800VBF_AllJets_v*"
        )

# Track quality filter
process.noscraping = cms.EDFilter(
    "FilterOutScraping",
    applyfilter = cms.untracked.bool(True),
    debugOn = cms.untracked.bool(False),
    numtrack = cms.untracked.uint32(10),
    thresh = cms.untracked.double(0.25)
    )

# Require a good vertex
process.primaryVertexFilter = cms.EDFilter(
    "VertexSelector",
    src = cms.InputTag("offlinePrimaryVertices"),
    cut = cms.string("!isFake && ndof > 4 && abs(z) <= 24 && position.Rho <= 2"),
    filter = cms.bool(True)
    )


###--------------------------------------------------------------
### MET Filters
# The iso-based HBHE noise filter
process.load('CommonTools.RecoAlgos.HBHENoiseFilter_cfi')

# The CSC beam halo tight filter
process.load('RecoMET.METAnalyzers.CSCHaloFilter_cfi')

# The HCAL laser filter (no monidified parameter from V00-00-09 of RecoMET.METFilters)
process.load('RecoMET.METFilters.hcalLaserEventFilter_cfi')

# The ECAL dead cell trigger primitive filter
process.load('RecoMET.METFilters.EcalDeadCellTriggerPrimitiveFilter_cfi')

# The EE bad SuperCrystal filter
process.load('RecoMET.METFilters.eeBadScFilter_cfi')

# The ECAL laser correction filter
process.load('RecoMET.METFilters.ecalLaserCorrFilter_cfi')

# The Good vertices collection needed by the tracking failure filter
process.goodVertices = cms.EDFilter(
  "VertexSelector",
  filter = cms.bool(False),
  src = cms.InputTag("offlinePrimaryVertices"),
  cut = cms.string("!isFake && ndof > 4 && abs(z) <= 24 && position.rho < 2")
)

# The tracking failure filter
process.load('RecoMET.METFilters.trackingFailureFilter_cfi')


###--------------------------------------------------------------
### customise PAT
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')

from PhysicsTools.PatAlgos.tools.coreTools import *

if iRunOnData == True:
    removeMCMatching(process, ['All'])
    runOnData(process)

# Add the PF MET
from PhysicsTools.PatAlgos.tools.metTools import *
addPfMET(process, 'PF')

# Switch to PF jets
from PhysicsTools.PatAlgos.tools.jetTools import *
switchJetCollection(process, 
                    cms.InputTag('ak5PFJets'),   
                    doJTA            = True,            
                    doBTagging       = False,            
                    jetCorrLabel     = ('AK5PF', ['L1FastJet', 'L2Relative', 'L3Absolute']),
                    doType1MET       = True,            
                    genJetCollection = cms.InputTag("ak5GenJets"),
                    doJetID          = False,
                    jetIdLabel       = "ak5"
                    )
if iRunOnData == True:
    process.patJetCorrFactors.levels = cms.vstring('L1FastJet','L2Relative','L3Absolute','L2L3Residual')
    process.AK5PFCombinedCorrector.correctors = cms.vstring('AK5PFL1FastJet', 
                                                            'AK5PFL2Relative', 
                                                            'AK5PFL3Absolute',
                                                            'AK5PFL2L3Residual')

# Jet selection
process.selectedPatJets.cut = cms.string("pt>10. && abs(eta)<5.")

# Remove overlaps ???
#process.cleanPatJets.finalCut = "!hasOverlaps('electrons') && !hasOverlaps('muons')"

# Apply loose PF jet ID
from PhysicsTools.SelectorUtils.pfJetIDSelector_cfi import pfJetIDSelector
process.goodPatJets = cms.EDFilter("PFJetIDSelectionFunctorFilter",
                                   filterParams = pfJetIDSelector.clone(),
                                   src = cms.InputTag("selectedPatJets"),
                                   filter = cms.bool(True)
                                   )

# Load the PU JetID sequence
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
# Apply type 0 MET corrections based on PFCandidate
process.load("JetMETCorrections.Type1MET.pfMETCorrectionType0_cfi")
process.pfType1CorrectedMet.applyType0Corrections = cms.bool(False)
process.pfType1CorrectedMet.srcType1Corrections = cms.VInputTag(
    cms.InputTag('pfMETcorrType0'),
    cms.InputTag('pfJetMETcorr', 'type1')
)

# Get PFMET from runMEtUncertainties
from PhysicsTools.PatUtils.tools.metUncertaintyTools import runMEtUncertainties
process.load("JetMETCorrections.Type1MET.pfMETsysShiftCorrections_cfi")

if iRunOnData == True:
    runMEtUncertainties(process,
                        electronCollection = cms.InputTag('cleanPatElectrons'),
                        photonCollection = '',
                        muonCollection = 'cleanPatMuons',
                        tauCollection = 'cleanPatTaus',
                        jetCollection = cms.InputTag('selectedPatJets'),
                        jetCorrLabel = 'L2L3Residual',
                        doSmearJets = False,
                        makeType1corrPFMEt = True,
                        makeType1p2corrPFMEt = False,
                        makePFMEtByMVA = False,
                        makeNoPileUpPFMEt = False,
                        doApplyType0corr = True,
                        sysShiftCorrParameter = process.pfMEtSysShiftCorrParameters_2012runAvsNvtx_data,
                        doApplySysShiftCorr = False,
                        )
    process.patPFJetMETtype1p2Corr.jetCorrLabel = cms.string('L2L3Residual')
    process.patPFJetMETtype2Corr.jetCorrLabel = cms.string('L2L3Residual')
else:
    runMEtUncertainties(process,
                        electronCollection = cms.InputTag('cleanPatElectrons'),
                        photonCollection = '',
                        muonCollection = 'cleanPatMuons',
                        tauCollection = 'cleanPatTaus',
                        jetCollection = cms.InputTag('selectedPatJets'),
                        jetCorrLabel = 'L3Absolute',
                        doSmearJets = True,
                        makeType1corrPFMEt = True,
                        makeType1p2corrPFMEt = False,
                        makePFMEtByMVA = False,
                        makeNoPileUpPFMEt = False,
                        doApplyType0corr = True,
                        sysShiftCorrParameter = process.pfMEtSysShiftCorrParameters_2012runAvsNvtx_mc,
                        doApplySysShiftCorr = False,
                        )

#Fix Type0 correction module
process.patPFMETtype0Corr.correction.par3 = cms.double(0.909209)
process.patPFMETtype0Corr.correction.par2 = cms.double(0.0303531)
process.patPFMETtype0Corr.correction.par1 = cms.double(-0.703151)
process.patPFMETtype0Corr.correction.par0 = cms.double(0.0)


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
### Paths 

process.p0 = cms.Path( process.HBHENoiseFilter )
process.p1 = cms.Path( process.CSCTightHaloFilter )
process.p2 = cms.Path( process.hcalLaserEventFilter )
process.p3 = cms.Path( process.EcalDeadCellTriggerPrimitiveFilter )
process.p4 = cms.Path( process.eeBadScFilter )
process.p5 = cms.Path( process.ecalLaserCorrFilter )
process.p6 = cms.Path( process.goodVertices * process.trackingFailureFilter )

process.p = cms.Path(    
    # Trigger filter
    process.hltHighLevel *

    # Basic filters
    process.noscraping *
    process.primaryVertexFilter *

    # MET filters (Move to be flags)

    # MET Correction
    process.type0PFMEtCorrection *
    
    # Generate PAT
    process.patDefaultSequence *
    process.goodPatJets *
    process.puJetIdSqeuence
)


###--------------------------------------------------------------
### Output

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
    ,'keep double_*_rho_*'
    ]

if iRunOnData == False:
    process.out.outputCommands += ['keep GenEventInfoProduct_*_*_*'
                                   ,'keep recoGenParticles_*_*_*'
                                   ,'keep GenMETs_*_*_*'
                                   ,'keep *_addPileupInfo_*_*'
                                   ,'keep LHEEventProduct_*_*_*'
                                   ]

process.out.fileName = 'patTuple.root'


###--------------------------------------------------------------
### Additional settings

# Want summary?
#process.options.wantSummary = False   ##  (to suppress the long output at the end of the job)

# Dump
if iDump == True:
    if iRunOnData == True:
        file = open('InvHiggs_Dump_Data_cfg.py','w')
    else:
        file = open('InvHiggs_Dump_MC_cfg.py','w')
    file.write(str(process.dumpPython()))
    file.close()

