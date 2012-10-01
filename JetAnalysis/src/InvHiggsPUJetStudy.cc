// -*- C++ -*-
//
// Package:    InvisibleHiggs/Analysis
// Class:      TInvHiggsPUJetStudy
// 
/**\class InvHiggsPUJetStudy InvHiggsPUJetStudy.cc InvisibleHiggs/Analysis/plugin/InvHiggsPUJetStudy.cc

 Description: Study of PUJets

 Implementation:
     
*/
//
// Original Author:  Phat Srimanobhas
//         Created:
//
//


// Framework
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

// DataFormats
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
#include "PhysicsTools/SelectorUtils/interface/PFJetIDSelectionFunctor.h"

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
#include "TLorentzVector.h"

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

class InvHiggsPUJetStudy : public edm::EDAnalyzer {

public:

  explicit InvHiggsPUJetStudy(const edm::ParameterSet&);
  ~InvHiggsPUJetStudy();
  
private:

  virtual void beginJob() ;
  virtual void beginRun(const edm::Run&, const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ; 

  //Phat--Trigger
  virtual bool isHLTPathAccepted(std::string pathName);
  const std::string getL1ConditionModuleName(const std::string& pathname);
  virtual bool isTriggerObjectFound(std::string objectName);
  
public:
  
private:

  // Trigger objects
  edm::InputTag trigEvent_;
  edm::InputTag trigResults_;
  edm::TriggerNames triggerNames;
  edm::Handle<edm::TriggerResults> trigResults;
  edm::Handle<trigger::TriggerEvent> trigEvent; 
  HLTConfigProvider hltConfig_;
  //
  double Jet1HLTPt[100];
  double Jet1HLTEta[100];
  double Jet1HLTPhi[100]; 
  double Jet1HLTM[100];  
  double Jet1HLTPx[100];
  double Jet1HLTPy[100];
  double Jet1HLTPz[100]; 
  double Jet1HLTE[100]; 
  double Jet1HLTEt[100];
  //
  double Jet2HLTPt[100];
  double Jet2HLTEta[100];
  double Jet2HLTPhi[100]; 
  double Jet2HLTM[100];
  double Jet2HLTPx[100];
  double Jet2HLTPy[100];
  double Jet2HLTPz[100]; 
  double Jet2HLTE[100]; 
  double Jet2HLTEt[100];
  //
  int    countHLTPair;

  // JetMET
  edm::InputTag jetTag_;
  edm::InputTag puJetMvaTag_;
  edm::InputTag puJetIdTag_;
  edm::InputTag metTag_;
  edm::Handle<edm::View<pat::Jet> > jets;
  edm::Handle<edm::ValueMap<float> > puJetIdMVA;
  edm::Handle<edm::ValueMap<int> > puJetIdFlag;
  double JetRecoPt[100];
  double JetRecoEta[100];
  double JetRecoPhi[100]; 
  double JetRecoM[100];  
  double JetRecoPx[100];
  double JetRecoPy[100];
  double JetRecoPz[100]; 
  double JetRecoE[100]; 
  double JetRecoEt[100];
  double JetRecoPUMVA[100];
  int    JetRecoPUFlag[100];
  double JetRecoNHEF[100];
  double JetRecoCHEF[100];
  double JetRecoNEMF[100];
  double JetRecoCEMF[100]; 
  double JetRecoN90Hits[100];
  double JetRecofHPD[100];
  double JetRecofRBX[100];
  double JetRecoIDEmf[100]; 
  int    JetRecoIDLOOSE[100];
  int    JetRecoIDTIGHT[100];
  int    countJetReco;

  // Selection
  bool   leadingJets_;

  edm::Handle<pat::METCollection> met;

  // Leptons
  edm::InputTag muonTag_;
  edm::InputTag electronTag_;  
  edm::Handle<pat::MuonCollection> muons; 
  edm::Handle<pat::ElectronCollection> electrons;
  
  // Output file
  //edm::Service<TFileService> fs;
  
  // tree
  //TTree * tree_;
  //InvHiggsInfo* info_;

  // PU re-weighting
  //edm::LumiReWeighting lumiWeights_;
};




InvHiggsPUJetStudy::InvHiggsPUJetStudy(const edm::ParameterSet& iConfig)
{
  //
  trigEvent_     = iConfig.getUntrackedParameter<edm::InputTag>("trigEvent",edm::InputTag("hltTriggerSummaryAOD","","HLT"));
  trigResults_   = iConfig.getUntrackedParameter<edm::InputTag>("trigResults",edm::InputTag("TriggerResults","","HLT"));
  //processname_   = iConfig.getUntrackedParameter<std::string>("processname",std::string("HLT")); 
  //
  jetTag_        = iConfig.getUntrackedParameter<edm::InputTag>("jetTag",edm::InputTag("patJets"));
  puJetMvaTag_   = iConfig.getUntrackedParameter<edm::InputTag>("puJetMvaTag",edm::InputTag("puJetMva:fullDiscriminant"));
  puJetIdTag_    = iConfig.getUntrackedParameter<edm::InputTag>("puJetIdTag",edm::InputTag("puJetMva:fullId"));
  metTag_        = iConfig.getUntrackedParameter<edm::InputTag>("metTag",edm::InputTag("patMET"));
  
  //
  muonTag_       = iConfig.getUntrackedParameter<edm::InputTag>("muonTag",edm::InputTag("patMuons"));
  electronTag_   = iConfig.getUntrackedParameter<edm::InputTag>("electronTag",edm::InputTag("patElectrons"));

  leadingJets_   = iConfig.getUntrackedParameter<bool>("leadingJets",true);
}


InvHiggsPUJetStudy::~InvHiggsPUJetStudy()
{
}


//
// member functions
//

// ------------ method called once each job just before starting event loop  ------------
void InvHiggsPUJetStudy::beginJob()
{
}

// -- called once per run
void InvHiggsPUJetStudy::beginRun(edm::Run const & iRun, edm::EventSetup const& iSetup)
{
  // HLT setup
  bool changed;
  hltConfig_.init(iRun, iSetup, trigResults_.process(), changed);
}


// ------------ method called once each job just after ending the event loop  ------------
void 
InvHiggsPUJetStudy::endJob() 
{
}

// ------------ method called to for each event  ------------
void
InvHiggsPUJetStudy::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{ 
  using namespace std;
  
  //unsigned long id          = iEvent.id().event();
  //unsigned long bx          = iEvent.bunchCrossing();
  //unsigned long orbit       = iEvent.orbitNumber();
  //unsigned long lb          = iEvent.luminosityBlock();
  //unsigned long run         = iEvent.id().run();
  //double time               = iEvent.time().value();
  
  //Clear all information 
  countHLTPair = 0;
  countJetReco = 0;
  for(int i=0; i<100; i++){
    Jet1HLTPt[i]  = -999;
    Jet1HLTEta[i] = -999;
    Jet1HLTPhi[i] = -999; 
    Jet1HLTM[i]   = -999;  
    Jet1HLTPx[i]  = -999;
    Jet1HLTPy[i]  = -999;
    Jet1HLTPz[i]  = -999; 
    Jet1HLTE[i]   = -999; 
    Jet1HLTEt[i]  = -999;
    //
    Jet2HLTPt[i]  = -999;
    Jet2HLTEta[i] = -999;
    Jet2HLTPhi[i] = -999; 
    Jet2HLTM[i]   = -999;
    Jet2HLTPx[i]  = -999;
    Jet2HLTPy[i]  = -999;
    Jet2HLTPz[i]  = -999; 
    Jet2HLTE[i]   = -999; 
    Jet2HLTEt[i]  = -999;
    //
    JetRecoPt[i]  = -999;
    JetRecoEta[i] = -999;
    JetRecoPhi[i] = -999; 
    JetRecoM[i]   = -999;  
    JetRecoPx[i]  = -999;
    JetRecoPy[i]  = -999;
    JetRecoPz[i]  = -999; 
    JetRecoE[i]   = -999; 
    JetRecoEt[i]  = -999;
    JetRecoPUMVA[i]   = -999;
    JetRecoPUFlag[i]  = -999;
    JetRecoNHEF[i]    = -999;
    JetRecoCHEF[i]    = -999;
    JetRecoNEMF[i]    = -999;
    JetRecoCEMF[i]    = -999; 
    JetRecoN90Hits[i] = -999;
    JetRecofHPD[i]    = -999;
    JetRecofRBX[i]    = -999;
    JetRecoIDEmf[i]   = -999; 
    JetRecoIDLOOSE[i] = -999;
    JetRecoIDTIGHT[i] = -999;
  }


  iEvent.getByLabel(trigEvent_,trigEvent);
  if(!trigEvent.isValid())   return;
  iEvent.getByLabel(trigResults_, trigResults); 
  if(!trigResults.isValid()) return;
  if(&trigResults) triggerNames = iEvent.triggerNames(*trigResults);
  
  iEvent.getByLabel(jetTag_, jets);
  if (!jets.isValid()) return; 
  iEvent.getByLabel(puJetMvaTag_,puJetIdMVA);
  iEvent.getByLabel(puJetIdTag_,puJetIdFlag);

  iEvent.getByLabel(metTag_, met);
  if (!met.isValid()) return; 

  iEvent.getByLabel(muonTag_,muons);
  iEvent.getByLabel(electronTag_,electrons);
  
  //Collect trigger objects
  int  checkPass = 0; //1=Leading,2=All,3=Both
  bool checkStoreHLT = false;
  countHLTPair = 0;
  const trigger::TriggerObjectCollection & toc(trigEvent->getObjects());

  const unsigned int n(hltConfig_.size());
  std::string pathname;
  std::string filtername = "dummy";
  for(unsigned int i=0; i!=n; ++i){
    pathname = hltConfig_.triggerName(i);
    if(pathname.find("HLT_DiPFJet40_PFMETnoMu65_MJJ600VBF_LeadingJets_v") != std::string::npos && isHLTPathAccepted(pathname))
      checkPass+=1;
    if(pathname.find("HLT_DiPFJet40_PFMETnoMu65_MJJ800VBF_AllJets_v") != std::string::npos && isHLTPathAccepted(pathname))
      checkPass+=2;
    
    if(checkPass==0) continue;
    if(checkStoreHLT==true) continue;
    
    std::vector<std::string> numpathmodules = hltConfig_.moduleLabels(pathname);
    std::vector<std::string>::iterator numpathmodule = numpathmodules.begin();
    for(; numpathmodule!= numpathmodules.end(); ++numpathmodule ){
      edm::InputTag testTag(*numpathmodule,"",trigResults_.process());
      if(hltConfig_.moduleType(*numpathmodule) != "HLTPFJetVBFFilter") continue;
      filtername = *numpathmodule;
       edm::InputTag hltTag(filtername,"",trigResults_.process());
       const int hltIndex = trigEvent->filterIndex(hltTag);
       if(hltIndex >= trigEvent->sizeFilters()) continue;
       const trigger::Keys & khlt = trigEvent->filterKeys(hltIndex);
       trigger::Keys::const_iterator kj = khlt.begin();
       for(;kj!=khlt.end();kj+=2){ 
	 Jet1HLTPt[countHLTPair]  = toc[*kj].pt();
	 Jet1HLTEta[countHLTPair] = toc[*kj].eta();
	 Jet1HLTPhi[countHLTPair] = toc[*kj].phi();
	 Jet1HLTM[countHLTPair]   = toc[*kj].mass();
	 Jet1HLTPx[countHLTPair]  = toc[*kj].px();
	 Jet1HLTPy[countHLTPair]  = toc[*kj].py();
	 Jet1HLTPz[countHLTPair]  = toc[*kj].pz(); 
	 Jet1HLTE[countHLTPair]   = toc[*kj].energy();
	 Jet1HLTEt[countHLTPair]  = toc[*kj].et();
	 //
	 Jet2HLTPt[countHLTPair]  = toc[*(kj+1)].pt();
	 Jet2HLTEta[countHLTPair] = toc[*(kj+1)].eta();
	 Jet2HLTPhi[countHLTPair] = toc[*(kj+1)].phi();
	 Jet2HLTM[countHLTPair]   = toc[*(kj+1)].mass();
	 Jet2HLTPx[countHLTPair]  = toc[*(kj+1)].px();
	 Jet2HLTPy[countHLTPair]  = toc[*(kj+1)].py();
	 Jet2HLTPz[countHLTPair]  = toc[*(kj+1)].pz(); 
	 Jet2HLTE[countHLTPair]   = toc[*(kj+1)].energy();
	 Jet2HLTEt[countHLTPair]  = toc[*(kj+1)].et();
	 //
	 countHLTPair++;
       }
       checkStoreHLT=true;
       break;
    }
  }
  
  //Collect RecoJet
  countJetReco = 0;
  for(unsigned i=0; i<jets->size(); ++i){
    
    //const pat::Jet& jet = jets->at(i).correctedJet("Uncorrected");
    //PFJetIDSelectionFunctor pfjetIDFunctorLoose(PFJetIDSelectionFunctor::FIRSTDATA, PFJetIDSelectionFunctor::LOOSE);
    //PFJetIDSelectionFunctor pfjetIDFunctorTight(PFJetIDSelectionFunctor::FIRSTDATA, PFJetIDSelectionFunctor::TIGHT);
    
    JetRecoPt[countJetReco]     = jets->at(i).pt();
    JetRecoEta[countJetReco]    = jets->at(i).eta();
    JetRecoPhi[countJetReco]    = jets->at(i).phi();
    JetRecoM[countJetReco]      = jets->at(i).mass();  
    JetRecoPx[countJetReco]     = jets->at(i).px();
    JetRecoPy[countJetReco]     = jets->at(i).py();
    JetRecoPz[countJetReco]     = jets->at(i).pz(); 
    JetRecoE[countJetReco]      = jets->at(i).energy();
    JetRecoEt[countJetReco]     = jets->at(i).et();
    
    JetRecoPUMVA[countJetReco]  = (*puJetIdMVA)[jets->refAt(i)];
    JetRecoPUFlag[countJetReco] = (*puJetIdFlag)[jets->refAt(i)];
    
    JetRecoNHEF[countJetReco]   = jets->at(i).neutralHadronEnergyFraction();
    JetRecoCHEF[countJetReco]   = jets->at(i).chargedHadronEnergyFraction();
    JetRecoNEMF[countJetReco]   = jets->at(i).neutralEmEnergyFraction();
    JetRecoCEMF[countJetReco]   = jets->at(i).chargedEmEnergyFraction();
    
    //JetRecoN90Hits[countJetReco] =  jet.jetID().n90Hits;
    //JetRecofHPD[countJetReco]    =  jet.jetID().fHPD;
    //JetRecofRBX[countJetReco]    =  jet.jetID().fRBX;
    //JetRecoIDEmf[countJetReco]   =  jet.jetID().restrictedEMF;

    //JetRecoIDLOOSE[countJetReco] = pfjetIDFunctorLoose(jets->at(i));
    //JetRecoIDTIGHT[countJetReco] = pfjetIDFunctorTight(jets->at(i));

    /*
      if(countJetReco==0){
      cout << " | "
      << JetRecoPt[0]    << " | "
      << JetRecoPUMVA[0] << " | "
      << JetRecoPUFlag[0] << " | "
      << PileupJetIdentifier::passJetId( JetRecoPUFlag[0], PileupJetIdentifier::kLoose ) << " | "
      //<< JetRecoN90Hits[0] << " | "
      // << JetRecofHPD[0] << " | "
      // << JetRecofRBX[0] << " | "
      // << JetRecoIDEmf[0] << " | "
      // << JetRecoIDLOOSE[0] << " | "
      // << JetRecoIDTIGHT[0] << " | "
      << endl;
      }
    */
    countJetReco++;
  }

  //Analyze start here
  TLorentzVector reco1;
  TLorentzVector reco2;
  TLorentzVector hlt1;
  TLorentzVector hlt2;
  int hltMatchPair = -1;
  for(int ireco = 0; ireco < countJetReco ; ireco++){
    if(JetRecoPt[ireco]<30.) continue;
    if(!PileupJetIdentifier::passJetId(JetRecoPUFlag[ireco], PileupJetIdentifier::kLoose)) continue;
    for(int ireco2 = ireco+1; ireco2 < countJetReco ; ireco2++){
      if(JetRecoPt[ireco2]<30.) continue;
      if(!PileupJetIdentifier::passJetId(JetRecoPUFlag[ireco2], PileupJetIdentifier::kLoose)) continue;
      reco1.SetPxPyPzE(JetRecoPx[ireco],JetRecoPy[ireco],JetRecoPz[ireco],JetRecoE[ireco]); 
      reco2.SetPxPyPzE(JetRecoPx[ireco2],JetRecoPy[ireco2],JetRecoPz[ireco2],JetRecoE[ireco2]);
      for(int iHLT = 0; iHLT<countHLTPair; iHLT++){
	hlt1.SetPxPyPzE(Jet1HLTPx[iHLT],Jet1HLTPy[iHLT],Jet1HLTPz[iHLT],Jet1HLTE[iHLT]); 
	hlt2.SetPxPyPzE(Jet2HLTPx[iHLT],Jet2HLTPy[iHLT],Jet2HLTPz[iHLT],Jet2HLTE[iHLT]);
	//Direct Match
	if(reco1.DeltaR(hlt1)<0.4 && reco2.DeltaR(hlt2)<0.4) hltMatchPair = iHLT;
	//Cross Match
	if(reco2.DeltaR(hlt1)<0.4 && reco1.DeltaR(hlt2)<0.4) hltMatchPair = iHLT;
	//
	if(hltMatchPair>=0) break;
      }
      if(leadingJets_) break; //only leading
      if(!leadingJets_ && hltMatchPair>=0) break; //only leading
    }
    if(leadingJets_) break; //only leading
    if(!leadingJets_ && hltMatchPair>=0) break; //only leading
  }
  
  //Result
  cout<<"Trigger Selection: " << checkPass
      <<", Match HLT-RECO: "<< hltMatchPair
      <<endl;
	
}


const std::string InvHiggsPUJetStudy::getL1ConditionModuleName(const std::string& pathname){
  std::string l1pathname = "dummy";
  std::vector<std::string> numpathmodules = hltConfig_.moduleLabels(pathname);
  for(std::vector<std::string>::iterator numpathmodule = numpathmodules.begin();
  numpathmodule!= numpathmodules.end(); ++numpathmodule ){
    if (hltConfig_.moduleType(*numpathmodule) == "HLTLevel1GTSeed") {
      l1pathname = *numpathmodule;
      //std::cout<<"L1 = "<<l1pathname<<std::endl;
      break;
    }
  } // end for
  return l1pathname;
}


bool 
InvHiggsPUJetStudy::isHLTPathAccepted(std::string pathName){
  bool output=false;
  if(&trigResults) {
    unsigned index = triggerNames.triggerIndex(pathName);
    if(index < triggerNames.size() && trigResults->accept(index)) output = true;
  }
  return output;
}


bool 
InvHiggsPUJetStudy::isTriggerObjectFound(std::string objectName){
  // processname_, triggerObj_ has to be defined before calling this method
  bool output=false;
  edm::InputTag testTag(objectName,"",trigResults_.process());
  const int index = trigEvent->filterIndex(testTag);
  if ( index < trigEvent->sizeFilters()){     
    const trigger::Keys & k = trigEvent->filterKeys(index);
    if (k.size()) output=true;
  }
  return output;
}


//define this as a plug-in
DEFINE_FWK_MODULE(InvHiggsPUJetStudy);
