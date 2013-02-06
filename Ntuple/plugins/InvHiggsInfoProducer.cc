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
// $Id: InvHiggsInfoProducer.cc,v 1.17 2013/01/22 20:12:25 chayanit Exp $
//
//


// framework
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Common/interface/ConditionsInEdm.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/ValueMap.h"


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

// PU jet ID
#include "CMGTools/External/interface/PileupJetIdentifier.h"

// vertices
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

// Beam Halo Summary
#include "DataFormats/METReco/interface/BeamHaloSummary.h"

// MC
#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

// Lumi
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"


// Math
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/deltaR.h"

// ROOT output stuff
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH1.h"
#include "TTree.h"
#include "TF1.h"
#include "TMath.h"

// TTree definition
#include "InvisibleHiggs/Ntuple/interface/InvHiggsInfo.h"


// system include files
#include <memory>
#include <sys/stat.h>

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
  void doMC(const GenEventInfoProduct& genEvt, const GenParticleCollection& genParticles);
  int hadronicTau(const reco::Candidate*);

  /// write trigger info
  void doTrigger(const edm::Event& iEvent, 
		 const edm::EventSetup& iSetup);

  /// write met filter info
  void doMETFilter(const edm::Event& iEvent, 
		   const edm::EventSetup& iSetup,
		   edm::Handle<edm::TriggerResults> metFlag);

  // PU re-weighting
  void doVertices(const std::vector<reco::Vertex>& vertices);
  void doPUReweighting(const edm::Event&);

  // write PAT objects
  void doJets(edm::Handle<edm::View<pat::Jet> > jets,
	      edm::Handle<edm::ValueMap<float> > puJetIdMVAs,
	      edm::Handle<edm::ValueMap<int> > puJetIdFlags,
	      const std::vector<pat::Muon>& muons,               //Loose muons
	      const std::vector<pat::Electron>& electrons);      //Loose electrons
  void doThirdJet(edm::Handle<edm::View<pat::Jet> > jets,
		  edm::Handle<edm::ValueMap<float> > puJetIdMVAs,
		  edm::Handle<edm::ValueMap<int> > puJetIdFlags,
		  const std::vector<pat::Muon>& muons,           //Loose muons
		  const std::vector<pat::Electron>& electrons);  //Loose electrons
  void doMuons(const std::vector<pat::Muon>& muons);             //Loose muons
  void doElectrons(const std::vector<pat::Electron>& electrons); //Loose electrons
  void doMET(const std::vector<pat::MET>& met,
	     const std::vector<pat::Muon>& muons,                //Tight muons
	     const std::vector<pat::Muon>& looseMuons,           //Loose muons
	     const std::vector<pat::Electron>& electrons,        //Tight electrons
	     const std::vector<pat::Electron>& looseElectrons);  //Loose electrons
  void doMHT(const std::vector<pat::MHT>& mht);
  void doZs(const reco::CandidateView& zs, int channel);
  void doWs(const reco::CandidateView& ws, int channel);

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

  // MC config
  edm::InputTag genEvtTag_;
  edm::InputTag genParticleTag_;
  bool mcPYTHIA_;

  edm::InputTag hltResultsTag_;
  std::string hltPath1Name_;
  std::string hltPath2Name_;
  edm::InputTag metResultsTag_;
  edm::InputTag vtxTag_;
  edm::InputTag jetTag_;
  edm::InputTag puJetMvaTag_;
  edm::InputTag puJetIdTag_;
  //
  edm::InputTag muonTag_;
  edm::InputTag looseMuonTag_;
  //
  edm::InputTag electronTag_;
  edm::InputTag looseElectronTag_;
  //
  edm::InputTag metTag_;
  edm::InputTag mhtTag_;
  edm::InputTag zMuTag_;
  edm::InputTag zElTag_;
  edm::InputTag wMuTag_;
  edm::InputTag wElTag_;
  
  // logic

  bool isMC_;
  bool useLeadingJets_;
  bool doPUWeights_;
  bool doPUJetID_;

  // HLT config helper
  HLTConfigProvider hltConfig_;
  unsigned hltBit1_;
  unsigned hltBit2_;
  bool doHltBit_;

  // PU re-weighting
  edm::LumiReWeighting lumiWeights_;

  // VBF jets
  unsigned tagJet1Index_;
  unsigned tagJet2Index_;
  math::XYZTLorentzVector tagJet1_;
  math::XYZTLorentzVector tagJet2_;
  double tagJetEtaMin_;
  double tagJetEtaMax_;

};




