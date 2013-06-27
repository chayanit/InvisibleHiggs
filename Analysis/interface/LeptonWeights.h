#ifndef LeptonWeights_h
#define LeptonWeights_h

// utility class for handling lepton weights. following functionality available :
//    - read tables of lepton weights
//    - save them in a TTree
//    - make that TTree a friend of the input TTree


#include "InvisibleHiggs/Analysis/interface/TableReader.h"

#include "TFile.h"
#include "TTree.h"

class LeptonWeights {

 public:
  LeptonWeights();
  ~LeptonWeights();

  void createTree(TTree* tree, 
		  std::string oFile,
		  bool isData);

  void addFriend(TTree* tree, 
		 std::string oFile);  

 private:
  
  // input files
  std::string muFile_;
  std::string elFile_;

  TableReader muTable_;
  TableReader elTable_;

};

#endif
