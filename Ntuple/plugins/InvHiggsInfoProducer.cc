// -*- C++ -*-
//
// Package:    InvisibleHiggs/Analysis
// Class:      TInvHiggsInfoProducer
// 
/**\class InvHiggsInfoProducer InvHiggsInfoProducer.cc InvisibleHiggs/Analysis/src/InvHiggsInfoProducer.cc

 Description: Produce invisible Higgs TInfo

 Implementation:
     
*/
//
// Original Author:  Jim Brooke
//         Created:  
// $Id: InvHiggsInfoProducer.cc,v 1.4 2012/07/27 09:07:57 jbrooke Exp $
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

// Electrons
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"

// MET
#include "DataFormats/METReco/interface/CaloMETFwd.h"
#include "DataFormats/METReco/interface/PFMETFwd.h"

// PAT candidates
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
#include "TMath.h"

// TTree definition
#include "InvisibleHiggs/Ntuple/interface/InvHiggsInfo.h"


#include <algorithm>


//
// class declaration
//

using namespace reco;

class InvHiggsInfoProducer : public edm::EDAnalyzer {
public:
  explicit InvHiggsInfoProducer(const edm::ParameterSet&);
  ~InvHiggsInfoProducer();
  
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

  // write PAT objects
  void doJets(std::vector<pat::Jet> jets);
  void doMuons(std::vector<pat::Muon> muons);
  void doElectrons(std::vector<pat::Electron> electrons);
  void doMET(std::vector<pat::MET> met);
  void doMHT(std::vector<pat::MHT> mht);

  void doVBFVariables(std::vector<pat::Jet> jets, std::vector<pat::MET> met, bool leading);

  // other stuff
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
  InvHiggsInfo* info_;

  // EDM input tags
  edm::InputTag hltResultsTag_;
  std::string hltPath1Name_;
  std::string hltPath2Name_;
//   edm::InputTag mcTag_;
//   std::string mcProducer_;
//   edm::InputTag hepProducer_;
  edm::InputTag jetTag_;
  edm::InputTag muonTag_;
  edm::InputTag electronTag_;
  edm::InputTag metTag_;
  edm::InputTag mhtTag_;

  // loigic
  bool isMC_;
  bool useLeadingJets_;

  // HLT config helper
  HLTConfigProvider hltConfig_;
  unsigned hltBit1_;
  unsigned hltBit2_;
  bool doHltBit_;

  // PU re-weighting
  edm::LumiReWeighting lumiWeights_;

};




InvHiggsInfoProducer::InvHiggsInfoProducer(const edm::ParameterSet& iConfig):
  tree_(0),
  info_(0),
  hltResultsTag_(iConfig.getUntrackedParameter<edm::InputTag>("hltResultsTag",edm::InputTag("TriggerResults","","HLT"))),
  hltPath1Name_(iConfig.getUntrackedParameter<std::string>("hltPath1Name",std::string("HLT_v1"))),
  hltPath2Name_(iConfig.getUntrackedParameter<std::string>("hltPath2Name",std::string("HLT_v1"))),
//   mcTag_(iConfig.getUntrackedParameter<edm::InputTag>("mcTag",edm::InputTag("generator"))),
//   mcProducer_ (iConfig.getUntrackedParameter<std::string>("producer", "g4SimHits")),
//   hepProducer_ (iConfig.getUntrackedParameter<edm::InputTag>("hepMCProducerTag", edm::InputTag("generator", "", "SIM"))),
  jetTag_(iConfig.getUntrackedParameter<edm::InputTag>("jetTag",edm::InputTag("patJets"))),
  muonTag_(iConfig.getUntrackedParameter<edm::InputTag>("muonTag",edm::InputTag("patMuons"))),
  electronTag_(iConfig.getUntrackedParameter<edm::InputTag>("electronTag",edm::InputTag("patElectrons"))),
  metTag_(iConfig.getUntrackedParameter<edm::InputTag>("metTag",edm::InputTag("patMET"))),
  mhtTag_(iConfig.getUntrackedParameter<edm::InputTag>("mhtTag",edm::InputTag("patMHT"))),
  isMC_(iConfig.getUntrackedParameter<bool>("isMC",false)),
  useLeadingJets_(iConfig.getUntrackedParameter<bool>("useLeadingJets",true)),
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
  tree_=fs_->make<TTree>("InvHiggsInfo", "");
  tree_->Branch("events", "InvHiggsInfo", &info_, 1000000, 1);
  
}


InvHiggsInfoProducer::~InvHiggsInfoProducer() {

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called once each job just before starting event loop  ------------
void InvHiggsInfoProducer::beginJob()
{
}

// -- called once per run
void InvHiggsInfoProducer::beginRun(edm::Run const & iRun, edm::EventSetup const& iSetup)
{
  // Get PDT Table for MC
  //iSetup.getData(fPDGTable);

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
	edm::LogWarning("InvHiggsInfoProducer") << "Could not find an HLT path matching "<<hltPath1Name_<<std::endl;
      }
    if (hltBit2_==(hltConfig_.triggerNames()).size())
      {
	edm::LogWarning("InvHiggsInfoProducer") << "Could not find an HLT path matching "<<hltPath2Name_<<std::endl;
      }
    else
      edm::LogInfo("InvHiggsInfoProducer") << hltPath1Name_ << " index is " << hltBit1_ << std::endl;
      edm::LogInfo("InvHiggsInfoProducer") << hltPath2Name_ << " index is " << hltBit2_ << std::endl;
  } // end of try loop
  catch (cms::Exception e) {
    edm::LogWarning("InvHiggsInfoProducer") << "Exception while trying to find HLT bit numbers" << std::endl;
    edm::LogWarning("InvHiggsInfoProducer") << e << std::endl;
  }

}


