
// this program reads backgrounds and writes a TeX files and card files for limit setting

#include "InvisibleHiggs/Analysis/interface/ProgramOptions.h"
#include "InvisibleHiggs/Analysis/interface/Cuts.h"
#include "InvisibleHiggs/Analysis/interface/Histogrammer.h"
#include "InvisibleHiggs/Analysis/interface/StackPlot.h"
#include "InvisibleHiggs/Analysis/interface/SumDatasets.h"
#include "InvisibleHiggs/Analysis/interface/Datasets.h"
#include "InvisibleHiggs/Analysis/interface/Constants.h"

#include "TTree.h"
#include "TMath.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TCanvas.h"


#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include <iostream>
#include <fstream>

bool lineariseEfficiency = false;

namespace po = boost::program_options;

//using namespace constants;


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
  int nCuts = cuts.nCuts();

  TFile* ofile = TFile::Open((oDir+std::string("/summary.root")).c_str(), "RECREATE");

  // get number of observed events
  std::cout << "Getting Z background" << std::endl;
  int nObs = 0;
  
  TFile* sigFile    = TFile::Open((oDir+std::string("/Signal/Search.root")).c_str(), "READ");
  if (sigFile!=0) {
    TH1D* hCounts = (TH1D*) sigFile->Get("hCounts");
    nObs          = hCounts->GetBinContent(1);
    sigFile->Close();
  }
  
  std::cout << "Observed events : " << nObs << std::endl;


  // get Z BG info
  std::cout << "Getting Z background" << std::endl;

  TFile* zFile    = TFile::Open((oDir+std::string("/ZBackground.root")).c_str(), "READ");
  if (zFile==0) {
    std::cerr << "Could not open " << oDir << "/ZBackground.root. Can't go any further" << std::endl;
    exit(1);
  }
  TH1D* hZ        = (TH1D*) zFile->Get("hZ_Est_S_DPhi");
  TH1D* hZ_Syst   = (TH1D*) zFile->Get("hZ_Est_S_DPhi_Syst");

  double nBG_Z       = hZ->GetBinContent(1);
  double stat_BG_Z   = hZ->GetBinError(1);
  double syst_BG_Z   = sqrt(pow(hZ_Syst->GetBinError(1),2)
			    +pow(nBG_Z*constants::syst_Z,2)); // for combine tool
  double syst_BG_Z_p = sqrt(pow(hZ_Syst->GetBinError(1),2)    // for summary table
			    +pow(nBG_Z*constants::syst_Z,2)
			    +pow(nBG_Z*constants::syst_Z_JES_p,2)
			    +pow(nBG_Z*constants::syst_Z_JER_p,2));
  double syst_BG_Z_m = sqrt(pow(hZ_Syst->GetBinError(1),2)
			    +pow(nBG_Z*constants::syst_Z,2)
			    +pow(nBG_Z*constants::syst_Z_JES_m,2)
			    +pow(nBG_Z*constants::syst_Z_JER_m,2)
			    +pow(nBG_Z*constants::syst_Z_UES_m,2));
  
  TH1D* zCtrl = (TH1D*) zFile->Get("hZ_Data_C_DPhi");
  int nCtrlZ = zCtrl->GetBinContent(1);    /// counts in ctrl region for limits
  
  zFile->Close();

  // get W BG info
  std::cout << "Getting W background" << std::endl;

  TFile* wFile    = TFile::Open((oDir+std::string("/WBackground.root")).c_str(), "READ");
  if (wFile==0) {
    std::cerr << "Could not open " << oDir << "/WBackground.root. Can't go any further" << std::endl;
    exit(1);
  }
  TH1D* hWMu      = (TH1D*) wFile->Get("hWMu_EstS_DPhi");
  TH1D* hWMu_Syst = (TH1D*) wFile->Get("hWMu_EstS_DPhi_Syst");
  TH1D* hWEl      = (TH1D*) wFile->Get("hWEl_EstS_DPhi");
  TH1D* hWEl_Syst = (TH1D*) wFile->Get("hWEl_EstS_DPhi_Syst");

  double nBG_WMu     = hWMu->GetBinContent(1);
  double stat_BG_WMu = hWMu->GetBinError(1);
  double syst_BG_WMu = sqrt(pow(hWMu_Syst->GetBinError(1),2)
			      +pow(nBG_WMu*constants::syst_WMu,2)); // for combine tool
  double syst_BG_WMu_p = sqrt(pow(hWMu_Syst->GetBinError(1),2)
			      +pow(nBG_WMu*constants::syst_WMu,2)
			      +pow(nBG_WMu*constants::syst_WMu_JER_p,2)
			      +pow(nBG_WMu*constants::syst_WMu_UES_m,2));
  double syst_BG_WMu_m = sqrt(pow(hWMu_Syst->GetBinError(1),2)
			      +pow(nBG_WMu*constants::syst_WMu,2)
			      +pow(nBG_WMu*constants::syst_WMu_JES_p,2)
			      +pow(nBG_WMu*constants::syst_WMu_JER_m,2));

  double nBG_WEl   = hWEl->GetBinContent(1);
  double stat_BG_WEl = hWEl->GetBinError(1);
  double syst_BG_WEl = sqrt(pow(hWEl_Syst->GetBinError(1),2) 
			      +pow(nBG_WEl*constants::syst_WEl,2)); // for combine tool
  double syst_BG_WEl_p = sqrt(pow(hWEl_Syst->GetBinError(1),2)
			      +pow(nBG_WEl*constants::syst_WEl,2)
			      +pow(nBG_WEl*constants::syst_WEl_JES_m,2)
			      +pow(nBG_WEl*constants::syst_WEl_JER_p,2)
			      +pow(nBG_WEl*constants::syst_WEl_UES_m,2));
  double syst_BG_WEl_m = sqrt(pow(hWEl_Syst->GetBinError(1),2)
			      +pow(nBG_WEl*constants::syst_WEl,2)
			      +pow(nBG_WEl*constants::syst_WEl_JES_p,2)
			      +pow(nBG_WEl*constants::syst_WEl_UES_p,2));
  
  wFile->Close();

  // W-tau BG
  std::cout << "Getting W->tau background with method " << options.wTauMethod << std::endl;

  double nBG_WTau(0.), stat_BG_WTau(0.), syst_BG_WTau(0.), syst_BG_WTau_p(0.), syst_BG_WTau_m(0.);

  if (options.wTauMethod==1) {
    nBG_WTau       = constants::nBG_WTau;
    stat_BG_WTau   = nBG_WTau*constants::stat_WTau;
    syst_BG_WTau_p = nBG_WTau*sqrt(pow(constants::stat_WTau,2)
				   +pow(constants::syst_WTau,2));
    syst_BG_WTau_m = syst_BG_WTau_p;
  }
  else {
    TFile* wTauFile    = TFile::Open((oDir+std::string("/WTauBackground.root")).c_str(), "READ");
    if (wTauFile==0) {
      std::cerr << "Could not open " << oDir << "/WTauBackground.root. Can't go any further" << std::endl;
      exit(1);
    }
    TH1D* hWTau      = (TH1D*) wTauFile->Get("hWTau_EstS_DPhi");
    TH1D* hWTau_Syst = (TH1D*) wTauFile->Get("hWTau_EstS_DPhi_Syst");
    
    nBG_WTau     = hWTau->GetBinContent(1);
    stat_BG_WTau = hWTau->GetBinError(1);
    syst_BG_WTau = sqrt(pow(hWTau_Syst->GetBinError(1),2)
			+pow(nBG_WTau*constants::syst_WTau,2)); // for combine tool
    syst_BG_WTau_p = sqrt(pow(hWTau_Syst->GetBinError(1),2)
			+pow(nBG_WTau*constants::syst_WTau,2)
			+pow(nBG_WTau*constants::syst_WTau_JES_m,2)
			+pow(nBG_WTau*constants::syst_WTau_JER_m,2)
			+pow(nBG_WTau*constants::syst_WTau_UES_p,2));
    syst_BG_WTau_m = sqrt(pow(hWTau_Syst->GetBinError(1),2)
			+pow(nBG_WTau*constants::syst_WTau,2)
			+pow(nBG_WTau*constants::syst_WTau_UES_m,2));
    wTauFile->Close();
  }


  // get QCD BG info
  double nBG_QCD(0.), errBG_QCD(0.), stat_BG_QCD(0.), syst_BG_QCD_p(0.), syst_BG_QCD_m(0.);
  std::cout << "Getting QCD background with method " << options.qcdMethod << std::endl;
  if (options.qcdMethod==1) {
    TFile* qFile = TFile::Open((oDir+std::string("/QCDBackground.root")).c_str(), "READ");
    if (qFile==0) {
      std::cerr << "Could not open " << oDir << "/QCDBackground.root. Can't go any further" << std::endl;
      exit(1);
    }
    TH1D* hQCD = (TH1D*) qFile->Get("hQCD_Est_S_DPhi");
    nBG_QCD   = hQCD->GetBinContent(1);
    stat_BG_QCD = hQCD->GetBinError(1);
    syst_BG_QCD_p = sqrt( pow(nBG_QCD*constants::syst_QCD1,2)
			 +pow(nBG_QCD*constants::syst_QCD1_JES_p,2)
			 +pow(nBG_QCD*constants::syst_QCD1_JER_p,2)
			 +pow(nBG_QCD*constants::syst_QCD1_UES_p,2));
    syst_BG_QCD_m = sqrt( pow(nBG_QCD*constants::syst_QCD1,2)
			 +pow(nBG_QCD*constants::syst_QCD1_JES_m,2)
			 +pow(nBG_QCD*constants::syst_QCD1_JER_m,2)
			 +pow(nBG_QCD*constants::syst_QCD1_UES_m,2));
    qFile->Close();
  }
  if (options.qcdMethod==2) {
//     qFile = TFile::Open((oDir+std::string("/QCDBackground.root")).c_str(), "READ");
//     hQCD = (TH1D*) qFile->Get("hQCD_Est_S_DPhi");
//     nBG_QCD   = hQCD->GetBinContent(1);
//     errBG_QCD = hQCD->GetBinError(1);  
  }
  else if (options.qcdMethod==3) {
    TFile* qFile = TFile::Open((oDir+std::string("/QCD3/QCD3.root")).c_str(), "READ");
    if (qFile==0) {
      std::cerr << "Could not open " << oDir << "/QCD3/QCD3.root. Can't go any further" << std::endl;
      exit(1);
    }
    TH2D* hQCD = (TH2D*) qFile->Get("hEst_METCJV");
    nBG_QCD     = hQCD->GetBinContent(2,1);
    stat_BG_QCD = hQCD->GetBinError(2,1);
    syst_BG_QCD_p = sqrt( pow(nBG_QCD*constants::syst_QCD3,2)
			 +pow(nBG_QCD*constants::syst_QCD3_JES_m,2)
			 +pow(nBG_QCD*constants::syst_QCD3_JER_m,2));
    syst_BG_QCD_m = sqrt( pow(nBG_QCD*constants::syst_QCD3,2)
			 +pow(nBG_QCD*constants::syst_QCD3_JES_p,2)
			 +pow(nBG_QCD*constants::syst_QCD3_JER_p,2)
			 +pow(nBG_QCD*constants::syst_QCD3_UES_m,2));
    qFile->Close();
  }

  // get MC background histograms
  std::cout << "Get MC backgrounds" << std::endl;
  TFile* mcFile = TFile::Open((oDir+std::string("/Efficiency.root")).c_str(), "READ");
  if (mcFile==0) {
    std::cerr << "Could not open " << oDir << "/Efficiency.root. Can't go any further" << std::endl;
    exit(1);
  }

  TH1D* hZNuNuMC = (TH1D*) mcFile->Get("hCutFlow_ZNuNu");
  double nBG_ZMC         = hZNuNuMC->GetBinContent(nCuts);
  double stat_BG_ZMC     = 0.;
  double syst_BG_ZMC     = hZNuNuMC->GetBinError(nCuts);

  TH1D* hWMuMC   = (TH1D*) mcFile->Get("hCutFlow_WMuNu");
  double nBG_WMuMC       = hWMuMC->GetBinContent(nCuts);
  double stat_BG_WMuMC   = 0.;
  double syst_BG_WMuMC   = hWMuMC->GetBinError(nCuts);

  TH1D* hWElMC   = (TH1D*) mcFile->Get("hCutFlow_WElNu");
  double nBG_WElMC       = hWElMC->GetBinContent(nCuts);
  double stat_BG_WElMC   = 0.;
  double syst_BG_WElMC   = hWElMC->GetBinError(nCuts);

  TH1D* hWTauMC  = (TH1D*) mcFile->Get("hCutFlow_WTauNu");
  double nBG_WTauMC      = hWTauMC->GetBinContent(nCuts);
  double stat_BG_WTauMC  = 0.;
  double syst_BG_WTauMC  = hWTauMC->GetBinError(nCuts);

  TH1D* hTTbar   = (TH1D*) mcFile->Get("hCutFlow_TTBar");
  double nBG_TTbar       = hTTbar->GetBinContent(nCuts);
  double stat_BG_TTbar   = 0.;
  double syst_BG_TTbar   = sqrt(pow(hTTbar->GetBinError(nCuts),2)
				  +pow(nBG_TTbar*constants::syst_TTbar,2)); // for combine tool
  double syst_BG_TTbar_p   = sqrt(pow(hTTbar->GetBinError(nCuts),2)
				  +pow(nBG_TTbar*constants::syst_lumi,2)
				  +pow(nBG_TTbar*constants::syst_TTbar,2));
  double syst_BG_TTbar_m   = sqrt(pow(hTTbar->GetBinError(nCuts),2)
				  +pow(nBG_TTbar*constants::syst_lumi,2)
				  +pow(nBG_TTbar*constants::syst_TTbar,2)
				  +pow(nBG_TTbar*constants::syst_TTbar_JES_m,2)
				  +pow(nBG_TTbar*constants::syst_TTbar_JER_m,2));

  TH1D* hSingleT = (TH1D*) mcFile->Get("hCutFlow_SingleTSum");
  double nBG_SingleT     = hSingleT->GetBinContent(nCuts);
  double stat_BG_SingleT = 0.;
  double syst_BG_SingleT = sqrt(pow(hSingleT->GetBinError(nCuts),2)
				  +pow(nBG_SingleT*constants::syst_SingleT,2)); // for combine tool
  double syst_BG_SingleT_p = sqrt(pow(hSingleT->GetBinError(nCuts),2)
				  +pow(nBG_SingleT*constants::syst_lumi,2)
				  +pow(nBG_SingleT*constants::syst_SingleT,2)
				  +pow(nBG_SingleT*constants::syst_SingleT_JES_p,2)
				  +pow(nBG_SingleT*constants::syst_SingleT_JER_p,2));
  double syst_BG_SingleT_m = sqrt(pow(hSingleT->GetBinError(nCuts),2)
				  +pow(nBG_SingleT*constants::syst_lumi,2)
				  +pow(nBG_SingleT*constants::syst_SingleT,2)
				  +pow(nBG_SingleT*constants::syst_SingleT_JES_m,2)
				  +pow(nBG_SingleT*constants::syst_SingleT_UES_m,2));


  TH1D* hDiboson = (TH1D*) mcFile->Get("hCutFlow_Diboson");
  double nBG_Diboson     = hDiboson->GetBinContent(nCuts);
  double stat_BG_Diboson = 0.;
  double syst_BG_Diboson = sqrt(pow(hDiboson->GetBinError(nCuts),2)
				  +pow(nBG_Diboson*constants::syst_Diboson,2)); // for combine tool
  double syst_BG_Diboson_p = sqrt(pow(hDiboson->GetBinError(nCuts),2)
				  +pow(nBG_Diboson*constants::syst_lumi,2)
				  +pow(nBG_Diboson*constants::syst_Diboson,2)
				  +pow(nBG_Diboson*constants::syst_Diboson_JES_p,2)
				  +pow(nBG_Diboson*constants::syst_Diboson_JER_p,2)
				  +pow(nBG_Diboson*constants::syst_Diboson_UES_p,2));
  double syst_BG_Diboson_m = sqrt(pow(hDiboson->GetBinError(nCuts),2)
				  +pow(nBG_Diboson*constants::syst_lumi,2)
				  +pow(nBG_Diboson*constants::syst_Diboson,2)
				  +pow(nBG_Diboson*constants::syst_Diboson_JES_m,2)
				  +pow(nBG_Diboson*constants::syst_Diboson_JER_m,2)
				  +pow(nBG_Diboson*constants::syst_Diboson_UES_m,2));
  
  TH1D* hDYLL    = (TH1D*) mcFile->Get("hCutFlow_DYLL");
  double nBG_DYLL        = hDYLL->GetBinContent(nCuts);
  double stat_BG_DYLL    = 0.;
  double syst_BG_DYLL    = sqrt(pow(hDYLL->GetBinError(nCuts),2)
				+pow(nBG_DYLL*constants::syst_DYLL,2)); // for combine tool
  double syst_BG_DYLL_p  = sqrt(pow(hDYLL->GetBinError(nCuts),2)
				+pow(nBG_DYLL*constants::syst_lumi,2)
				+pow(nBG_DYLL*constants::syst_DYLL,2)
				+pow(nBG_DYLL*constants::syst_DYLL_JES_p,2)
				+pow(nBG_DYLL*constants::syst_DYLL_JER_p,2));
  double syst_BG_DYLL_m  = sqrt(pow(hDYLL->GetBinError(nCuts),2)
				+pow(nBG_DYLL*constants::syst_lumi,2)
				+pow(nBG_DYLL*constants::syst_DYLL,2)
				+pow(nBG_DYLL*constants::syst_DYLL_JES_m,2)
				+pow(nBG_DYLL*constants::syst_DYLL_JER_m,2)
				+pow(nBG_DYLL*constants::syst_DYLL_UES_m,2));

  mcFile->Close();


  // calculate some numbers
  double nBG_Total = (nBG_Z +
		      nBG_WMu + 
		      nBG_WEl + 
		      nBG_WTau + 
		      nBG_QCD + 
		      nBG_TTbar + 
		      nBG_SingleT + 
		      nBG_Diboson + 
		      nBG_DYLL);

  double stat_BG_Total = sqrt( pow(stat_BG_Z,2) +
			       pow(stat_BG_WMu,2) +
			       pow(stat_BG_WEl,2) +
			       pow(stat_BG_WTau,2) +
			       pow(stat_BG_QCD,2) +
			       pow(stat_BG_TTbar,2) +
			       pow(stat_BG_SingleT,2) +
			       pow(stat_BG_Diboson,2) +
			       pow(stat_BG_DYLL,2));

  double syst_BG_Total_p = sqrt( pow(syst_BG_Z_p,2) +
				 pow(syst_BG_WMu_p,2) +
				 pow(syst_BG_WEl_p,2) +
				 pow(syst_BG_WTau_p,2) +
				 pow(syst_BG_QCD_p,2) +
				 pow(syst_BG_TTbar_p,2) +
				 pow(syst_BG_SingleT_p,2) +
				 pow(syst_BG_Diboson_p,2) +
				 pow(syst_BG_DYLL_p,2));

  double syst_BG_Total_m = sqrt( pow(syst_BG_Z_m,2) +
				 pow(syst_BG_WMu_m,2) +
				 pow(syst_BG_WEl_m,2) +
				 pow(syst_BG_WTau_m,2) +
				 pow(syst_BG_QCD_m,2) +
				 pow(syst_BG_TTbar_m,2) +
				 pow(syst_BG_SingleT_m,2) +
				 pow(syst_BG_Diboson_m,2) +
				 pow(syst_BG_DYLL_m,2));

  double nBG_Other = (nBG_TTbar + 
		      nBG_SingleT + 
		      nBG_Diboson +
		      nBG_DYLL);

  double stat_BG_Other =  sqrt( pow(stat_BG_TTbar,2) +
				pow(stat_BG_SingleT,2) + 
				pow(stat_BG_Diboson,2) +
				pow(stat_BG_DYLL,2));
  
  double syst_BG_Other_p =  sqrt( pow(syst_BG_TTbar_p,2) +
				  pow(syst_BG_SingleT_p,2) + 
				  pow(syst_BG_Diboson_p,2) +
				  pow(syst_BG_DYLL_p,2));

  double syst_BG_Other_m =  sqrt( pow(syst_BG_TTbar_m,2) +
				  pow(syst_BG_SingleT_m,2) + 
				  pow(syst_BG_Diboson_m,2) +
				  pow(syst_BG_DYLL_m,2));


  // write background summary table
  std::cout << "Writing BG summary TeX file" << std::endl;

  ofstream texFile;
  texFile.open(options.oDir+std::string("/summary.tex"));

  texFile << "Background \t & $N_{est} \\pm (stat) \\pm (syst)$ - data  \t & $N_{est} \\pm (syst)$ - MC \\\\" << std::endl;
  texFile << "\\hline";
  texFile << "$Z \\rightarrow \\nu\\nu$ \t & $" << nBG_Z << " \\pm " << stat_BG_Z << " \\pm " << syst_BG_Z_m << "/" << syst_BG_Z_p  << "$ \t &  $" << nBG_ZMC << " \\pm " << syst_BG_ZMC << "$ \\\\" << std::endl;
  texFile << "$W \\rightarrow e \\nu$  \t & $" << nBG_WEl << " \\pm " << stat_BG_WEl << " \\pm " << syst_BG_WEl_m << "/" << syst_BG_WEl_p  << "$ \t &  $" << nBG_WElMC << " \\pm " << syst_BG_WElMC << "$ \\\\" << std::endl;
  texFile << "$W \\rightarrow \\mu\\nu$ \t & $" << nBG_WMu << " \\pm " << stat_BG_WMu << " \\pm " << syst_BG_WMu_m << "/" << syst_BG_WMu_p  << "$ \t & $" << nBG_WMuMC << " \\pm " << syst_BG_WMuMC << "$ \\\\" << std::endl;
  texFile << "$W \\rightarrow \\tau \\nu$ \t & $" << nBG_WTau << " \\pm " << stat_BG_WTau << " \\pm " << syst_BG_WTau_m << "/" << syst_BG_WTau_p  << "$ \t & $" << nBG_WTauMC << " \\pm " << syst_BG_WTauMC << "$ \\\\" << std::endl;
  texFile << "QCD multijet \t & $" << nBG_QCD << " \\pm " << stat_BG_QCD << " \\pm " << syst_BG_QCD_m << "/" << syst_BG_QCD_p  << "$ \t & - \\\\" << std::endl;
  texFile << "$t\\bar{t}$ \t & -	\t & $" << nBG_TTbar << " \\pm " << syst_BG_TTbar_m << "/" << syst_BG_TTbar_p << "$ \\\\" << std::endl;
  texFile << "single t \t & -  \t & $" << nBG_SingleT << " \\pm " << syst_BG_SingleT_m << "/" << syst_BG_SingleT_p << "$ \\\\" << std::endl;
  texFile << "$VV$ \t & -  \t & $" << nBG_Diboson << " \\pm " << syst_BG_Diboson_m << "/" << syst_BG_Diboson_p << "$ \\\\" << std::endl;
  texFile << "DY \t & -  \t & $" << nBG_DYLL << " \\pm " << syst_BG_DYLL_m << "/" << syst_BG_DYLL_p << "$ \\\\" << std::endl;
  texFile << "Sum other BGs \t & -  \t & $" << nBG_Other << " \\pm " << syst_BG_Other_m << "/" << syst_BG_Other_p << "$ \\\\" << std::endl;
  texFile << "\\hline" << std::endl;
  texFile << "Total  & \\multicolumn{2}{c|}{$" << nBG_Total << "\\pm" << stat_BG_Total << "\\pm" << syst_BG_Total_m << "/" << syst_BG_Total_p << "$}  \\\\" << std::endl;
  texFile << "Observed & \\multicolumn{2}{c|}{$" << nObs << "$}  \\\\" << std::endl;

  texFile.close();

