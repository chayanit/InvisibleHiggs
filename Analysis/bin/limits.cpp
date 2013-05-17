
#include "InvisibleHiggs/Analysis/interface/ProgramOptions.h"
#include "InvisibleHiggs/Analysis/interface/Cuts.h"
#include "InvisibleHiggs/Analysis/interface/Histogrammer.h"
#include "InvisibleHiggs/Analysis/interface/StackPlot.h"
#include "InvisibleHiggs/Analysis/interface/SumDatasets.h"
#include "InvisibleHiggs/Analysis/interface/Datasets.h"

#include "TROOT.h"
#include "TStyle.h"
#include "TTree.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TGraph.h"
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


  // read background estimates from file
  std::cout << "Backgrounds" << std::endl;
  double bgZjets(0.), err_bgZjets(0.);
  double bgWjets(0.), err_bgWjets(0.);
  double bgQCD(0.), err_bgQCD(0.);
  double bgOther(0.), err_bgOther(0.);
  double nObs(0.);

  ifstream file;
  double n(0.), err_n(0.);
  std::string name;

  file.open((oDir+std::string("/summary.txt")).c_str());
  std::string line;
  getline(file, line);

  while (file >> name >> n >> err_n) {
    if (name == "Z") {
      bgZjets = n;
      err_bgZjets = err_n;
    }
    if (name == "W") {
      bgWjets = n;
      err_bgWjets = err_n;
    }
    if (name == "QCD") {
      bgQCD = n;
      err_bgQCD = err_n;
    }
    if (name == "Other") {
      bgOther = n;
      err_bgOther = err_n;
    }
    if (name == "Observed") {
      nObs = n;
    }
  }
  file.close();

  std::cout << "Z+jets  : " << bgZjets << " +/- " << err_bgZjets << std::endl;
  std::cout << "W+jets  : " << bgWjets << " +/- " << err_bgWjets << std::endl;
  std::cout << "QCD     : " << bgQCD << " +/- " << err_bgQCD << std::endl;
  std::cout << "Other   : " << bgOther << " +/- " << err_bgOther << std::endl;

  // total BG
  double bgTot     = bgZjets + bgWjets + bgQCD + bgOther;
  double err_bgTot = sqrt( pow(err_bgZjets, 2) + pow(err_bgWjets, 2) + pow(err_bgQCD, 2) + pow(err_bgOther, 2) );

  if (nObs==0.) nObs=(int) bgTot;

  std::cout << "Total   : " << bgTot << " +/- " << err_bgTot << std::endl;
  std::cout << std::endl;

  std::cout << "Obs     : " << nObs << std::endl;
  std::cout << std::endl;

  // read signal efficiencies from cut flow histograms
  std::cout << "Signal efficiency" << std::endl;
 std:;cout << std::endl;

  std::vector<double> mH, xsH, effSignal, err_effSignal;

  /// MASSES AND CROSS-SECTIONS HARDCODED HERE !!!
  static const double mArr[] = {110., 125., 150., 200., 300., 400.};
  mH.assign(mArr, mArr+6);

  static const double xsArr[] = {1.809, 1.649, 1.280, 0.8685, 0.4408, 0.2543};
  xsH.assign(xsArr, xsArr+6);

  TFile* sigFile = TFile::Open( (oDir+"/Efficiency.root").c_str() , "READ");

  std::cout << "mass\txs\teff\terr" << std::endl;

  for (unsigned i=0; i<mH.size(); ++i) {
    std::string name("hCutFlow_SignalM");
    stringstream ss;
    ss << (int) mH.at(i);
    name += ss.str();
    name += std::string("_POWHEG");
    TH1D* h = (TH1D*) sigFile->Get(name.c_str());
    double n = h->GetBinContent(10);
    double err = h->GetBinError(10);
    std::cout << n << "\t" << err << std::endl;
    effSignal.push_back (n / (lumi * xsH.at(i)) );
    err_effSignal.push_back (err / (lumi * xsH.at(i)) );
    std::cout << mH.at(i) << "\t" << xsH.at(i) << "\t" << effSignal.at(i) << "\t" << err_effSignal.at(i) << std::endl;
  }
  
  std::cout << std::endl;

  /// calculate limits
  using namespace RooFit ;
  using namespace RooStats ;

  // model
  std::cout << "Setting up model" << std::endl;

  // variables
