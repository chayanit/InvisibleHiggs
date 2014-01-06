import FWCore.ParameterSet.Config as cms

def addInvHiggsProcess(process, iRunOnData=True, iData="ParkedData", iHLTFilter="MET", iMCSignal=False, iFile='/store/data/Run2012C/VBF1Parked/AOD/22Jan2013-v1/20000/40F56410-D979-E211-9843-002618943849.root', iMaxEvent=100):
    
    ###--------------------------------------------------------------
    ### Load PAT, because it insists on defining the process for you
    #from PhysicsTools.PatAlgos.patTemplate_cfg import *
    ###--------------------------------------------------------------


    ###--------------------------------------------------------------
    ### Input
    process.maxEvents.input   = iMaxEvent
    process.source.fileNames  = [iFile]
    ###--------------------------------------------------------------


    ###--------------------------------------------------------------
    ### GlobalTag
    if iRunOnData == True:
        process.GlobalTag.globaltag = "FT53_V21A_AN6::All"
    else:
        process.GlobalTag.globaltag = "START53_V27::All"
    ###--------------------------------------------------------------

    
    ###--------------------------------------------------------------
    ### Logger
    process.load("FWCore.MessageService.MessageLogger_cfi")
    process.load("FWCore.MessageLogger.MessageLogger_cfi")
    process.MessageLogger.cerr.FwkReport = cms.untracked.PSet(
        reportEvery = cms.untracked.int32(2000),
        limit = cms.untracked.int32(10000000)
        )
    ###--------------------------------------------------------------


    ###--------------------------------------------------------------
    ### L1 Trigger
    process.load('L1Trigger.Skimmer.l1Filter_cfi')
    process.l1Filter.algorithms = cms.vstring('L1_ETM40')
    ###--------------------------------------------------------------


    ###--------------------------------------------------------------
    ### HLT Trigger(s)
    # If it's MC signal, no trigger will be applied
    if iRunOnData == False and iMCSignal == True:
        iHLTFilter = "NoTrig"

    # Load hltHighLevel
    process.load('HLTrigger.HLTfilters.hltHighLevel_cfi')
    process.hltHighLevel.TriggerResultsTag = cms.InputTag("TriggerResults","","HLT")
    process.hltHighLevel.throw = cms.bool(False)  # Tolerate if triggers not available
    process.hltHighLevel.andOr = cms.bool(True)   # True = OR, False = AND
    if (iRunOnData == True and iHLTFilter.find("MET")==0):
        process.hltHighLevel.HLTPaths = cms.vstring(
            #"HLT_DiPFJet40_PFMETnoMu65_MJJ600VBF_LeadingJets_v*",
            #"HLT_DiPFJet40_PFMETnoMu65_MJJ800VBF_AllJets_v*"
	    "HLT_DiJet35_MJJ700_AllJets_DEta3p5_VBF_v*",
	    "HLT_DiJet30_MJJ700_AllJets_DEta3p5_VBF_v*"
            )
    elif (iHLTFilter.find("SingleMu")==0):
        process.hltHighLevel.HLTPaths = cms.vstring("HLT_IsoMu24_eta2p1_v*","HLT_Mu40_eta2p1_v*")
    #elif (iHLTFilter.find("DoubleMu")==0):
    #    process.hltHighLevel.HLTPaths = cms.vstring("HLT_Mu17_Mu8_v*")
    #elif (iHLTFilter.find("SingleElectron")==0):
    #    process.hltHighLevel.HLTPaths = cms.vstring("HLT_Ele27_WP80_v*")
    #elif (iHLTFilter.find("DoubleElectron")==0):
    #    process.hltHighLevel.HLTPaths = cms.vstring("HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*")
    elif (iHLTFilter.find("NoTrig")==0):
        process.hltHighLevel.HLTPaths = cms.vstring("*")
    else:
        process.hltHighLevel.HLTPaths = cms.vstring(
            #"HLT_DiPFJet40_PFMETnoMu65_MJJ600VBF_LeadingJets_v*",
            #"HLT_DiPFJet40_PFMETnoMu65_MJJ800VBF_AllJets_v*"
            "HLT_DiJet35_MJJ700_AllJets_DEta3p5_VBF_v*",
            "HLT_DiJet30_MJJ700_AllJets_DEta3p5_VBF_v*"
            )

    ### VBF filter for MC Background
    process.vbfFilter = cms.EDFilter(
        "HLTPFJetVBFFilter",
        saveTags = cms.bool(True),
        triggerType = cms.int32(85),
        inputTag = cms.InputTag("ak5PFJets"),
        leadingJetOnly = cms.bool(False),
        minPtHigh = cms.double(25.0),
        minPtLow = cms.double(25.0),
        maxEta = cms.double(5.0),
        etaOpposite = cms.bool(True),
        minDeltaEta = cms.double(3.0),
        minInvMass = cms.double(500.0)
    )

    ###--------------------------------------------------------------

    ###--------------------------------------------------------------
    print "-----------------------------------------------"
    print "INVISIBLE HIGGS: Ntuple V13 for Parked Data"
    print "-----------------------------------------------"
    print "RunOnData = ", iRunOnData
    if iRunOnData == True:
        print "Dataset = ", iData
    else:
        if iMCSignal == True:
            print "Dataset = MC Signal or DYNoTrig"
        else:
            print "Dataset = MC Background"
    print "GlobalTag = ", process.GlobalTag.globaltag
    print "Trigger = ", process.hltHighLevel.HLTPaths
    print "Sample input file = ", iFile
    print "Max events = ", iMaxEvent
    print "-----------------------------------------------"
    ###--------------------------------------------------------------

    
    ###-------------------------------------------------------------- 
    ### Basic filters
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


    ###--------------------------------------------------------------
    ### MET Filters
    # The iso-based HBHE noise filter
    process.load('CommonTools.RecoAlgos.HBHENoiseFilter_cfi')

    # The CSC beam halo tight filter
    process.load('RecoMET.METAnalyzers.CSCHaloFilter_cfi')
    
    # The HCAL laser filter
    process.load("RecoMET.METFilters.hcalLaserEventFilter_cfi")
    #process.load("EventFilter.HcalRawToDigi.hcallasereventfilter2012_cff")
    #process.hcallasereventfilter2012.eventFileName = cms.string('HCALLaser2012AllDatasets.txt.gz')
    process.load("EventFilter.HcalRawToDigi.hcallaserFilterFromTriggerResult_cff")
    
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

    # The tracking POG filters
    process.load('RecoMET.METFilters.trackingPOGFilters_cff')
    ###--------------------------------------------------------------


    ###--------------------------------------------------------------
    ### customise PAT
    process.load('JetMETCorrections.Configuration.DefaultJEC_cff')

    from PhysicsTools.PatAlgos.tools.coreTools import removeMCMatching,runOnData

    if iRunOnData == True:
        removeMCMatching(process, ['All'])
        runOnData(process)

    # Add the PF MET
    from PhysicsTools.PatAlgos.tools.metTools import addPfMET
    addPfMET(process, 'PF')

    # Switch to PF jets
    from PhysicsTools.PatAlgos.tools.jetTools import switchJetCollection
    if iRunOnData == True:
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
    else:
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

    from PhysicsTools.PatAlgos.tools.pfTools import usePFIso
    usePFIso(process)

    process.patMuons.isoDeposits = cms.PSet()
    process.patMuons.isolationValues = cms.PSet()
    process.patElectrons.isolationValues = cms.PSet(
        pfChargedHadrons = cms.InputTag("elPFIsoValueCharged03PFIdPFIso"),
        pfChargedAll = cms.InputTag("elPFIsoValueChargedAll03PFIdPFIso"),
        pfPUChargedHadrons = cms.InputTag("elPFIsoValuePU03PFIdPFIso"),
        pfNeutralHadrons = cms.InputTag("elPFIsoValueNeutral03PFIdPFIso"),
        pfPhotons = cms.InputTag("elPFIsoValueGamma03PFIdPFIso")
        )
    process.patElectrons.isolationValuesNoPFId = cms.PSet(
        pfChargedHadrons = cms.InputTag("elPFIsoValueCharged03NoPFIdPFIso"),
        pfChargedAll = cms.InputTag("elPFIsoValueChargedAll03NoPFIdPFIso"),
        pfPUChargedHadrons = cms.InputTag("elPFIsoValuePU03NoPFIdPFIso"),
        pfNeutralHadrons = cms.InputTag("elPFIsoValueNeutral03NoPFIdPFIso"),
        pfPhotons = cms.InputTag("elPFIsoValueGamma03NoPFIdPFIso")
        )
    process.patDefaultSequence.replace(process.patElectrons,process.eleIsoSequence+process.patElectrons)
    process.cleanPatTaus.preselection = cms.string('tauID("decayModeFinding") > 0.5 & tauID("byLooseCombinedIsolationDeltaBetaCorr") > 0.5')
    ###--------------------------------------------------------------
    

    ###--------------------------------------------------------------
    ### Object (pre) selection
    # Jet selection
    process.selectedPatJets.cut = cms.string("pt>10. && abs(eta)<5.")

    # Remove overlaps ???
    # process.cleanPatJets.finalCut = "!hasOverlaps('electrons') && !hasOverlaps('muons')"

    # Apply loose PF jet ID
    from PhysicsTools.SelectorUtils.pfJetIDSelector_cfi import pfJetIDSelector
    process.goodPatJets = cms.EDFilter("PFJetIDSelectionFunctorFilter",
                                       filterParams = pfJetIDSelector.clone(),
                                       src = cms.InputTag("selectedPatJets"),
                                       filter = cms.bool(True)
                                       )

    process.selectedPatMuons.cut = cms.string("isGlobalMuon && pt>10. && abs(eta)<2.5")
    
    process.selectedPatElectrons.cut = cms.string("pt>10. && abs(eta)<2.5")
    ###--------------------------------------------------------------


    ###--------------------------------------------------------------
    ### Load the PU JetID sequence
    if iRunOnData == True:
    	process.load("CMGTools.External.pujetidsequence_cff")
    	process.puJetMva.jets = cms.InputTag("goodPatJets")
    	process.puJetId.jets = cms.InputTag("goodPatJets")
    else:
        process.load("CMGTools.External.pujetidsequence_cff")
        process.puJetMva.jets		= cms.InputTag("goodPatJets")
        process.puJetId.jets 		= cms.InputTag("goodPatJets")
	process.puJetMvaSmeared		= process.puJetMva.clone()
	process.puJetIdSmeared		= process.puJetId.clone()  
	process.puJetMvaResUp 		= process.puJetMva.clone()
	process.puJetIdResUp 		= process.puJetId.clone()
        process.puJetMvaResDown		= process.puJetMva.clone()
        process.puJetIdResDown  	= process.puJetId.clone()
        process.puJetMvaEnUp   		= process.puJetMva.clone()
        process.puJetIdEnUp    		= process.puJetId.clone()
        process.puJetMvaEnDown 		= process.puJetMva.clone()
        process.puJetIdEnDown  		= process.puJetId.clone()

        process.puJetIdSmeared.jets     = cms.InputTag("smearedGoodPatJets")
        process.puJetMvaSmeared.jetids  = cms.InputTag("puJetIdSmeared")
        process.puJetMvaSmeared.jets    = cms.InputTag("smearedGoodPatJets")

	process.puJetIdResUp.jets 	= cms.InputTag("smearedGoodPatJetsResUp")
	process.puJetMvaResUp.jetids 	= cms.InputTag("puJetIdResUp")
	process.puJetMvaResUp.jets 	= cms.InputTag("smearedGoodPatJetsResUp")

        process.puJetIdResDown.jets     = cms.InputTag("smearedGoodPatJetsResDown")
        process.puJetMvaResDown.jetids  = cms.InputTag("puJetIdResDown")
        process.puJetMvaResDown.jets    = cms.InputTag("smearedGoodPatJetsResDown")

        process.puJetIdEnUp.jets        = cms.InputTag("shiftedGoodPatJetsEnUpForCorrMEt")
        process.puJetMvaEnUp.jetids     = cms.InputTag("puJetIdEnUp")
        process.puJetMvaEnUp.jets       = cms.InputTag("shiftedGoodPatJetsEnUpForCorrMEt")

        process.puJetIdEnDown.jets      = cms.InputTag("shiftedGoodPatJetsEnDownForCorrMEt")
        process.puJetMvaEnDown.jetids   = cms.InputTag("puJetIdEnDown")
        process.puJetMvaEnDown.jets     = cms.InputTag("shiftedGoodPatJetsEnDownForCorrMEt")
    ###--------------------------------------------------------------


    ###--------------------------------------------------------------
    ### MET
    # Apply type 0 MET corrections based on PFCandidate
    process.load("JetMETCorrections.Type1MET.pfMETCorrectionType0_cfi")
    process.pfType1CorrectedMet.applyType0Corrections = cms.bool(False)
    process.pfType1CorrectedMet.srcType1Corrections = cms.VInputTag(
        cms.InputTag('pfMETcorrType0'),
        cms.InputTag('pfJetMETcorr', 'type1')
        )

    # Get loose ID/Iso muons and veto ID electrons
    process.load("InvisibleHiggs.Ntuple.PhysicsObjectCandidates_cff")

    # Get PFMET from runMEtUncertainties
    from PhysicsTools.PatUtils.tools.metUncertaintyTools import runMEtUncertainties
    process.load("JetMETCorrections.Type1MET.pfMETsysShiftCorrections_cfi")

    if iRunOnData == True:
        runMEtUncertainties(process,
                            electronCollection = cms.InputTag('selectVetoElectrons'),
                            photonCollection = '',
                            muonCollection = 'selectLooseMuons',
                            tauCollection = '',
                            jetCollection = cms.InputTag('goodPatJets'),
                            jetCorrLabel = 'L2L3Residual',
                            doSmearJets = False,
                            makeType1corrPFMEt = True,
                            makeType1p2corrPFMEt = False,
                            makePFMEtByMVA = False,
                            makeNoPileUpPFMEt = False,
                            doApplyType0corr = True,
                            sysShiftCorrParameter = process.pfMEtSysShiftCorrParameters_2012runAvsNvtx_data,
                            doApplySysShiftCorr = False,
			    addToPatDefaultSequence = False,
                            )
        process.patPFJetMETtype1p2Corr.jetCorrLabel = cms.string('L2L3Residual')
        process.patPFJetMETtype2Corr.jetCorrLabel = cms.string('L2L3Residual')
    else:
        runMEtUncertainties(process,
                            electronCollection = cms.InputTag('selectVetoElectrons'),
                            photonCollection = '',
                            muonCollection = 'selectLooseMuons',
                            tauCollection = '',
                            jetCollection = cms.InputTag('goodPatJets'),
                            jetCorrLabel = 'L3Absolute',
                            doSmearJets = True,
                            makeType1corrPFMEt = True,
                            makeType1p2corrPFMEt = False,
                            makePFMEtByMVA = False,
                            makeNoPileUpPFMEt = False,
                            doApplyType0corr = True,
                            sysShiftCorrParameter = process.pfMEtSysShiftCorrParameters_2012runAvsNvtx_mc,
                            doApplySysShiftCorr = False,
			    addToPatDefaultSequence = False,
                            )

    # Fix Type0 correction module
    #process.patPFMETtype0Corr.correction.par3 = cms.double(0.909209)
    #process.patPFMETtype0Corr.correction.par2 = cms.double(0.0303531)
    #process.patPFMETtype0Corr.correction.par1 = cms.double(-0.703151)
    #process.patPFMETtype0Corr.correction.par0 = cms.double(0.0)
    # Need this line for CMSSW_5_3_11
    process.producePatPFMETCorrections.replace(process.patPFJetMETtype2Corr,process.patPFJetMETtype2Corr + process.type0PFMEtCorrectionPFCandToVertexAssociation + process.patPFMETtype0Corr)
    #process.producePatPFMETCorrections.replace(process.patPFJetMETtype2Corr,process.patPFJetMETtype2Corr + process.patPFMETtype0Corr)
    ###--------------------------------------------------------------

    # PAT/ntuples one step  
    # Z and W candidates    
    process.load('InvisibleHiggs/Ntuple/WCandidates_cff')
    process.load('InvisibleHiggs/Ntuple/ZCandidates_cff')
    # Ntuple producer       
    process.load('InvisibleHiggs/Ntuple/invHiggsInfo_cfi')
    if iRunOnData == False: 
        # Jet/MET uncertainty
        process.invHiggsInfo.jetTag      = cms.untracked.InputTag("smearedGoodPatJets")
        process.invHiggsInfo.metTag      = cms.untracked.InputTag("patType1CorrectedPFMet")
        process.invHiggsInfo.puJetMvaTag = cms.untracked.InputTag("puJetMvaSmeared", "full53xDiscriminant")
        process.invHiggsInfo.puJetIdTag  = cms.untracked.InputTag("puJetMvaSmeared", "full53xId")

        # PU re-weighting
        process.invHiggsInfo.puMCFile    = cms.untracked.string("PUHistS10.root")
        process.invHiggsInfo.puDataFile  = cms.untracked.string("PUHistRun2012All_forParked.root")
        process.invHiggsInfo.puMCHist    = cms.untracked.string("pileup")
        process.invHiggsInfo.puDataHist  = cms.untracked.string("pileup")
        process.invHiggsInfo.mcPYTHIA    = cms.untracked.bool(True)

        process.invHiggsInfo.trigCorrFile   = cms.untracked.string("HLTParkedEff_v1.root")

	process.invHiggsInfo.leptCorrFile   = cms.untracked.string("leptonWeights.root")
    # TTree output file
    process.load("CommonTools.UtilAlgos.TFileService_cfi")
    process.TFileService.fileName = cms.string('invHiggsInfo.root')
    ###--------------------------------------------------------------

    ###--------------------------------------------------------------
    ### Tau
    # removeSpecificPATObjects( process, ['Taus'] )
    # process.patDefaultSequence.remove( process.patTaus )
    process.load("RecoTauTag.Configuration.RecoPFTauTag_cff")
    ###--------------------------------------------------------------


    ###--------------------------------------------------------------
    ### Trigger matching
    # from PhysicsTools.PatAlgos.tools.trigTools import *

    # process.metTriggerMatch = cms.EDProducer(
    #     "PATTriggerMatcherDRLessByR"                    # match by DeltaR only, best match by DeltaR
    #     , src     = cms.InputTag( 'patMETs' )
    #     , matched = cms.InputTag( 'patTrigger' )          # default producer label as defined in PhysicsTools/PatAlgos/python/triggerLayer1/triggerProducer_cfi.py
    #     , matchedCuts = cms.string( 'path( "HLT_MET100_v*" )' )
    #     , maxDPtRel = cms.double( 0.5 )
    #     , maxDeltaR = cms.double( 0.5 )
    #     , resolveAmbiguities    = cms.bool( True )        # only one match per trigger object
    #     , resolveByMatchQuality = cms.bool( True )        # take best match found per reco object: by DeltaR here (s. above)
    # )
    # process.jetTriggerMatch = cms.EDProducer(
    #     "PATTriggerMatcherDRLessByR"                    # match by DeltaR only, best match by DeltaR
    #     , src     = cms.InputTag( 'patJets' )
    #     , matched = cms.InputTag( 'patTrigger' )          # default producer label as defined in PhysicsTools/PatAlgos/python/triggerLayer1/triggerProducer_cfi.py
    #     , matchedCuts = cms.string( 'path( "HLT_Jet30_v*" )' )
    #     , maxDPtRel = cms.double( 0.5 )
    #     , maxDeltaR = cms.double( 0.5 )
    #     , resolveAmbiguities    = cms.bool( True )        # only one match per trigger object
    #     , resolveByMatchQuality = cms.bool( True )        # take best match found per reco object: by DeltaR here (s. above)
    # )
    
    # switchOnTriggerMatchEmbedding( process, [ 'metTriggerMatch', 'jetTriggerMatch' ] )
    ###--------------------------------------------------------------


    ###--------------------------------------------------------------
    ### Paths
    process.p0 = cms.Path( process.HBHENoiseFilter )
    process.p1 = cms.Path( process.CSCTightHaloFilter )
    process.p2 = cms.Path( process.hcalLaserEventFilter )
    process.p3 = cms.Path( process.EcalDeadCellTriggerPrimitiveFilter )
    process.p4 = cms.Path( process.eeBadScFilter )
    process.p5 = cms.Path( process.ecalLaserCorrFilter )
    process.p6 = cms.Path( process.goodVertices * process.trackingFailureFilter )
    process.p7 = cms.Path( process.trkPOGFilters )
    if iRunOnData == True:
        #process.p8 = cms.Path( process.hcallLaserEvent2012Filter )
        process.p8 = cms.Path( process.hcalfilter )
    else:
        process.p8 = cms.Path( process.primaryVertexFilter )

    if iRunOnData == True:
    	process.p = cms.Path(    
        	# Trigger filter
		process.l1Filter *
        	process.hltHighLevel *
        
        	# Basic filters
        	process.noscraping *
        	process.primaryVertexFilter *
        	
        	# MET filters (Move to be flagged)
        
        	# MET Correction
        	process.type0PFMEtCorrection *
        
        	# Tau
        	process.recoTauClassicHPSSequence *
        
        	# Generate PAT
        	process.pfParticleSelectionSequence *
        	process.patDefaultSequence *
        	process.goodPatJets *
		process.PhysicsObjectSequence *
		process.metUncertaintySequence *
        	process.puJetIdSqeuence *
                process.WSequence *
                process.ZSequence *
                process.invHiggsInfo
		)
    elif (iMCSignal == True):
        process.p = cms.Path(
                # Trigger filter
                #process.hltHighLevel *

                # Basic filters
                process.noscraping *
                process.primaryVertexFilter *

                # MET filters (Move to be flags)

                # MET Correction
                process.type0PFMEtCorrection *

                # Tau
                process.recoTauClassicHPSSequence *

                # Generate PAT
                process.pfParticleSelectionSequence *
                process.patDefaultSequence *
                process.goodPatJets *
                process.PhysicsObjectSequence *
                process.metUncertaintySequence *
                process.puJetIdSqeuence *
		process.puJetIdSmeared *
		process.puJetMvaSmeared *
        	process.puJetIdResUp *
		process.puJetMvaResUp *
         	process.puJetIdResDown *
        	process.puJetMvaResDown *
        	process.puJetIdEnUp *
        	process.puJetMvaEnUp *
        	process.puJetIdEnDown *
        	process.puJetMvaEnDown *
                process.WSequence *
                process.ZSequence *
                process.invHiggsInfo
        	)
    else :
        process.p = cms.Path(
                # Trigger filter
                #process.hltHighLevel *
		process.vbfFilter * 

                # Basic filters
                process.noscraping *
                process.primaryVertexFilter *

                # MET filters (Move to be flags)

                # MET Correction
                process.type0PFMEtCorrection *

                # Tau
                process.recoTauClassicHPSSequence *

                # Generate PAT
                process.pfParticleSelectionSequence *
                process.patDefaultSequence *
                process.goodPatJets *
                process.PhysicsObjectSequence *
                process.metUncertaintySequence *
                process.puJetIdSqeuence *
                process.puJetIdSmeared *
                process.puJetMvaSmeared *
                process.puJetIdResUp *
                process.puJetMvaResUp *
                process.puJetIdResDown *
                process.puJetMvaResDown *
                process.puJetIdEnUp *
                process.puJetMvaEnUp *
                process.puJetIdEnDown *
                process.puJetMvaEnDown *
                process.WSequence *
                process.ZSequence *
                process.invHiggsInfo
                )
    ###--------------------------------------------------------------


    ###--------------------------------------------------------------
    ### Output
    process.out.outputCommands += [
        # trigger results
        'keep edmTriggerResults_*_*_*'
        ,'keep *_hltTriggerSummaryAOD_*_*'
        # L1
        ,'keep *_l1extraParticles_MET_RECO'
        ,'keep *_l1extraParticles_MHT_RECO'
        # good jets
        ,'keep *_goodPatJets_*_*'
        # PU jet ID
        ,'keep *_puJetId*_*_*'
        ,'keep *_puJetMva*_*_*'
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
                
    del(process.out)
    del(process.outpath)
    ###--------------------------------------------------------------

