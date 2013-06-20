#include "InvisibleHiggs/Analysis/interface/LeptonWeights.h"

#include "InvisibleHiggs/Analysis/interface/TableReader.h"

#include "TFile.h"
#include "TTree.h"

LeptonWeights::LeptonWeights(TTree* tree, 
			     std::string oFile,
			     bool recreate) :
  tree_(tree),
  oFile_(oFile),
  lwTree_(0),
  lwFile_(0)
{
  
  if (recreate) {

    // get weights
    muFile_ = std::string(getenv("CMSSW_BASE"))+std::string("/src/InvisibleHiggs/Analysis/data/MuonWeights.txt");
    std::cout << "Reading muon weights from " << muFile_ << std::endl;
    TableReader muTable(muFile_);
    
    elFile_ = std::string(getenv("CMSSW_BASE"))+std::string("/src/InvisibleHiggs/Analysis/data/ElectronWeights.txt"); 
    std::cout << "Reading electron weights from " << elFile_ << std::endl;
    TableReader eleTable(elFile_);
    
    // stuff to read what we need from the existing tree
    tree_->SetMakeClass(1);
    tree_->SetBranchAddress("ele1Pt", &ele1Pt_, &b_events_ele1Pt_);
    tree_->SetBranchAddress("ele1Eta", &ele1Eta_, &b_events_ele1Eta_);
    tree_->SetBranchAddress("ele2Pt", &ele2Pt_, &b_events_ele2Pt_);
    tree_->SetBranchAddress("ele2Eta", &ele2Eta_, &b_events_ele2Eta_);
    tree_->SetBranchAddress("mu1Pt", &mu1Pt_, &b_events_mu1Pt_);
    tree_->SetBranchAddress("mu1Eta", &mu1Eta_, &b_events_mu1Eta_);
    tree_->SetBranchAddress("mu2Pt", &mu2Pt_, &b_events_mu2Pt_);
    tree_->SetBranchAddress("mu2Eta", &mu2Eta_, &b_events_mu2Eta_);
    
    // create new tree
    lwFile_ = TFile::Open(oFile_.c_str(), "RECREATE");
    
    lwTree_ = new TTree("lwTree", "Lepton weights");
    lwTree_->SetDirectory(0);
    lwTree_->Branch("el1Weight", &el1Weight_, "el1Weight/D");
    lwTree_->Branch("el1WeightErr", &el1WeightErr_, "el1WeightErr/D");
    lwTree_->Branch("mu1Weight", &mu1Weight_, "mu1Weight/D");
    lwTree_->Branch("mu1WeightErr", &mu1WeightErr_, "mu1WeightErr/D");
    lwTree_->Branch("mu2Weight", &mu2Weight_, "mu2Weight/D");
    lwTree_->Branch("mu2WeightErr", &mu2WeightErr_, "mu2WeightErr/D");
    
    // fill the new branch
    int nEvt = tree_->GetEntriesFast();
    for (int iEvt=0; iEvt<nEvt; ++iEvt) {
      
      long nb = tree_->GetEntry(iEvt);
      if (nb == 0) {  std::cout << "GetEntry() failed" << std::endl; }
      
      el1Weight_ = 1.;
      el1WeightErr_ = 0.;
      if (ele1Pt_>0.) {
	el1Weight_ = eleTable.Val(ele1Pt_, ele1Eta_);
	el1WeightErr_ = eleTable.Err(ele1Pt_, ele1Eta_);
      }      
      
      mu1Weight_ = 1.;
      mu1WeightErr_ = 0.;
      if (mu1Pt_>0.) {
	mu1Weight_ = muTable.Val(mu1Pt_, mu1Eta_);
	mu1WeightErr_ = muTable.Err(mu1Pt_, mu1Eta_);
      }
      
      mu2Weight_ = 1.;
      mu2WeightErr_ = 0.;
      if (mu2Pt_>0.) {
	mu2Weight_ = muTable.Val(mu2Pt_, mu2Eta_);    
	mu2WeightErr_ = muTable.Err(mu2Pt_, mu2Eta_);    
      }
      
      lwTree_->Fill();
      
    }
    
    // write out tree
    lwTree_->Write("", TObject::kOverwrite);
    lwFile_->Close();

  }

  // add friend
  tree_->AddFriend("lwTree", oFile_.c_str());

  std::cout << "Added lepton weights tree" << std::endl;

}

LeptonWeights::~LeptonWeights() {


  if (lwTree_!=0) delete lwTree_;
  if (lwFile_!=0) delete lwFile_;

}