InvHiggsInfoProducer::InvHiggsInfoProducer(const edm::ParameterSet& iConfig):
  tree_(0),
  info_(0),

  // MC
  genEvtTag_(iConfig.getUntrackedParameter<edm::InputTag>("genEvtTag",edm::InputTag("generator","","SIM"))),
  genParticleTag_(iConfig.getUntrackedParameter<edm::InputTag>("genParticleTag", edm::InputTag("genParticles","","SIM"))),
  mcPYTHIA_(iConfig.getUntrackedParameter<bool>("mcPYTHIA",true)),

  // Trigger
  hltResultsTag_(iConfig.getUntrackedParameter<edm::InputTag>("hltResultsTag",edm::InputTag("TriggerResults","","HLT"))),
  hltPath1Name_(iConfig.getUntrackedParameter<std::string>("hltPath1Name",std::string("HLT_v1"))),
  hltPath2Name_(iConfig.getUntrackedParameter<std::string>("hltPath2Name",std::string("HLT_v1"))),

  // MET filters
  metResultsTag_(iConfig.getUntrackedParameter<edm::InputTag>("metResultsTag",edm::InputTag("TriggerResults","","PAT"))),

  // MC information
  //mcTag_(iConfig.getUntrackedParameter<edm::InputTag>("mcTag",edm::InputTag("generator"))),
  //mcProducer_ (iConfig.getUntrackedParameter<std::string>("producer", "g4SimHits")),
  //hepProducer_ (iConfig.getUntrackedParameter<edm::InputTag>("hepMCProducerTag", edm::InputTag("generator", "", "SIM"))),
  
  // Physics objects
  vtxTag_(iConfig.getUntrackedParameter<edm::InputTag>("vtxTag",edm::InputTag("offlinePrimaryVertices"))),
  jetTag_(iConfig.getUntrackedParameter<edm::InputTag>("jetTag",edm::InputTag("patJets"))),
  puJetMvaTag_(iConfig.getUntrackedParameter<edm::InputTag>("puJetMvaTag",edm::InputTag("puJetMva:fullDiscriminant"))),
  puJetIdTag_(iConfig.getUntrackedParameter<edm::InputTag>("puJetIdTag",edm::InputTag("puJetMva:fullId"))),
  
  muonTag_(iConfig.getUntrackedParameter<edm::InputTag>("muonTag",edm::InputTag("patMuons"))),
  looseMuonTag_(iConfig.getUntrackedParameter<edm::InputTag>("looseMuonTag",edm::InputTag("patMuons"))),
  
  electronTag_(iConfig.getUntrackedParameter<edm::InputTag>("electronTag",edm::InputTag("patElectrons"))),
  looseElectronTag_(iConfig.getUntrackedParameter<edm::InputTag>("looseElectronTag",edm::InputTag("patElectrons"))),
  
  metTag_(iConfig.getUntrackedParameter<edm::InputTag>("metTag",edm::InputTag("patMET"))),
  mhtTag_(iConfig.getUntrackedParameter<edm::InputTag>("mhtTag",edm::InputTag("patMHT"))),

  // W,Z candidates
  zMuTag_(iConfig.getUntrackedParameter<edm::InputTag>("zMuTag",edm::InputTag(""))),
  zElTag_(iConfig.getUntrackedParameter<edm::InputTag>("zElTag",edm::InputTag(""))),
  wMuTag_(iConfig.getUntrackedParameter<edm::InputTag>("wMuTag",edm::InputTag(""))),
  wElTag_(iConfig.getUntrackedParameter<edm::InputTag>("wElTag",edm::InputTag(""))),
  
  //
  isMC_(iConfig.getUntrackedParameter<bool>("isMC",false)),
  useLeadingJets_(iConfig.getUntrackedParameter<bool>("useLeadingJets",true)),
  doPUWeights_(false),
  doPUJetID_(iConfig.getUntrackedParameter<bool>("doPUJetID",true)),
  hltConfig_(),
  hltBit1_(0),
  hltBit2_(0),
  doHltBit_(true),
  lumiWeights_(),
  tagJet1Index_(-1),
  tagJet2Index_(-1),
  tagJetEtaMin_(0.),
  tagJetEtaMax_(0.)
  
