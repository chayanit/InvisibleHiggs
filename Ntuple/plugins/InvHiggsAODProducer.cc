// -*- C++ -*-
//
// Package:    InvisibleHiggs/Analysis
// Class:      InvHiggsTreeProducer
// 
/**\class InvHiggsAODProducer InvHiggsAODProducer.cc InvisibleHiggs/Analysis/src/InvHiggsAODProducer.cc

 Description: Produce invisible Higgs TTreeom AOD/AODSIM

 Implementation:
     
*/
//
// Original Author:  Jim Brooke
//         Created:  
// $Id: InvHiggsAODProducer.cc,v 1.4 2012/06/08 13:11:14 jbrooke Exp $
//
//


// system include files
#include <memory>

// framework
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Common/interface/ConditionsInEdm.h"


// L1
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetup.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"

#include "DataFormats/L1Trigger/interface/L1JetParticleFwd.h"
#include "DataFormats/L1Trigger/interface/L1JetParticle.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "L1Trigger/GlobalTriggerAnalyzer/interface/L1GtUtils.h"

// HLT
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

// calo jets
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/JetID.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"

// PF jets
#include "DataFormats/JetReco/interface/PFJetCollection.h"

// Muons
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"

// Electrons
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"

// MET
#include "DataFormats/METReco/interface/CaloMETFwd.h"
#include "DataFormats/METReco/interface/PFMETFwd.h"
#include "DataFormats/METReco/interface/PFMET.h"

// Vertices
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

// Beam Halo Summary
#include "DataFormats/METReco/interface/BeamHaloSummary.h"

// MC
#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"

// Lumi
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"


// Math
#include "DataFormats/Math/interface/LorentzVector.h"

// ROOT output stuff
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH1.h"
#include "TTree.h"
#include "TF1.h"

// TTree definition
#include "InvisibleHiggs/Ntuple/interface/InvHiggsEvent.h"


#include <algorithm>


//
// class declaration
//

using namespace reco;

