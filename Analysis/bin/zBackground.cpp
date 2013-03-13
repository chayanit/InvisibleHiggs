
#include "InvisibleHiggs/Analysis/interface/ProgramOptions.h"
#include "InvisibleHiggs/Analysis/interface/Cuts.h"
#include "InvisibleHiggs/Analysis/interface/Histogrammer.h"
#include "InvisibleHiggs/Analysis/interface/StackPlot.h"
#include "InvisibleHiggs/Analysis/interface/SumDatasets.h"
#include "InvisibleHiggs/Analysis/interface/Datasets.h"

#include "TTree.h"

#include <iostream>
#include <cmath>

int main(int argc, char* argv[]) {

  ProgramOptions options(argc, argv);

  double lumi = options.lumi;

  Datasets datasets(options.iDir);
  datasets.readFile(options.datasetFile);

  // cuts
  Cuts cuts;
  TCut puWeight("puWeight");
  TCut allCuts = puWeight * cuts.allCuts();

  // estimate yields from MC

  // DY jets
  double nDYZMuMu, err_nDYZMuMu;
  double nDYZMuMuHiDPhi, err_nDYZMuMuHiDPhi, nDYZMuMuLoDPhi, err_nDYZMuMuLoDPhi;
  
  std::string name("DYJetsToLL");
  Dataset dataset = datasets.getDataset(name);
  TFile* file = datasets.getTFile(name);
  TTree* tree = (TTree*) file->Get("invHiggsInfo/InvHiggsInfo");

  double zMuMu = (double) tree->Draw("", cuts.zMuMu());
  double zMuMuHiDPhi = (double) tree->Draw("", cuts.zMuMuHiDPhi());
  double zMuMuLoDPhi = (double) tree->Draw("", cuts.zMuMuLoDPhi());
  
  nDYZMuMu       = zMuMu * dataset.sigma * lumi / dataset.nEvents ;
  nDYZMuMuHiDPhi = zMuMuHiDPhi * dataset.sigma * lumi / dataset.nEvents ;
  nDYZMuMuLoDPhi = zMuMuLoDPhi * dataset.sigma * lumi / dataset.nEvents ;
  
  err_nDYZMuMu       = sqrt(zMuMu) * dataset.sigma * lumi / dataset.nEvents ;
  err_nDYZMuMuHiDPhi = sqrt(zMuMuHiDPhi) * dataset.sigma * lumi / dataset.nEvents ;    
  err_nDYZMuMuLoDPhi = sqrt(zMuMuLoDPhi) * dataset.sigma * lumi / dataset.nEvents ;

  delete tree;
  file->Close();

  std::cout << "DY       : " << nDYZMuMu << " +/- " << err_nDYZMuMu << "\t"
	    << nDYZMuMuHiDPhi << " +/- " << err_nDYZMuMuHiDPhi << "\t"
	    << nDYZMuMuLoDPhi << " +/- " << err_nDYZMuMuLoDPhi << std::endl;


  // TTbar
  double nTTbarZMuMu, err_nTTbarZMuMu;
  double nTTbarZMuMuHiDPhi, err_nTTbarZMuMuHiDPhi, nTTbarZMuMuLoDPhi, err_nTTbarZMuMuLoDPhi;
  
  name = std::string("TTbar");
  dataset = datasets.getDataset(name);
  file = datasets.getTFile(name);
  tree = (TTree*) file->Get("invHiggsInfo/InvHiggsInfo");

  zMuMu = (double) tree->Draw("", cuts.zMuMu());
  zMuMuHiDPhi = (double) tree->Draw("", cuts.zMuMuHiDPhi());
  zMuMuLoDPhi = (double) tree->Draw("", cuts.zMuMuLoDPhi());
  
  nTTbarZMuMu       = zMuMu * dataset.sigma * lumi / dataset.nEvents ;
  nTTbarZMuMuHiDPhi = zMuMuHiDPhi * dataset.sigma * lumi / dataset.nEvents ;
  nTTbarZMuMuLoDPhi = zMuMuLoDPhi * dataset.sigma * lumi / dataset.nEvents ;
  
  err_nTTbarZMuMu       = sqrt(zMuMu) * dataset.sigma * lumi / dataset.nEvents ;
  err_nTTbarZMuMuHiDPhi = sqrt(zMuMuHiDPhi) * dataset.sigma * lumi / dataset.nEvents ;    
  err_nTTbarZMuMuLoDPhi = sqrt(zMuMuLoDPhi) * dataset.sigma * lumi / dataset.nEvents ;

  delete tree;
  file->Close();

  std::cout << "TTbar    : " << nTTbarZMuMu << " +/- " << err_nTTbarZMuMu << "\t"
	    << nTTbarZMuMuHiDPhi << " +/- " << err_nTTbarZMuMuHiDPhi << "\t"
	    << nTTbarZMuMuLoDPhi << " +/- " << err_nTTbarZMuMuLoDPhi << std::endl;



  // calculate efficiencies from MC


  // get numbers from data control regions
  std::cout << "Getting counts from data" << std::endl;

  file = datasets.getTFile("METABCD");
  tree = (TTree*) file->Get("invHiggsInfo/InvHiggsInfo");

  int nDataZMuMu       = tree->Draw("", cuts.zMuMu());
  int nDataZMuMuLoDPhi = tree->Draw("", cuts.zMuMuLoDPhi());
  int nDataZMuMuHiDPhi = tree->Draw("", cuts.zMuMuHiDPhi());

  double err_nDataZMuMu       = sqrt(nDataZMuMu);
  double err_nDataZMuMuLoDPhi = sqrt(nDataZMuMuLoDPhi);
  double err_nDataZMuMuHiDPhi = sqrt(nDataZMuMuHiDPhi);

  std::cout << "Data     : " << nDataZMuMu << " +/- " << err_nDataZMuMu << "\t"
	    << nDataZMuMuHiDPhi << " +/- " << err_nDataZMuMuHiDPhi << "\t"
	    << nDataZMuMuLoDPhi << " +/- " << err_nDataZMuMuLoDPhi << std::endl;
  std::cout << std::endl;
  
  // produce final output



}