{
  // set up output
  tree_=fs_->make<TTree>("InvHiggsInfo", "");
  tree_->Branch("events", "InvHiggsInfo", &info_, 1000000, 1);

  // check PU files exists, and reweight if they do
  std::string puMCFile = iConfig.getUntrackedParameter<std::string>("puMCFile", "");
  std::string puDataFile = iConfig.getUntrackedParameter<std::string>("puDataFile", "");

  struct stat buf;
  if ((stat(puMCFile.c_str(), &buf) != -1) && (stat(puDataFile.c_str(), &buf) != -1)) {
    lumiWeights_ = edm::LumiReWeighting(puMCFile,
					puDataFile,
					iConfig.getUntrackedParameter<std::string>("puMCHist", "pileup"),
					iConfig.getUntrackedParameter<std::string>("puDataHist", "pileup"));
    doPUWeights_ = true;
  }
  else {
    edm::LogWarning("InvHiggsInfoProducer") << "No PU reweighting inputs - not going to calculate weights"<<std::endl;
  }
  
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

    edm::Handle<GenEventInfoProduct> genEvt;
    iEvent.getByLabel(genEvtTag_,genEvt);
    
    edm::Handle<GenParticleCollection> genParticles;
    iEvent.getByLabel(genParticleTag_, genParticles);   
    
    if (genEvt.isValid() && genParticles.isValid()) doMC(*genEvt, *genParticles);
    else {
      edm::LogWarning("InvHiggsInfoProducer") << "GenEventInfoProduct not found, branch will not be filled!" << std::endl;
    }
    
    if (doPUWeights_) doPUReweighting(iEvent);

  }
 
  // event & trigger info
  doEventInfo(iEvent);
  doTrigger(iEvent, iSetup);

  // met filters
  edm::Handle<edm::TriggerResults> metFlag;
  iEvent.getByLabel(metResultsTag_, metFlag);
  //if (!metFlag.isValid()) return;
  doMETFilter(iEvent, iSetup, metFlag);

  edm::Handle<std::vector<reco::Vertex> > vertices;
  iEvent.getByLabel(vtxTag_, vertices);

  edm::Handle<edm::View<pat::Jet> > jets;
  iEvent.getByLabel(jetTag_, jets);

  edm::Handle<edm::ValueMap<float> > puJetIdMVA;
  iEvent.getByLabel(puJetMvaTag_,puJetIdMVA);
  
  edm::Handle<edm::ValueMap<int> > puJetIdFlag;
  iEvent.getByLabel(puJetIdTag_,puJetIdFlag);

  edm::Handle<pat::METCollection> met;
  iEvent.getByLabel(metTag_, met);

  edm::Handle<pat::MHTCollection> mht;
  iEvent.getByLabel(mhtTag_, mht);

  edm::Handle<pat::MuonCollection> muons;
  iEvent.getByLabel(muonTag_,muons);

  edm::Handle<pat::MuonCollection> looseMuons;
  iEvent.getByLabel(looseMuonTag_,looseMuons);

  edm::Handle<pat::ElectronCollection> electrons;
  iEvent.getByLabel(electronTag_,electrons); 

  edm::Handle<pat::ElectronCollection> looseElectrons;
  iEvent.getByLabel(looseElectronTag_,looseElectrons);

  edm::Handle<reco::CandidateView> zMus;
  iEvent.getByLabel(zMuTag_, zMus);

  edm::Handle<reco::CandidateView> zEls;
  iEvent.getByLabel(zElTag_, zEls);

  edm::Handle<reco::CandidateView> wMus;
  iEvent.getByLabel(wMuTag_, wMus);

  edm::Handle<reco::CandidateView> wEls;
  iEvent.getByLabel(wElTag_, wEls);

  if (vertices.isValid()) doVertices(*vertices);

  // do jets before MET etc. because this defines the VBF pair
  if (jets.isValid()) doJets(jets, puJetIdMVA, puJetIdFlag, *looseMuons, *looseElectrons);

  if (jets.isValid()) doThirdJet(jets, puJetIdMVA, puJetIdFlag, *looseMuons, *looseElectrons);

  if (met.isValid()) doMET(*met, *muons, *looseMuons, *electrons, *looseElectrons);

  if (mht.isValid()) doMHT(*mht);

  if (looseMuons.isValid()) doMuons(*looseMuons);

  if (looseElectrons.isValid()) doElectrons(*looseElectrons);
  
  if (zMus.isValid()) doZs(*zMus, 1);

  if (zEls.isValid()) doZs(*zEls, 2);

  if (wMus.isValid()) doWs(*wMus, 1);

  if (wEls.isValid()) doWs(*wEls, 2);

  // fill TInfo
  tree_->Fill();
  
  delete info_;

}


