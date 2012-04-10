// -*- C++ -*-
//
// Package:    InvisibleHiggs/Analysis
// Class:      TTreeProducer
// 
/**\class TreeProducer TreeProducer.cc InvisibleHiggs/Analysis/src/TreeProducer.cc

 Description: Produce invisible Higgs TTree

 Implementation:
     
*/
//
// Original Author:  Jim Brooke
//         Created:  
// $Id: $
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

// candidates
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/MHT.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

// Vertices
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

// Beam Halo Summary
#include "DataFormats/METReco/interface/BeamHaloSummary.h"

// MC
#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"

// ROOT output stuff
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH1.h"
#include "TTree.h"
#include "TF1.h"

// TTree definition
#include "InvisibleHiggs/Ntuple/interface/Event.h"


//
// class declaration
//

using namespace reco;

class TreeProducer : public edm::EDAnalyzer {
public:
  explicit TreeProducer(const edm::ParameterSet&);
  ~TreeProducer();
  
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
  void doJets(const edm::Event&, const edm::EventSetup&);
  void doMuons(const edm::Event&);
  void doElectrons(const edm::Event&);
  void doVertices(const edm::Event&);
  void doGlobal(const edm::Event&);

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
  Event* event_;

  // EDM input tags
  std::string l1JetsTag_;
  edm::InputTag l1BitsTag_;
  edm::InputTag hltResultsTag_;
  edm::InputTag hltEventTag_;
  std::string hltPathName_;
  edm::InputTag hltL3Tag_;
  edm::InputTag mcTag_;
  std::string mcProducer_;
  edm::InputTag hepProducer_;
  std::string jetCorrectorServiceName_;
  edm::InputTag jetTag_;
  edm::InputTag muonTag_;
  edm::InputTag electronTag_;
  edm::InputTag metTag_;
  edm::InputTag vertexTag_;

  // HLT config helper
  HLTConfigProvider hltConfig_;
  unsigned hltBit_;
  bool doHltBit_;

};




TreeProducer::TreeProducer(const edm::ParameterSet& iConfig):
  tree_(0),
  event_(0),
  l1JetsTag_(iConfig.getUntrackedParameter<std::string>("l1JetsTag",std::string("l1extraParticles"))),
  l1BitsTag_(iConfig.getUntrackedParameter<edm::InputTag>("l1BitsTag",edm::InputTag("gtDigis"))),
  hltResultsTag_(iConfig.getUntrackedParameter<edm::InputTag>("hltResultsTag",edm::InputTag("TriggerResults","","HLT"))),
  hltEventTag_(iConfig.getUntrackedParameter<edm::InputTag>("hltEventTag",edm::InputTag("hltTriggerSummaryAOD","","HLT"))),
  hltPathName_(iConfig.getUntrackedParameter<std::string>("hltPathName",std::string("HLT_v1"))),
  hltL3Tag_(iConfig.getUntrackedParameter<edm::InputTag>("hltL3Tag",edm::InputTag("hltFilter","","HLT"))),
  mcTag_(iConfig.getUntrackedParameter<edm::InputTag>("mcTag",edm::InputTag("generator"))),
  mcProducer_ (iConfig.getUntrackedParameter<std::string>("producer", "g4SimHits")),
  hepProducer_ (iConfig.getUntrackedParameter<edm::InputTag>("hepMCProducerTag", edm::InputTag("generator", "", "SIM"))),
  jetCorrectorServiceName_(iConfig.getUntrackedParameter<std::string>("jetCorrectorServiceName","ic5CaloL1L2L3Residual")),
  jetTag_(iConfig.getUntrackedParameter<edm::InputTag>("jetTag",edm::InputTag("patPFJets"))),
  muonTag_(iConfig.getUntrackedParameter<edm::InputTag>("muonTag",edm::InputTag("patMuons"))),
  electronTag_(iConfig.getUntrackedParameter<edm::InputTag>("electronTag",edm::InputTag("patElectrons"))),
  metTag_(iConfig.getUntrackedParameter<edm::InputTag>("metTag",edm::InputTag("patCaloMET"))),
  vertexTag_(iConfig.getUntrackedParameter<edm::InputTag>("verticesTag", edm::InputTag("offlinePrimaryVertices")))
{
  // set up output
  tree_=fs_->make<TTree>("InvHiggsTree", "");
  tree_->Branch("events", "Event", &event_, 64000, 1);
  
}


TreeProducer::~TreeProducer() {

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called once each job just before starting event loop  ------------
void TreeProducer::beginJob()
{
}

// -- called once per run
void TreeProducer::beginRun(edm::Run const & iRun, edm::EventSetup const& iSetup)
{
  // Get PDT Table if MC
  //iSetup.getData(fPDGTable);

  // HLT setup
  bool changed;
  hltConfig_.init(iRun, iSetup, hltResultsTag_.process(), changed);
  
  // HLT Path -- No BPTX
  try{
    hltBit_=(hltConfig_.triggerNames()).size();  // default setting -- trigger not found
    for (uint i=0;i<(hltConfig_.triggerNames()).size();++i) // loop over trigger names
      {
	std::string trigName=hltConfig_.triggerName(i);

	if (hltPathName_.size()>0 &&
	    trigName.find(hltPathName_)!= std::string::npos)
	  {
	    hltBit_ = hltConfig_.triggerIndex(trigName); // could just set to i, but let's be careful...
	  }
      }
    if (hltBit_==(hltConfig_.triggerNames()).size())
      {
	edm::LogWarning("TreeProducer") << "Could not find an HLT path matching "<<hltPathName_<<".  Branch will not be filled."<<std::endl;
	doHltBit_ = false;
      }
    else
      edm::LogInfo("TreeProducer") << hltPathName_ << " index is " << hltBit_ << std::endl;
  } // end of try loop
  catch (cms::Exception e) {
    edm::LogWarning("TreeProducer") << "HLTJetNoBPTX:  Could not find an HLT path matching " << hltPathName_ << ".  Branch will not be filled" << std::endl;
    doHltBit_ = false;
  }

}


// ------------ method called once each job just after ending the event loop  ------------
void 
TreeProducer::endJob() {
}

// ------------ method called to for each event  ------------
void
TreeProducer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
 
  event_ = new Event();
 
  //  doMC(iEvent);
 
  // event & trigger info
  doEventInfo(iEvent);
  doTrigger(iEvent, iSetup);

  // general RECO info
  doJets(iEvent, iSetup);
  doMuons(iEvent);
  doElectrons(iEvent);
  doVertices(iEvent);
  doGlobal(iEvent);

  // fill TTree
  tree_->Fill();
  
  delete event_;

}



