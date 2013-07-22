
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

using namespace constants;


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

  TFile* ofile = TFile::Open((oDir+std::string("/example.root")).c_str(), "RECREATE");

  // get Z BG info
  std::cout << "Get data driven backgrounds" << std::endl;

  TFile* zFile    = TFile::Open((oDir+std::string("/ZBackground.root")).c_str(), "READ");
  TH1D* hZ        = (TH1D*) zFile->Get("hZ_Est_S_DPhi");
  TH1D* hZ_Syst   = (TH1D*) zFile->Get("hZ_Est_S_DPhi_Syst");

  double nBG_Z       = hZ->GetBinContent(1);
  double stat_BG_Z   = hZ->GetBinError(1);
  double syst_BG_Z   = sqrt(pow(hZ_Syst->GetBinError(1),2)+pow(constants::syst_Z,2)); //hZ_Syst->GetBinError(1);

  TH1D* zCtrl = (TH1D*) zFile->Get("hZ_Data_C_DPhi");
  int nCtrlZ = zCtrl->GetBinContent(1);    /// counts in ctrl region for limits

  zFile->Close();

  // get W BG info
  TFile* wFile    = TFile::Open((oDir+std::string("/WBackground.root")).c_str(), "READ");
  TH1D* hWMu      = (TH1D*) wFile->Get("hWMu_EstS_DPhi");
  TH1D* hWMu_Syst = (TH1D*) wFile->Get("hWMu_EstS_DPhi_Syst");
  TH1D* hWEl      = (TH1D*) wFile->Get("hWEl_EstS_DPhi");
  TH1D* hWEl_Syst = (TH1D*) wFile->Get("hWEl_EstS_DPhi_Syst");

  double nBG_WMu     = hWMu->GetBinContent(1);
  double stat_BG_WMu = hWMu->GetBinError(1);
  double syst_BG_WMu = sqrt(pow(hWMu_Syst->GetBinError(1),2)+pow(constants::syst_WMu,2));

  double nBG_WEl   = hWEl->GetBinContent(1);
  double stat_BG_WEl = hWEl->GetBinError(1);
  double syst_BG_WEl = sqrt(pow(hWEl_Syst->GetBinError(1),2)+pow(constants::syst_WEl,2));

  wFile->Close();

  // W-tau BG
  double nBG_WTau     = constants::nBG_WTau;
  double stat_BG_WTau = nBG_WTau*constants::stat_WTau;
  double syst_BG_WTau = nBG_WTau*constants::syst_WTau;

  
  // get QCD BG info
  double nBG_QCD(0.), errBG_QCD(0.), stat_BG_QCD(0.), syst_BG_QCD(0.);
  std::cout << "Using QCD Method " << options.qcdMethod << std::endl;
  if (options.qcdMethod==1) {
    TFile* qFile = TFile::Open((oDir+std::string("/QCDBackground.root")).c_str(), "READ");
    TH1D* hQCD = (TH1D*) qFile->Get("hQCD_Est_S_DPhi");
    nBG_QCD   = hQCD->GetBinContent(1);
    stat_BG_QCD = hQCD->GetBinError(1);
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
    TH2D* hQCD = (TH2D*) qFile->Get("hEst_METCJV");
    nBG_QCD   = hQCD->GetBinContent(2,1);
    stat_BG_QCD = hQCD->GetBinError(2,1);
    qFile->Close();
  }

  // get MC background histograms
  std::cout << "Get MC backgrounds" << std::endl;
  TFile* mcFile = TFile::Open((oDir+std::string("/Efficiency.root")).c_str(), "READ");

  TH1D* hZNuNuMC = (TH1D*) mcFile->Get("hCutFlow_ZNuNu");
  double nBG_ZMC         = hZNuNuMC->GetBinContent(nCuts);
  double stat_BG_ZMC     = hZNuNuMC->GetBinError(nCuts);
  double syst_BG_ZMC     = 0.;

  TH1D* hWMuMC   = (TH1D*) mcFile->Get("hCutFlow_WMuNu");
  double nBG_WMuMC       = hWMuMC->GetBinContent(nCuts);
  double stat_BG_WMuMC   = hWMuMC->GetBinError(nCuts);
  double syst_BG_WMuMC   = 0.;

  TH1D* hWElMC   = (TH1D*) mcFile->Get("hCutFlow_WElNu");
  double nBG_WElMC       = hWElMC->GetBinContent(nCuts);
  double stat_BG_WElMC   = hWElMC->GetBinError(nCuts);
  double syst_BG_WElMC   = 0.;

  TH1D* hWTauMC  = (TH1D*) mcFile->Get("hCutFlow_WTauNu");
  double nBG_WTauMC      = hWTauMC->GetBinContent(nCuts);
  double stat_BG_WTauMC  = hWTauMC->GetBinError(nCuts);
  double syst_BG_WTauMC  = 0.;

  TH1D* hTTBar   = (TH1D*) mcFile->Get("hCutFlow_TTBar");
  double nBG_TTBar       = hTTBar->GetBinContent(nCuts);
  double stat_BG_TTBar   = hTTBar->GetBinError(nCuts);
  double syst_BG_TTBar   = 0.;

  TH1D* hSingleT = (TH1D*) mcFile->Get("hCutFlow_SingleTSum");
  double nBG_SingleT     = hSingleT->GetBinContent(nCuts);
  double stat_BG_SingleT = hSingleT->GetBinError(nCuts);
  double syst_BG_SingleT = 0.;

  TH1D* hDiboson = (TH1D*) mcFile->Get("hCutFlow_Diboson");
  double nBG_Diboson     = hDiboson->GetBinContent(nCuts);
  double stat_BG_Diboson = hDiboson->GetBinError(nCuts);
  double syst_BG_Diboson = 0.;

  mcFile->Close();


  // calculate some numbers
  double nBG_Total = (nBG_Z +
		      nBG_WMu + 
		      nBG_WEl + 
		      nBG_WTau + 
		      nBG_QCD + 
		      nBG_TTBar + 
		      nBG_SingleT + 
		      nBG_Diboson);

  double stat_BG_Total = sqrt( pow(stat_BG_Z,2) +
			     pow(stat_BG_WMu,2) +
			     pow(stat_BG_WEl,2) +
			     pow(stat_BG_WTau,2) +
			     pow(stat_BG_QCD,2) +
			     pow(stat_BG_TTBar,2) +
			     pow(stat_BG_SingleT,2) +
			     pow(stat_BG_Diboson,2));

  double syst_BG_Total = sqrt( pow(syst_BG_Z,2) +
			     pow(syst_BG_WMu,2) +
			     pow(syst_BG_WEl,2) +
			     pow(syst_BG_WTau,2) +
			     pow(syst_BG_QCD,2) +
			     pow(syst_BG_TTBar,2) +
			     pow(syst_BG_SingleT,2) +
			     pow(syst_BG_Diboson,2));

  double nBG_Other = (nBG_TTBar + 
		      nBG_SingleT + 
		      nBG_Diboson);

  double stat_BG_Other =  sqrt( pow(stat_BG_TTBar,2) +
			      pow(stat_BG_SingleT,2) + 
			      pow(stat_BG_Diboson,2));
  
  double syst_BG_Other =  sqrt( pow(syst_BG_TTBar,2) +
			      pow(syst_BG_SingleT,2) + 
			      pow(syst_BG_Diboson,2));
  

  // get number of observed events
  int nObs = 0;

  // write background summary table
  std::cout << "Writing BG summary TeX file" << std::endl;

  ofstream texFile;
  texFile.open(options.oDir+std::string("/summary.tex"));

  texFile << "Background \t & $N_{est}$ (data) \t & $N_{est}$ (MC) \\\\" << std::endl;
  texFile << "\\hline";
  texFile << "$Z \\rightarrow \\nu\\nu$ \t & $" << nBG_Z << " \\pm " << stat_BG_Z << "$ \t &  $" << nBG_ZMC << " \\pm " << stat_BG_ZMC << "$ \\\\" << std::endl;
  texFile << "$W \\rightarrow e \\nu$  \t & $" << nBG_WEl << " \\pm " << stat_BG_WEl << "$ \t &  $" << nBG_WElMC << " \\pm " << stat_BG_WElMC << "$ \\\\" << std::endl;
  texFile << "$W \\rightarrow \\mu\\nu$ \t & $" << nBG_WMu << " \\pm " << stat_BG_WMu << "$ \t & $" << nBG_WMuMC << " \\pm " << stat_BG_WMuMC << "$ \\\\" << std::endl;
  texFile << "$W \\rightarrow \\tau \\nu$ \t & $" << nBG_WTau << " \\pm " << stat_BG_WTau << "$ \t & $" << nBG_WTauMC << " \\pm " << stat_BG_WTauMC << "$ \\\\" << std::endl;
  texFile << "QCD multijet \t & $" << nBG_QCD << " \\pm " << stat_BG_QCD << "$ \t & - \\\\" << std::endl;
  texFile << "$t\\bar{t}$ \t & -	\t & $" << nBG_TTBar << " \\pm << " << stat_BG_TTBar << "$ \\\\" << std::endl;
  texFile << "single t \t & -  \t & $" << nBG_SingleT << " \\pm " << stat_BG_SingleT << "$ \\\\" << std::endl;
  texFile << "$VV$ \t & -  \t & $" << nBG_Diboson << " \\pm " << stat_BG_Diboson<< "$ \\\\" << std::endl;
  texFile << "\\hline" << std::endl;
  texFile << "Total  & \\multicolumn{2}{c|}{$" << nBG_Total << "\\pm" << stat_BG_Total << "$ (stat) $\\pm" << syst_BG_Total << "$ (syst)}  \\\\" << std::endl;
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

  double fZ     = 1 + (syst_BG_Z/nBG_Z);
  double fWMu   = 1 + (sqrt(pow(syst_BG_WMu,2)+pow(stat_BG_WMu,2))/nBG_WMu);
  double fWEl   = 1 + (sqrt(pow(syst_BG_WEl,2)+pow(stat_BG_WEl,2))/nBG_WEl);
  double fWTau  = 1 + (sqrt(pow(syst_BG_WTau,2)+pow(stat_BG_WTau,2))/nBG_WTau);
  double fQCD   = 1 + (sqrt(pow(syst_BG_QCD,2)+pow(stat_BG_QCD,2))/nBG_QCD);
  double fOther = 1 + (syst_BG_Other/nBG_Other);

  double aZ    = nBG_Z / nCtrlZ;

  // write combine tool card file
  std::cout << "Writing combine tool card files" << std::endl;

  for (int i=0; i<nM; ++i) {

    double fSig    = 1 + sqrt(pow(syst_Sig_PDF[i],2)+pow(syst_Sig_JES[i],2));

    std::stringstream ss;
    ss << (int) mH[i];

    std::string filename = options.oDir+std::string("/card")+ss.str()+std::string(".txt");
    txtFile.open(filename);
    txtFile << "# Invisible Higgs analysis for mH=" << mH[i] << " GeV" << std::endl;
    txtFile << "imax 1" << std::endl;
    txtFile << "jmax 6  number of backgrounds" << std::endl;
    txtFile << "kmax 8  number of nuisance parameters (sources of systematical uncertainties)" << std::endl;
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
    txtFile << "lumi      lnN   " << fLum << "\t - \t - \t - \t - \t - \t" << fLum << std::endl;
    //    txtFile << "jes       lnN   " << fSigJES << "\t" << fZJES << "\t" << fWMuJES << "\t" << fWElJES << "\t" << fWTauJES << "\t - \t" << fOtherJES << std::endl;
    txtFile << "qqh_norm  lnN   " << fSig      << "\t - \t - \t - \t - \t - \t - \t" << std::endl;
    txtFile << "zvv_stat  gmN " << nCtrlZ << "  -\t" << aZ << "\t - \t - \t - \t - \t - " << std::endl;
    txtFile << "zvv_norm  lnN     - \t" << fZ << "\t - \t - \t - \t - \t - " << std::endl;
    txtFile << "wmu_norm  lnN     - \t - \t" << fWMu << "\t - \t - \t - \t - " << std::endl;
    txtFile << "wel_norm  lnN     - \t - \t - \t" << fWEl << "\t - \t - \t - " << std::endl;
    txtFile << "wtau_norm lnN     - \t - \t - \t - \t" << fWTau << "\t - \t - " << std::endl;
    txtFile << "qcd_norm  lnN     - \t - \t - \t - \t - \t" << fQCD << "\t - " << std::endl;
    txtFile.close();

  }


  // clean up
  ofile->Close();

}