void InvHiggsInfoProducer::doMC(const GenEventInfoProduct& genEvt, const GenParticleCollection& genParticles) {

  //   const HepMC::GenEvent *evt = genEvt.GetEvent();
  
  //   for(HepMC::GenEvent::vertex_const_iterator v = evt->vertices_begin();
  //       v!= evt->vertices_end();
  //       ++v) {
  
  //     if((*v)->barcode()==-1)  {
  
  //       info_->mcVtxX = (*v)->point3d().x();
  //       info_->mcVtxY = (*v)->point3d().y();
  //       info_->mcVtxR = (*v)->point3d().r();
  //       info_->mcVtxZ = (*v)->point3d().z();
  
  //     }
  //   }
  
  // find Higgs and tag quarks assuming PYTHIA process 123,124
  // W,Z MC information
  if (mcPYTHIA_) {
  
    //Higgs
    const GenParticle& higgs = genParticles[8];
    info_->mcHiggsMass = higgs.mass();
    info_->mcHiggsPt   = higgs.pt();
    info_->mcHiggsPhi  = higgs.phi();
    info_->mcHiggsEta  = higgs.eta();
    
    const GenParticle& q1 = genParticles[6];
    info_->mcQ1Pt   = q1.pt();
    info_->mcQ1Eta  = q1.eta();
    info_->mcQ1Phi  = q1.phi();
    
    const GenParticle& q2 = genParticles[7];
    info_->mcQ2Pt   = q2.pt();
    info_->mcQ2Eta  = q2.eta();
    info_->mcQ2Phi  = q2.phi();
    
    math::XYZTLorentzVector mcVbfP4=q1.p4() + q2.p4();
    info_->mcVBFEt   = mcVbfP4.Pt();
    info_->mcVBFEta  = mcVbfP4.Eta();
    info_->mcVBFPhi  = mcVbfP4.Phi();
    info_->mcVBFM    = mcVbfP4.M();
    info_->mcVBFDEta = fabs(q1.eta() - q2.eta());
    info_->mcVBFDPhi = fabs(fabs(fabs(q1.phi()-q2.phi())-TMath::Pi())-TMath::Pi());

    //W,Z
    bool foundw1 = false;
    bool foundz1 = false;
    for(size_t i = 0; i < genParticles.size(); ++ i) {
      const GenParticle & p = genParticles[i];
      //W
      if(abs(p.pdgId())==24 && p.status()==3 && (!foundw1) ){ //
	foundw1 = true;  
	//std::cout<<p.pt()<<std::endl;
	info_->wgenmass = p.mass();
	info_->wgenmt   = p.mt(); 
	info_->wgenpt   = p.pt();
	info_->wgeneta  = p.eta();
	info_->wgenphi  = p.phi();
	info_->wgene    = p.energy(); 
	for(int j=0;p.daughter(j)!=NULL;j++) {//loop on W daughter
	  const reco::Candidate* pl = p.daughter(j);  
	  if(abs(pl->pdgId())==11){
	    info_->wltype  = 1;
	    info_->wlpt    = pl->pt();
	    info_->wleta   = pl->eta();
	    info_->wlphi   = pl->phi();
	    info_->wle     = pl->energy();   
	  }
	  else if(abs(pl->pdgId())==13){ 
	    info_->wltype  = 2;
	    info_->wlpt    = pl->pt();
	    info_->wleta   = pl->eta();
	    info_->wlphi   = pl->phi();
	    info_->wle     = pl->energy();
	  }
	  else if(abs(pl->pdgId())==15){ 
	    info_->wltype  = 3;
	    info_->wlpt    = pl->pt();
	    info_->wleta   = pl->eta();
	    info_->wlphi   = pl->phi();
	    info_->wle     = pl->energy();
	    info_->wtauhadron = hadronicTau(pl);
	  }
	  else if(abs(pl->pdgId())==12 ||abs(pl->pdgId())==14 ||abs(pl->pdgId())==16){
	    info_->wmetpt  = pl->pt();
	    info_->wmeteta = pl->eta();
	    info_->wmetphi = pl->phi();
	    info_->wmete   = pl->energy();
	  }
	  else {}
	}
      }        
      //Z
      if(p.pdgId()==23 && p.status()==3 && (!foundz1) ){//
	foundz1 = true;  
	//std::cout<<p.pt()<<std::endl; 
	info_->zgenmass = p.mass();
	info_->zgenmt   = p.mt(); 
	info_->zgenpt   = p.pt();
	info_->zgeneta  = p.eta();
	info_->zgenphi  = p.phi();
	info_->zgene    = p.energy();
	for(int j=0;p.daughter(j)!=NULL;j++){//loop on Z daughter
	  const reco::Candidate* pl = p.daughter(j);  
	  if(pl->pdgId()==11){  
	    info_->zltype  = 1;
	    info_->zlmpt   = pl->pt();
	    info_->zlmeta  = pl->eta();
	    info_->zlmphi  = pl->phi();
	    info_->zlme    = pl->energy();   
	  }
	  else if(pl->pdgId()==13){  
	    info_->zltype  = 2;
	    info_->zlmpt   = pl->pt();
	    info_->zlmeta  = pl->eta();
	    info_->zlmphi  = pl->phi();
	    info_->zlme    = pl->energy();   
	  }
	  else if(pl->pdgId()==15){  
	    info_->zltype  = 3;
	    info_->zlmpt   = pl->pt();
	    info_->zlmeta  = pl->eta();
	    info_->zlmphi  = pl->phi();
	    info_->zlme    = pl->energy();   
	  }
	  else if(pl->pdgId()==12 || pl->pdgId()==14 || pl->pdgId()==16){  
	    info_->zltype  = 4;
	    info_->zlmpt   = pl->pt();
	    info_->zlmeta  = pl->eta();
	    info_->zlmphi  = pl->phi();
	    info_->zlme    = pl->energy();   
	  }
	  else if(pl->pdgId()==-11){
	    info_->zltype  = 1;
	    info_->zlppt   = pl->pt();
	    info_->zlpeta  = pl->eta();
	    info_->zlpphi  = pl->phi();
	    info_->zlpe    = pl->energy();   
	  }
	  else if(pl->pdgId()==-13){  
	    info_->zltype  = 2;
	    info_->zlppt   = pl->pt();
	    info_->zlpeta  = pl->eta();
	    info_->zlpphi  = pl->phi();
	    info_->zlpe    = pl->energy();   
	  }
	  else if(pl->pdgId()==-15){  
	    info_->zltype  = 3;
	    info_->zlppt   = pl->pt();
	    info_->zlpeta  = pl->eta();
	    info_->zlpphi  = pl->phi();
	    info_->zlpe    = pl->energy();   
	  }
	  else if(pl->pdgId()==-12 || pl->pdgId()==-14 || pl->pdgId()==-16){  
	    info_->zltype  = 4;
	    info_->zlppt   = pl->pt();
	    info_->zlpeta  = pl->eta();
	    info_->zlpphi  = pl->phi();
	    info_->zlpe    = pl->energy();   
	  }
	  else{}
	}
      }        
    }// end of genParticle Loop
  }// end if (mcPYTHIA)
}


