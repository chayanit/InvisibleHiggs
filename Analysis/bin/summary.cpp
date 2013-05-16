
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

namespace po = boost::program_options;

int main(int argc, char* argv[]) {

  // setup
  TH1::SetDefaultSumw2();

  ProgramOptions options(argc, argv);

  double lumi = options.lumi;
  std::string iDir = options.iDir;
  std::string oDir = options.oDir;

  //  Datasets datasets(iDir);
  //  datasets.readFile(options.datasetFile);

  Cuts cuts;

  TFile* ofile = TFile::Open((oDir+std::string("/example.root")).c_str(), "RECREATE");

  // now do something

  // get data driven background info
  std::cout << "Get data driven backgrounds" << std::endl;
  TFile* zFile = TFile::Open((oDir+std::string("/ZBackground.root")).c_str(), "READ");
  TFile* wFile = TFile::Open((oDir+std::string("/WBackground.root")).c_str(), "READ");
  TFile* qFile = TFile::Open((oDir+std::string("/QCDBackground.root")).c_str(), "READ");
  TH1D* hZ   = (TH1D*) zFile->Get("hZ_Est_S_DPhi");
  TH1D* hWMu = (TH1D*) wFile->Get("hWMu_EstS_DPhi");
  TH1D* hWEl = (TH1D*) wFile->Get("hWEl_EstS_DPhi");
  TH1D* hQCD = (TH1D*) qFile->Get("hQCD_Est_S_DPhi");
  double nBG_Z     = hZ->GetBinContent(1);
  double errBG_Z   = hZ->GetBinError(1);
  double nBG_WMu   = hWMu->GetBinContent(1);
  double errBG_WMu = hWMu->GetBinError(1);
  double nBG_WEl   = hWEl->GetBinContent(1);
  double errBG_WEl = hWEl->GetBinError(1);
  double nBG_QCD   = hQCD->GetBinContent(1);
  double errBG_QCD = hQCD->GetBinError(1);

  // get MC background histograms
  std::cout << "Get MC backgrounds" << std::endl;
  TFile* mcFile = TFile::Open((oDir+std::string("/Efficiency.root")).c_str(), "READ");
  TH1D* hTTBar   = (TH1D*) mcFile->Get("hCutFlow_TTBar");
  TH1D* hZNuNuMC = (TH1D*) mcFile->Get("hCutFlow_ZNuNu");
  TH1D* hWMuMC   = (TH1D*) mcFile->Get("hCutFlow_WMuNu");
  TH1D* hWElMC   = (TH1D*) mcFile->Get("hCutFlow_WElNu");
  TH1D* hWTauHad = (TH1D*) mcFile->Get("hCutFlow_WTauNu");
  TH1D* hSingleT = (TH1D*) mcFile->Get("hCutFlow_SingleTSum");
  TH1D* hDiboson = (TH1D*) mcFile->Get("hCutFlow_Diboson");
  double nBG_TTBar     = hTTBar->GetBinContent(10);
  double errBG_TTBar   = hTTBar->GetBinError(10);
  double nBG_ZMC       = hZNuNuMC->GetBinContent(10);
  double errBG_ZMC     = hZNuNuMC->GetBinError(10);
  double nBG_WMuMC     = hWMuMC->GetBinContent(10);
  double errBG_WMuMC   = hWMuMC->GetBinError(10);
  double nBG_WElMC     = hWElMC->GetBinContent(10);
  double errBG_WElMC   = hWElMC->GetBinError(10);
  double nBG_WTauHad   = hWTauHad->GetBinContent(10);
  double errBG_WTauHad = hWTauHad->GetBinError(10);
  double nBG_SingleT   = hSingleT->GetBinContent(10);
  double errBG_SingleT = hSingleT->GetBinError(10);
  double nBG_Diboson   = hDiboson->GetBinContent(10);
  double errBG_Diboson = hDiboson->GetBinError(10);

  // calculate some numbers
  double nBG_Total = (nBG_Z +
		      nBG_WMu + 
		      nBG_WEl + 
		      nBG_QCD + 
		      nBG_TTBar + 
		      nBG_WTauHad + 
		      nBG_SingleT + 
		      nBG_Diboson);

  double errBG_Total = sqrt( pow(errBG_Z,2) +
			     pow(errBG_WMu,2) +
			     pow(errBG_WEl,2) +
			     pow(errBG_QCD,2) +
			     pow(errBG_WTauHad,2) +
			     pow(errBG_TTBar,2) +
			     pow(errBG_SingleT,2) +
			     pow(errBG_Diboson,2));

  double nBG_W = (nBG_WMu + 
		  nBG_WEl);

  double errBG_W = sqrt( pow(errBG_WMu,2) + 
			 pow(errBG_WEl,2));

  double nBG_Other = (nBG_TTBar + 
		      nBG_WTauHad + 
		      nBG_SingleT + 
		      nBG_Diboson);

  double errBG_Other =  sqrt( pow(errBG_TTBar,2) +
			      pow(errBG_WTauHad,2) + 
			      pow(errBG_SingleT,2) + 
			      pow(errBG_Diboson,2));
  

  // write background summary table
  std::cout << "Writing BG summary TeX file" << std::endl;

  ofstream texFile;
  texFile.open(options.oDir+std::string("/summary.tex"));

  texFile << "Background \t & $N_{est}$ (data) \t & $N_{est}$ (MC) \\\\" << std::endl;
  texFile << "\\hline";
  texFile << "$Z \\rightarrow \\nu\\nu$ \t & $" << nBG_Z << " \\pm " << errBG_Z << "$ \t &  $" << nBG_ZMC << " \\pm " << errBG_ZMC << "$ \\\\" << std::endl;
  texFile << "$W \\rightarrow \\mu\\nu$ \t & $" << nBG_WMu << " \\pm " << errBG_WMu << "$ \t & $" << nBG_WMuMC << " \\pm " << errBG_WMuMC << "$ \\\\" << std::endl;
  texFile << "$W \\rightarrow e \\nu$  \t & $" << nBG_WEl << " \\pm " << errBG_WEl << "$ \t &  $" << nBG_WElMC << " \\pm " << errBG_WElMC << "$ \\\\" << std::endl;
  texFile << "$W \\rightarrow \\tau \\nu$ \t& - & $" << nBG_WTauHad << " \\pm " << errBG_WTauHad << "$ \\\\" << std::endl;
  texFile << "QCD multijet \t & $" << nBG_QCD << " \\pm " << errBG_QCD << "$ \t & - \\\\" << std::endl;
  texFile << "$t\\bar{t}$ \t & -	\t & $" << nBG_TTBar << " \\pm << " << errBG_TTBar << "$ \\\\" << std::endl;
  texFile << "single t \t & -  \t & $" << nBG_SingleT << " \\pm " << errBG_SingleT << "$ \\\\" << std::endl;
  texFile << "$VV$ \t & -  \t & $" << nBG_Diboson << " \\pm " << errBG_Diboson<< "$ \\\\" << std::endl;
  texFile << "\\hline" << std::endl;
  texFile << "Total  & \\multicolumn{2}{c|}{$" << nBG_Total << " << \\pm " << errBG_Total << "$}  \\\\" << std::endl;

  texFile.close();

  // write limit card file
  std::cout << "Writing limit card file" << std::endl;

  ofstream txtFile;
  txtFile.open(options.oDir+std::string("/summary.txt"));

  txtFile << "Background \t  N \t err" << std::endl;
  txtFile << "Z\t" <<  nBG_Z << "\t" << errBG_Z << std::endl;
  txtFile << "W\t" <<  nBG_W << "\t" << errBG_W << std::endl;
  txtFile << "QCD\t" <<  nBG_QCD << "\t" << errBG_QCD << std::endl;
  txtFile << "Other\t" <<  nBG_Other << "\t" << errBG_Other << std::endl;
  txtFile << std::endl;

  txtFile.close();


  // write combine tool card file
  std::cout << "Writing combine tool card file" << std::endl;

  txtFile.open(options.oDir+std::string("/combine.txt"));

  txtFile.close();


  // clean up
  zFile->Close();
  wFile->Close();
  qFile->Close();
  mcFile->Close();

  ofile->Close();

}
