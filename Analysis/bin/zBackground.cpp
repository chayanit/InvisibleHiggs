
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

  double rBF = 5.626;  //  MCFM + NLO

  Datasets datasets(options.iDir);
  datasets.readFile(options.datasetFile);

  // cuts
  Cuts cuts;
  TCut puWeight("puWeight");
  TCut allCuts = puWeight * cuts.allCuts();

  TCut cutZGen  = puWeight * cuts.zMuMuGen();
  TCut cutZGenM  = puWeight * cuts.zMuMuGenMass();
  TCut cutZReco = puWeight * (cuts.zMuMuReco() + cuts.zMuMuGen());
  TCut cutVBFC  = puWeight * (cuts.zMuMuVBF() + cuts.zMuMuGen());
  TCut cutVBFS  = puWeight * (cuts.vbf() + cuts.zMuMuGen());


  // DY jets
  std::string name("DYJetsToLL");
  Dataset dataset = datasets.getDataset(name);
  TFile* file     = datasets.getTFile(name);
  TTree* tree     = (TTree*) file->Get("invHiggsInfo/InvHiggsInfo");

  double nZGen        = (double) tree->Draw("", cutZGen);
  double nDYZGen      = nZGen * dataset.sigma * lumi / dataset.nEvents;
  double err_nDYZGen  = sqrt(nZGen) * dataset.sigma * lumi / dataset.nEvents ;

  double nZReco       = (double) tree->Draw("", cutZReco);
  double nDYZReco     = nZReco * dataset.sigma * lumi / dataset.nEvents;
  double err_nDYZReco = sqrt(nZReco) * dataset.sigma * lumi / dataset.nEvents ;
  
  double nCVBF        = (double) tree->Draw("", cutVBFC);
  double nDYCVBF      = nCVBF * dataset.sigma * lumi / dataset.nEvents;
  double err_nDYCVBF  = sqrt(nCVBF) * dataset.sigma * lumi / dataset.nEvents;

  double nSVBF        = (double) tree->Draw("", cutVBFS);
  double nDYSVBF      = nSVBF * dataset.sigma * lumi / dataset.nEvents;
  double err_nDYSVBF  = sqrt(nSVBF) * dataset.sigma * lumi / dataset.nEvents;

  double nZGenM       = (double) tree->Draw("", cutZGenM);
  double nDYZGenM     = nZGenM * dataset.sigma * lumi / dataset.nEvents;
  double err_nDYZGenM = sqrt(nZGenM) * dataset.sigma * lumi / dataset.nEvents ;

  //  double nSVBF        = (double) tree->Draw("", cutVBFS);

  std::cout << "DY      : " << nDYZGen << " +/- " << err_nDYZGen << "\t" << nDYZReco << " +/- " << err_nDYZReco << std::endl;

  delete tree;
  file->Close();

  // DY Jets EWK
  name = std::string("DYJetsToLL_EWK");
  dataset = datasets.getDataset(name);
  file     = datasets.getTFile(name);
  tree     = (TTree*) file->Get("invHiggsInfo/InvHiggsInfo");

  nZGen                  = (double) tree->Draw("", cutZGen);
  nZReco                 = (double) tree->Draw("", cutZReco);

  double nDYEwkZGen      = nZGen * dataset.sigma * lumi / dataset.nEvents;
  double nDYEwkZReco     = nZReco * dataset.sigma * lumi / dataset.nEvents;
  
  double err_nDYEwkZGen  = sqrt(nZGen) * dataset.sigma * lumi / dataset.nEvents ;
  double err_nDYEwkZReco = sqrt(nZReco) * dataset.sigma * lumi / dataset.nEvents ;

  std::cout << "DY(ewk) : " << nDYEwkZGen << " +/- " << err_nDYEwkZGen << "\t" << nDYEwkZReco << " +/- " << err_nDYEwkZReco << std::endl;

  delete tree;
  file->Close();

  // DY Jets ptZ-100
  name = std::string("DYJetsToLL_PtZ-100");
  dataset = datasets.getDataset(name);
  file     = datasets.getTFile(name);
  tree     = (TTree*) file->Get("invHiggsInfo/InvHiggsInfo");

  nZGen                  = (double) tree->Draw("", cutZGen);
  nZReco                 = (double) tree->Draw("", cutZReco);

  double nDY100ZGen      = nZGen * dataset.sigma * lumi / dataset.nEvents;
  double nDY100ZReco     = nZReco * dataset.sigma * lumi / dataset.nEvents;
  
  double err_nDY100ZGen  = sqrt(nZGen) * dataset.sigma * lumi / dataset.nEvents ;
  double err_nDY100ZReco = sqrt(nZReco) * dataset.sigma * lumi / dataset.nEvents ;

  std::cout << "DY(100) : " << nDY100ZGen << " +/- " << err_nDY100ZGen << "\t" << nDY100ZReco << " +/- " << err_nDY100ZReco << std::endl;

  std::cout << std::endl;

  // calculate efficiencies
  std::cout << "Efficiencies" << std::endl;

  double epsilonMuMu = nDYZReco / nDYZGen;
  double err_epsilonMuMu = epsilonMuMu * sqrt( pow(err_nDYZReco/nDYZReco,2) +
					       pow(err_nDYZGen/nDYZGen,2) );
  
  std::cout << "epsilon_mumu  : " << epsilonMuMu << " +/- " << err_epsilonMuMu << std::endl;

  double epsilonSVBF =  nDYSVBF / nDYZGenM;
  double err_epsilonSVBF = epsilonSVBF * sqrt( pow(err_nDYSVBF/nDYSVBF,2) +
					       pow(err_nDYZGenM/nDYZGenM,2) );

  std::cout << "epsilon_S_VBF : " << epsilonSVBF << " +/- " << err_epsilonSVBF << std::endl;

  double epsilonCVBF = nDYCVBF / nDYZReco;
  double err_epsilonCVBF = epsilonCVBF * sqrt( pow(err_nDYCVBF/nDYCVBF,2) +
					       pow(err_nDYZReco/nDYZReco,2) );

  std::cout << "epsilon_C_VBF : " << epsilonCVBF << " +/- " << err_epsilonCVBF << std::endl;

  std::cout << std::endl;
  std::cout << "epsilon_S_VBF / epsilon_C_VBF : " << epsilonSVBF / epsilonCVBF << std::endl;

  std::cout << std::endl;
  
  // estimate backgrounds in control region
  std::cout << "Control region" << std::endl;

  std::cout << "Dataset  : no dphi \t\tdphi>2.6 \t\tdphi<1.0" << std::endl;

  // TTbar
  name = std::string("TTbar");
  dataset = datasets.getDataset(name);
  file = datasets.getTFile(name);
  tree = (TTree*) file->Get("invHiggsInfo/InvHiggsInfo");

  double zMuMu = (double) tree->Draw("", cuts.zMuMuVBF());
  double zMuMuHiDPhi = (double) tree->Draw("", cuts.zMuMuVBFHiDPhi());
  double zMuMuLoDPhi = (double) tree->Draw("", cuts.zMuMuVBFLoDPhi());
  
  double nTTbarZMuMu       = zMuMu * dataset.sigma * lumi / dataset.nEvents ;
  double nTTbarZMuMuHiDPhi = zMuMuHiDPhi * dataset.sigma * lumi / dataset.nEvents ;
  double nTTbarZMuMuLoDPhi = zMuMuLoDPhi * dataset.sigma * lumi / dataset.nEvents ;
  
  double err_nTTbarZMuMu       = sqrt(zMuMu) * dataset.sigma * lumi / dataset.nEvents ;
  double err_nTTbarZMuMuHiDPhi = sqrt(zMuMuHiDPhi) * dataset.sigma * lumi / dataset.nEvents ;    
  double err_nTTbarZMuMuLoDPhi = sqrt(zMuMuLoDPhi) * dataset.sigma * lumi / dataset.nEvents ;

  delete tree;
  file->Close();

  std::cout << "TTbar MC : " << nTTbarZMuMu << " +/- " << err_nTTbarZMuMu << "\t"
	    << nTTbarZMuMuHiDPhi << " +/- " << err_nTTbarZMuMuHiDPhi << "\t"
	    << nTTbarZMuMuLoDPhi << " +/- " << err_nTTbarZMuMuLoDPhi << std::endl;

  double nBGZMuMu = nTTbarZMuMu;
  double nBGZMuMuHiDPhi = nTTbarZMuMuHiDPhi;
  double nBGZMuMuLoDPhi = nTTbarZMuMuLoDPhi;

  double err_nBGZMuMu = err_nTTbarZMuMu;
  double err_nBGZMuMuHiDPhi = err_nTTbarZMuMuHiDPhi;
  double err_nBGZMuMuLoDPhi = err_nTTbarZMuMuLoDPhi;


  // get numbers from data control regions
  file = datasets.getTFile("METABCD");
  tree = (TTree*) file->Get("invHiggsInfo/InvHiggsInfo");

  int nDataZMuMu       = tree->Draw("", cuts.zMuMuVBF());
  int nDataZMuMuLoDPhi = tree->Draw("", cuts.zMuMuVBFLoDPhi());
  int nDataZMuMuHiDPhi = tree->Draw("", cuts.zMuMuVBFHiDPhi());

  double err_nDataZMuMu       = sqrt(nDataZMuMu);
  double err_nDataZMuMuLoDPhi = sqrt(nDataZMuMuLoDPhi);
  double err_nDataZMuMuHiDPhi = sqrt(nDataZMuMuHiDPhi);

  std::cout << "Data     : " << nDataZMuMu << " +/- " << err_nDataZMuMu << "\t"
	    << nDataZMuMuHiDPhi << " +/- " << err_nDataZMuMuHiDPhi << "\t"
	    << nDataZMuMuLoDPhi << " +/- " << err_nDataZMuMuLoDPhi << std::endl;

  double nDataBGZMuMu       = nDataZMuMu - nBGZMuMu;
  double nDataBGZMuMuHiDPhi = nDataZMuMuHiDPhi - nBGZMuMuHiDPhi;
  double nDataBGZMuMuLoDPhi = nDataZMuMuLoDPhi - nBGZMuMuLoDPhi;

  double err_nDataBGZMuMu       = sqrt( nDataZMuMu + err_nBGZMuMu );
  double err_nDataBGZMuMuHiDPhi = sqrt( nDataZMuMuHiDPhi + err_nBGZMuMuHiDPhi );
  double err_nDataBGZMuMuLoDPhi = sqrt( nDataZMuMuLoDPhi + err_nBGZMuMuLoDPhi );

  std::cout << "Data-BG  : " << nDataBGZMuMu << " +/- " << err_nDataBGZMuMu << "\t"
	    << nDataBGZMuMuHiDPhi << " +/- " << err_nDataBGZMuMuHiDPhi << "\t"
	    << nDataBGZMuMuLoDPhi << " +/- " << err_nDataBGZMuMuLoDPhi << std::endl;
  std::cout << std::endl;
  

  // calculate final results
  std::cout << "Final BG estimate" << std::endl;
  double nEst       = (nDataZMuMu - nBGZMuMu) * rBF * epsilonSVBF / (epsilonCVBF * epsilonMuMu);
  double nEstHiDPhi = (nDataZMuMuHiDPhi - nBGZMuMuHiDPhi) * rBF * epsilonSVBF / (epsilonCVBF * epsilonMuMu);
  double nEstLoDPhi = (nDataZMuMuLoDPhi - nBGZMuMuLoDPhi) * rBF * epsilonSVBF / (epsilonCVBF * epsilonMuMu);

  double err_nEst = nEst * sqrt ( 1/nDataZMuMu +
				  pow(err_epsilonSVBF/epsilonSVBF, 2) +
				  pow(err_epsilonCVBF/epsilonCVBF, 2) +
				  pow(err_epsilonMuMu/epsilonMuMu, 2) +
				  pow(err_nBGZMuMu/nBGZMuMu, 2) );

  double err_nEstHiDPhi = nEstHiDPhi * sqrt ( 1/nDataZMuMu +
				  pow(err_epsilonSVBF/epsilonSVBF, 2) +
				  pow(err_epsilonCVBF/epsilonCVBF, 2) +
				  pow(err_epsilonMuMu/epsilonMuMu, 2) +
				  pow(err_nBGZMuMu/nBGZMuMu, 2) );

  double err_nEstLoDPhi = nEstLoDPhi * sqrt ( 1/nDataZMuMu +
				  pow(err_epsilonSVBF/epsilonSVBF, 2) +
				  pow(err_epsilonCVBF/epsilonCVBF, 2) +
				  pow(err_epsilonMuMu/epsilonMuMu, 2) +
				  pow(err_nBGZMuMu/nBGZMuMu, 2) );

  std::cout << "Estimate : " << nEst << " +/- " << err_nEst << "\t"
	    << nEstHiDPhi << " +/- " << err_nEstHiDPhi << "\t"
	    << nEstLoDPhi << " +/- " << err_nEstLoDPhi << std::endl;


}