//   RooRealVar nobs("nobs","", nObs, 0, 2000);
//   RooRealVar nbg("nbg", "", bgTot, 0, 2000);
//   RooRealVar bg_mu("bg_mu", "", bgTot, 0, 200);
//   RooRealVar bg_sigma("bg_sigma", "", err_bgTot, 0, 200);
//   RooRealVar nsig("nsig", "", 0, 0, 2000);
//   RooRealVar sig_mu("sig_mu", "", 0, 0, 100);

//   // pdf
//   RooAddition mean("mean", "", RooArgList(bg_mu, sig_mu));
//   RooPoisson pdf("pdf", "", nobs, mean);

//   // dataset
//   RooDataSet* data = new RooDataSet("Data", "Data", RooArgSet(nobs));
//   data->add(nobs);
//   data->Print("v");

  // Model
  RooWorkspace w("w", true);

  // 
  w.factory("sum:nexp(s[3,0,500],b[500,0,1000])");

  // signal
  w.factory("Poisson:pdf(nobs[0,1000],nexp)");

  // background
  w.factory("Gaussian:constraint(b0[0,1000],b,sigmab[1])");
  w.var("b0")->setVal(bgTot);
  w.var("b0")->setConstant(true); // needed for being treated as global observables
  w.var("sigmab")->setVal(err_bgTot);  
  w.var("b")->setMax(bgTot+10*(err_bgTot/bgTot));

  w.factory("PROD:model(pdf,constraint)");

  // data
  RooRealVar* nobs = w.var("nobs");
  nobs->setVal(nObs);

  RooDataSet data("data","", *nobs );
  data.add(*nobs );
  w.import(data);

  // signal+background model
  ModelConfig sbModel("sbModel", &w);
  sbModel.SetPdf( *w.pdf("model") );
  sbModel.SetParametersOfInterest( *w.var("s") );
  sbModel.SetObservables( *w.var("nobs") );
  sbModel.SetNuisanceParameters( *w.var("b") );

  sbModel.SetSnapshot(*w.var("s"));
  sbModel.SetGlobalObservables(*w.var("b0"));

  w.Print();

  // for bayesian/hybrid
  w.factory("Uniform::prior_s(s)");
  sbModel.SetPriorPdf(*w.pdf("prior_s"));

  // background only model (copy of S+B model with S=0)
  ModelConfig * bModel = sbModel.Clone();
  bModel->SetName("bModel");
  w.var("s")->setVal(0);
  bModel->SetSnapshot( *w.var("s") );
  w.var("s")->setVal(nObs);

  AsymptoticCalculator *  ac = new AsymptoticCalculator(data, *bModel, sbModel);
  ac->SetOneSided(true);  // for one-side tests (limits)
  AsymptoticCalculator::SetPrintLevel(-1);
  //  ac->SetToys(1000,500);    // 1000 for null (S+B) , 50 for alt (B)
  
  HypoTestInverter calc(*ac);
  
  // set confidence level (e.g. 95% upper limits)
  calc.SetConfidenceLevel(0.95);
  
  // for CLS
  bool useCLs = true;
  calc.UseCLs(useCLs);
  calc.SetVerbose(false);
  
  ToyMCSampler *toymcs = (ToyMCSampler*)calc.GetHypoTestCalculator()->GetTestStatSampler();
  // for number counting (extended pdf do not need this)
  // toymcs->SetNEventsPerToy(1);
  
  
  // profile likelihood test statistics 
  ProfileLikelihoodTestStat profll(*sbModel.GetPdf());
  // for CLs (bounded intervals) use one-sided profile likelihood
  if (useCLs) profll.SetOneSided(true);
  
  // ratio of profile likelihood - need to pass snapshot for the alt 
  // RatioOfProfiledLikelihoodsTestStat ropl(*sbModel->GetPdf(), *bModel->GetPdf(), bModel->GetSnapshot());
  
  
  // set the test statistic to use 
  toymcs->SetTestStatistic(&profll);

  int npoints = 100;  // number of points to scan
  double poimin = 0;
  double poimax = 5*xsH[0]*lumi*effSignal[0];  // scan up to 10 times SM production XS

  
  std::cout << "Doing a fixed scan  in interval : " << poimin << " , " << poimax << std::endl;
  calc.SetFixedScan(npoints,poimin,poimax);
  
  HypoTestInverterResult * r = calc.GetInterval();
  
  double upperLimit = r->UpperLimit();
  double ulError = r->UpperLimitEstimatedError();

  double expLimit = r->GetExpectedUpperLimit(0);
  double expLimitM1Sig = r->GetExpectedUpperLimit(-1);
  double expLimitP1Sig = r->GetExpectedUpperLimit(+1);
  double expLimitM2Sig = r->GetExpectedUpperLimit(-2);
  double expLimitP2Sig = r->GetExpectedUpperLimit(+2);

  std::cout << "Observed upper limit          : " << upperLimit << std::endl;
  std::cout << "Expected upper limit (median) : " << expLimit << std::endl;
  std::cout << "Expected upper limit (-1sig)  : " << expLimitM1Sig << std::endl;
  std::cout << "Expected upper limit (+1sig)  : " << expLimitP1Sig << std::endl;
  std::cout << "Expected upper limit (-2Sig)  : " << expLimitM2Sig << std::endl;
  std::cout << "Expected upper limit (+2sig)  : " << expLimitP2Sig << std::endl;
  std::cout << std::endl;

  std::cout << "Exp limit on sigma x BF (mH=125) : " << expLimit / (lumi * effSignal[1] * xsH[1]) << std::endl;
  std::cout << std::endl;

  // convert to x-section
  double xsExp[10], xsExpM1Sig[10], xsExpP1Sig[10], xsExpM2Sig[10], xsExpP2Sig[10];
  double xsiExp[10], xsiExpM1Sig[10], xsiExpP1Sig[10], xsiExpM2Sig[10], xsiExpP2Sig[10];

  for (unsigned j=0; j<mH.size(); ++j) {
    xsExp[j]       = expLimit / (lumi * effSignal[j]);
    xsExpM1Sig[j]  = expLimitM1Sig / (lumi * effSignal[j]);
    xsExpP1Sig[j]  = expLimitP1Sig / (lumi * effSignal[j]);
    xsExpM2Sig[j]  = expLimitM2Sig / (lumi * effSignal[j]);
    xsExpP2Sig[j]  = expLimitP2Sig / (lumi * effSignal[j]);
    xsiExp[j]      = xsExp[j] / xsH[j];
    xsiExpM1Sig[j] = xsExpM1Sig[j] / xsH[j];
    xsiExpP1Sig[j] = xsExpP1Sig[j] / xsH[j];
    xsiExpM2Sig[j] = xsExpM2Sig[j] / xsH[j];
    xsiExpP2Sig[j] = xsExpP2Sig[j] / xsH[j];
    std::cout << mH[j] << " " << xsH[j] << " " << xsExp[j] << " " << xsiExp[j] << std::endl;
  }

  // plot xs limit
  //  TStyle* style = gStyle;
  gStyle->SetOptStat(0);
  //  gROOT->SetStyle("Default");  

  TCanvas canvas;

  TH1D* h = new TH1D("", "", 1, 100., 450.);
  h->SetMaximum(3.);
  h->SetMinimum(0.);
  h->GetXaxis()->SetTitle("m_{H} [GeV]");
  h->GetYaxis()->SetTitle("#sigma #times BF(H #rightarrow inv)");
  h->Draw();

  TGraphAsymmErrors gXSExpLimit2Sig(mH.size(), &mH[0], &xsExp[0], 0, 0, &xsExpM2Sig[0], &xsExpP2Sig[0]);
  gXSExpLimit2Sig.SetFillColor(kYellow);
  gXSExpLimit2Sig.SetFillStyle(1001);
  gXSExpLimit2Sig.SetLineColor(0);
  gXSExpLimit2Sig.SetLineStyle(0);
  gXSExpLimit2Sig.SetLineWidth(0);
  gXSExpLimit2Sig.Draw("3");

  TGraphAsymmErrors gXSExpLimit1Sig(mH.size(), &mH[0], &xsExp[0], 0, 0, &xsExpM1Sig[0], &xsExpP1Sig[0]);
  gXSExpLimit1Sig.SetFillColor(kGreen);
  gXSExpLimit1Sig.SetFillStyle(1001);
  gXSExpLimit1Sig.SetLineColor(0);
  gXSExpLimit1Sig.SetLineStyle(0);
  gXSExpLimit1Sig.SetLineWidth(0);
  gXSExpLimit1Sig.Draw("3");

  TGraph gXSExpLimit(mH.size(), &mH[0], &xsExp[0]);
  gXSExpLimit.SetLineColor(kBlack);
  gXSExpLimit.SetLineStyle(2);
  gXSExpLimit.Draw("L");

  TGraph gXSProd(mH.size(), &mH[0], &xsH[0]);
  gXSProd.SetLineColor(kBlue);
  gXSProd.Draw("L");

  TLegend leg(0.5, 0.65, 0.75, 0.85, "95% CL limits", "NDC");
  leg.SetFillColor(0);
  leg.AddEntry(&gXSExpLimit, "Expected limit", "L");
  leg.AddEntry(&gXSExpLimit1Sig, "Expected limit (1 #sigma)", "F");
  leg.AddEntry(&gXSExpLimit2Sig, "Expected limit (2 #sigma)", "F");
  leg.AddEntry(&gXSProd, "#sigma_{VBF} (SM)", "L");
  leg.Draw();

  canvas.Print( (oDir+std::string("/XSLimit.pdf")).c_str() );
  
  // xsiExp plot
  h->GetYaxis()->SetTitle("#sigma #times BF(H #rightarrow inv) / #sigma(SM)");
  h->Draw();

  TGraphAsymmErrors gXsiExpLimit2Sig(mH.size(), &mH[0], &xsiExp[0], 0, 0, &xsiExpM2Sig[0], &xsiExpP2Sig[0]);
  gXsiExpLimit2Sig.SetFillColor(kYellow);
  gXsiExpLimit2Sig.SetFillStyle(1001);
  gXsiExpLimit2Sig.SetLineColor(0);
  gXsiExpLimit2Sig.SetLineStyle(0);
  gXsiExpLimit2Sig.SetLineWidth(0);
  gXsiExpLimit2Sig.Draw("3");

  TGraphAsymmErrors gXsiExpLimit1Sig(mH.size(), &mH[0], &xsiExp[0], 0, 0, &xsiExpM1Sig[0], &xsiExpP1Sig[0]);
  gXsiExpLimit1Sig.SetFillColor(kGreen);
  gXsiExpLimit1Sig.SetFillStyle(1001);
  gXsiExpLimit1Sig.SetLineColor(0);
  gXsiExpLimit1Sig.SetLineStyle(0);
  gXsiExpLimit1Sig.SetLineWidth(0);
  gXsiExpLimit1Sig.Draw("3");

  TGraph gXsiExpLimit(mH.size(), &mH[0], &xsiExp[0]);
  gXsiExpLimit.SetLineColor(kBlack);
  gXsiExpLimit.SetLineStyle(2);
  gXsiExpLimit.Draw("L");

  TLine smLine(100., 1., 500., 1.);
  smLine.SetLineColor(kBlue);
  smLine.Draw();

  leg.Draw();

  canvas.Print( (oDir+std::string("/xsiLimit.pdf")).c_str() );

  // write out workspace
  w.writeToFile( (options.oDir+std::string("/workspace.root")).c_str() );

  std::cout << "Done" << std::endl;

}