// ------------ method called once each job just after ending the event loop  ------------
void 
InvHiggsInfoProducer::endJob() {
}

// ------------ method called to for each event  ------------
void
InvHiggsInfoProducer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
 
  info_ = new InvHiggsInfo();
 
  if (! iEvent.eventAuxiliary().isRealData()) {
    doMC(iEvent);
    doPUReweighting(iEvent);
  }
 
  // event & trigger info
  doEventInfo(iEvent);
  doTrigger(iEvent, iSetup);

  edm::Handle<pat::METCollection> met;
  iEvent.getByLabel(metTag_, met);

  edm::Handle<pat::MHTCollection> mht;
  iEvent.getByLabel(mhtTag_, mht);

  edm::Handle<pat::JetCollection> jets;
  iEvent.getByLabel(jetTag_, jets);

  edm::Handle<pat::MuonCollection> muons;
  iEvent.getByLabel(muonTag_,muons);

  edm::Handle<pat::ElectronCollection> electrons;
  iEvent.getByLabel(electronTag_,electrons);

  if (jets.isValid()) {
    doJets(*jets);
  }

  if (met.isValid()) {
    doMET(*met);
  }

  if (mht.isValid()) {
    doMHT(*mht);
  }

  if (electrons.isValid()) {
    doElectrons(*electrons);
  }

  if (muons.isValid()) {
    doMuons(*muons);
  }

  if (jets.isValid() && met.isValid()) {
    doVBFVariables(*jets, *met, useLeadingJets_);
  }

  // fill TInfo
  tree_->Fill();
  
  delete info_;

}



void InvHiggsInfoProducer::doMC(const edm::Event& iEvent) {

//   edm::Handle<edm::HepMCProduct> mcHandle;
//   iEvent.getByLabel(mcTag_,mcHandle);
  
//   if (mcHandle.isValid()) {
    
//     const edm::HepMCProduct *mcProd = mcHandle.product();
//     const HepMC::GenEvent *evt = mcProd->GetEvent();
    
//   }

}

void InvHiggsInfoProducer::doEventInfo(const edm::Event& iEvent) {

  unsigned long id          = iEvent.id().event();
  unsigned long bx          = iEvent.bunchCrossing();
  unsigned long orbit       = iEvent.orbitNumber();
  unsigned long lb          = iEvent.luminosityBlock();
  unsigned long run         = iEvent.id().run();
  double time               = iEvent.time().value();

  info_->id = id;
  info_->bx = bx;
  info_->lb = lb;
  info_->run = run;

}
  

void InvHiggsInfoProducer::doTrigger(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

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
  info_->hltResult1 = hltResult1; 
  info_->hltResult2 = hltResult2; 

  // Store HLT prescale info
//   info_->hltPrescaleIndex=hltConfig_.prescaleSet(iEvent, iSetup);
//   info_->hltPrescale=hltConfig_.prescaleValue(iEvent, iSetup, hltPathName_);
  
}


void InvHiggsInfoProducer::doJets(std::vector<pat::Jet> jets) {

  unsigned njet=0;
  
  if (jets.size()>0) {
    info_->jet1Pt = jets.at(0).pt();    
    info_->jet1Eta = jets.at(0).eta();    
    info_->jet1Phi = jets.at(0).phi();    
    info_->jet1Mass = jets.at(0).mass();    
  }
  if (jets.size() > 1) {
    info_->jet2Pt = jets.at(1).pt();    
    info_->jet2Eta = jets.at(1).eta();    
    info_->jet2Phi = jets.at(1).phi();    
    info_->jet2Mass = jets.at(1).mass();    
  }

}

void InvHiggsInfoProducer::doMuons(std::vector<pat::Muon> muons) {

  if (muons.size()>0) {
    info_->mu1Pt = muons.at(0).pt();    
    info_->mu1Eta = muons.at(0).eta();    
    info_->mu1Phi = muons.at(0).phi();    
  }
  if (muons.size()>1) {
    info_->mu2Pt = muons.at(1).pt();    
    info_->mu2Eta = muons.at(1).eta();    
    info_->mu2Phi = muons.at(1).phi();    
    
    // leading pair mass
    math::XYZTLorentzVector pair = muons.at(0).p4() + muons.at(1).p4();
    info_->mMuMu = pair.M();
  }
  
}