void TreeProducer::doMC(const edm::Event& iEvent) {

  edm::Handle<edm::HepMCProduct> mcHandle;
  iEvent.getByLabel(mcTag_,mcHandle);
  
  if (mcHandle.isValid()) {
    
    const edm::HepMCProduct *mcProd = mcHandle.product();
    const HepMC::GenEvent *evt = mcProd->GetEvent();
    
  }

}

void TreeProducer::doEventInfo(const edm::Event& iEvent) {

  unsigned long id          = iEvent.id().event();
  unsigned long bx          = iEvent.bunchCrossing();
  unsigned long orbit       = iEvent.orbitNumber();
  unsigned long lb          = iEvent.luminosityBlock();
  unsigned long run         = iEvent.id().run();
  double time               = iEvent.time().value();

  event_->id = id;
  event_->bx = bx;
  event_->lb = lb;
  event_->run = run;

}
  

void TreeProducer::doTrigger(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

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


  bool hltResult(false);

  // get HLT results
  edm::Handle<edm::TriggerResults> HLTR;
  iEvent.getByLabel(hltResultsTag_, HLTR);

  if (HLTR.isValid()) {
    // triggerIndex must be less than the size of HLTR or you get a CMSException: _M_range_check
    if (doHltBit_ && hltBit_ < HLTR->size()) hltResult = HLTR->accept(hltBit_);
  }
  else {
    if (doHltBit_) edm::LogWarning("MissingProduct") << "HLT information not found for HltBit1.  Branch will not be filled" << std::endl;
    doHltBit_ = false;
  }

  // store bits
  event_->hltResult = hltResult; 

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



void TreeProducer::doJets(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  //  const JetCorrector* jetCorrector = JetCorrector::getJetCorrector(jetCorrectorServiceName_,iSetup);

  edm::Handle<reco::PFCandidateCollection> jets;
   iEvent.getByLabel(jetTag_, jets);
   unsigned njet=0;

   if (jets.isValid()) {

     for(PFCandidateCollection::const_iterator it=jets->begin(); 
	 it!=jets->end();
	 ++it, ++njet) {

       //	 edm::RefToBase<reco::PFJet> jetRef(edm::Ref<reco::PFJetCollection>(jets,njet));  
       //	 double scale = jetCorrector->correction(*it,jetRef,iEvent,iSetup);

	 // store jet in TTree
	 double et = it->et();
	 double et_corr = it->et();
	 double eta = it->eta();
	 double phi = it->phi();
	 double etEm = 0.; //it->emEnergyInEB();
	 double etHad = 0.; //it->hadEnergyInHB();
	 int n60 = 0; //it->n60();
	 int n90 = 0; //it->n90();
	 double fhpd = 0.;
	 int n90hits = 0;

	 event_->addJet(et, eta, phi, etEm, etHad, n60, n90, fhpd, n90hits); 

     } // loop over jets
   } // if (caloJets.isValid())
 
}


void TreeProducer::doMuons(const edm::Event& iEvent) {

  // loop over reco muons
  edm::Handle<pat::MuonCollection> muons;
  iEvent.getByLabel(muonTag_,muons);

  if (muons.isValid()) {
    for(pat::MuonCollection::const_iterator it =muons->begin();
	it!=muons->end();
	it++) {

      double pt = it->pt();
      double eta = it->eta();
      double phi = it->phi();
      int type = (0xf & it->type());

      event_->addMuon(pt, eta, phi, type);

    }
  }
  
}


void TreeProducer::doElectrons(const edm::Event& iEvent) {

  edm::Handle<pat::ElectronCollection> electrons;
  iEvent.getByLabel(electronTag_,electrons);

  if (electrons.isValid()) {
    for(pat::ElectronCollection::const_iterator it =electrons->begin();
	it!=electrons->end();
	it++) {

      double pt = it->pt();
      double eta = it->eta();
      double phi = it->phi();

      event_->addElectron(pt, eta, phi);

    }
  }
  
}


void TreeProducer::doVertices(const edm::Event& iEvent) {

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
  
} // void TreeProducer::doVertices(const edm::Event& iEvent)


void TreeProducer::doGlobal(const edm::Event& iEvent) {

  // MJJ
  



  // calo MET



  // PFMHT
  



  // beam halo
  edm::Handle<BeamHaloSummary> haloSummary;
  iEvent.getByLabel("BeamHaloSummary", haloSummary);

  if (haloSummary.isValid()) {
    event_->beamHaloCSCTight = haloSummary->CSCTightHaloId();
    event_->beamHaloCSCLoose = haloSummary->CSCLooseHaloId();
  }

  return;
}



//define this as a plug-in
DEFINE_FWK_MODULE(TreeProducer);