int InvHiggsInfoProducer::hadronicTau(const reco::Candidate* tau)
{
   for(unsigned int i=0; i<tau->numberOfDaughters();++i){
   if(abs(tau->daughter(i)->pdgId())>99 || abs(tau->daughter(i)->pdgId())<10) return 1;
   if(abs(tau->daughter(i)->pdgId())==11) return 2; 
   if(abs(tau->daughter(i)->pdgId())==13) return 3;
   if(abs(tau->daughter(i)->pdgId())==15||abs(tau->daughter(i)->pdgId())==24) return hadronicTau(tau->daughter(i));
    }
   // if tau is stable
   return 4;   
}


void InvHiggsInfoProducer::doEventInfo(const edm::Event& iEvent) {

  unsigned long id          = iEvent.id().event();
  unsigned long bx          = iEvent.bunchCrossing();
  //unsigned long orbit       = iEvent.orbitNumber();
  unsigned long lb          = iEvent.luminosityBlock();
  unsigned long run         = iEvent.id().run();
  //double time               = iEvent.time().value();

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

  // store HLT prescale info
  //info_->hltPrescaleIndex=hltConfig_.prescaleSet(iEvent, iSetup);
  //info_->hltPrescale=hltConfig_.prescaleValue(iEvent, iSetup, hltPathName_);
  
}


void InvHiggsInfoProducer::doMETFilter(const edm::Event& iEvent, 
				       const edm::EventSetup& iSetup, 
				       edm::Handle<edm::TriggerResults> metFlag) {

  bool METFlag[10]={false,false,false,false,false,false,false,false,false,false};

  for(unsigned ii = 0 ; ii < metFlag->size(); ii++){
    METFlag[ii] = metFlag->accept(ii);
    //std::cout<<ii<<" "<<METFlag[ii]<<std::endl;
  }
  info_->metflag0=METFlag[0];
  info_->metflag1=METFlag[1];
  info_->metflag2=METFlag[2];
  info_->metflag3=METFlag[3];
  info_->metflag4=METFlag[4];
  info_->metflag5=METFlag[5];
  info_->metflag6=METFlag[6];  
}


void InvHiggsInfoProducer::doVertices(const std::vector<reco::Vertex>&  vertices) {

  std::vector<reco::Vertex>::const_iterator v;
  for (v=vertices.begin(); v!=vertices.end(); ++v) {

    if (v->isValid() && 
	!v->isFake() &&
	v->ndof() > 4 &&
	abs(v->z()) <= 24 &&
	v->position().rho() < 2) info_->nVtx++;

  }

}


void InvHiggsInfoProducer::doPUReweighting(const edm::Event& iEvent) {
  
  double weight = 1.;

  edm::Handle<std::vector< PileupSummaryInfo > >  puInfo;
  iEvent.getByLabel(edm::InputTag("addPileupInfo"), puInfo);
  
  if (puInfo.isValid()) {
    std::vector<PileupSummaryInfo>::const_iterator pvi;
    
    float tnpv = -1;
    for(pvi = puInfo->begin(); pvi != puInfo->end(); ++pvi) {
      
      int bx = pvi->getBunchCrossing();
      
      if(bx == 0) { 
	tnpv = pvi->getTrueNumInteractions();
	continue;
      }
      
    }
    
    weight = lumiWeights_.weight( tnpv );
    
  }

  info_->puWeight = weight;

}