class InvHiggsAODProducer : public edm::EDAnalyzer {
public:
  explicit InvHiggsAODProducer(const edm::ParameterSet&);
  ~InvHiggsAODProducer();
  
private:
  virtual void beginJob() ;
  virtual void beginRun(const edm::Run&, const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  void doEventInfo(const edm::Event&);

  /// write MC info
  void doMC(const edm::Event&);

  /// write trigger info
  void doTrigger(const edm::Event&, const edm::EventSetup&);

  // write basic RECO objects
  void doCaloJets(const edm::Event&, const edm::EventSetup&);
  void doPFJets(const edm::Event&, const edm::EventSetup&);
  void doMuons(const edm::Event&);
  void doElectrons(const edm::Event&);
  void doVertices(const edm::Event&);
  void doGlobal(const edm::Event&);

  void doVBFJetVariables(std::vector<reco::CaloJet> jets);

  void doPUReweighting(const edm::Event&);

public:
  
  struct l1jet_gt : public std::binary_function<l1extra::L1JetParticle, l1extra::L1JetParticle, bool> {
    bool operator()(const l1extra::L1JetParticle& x, const l1extra::L1JetParticle& y) {
      return ( x.et() > y.et() ) ;
    }
  };

private:
  
  // output file
  edm::Service<TFileService> fs_;
  
  // tree
  TTree * tree_;
  InvHiggsEvent* event_;

  // EDM input tags
  bool useMC_;
  std::string l1JetsTag_;
  edm::InputTag l1BitsTag_;
  edm::InputTag hltResultsTag_;
  edm::InputTag hltEventTag_;
  std::string hltPath1Name_;
  std::string hltPath2Name_;
  edm::InputTag hltL3Tag_;
  edm::InputTag mcTag_;
  std::string mcProducer_;
  edm::InputTag hepProducer_;
  std::string jetCorrectorServiceName_;
  edm::InputTag caloJetTag_;
  edm::InputTag caloJetIDTag_;
  edm::InputTag pfJetTag_;
  edm::InputTag muonTag_;
  edm::InputTag electronTag_;
  edm::InputTag caloMETTag_;
  edm::InputTag pfMETTag_;
  edm::InputTag pfMHTTag_;
  edm::InputTag vertexTag_;
  edm::InputTag haloTag_;

  // HLT config helper
  HLTConfigProvider hltConfig_;
  unsigned hltBit1_;
  unsigned hltBit2_;
  bool doHltBit_;

  // PU re-weighting
  //edm::LumiReWeighting lumiWeights_;

};




InvHiggsAODProducer::InvHiggsAODProducer(const edm::ParameterSet& iConfig):
  tree_(0),
  event_(0),
  useMC_(iConfig.getUntrackedParameter<bool>("useMC",false)),
  l1JetsTag_(iConfig.getUntrackedParameter<std::string>("l1JetsTag",std::string("l1extraParticles"))),
  l1BitsTag_(iConfig.getUntrackedParameter<edm::InputTag>("l1BitsTag",edm::InputTag("gtDigis"))),
  hltResultsTag_(iConfig.getUntrackedParameter<edm::InputTag>("hltResultsTag",edm::InputTag("TriggerResults","","HLT"))),
  hltEventTag_(iConfig.getUntrackedParameter<edm::InputTag>("hltEventTag",edm::InputTag("hltTriggerSummaryAOD","","HLT"))),
  hltPath1Name_(iConfig.getUntrackedParameter<std::string>("hltPathName",std::string("HLT_v1"))),
  hltPath2Name_(iConfig.getUntrackedParameter<std::string>("hltPathName",std::string("HLT_v1"))),
  hltL3Tag_(iConfig.getUntrackedParameter<edm::InputTag>("hltL3Tag",edm::InputTag("hltFilter","","HLT"))),
  mcTag_(iConfig.getUntrackedParameter<edm::InputTag>("mcTag",edm::InputTag("generator"))),
  mcProducer_ (iConfig.getUntrackedParameter<std::string>("producer", "g4SimHits")),
  hepProducer_ (iConfig.getUntrackedParameter<edm::InputTag>("hepMCProducerTag", edm::InputTag("generator", "", "SIM"))),
  jetCorrectorServiceName_(iConfig.getUntrackedParameter<std::string>("jetCorrectorServiceName","ic5CaloL1L2L3Residual")),
  caloJetTag_(iConfig.getUntrackedParameter<edm::InputTag>("caloJetTag",edm::InputTag("ak5CaloJets"))),
  caloJetIDTag_(iConfig.getUntrackedParameter<edm::InputTag>("caloJetIDTag",edm::InputTag("ak5CaloJets"))),
  pfJetTag_(iConfig.getUntrackedParameter<edm::InputTag>("pfJetTag",edm::InputTag("ak5PFJets"))),
  muonTag_(iConfig.getUntrackedParameter<edm::InputTag>("muonTag",edm::InputTag("muons"))),
  electronTag_(iConfig.getUntrackedParameter<edm::InputTag>("electronTag",edm::InputTag("GsfElectrons"))),
  caloMETTag_(iConfig.getUntrackedParameter<edm::InputTag>("caloMETTag",edm::InputTag("caloMET"))),
  pfMETTag_(iConfig.getUntrackedParameter<edm::InputTag>("pfMETTag",edm::InputTag("pfMET"))),
  pfMHTTag_(iConfig.getUntrackedParameter<edm::InputTag>("pfMHTTag",edm::InputTag("pfMHT"))),
  vertexTag_(iConfig.getUntrackedParameter<edm::InputTag>("verticesTag", edm::InputTag("offlinePrimaryVertices"))),
  haloTag_(iConfig.getUntrackedParameter<edm::InputTag>("metTag",edm::InputTag("BeamHaloSummary"))),
  hltConfig_(),
  hltBit1_(0),
  hltBit2_(0),
  doHltBit_(true)
//   lumiWeights_(iConfig.getUntrackedParameter<std::string>("puMCFile", ""),
// 	       iConfig.getUntrackedParameter<std::string>("puDataFile", ""),
// 	       iConfig.getUntrackedParameter<std::string>("puMCHist", ""),
// 	       iConfig.getUntrackedParameter<std::string>("puDataHist", ""))
{
  // set up output
  tree_=fs_->make<TTree>("InvHiggsTree", "");
  tree_->Branch("events", "InvHiggsEvent", &event_, 1000000, 1);
  
}


InvHiggsAODProducer::~InvHiggsAODProducer() {

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called once each job just before starting event loop  ------------
void InvHiggsAODProducer::beginJob()
{
}

// -- called once per run
void InvHiggsAODProducer::beginRun(edm::Run const & iRun, edm::EventSetup const& iSetup)
{
  // Get PDT Table if MC
  //  iSetup.getData(fPDGTable);

  // HLT setup
  bool changed;
  hltConfig_.init(iRun, iSetup, hltResultsTag_.process(), changed);
  
  // HLT Path -- No BPTX
  try{
    hltBit1_=(hltConfig_.triggerNames()).size();  // default setting -- trigger not found
    hltBit2_=(hltConfig_.triggerNames()).size();  // default setting -- trigger not found
    for (uint i=0;i<(hltConfig_.triggerNames()).size();++i) // loop over trigger names
      {
	std::string trigName=hltConfig_.triggerName(i);

	if (hltPath1Name_.size()>0 &&
	    trigName.find(hltPath1Name_)!= std::string::npos)
	  {
	    hltBit1_ = hltConfig_.triggerIndex(trigName);
	  }
	if (hltPath2Name_.size()>0 &&
	    trigName.find(hltPath2Name_)!= std::string::npos)
	  {
	    hltBit2_ = hltConfig_.triggerIndex(trigName);
	  }
      }
    if (hltBit1_==(hltConfig_.triggerNames()).size())
      {
	edm::LogWarning("InvHiggsAODProducer") << "Could not find an HLT path matching "<<hltPath1Name_<<std::endl;
      }
    if (hltBit2_==(hltConfig_.triggerNames()).size())
      {
	edm::LogWarning("InvHiggsAODProducer") << "Could not find an HLT path matching "<<hltPath2Name_<<std::endl;
      }
    else
      edm::LogInfo("InvHiggsAODProducer") << hltPath1Name_ << " index is " << hltBit1_ << std::endl;
      edm::LogInfo("InvHiggsAODProducer") << hltPath2Name_ << " index is " << hltBit2_ << std::endl;
  } // end of try loop
  catch (cms::Exception e) {
    edm::LogWarning("InvHiggsAODProducer") << "Exception while trying to find HLT bit numbers" << std::endl;
    edm::LogWarning("InvHiggsAODProducer") << e << std::endl;
  }

}


// ------------ method called once each job just after ending the event loop  ------------
void 
InvHiggsAODProducer::endJob() {
}

// ------------ method called to for each event  ------------
void
InvHiggsAODProducer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
 
  event_ = new InvHiggsEvent();
 
  if (! iEvent.eventAuxiliary().isRealData()) {
    doMC(iEvent);
    doPUReweighting(iEvent);
  }
 
  // event & trigger info
  doEventInfo(iEvent);
  doTrigger(iEvent, iSetup);

  doCaloJets(iEvent, iSetup);
  doPFJets(iEvent, iSetup);
  doMuons(iEvent);
  doElectrons(iEvent);
  doVertices(iEvent);
  doGlobal(iEvent);

  // fill TTree
  tree_->Fill();
  
  delete event_;

}



void InvHiggsAODProducer::doMC(const edm::Event& iEvent) {

  edm::Handle<edm::HepMCProduct> mcHandle;
  iEvent.getByLabel(mcTag_,mcHandle);
  
//   if (mcHandle.isValid()) {
    
//     const edm::HepMCProduct *mcProd = mcHandle.product();
//     const HepMC::GenEvent *evt = mcProd->GetEvent();
    
//   }

}

void InvHiggsAODProducer::doEventInfo(const edm::Event& iEvent) {

  unsigned long id          = iEvent.id().event();
  unsigned long bx          = iEvent.bunchCrossing();
  //unsigned long orbit       = iEvent.orbitNumber();
  unsigned long lb          = iEvent.luminosityBlock();
  unsigned long run         = iEvent.id().run();
  //double time               = iEvent.time().value();

  event_->id = id;
  event_->bx = bx;
  event_->lb = lb;
  event_->run = run;

}
  

void InvHiggsAODProducer::doTrigger(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  // get GT data
//   edm::ESHandle<L1GtTriggerMenu> menuRcd;
//   iSetup.get<L1GtTriggerMenuRcd>().get(menuRcd) ;
//   const L1GtTriggerMenu* menu = menuRcd.product();
  
//   edm::Handle<L1GlobalTriggerReadoutRecord> gtReadoutRecord;
//   iEvent.getByLabel(l1BitsTag_, gtReadoutRecord);

//   // L1 trigger bits (for triggered BX)
//   uint64_t gtAlgoWord0(0), gtAlgoWord1(0), gtTechWord(0);

//   DecisionWord gtDecisionWord = gtReadoutRecord->decisionWord();
//   unsigned i=0;
//   for(DecisionWord::const_iterator itr=gtDecisionWord.begin();
//       itr != gtDecisionWord.end();
//       itr++, i++) {
//     if (*itr) {
//       if(i<64) { gtAlgoWord0 |= (1LL<<i); }
//       else { gtAlgoWord1 |= (1LL<<(i-64)); }
//     } 
//   }
  
//   TechnicalTriggerWord gtTechDecisionWord = gtReadoutRecord->technicalTriggerWord();    
//   i=0;
//   for(TechnicalTriggerWord::const_iterator itr=gtDecisionWord.begin();
//       itr != gtDecisionWord.end();
//       itr++, i++) {
//     if (*itr) {
//       gtTechWord |= (1LL<<i);
//     } 
//   }

//   event_->gtAlgoWord0 = gtAlgoWord0;
//   event_->gtAlgoWord1 = gtAlgoWord1;
//   event_->gtTechWord = gtTechWord;


  bool hltResult1(false);
  bool hltResult2(false);

  // get HLT results
  edm::Handle<edm::TriggerResults> HLTR;
  iEvent.getByLabel(hltResultsTag_, HLTR);

  if (HLTR.isValid()) {
    // triggerIndex must be less than the size of HLTR or you get a CMSException: _M_range_check
    if (doHltBit_ && hltBit1_ < HLTR->size()) hltResult1 = HLTR->accept(hltBit1_);
    if (doHltBit_ && hltBit2_ < HLTR->size()) hltResult2 = HLTR->accept(hltBit2_);
  }
  else {
    if (doHltBit_) edm::LogWarning("MissingProduct") << "HLT information not found.  Branch will not be filled" << std::endl;
    doHltBit_ = false;
  }

  // store bits
  event_->hltResult1 = hltResult1; 
  event_->hltResult2 = hltResult2; 

  // Store HLT prescale info
//   event_->hltPrescaleIndex=hltConfig_.prescaleSet(iEvent, iSetup);
//   event_->hltPrescale=hltConfig_.prescaleValue(iEvent, iSetup, hltPathName_);
  

  // L1 jets
  edm::Handle<l1extra::L1JetParticleCollection> l1CenJets;
  iEvent.getByLabel(l1JetsTag_, "Central", l1CenJets);  
  edm::Handle<l1extra::L1JetParticleCollection> l1TauJets;
  iEvent.getByLabel(l1JetsTag_, "Tau", l1TauJets);
  
  if (l1CenJets.isValid() && l1TauJets.isValid()) { 

    // merge and sort central and tau jet collections
    std::vector<l1extra::L1JetParticle> l1jets;
    
    l1jets.insert(l1jets.end(), l1CenJets->begin(), l1CenJets->end());
    l1jets.insert(l1jets.end(), l1TauJets->begin(), l1TauJets->end());
    std::sort(l1jets.begin(), l1jets.end(), l1jet_gt());
    
    for (std::vector<l1extra::L1JetParticle>::const_iterator jet=l1jets.begin(); jet!=l1jets.end(); ++jet) {
      int type = jet->type();   // 0 = central, 1 = forward, 2 = tau
      double et = jet->et();
      double phi = jet->phi();
      double eta = jet->eta();
      event_->addL1Jet(et, eta, phi, type);
    }
    
  }

  // get HLT jets
  edm::Handle<trigger::TriggerEvent> trgEvent;

  //std::cout <<"hltEventTag = "<<hltEventTag_<<std::endl;
  iEvent.getByLabel(hltEventTag_, trgEvent);

  // for Stopped HSCP L3 filter  
  if (trgEvent.isValid()) {

    const unsigned int filterIndex (trgEvent->filterIndex(hltL3Tag_) );

    if (filterIndex < trgEvent->sizeFilters()) {
            
      const trigger::TriggerObjectCollection& TOC(trgEvent->getObjects());
      const trigger::Keys& keys( trgEvent->filterKeys(filterIndex) );

      for ( unsigned hlto = 0; 
	    hlto < keys.size();
	    hlto++ ) {
	trigger::size_type hltf = keys[hlto];
	const trigger::TriggerObject& L3obj(TOC[hltf]);

	double et = L3obj.et();
	double phi = L3obj.phi();
	double eta = L3obj.eta();
	event_->addHltJet(et, eta, phi);
	
      }
      
    }

  }
							       
}



void InvHiggsAODProducer::doCaloJets(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  const JetCorrector* jetCorrector=0;
  if (jetCorrectorServiceName_.length()>0) jetCorrector = JetCorrector::getJetCorrector(jetCorrectorServiceName_,iSetup);

  edm::Handle<reco::CaloJetCollection> jets;
  edm::Handle<edm::ValueMap<reco::JetID> > jetIDs;

  iEvent.getByLabel(caloJetTag_, jets);
  iEvent.getByLabel(caloJetIDTag_,jetIDs);
  
  unsigned njet=0;
  
  if (jets.isValid()) {
    
    for(CaloJetCollection::const_iterator it=jets->begin(); 
	it!=jets->end();
	++it, ++njet) {
      
      edm::RefToBase<reco::Jet> jetRef(edm::Ref<reco::CaloJetCollection>(jets,njet));  
      double scale=0;
      if (jetCorrector!=0) scale = jetCorrector->correction(*it,jetRef,iEvent,iSetup);
      
      // store jet in TTree
      double et = it->et();
      double etcorr = it->et()*scale;
      double eta = it->eta();
      double phi = it->phi();
      double emf = it->emEnergyFraction();
      int n60 = it->n60();
      int n90 = it->n90();
      double fhpd = 0.;
      double frbx = 0.;
      int n90hits = 0;
      
      if (jetIDs.isValid()) {
	fhpd = (*jetIDs)[jetRef].fHPD;
	frbx = (*jetIDs)[jetRef].fRBX;
	n90hits = int((*jetIDs)[jetRef].n90Hits);
      }

      event_->addCaloJet(et, etcorr, eta, phi, emf, n60, n90, fhpd, frbx, n90hits); 
      
    } // loop over jets

    // calo mjj
    double mass = 0.;
    if (jets->size()>2) {
      math::XYZTLorentzVector pair = jets->at(0).p4() + jets->at(1).p4();
      mass = pair.M();
    }
    event_->caloMjj = mass;

    doVBFJetVariables(*jets);
    
  } // if (caloJets.isValid())
  
}


void InvHiggsAODProducer::doPFJets(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  const JetCorrector* jetCorrector=0;
  if (jetCorrectorServiceName_.length()>0) jetCorrector = JetCorrector::getJetCorrector(jetCorrectorServiceName_,iSetup);

  edm::Handle<reco::PFJetCollection> jets;
  iEvent.getByLabel(pfJetTag_, jets);
  unsigned njet=0;
  
  if (jets.isValid()) {
    
    for(PFJetCollection::const_iterator it=jets->begin(); 
	it!=jets->end();
	++it, ++njet) {
      
      edm::RefToBase<reco::Jet> jetRef(edm::Ref<reco::PFJetCollection>(jets,njet));  
      double scale=0;
      if (jetCorrector!=0) scale = jetCorrector->correction(*it,jetRef,iEvent,iSetup);
      
      // store jet in TTree
      double et = it->et();
      double etcorr = it->et() * scale;
      double eta = it->eta();
      double phi = it->phi();
      double nhf = it->neutralHadronEnergyFraction();
      double nef = it->neutralEmEnergyFraction();
      double chf = it->chargedHadronEnergyFraction();
      double cef = it->chargedEmEnergyFraction();
      int nconst = it->getPFConstituents().size();
      int cmult  = it->chargedHadronMultiplicity();
      
      event_->addPFJet(et, etcorr, eta, phi, nhf, nef, chf, cef, nconst, cmult); 
      
    } // loop over jets

    // mjj
    double mass = 0.;
    if (jets->size()>2) {
      math::XYZTLorentzVector pair = jets->at(0).p4() + jets->at(1).p4();
      mass = pair.M();
    }
    event_->pfMjj = mass;
    
  } // if (caloJets.isValid())
  
}


void InvHiggsAODProducer::doMuons(const edm::Event& iEvent) {

  // loop over reco muons
  edm::Handle<reco::MuonCollection> muons;
  iEvent.getByLabel(muonTag_,muons);

  if (muons.isValid()) {
    for(reco::MuonCollection::const_iterator it =muons->begin();
	it!=muons->end();
	it++) {

      double pt = it->pt();
      double eta = it->eta();
      double phi = it->phi();
      int type = (0xf & it->type());

      event_->addMuon(pt, eta, phi, type);

    }

    // leading pair mass
    double mass = 0.;
    if (muons->size()>2) {
      math::XYZTLorentzVector pair = muons->at(0).p4() + muons->at(1).p4();
      mass = pair.M();
    }
    event_->mMuMu = mass;

  }
  
}


void InvHiggsAODProducer::doElectrons(const edm::Event& iEvent) {

  edm::Handle<reco::GsfElectronCollection> electrons;
  iEvent.getByLabel(electronTag_,electrons);

  if (electrons.isValid()) {
    for(reco::GsfElectronCollection::const_iterator it =electrons->begin();
	it!=electrons->end();
	it++) {

      double pt = it->pt();
      double eta = it->eta();
      double phi = it->phi();

      event_->addElectron(pt, eta, phi);

    }

    // leading pair mass
    double mass = 0.;
    if (electrons->size()>2) {
      math::XYZTLorentzVector pair = electrons->at(0).p4() + electrons->at(1).p4();
      mass = pair.M();
    }
    event_->mEE = mass;
    
  }
  
}


void InvHiggsAODProducer::doVertices(const edm::Event& iEvent) {

  edm::Handle<reco::VertexCollection> vertices;
  iEvent.getByLabel(vertexTag_, vertices);

  if (vertices.isValid()) {
    for(reco::VertexCollection::const_iterator it=vertices->begin();
	it!=vertices->end();
	++it) {
      
      if (!it->isFake()) {

	int ndof = it->ndof();
	double z = it->z();
	double rho = it->position().rho();
	double phi = it->position().phi();

	event_->addVertex(z, rho, phi, ndof);

      }
      
    }
  }
  
} // void InvHiggsAODProducer::doVertices(const edm::Event& iEvent)


void InvHiggsAODProducer::doGlobal(const edm::Event& iEvent) {

  // calo MET
  edm::Handle<reco::CaloMETCollection> calomet;
  iEvent.getByLabel(caloMETTag_, calomet);

  if (calomet.isValid()) {

    event_->caloMET = calomet->at(0).pt();
    event_->caloMETSig = calomet->at(0).mEtSig();

  }

  // PFMET
  edm::Handle<reco::PFMETCollection> pfmet;
  iEvent.getByLabel(pfMETTag_, pfmet);

  if (pfmet.isValid()) {

    event_->pfMET = pfmet->at(0).pt();
    event_->pfMETSig = pfmet->at(0).mEtSig();

  }

  // calculate PFMHT from jets
  // TODO !

  // beam halo
  edm::Handle<BeamHaloSummary> haloSummary;
  iEvent.getByLabel(haloTag_, haloSummary);

  if (haloSummary.isValid()) {
    event_->beamHalo = haloSummary->CSCTightHaloId();
  }

  return;
}



/// identify tag jets and store some variables
void InvHiggsAODProducer::doVBFJetVariables(std::vector<reco::CaloJet> jets) {

  bool vbfFound = false;

  for (unsigned i=0; i<jets.size()-1 && !vbfFound; ++i) {
    for (unsigned j=i; j<jets.size() && !vbfFound; ++j) {

      // VBF jet vector sum
      math::XYZTLorentzVector v1=jets.at(i).p4();
      math::XYZTLorentzVector v2=jets.at(j).p4();
      math::XYZTLorentzVector vbfp4=v1+v2;

      double deta = fabs(jets.at(i).eta() - jets.at(j).eta());

      if (v1.Et() > 30. &&
	  v2.Et() > 30. &&
	  deta > 3.5 &&
	  vbfp4.M() > 800.) {

	vbfFound = true;

	event_->vbfJet1Index = i;
	event_->vbfJet2Index = j;
	
	event_->vbfEt  = vbfp4.Et();
	event_->vbfEta = vbfp4.Eta();
	event_->vbfPhi = vbfp4.Phi();
	event_->vbfM   = vbfp4.M();

	// find highest Et jet not in VBF pair
	for (unsigned k=0; k< jets.size(); ++k) {
	  if (k!=i && k!=j) {
	    event_->vbfJet3Phi = jets.at(k).phi();	    
	    break;
	  }
	}

	// find nearest jet to vbf vector
	double dRmin = 999.0;  // 
	unsigned nrJetIndex = 999999;
	for (unsigned k=0; k< jets.size(); ++k) {
	  double dR = sqrt(pow(vbfp4.Eta()-jets.at(k).eta(), 2) + pow(vbfp4.Phi()-jets.at(k).phi(), 2));
	  if (dR<dRmin) {
	    nrJetIndex = k;
	    dRmin = dR;
	  }
	}

	if (nrJetIndex < jets.size()) {
	  event_->vbfNearJetEt = jets.at(nrJetIndex).et();
	  event_->vbfNearJetDR = dRmin;
	}

	// find highest et jet with eta between tag jets
	double etamin = std::min(v1.Eta(), v2.Eta());
	double etamax = std::max(v1.Eta(), v2.Eta());
	for (unsigned k=0; k< jets.size(); ++k) {
	  if (jets.at(k).eta() > etamin &&
	      jets.at(k).eta() < etamax) {
	    event_->vbfCenJetEt = jets.at(k).et();
	    break;
	  }
	}
	double etamin2 = std::min(v1.Eta(), v2.Eta())+0.5;
	double etamax2 = std::max(v1.Eta(), v2.Eta())-0.5;
	for (unsigned k=0; k< jets.size(); ++k) {
	  if (jets.at(k).eta() > etamin2 &&
	      jets.at(k).eta() < etamax2) {
	    //event_->vbfCenJet2Et = jets.at(k).et();
	    break;
	  }
	}

      }

    }
  }


}


void InvHiggsAODProducer::doPUReweighting(const edm::Event& iEvent) {

//   edm::Handle<std::vector< PileupSummaryInfo > >  puInfo;
//   iEvent.getByLabel(edm::InputTag("addPileupInfo"), puInfo);
  
//   std::vector<PileupSummaryInfo>::const_iterator pvi;
  
//   float tnpv = -1;
//   for(pvi = puInfo->begin(); pvi != puInfo->end(); ++pvi) {
    
//     int bx = pvi->getBunchCrossing();
    
//     if(bx == 0) { 
//       tnpv = pvi->getTrueNumInteractions();
//       continue;
//     }
    
//   }
  
//   double weight = lumiWeights_.weight( tnpv );
  
//   // example code below does not compile!
//   //edm::EventBase* iEventB = dynamic_cast<edm::EventBase*>(&iEvent);
//   //double weight = lumiWeights_.weight( (*iEventB) );
  
//   event_->puWeight = weight;

}


//define this as a plug-in
DEFINE_FWK_MODULE(InvHiggsAODProducer);
