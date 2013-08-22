
#include "InvisibleHiggs/Analysis/interface/ProgramOptions.h"
#include "InvisibleHiggs/Analysis/interface/Cuts.h"
#include "InvisibleHiggs/Analysis/interface/Histogrammer.h"
#include "InvisibleHiggs/Analysis/interface/StackPlot.h"
#include "InvisibleHiggs/Analysis/interface/SumDatasets.h"
#include "InvisibleHiggs/Analysis/interface/Datasets.h"
#include "InvisibleHiggs/Analysis/interface/Constants.h"

#include "TROOT.h"
#include "TStyle.h"
#include "TTree.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TGraph.h"
#include "TPaveText.h"
#include "TGraphAsymmErrors.h"

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>

#include <iostream>
#include <fstream>

#include "RooStats/HypoTestResult.h"
#include "RooRealVar.h"
#include "RooPlot.h"
#include "RooDataSet.h"
#include "RooTreeDataStore.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TStopwatch.h"

#include "RooStats/ProfileLikelihoodCalculator.h"
#include "RooStats/ProfileLikelihoodTestStat.h"
#include "RooStats/AsymptoticCalculator.h"
#include "RooStats/HybridCalculator.h"
#include "RooStats/MCMCCalculator.h"
#include "RooStats/UniformProposal.h"
#include "RooStats/FeldmanCousins.h"
#include "RooStats/NumberCountingPdfFactory.h"
#include "RooStats/ConfInterval.h"
#include "RooStats/HypoTestInverter.h"
#include "RooStats/PointSetInterval.h"
#include "RooStats/LikelihoodInterval.h"
#include "RooStats/LikelihoodIntervalPlot.h"
#include "RooStats/RooStatsUtils.h"
#include "RooStats/ModelConfig.h"
#include "RooStats/MCMCInterval.h"
#include "RooStats/MCMCIntervalPlot.h"
#include "RooStats/ProposalFunction.h"
#include "RooStats/ProposalHelper.h"
#include "RooFitResult.h"
#include "TGraph2D.h"

// use this order for safety on library loading
using namespace RooFit ;
using namespace RooStats ;

namespace po = boost::program_options;