//   // write limit card file
//   std::cout << "Writing limit card file" << std::endl;

   ofstream txtFile;
//   txtFile.open(options.oDir+std::string("/summary.txt"));

//   txtFile << "Background \t  N \t err" << std::endl;
//   txtFile << "Z\t" <<  nBG_Z << "\t" << errBG_Z << std::endl;
//   txtFile << "W\t" <<  nBG_W << "\t" << errBG_W << std::endl;
//   txtFile << "QCD\t" <<  nBG_QCD << "\t" << errBG_QCD << std::endl;
//   txtFile << "Other\t" <<  nBG_Other << "\t" << errBG_Other << std::endl;
//   txtFile << "Observed\t" <<  nObs << std::endl;
//   txtFile << std::endl;

//   txtFile.close();


  // linearise signal efficiency and recompute yields...
  std::vector<double> effSig(constants::nM);
  std::vector<double> errEffSig(constants::nM);
  std::vector<double> nSig(constants::nM);
  std::vector<double> errNSig(constants::nM);

  for (int i=0; i<constants::nM; ++i) {

    std::stringstream ss;
    ss << (int) constants::mH[i];
    
    // signal yield
    TFile* sigFile = TFile::Open((oDir+std::string("/Efficiency.root")).c_str(), "READ");
    if (sigFile==0) {
      std::cerr << "Could not open " << oDir << "/Efficiency.root. Can't go any further" << std::endl;
      exit(1);
    }
    std::string hstring = std::string("hCutFlow_SignalM")+ss.str()+std::string("_POWHEG");
    TH1D* sigCutFlow = (TH1D*) sigFile->Get(hstring.c_str());
    nSig.at(i)      = sigCutFlow->GetBinContent(nCuts);
    errNSig.at(i)   = sigCutFlow->GetBinError(nCuts);
    effSig.at(i)    = sigCutFlow->GetBinContent(nCuts)/(constants::xsH[i]*lumi);
    errEffSig.at(i) = sigCutFlow->GetBinError(nCuts)/(constants::xsH[i]*lumi);

    std::cout << "mH : " << constants::mH[i] << "  eff : " << effSig.at(i) << "+/-" << errEffSig.at(i) << "  yield=" << nSig.at(i) << "+/-" << errNSig.at(i) <<std::endl;
    sigFile->Close();

  }

  TGraphErrors gSigEff(constants::nM, &constants::mH[0], &effSig[0], 0, &errEffSig[0]);
  TF1 f1("f1", "[0]*x + [1]", constants::mH[0], constants::mH[constants::nM]);
  gSigEff.Fit(&f1);

  double m = f1.GetParameter(0);
  double c = f1.GetParameter(1);

  std::cout << "Linearising signal efficiency" << std::endl;
  std::cout << "Fit results : m=" << m << ", c=" << c << std::endl;
  std::cout << std::endl;


  TCanvas canvas;

  gSigEff.SetMarkerColor(kBlue);
  gSigEff.SetMarkerStyle(23);
  gSigEff.GetYaxis()->SetTitle("Efficiency");
  gSigEff.GetYaxis()->SetTitle("m_{H} [GeV]");
  gSigEff.Draw("AP");
  
  std::string pdfName = options.oDir+std::string("/SignalEfficiency.pdf");
  canvas.Print(pdfName.c_str());
  
  if (lineariseEfficiency) {
    for (int i=0; i<constants::nM; ++i) {
      
      effSig.at(i) = m * constants::mH[i] + c;
      nSig.at(i)   = constants::xsH[i] * lumi * effSig[i];

      std::cout << "mH : " << constants::mH[i] << "  eff : " << effSig.at(i) << "+/-" << errEffSig.at(i) << "  yield=" << nSig.at(i) << "+/-" << errNSig.at(i) << std::endl;
    
    }
  }

  // calculate factors for combine tool
  double fLum  = 1 + constants::syst_lumi;

  double fZ     = 1 + syst_BG_Z/nBG_Z;
  double fZJESp = 1 + constants::syst_Z_JES_p;
  double fZJESm = 1 + constants::syst_Z_JES_m;
  double fZJERp = 1 + constants::syst_Z_JER_p;
  double fZJERm = 1 + constants::syst_Z_JER_m;
  double fZUESp = 1 + constants::syst_Z_UES_p;
  double fZUESm = 1 + constants::syst_Z_UES_m;

  double fWMu     = 1 + sqrt(pow(syst_BG_WMu/nBG_WMu,2) + pow(stat_BG_WMu/nBG_WMu,2));
  double fWMuJESp = 1 + constants::syst_WMu_JES_p;
  double fWMuJESm = 1 + constants::syst_WMu_JES_m;
  double fWMuJERp = 1 + constants::syst_WMu_JER_p;
  double fWMuJERm = 1 + constants::syst_WMu_JER_m;
  double fWMuUESp = 1 + constants::syst_WMu_UES_p;
  double fWMuUESm = 1 + constants::syst_WMu_UES_m;

  double fWEl     = 1 + sqrt(pow(syst_BG_WEl/nBG_WEl,2) + pow(stat_BG_WEl/nBG_WEl,2));
  double fWElJESp = 1 + constants::syst_WEl_JES_p;
  double fWElJESm = 1 + constants::syst_WEl_JES_m;
  double fWElJERp = 1 + constants::syst_WEl_JER_p;
  double fWElJERm = 1 + constants::syst_WEl_JER_m;
  double fWElUESp = 1 + constants::syst_WEl_UES_p;
  double fWElUESm = 1 + constants::syst_WEl_UES_m;

  double fWTau     = 1 + sqrt(pow(syst_BG_WTau/nBG_WTau,2) + pow(stat_BG_WTau/nBG_WTau,2));
  double fWTauJESp = 1 + constants::syst_WTau_JES_p;
  double fWTauJESm = 1 + constants::syst_WTau_JES_m;
  double fWTauJERp = 1 + constants::syst_WTau_JER_p;
  double fWTauJERm = 1 + constants::syst_WTau_JER_m;
  double fWTauUESp = 1 + constants::syst_WTau_UES_p;
  double fWTauUESm = 1 + constants::syst_WTau_UES_m;

  double fQCD     = 1 + sqrt(pow(constants::syst_QCD3,2) + pow(stat_BG_QCD/nBG_QCD,2));
  double fQCDJESp = 1 + constants::syst_QCD3_JES_p;
  double fQCDJESm = 1 + constants::syst_QCD3_JES_m;
  double fQCDJERp = 1 + constants::syst_QCD3_JER_p;
  double fQCDJERm = 1 + constants::syst_QCD3_JER_m;
  double fQCDUESp = 1 + constants::syst_QCD3_UES_p;
  double fQCDUESm = 1 + constants::syst_QCD3_UES_m;
 
  double fOther     = 1 + (sqrt(pow(syst_BG_TTbar,2) + pow(syst_BG_SingleT,2) + pow(syst_BG_Diboson,2) + pow(syst_BG_DYLL,2))/nBG_Other);
  double fOtherJESp = 1 + constants::syst_Other_JES_p;
  double fOtherJESm = 1 + constants::syst_Other_JES_m;
  double fOtherJERp = 1 + constants::syst_Other_JER_p;
  double fOtherJERm = 1 + constants::syst_Other_JER_m;
  double fOtherUESp = 1 + constants::syst_Other_UES_p;
  double fOtherUESm = 1 + constants::syst_Other_UES_m;

  double aZ    = nBG_Z / nCtrlZ;

  // write combine tool card file
  std::cout << "Writing combine tool card files" << std::endl;

  for (int i=0; i<constants::nM; ++i) {

    double fSigPDF   = 1 + constants::syst_Sig_PDF[i];
    double fSigQCD   = 1 + constants::syst_Sig_QCD[i];
    double fSigJESp  = 1 + constants::syst_Sig_JES_p[i];
    double fSigJESm  = 1 + constants::syst_Sig_JES_m[i];
    double fSigJERp  = 1 + constants::syst_Sig_JER_p[i];
    double fSigJERm  = 1 + constants::syst_Sig_JER_m[i];
    double fSigUESp  = 1 + constants::syst_Sig_UES_p[i];
    double fSigUESm  = 1 + constants::syst_Sig_UES_m[i];

    std::stringstream ss;
    ss << (int) constants::mH[i];

    std::string filename = options.oDir+std::string("/card")+ss.str()+std::string(".txt");
    txtFile.open(filename);
    txtFile << "# Invisible Higgs analysis for mH=" << constants::mH[i] << " GeV" << std::endl;
    txtFile << "imax 1" << std::endl;
    txtFile << "jmax 6  number of backgrounds" << std::endl;
    txtFile << "kmax 13  number of nuisance parameters (sources of systematical uncertainties)" << std::endl;
    txtFile << "------------" << std::endl;
    txtFile << "# we have just one channel, in which we observe 0 events" << std::endl;
    txtFile << "bin 1" << std::endl;
    txtFile << "observation " << nObs << std::endl;
    txtFile << "------------" << std::endl;
    txtFile << "bin               1  \t 1   \t 1   \t 1  \t 1    \t 1   \t 1     " << std::endl;
    txtFile << "process          qqH \t zvv \t wmu \t wel \t wtau \t qcd \t others" << std::endl;
    txtFile << "process           0  \t 1   \t 2   \t 3  \t 4    \t 5   \t 6     " << std::endl;
    txtFile << "rate            " << nSig.at(i) << "\t" << nBG_Z << "\t" << nBG_WMu << "\t" << nBG_WEl << "\t" << nBG_WTau << "\t" << nBG_QCD << "\t" << nBG_Other << std::endl;
    txtFile << "------------" << std::endl;
    txtFile << "CMS_lumi      lnN   " << fLum << "\t - \t - \t - \t - \t - \t" << fLum << std::endl;
    txtFile << "CMS_scale_j   lnN   " << fSigJESm << "/" << fSigJESp << "\t" << fZJESm << "/" << fZJESp << "\t" << fWMuJESm << "/" << fWMuJESp << "\t" << fWElJESm << "/" << fWElJESp << "\t" << fWTauJESm << "/" << fWTauJESp << "\t" << fQCDJESm << "/" << fQCDJESp << "\t" << fOtherJESm << "/" << fOtherJESp << std::endl;
    txtFile << "CMS_res_j     lnN   " << fSigJERm << "/" << fSigJERp << "\t" << fZJERm << "/" << fZJERp << "\t" << fWMuJERm << "/" << fWMuJERp << "\t" << fWElJERm << "/" << fWElJERp << "\t" << fWTauJERm << "/" << fWTauJERp << "\t" << fQCDJERm << "/" << fQCDJERp << "\t" << fOtherJERm << "/" << fOtherJERp << std::endl;
    txtFile << "CMS_scale_met lnN   " << fSigUESm << "/" << fSigUESp << "\t" << fZUESm << "/" << fZUESp << "\t" << fWMuUESm << "/" << fWMuUESp << "\t" << fWElUESm << "/" << fWElUESp << "\t" << fWTauUESm << "/" << fWTauUESp << "\t" << fQCDUESm << "/" << fQCDUESp << "\t" << fOtherUESm << "/" << fOtherUESp << std::endl;
    txtFile << "pdf_qqbar     lnN   " << fSigPDF  << "\t - \t - \t - \t - \t - \t - \t" << std::endl;
    txtFile << "QCD_qqH       lnN   " << fSigQCD  << "\t - \t - \t - \t - \t - \t - \t" << std::endl;
    txtFile << "CMS_VBFHinv_zvv_stat      gmN " << nCtrlZ << "  -\t" << aZ << "\t - \t - \t - \t - \t - " << std::endl;
    txtFile << "CMS_VBFHinv_zvv_norm      lnN     - \t" << fZ << "\t - \t - \t - \t - \t - " << std::endl;
    txtFile << "CMS_VBFHinv_wmu_norm      lnN     - \t - \t" << fWMu << "\t - \t - \t - \t - " << std::endl;
    txtFile << "CMS_VBFHinv_wel_norm      lnN     - \t - \t - \t" << fWEl << "\t - \t - \t - " << std::endl;
    txtFile << "CMS_VBFHinv_wtau_norm     lnN     - \t - \t - \t - \t" << fWTau << "\t - \t - " << std::endl;
    txtFile << "CMS_VBFHinv_qcd_norm      lnN     - \t - \t - \t - \t - \t" << fQCD << "\t - " << std::endl;
    txtFile << "CMS_VBFHinv_other_norm    lnN     - \t - \t - \t - \t - \t - \t" << fOther << std::endl;
    txtFile.close();

  }


  // clean up
  ofile->Close();

}
