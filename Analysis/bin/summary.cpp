
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
			    +pow(nBG_Z*constants::syst_Z_Mu_m,2)
			    //+pow(nBG_Z*constants::syst_Z_JES_p,2)
			    +pow(nBG_Z*constants::syst_Z_JER_p,2));
  double syst_BG_Z_m = sqrt(pow(hZ_Syst->GetBinError(1),2)
			    +pow(nBG_Z*constants::syst_Z,2)
			    +pow(nBG_Z*constants::syst_Z_Mu_p,2)
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
			      +pow(nBG_WMu*constants::syst_WMu_Mu_m,2)
                              +pow(nBG_WMu*constants::syst_WMu_JES_m,2)
			      +pow(nBG_WMu*constants::syst_WMu_JER_p,2));
  double syst_BG_WMu_m = sqrt(pow(hWMu_Syst->GetBinError(1),2)
			      +pow(nBG_WMu*constants::syst_WMu,2)
                              +pow(nBG_WMu*constants::syst_WMu_Mu_p,2)
			      +pow(nBG_WMu*constants::syst_WMu_JES_p,2)
			      +pow(nBG_WMu*constants::syst_WMu_JER_m,2)
                              +pow(nBG_WMu*constants::syst_WMu_UES_p,2));

  double nBG_WEl   = hWEl->GetBinContent(1);
  double stat_BG_WEl = hWEl->GetBinError(1);
  double syst_BG_WEl = sqrt(pow(hWEl_Syst->GetBinError(1),2) 
			      +pow(nBG_WEl*constants::syst_WEl,2)); // for combine tool
  double syst_BG_WEl_p = sqrt(pow(hWEl_Syst->GetBinError(1),2)
			      +pow(nBG_WEl*constants::syst_WEl,2)
			      +pow(nBG_WEl*constants::syst_WEl_El_m,2)
			      +pow(nBG_WEl*constants::syst_WEl_JES_m,2)
			      +pow(nBG_WEl*constants::syst_WEl_JER_m,2)
			      +pow(nBG_WEl*constants::syst_WEl_UES_m,2));
  double syst_BG_WEl_m = sqrt(pow(hWEl_Syst->GetBinError(1),2)
			      +pow(nBG_WEl*constants::syst_WEl,2)
                              +pow(nBG_WEl*constants::syst_WEl_El_p,2)
			      +pow(nBG_WEl*constants::syst_WEl_JES_p,2)
                              +pow(nBG_WEl*constants::syst_WEl_JER_p,2)
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
                        +pow(nBG_WTau*constants::syst_WTau_El_p,2)
                        +pow(nBG_WTau*constants::syst_WTau_Mu_m,2)
			+pow(nBG_WTau*constants::syst_WTau_JES_m,2)
			+pow(nBG_WTau*constants::syst_WTau_UES_m,2));
    syst_BG_WTau_m = sqrt(pow(hWTau_Syst->GetBinError(1),2)
			+pow(nBG_WTau*constants::syst_WTau,2)
                        +pow(nBG_WTau*constants::syst_WTau_El_m,2)
                        +pow(nBG_WTau*constants::syst_WTau_Mu_p,2)
			+pow(nBG_WTau*constants::syst_WTau_JER_p,2));
    wTauFile->Close();
  }


  // get QCD BG info
  double nBG_QCD(0.), errBG_QCD(0.), stat_BG_QCD(0.), stat_MC_QCD(0.), syst_BG_QCD(0.), syst_BG_QCD_p(0.), syst_BG_QCD_m(0.);
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
    TH2D* hQCD 		= (TH2D*) qFile->Get("hEst_METCJV");		// for datacards
    TH2D* hQCD_Stat	= (TH2D*) qFile->Get("hEst_METCJV_Stat");	// only data stat
    TH2D* hQCD_Syst 	= (TH2D*) qFile->Get("hEst_METCJV_Syst");	// only MC stat
    nBG_QCD     = hQCD->GetBinContent(2,1);
    stat_BG_QCD = hQCD->GetBinError(2,1);
    stat_MC_QCD = hQCD_Stat->GetBinError(2,1);
    syst_BG_QCD_p = sqrt(pow(hQCD_Syst->GetBinError(2,1),2) 
			 +pow(nBG_QCD*constants::syst_QCD3,2)
                         +pow(nBG_QCD*constants::syst_QCD3_El_p,2)
			 +pow(nBG_QCD*constants::syst_QCD3_JES_m,2)
			 +pow(nBG_QCD*constants::syst_QCD3_JER_m,2)
                         +pow(nBG_QCD*constants::syst_QCD3_UES_m,2));
    syst_BG_QCD_m = sqrt(pow(hQCD_Syst->GetBinError(2,1),2) 
			 +pow(nBG_QCD*constants::syst_QCD3,2)
                         +pow(nBG_QCD*constants::syst_QCD3_El_m,2)
                         +pow(nBG_QCD*constants::syst_QCD3_Mu_m,2)
			 +pow(nBG_QCD*constants::syst_QCD3_JES_p,2)
			 +pow(nBG_QCD*constants::syst_QCD3_JER_p,2)
			 +pow(nBG_QCD*constants::syst_QCD3_UES_p,2));
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
				  +pow(nBG_TTbar*constants::syst_TTbar,2)
                                  +pow(nBG_TTbar*constants::syst_TTbar_JES_p,2)
                                  +pow(nBG_TTbar*constants::syst_TTbar_JER_p,2));
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
                                  +pow(nBG_SingleT*constants::syst_SingleT_El_m,2)
				  +pow(nBG_SingleT*constants::syst_SingleT_JES_p,2)
				  +pow(nBG_SingleT*constants::syst_SingleT_JER_m,2));
  double syst_BG_SingleT_m = sqrt(pow(hSingleT->GetBinError(nCuts),2)
				  +pow(nBG_SingleT*constants::syst_lumi,2)
				  +pow(nBG_SingleT*constants::syst_SingleT,2)
                                  +pow(nBG_SingleT*constants::syst_SingleT_El_p,2)
				  +pow(nBG_SingleT*constants::syst_SingleT_JES_m,2)
				  +pow(nBG_SingleT*constants::syst_SingleT_UES_m,2));


  TH1D* hDiboson 	= (TH1D*) mcFile->Get("hCutFlow_Diboson"); 		//with WG
  TH1D* hDibosonNoWG 	= (TH1D*) mcFile->Get("hCutFlow_DibosonNoWG");		//remove WG
  TH1D* hWG      	= (TH1D*) mcFile->Get("hCutFlow_WG");			//WG weighted
  TH1D* hWG_unweighted 	= (TH1D*) mcFile->Get("hCutFlow_unweighted_WG"); 	//WG unweighted
  double nBG_Diboson     = hDiboson->GetBinContent(nCuts);
  double nBG_DibosonNoWG = hDibosonNoWG->GetBinContent(nCuts);		// for data cards
  double nBG_WG		 = hWG->GetBinContent(nCuts);			// for data cards
  int nBG_WG_unweighted	 = hWG_unweighted->GetBinContent(nCuts);	// for data cards
  double stat_BG_Diboson = 0.;
  double syst_BG_Diboson = sqrt(pow(hDibosonNoWG->GetBinError(nCuts),2)
				  +pow(nBG_DibosonNoWG*constants::syst_DibosonNoWG,2)); // for combine tool
  double syst_BG_Diboson_p = sqrt(pow(hDiboson->GetBinError(nCuts),2)
				  +pow(nBG_Diboson*constants::syst_lumi,2)
				  +pow(nBG_Diboson*constants::syst_Diboson,2)
                                  +pow(nBG_Diboson*constants::syst_Diboson_El_m,2)
                                  +pow(nBG_Diboson*constants::syst_Diboson_Mu_m,2)
				  +pow(nBG_Diboson*constants::syst_Diboson_UES_p,2));
  double syst_BG_Diboson_m = sqrt(pow(hDiboson->GetBinError(nCuts),2)
				  +pow(nBG_Diboson*constants::syst_lumi,2)
				  +pow(nBG_Diboson*constants::syst_Diboson,2)
                                  +pow(nBG_Diboson*constants::syst_Diboson_El_p,2)
                                  +pow(nBG_Diboson*constants::syst_Diboson_Mu_p,2)
				  +pow(nBG_Diboson*constants::syst_Diboson_JES_m,2)
				  +pow(nBG_Diboson*constants::syst_Diboson_JER_p,2)
				  +pow(nBG_Diboson*constants::syst_Diboson_UES_m,2));
  
  TH1D* hDYLL    = (TH1D*) mcFile->Get("hCutFlow_DYLL");
  double nBG_DYLL        = hDYLL->GetBinContent(nCuts);
  double stat_BG_DYLL    = 0.;
  double syst_BG_DYLL    = sqrt(pow(hDYLL->GetBinError(nCuts),2)
				+pow(nBG_DYLL*constants::syst_DYLL,2)); // for combine tool
  double syst_BG_DYLL_p  = sqrt(pow(hDYLL->GetBinError(nCuts),2)
				+pow(nBG_DYLL*constants::syst_lumi,2)
				+pow(nBG_DYLL*constants::syst_DYLL,2)
                                +pow(nBG_DYLL*constants::syst_DYLL_El_m,2)
				+pow(nBG_DYLL*constants::syst_DYLL_Mu_m,2)
				+pow(nBG_DYLL*constants::syst_DYLL_JES_p,2)
				+pow(nBG_DYLL*constants::syst_DYLL_JER_p,2)
				+pow(nBG_DYLL*constants::syst_DYLL_UES_p,2));
  double syst_BG_DYLL_m  = sqrt(pow(hDYLL->GetBinError(nCuts),2)
				+pow(nBG_DYLL*constants::syst_lumi,2)
				+pow(nBG_DYLL*constants::syst_DYLL,2)
				+pow(nBG_DYLL*constants::syst_DYLL_El_p,2)
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
			       pow(stat_MC_QCD,2) +
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

  double nBG_OtherNoWG = (nBG_TTbar +
                      	  nBG_SingleT +
                      	  nBG_DibosonNoWG +
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
  texFile << "$Z \\rightarrow \\nu\\nu$ \t & $" << nBG_Z << " \\pm " << stat_BG_Z << " \\pm " << syst_BG_Z_p << "/" << syst_BG_Z_m  << "$ \t &  $" << nBG_ZMC << " \\pm " << syst_BG_ZMC << "$ \\\\" << std::endl;
  texFile << "$W \\rightarrow e \\nu$  \t & $" << nBG_WEl << " \\pm " << stat_BG_WEl << " \\pm " << syst_BG_WEl_p << "/" << syst_BG_WEl_m  << "$ \t &  $" << nBG_WElMC << " \\pm " << syst_BG_WElMC << "$ \\\\" << std::endl;
  texFile << "$W \\rightarrow \\mu\\nu$ \t & $" << nBG_WMu << " \\pm " << stat_BG_WMu << " \\pm " << syst_BG_WMu_p << "/" << syst_BG_WMu_m  << "$ \t & $" << nBG_WMuMC << " \\pm " << syst_BG_WMuMC << "$ \\\\" << std::endl;
  texFile << "$W \\rightarrow \\tau \\nu$ \t & $" << nBG_WTau << " \\pm " << stat_BG_WTau << " \\pm " << syst_BG_WTau_p << "/" << syst_BG_WTau_m  << "$ \t & $" << nBG_WTauMC << " \\pm " << syst_BG_WTauMC << "$ \\\\" << std::endl;
  texFile << "QCD multijet \t & $" << nBG_QCD << " \\pm " << stat_MC_QCD << " \\pm " << syst_BG_QCD_p << "/" << syst_BG_QCD_m  << "$ \t & - \\\\" << std::endl;
  texFile << "$t\\bar{t}$ \t & -	\t & $" << nBG_TTbar << " \\pm " << syst_BG_TTbar_p << "/" << syst_BG_TTbar_m << "$ \\\\" << std::endl;
  texFile << "single t \t & -  \t & $" << nBG_SingleT << " \\pm " << syst_BG_SingleT_p << "/" << syst_BG_SingleT_m << "$ \\\\" << std::endl;
  texFile << "$VV$ \t & -  \t & $" << nBG_Diboson << " \\pm " << syst_BG_Diboson_p << "/" << syst_BG_Diboson_m << "$ \\\\" << std::endl;
  texFile << "DY \t & -  \t & $" << nBG_DYLL << " \\pm " << syst_BG_DYLL_p << "/" << syst_BG_DYLL_m << "$ \\\\" << std::endl;
  texFile << "Sum other BGs \t & -  \t & $" << nBG_Other << " \\pm " << syst_BG_Other_p << "/" << syst_BG_Other_m << "$ \\\\" << std::endl;
  texFile << "\\hline" << std::endl;
  texFile << "Total  & \\multicolumn{2}{c|}{$" << nBG_Total << "\\pm" << stat_BG_Total << "\\pm" << syst_BG_Total_p << "/" << syst_BG_Total_m << "$}  \\\\" << std::endl;
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


  // get signal efficiency
   std::cout << "Getting signal efficiency" << std::endl;
  std::vector<double> effSig(constants::nM);
  std::vector<double> errEffSig(constants::nM);
  std::vector<double> nSig(constants::nM);
  std::vector<double> errNSig(constants::nM);
  std::vector<double> nSigggH(constants::nM);
  std::vector<double> errNSigggH(constants::nM);
  std::vector<double> nSigggH_unweighted(constants::nM);

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

    hstring = std::string("hCutFlow_GluGluM")+ss.str();
    TH1D* sigggHCutFlow = (TH1D*) sigFile->Get(hstring.c_str());
    nSigggH.at(i)   = sigggHCutFlow->GetBinContent(nCuts);
    errNSigggH.at(i)= sigggHCutFlow->GetBinError(nCuts);

    hstring = std::string("hCutFlow_unweighted_GluGluM")+ss.str();
    TH1D* sigggHCutFlow_unweighted = (TH1D*) sigFile->Get(hstring.c_str());
    nSigggH_unweighted.at(i)   = sigggHCutFlow_unweighted->GetBinContent(nCuts);

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
  double fZElp  = 1 + constants::syst_Z_El_p;
  double fZElm  = 1 + constants::syst_Z_El_m;
  double fZMup  = 1 + constants::syst_Z_Mu_p;
  double fZMum  = 1 + constants::syst_Z_Mu_m;
  double fZJESp = 1 + constants::syst_Z_JES_p;
  double fZJESm = 1 + constants::syst_Z_JES_m;
  double fZJERp = 1 + constants::syst_Z_JER_p;
  double fZJERm = 1 + constants::syst_Z_JER_m;
  double fZUESp = 1 + constants::syst_Z_UES_p;
  double fZUESm = 1 + constants::syst_Z_UES_m;

  double fWMu     = 1 + sqrt(pow(syst_BG_WMu/nBG_WMu,2) + pow(stat_BG_WMu/nBG_WMu,2));
  double fWMuElp  = 1 + constants::syst_WMu_El_p;
  double fWMuElm  = 1 + constants::syst_WMu_El_m;
  double fWMuMup  = 1 + constants::syst_WMu_Mu_p;
  double fWMuMum  = 1 + constants::syst_WMu_Mu_m;
  double fWMuJESp = 1 + constants::syst_WMu_JES_p;
  double fWMuJESm = 1 + constants::syst_WMu_JES_m;
  double fWMuJERp = 1 + constants::syst_WMu_JER_p;
  double fWMuJERm = 1 + constants::syst_WMu_JER_m;
  double fWMuUESp = 1 + constants::syst_WMu_UES_p;
  double fWMuUESm = 1 + constants::syst_WMu_UES_m;

  double fWEl     = 1 + sqrt(pow(syst_BG_WEl/nBG_WEl,2) + pow(stat_BG_WEl/nBG_WEl,2));
  double fWElElp  = 1 + constants::syst_WEl_El_p;
  double fWElElm  = 1 + constants::syst_WEl_El_m;
  double fWElMup  = 1 + constants::syst_WEl_Mu_p;
  double fWElMum  = 1 + constants::syst_WEl_Mu_m;
  double fWElJESp = 1 + constants::syst_WEl_JES_p;
  double fWElJESm = 1 + constants::syst_WEl_JES_m;
  double fWElJERp = 1 + constants::syst_WEl_JER_p;
  double fWElJERm = 1 + constants::syst_WEl_JER_m;
  double fWElUESp = 1 + constants::syst_WEl_UES_p;
  double fWElUESm = 1 + constants::syst_WEl_UES_m;

  double fWTau     = 1 + sqrt(pow(syst_BG_WTau/nBG_WTau,2) + pow(stat_BG_WTau/nBG_WTau,2));
  double fWTauElp  = 1 + constants::syst_WTau_El_p;
  double fWTauElm  = 1 + constants::syst_WTau_El_m;
  double fWTauMup  = 1 + constants::syst_WTau_Mu_p;
  double fWTauMum  = 1 + constants::syst_WTau_Mu_m;
  double fWTauJESp = 1 + constants::syst_WTau_JES_p;
  double fWTauJESm = 1 + constants::syst_WTau_JES_m;
  double fWTauJERp = 1 + constants::syst_WTau_JER_p;
  double fWTauJERm = 1 + constants::syst_WTau_JER_m;
  double fWTauUESp = 1 + constants::syst_WTau_UES_p;
  double fWTauUESm = 1 + constants::syst_WTau_UES_m;

  double fQCD     = 1 + sqrt(pow(constants::syst_QCD3,2) + pow(stat_BG_QCD/nBG_QCD,2));
  double fQCDElp  = 1 + constants::syst_QCD3_El_p;
  double fQCDElm  = 1 + constants::syst_QCD3_El_m;
  double fQCDMup  = 1 + constants::syst_QCD3_Mu_p;
  double fQCDMum  = 1 + constants::syst_QCD3_Mu_m;
  double fQCDJESp = 1 + constants::syst_QCD3_JES_p;
  double fQCDJESm = 1 + constants::syst_QCD3_JES_m;
  double fQCDJERp = 1 + constants::syst_QCD3_JER_p;
  double fQCDJERm = 1 + constants::syst_QCD3_JER_m;
  double fQCDUESp = 1 + constants::syst_QCD3_UES_p;
  double fQCDUESm = 1 + constants::syst_QCD3_UES_m;
 
  double fOther     = 1 + (sqrt(pow(syst_BG_TTbar,2) + pow(syst_BG_SingleT,2) + pow(syst_BG_Diboson,2) + pow(syst_BG_DYLL,2))/nBG_OtherNoWG);
  double fOtherElp  = 1 + constants::syst_OtherNoWG_El_p;
  double fOtherElm  = 1 + constants::syst_OtherNoWG_El_m;
  double fOtherMup  = 1 + constants::syst_OtherNoWG_Mu_p;
  double fOtherMum  = 1 + constants::syst_OtherNoWG_Mu_m;
  double fOtherJESp = 1 + constants::syst_OtherNoWG_JES_p;
  double fOtherJESm = 1 + constants::syst_OtherNoWG_JES_m;
  double fOtherJERp = 1 + constants::syst_OtherNoWG_JER_p;
  double fOtherJERm = 1 + constants::syst_OtherNoWG_JER_m;
  double fOtherUESp = 1 + constants::syst_OtherNoWG_UES_p;
  double fOtherUESm = 1 + constants::syst_OtherNoWG_UES_m;
  
  double fWG     = 1 + constants::syst_WG;
  double fWGElp  = 1 + constants::syst_WG_El_p;
  double fWGElm  = 1 + constants::syst_WG_El_m;
  double fWGMup  = 1 + constants::syst_WG_Mu_p;
  double fWGMum  = 1 + constants::syst_WG_Mu_m;
  double fWGJESp = 1 + constants::syst_WG_JES_p;
  double fWGJESm = 1 + constants::syst_WG_JES_m;
  double fWGJERp = 1 + constants::syst_WG_JER_p;
  double fWGJERm = 1 + constants::syst_WG_JER_m;
  double fWGUESp = 1 + constants::syst_WG_UES_p;
  double fWGUESm = 1 + constants::syst_WG_UES_m;

  double aZ    = nBG_Z / nCtrlZ;
  double aWG   = nBG_WG / nBG_WG_unweighted;

  // write combine tool card file
  std::cout << "Writing combine tool card files" << std::endl;

  for (int i=0; i<constants::nM; ++i) {

    double fSig      = 1 + errNSig.at(i)/nSig.at(i);
    double fSigPDF   = 1 + constants::syst_Sig_PDF[i];
    double fSigQCD   = 1 + constants::syst_Sig_QCD[i];
    double fSigElp   = 1 + constants::syst_Sig_El_p[i];
    double fSigElm   = 1 + constants::syst_Sig_El_m[i];
    double fSigMup   = 1 + constants::syst_Sig_Mu_p[i];
    double fSigMum   = 1 + constants::syst_Sig_Mu_m[i];
    double fSigJESp  = 1 + constants::syst_Sig_JES_p[i];
    double fSigJESm  = 1 + constants::syst_Sig_JES_m[i];
    double fSigJERp  = 1 + constants::syst_Sig_JER_p[i];
    double fSigJERm  = 1 + constants::syst_Sig_JER_m[i];
    double fSigUESp  = 1 + constants::syst_Sig_UES_p[i];
    double fSigUESm  = 1 + constants::syst_Sig_UES_m[i];

    double aSigggH	= nSigggH.at(i)/nSigggH_unweighted.at(i);	//for gmN
    double fSigggH      = 1 + errNSigggH.at(i)/nSigggH.at(i);
    double fSigggHPDF   = 1 + constants::syst_Sig_ggH_PDF[i];
    double fSigggHQCD   = 1 + constants::syst_Sig_ggH_QCD[i];
    double fSigggHElp   = 1 + constants::syst_Sig_ggH_El_p[i];
    double fSigggHElm   = 1 + constants::syst_Sig_ggH_El_m[i];
    double fSigggHMup   = 1 + constants::syst_Sig_ggH_Mu_p[i];
    double fSigggHMum   = 1 + constants::syst_Sig_ggH_Mu_m[i];
    double fSigggHJESp  = 1 + constants::syst_Sig_ggH_JES_p[i];
    double fSigggHJESm  = 1 + constants::syst_Sig_ggH_JES_m[i];
    double fSigggHJERp  = 1 + constants::syst_Sig_ggH_JER_p[i];
    double fSigggHJERm  = 1 + constants::syst_Sig_ggH_JER_m[i];
    double fSigggHUESp  = 1 + constants::syst_Sig_ggH_UES_p[i];
    double fSigggHUESm  = 1 + constants::syst_Sig_ggH_UES_m[i];

    std::stringstream ss;
    ss << (int) constants::mH[i];

    std::string filename = options.oDir+std::string("/vbfhinv_")+ss.str()+std::string("_8TeV.txt");
    txtFile.open(filename);
    txtFile << "# Invisible Higgs analysis for mH=" << constants::mH[i] << " GeV" << std::endl;
    txtFile << "imax 1" << std::endl;
    txtFile << "jmax 8  number of backgrounds" << std::endl;
    txtFile << "kmax 21  number of nuisance parameters (sources of systematical uncertainties)" << std::endl;
    txtFile << "------------" << std::endl;
    txtFile << "# we have just one channel, in which we observe 0 events" << std::endl;
    txtFile << "bin 1" << std::endl;
    txtFile << "observation " << nObs << std::endl;
    txtFile << "------------" << std::endl;
    txtFile << "bin              1  \t  1  \t  1  \t 1  \t 1  \t 1  \t 1    \t 1   \t 1     " << std::endl;
    txtFile << "process          ggH\t qqH \t zvv \t wmu \t wel \t wtau \t qcd \t others \t wg" << std::endl;
    txtFile << "process          -1 \t 0  \t 1   \t 2   \t 3  \t 4    \t 5   \t 6  \t 7   " << std::endl;
    txtFile << "rate            " << nSigggH.at(i) << "\t" << nSig.at(i) << "\t" << nBG_Z << "\t" << nBG_WMu << "\t" << nBG_WEl << "\t" << nBG_WTau << "\t" << nBG_QCD << "\t" << nBG_OtherNoWG << "\t" << nBG_WG << std::endl;
    txtFile << "------------" << std::endl;
    txtFile << "lumi_8TeV     lnN   " << fLum << "\t" << fLum << "\t - \t - \t - \t - \t - \t" << fLum  << "\t" << fLum << std::endl;
    txtFile << "CMS_eff_e     lnN   " << fSigggHElm << "/" << fSigggHElp << "\t" << fSigElm << "/" << fSigElp << "\t" << fZElm << "/" << fZElp << "\t" << fWMuElm << "/" << fWMuElp << "\t" << fWElElm << "/" << fWElElp << "\t" << fWTauElm << "/" << fWTauElp << "\t" << fQCDElm << "/" << fQCDElp << "\t" << fOtherElm << "/" << fOtherElp << "\t" << fWGElm << "/" << fWGElp << std::endl;
    txtFile << "CMS_eff_m     lnN   " << fSigggHMum << "/" << fSigggHMup << "\t" << fSigMum << "/" << fSigMup << "\t" << fZMum << "/" << fZMup << "\t" << fWMuMum << "/" << fWMuMup << "\t" << fWElMum << "/" << fWElMup << "\t" << fWTauMum << "/" << fWTauMup << "\t" << fQCDMum << "/" << fQCDMup << "\t" << fOtherMum << "/" << fOtherMup << "\t" << fWGMum << "/" << fWGMup << std::endl;
    txtFile << "CMS_scale_j   lnN   " << fSigggHJESm << "/" << fSigggHJESp << "\t" << fSigJESm << "/" << fSigJESp << "\t" << fZJESm << "/" << fZJESp << "\t" << fWMuJESm << "/" << fWMuJESp << "\t" << fWElJESm << "/" << fWElJESp << "\t" << fWTauJESm << "/" << fWTauJESp << "\t" << fQCDJESm << "/" << fQCDJESp << "\t" << fOtherJESm << "/" << fOtherJESp <<  "\t" << fWGJESm << "/" << fWGJESp << std::endl;
    txtFile << "CMS_res_j     lnN   " << fSigggHJERm << "/" << fSigggHJERp << "\t" << fSigJERm << "/" << fSigJERp << "\t" << fZJERm << "/" << fZJERp << "\t" << fWMuJERm << "/" << fWMuJERp << "\t" << fWElJERm << "/" << fWElJERp << "\t" << fWTauJERm << "/" << fWTauJERp << "\t" << fQCDJERm << "/" << fQCDJERp << "\t" << fOtherJERm << "/" << fOtherJERp <<  "\t" << fWGJERm << "/" << fWGJERp << std::endl;
    txtFile << "CMS_scale_met lnN   " << fSigggHUESm << "/" << fSigggHUESp << "\t" << fSigUESm << "/" << fSigUESp << "\t" << fZUESm << "/" << fZUESp << "\t" << fWMuUESm << "/" << fWMuUESp << "\t" << fWElUESm << "/" << fWElUESp << "\t" << fWTauUESm << "/" << fWTauUESp << "\t" << fQCDUESm << "/" << fQCDUESp << "\t" << fOtherUESm << "/" << fOtherUESp <<  "\t" << fWGUESm << "/" << fWGUESp << std::endl;
    txtFile << "pdf_qqbar                 lnN    - \t" << fSigPDF  << "\t - \t - \t - \t - \t - \t - \t - \t" << std::endl;
    txtFile << "QCDscale_qqH              lnN    - \t" << fSigQCD  << "\t - \t - \t - \t - \t - \t - \t - \t" << std::endl;
    txtFile << "pdf_gg                    lnN   " << fSigggHPDF  << "\t - \t - \t - \t - \t - \t - \t - \t - \t" << std::endl;
    txtFile << "QCDscale_ggH2in           lnN   " << fSigggHQCD  << "\t - \t - \t - \t - \t - \t - \t - \t - \t" << std::endl;
    //    txtFile << "UEPS                      lnN   " << fSigggHUEPS  << "\t - \t - \t - \t - \t - \t - \t" << std::endl;
    txtFile << "CMS_VBFHinv_zvv_stat      gmN " << nCtrlZ << " -\t -\t" << aZ << "\t - \t - \t - \t - \t - \t - " << std::endl;
    txtFile << "CMS_VBFHinv_zvv_norm      lnN     - \t - \t" << fZ << "\t - \t - \t - \t - \t - \t - " << std::endl;
    txtFile << "CMS_VBFHinv_wmu_norm      lnN     - \t - \t - \t" << fWMu << "\t - \t - \t - \t - \t - " << std::endl;
    txtFile << "CMS_VBFHinv_wel_norm      lnN     - \t - \t - \t - \t" << fWEl << "\t - \t - \t - \t - " << std::endl;
    txtFile << "CMS_VBFHinv_wtau_norm     lnN     - \t - \t - \t - \t - \t" << fWTau << "\t - \t - \t - " << std::endl;
    txtFile << "CMS_VBFHinv_qcd_norm      lnN     - \t - \t - \t - \t - \t - \t" << fQCD << "\t - \t -" << std::endl;
    txtFile << "CMS_VBFHinv_other_norm    lnN     - \t - \t - \t - \t - \t - \t - \t" << fOther << "\t - " << std::endl;
    txtFile << "CMS_VBFHinv_wg_norm	  gmN " << nBG_WG_unweighted << " - \t - \t - \t - \t - \t - \t - \t - \t" << aWG << std::endl;
    txtFile << "CMS_VBFHinv_wg_xsunc	  lnN	  - \t - \t - \t - \t - \t - \t - \t - \t" << fWG << std::endl;
    txtFile << "CMS_VBFHinv_ggH_norm      gmN " << nSigggH_unweighted.at(i) << "\t" << aSigggH << " \t - \t - \t - \t - \t - \t - \t - \t - \t"  << std::endl;
    txtFile << "CMS_VBFHinv_qqH_norm      lnN     - \t " << fSig << "\t - \t - \t - \t - \t - \t - \t - \t"  << std::endl;
    txtFile.close();

  }


  // clean up
  ofile->Close();

}