int main(int argc, char* argv[]) {

  ProgramOptions options(argc, argv);

  std::string iDir = options.iDir;
  std::string oDir = options.oDir;
  double lumi = options.lumi;


  // linear fit to signal efficiency




  // read in limit tree
  std::string ifilename = oDir+std::string("/combineMerge.root");
  TFile* ifile = TFile::Open(ifilename.c_str());
  TTree *t = (TTree *) ifile->Get("limit");
  if (t == 0) {
    std::cerr << "TFile " << ifile->GetName() << " does not contain the tree" << std::endl;
    return 0; 
  }

  Double_t mass, limit, limitErr = 0;
  Float_t t_cpu, t_real;
  Int_t syst, iChannel, iToy, iMass;
  Float_t quant = -1;
  
  t->SetBranchAddress("mh", &mass);
  t->SetBranchAddress("limit", &limit);
  if (t->GetBranch("limitErr")) t->SetBranchAddress("limitErr", &limitErr);
  if (t->GetBranch("quantileExpected") == 0) { 
    std::cerr << "TFile " << ifile->GetName() << " does not have precomputed quantiles" << std::endl;
    return 0; 
  }
  t->SetBranchAddress("quantileExpected", &quant);
  t->SetBranchAddress("syst", &syst);



  // create arrays
  std::vector<double> massH, lim2SigM, lim1SigM, limExp, lim1SigP, lim2SigP;
  std::vector<double> xs2SigM, xs1SigM, xsExp, xs1SigP, xs2SigP;
  std::vector<double> limObs, xsObs;

  // loop over points
  double oldMass;
  for (size_t i = 0, n = t->GetEntries(); i < n; ++i) {
    t->GetEntry(i);

    //    std::cout << "Entry :  m=" << mass << ", quantile=" << quant << ", limit=" << limit << std::endl;

    if (fabs(quant+1)<0.001) limObs.push_back(limit);
    if (fabs(quant-0.025)<0.001) lim2SigM.push_back(limit);
    if (fabs(quant-0.16)<0.001) lim1SigM.push_back(limit);
    if (fabs(quant-0.5)<0.001) limExp.push_back(limit);
    if (fabs(quant-0.84)<0.001) lim1SigP.push_back(limit);
    if (fabs(quant-0.975)<0.001) lim2SigP.push_back(limit);

    if (mass!=oldMass) {
      massH.push_back(mass);
      oldMass = mass;
    }

  }

  // std:cout << massH.size() << " : " << limExp.size() << std::endl;

  bool doObs=false;
  if (limObs.size()>0) doObs=true;

  if (doObs) {
    std::cout << "Observed" << std::endl;
    std::cout << "mH  : lim" << std::endl;
    for (unsigned i=0; i<limObs.size(); ++i) {
      std::cout << massH.at(i) << " : " << limObs.at(i) << std::endl;
    }
  }
  else {
    std::cout << "Running blind" << std::endl;
  }
  std::cout << std::endl;

  // calculate observed XS limit
  for (unsigned i=0; i<limObs.size(); ++i) {
    xsObs.push_back(limObs.at(i) * constants::xsH[i]);
  }

  std::cout << "Expected" << std::endl;
  std::cout << "mH  : -2sig, -1sig, median, +1sig, +2sig" << std::endl;
  for (unsigned i=0; i<massH.size(); ++i) {
    std::cout << massH.at(i) << " : " << lim2SigM.at(i) << ", " << lim1SigM.at(i) << ", " << limExp.at(i) << ", " << lim1SigP.at(i) << ", " << lim2SigP.at(i) << std::endl;
  }
  
  // fix error bands and calculate xs limit 
  for (unsigned i=0; i<massH.size(); ++i) {
    lim2SigP.at(i) = lim2SigP.at(i) - limExp.at(i);
    lim1SigP.at(i) = lim1SigP.at(i) - limExp.at(i);
    lim2SigM.at(i) = limExp.at(i)   - lim2SigM.at(i);
    lim1SigM.at(i) = limExp.at(i)   - lim1SigM.at(i);

    xs2SigP.push_back(lim2SigP.at(i) * constants::xsH[i]);
    xs1SigP.push_back(lim1SigP.at(i) * constants::xsH[i]);
    xsExp.push_back  (limExp.at(i)   * constants::xsH[i]);
    xs1SigM.push_back(lim1SigM.at(i) * constants::xsH[i]);
    xs2SigM.push_back(lim2SigM.at(i) * constants::xsH[i]);
  }
  

  //  TStyle* style = gStyle;
  gStyle->SetOptStat(0);
  //  gROOT->SetStyle("Default");  

  TCanvas canvas;

  TH1D* h = new TH1D("", "", 1, 100., 410.);
  h->SetMaximum(3.);
  h->SetMinimum(0.);
  h->GetXaxis()->SetTitle("m_{H} [GeV]");
  h->GetYaxis()->SetTitle("#sigma #times BF(H #rightarrow inv)");
  h->Draw();

  TGraphAsymmErrors gXS2Sig(massH.size(), &massH[0], &xsExp[0], 0, 0, &xs2SigM[0], &xs2SigP[0]);
  gXS2Sig.SetFillColor(kYellow);
  gXS2Sig.SetFillStyle(1001);
  gXS2Sig.SetLineColor(0);
  gXS2Sig.SetLineStyle(0);
  gXS2Sig.SetLineWidth(0);
  gXS2Sig.Draw("3");

  TGraphAsymmErrors gXS1Sig(massH.size(), &massH[0], &xsExp[0], 0, 0, &xs1SigM[0], &xs1SigP[0]);
  gXS1Sig.SetFillColor(kGreen);
  gXS1Sig.SetFillStyle(1001);
  gXS1Sig.SetLineColor(0);
  gXS1Sig.SetLineStyle(0);
  gXS1Sig.SetLineWidth(0);
  gXS1Sig.Draw("3");

  TGraph gXS(massH.size(), &massH[0], &xsExp[0]);
  gXS.SetLineColor(kBlack);
  gXS.SetLineStyle(2);
  gXS.Draw("L");

  TGraph gXSObs;
  if (doObs) {
    gXSObs = TGraph(massH.size(), &massH[0], &xsObs[0]);
    gXSObs.SetLineColor(kBlack);
    gXSObs.SetLineStyle(1);
    gXSObs.Draw("L");
  }

  TGraph gXSProd(constants::nM, &constants::mH[0], &constants::xsH[0]);
  gXSProd.SetLineColor(kBlue);
  gXSProd.Draw("L");

  TLegend leg(0.5, 0.65, 0.75, 0.85, "95% CL limits", "NDC");
  leg.SetFillColor(0);
  leg.SetLineColor(0);
  if (doObs) leg.AddEntry(&gXSObs, "Observed limit", "L");
  leg.AddEntry(&gXS, "Expected limit", "L");
  leg.AddEntry(&gXS1Sig, "Expected limit (1 #sigma)", "F");
  leg.AddEntry(&gXS2Sig, "Expected limit (2 #sigma)", "F");
  leg.AddEntry(&gXSProd, "#sigma_{VBF} (SM)", "L");
  leg.Draw();

  TPaveText* cms = new TPaveText(0.12, 0.68, 0.35, 0.9, "NDC");
  cms->SetFillColor(0);
  cms->SetFillStyle(4000);
  cms->SetBorderSize(0);
  cms->SetLineColor(0);
  cms->SetTextAlign(12);
  cms->AddText("CMS Preliminary 2012");
  cms->AddText("#int L = 19.6 fb^{-1}");
  cms->Draw();

  canvas.Print( (oDir+std::string("/XSLimit.pdf")).c_str() );


  // BF limit
  h->SetMaximum(3.);
  h->SetMinimum(0.);
  h->GetXaxis()->SetTitle("m_{H} [GeV]");
  h->GetYaxis()->SetTitle("#sigma #times BF(H #rightarrow inv) / #sigma_{VBF}(SM)");
  h->Draw();

  TGraphAsymmErrors gLimit2Sig(massH.size(), &massH[0], &limExp[0], 0, 0, &lim2SigM[0], &lim2SigP[0]);
  gLimit2Sig.SetFillColor(kYellow);
  gLimit2Sig.SetFillStyle(1001);
  gLimit2Sig.SetLineColor(0);
  gLimit2Sig.SetLineStyle(0);
  gLimit2Sig.SetLineWidth(0);
  gLimit2Sig.Draw("3");

  TGraphAsymmErrors gLimit1Sig(massH.size(), &massH[0], &limExp[0], 0, 0, &lim1SigM[0], &lim1SigP[0]);
  gLimit1Sig.SetFillColor(kGreen);
  gLimit1Sig.SetFillStyle(1001);
  gLimit1Sig.SetLineColor(0);
  gLimit1Sig.SetLineStyle(0);
  gLimit1Sig.SetLineWidth(0);
  gLimit1Sig.Draw("3");

  TGraph gLimit(massH.size(), &massH[0], &limExp[0]);
  gLimit.SetLineColor(kBlack);
  gLimit.SetLineStyle(2);
  gLimit.Draw("L");

  TGraph gLimitObs;
  if (doObs) {
    gLimitObs = TGraph(massH.size(), &massH[0], &limObs[0]);
    gLimitObs.SetLineColor(kBlack);
    gLimitObs.SetLineStyle(1);
    gLimitObs.Draw("L");
  }

  TLegend leg2(0.37, 0.65, 0.61, 0.85, "95% CL limits", "NDC");
  leg2.SetFillColor(0);
  leg2.SetLineColor(0);
  if (doObs) leg2.AddEntry(&gLimitObs, "Observed limit", "L");
  leg2.AddEntry(&gLimit, "Expected limit", "L");
  leg2.AddEntry(&gLimit1Sig, "Expected limit (1 #sigma)", "F");
  leg2.AddEntry(&gLimit2Sig, "Expected limit (2 #sigma)", "F");
  //  leg2.AddEntry(&gXSProd, "#sigma_{VBF} (SM)", "L");
  leg2.Draw();

  cms->Draw();

  canvas.Print( (oDir+std::string("/xsiLimit.pdf")).c_str() );


}