void InvHiggsInfoProducer::doElectrons(std::vector<pat::Electron> electrons) {

  if (electrons.size()>0) {
    info_->ele1Pt = electrons.at(0).pt();    
    info_->ele1Eta = electrons.at(0).eta();    
    info_->ele1Phi = electrons.at(0).phi();    
  }
  if (electrons.size()>1) {
    info_->ele2Pt = electrons.at(1).pt();    
    info_->ele2Eta = electrons.at(1).eta();    
    info_->ele2Phi = electrons.at(1).phi();    
    
    // leading pair mass
    math::XYZTLorentzVector pair = electrons.at(0).p4() + electrons.at(1).p4();
    info_->mEE = pair.M();
  }
  
}


void InvHiggsInfoProducer::doMET(std::vector<pat::MET> met) {

  info_->met    = met.at(0).pt();
  info_->metPhi = met.at(0).phi();
  //    info_->metSig = met->at(0).mEtSig();

}

void InvHiggsInfoProducer::doMHT(std::vector<pat::MHT> mht) {

  info_->mht    = mht.at(0).pt();
  info_->mhtPhi = mht.at(0).phi();
  //    info_->mhtSig = mht->at(0).significance();

}


/// identify tag jets and store some variables
void InvHiggsInfoProducer::doVBFVariables(std::vector<pat::Jet> jets, std::vector<pat::MET> met, bool leading) {

  bool vbfFound = false;

  for (unsigned i=0; i<jets.size()-1 && !vbfFound; ++i) {
    for (unsigned j=i+1; j<jets.size() && !vbfFound; ++j) {

      // VBF jet vector sum
      math::XYZTLorentzVector v1=jets.at(i).p4();
      math::XYZTLorentzVector v2=jets.at(j).p4();
      math::XYZTLorentzVector vbfp4=v1+v2;
      
      double deta = fabs(jets.at(i).eta() - jets.at(j).eta());
      
      // stop if this pair passes VBF conditions
      // or is the first pair
      if (leading ||
	  (v1.Pt() > 30. &&
      	  v2.Pt() > 30. &&
      	  deta > 3.5 &&
	   vbfp4.M() > 800.)) {
	
      	vbfFound = true;
	
//       	info_->vbfJet1Index = i;
//       	info_->vbfJet2Index = j;
      
	info_->vbfEt  = vbfp4.Pt();
	info_->vbfEta = vbfp4.Eta();
	info_->vbfPhi = vbfp4.Phi();
	info_->vbfM   = vbfp4.M();
	
	// find highest Et jet not in VBF pair
	for (unsigned k=0; k< jets.size(); ++k) {
	  if (k!=i && k!=j) {
	    info_->vbfJet3Phi = jets.at(k).phi();	    
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
	  info_->vbfNearJetEt = jets.at(nrJetIndex).et();
	  info_->vbfNearJetDR = dRmin;
	}
	
	// find highest et jet with eta between tag jets
	double etamin = std::min(v1.Eta(), v2.Eta());
	double etamax = std::max(v1.Eta(), v2.Eta());
	for (unsigned k=0; k< jets.size(); ++k) {
	  if (jets.at(k).eta() > etamin &&
	      jets.at(k).eta() < etamax) {
	    info_->vbfCenJetEt = jets.at(k).et();
	    break;
	  }
	}
	double etamin2 = std::min(v1.Eta(), v2.Eta())+0.5;
	double etamax2 = std::max(v1.Eta(), v2.Eta())-0.5;
	for (unsigned k=0; k< jets.size(); ++k) {
	  if (jets.at(k).eta() > etamin2 &&
	      jets.at(k).eta() < etamax2) {
	    //info_->vbfCenJet2Et = jets.at(k).et();
	    break;
	  }
	}

      }
	
    }
  }

  double jmdphi1 = abs(abs(abs(met.at(0).phi()-jets.at(0).phi())-TMath::Pi())-TMath::Pi());
  double jmdphi2 = abs(abs(abs(met.at(0).phi()-jets.at(1).phi())-TMath::Pi())-TMath::Pi());

  info_->jetMETdPhi = std::min(jmdphi1, jmdphi2);


}


void InvHiggsInfoProducer::doPUReweighting(const edm::Event& iEvent) {
  
  double weight = 0.;

//   edm::Handle<std::vector< PileupSummaryInfo > >  puInfo;
//   iEvent.getByLabel(edm::InputTag("addPileupInfo"), puInfo);
  
//   if (puInfo.isValid()) {
//     std::vector<PileupSummaryInfo>::const_iterator pvi;
    
//     float tnpv = -1;
//     for(pvi = puInfo->begin(); pvi != puInfo->end(); ++pvi) {
      
//       int bx = pvi->getBunchCrossing();
      
//       if(bx == 0) { 
// 	tnpv = pvi->getTrueNumInteractions();
// 	continue;
//       }
      
//     }
    
//     weight = lumiWeights_.weight( tnpv );
    
    // example code below does not compile!
    //edm::EventBase* iEventB = dynamic_cast<edm::EventBase*>(&iEvent);
    //double weight = lumiWeights_.weight( (*iEventB) );
  //  }
 
  info_->puWeight = weight;

}


//define this as a plug-in
DEFINE_FWK_MODULE(InvHiggsInfoProducer);