void InvHiggsInfoProducer::doJets(edm::Handle<edm::View<pat::Jet> > jets, 
				  edm::Handle<edm::ValueMap<float> > puJetIdMVAs, 
				  edm::Handle<edm::ValueMap<int> > puJetIdFlags,
				  const std::vector<pat::Muon>& muons,
				  const std::vector<pat::Electron>& electrons) {

  // VBF jets defined here

  // reset local variables
  tagJet1Index_ = -1;
  tagJet1Index_ = -1;
  tagJet1_.SetXYZT(0., 0., 0., 0.);
  tagJet2_.SetXYZT(0., 0., 0., 0.);
  tagJetEtaMin_ = 0.;
  tagJetEtaMax_ = 0.;

  // find VBF jets
  bool foundFirst = false;
  bool foundSecond = false;
  
  for (unsigned i=0; i<jets->size() && !foundSecond; ++i) {

    // Check overlap between jet and (muon, electron)
    bool checkOverlap = false;
    for(unsigned iLep=0; iLep<muons.size(); iLep++){
      if(reco::deltaR(muons.at(iLep).eta(),muons.at(iLep).phi(),jets->at(i).eta(),jets->at(i).phi())>0.3) continue;
      checkOverlap = true;
      //std::cout<<"Overlap with muons"<<std::endl;
      break;
    }
    for(unsigned iLep=0; iLep<electrons.size(); iLep++){ 
      if(reco::deltaR(electrons.at(iLep).eta(),electrons.at(iLep).phi(),jets->at(i).eta(),jets->at(i).phi())>0.3) continue; 
      checkOverlap = true;
      //std::cout<<"Overlap with electrons"<<std::endl;
      break;
    }
    if(checkOverlap) continue;
    
    // check jet is associated with PV
    int puflag = (*puJetIdFlags)[jets->refAt(i)];
    if ( PileupJetIdentifier::passJetId( puflag, PileupJetIdentifier::kLoose ) ) {
      
      // currently just taking the leading pair
      // could check here that second jet  is in opposite hemisphere
      if (foundFirst && !foundSecond) {
	
	tagJet2Index_ = i;
	tagJet2_ = jets->at(i).p4();
	
	info_->jet2Index  = i;
	info_->jet2Pt     = jets->at(i).pt();    
	info_->jet2Eta    = jets->at(i).eta();    
	info_->jet2Phi    = jets->at(i).phi();    
	info_->jet2M      = jets->at(i).mass();    
	info_->jet2PUMVA  = (*puJetIdMVAs)[jets->refAt(i)];
	info_->jet2PUFlag = (*puJetIdFlags)[jets->refAt(i)];

	foundSecond = true;

      }
      
      if (!foundFirst) {
	
	tagJet1Index_ = i;
	tagJet1_ = jets->at(i).p4();
	
	info_->jet1Index  = i;
	info_->jet1Pt     = jets->at(i).pt();    
	info_->jet1Eta    = jets->at(i).eta();    
	info_->jet1Phi    = jets->at(i).phi();    
	info_->jet1M      = jets->at(i).mass();
	info_->jet1PUMVA  = (*puJetIdMVAs)[jets->refAt(i)];
	info_->jet1PUFlag = (*puJetIdFlags)[jets->refAt(i)];
	
	foundFirst = true;
	
      }

    }
  }
  
  // save some info about the VBF pair

  tagJetEtaMin_ = std::min(tagJet1_.eta(), tagJet2_.eta());
  tagJetEtaMax_ = std::max(tagJet1_.eta(), tagJet2_.eta());

  math::XYZTLorentzVector vbfp4=tagJet1_ + tagJet2_;
  info_->vbfEt   = vbfp4.Pt();
  info_->vbfEta  = vbfp4.Eta();
  info_->vbfPhi  = vbfp4.Phi();
  info_->vbfM    = vbfp4.M();
  info_->vbfDEta = fabs(tagJet1_.eta() - tagJet2_.eta());
  info_->vbfDPhi = fabs(fabs(fabs(tagJet1_.phi()-tagJet2_.phi())-TMath::Pi())-TMath::Pi());
 
}


