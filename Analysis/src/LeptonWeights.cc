#include "InvisibleHiggs/Analysis/interface/LeptonWeights.h"

#include "TFile.h"
#include "TTree.h"

LeptonWeights::LeptonWeights() :
  muFile_(std::string(getenv("CMSSW_BASE"))+std::string("/src/InvisibleHiggs/Analysis/data/MuonWeights.txt")),
  elFile_(std::string(getenv("CMSSW_BASE"))+std::string("/src/InvisibleHiggs/Analysis/data/ElectronWeights.txt")),
  muTable_(muFile_),
  elTable_(elFile_)
{

  // get weights
  std::cout << "Reading muon weights from " << muFile_ << std::endl;
  std::cout << "Reading electron weights from " << elFile_ << std::endl;
  
}

void LeptonWeights::createTree(TTree* tree, 
			       std::string oFile,
			       bool isData) {

  // input tree
  Double_t        ele1Pt;
  Double_t        ele1Eta;
  Double_t        ele2Pt;
  Double_t        ele2Eta;
  Double_t        mu1Pt;
  Double_t        mu1Eta;
  Double_t        mu2Pt;
  Double_t        mu2Eta;
 
  TBranch        *b_events_ele1Pt;   //!
  TBranch        *b_events_ele1Eta;   //!
  TBranch        *b_events_ele2Pt;   //!
  TBranch        *b_events_ele2Eta;   //!
  TBranch        *b_events_mu1Pt;   //!
  TBranch        *b_events_mu1Eta;   //!
  TBranch        *b_events_mu2Pt;   //!
  TBranch        *b_events_mu2Eta;   //!

  // new friend tree
  TFile* lwFile;
  TTree* lwTree;

  Double_t el1Weight;
  Double_t el1WeightErr;
  Double_t mu1Weight;
  Double_t mu1WeightErr;
  Double_t mu2Weight;
  Double_t mu2WeightErr;

  // stuff to read what we need from the existing tree
  tree->SetMakeClass(1);
  tree->SetBranchAddress("ele1Pt", &ele1Pt, &b_events_ele1Pt);
  tree->SetBranchAddress("ele1Eta", &ele1Eta, &b_events_ele1Eta);
  tree->SetBranchAddress("ele2Pt", &ele2Pt, &b_events_ele2Pt);
  tree->SetBranchAddress("ele2Eta", &ele2Eta, &b_events_ele2Eta);
  tree->SetBranchAddress("mu1Pt", &mu1Pt, &b_events_mu1Pt);
  tree->SetBranchAddress("mu1Eta", &mu1Eta, &b_events_mu1Eta);
  tree->SetBranchAddress("mu2Pt", &mu2Pt, &b_events_mu2Pt);
  tree->SetBranchAddress("mu2Eta", &mu2Eta, &b_events_mu2Eta);
  
  // create new tree
  lwFile = TFile::Open(oFile.c_str(), "RECREATE");
  
  lwTree = new TTree("lwTree", "Lepton weights");
  lwTree->SetDirectory(0);
  lwTree->Branch("el1Weight", &el1Weight, "el1Weight/D");
  lwTree->Branch("el1WeightErr", &el1WeightErr, "el1WeightErr/D");
  lwTree->Branch("mu1Weight", &mu1Weight, "mu1Weight/D");
  lwTree->Branch("mu1WeightErr", &mu1WeightErr, "mu1WeightErr/D");
  lwTree->Branch("mu2Weight", &mu2Weight, "mu2Weight/D");
  lwTree->Branch("mu2WeightErr", &mu2WeightErr, "mu2WeightErr/D");
  
  // fill the new branch
  int nEvt = tree->GetEntriesFast();
  for (int iEvt=0; iEvt<nEvt; ++iEvt) {
    
    long nb = tree->GetEntry(iEvt);
    if (nb == 0) {  std::cout << "GetEntry() failed" << std::endl; }
    
    el1Weight = 1.;
    el1WeightErr = 0.;
    if (ele1Pt>0. && !isData) {
      el1Weight = elTable_.Val(ele1Pt, ele1Eta);
      el1WeightErr = elTable_.Err(ele1Pt, ele1Eta);
    }      
    
    mu1Weight = 1.;
    mu1WeightErr = 0.;
    if (mu1Pt>0. && !isData) {
      mu1Weight = muTable_.Val(mu1Pt, mu1Eta);
      mu1WeightErr = muTable_.Err(mu1Pt, mu1Eta);
    }
    
    mu2Weight = 1.;
    mu2WeightErr = 0.;
    if (mu2Pt>0. && !isData) {
      mu2Weight = muTable_.Val(mu2Pt, mu2Eta);    
      mu2WeightErr = muTable_.Err(mu2Pt, mu2Eta);    
    }
    
    lwTree->Fill();
    
  }
  
  // write out tree
  lwTree->Write("", TObject::kOverwrite);
  lwFile->Close();

  std::cout << "Wrote lepton weights to " << oFile << std::endl;
  
  if (lwTree!=0) delete lwTree;
  if (lwFile!=0) delete lwFile;

}


void LeptonWeights::addFriend(TTree* tree, 
			      std::string oFile) {

  std::cout << "Adding lepton weights tree : " << oFile << std::endl;
  
  tree->AddFriend("lwTree", oFile.c_str());
  
}

LeptonWeights::~LeptonWeights() {

}
