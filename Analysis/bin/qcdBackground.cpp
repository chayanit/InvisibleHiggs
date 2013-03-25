
#include "InvisibleHiggs/Analysis/interface/ProgramOptions.h"
#include "InvisibleHiggs/Analysis/interface/Cuts.h"
#include "InvisibleHiggs/Analysis/interface/Histogrammer.h"
#include "InvisibleHiggs/Analysis/interface/StackPlot.h"
#include "InvisibleHiggs/Analysis/interface/SumDatasets.h"
#include "InvisibleHiggs/Analysis/interface/Datasets.h"

#include "TTree.h"
#include "TMath.h"
#include "TH1D.h"

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {

  TH1::SetDefaultSumw2();

  ProgramOptions options(argc, argv);

  double lumi = options.lumi;

  std::cout << "Integrated luminosity : " << lumi << " pb-1" << std::endl;

  // input datasets
  Datasets datasets(options.iDir);
  datasets.readFile(options.datasetFile);

  // output file
  TFile* ofile = TFile::Open( (options.oDir+std::string("/ZBackground.root")).c_str(), "UPDATE");

  // cuts
  Cuts cuts;
  TCut puWeight("puWeight");

  
  // estimate backgrounds in control regions from MC
  



  // estimate backgrounds to QCD in control regeions
  std::cout << "Estimating background to QCD control regions" << std::endl;
  std::cout << std::endl;
  std::cout << "Dataset  : loose dphi>2.6\tloose dphi<1.0\t\tloose dphi>2.6" << std::endl;

  // W->tau
  TCut wtau("wtauhadron>0");
  std::vector<double> wtauLooseHiDPhi, err_wtauLooseHiDPhi, wtauLooseLoDPhi, err_wtauLooseLoDPhi;
  std::vector<double> wtauTightHiDPhi, err_wtauTightHiDPhi;

  double nWtauLooseHiDPhi, err_nWtauLooseHiDPhi, nWtauLooseLoDPhi, err_nWtauLooseLoDPhi;
  double nWtauTightHiDPhi, err_nWtauTightHiDPhi;
  

  for (int i=1; i<5; ++i) {

    std::string name("W");
    name += boost::lexical_cast<std::string>(i);
    name += std::string("Jets");

    Dataset dataset = datasets.getDataset(name);
    TFile* file = datasets.getTFile(name);
    TTree* tree = (TTree*) file->Get("invHiggsInfo/InvHiggsInfo");

    double looseHiDPhi = (double) tree->Draw("", cuts.qcdLooseHiDPhi()+TCut("wtauhadron>0"));
    double looseLoDPhi = (double) tree->Draw("", cuts.qcdLooseLoDPhi()+TCut("wtauhadron>0"));
    double tightHiDPhi = (double) tree->Draw("", cuts.qcdTightHiDPhi()+TCut("wtauhadron>0"));

    wtauLooseHiDPhi.push_back( looseHiDPhi * dataset.sigma * lumi / dataset.nEvents );
    wtauLooseLoDPhi.push_back( looseLoDPhi * dataset.sigma * lumi / dataset.nEvents );
    wtauTightHiDPhi.push_back( tightHiDPhi * dataset.sigma * lumi / dataset.nEvents );
    
    err_wtauLooseHiDPhi.push_back( sqrt(looseHiDPhi) * dataset.sigma * lumi / dataset.nEvents );
    err_wtauLooseLoDPhi.push_back( sqrt(looseLoDPhi) * dataset.sigma * lumi / dataset.nEvents );
    err_wtauTightHiDPhi.push_back( sqrt(tightHiDPhi) * dataset.sigma * lumi / dataset.nEvents );    

    delete tree;
    file->Close();

  }

  nWtauLooseHiDPhi = wtauLooseHiDPhi.at(0) + wtauLooseHiDPhi.at(1) + wtauLooseHiDPhi.at(2) + wtauLooseHiDPhi.at(3);
  nWtauLooseLoDPhi = wtauLooseLoDPhi.at(0) + wtauLooseLoDPhi.at(1) + wtauLooseLoDPhi.at(2) + wtauLooseLoDPhi.at(3);
  nWtauTightHiDPhi = wtauTightHiDPhi.at(0) + wtauTightHiDPhi.at(1) + wtauTightHiDPhi.at(2) + wtauTightHiDPhi.at(3);

  err_nWtauLooseHiDPhi = sqrt(pow(err_wtauLooseHiDPhi.at(0),2) + pow(err_wtauLooseHiDPhi.at(1),2) + pow(err_wtauLooseHiDPhi.at(2),2) + pow(err_wtauLooseHiDPhi.at(3),2));
  err_nWtauLooseLoDPhi = sqrt(pow(err_wtauLooseLoDPhi.at(0),2) + pow(err_wtauLooseLoDPhi.at(1),2) + pow(err_wtauLooseLoDPhi.at(2),2) + pow(err_wtauLooseLoDPhi.at(3),2));
  err_nWtauTightHiDPhi = sqrt(pow(err_wtauTightHiDPhi.at(0),2) + pow(err_wtauTightHiDPhi.at(1),2) + pow(err_wtauTightHiDPhi.at(2),2) + pow(err_wtauTightHiDPhi.at(3),2));


  std::cout << "W->tau   : " << nWtauLooseHiDPhi << " +/- " << err_nWtauLooseHiDPhi << "\t"
	    << nWtauLooseLoDPhi << " +/- " << err_nWtauLooseLoDPhi << "\t"
	    << nWtauTightHiDPhi << " +/- " << err_nWtauTightHiDPhi << std::endl;

  // TTbar
  double nTTbarLooseHiDPhi, err_nTTbarLooseHiDPhi, nTTbarLooseLoDPhi, err_nTTbarLooseLoDPhi;
  double nTTbarTightHiDPhi, err_nTTbarTightHiDPhi;
  
  std::string name("TTbar");
  Dataset dataset = datasets.getDataset(name);
  TFile* file = datasets.getTFile(name);
  TTree* tree = (TTree*) file->Get("invHiggsInfo/InvHiggsInfo");

  double looseHiDPhi = (double) tree->Draw("", cuts.qcdLooseHiDPhi());
  double looseLoDPhi = (double) tree->Draw("", cuts.qcdLooseLoDPhi());
  double tightHiDPhi = (double) tree->Draw("", cuts.qcdTightHiDPhi());
  
  nTTbarLooseHiDPhi = looseHiDPhi * dataset.sigma * lumi / dataset.nEvents ;
  nTTbarLooseLoDPhi = looseLoDPhi * dataset.sigma * lumi / dataset.nEvents ;
  nTTbarTightHiDPhi = tightHiDPhi * dataset.sigma * lumi / dataset.nEvents ;
  
  err_nTTbarLooseHiDPhi = sqrt(looseHiDPhi) * dataset.sigma * lumi / dataset.nEvents ;
  err_nTTbarLooseLoDPhi = sqrt(looseLoDPhi) * dataset.sigma * lumi / dataset.nEvents ;
  err_nTTbarTightHiDPhi = sqrt(tightHiDPhi) * dataset.sigma * lumi / dataset.nEvents ;    

  delete tree;
  file->Close();

  std::cout << "TTbar    : " << nTTbarLooseHiDPhi << " +/- " << err_nTTbarLooseHiDPhi << "\t"
	    << nTTbarLooseLoDPhi << " +/- " << err_nTTbarLooseLoDPhi << "\t"
	    << nTTbarTightHiDPhi << " +/- " << err_nTTbarTightHiDPhi << std::endl;
  

  // DY
  double nDYLooseHiDPhi, err_nDYLooseHiDPhi, nDYLooseLoDPhi, err_nDYLooseLoDPhi;
  double nDYTightHiDPhi, err_nDYTightHiDPhi;
  
  name = std::string("DYJetsToLL");
  dataset = datasets.getDataset(name);
  file = datasets.getTFile(name);
  tree = (TTree*) file->Get("invHiggsInfo/InvHiggsInfo");

  looseHiDPhi = (double) tree->Draw("", cuts.qcdLooseHiDPhi());
  looseLoDPhi = (double) tree->Draw("", cuts.qcdLooseLoDPhi());
  tightHiDPhi = (double) tree->Draw("", cuts.qcdTightHiDPhi());
  
  nDYLooseHiDPhi = looseHiDPhi * dataset.sigma * lumi / dataset.nEvents ;
  nDYLooseLoDPhi = looseLoDPhi * dataset.sigma * lumi / dataset.nEvents ;
  nDYTightHiDPhi = tightHiDPhi * dataset.sigma * lumi / dataset.nEvents ;
  
  err_nDYLooseHiDPhi = sqrt(looseHiDPhi) * dataset.sigma * lumi / dataset.nEvents ;
  err_nDYLooseLoDPhi = sqrt(looseLoDPhi) * dataset.sigma * lumi / dataset.nEvents ;
  err_nDYTightHiDPhi = sqrt(tightHiDPhi) * dataset.sigma * lumi / dataset.nEvents ;    

  delete tree;
  file->Close();

  std::cout << "DY       : " << nDYLooseHiDPhi << " +/- " << err_nDYLooseHiDPhi << "\t"
	    << nDYLooseLoDPhi << " +/- " << err_nDYLooseLoDPhi << "\t"
	    << nDYTightHiDPhi << " +/- " << err_nDYTightHiDPhi << std::endl;

  // Diboson
  std::vector<double> dibosonLooseHiDPhi, err_dibosonLooseHiDPhi, dibosonLooseLoDPhi, err_dibosonLooseLoDPhi;
  std::vector<double> dibosonTightHiDPhi, err_dibosonTightHiDPhi;

  double nDibosonLooseHiDPhi, err_nDibosonLooseHiDPhi, nDibosonLooseLoDPhi, err_nDibosonLooseLoDPhi;
  double nDibosonTightHiDPhi, err_nDibosonTightHiDPhi;
  

  for (unsigned i=0; i<datasets.size(); ++i) {

    Dataset dataset = datasets.getDataset(i);

    if (dataset.name == "WW" || dataset.name == "WZ" || dataset.name == "ZZ" ) {

      TFile* file = datasets.getTFile(dataset.name);
      TTree* tree = (TTree*) file->Get("invHiggsInfo/InvHiggsInfo");
      
      double looseHiDPhi = (double) tree->Draw("", cuts.qcdLooseHiDPhi());
      double looseLoDPhi = (double) tree->Draw("", cuts.qcdLooseLoDPhi());
      double tightHiDPhi = (double) tree->Draw("", cuts.qcdTightHiDPhi());
      
      dibosonLooseHiDPhi.push_back( looseHiDPhi * dataset.sigma * lumi / dataset.nEvents );
      dibosonLooseLoDPhi.push_back( looseLoDPhi * dataset.sigma * lumi / dataset.nEvents );
      dibosonTightHiDPhi.push_back( tightHiDPhi * dataset.sigma * lumi / dataset.nEvents );
      
      err_dibosonLooseHiDPhi.push_back( sqrt(looseHiDPhi) * dataset.sigma * lumi / dataset.nEvents );
      err_dibosonLooseLoDPhi.push_back( sqrt(looseLoDPhi) * dataset.sigma * lumi / dataset.nEvents );
      err_dibosonTightHiDPhi.push_back( sqrt(tightHiDPhi) * dataset.sigma * lumi / dataset.nEvents );    
      
      delete tree;
      file->Close();

    }

  }

  nDibosonLooseHiDPhi = dibosonLooseHiDPhi.at(0) + dibosonLooseHiDPhi.at(1) + dibosonLooseHiDPhi.at(2);
  nDibosonLooseLoDPhi = dibosonLooseLoDPhi.at(0) + dibosonLooseLoDPhi.at(1) + dibosonLooseLoDPhi.at(2);
  nDibosonTightHiDPhi = dibosonTightHiDPhi.at(0) + dibosonTightHiDPhi.at(1) + dibosonTightHiDPhi.at(2);

  err_nDibosonLooseHiDPhi = sqrt(pow(err_dibosonLooseHiDPhi.at(0),2) + pow(err_dibosonLooseHiDPhi.at(1),2) + pow(err_dibosonLooseHiDPhi.at(2),2) );
  err_nDibosonLooseLoDPhi = sqrt(pow(err_dibosonLooseLoDPhi.at(0),2) + pow(err_dibosonLooseLoDPhi.at(1),2) + pow(err_dibosonLooseLoDPhi.at(2),2) );
  err_nDibosonTightHiDPhi = sqrt(pow(err_dibosonTightHiDPhi.at(0),2) + pow(err_dibosonTightHiDPhi.at(1),2) + pow(err_dibosonTightHiDPhi.at(2),2) );


  std::cout << "Diboson  : " << nDibosonLooseHiDPhi << " +/- " << err_nDibosonLooseHiDPhi << "\t"
	    << nDibosonLooseLoDPhi << " +/- " << err_nDibosonLooseLoDPhi << "\t"
	    << nDibosonTightHiDPhi << " +/- " << err_nDibosonTightHiDPhi << std::endl;


  // read Z background estimates from file
  double nZNuNuLooseHiDPhi(0.), err_nZNuNuLooseHiDPhi(0.), nZNuNuLooseLoDPhi(0.), err_nZNuNuLooseLoDPhi(0.);
  double nZNuNuTightHiDPhi(0.), err_nZNuNuTightHiDPhi(0.);

  ifstream ifile;
  double n, err_n;

  ifile.open((options.oDir+std::string("/zjets.txt")).c_str());
  while (ifile >> name >> n >> err_n) {
    if (name == "QCDLooseHiDPhi") {
      nZNuNuLooseHiDPhi = n;
      err_nZNuNuLooseHiDPhi = err_n;
    }
    if (name == "QCDLooseLoDPhi") {
      nZNuNuLooseLoDPhi = n;
      err_nZNuNuLooseLoDPhi = err_n;
    }
    if (name == "QCDTightHiDPhi") {
      nZNuNuTightHiDPhi = n;
      err_nZNuNuTightHiDPhi = err_n;
    }
  }
  ifile.close();

  std::cout << "ZNuNu    : " << nZNuNuLooseHiDPhi << " +/- " << err_nZNuNuLooseHiDPhi << "\t"
	    << nZNuNuLooseLoDPhi << " +/- " << err_nZNuNuLooseLoDPhi << "\t"
	    << nZNuNuTightHiDPhi << " +/- " << err_nZNuNuTightHiDPhi << std::endl;


  // read W background estimates from file
  double nWLNuLooseHiDPhi(0.), err_nWLNuLooseHiDPhi(0.), nWLNuLooseLoDPhi(0.), err_nWLNuLooseLoDPhi(0.);
  double nWLNuTightHiDPhi(0.), err_nWLNuTightHiDPhi(0.);

  ifile.open((options.oDir+std::string("/wjets.txt")).c_str());
  while (ifile >> name >> n >> err_n) {
    if (name == "QCDLooseHiDPhi") {
      nWLNuLooseHiDPhi = n;
      err_nWLNuLooseHiDPhi = err_n;
    }
    if (name == "QCDLooseLoDPhi") {
      nWLNuLooseLoDPhi = n;
      err_nWLNuLooseLoDPhi = err_n;
    }
    if (name == "QCDTightHiDPhi") {
      nWLNuTightHiDPhi = n;
      err_nWLNuTightHiDPhi = err_n;
    }
  }
  ifile.close();

  std::cout << "WLNu     : " << nWLNuLooseHiDPhi << " +/- " << err_nWLNuLooseHiDPhi << "\t"
	    << nWLNuLooseLoDPhi << " +/- " << err_nWLNuLooseLoDPhi << "\t"
	    << nWLNuTightHiDPhi << " +/- " << err_nWLNuTightHiDPhi << std::endl;


  // get the data
  std::cout << "Getting counts from data" << std::endl;

  file = datasets.getTFile("METABCD");
  tree = (TTree*) file->Get("invHiggsInfo/InvHiggsInfo");

  int nDataLooseHiDPhi = tree->Draw("", cuts.qcdLooseHiDPhi());
  int nDataLooseLoDPhi = tree->Draw("", cuts.qcdLooseLoDPhi());
  int nDataTightHiDPhi = tree->Draw("", cuts.qcdTightHiDPhi());

  double err_nDataLooseHiDPhi = sqrt(nDataLooseHiDPhi);
  double err_nDataLooseLoDPhi = sqrt(nDataLooseLoDPhi);
  double err_nDataTightHiDPhi = sqrt(nDataTightHiDPhi);

  std::cout << "Data     : " << nDataLooseHiDPhi << " +/- " << err_nDataLooseHiDPhi << "\t"
	    << nDataLooseLoDPhi << " +/- " << err_nDataLooseLoDPhi << "\t"
	    << nDataTightHiDPhi << " +/- " << err_nDataTightHiDPhi << std::endl;

  std::cout << std::endl;


  // estimate ratio
  std::cout << "Estimating ratio dphi<1.0 / dphi>2.6" << std::endl;

  double nEstLooseHiDPhi = double(nDataLooseHiDPhi) - nZNuNuLooseHiDPhi - nWLNuLooseHiDPhi - nWtauLooseHiDPhi - nTTbarLooseHiDPhi - nDYLooseHiDPhi - nDibosonLooseHiDPhi;
  double nEstLooseLoDPhi = double(nDataLooseLoDPhi) - nZNuNuLooseLoDPhi - nWLNuLooseLoDPhi - nWtauLooseLoDPhi - nTTbarLooseLoDPhi - nDYLooseLoDPhi - nDibosonLooseLoDPhi;

  double err_nEstLooseHiDPhi = sqrt( nDataLooseHiDPhi + pow(err_nZNuNuLooseHiDPhi,2) + pow(err_nWLNuLooseHiDPhi,2) + pow(err_nTTbarLooseHiDPhi,2) + pow(err_nDYLooseHiDPhi,2) + pow(err_nDibosonLooseHiDPhi,2) );
  double err_nEstLooseLoDPhi = sqrt( nDataLooseLoDPhi + pow(err_nZNuNuLooseLoDPhi,2) + pow(err_nWLNuLooseLoDPhi,2) + pow(err_nTTbarLooseLoDPhi,2) + pow(err_nDYLooseLoDPhi,2) + pow(err_nDibosonLooseLoDPhi,2) );

  double nEstTightHiDPhi = double(nDataTightHiDPhi) - nZNuNuTightHiDPhi - nWLNuTightHiDPhi - nWtauTightHiDPhi - nTTbarTightHiDPhi - nDYTightHiDPhi - nDibosonTightHiDPhi;
  double err_nEstTightHiDPhi = sqrt( nDataTightHiDPhi + pow(err_nZNuNuTightHiDPhi,2) + pow(err_nWLNuTightHiDPhi,2) + pow(err_nTTbarTightHiDPhi,2) + pow(err_nDYTightHiDPhi,2) + pow(err_nDibosonTightHiDPhi,2) );

  std::cout << "Est QCD  : " << nEstLooseHiDPhi << " +/- " << err_nEstLooseHiDPhi << "\t"
	    << nEstLooseLoDPhi << " +/- " << err_nEstLooseLoDPhi << "\t"
	    << nEstTightHiDPhi << " +/- " << err_nEstTightHiDPhi << std::endl;
  std::cout << std::endl;

  // compute ratio
  double ratio = nEstLooseLoDPhi / nEstLooseHiDPhi;
  double err_ratio = ratio*sqrt( pow(err_nEstLooseLoDPhi/nEstLooseLoDPhi,2) + pow(err_nEstLooseHiDPhi/nEstLooseHiDPhi,2) );

  std::cout << "Ratio N(dphi<1.0)/N(dphi>2.6) : " << ratio << " +/\- " << err_ratio << std::endl;
  std::cout << std::endl;

  // calculate QCD background estimate
  double nEstTightLoDPhi = nEstTightHiDPhi * ratio;
  double err_nEstTightLoDPhi = nEstTightLoDPhi * sqrt( pow(err_nEstTightHiDPhi/nEstTightHiDPhi, 2) + pow(err_ratio/ratio,2) );

  std::cout << "N expected (dphi < 1.0)       : " <<  nEstTightLoDPhi << " +/- " << err_nEstTightLoDPhi << std::endl;

  // write output file
  ofstream txtFile;
  txtFile.open(options.oDir+std::string("/qcd.txt"));
  txtFile << "Signal\t" << nEstTightLoDPhi << "\t" << err_nEstTightLoDPhi << std::endl;
  txtFile.close();

}