void InvHiggsInfoProducer::doThirdJet(edm::Handle<edm::View<pat::Jet> > jets, 
				      edm::Handle<edm::ValueMap<float> > puJetIdMVAs, 
				      edm::Handle<edm::ValueMap<int> > puJetIdFlags,
				      const std::vector<pat::Muon>& muons,
				      const std::vector<pat::Electron>& electrons) 
{

  // Find 3rd jet
  for (unsigned i=0; i<jets->size(); ++i) {

    // Not Jet1 or Jet2
    if( i == tagJet1Index_ || i == tagJet2Index_ ) continue;

    // Check overlap between jet and (muon, electron) 
    bool checkOverlap = false;
    for(unsigned iLep=0; iLep<muons.size(); iLep++){
      if(reco::deltaR(muons.at(iLep).eta(),muons.at(iLep).phi(),jets->at(i).eta(),jets->at(i).phi())>0.3) continue;
      checkOverlap = true;
      //std::cout<<"Overlap with muons"<<std::endl;
      break;
    }
    for(unsigned iLep=0; iLep<electrons.size(); iLep++){ 
      if(reco::deltaR(electrons.at(iLep).eta(),electrons.at(iLep).phi(),jets->at(i).eta(),jets->at(i).phi())>0.3) continue; 
      checkOverlap = true;
      //std::cout<<"Overlap with electrons"<<std::endl;
      break;
    }
    if(checkOverlap) continue;

    // associated with PV and not tag jet
    int puflag = (*puJetIdFlags)[jets->refAt(i)];
    if ( PileupJetIdentifier::passJetId( puflag, PileupJetIdentifier::kLoose ) ) {

      // store in ntuple
      info_->jet3Et  = jets->at(i).pt();
      info_->jet3Eta = jets->at(i).eta();
      info_->jet3Phi = jets->at(i).phi();
      info_->jet3M   = jets->at(i).mass();

      // calculate Zeppenfeld variable
      info_->jet3EtaStar = jets->at(i).eta() - (tagJetEtaMin_ + tagJetEtaMax_) / 2;

      // check if it is between tag jets in eta
      if (jets->at(i).eta() > tagJetEtaMin_ &&
	  jets->at(i).eta() < tagJetEtaMax_ ) {

	// store in ntuple
	info_->cenJetEt  = jets->at(i).pt();
	info_->cenJetEta = jets->at(i).eta();
	info_->cenJetPhi = jets->at(i).phi();
	info_->cenJetM   = jets->at(i).mass();

      }

    }

  }
  
}


void InvHiggsInfoProducer::doMuons(const std::vector<pat::Muon>& muons) {

  if (muons.size()>0) {
    info_->mu1Pt = muons.at(0).pt();    
    info_->mu1Eta = muons.at(0).eta();    
    info_->mu1Phi = muons.at(0).phi();    
  }
  if (muons.size()>1) {
    info_->mu2Pt = muons.at(1).pt();    
    info_->mu2Eta = muons.at(1).eta();    
    info_->mu2Phi = muons.at(1).phi();    
  }
  if (muons.size()>2) {
    info_->mu3Pt = muons.at(2).pt();    
    info_->mu3Eta = muons.at(2).eta();    
    info_->mu3Phi = muons.at(2).phi();
  }
    
    // leading pair mass
    // math::XYZTLorentzVector pair = muons.at(0).p4() + muons.at(1).p4();
    // info_->mMuMu = pair.M();
  
}


void InvHiggsInfoProducer::doElectrons(const std::vector<pat::Electron>& electrons) {

  if (electrons.size()>0) {
    info_->ele1Pt = electrons.at(0).pt();    
    info_->ele1Eta = electrons.at(0).eta();    
    info_->ele1Phi = electrons.at(0).phi();    
  }
  if (electrons.size()>1) {
    info_->ele2Pt = electrons.at(1).pt();    
    info_->ele2Eta = electrons.at(1).eta();    
    info_->ele2Phi = electrons.at(1).phi();   
  }
  if (electrons.size()>2) {
    info_->ele3Pt = electrons.at(2).pt();    
    info_->ele3Eta = electrons.at(2).eta();    
    info_->ele3Phi = electrons.at(2).phi();
  }

    // leading pair mass
    // math::XYZTLorentzVector pair = electrons.at(0).p4() + electrons.at(1).p4();
    // info_->mEE = pair.M();
  
}


