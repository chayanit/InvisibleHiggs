// -*- C++ -*-
//
// Package:    InvHiggsPUJetNtuple
// Class:      InvHiggsPUJetNtuple
// 
/**\class InvHiggsPUJetNtuple InvHiggsPUJetNtuple.cc PhatAnalysis/MyAnalyzer/src/InvHiggsPUJetNtuple.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Phat Srimanobhas,40 1-A11,+41227671646,
//         Created:  Tue Apr 10 16:47:40 CEST 2012
// $Id: InvHiggsPUJetNtuple.cc,v 1.2 2012/10/29 21:17:57 srimanob Exp $
//
//

// system include files
#include <memory>
#include <string>

// user include files
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"

// DataFormats
#include "DataFormats/Common/interface/ConditionsInEdm.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/MHT.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"


#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include "CMGTools/External/interface/PileupJetIdentifier.h"

//#include "CondFormats/DataRecord/interface/L1GtStableParametersRcd.h"

#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TMath.h"
#include "TLorentzVector.h"

//
// class declaration
//

class InvHiggsPUJetNtuple : public edm::EDAnalyzer {
  
public:
  explicit InvHiggsPUJetNtuple(const edm::ParameterSet&);
  ~InvHiggsPUJetNtuple();
  
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  
private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  virtual void beginRun(edm::Run const&, edm::EventSetup const&);
  virtual void endRun(edm::Run const&, edm::EventSetup const&);
  virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
  virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&); 
  //virtual bool isHLTPathAccepted(std::string pathName);
  //virtual void setErrors(TH1F&, const TH1F&);
  //const std::string getL1ConditionModuleName(const std::string& pathname);
  //virtual bool isTriggerObjectFound(std::string objectName);
  
  // ----------member data ---------------------------
  // InputTag 
  edm::InputTag METResultsLabel_;
  edm::InputTag VertexLabel_;
  edm::InputTag METLabel_;
  edm::InputTag JetLabel_;
  edm::InputTag puJetMvaTag_;
  edm::InputTag puJetIdTag_;
  edm::InputTag MuonLabel_;
  edm::InputTag LooseMuonLabel_;
  edm::InputTag ElectronLabel_;
  edm::InputTag LooseElectronLabel_;
  edm::InputTag TauLabel_;
  edm::InputTag PhotonLabel_;
  
  // Histogram
  edm::Service<TFileService> fs;
  TTree *MyNtuple_;
  const static Int_t MAX = 100;
  const static Int_t MAXX = 5000;

  // Event information
  Int_t eventNumber;
  Int_t runNumber;
  Int_t lumiSection;
  Int_t bunchCrossing;

  // METFlags
  Int_t metflag0;
  Int_t metflag1;  
  Int_t metflag2;
  Int_t metflag3;  
  Int_t metflag4;
  Int_t metflag5;
  Int_t metflag6;
  
  // Primary Vertex
  Int_t   nPV;
  Float_t PVx[MAX];
  Float_t PVy[MAX];
  Float_t PVz[MAX];
  Float_t PVchi2[MAX];
  Float_t PVndof[MAX];
  Float_t PVntracks[MAX];

  // MET
  Float_t met;
  Float_t met_px;
  Float_t met_py;
  Float_t met_phi;

  // PFJet
  Int_t   n_jet;
  Float_t jet_pt[MAX];
  Float_t jet_eta[MAX];
  Float_t jet_phi[MAX];
  Float_t jet_mass[MAX];
  Float_t jet_nhef[MAX];
  Float_t jet_chef[MAX];
  Float_t jet_nemf[MAX];
  Float_t jet_cemf[MAX];
  Int_t   jet_puflag[MAX];
  
  // Muon
  Int_t   n_muon;
  Float_t muon_pt[MAX];
  Float_t muon_eta[MAX];
  Float_t muon_phi[MAX];
  Float_t muon_mass[MAX];
  Float_t muon_charge[MAX];

  // Loose Muon
  Int_t   n_lmuon;
  Float_t lmuon_pt[MAX];
  Float_t lmuon_eta[MAX];
  Float_t lmuon_phi[MAX];
  Float_t lmuon_mass[MAX];
  Float_t lmuon_charge[MAX];
  
  // Electron
  Int_t   n_electron;
  Float_t electron_pt[MAX];
  Float_t electron_eta[MAX];
  Float_t electron_phi[MAX];
  Float_t electron_mass[MAX];
  Float_t electron_charge[MAX];  

  // Loose Electron
  Int_t   n_lelectron;
  Float_t lelectron_pt[MAX];
  Float_t lelectron_eta[MAX];
  Float_t lelectron_phi[MAX];
  Float_t lelectron_mass[MAX];
  Float_t lelectron_charge[MAX];

  // Tau
  Int_t   n_tau;
  Float_t tau_pt[MAX];
  Float_t tau_eta[MAX];
  Float_t tau_phi[MAX];
  Float_t tau_mass[MAX];
  Float_t tau_charge[MAX];
  
  // Photon
  Int_t   n_photon;
  Float_t photon_pt[MAX];
  Float_t photon_eta[MAX];
  Float_t photon_phi[MAX];
  Float_t photon_mass[MAX];
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
InvHiggsPUJetNtuple::InvHiggsPUJetNtuple(const edm::ParameterSet& iConfig)
{ 
  // InputTag
  METResultsLabel_      = iConfig.getParameter<edm::InputTag>("METResultsLabel");
  VertexLabel_          = iConfig.getParameter<edm::InputTag>("VertexLabel");
  METLabel_             = iConfig.getParameter<edm::InputTag>("METLabel"); 
  JetLabel_             = iConfig.getParameter<edm::InputTag>("JetLabel");
  puJetMvaTag_          = iConfig.getParameter<edm::InputTag>("puJetMvaTag");
  puJetIdTag_           = iConfig.getParameter<edm::InputTag>("puJetIdTag");
  MuonLabel_            = iConfig.getParameter<edm::InputTag>("MuonLabel");
  LooseMuonLabel_       = iConfig.getParameter<edm::InputTag>("LooseMuonLabel");
  ElectronLabel_        = iConfig.getParameter<edm::InputTag>("ElectronLabel");
  LooseElectronLabel_   = iConfig.getParameter<edm::InputTag>("LooseElectronLabel");
  TauLabel_             = iConfig.getParameter<edm::InputTag>("TauLabel");
  PhotonLabel_          = iConfig.getParameter<edm::InputTag>("PhotonLabel");
  
  // Tree
  MyNtuple_ = fs->make<TTree>("InvHiggs","InvHiggs");

  // Event Information
  MyNtuple_->Branch("eventNumber",      &eventNumber,     "eventNumber/I");
  MyNtuple_->Branch("runNumber",        &runNumber,       "runNumber/I");
  MyNtuple_->Branch("lumiSection",      &lumiSection,     "lumiSection/I"); 
  MyNtuple_->Branch("bunchCrossing",    &bunchCrossing,   "bunchCrossing/I");

  // METFlag
  MyNtuple_->Branch("metflag0",         &metflag0,        "metflag0/I");
  MyNtuple_->Branch("metflag1",         &metflag1,        "metflag1/I");
  MyNtuple_->Branch("metflag2",         &metflag2,        "metflag2/I");
  MyNtuple_->Branch("metflag3",         &metflag3,        "metflag3/I");
  MyNtuple_->Branch("metflag4",         &metflag4,        "metflag4/I");
  MyNtuple_->Branch("metflag5",         &metflag5,        "metflag5/I");
  MyNtuple_->Branch("metflag6",         &metflag6,        "metflag6/I");
  
  // Primary Vertex
  MyNtuple_->Branch("nPV",              &nPV,             "nPV/I");
  MyNtuple_->Branch("PVx",              PVx,              "PVx[nPV]/F");
  MyNtuple_->Branch("PVy",              PVy,              "PVy[nPV]/F"); 
  MyNtuple_->Branch("PVz",              PVz,              "PVz[nPV]/F");
  MyNtuple_->Branch("PVchi2",           PVchi2,           "PVchi2[nPV]/F");
  MyNtuple_->Branch("PVndof",           PVndof,           "PVndof[nPV]/F");
  MyNtuple_->Branch("PVntracks",        PVntracks,        "PVntracks[nPV]/F");
  
  // MET
  MyNtuple_->Branch("met",              &met,             "met/F");
  MyNtuple_->Branch("met_px",           &met_px,          "met_px/F");
  MyNtuple_->Branch("met_py",           &met_py,          "met_py/F");
  MyNtuple_->Branch("met_phi",          &met_phi,         "met_phi/F");
  
  // Jets
  MyNtuple_->Branch("n_jet",            &n_jet,           "n_jet/I");
  MyNtuple_->Branch("jet_pt",           jet_pt,           "jet_pt[n_jet]/F");
  MyNtuple_->Branch("jet_eta",          jet_eta,          "jet_eta[n_jet]/F");
  MyNtuple_->Branch("jet_phi",          jet_phi,          "jet_phi[n_jet]/F");
  MyNtuple_->Branch("jet_mass",         jet_mass,         "jet_mass[n_jet]/F");
  MyNtuple_->Branch("jet_nhef",         jet_nhef,         "jet_nhef[n_jet]/F");
  MyNtuple_->Branch("jet_chef",         jet_chef,         "jet_chef[n_jet]/F");
  MyNtuple_->Branch("jet_nemf",         jet_nemf,         "jet_nemf[n_jet]/F");
  MyNtuple_->Branch("jet_cemf",         jet_cemf,         "jet_cemf[n_jet]/F");
  MyNtuple_->Branch("jet_puflag",       jet_puflag,       "jet_puflag[n_jet]/I");

  // Muons 
  MyNtuple_->Branch("n_muon",           &n_muon,          "n_muon/I");
  MyNtuple_->Branch("muon_pt",          muon_pt,          "muon_pt[n_muon]/F");
  MyNtuple_->Branch("muon_eta",         muon_eta,         "muon_eta[n_muon]/F");
  MyNtuple_->Branch("muon_phi",         muon_phi,         "muon_phi[n_muon]/F");
  MyNtuple_->Branch("muon_mass",        muon_mass,        "muon_mass[n_muon]/F");
  MyNtuple_->Branch("muon_charge",      muon_charge,      "muon_charge[n_muon]/F");

  // Loose Muons 
  MyNtuple_->Branch("n_lmuon",          &n_lmuon,         "n_lmuon/I");
  MyNtuple_->Branch("lmuon_pt",         lmuon_pt,         "lmuon_pt[n_lmuon]/F");
  MyNtuple_->Branch("lmuon_eta",        lmuon_eta,        "lmuon_eta[n_lmuon]/F");
  MyNtuple_->Branch("lmuon_phi",        lmuon_phi,        "lmuon_phi[n_lmuon]/F");
  MyNtuple_->Branch("lmuon_mass",       lmuon_mass,       "lmuon_mass[n_lmuon]/F");
  MyNtuple_->Branch("lmuon_charge",     lmuon_charge,     "lmuon_charge[n_lmuon]/F");
 
  // Electrons
  MyNtuple_->Branch("n_electron",       &n_electron,      "n_electron/I");
  MyNtuple_->Branch("electron_pt",      electron_pt,      "electron_pt[n_electron]/F");
  MyNtuple_->Branch("electron_eta",     electron_eta,     "electron_eta[n_electron]/F");
  MyNtuple_->Branch("electron_phi",     electron_phi,     "electron_phi[n_electron]/F");
  MyNtuple_->Branch("electron_mass",    electron_mass,    "electron_mass[n_electron]/F");
  MyNtuple_->Branch("electron_charge",  electron_charge,  "electron_charge[n_electron]/F");

  // Loose Electrons
  MyNtuple_->Branch("n_lelectron",      &n_lelectron,     "n_lelectron/I");
  MyNtuple_->Branch("lelectron_pt",     lelectron_pt,     "lelectron_pt[n_lelectron]/F");
  MyNtuple_->Branch("lelectron_eta",    lelectron_eta,    "lelectron_eta[n_lelectron]/F");
  MyNtuple_->Branch("lelectron_phi",    lelectron_phi,    "lelectron_phi[n_lelectron]/F");
  MyNtuple_->Branch("lelectron_mass",   lelectron_mass,   "lelectron_mass[n_lelectron]/F");
  MyNtuple_->Branch("lelectron_charge", lelectron_charge, "lelectron_charge[n_lelectron]/F");

  // Taus 
  MyNtuple_->Branch("n_tau",            &n_tau,           "n_tau/I");
  MyNtuple_->Branch("tau_pt",           tau_pt,           "tau_pt[n_tau]/F");
  MyNtuple_->Branch("tau_eta",          tau_eta,          "tau_eta[n_tau]/F");
  MyNtuple_->Branch("tau_phi",          tau_phi,          "tau_phi[n_tau]/F");
  MyNtuple_->Branch("tau_mass",         tau_mass,         "tau_mass[n_tau]/F");
  MyNtuple_->Branch("tau_charge",       tau_charge,       "tau_charge[n_tau]/F");
  
  // Photons 
  MyNtuple_->Branch("n_photon",         &n_photon,        "n_photon/I");
  MyNtuple_->Branch("photon_pt",        photon_pt,        "photon_pt[n_photon]/F");
  MyNtuple_->Branch("photon_eta",       photon_eta,       "photon_eta[n_photon]/F");
  MyNtuple_->Branch("photon_phi",       photon_phi,       "photon_phi[n_photon]/F");
  MyNtuple_->Branch("photon_mass",      photon_mass,      "photon_mass[n_photon]/F");

}

InvHiggsPUJetNtuple::~InvHiggsPUJetNtuple()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
InvHiggsPUJetNtuple::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  //desc.setUnknown();
  desc.add<edm::InputTag>("METResultsLabel",edm::InputTag("TriggerResults","","PAT"));
  desc.add<edm::InputTag>("VertexLabel",edm::InputTag("offlinePrimaryVertices")); 
  desc.add<edm::InputTag>("METLabel",edm::InputTag("met")); 
  desc.add<edm::InputTag>("JetLabel",edm::InputTag("ak5PFJet"));
  desc.add<edm::InputTag>("puJetMvaTag",edm::InputTag("puJetMva:fullDiscriminant"));
  desc.add<edm::InputTag>("puJetIdTag",edm::InputTag("puJetMva:fullId"));
  desc.add<edm::InputTag>("MuonLabel",edm::InputTag("muons"));
  desc.add<edm::InputTag>("LooseMuonLabel",edm::InputTag("muons"));
  desc.add<edm::InputTag>("ElectronLabel",edm::InputTag("electrons")); 
  desc.add<edm::InputTag>("LooseElectronLabel",edm::InputTag("electrons")); 
  desc.add<edm::InputTag>("TauLabel",edm::InputTag("taus"));
  desc.add<edm::InputTag>("PhotonLabel",edm::InputTag("photons"));
  descriptions.addDefault(desc);
}


//
// member functions
//

// ------------ method called for each event  ------------
void
InvHiggsPUJetNtuple::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  using namespace std;
  using namespace trigger;
  
  edm::Handle<edm::TriggerResults> metFlag;
  iEvent.getByLabel(METResultsLabel_, metFlag);
  
  edm::Handle<reco::VertexCollection> recVtxs;
  iEvent.getByLabel(VertexLabel_,recVtxs);
  if(!recVtxs.isValid()) return;
  
  edm::Handle<edm::View<pat::MET> > metSrc;
  iEvent.getByLabel(METLabel_, metSrc);
  if(!metSrc.isValid())      return;
  const edm::View<pat::MET> & mets = *metSrc;
  if(mets.size()<=0) return;
  
  edm::Handle<edm::View<pat::Jet> > jetSrc;
  iEvent.getByLabel(JetLabel_, jetSrc); 
  if(!jetSrc.isValid())      return;
  const edm::View<pat::Jet> & jets = *jetSrc;
  if(jets.size()<=0) return;  
  
  edm::Handle<edm::ValueMap<float> > puJetIdMVA;
  iEvent.getByLabel(puJetMvaTag_,puJetIdMVA);
  
  edm::Handle<edm::ValueMap<int> > puJetIdFlags;
  iEvent.getByLabel(puJetIdTag_,puJetIdFlags);
  
  edm::Handle<edm::View<pat::Muon> > muons;
  iEvent.getByLabel(MuonLabel_, muons); 
  
  edm::Handle<edm::View<pat::Muon> > lmuons;
  iEvent.getByLabel(LooseMuonLabel_, lmuons);
  
  edm::Handle<edm::View<pat::Electron> > electrons;
  iEvent.getByLabel(ElectronLabel_, electrons);
  
  edm::Handle<edm::View<pat::Electron> > lelectrons;
  iEvent.getByLabel(LooseElectronLabel_, lelectrons);
  
  edm::Handle<edm::View<pat::Photon> > photons;
  iEvent.getByLabel(PhotonLabel_, photons);
  
  edm::Handle<edm::View<pat::Tau> > taus;
  iEvent.getByLabel(TauLabel_, taus);
  
  //
  eventNumber   = iEvent.id().event(); 
  runNumber     = iEvent.id().run(); 
  lumiSection   = iEvent.luminosityBlock();
  bunchCrossing = iEvent.bunchCrossing();
  //cout<<"Run: "<<runNumber<<", Event: "<<eventNumber<<endl;
  
  // 
  nPV         = 0; 
  met         = 0;
  met_px      = 0;
  met_py      = 0;
  met_phi     = 0;
  n_jet       = 0;
  n_muon      = 0;
  n_lmuon     = 0;
  n_electron  = 0;
  n_lelectron = 0;
  n_photon    = 0;
  n_tau       = 0;
  
  bool METFlag[10]={false,false,false,false,false,false,false,false,false,false};
  for(unsigned ii = 0 ; ii < metFlag->size(); ii++){
    METFlag[ii] = metFlag->accept(ii);
    //std::cout<<ii<<" "<<METFlag[ii]<<std::endl;
  }
  metflag0 = METFlag[0];
  metflag1 = METFlag[1];
  metflag2 = METFlag[2];
  metflag3 = METFlag[3];
  metflag4 = METFlag[4];
  metflag5 = METFlag[5];
  metflag6 = METFlag[6];
  
  // Primary vertex
  reco::Vertex primaryvtx;
  nPV=0;
  for(unsigned int ind=0;ind<recVtxs->size();ind++){
    if(!((*recVtxs)[ind].isFake())){
      PVx[nPV]      = (*recVtxs)[ind].x();
      PVy[nPV]      = (*recVtxs)[ind].y();
      PVz[nPV]      = (*recVtxs)[ind].z();
      PVchi2[nPV]   = (*recVtxs)[ind].chi2();
      PVndof[nPV]   = (*recVtxs)[ind].ndof();
      PVntracks[nPV]= (*recVtxs)[ind].tracksSize();
      if(nPV == 0) primaryvtx = (*recVtxs)[ind];
      nPV++;
    }
  }

  // MET
  met     = mets[0].et();
  met_px  = mets[0].px();
  met_py  = mets[0].py();
  met_phi = mets[0].phi();
  
  // Muons
  n_muon = 0;
  for(unsigned int ind=0; ind<(*muons).size() && ind<(unsigned int)MAX; ind++){
    const pat::Muon& muon = (*muons)[ind];
    //if(fabs(muon.muonBestTrack()->dxy(primaryvtx.position()))>=0.2) continue;
    //if(fabs(muon.muonBestTrack()->dz(primaryvtx.position()))>=0.5) continue;
    muon_pt[n_muon]     = muon.pt();
    muon_eta[n_muon]    = muon.eta();
    muon_phi[n_muon]    = muon.phi();
    muon_mass[n_muon]   = muon.mass();
    muon_charge[n_muon] = muon.charge();
    n_muon++;
  }
  
  // Loose Muons
  n_lmuon = 0;
  for(unsigned int ind=0; ind<(*lmuons).size() && ind<(unsigned int)MAX; ind++){
    const pat::Muon& lmuon = (*lmuons)[ind];
    //if(fabs(lmuon.muonBestTrack()->dxy(primaryvtx.position()))>=0.2) continue;
    //if(fabs(lmuon.muonBestTrack()->dz(primaryvtx.position()))>=0.5) continue;
    lmuon_pt[n_lmuon]     = lmuon.pt();
    lmuon_eta[n_lmuon]    = lmuon.eta();
    lmuon_phi[n_lmuon]    = lmuon.phi();
    lmuon_mass[n_lmuon]   = lmuon.mass();
    lmuon_charge[n_lmuon] = lmuon.charge();
    n_lmuon++;
  }
  
  // Electrons
  n_electron = 0;
  for(unsigned int ind=0; ind<(*electrons).size() && ind<(unsigned int)MAX; ind++){
    const pat::Electron& electron = (*electrons)[ind];
    //
    electron_pt[n_electron]     = electron.pt();
    electron_eta[n_electron]    = electron.eta();
    electron_phi[n_electron]    = electron.phi();
    electron_mass[n_electron]   = electron.mass();
    electron_charge[n_electron] = electron.charge();
    n_electron++;
  }
  
  // Loose Electrons
  n_lelectron = 0;
  for(unsigned int ind=0; ind<(*lelectrons).size() && ind<(unsigned int)MAX; ind++){
    const pat::Electron& lelectron = (*lelectrons)[ind];
    //
    lelectron_pt[n_lelectron]     = lelectron.pt();
    lelectron_eta[n_lelectron]    = lelectron.eta();
    lelectron_phi[n_lelectron]    = lelectron.phi();
    lelectron_mass[n_lelectron]   = lelectron.mass();
    lelectron_charge[n_lelectron] = lelectron.charge();
    n_lelectron++;
  }
  
  // Taus
  n_tau = 0;
  for(unsigned int ind=0; ind<(*taus).size() && ind<(unsigned int)MAX; ind++){
    const pat::Tau& tau = (*taus)[ind];
    //
    tau_pt[n_tau]     = tau.pt();
    tau_eta[n_tau]    = tau.eta();
    tau_phi[n_tau]    = tau.phi();
    tau_mass[n_tau]   = tau.mass();
    tau_charge[n_tau] = tau.charge();
    n_tau++;
  }
  
  // Photons
  n_photon = 0;
  for(unsigned int ind=0; ind<(*photons).size() && ind<(unsigned int)MAX; ind++){
    const pat::Photon& photon = (*photons)[ind];
    //
    photon_pt[n_photon]   = photon.pt();
    photon_eta[n_photon]  = photon.eta();
    photon_phi[n_photon]  = photon.phi();
    photon_mass[n_photon] = photon.mass();
    n_photon++;
  }
  
  // PFJet
  int idflag;
  n_jet  = 0;
  bool checkOverlap = false;
  for(unsigned int i=0;i<jets.size() && i<(unsigned int)MAX;i++){
    checkOverlap = false;
    for(int j=0; j<n_muon; j++){
      if(deltaR(jets[i].eta(),jets[i].phi(),muon_eta[j],muon_phi[j])<0.5){
	checkOverlap = true;
	break;
      }
    }
    for(int j=0; j<n_electron; j++){
      if(deltaR(jets[i].eta(),jets[i].phi(),electron_eta[j],electron_phi[j])<0.5){
	checkOverlap = true;
	break;
      }
    }
    if(checkOverlap==true) continue;
    //
    jet_pt[n_jet]     = jets[i].pt();
    jet_eta[n_jet]    = jets[i].eta();
    jet_phi[n_jet]    = jets[i].phi();
    jet_mass[n_jet]   = jets[i].mass();
    jet_nhef[n_jet]   = jets[i].neutralHadronEnergyFraction();
    jet_chef[n_jet]   = jets[i].chargedHadronEnergyFraction();
    jet_nemf[n_jet]   = jets[i].neutralEmEnergyFraction();
    jet_cemf[n_jet]   = jets[i].chargedEmEnergyFraction();
    //
    jet_puflag[n_jet] = 0;
    idflag = (*puJetIdFlags)[jetSrc->refAt(i)];
    if( PileupJetIdentifier::passJetId( idflag, PileupJetIdentifier::kLoose  )) jet_puflag[n_jet] = 1;
    if( PileupJetIdentifier::passJetId( idflag, PileupJetIdentifier::kMedium )) jet_puflag[n_jet] = 2;
    if( PileupJetIdentifier::passJetId( idflag, PileupJetIdentifier::kTight  )) jet_puflag[n_jet] = 3;
    //
    n_jet++;
  }
  
  MyNtuple_->Fill();
}

// ------------ method called once each job just before starting event loop  ------------
void 
InvHiggsPUJetNtuple::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
InvHiggsPUJetNtuple::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
void 
InvHiggsPUJetNtuple::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup)
{
  //--- htlConfig_
  //bool changed(true);
  //if (!hltConfig_.init(iRun, iSetup,processname_, changed)) {
  //LogDebug("HLTJetMETDQMSource") << "HLTConfigProvider failed to initialize.";
  //}
}

// ------------ method called when ending the processing of a run  ------------
void 
InvHiggsPUJetNtuple::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
InvHiggsPUJetNtuple::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
InvHiggsPUJetNtuple::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

//define this as a plug-in
DEFINE_FWK_MODULE(InvHiggsPUJetNtuple);
