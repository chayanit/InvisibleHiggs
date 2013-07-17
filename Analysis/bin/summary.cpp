
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

  TFile* zFile = TFile::Open((oDir+std::string("/ZBackground.root")).c_str(), "READ");
  TH1D* hZ   = (TH1D*) zFile->Get("hZ_Est_S_DPhi");
  double nBG_Z     = hZ->GetBinContent(1);
  double errBG_Z   = hZ->GetBinError(1);
  TH1D* zCtrl = (TH1D*) zFile->Get("hZ_Data_C_DPhi");
  int nCtrlZ = zCtrl->GetBinContent(1);    /// counts in ctrl region for limits
  zFile->Close();

  // get W BG info
  TFile* wFile = TFile::Open((oDir+std::string("/WBackground.root")).c_str(), "READ");
  TH1D* hWMu = (TH1D*) wFile->Get("hWMu_EstS_DPhi");
  TH1D* hWEl = (TH1D*) wFile->Get("hWEl_EstS_DPhi");
  double nBG_WMu   = hWMu->GetBinContent(1);
  double errBG_WMu = hWMu->GetBinError(1);
  double nBG_WEl   = hWEl->GetBinContent(1);
  double errBG_WEl = hWEl->GetBinError(1);
  wFile->Close();
  
  // get QCD BG info
  double nBG_QCD(0.), errBG_QCD(0.);
  std::cout << "Using QCD Method " << options.qcdMethod << std::endl;
  if (options.qcdMethod==1) {
    TFile* qFile = TFile::Open((oDir+std::string("/QCDBackground.root")).c_str(), "READ");
    TH1D* hQCD = (TH1D*) qFile->Get("hQCD_Est_S_DPhi");
    nBG_QCD   = hQCD->GetBinContent(1);
    errBG_QCD = nBG_QCD * sqrt(pow(hQCD->GetBinError(1)/nBG_QCD,2) + pow(constants::syst_QCD1,2));
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
    errBG_QCD = nBG_QCD * sqrt(pow(hQCD->GetBinError(2,1)/nBG_QCD,2) + pow(constants::syst_QCD3,2));
    qFile->Close();
  }

  // get MC background histograms
  std::cout << "Get MC backgrounds" << std::endl;
  TFile* mcFile = TFile::Open((oDir+std::string("/Efficiency.root")).c_str(), "READ");

  TH1D* hTTBar   = (TH1D*) mcFile->Get("hCutFlow_TTBar");
  double nBG_TTBar     = hTTBar->GetBinContent(nCuts);
  double errBG_TTBar   = hTTBar->GetBinError(nCuts);

  TH1D* hZNuNuMC = (TH1D*) mcFile->Get("hCutFlow_ZNuNu");
  double nBG_ZMC       = hZNuNuMC->GetBinContent(nCuts);
  double errBG_ZMC     = hZNuNuMC->GetBinError(nCuts);

  TH1D* hWMuMC   = (TH1D*) mcFile->Get("hCutFlow_WMuNu");
  double nBG_WMuMC     = hWMuMC->GetBinContent(nCuts);
  double errBG_WMuMC   = hWMuMC->GetBinError(nCuts);

  TH1D* hWElMC   = (TH1D*) mcFile->Get("hCutFlow_WElNu");
  double nBG_WElMC     = hWElMC->GetBinContent(nCuts);
  double errBG_WElMC   = hWElMC->GetBinError(nCuts);

  TH1D* hWTauHad = (TH1D*) mcFile->Get("hCutFlow_WTauNu");
  double nBG_WTauHad   = hWTauHad->GetBinContent(nCuts);
  double errBG_WTauHad = hWTauHad->GetBinError(nCuts);

  TH1D* hSingleT = (TH1D*) mcFile->Get("hCutFlow_SingleTSum");
  double nBG_SingleT   = hSingleT->GetBinContent(nCuts);
  double errBG_SingleT = hSingleT->GetBinError(nCuts);

  TH1D* hDiboson = (TH1D*) mcFile->Get("hCutFlow_Diboson");
  double nBG_Diboson   = hDiboson->GetBinContent(nCuts);
  double errBG_Diboson = hDiboson->GetBinError(nCuts);

  mcFile->Close();


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
		  nBG_WEl +
		  constants::nBG_WTau);

  double errBG_W = sqrt( pow(errBG_WMu,2) + 
			 pow(errBG_WEl,2) + 
			 pow(constants::errBG_WTau,2));

  double nBG_Other = (nBG_TTBar + 
		      nBG_SingleT + 
		      nBG_Diboson);

  double errBG_Other =  sqrt( pow(errBG_TTBar,2) +
			      pow(errBG_SingleT,2) + 
			      pow(errBG_Diboson,2));
  
  // get number of observed events
  int nObs = 0;

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
  texFile << "Observed & \\multicolumn{2}{c|}{$" << nObs << "$}  \\\\" << std::endl;

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
  txtFile << "Observed\t" <<  nObs << std::endl;
  txtFile << std::endl;

  txtFile.close();


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
  double fZ = nBG_Z / nCtrlZ;
  double fW = 1 + (errBG_W / nBG_W);
  double fQCD = 1 + (errBG_QCD / nBG_QCD);

  // write combine tool card file
  std::cout << "Writing combine tool card files" << std::endl;

  for (int i=0; i<constants::nM; ++i) {

    std::stringstream ss;
    ss << (int) constants::mH[i];

    std::string filename = options.oDir+std::string("/card")+ss.str()+std::string(".txt");
    txtFile.open(filename);
    txtFile << "# Invisible Higgs analysis for mH=" << constants::mH[i] << " GeV" << std::endl;
    txtFile << "imax 1" << std::endl;
    txtFile << "jmax 4  number of backgrounds" << std::endl;
    txtFile << "kmax 6  number of nuisance parameters (sources of systematical uncertainties)" << std::endl;
    txtFile << "------------" << std::endl;
    txtFile << "# we have just one channel, in which we observe 0 events" << std::endl;
    txtFile << "bin 1" << std::endl;
    txtFile << "observation " << nObs << std::endl;
    txtFile << "------------" << std::endl;
    txtFile << "bin               1 \t 1 \t 1 \t 1 \t 1" << std::endl;
    txtFile << "process          ggH \t zJets \t wJets \t qcd \t others" << std::endl;
    txtFile << "process           0 \t 1 \t 2 \t 3 \t 4 " << std::endl;
    txtFile << "rate            " << nSig.at(i) << "\t" << nBG_Z << "\t" << nBG_W << "\t" << nBG_QCD << "\t" << nBG_Other << std::endl;
    txtFile << "------------" << std::endl;
    txtFile << "lumi      lnN    1.04   -     -     -   1.04   lumi affects both signal and w->tau. lnN = lognormal" << std::endl;
    txtFile << "xs_ggH    lnN    1.10   -     -     -    -      gg->H cross section + signal efficiency" << std::endl;
    txtFile << "Z_norm    gmN " << nCtrlZ << "  -   " << fZ << "   -     -     -      " << std::endl;
    txtFile << "W_norm    lnN     -     -   " << fW << "   -     -      " << std::endl;
    txtFile << "qcd_norm  lnN     -     -     -   " << fQCD << "   -      " << std::endl;
    txtFile << "bg_others lnN     -     -     -    -     1.10   10% placeholder" << std::endl;
    txtFile.close();

  }


  // clean up
  ofile->Close();

}