void InvHiggsInfoProducer::doMET(const std::vector<pat::MET>& met,
				 const std::vector<pat::Muon>& muons,
				 const std::vector<pat::Muon>& looseMuons,
				 const std::vector<pat::Electron>& electrons,
				 const std::vector<pat::Electron>& looseElectrons) {

  info_->met    = met.at(0).pt();
  info_->metPhi = met.at(0).phi();
  //    info_->metSig = met->at(0).mEtSig();

  // delta phi between MET and nearest tag jet
  double jmdphi1 = fabs(fabs(fabs(met.at(0).phi()-tagJet1_.phi())-TMath::Pi())-TMath::Pi());
  double jmdphi2 = fabs(fabs(fabs(met.at(0).phi()-tagJet2_.phi())-TMath::Pi())-TMath::Pi());
  
  info_->jetMETdPhi = std::min(jmdphi1, jmdphi2);
  
  //metNoMuon
  Double_t metx = met.at(0).px();
  Double_t mety = met.at(0).py();
  for(unsigned iLep=0; iLep<muons.size(); iLep++){
    metx = metx + muons.at(iLep).px();
    mety = mety + muons.at(iLep).py();
  }
  info_->metNoMuon = TMath::Sqrt(metx*metx + mety*mety);
  info_->metNoMuonPhi = TMath::ATan2(mety,metx);

  //metNoLooseMuon
  metx = met.at(0).px();
  mety = met.at(0).py();
  for(unsigned iLep=0; iLep<looseMuons.size(); iLep++){
    metx = metx + looseMuons.at(iLep).px();
    mety = mety + looseMuons.at(iLep).py();
  }
  info_->metNoLooseMuon = TMath::Sqrt(metx*metx + mety*mety);
  info_->metNoLooseMuonPhi = TMath::ATan2(mety,metx);
  
  //metNoElectron
  metx = met.at(0).px();
  mety = met.at(0).py();
  for(unsigned iLep=0; iLep<electrons.size(); iLep++){
    metx = metx + electrons.at(iLep).px();
    mety = mety + electrons.at(iLep).py();
  }
  info_->metNoElectron = TMath::Sqrt(metx*metx + mety*mety);
  info_->metNoElectronPhi = TMath::ATan2(mety,metx);  

  //metNoLooseElectron
  metx = met.at(0).px();
  mety = met.at(0).py();
  for(unsigned iLep=0; iLep<looseElectrons.size(); iLep++){
    metx = metx + looseElectrons.at(iLep).px();
    mety = mety + looseElectrons.at(iLep).py();
  }
  info_->metNoLooseElectron = TMath::Sqrt(metx*metx + mety*mety);
  info_->metNoLooseElectronPhi = TMath::ATan2(mety,metx);
}

void InvHiggsInfoProducer::doMHT(const std::vector<pat::MHT>& mht) {

  info_->mht    = mht.at(0).pt();
  info_->mhtPhi = mht.at(0).phi();
  //    info_->mhtSig = mht->at(0).significance();

}


void InvHiggsInfoProducer::doZs(const reco::CandidateView& zs, int channel) {

  if (zs.size() > 0) {

    for (unsigned i=0; i<zs.size(); ++i) {

      info_->nZ      += 1;
      
      if (zs[i].pt() > info_->zPt) {
	info_->zPt      = zs[i].pt();
	info_->zEta     = zs[i].eta();
	info_->zPhi     = zs[i].phi();
	info_->zMass    = zs[i].mass();
	info_->zChannel = channel;

	const reco::Candidate *Zboson = &(zs[i]);
	if(Zboson->numberOfDaughters() < 2) continue;
	info_->zDau1Pt     = Zboson->daughter(0)->pt();
	info_->zDau1Eta    = Zboson->daughter(0)->eta();
	info_->zDau1Phi    = Zboson->daughter(0)->phi();
	info_->zDau1Charge = Zboson->daughter(0)->charge();
	info_->zDau2Pt     = Zboson->daughter(1)->pt();
	info_->zDau2Eta    = Zboson->daughter(1)->eta();
	info_->zDau2Phi    = Zboson->daughter(1)->phi();
	info_->zDau2Charge = Zboson->daughter(1)->charge();
	
      }

    }
  }

}


void InvHiggsInfoProducer::doWs(const reco::CandidateView& ws, int channel) {

  if (ws.size() > 0) {

    for (unsigned i=0; i<ws.size(); ++i) {

      info_->nW      += 1;
      
      if (ws[i].pt() > info_->wPt) {
	info_->wPt      = ws[i].pt();
	info_->wEta     = ws[i].eta();
	info_->wPhi     = ws[i].phi();
	info_->wMt      = ws[i].mt();
	info_->wChannel = channel;

	const reco::Candidate *Wboson = &(ws[i]);
	if(Wboson->daughter(0)){ 
	//std::cout<<"W boson daughter0"<<std::endl;
	  info_->wDaulPt     = Wboson->daughter(0)->pt();
	  info_->wDaulEta    = Wboson->daughter(0)->eta();
	  info_->wDaulPhi    = Wboson->daughter(0)->phi();
	  info_->wDaulCharge = Wboson->daughter(0)->charge();
	}
	else if(Wboson->daughter(1)){
	//std::cout<<"W boson daughter1"<<std::endl;
	  info_->wDaulPt     = Wboson->daughter(1)->pt();
	  info_->wDaulEta    = Wboson->daughter(1)->eta();
	  info_->wDaulPhi    = Wboson->daughter(1)->phi();
	  info_->wDaulCharge = Wboson->daughter(1)->charge();
	}
      }
    }
  }
  
}



//define this as a plug-in
DEFINE_FWK_MODULE(InvHiggsInfoProducer);
