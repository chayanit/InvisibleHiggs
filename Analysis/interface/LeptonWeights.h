#ifndef LeptonWeights_h
#define LeptonWeights_h

// this class will create a TTree containing lepton ID weights, and make it a friend of the input TTree

#include "TFile.h"
#include "TTree.h"

class LeptonWeights {

 public:
  LeptonWeights(TTree* tree, 
		std::string oFile,
		bool recreate);
  ~LeptonWeights();

 private:

  // input files
  std::string muFile_;
  std::string elFile_;

  // old tree
  TTree* tree_;

  Double_t        ele1Pt_;
  Double_t        ele1Eta_;
  Double_t        ele2Pt_;
  Double_t        ele2Eta_;
  Double_t        mu1Pt_;
  Double_t        mu1Eta_;
  Double_t        mu2Pt_;
  Double_t        mu2Eta_;
 
  TBranch        *b_events_ele1Pt_;   //!
  TBranch        *b_events_ele1Eta_;   //!
  TBranch        *b_events_ele2Pt_;   //!
  TBranch        *b_events_ele2Eta_;   //!
  TBranch        *b_events_mu1Pt_;   //!
  TBranch        *b_events_mu1Eta_;   //!
  TBranch        *b_events_mu2Pt_;   //!
  TBranch        *b_events_mu2Eta_;   //!

  // new friend tree
  std::string oFile_;
  TFile* lwFile_;
  TTree* lwTree_;

  Double_t el1Weight_;
  Double_t el1WeightErr_;
  Double_t mu1Weight_;
  Double_t mu1WeightErr_;
  Double_t mu2Weight_;
  Double_t mu2WeightErr_;

};

#endif
