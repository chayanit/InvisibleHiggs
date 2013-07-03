
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
#include "TH1D.h"
#include "TH2D.h"
#include "TGraphAsymmErrors.h"
#include "TCanvas.h"
#include "TLegend.h"

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
  std::string oDir = options.oDir + std::string("/QCD3");

  if (oDir!="") {
    boost::filesystem::path opath(oDir);
    if (!exists(opath)) {
      std::cout << "Creating output directory : " << oDir << std::endl;
      boost::filesystem::create_directory(opath);
    }
    std::cout << "Writing results to " << oDir << std::endl;
  }

  Datasets datasets(iDir);
  datasets.readFile(options.datasetFile);

  Cuts cuts;
  TCut cutBase = cuts.cut("trigger")+
    cuts.cut("metFilter")+
    cuts.cut("EVeto")+
    cuts.cut("MuVeto")+
    cuts.cut("dijet")+
    cuts.cut("sgnEtaJJ")+
    cuts.cut("dEtaJJ")+
    cuts.cut("Mjj");
  TCut cutSpesh = cuts.zMuMuGen() +
    cuts.cut("trigger")+
    cuts.cut("metFilter")+
    cuts.cut("EVeto")+
    cuts.cut("dijet")+
    cuts.cut("sgnEtaJJ")+
    cuts.cut("dEtaJJ")+
    cuts.cut("Mjj");
  TCut cutLoDPhi = cuts.cut("dPhiJJ"); 
  TCut cutHiDPhi("vbfDPhi>2.6"); 

  TCut puWeight("puWeight");
  TCut trigWeight( "(trigCorrWeight>0.) ? trigCorrWeight : 1." );

  TFile* ofile = TFile::Open((oDir+std::string("/QCD3.root")).c_str(), "RECREATE");

  double metEdges[3] = { 0., 130., 1000. };
  double cjvEdges[3] = { -100., 30., 1000. };
  int metBins = 25;
  double metMax = 250.;
  int cjvBins = 25;
  double cjvMax = 250.;

  // ABCD method
  TH2D* hData_METCJV = new TH2D("hData_METCJV", "", 2, metEdges, 2, cjvEdges);
  TH2D* hBG_METCJV = new TH2D("hBG_METCJV", "", 2, metEdges, 2, cjvEdges);
  TH2D* hSignal_METCJV = new TH2D("hSignal_METCJV", "", 2, metEdges, 2, cjvEdges);

  // ABCD closure test
  TH2D* hData_HiDPhi_METCJV = new TH2D("hData_HiDPhi_METCJV", "", 2, metEdges, 2, cjvEdges);
  TH2D* hBG_HiDPhi_METCJV = new TH2D("hBG_HiDPhi_METCJV", "", 2, metEdges, 2, cjvEdges);
  TH2D* hSignal_HiDPhi_METCJV = new TH2D("hSignal_HiDPhi_METCJV", "", 2, metEdges, 2, cjvEdges);

  // control plots
  TH1D* hData_CJVP_MET  = new TH1D("hData_CJVP_MET",  "", metBins, 0., metMax);
  TH1D* hBG_CJVP_MET    = new TH1D("hBG_CJVP_MET",    "", metBins, 0., metMax);
  TH1D* hData_CJVF_MET  = new TH1D("hData_CJVF_MET",  "", metBins, 0., metMax);
  TH1D* hBG_CJVF_MET    = new TH1D("hBG_CJVF_MET",    "", metBins, 0., metMax);

  TH1D* hData_METP_CJEt = new TH1D("hData_METP_CJEt", "", cjvBins, 0., cjvMax);
  TH1D* hBG_METP_CJEt   = new TH1D("hBG_METP_CJEt",   "", cjvBins, 0., cjvMax);
  TH1D* hData_METF_CJEt = new TH1D("hData_METF_CJEt", "", cjvBins, 0., cjvMax);
  TH1D* hBG_METF_CJEt   = new TH1D("hBG_METF_CJEt",   "", cjvBins, 0., cjvMax);


  // loop over MC datasets
  for (unsigned i=0; i<datasets.size(); ++i) {

    Dataset dataset = datasets.getDataset(i);
    TFile* file = datasets.getTFile(dataset.name);
    TTree* tree = (TTree*) file->Get("invHiggsInfo/InvHiggsInfo");

    TCut cutD = cuts.cutDataset(dataset.name);

    TH2D* hTmp_METCJV        = new TH2D("hTmp_METCJV", "", 2, metEdges, 2, cjvEdges);
    TH2D* hTmp_HiDPhi_METCJV = new TH2D("hTmp_HiDPhi_METCJV", "", 2, metEdges, 2, cjvEdges);

    TH1D* hTmp_CJVP_MET  = new TH1D("hTmp_CJVP_MET",  "", metBins, 0., metMax);
    TH1D* hTmp_CJVF_MET  = new TH1D("hTmp_CJVF_MET",  "", metBins, 0., metMax);    
    TH1D* hTmp_METP_CJEt = new TH1D("hTmp_METP_CJEt", "", cjvBins, 0., cjvMax);
    TH1D* hTmp_METF_CJEt = new TH1D("hTmp_METF_CJEt", "", cjvBins, 0., cjvMax);
   
    double weight = (dataset.isData ? 1. : lumi * dataset.sigma / dataset.nEvents);

    if (dataset.isData) {
      std::cout << "Analysing Data         : " << dataset.name << std::endl;
      
      TCut cutLo = cutBase +cutLoDPhi;
      TCut cutHi = cutBase +cutHiDPhi;
      tree->Draw("cenJetEt:met>>hTmp_METCJV",        cutLo);
      tree->Draw("cenJetEt:met>>hTmp_HiDPhi_METCJV", cutHi);

      // remove signal region!
      hTmp_METCJV->SetBinContent(2, 1, 0.);

      hData_METCJV->Add(hTmp_METCJV, 1.);
      hData_HiDPhi_METCJV->Add(hTmp_HiDPhi_METCJV, 1.);

      // control plots
      TCut ctrlMETP = cutHi + TCut("met>130");
      TCut ctrlMETF = cutHi + TCut("met<130");
      TCut ctrlCJVP = cutHi + TCut("cenJetEt<30.");
      TCut ctrlCJVF = cutHi + TCut("cenJetEt>30.");

      tree->Draw("met>>hTmp_CJVP_MET",          ctrlCJVP);
      tree->Draw("met>>hTmp_CJVF_MET",          ctrlCJVF);
      tree->Draw("cenJetEt>>hTmp_METP_CJEt",    ctrlMETP);
      tree->Draw("cenJetEt>>hTmp_METF_CJEt",    ctrlMETF);

      hData_CJVP_MET->Add(hTmp_CJVP_MET, 1.);
      hData_CJVF_MET->Add(hTmp_CJVF_MET, 1.);
      hData_METP_CJEt->Add(hTmp_METP_CJEt, 1.);
      hData_METF_CJEt->Add(hTmp_METF_CJEt, 1.);

    }
    else if (dataset.name=="SignalM125_POWHEG") {
      std::cout << "Analysing Signal MC        : " << dataset.name << std::endl;
      
      TCut cutLo = puWeight * (cutBase + cutLoDPhi);
      TCut cutHi = puWeight * (cutBase + cutHiDPhi);

      tree->Draw("cenJetEt:met>>hTmp_METCJV",        cutLo);
      tree->Draw("cenJetEt:met>>hTmp_HiDPhi_METCJV", cutHi);

      hTmp_METCJV->Scale(weight);
      hTmp_HiDPhi_METCJV->Scale(weight);

      hSignal_METCJV->Add(hTmp_METCJV, 1.);
      hSignal_HiDPhi_METCJV->Add(hTmp_HiDPhi_METCJV, 1.);

    }
    else {
      std::cout << "Analysing BG MC        : " << dataset.name << std::endl;
      
      TCut cutLo = puWeight * trigWeight * (cutD + cutBase + cutLoDPhi);
      TCut cutHi = puWeight * trigWeight * (cutD + cutBase + cutHiDPhi);
      if (dataset.name=="WJets" ||
	  dataset.name=="W1Jets" ||
	  dataset.name=="W2Jets" ||
	  dataset.name=="W3Jets" ||
	  dataset.name=="W4Jets") {
	cutLo *= cuts.wWeight();
	cutHi *= cuts.wWeight();
      }  
      tree->Draw("cenJetEt:met>>hTmp_METCJV",        cutLo);
      tree->Draw("cenJetEt:met>>hTmp_HiDPhi_METCJV", cutHi);

      hTmp_METCJV->Scale(weight);
      hTmp_HiDPhi_METCJV->Scale(weight);

      hBG_METCJV->Add(hTmp_METCJV, 1.);
      hBG_HiDPhi_METCJV->Add(hTmp_HiDPhi_METCJV, 1.);

      // control plots
      TCut ctrlMETP = cutHi + TCut("met>130");
      TCut ctrlMETF = cutHi + TCut("met<130");
      TCut ctrlCJVP = cutHi + TCut("cenJetEt<30.");
      TCut ctrlCJVF = cutHi + TCut("cenJetEt>30.");

      tree->Draw("met>>hTmp_CJVP_MET",          ctrlCJVP);
      tree->Draw("met>>hTmp_CJVF_MET",          ctrlCJVF);
      tree->Draw("cenJetEt>>hTmp_METP_CJEt",    ctrlMETP);
      tree->Draw("cenJetEt>>hTmp_METF_CJEt",    ctrlMETF);

      hTmp_CJVP_MET->Scale(weight);
      hTmp_CJVF_MET->Scale(weight);
      hTmp_METP_CJEt->Scale(weight);
      hTmp_METF_CJEt->Scale(weight);

      hBG_CJVP_MET->Add(hTmp_CJVP_MET, 1.);
      hBG_CJVF_MET->Add(hTmp_CJVF_MET, 1.);
      hBG_METP_CJEt->Add(hTmp_METP_CJEt, 1.);
      hBG_METF_CJEt->Add(hTmp_METF_CJEt, 1.);

//       // fake Z->nunu EWK for DY->LL
//       if (dataset.name=="DYJetsToLL_EWK") {
	
// 	// pretend this is Z->nunu EWK
// 	TCut cutSpeshLo = puWeight * trigWeight * (cuts.zMuMuGen() + cuts.zMuMuGenMass() + cutSpesh + cutLoDPhi);
// 	TCut cutSpeshHi = puWeight * trigWeight * (cuts.zMuMuGen() + cuts.zMuMuGenMass() + cutSpesh + cutHiDPhi);
	
// 	hTmp_METCJV->Reset();
// 	hTmp_HiDPhi_METCJV->Reset();

// 	tree->Draw("cenJetEt:met>>hTmp_METCJV", cutSpeshLo);
// 	tree->Draw("cenJetEt:met>>hTmp_HiDPhi_METCJV", cutSpeshHi);
	
// 	weight *= 5.626;

// 	hTmp_METCJV->Scale(weight);
// 	hTmp_HiDPhi_METCJV->Scale(weight);
	
// // 	hBG_METCJV->Add(hTmp_METCJV, 1.);
// // 	hBG_HiDPhi_METCJV->Add(hTmp_HiDPhi_METCJV, 1.);

//       }

      // print out
      std::cout << "  A = " << hTmp_METCJV->GetBinContent(1, 2) << " +/- " << hTmp_METCJV->GetBinError(1, 2) << std::endl;
      std::cout << "  B = " << hTmp_METCJV->GetBinContent(2, 2) << " +/- " << hTmp_METCJV->GetBinError(2, 2) << std::endl;
      std::cout << "  C = " << hTmp_METCJV->GetBinContent(1, 1) << " +/- " << hTmp_METCJV->GetBinError(1, 1) << std::endl;
      std::cout << "  D = " << hTmp_METCJV->GetBinContent(2, 1) << " +/- " << hTmp_METCJV->GetBinError(2, 1) << std::endl;
      std::cout << std::endl;


    }

    delete hTmp_METCJV;
    delete hTmp_HiDPhi_METCJV;

  }

  TH2D* hEst_METCJV = new TH2D("hEst_METCJV", "", 2, metEdges, 2, cjvEdges);
  hEst_METCJV->Add(hData_METCJV, hBG_METCJV, +1., -1.);

  // clear signal region
  hEst_METCJV->SetBinContent(2, 1, 0.);

  // do the ABCD calculation
  double a = hEst_METCJV->GetBinContent(1, 2);
  double b = hEst_METCJV->GetBinContent(2, 2);
  double c = hEst_METCJV->GetBinContent(1, 1);
  double d = b * c / a;

  double a_ferr = hEst_METCJV->GetBinError(1,2) / hEst_METCJV->GetBinContent(1,2);
  double b_ferr = hEst_METCJV->GetBinError(2,2) / hEst_METCJV->GetBinContent(2,2);
  double c_ferr = hEst_METCJV->GetBinError(1,1) / hEst_METCJV->GetBinContent(1,1);
  double d_ferr = sqrt( pow(a_ferr,2) + pow(b_ferr,2) + pow(c_ferr,2) );

  hEst_METCJV->SetBinContent(2, 1, d);
  hEst_METCJV->SetBinError(2, 1, d * d_ferr);

  // print out
  std::cout << "Data :" << std::endl;
  std::cout << "  A = " << hData_METCJV->GetBinContent(1, 2) << " +/- " << hData_METCJV->GetBinError(1, 2) << std::endl;
  std::cout << "  B = " << hData_METCJV->GetBinContent(2, 2) << " +/- " << hData_METCJV->GetBinError(2, 2) << std::endl;
  std::cout << "  C = " << hData_METCJV->GetBinContent(1, 1) << " +/- " << hData_METCJV->GetBinError(1, 1) << std::endl;
  std::cout << std::endl;

  std::cout << "BG :" << std::endl;
  std::cout << "  A = " << hBG_METCJV->GetBinContent(1, 2) << " +/- " << hBG_METCJV->GetBinError(1, 2) << std::endl;
  std::cout << "  B = " << hBG_METCJV->GetBinContent(2, 2) << " +/- " << hBG_METCJV->GetBinError(2, 2) << std::endl;
  std::cout << "  C = " << hBG_METCJV->GetBinContent(1, 1) << " +/- " << hBG_METCJV->GetBinError(1, 1) << std::endl;
  std::cout << "  D = " << hBG_METCJV->GetBinContent(2, 1) << " +/- " << hBG_METCJV->GetBinError(2, 1) << std::endl;
  std::cout << std::endl;

  std::cout << "Signal :" << std::endl;
  std::cout << "  A = " << hSignal_METCJV->GetBinContent(1, 2) << " +/- " << hSignal_METCJV->GetBinError(1, 2) << std::endl;
  std::cout << "  B = " << hSignal_METCJV->GetBinContent(2, 2) << " +/- " << hSignal_METCJV->GetBinError(2, 2) << std::endl;
  std::cout << "  C = " << hSignal_METCJV->GetBinContent(1, 1) << " +/- " << hSignal_METCJV->GetBinError(1, 1) << std::endl;
  std::cout << "  D = " << hSignal_METCJV->GetBinContent(2, 1) << " +/- " << hSignal_METCJV->GetBinError(2, 1) << std::endl;
  std::cout << std::endl;

  std::cout << "Data-BG :" << std::endl;
  std::cout << "  A = " << a << " +/- " << a * a_ferr << std::endl;
  std::cout << "  B = " << b << " +/- " << b * b_ferr << std::endl;
  std::cout << "  C = " << c << " +/- " << c * c_ferr << std::endl;
  std::cout << std::endl;
  std::cout << "Estimate :" << std::endl;
  std::cout << "  D = " << d << " +/- " << d * d_ferr << std::endl;
  std::cout << std::endl;


  // do the dphi>2.6 cross-check
  std::cout << std::endl;
  std::cout << "Closure test (dphi>2.6)" << std::endl;
  std::cout << std::endl;

  TH2D* hEst_HiDPhi_METCJV = new TH2D("hEst_HiDPhi_METCJV", "", 2, metEdges, 2, cjvEdges);
  hEst_HiDPhi_METCJV->Add(hData_HiDPhi_METCJV, hBG_HiDPhi_METCJV, +1., -1.);

  // do the ABCD calculation
  double a2 = hEst_HiDPhi_METCJV->GetBinContent(1, 2);
  double b2 = hEst_HiDPhi_METCJV->GetBinContent(2, 2);
  double c2 = hEst_HiDPhi_METCJV->GetBinContent(1, 1);
  double d2 = b2 * c2 / a2;

  double a2_ferr = hEst_HiDPhi_METCJV->GetBinError(1,2) / hEst_HiDPhi_METCJV->GetBinContent(1,2);
  double b2_ferr = hEst_HiDPhi_METCJV->GetBinError(2,2) / hEst_HiDPhi_METCJV->GetBinContent(2,2);
  double c2_ferr = hEst_HiDPhi_METCJV->GetBinError(1,1) / hEst_HiDPhi_METCJV->GetBinContent(1,1);
  double d2_ferr = sqrt( pow(a2_ferr,2) + pow(b2_ferr,2) + pow(c2_ferr,2) );

  // print out
  std::cout << "Data :" << std::endl;
  std::cout << "  A = " << hData_HiDPhi_METCJV->GetBinContent(1, 2) << " +/- " << hData_HiDPhi_METCJV->GetBinError(1, 2) << std::endl;
  std::cout << "  B = " << hData_HiDPhi_METCJV->GetBinContent(2, 2) << " +/- " << hData_HiDPhi_METCJV->GetBinError(2, 2) << std::endl;
  std::cout << "  C = " << hData_HiDPhi_METCJV->GetBinContent(1, 1) << " +/- " << hData_HiDPhi_METCJV->GetBinError(1, 1) << std::endl;
  std::cout << "  D = " << hData_HiDPhi_METCJV->GetBinContent(2, 1) << " +/- " << hData_HiDPhi_METCJV->GetBinError(2, 1) << std::endl;
  std::cout << std::endl;

  std::cout << "BG :" << std::endl;
  std::cout << "  A = " << hBG_HiDPhi_METCJV->GetBinContent(1, 2) << " +/- " << hBG_HiDPhi_METCJV->GetBinError(1, 2) << std::endl;
  std::cout << "  B = " << hBG_HiDPhi_METCJV->GetBinContent(2, 2) << " +/- " << hBG_HiDPhi_METCJV->GetBinError(2, 2) << std::endl;
  std::cout << "  C = " << hBG_HiDPhi_METCJV->GetBinContent(1, 1) << " +/- " << hBG_HiDPhi_METCJV->GetBinError(1, 1) << std::endl;
  std::cout << "  D = " << hBG_HiDPhi_METCJV->GetBinContent(2, 1) << " +/- " << hBG_HiDPhi_METCJV->GetBinError(2, 1) << std::endl;
  std::cout << std::endl;

  std::cout << "Signal :" << std::endl;
  std::cout << "  A = " << hSignal_HiDPhi_METCJV->GetBinContent(1, 2) << " +/- " << hSignal_HiDPhi_METCJV->GetBinError(1, 2) << std::endl;
  std::cout << "  B = " << hSignal_HiDPhi_METCJV->GetBinContent(2, 2) << " +/- " << hSignal_HiDPhi_METCJV->GetBinError(2, 2) << std::endl;
  std::cout << "  C = " << hSignal_HiDPhi_METCJV->GetBinContent(1, 1) << " +/- " << hSignal_HiDPhi_METCJV->GetBinError(1, 1) << std::endl;
  std::cout << "  D = " << hSignal_HiDPhi_METCJV->GetBinContent(2, 1) << " +/- " << hSignal_HiDPhi_METCJV->GetBinError(2, 1) << std::endl;
  std::cout << std::endl;

  std::cout << "Data-BG :" << std::endl;
  std::cout << "  A = " << hEst_HiDPhi_METCJV->GetBinContent(1, 2) << " +/- " << hEst_HiDPhi_METCJV->GetBinError(1, 2) << std::endl;
  std::cout << "  B = " << hEst_HiDPhi_METCJV->GetBinContent(2, 2) << " +/- " << hEst_HiDPhi_METCJV->GetBinError(2, 2) << std::endl;
  std::cout << "  C = " << hEst_HiDPhi_METCJV->GetBinContent(1, 1) << " +/- " << hEst_HiDPhi_METCJV->GetBinError(1, 1) << std::endl;
  std::cout << "  D = " << hEst_HiDPhi_METCJV->GetBinContent(2, 1) << " +/- " << hEst_HiDPhi_METCJV->GetBinError(2, 1) << std::endl;
  std::cout << std::endl;

  std::cout << "Estimate :" << std::endl;
  std::cout << "  D = " << d2 << " +/- " << d2 * d2_ferr << std::endl;
  std::cout << std::endl;


  // control plots
  TH1D* hEst_CJVP_MET  = new TH1D("hEst_CJVP_MET",  "", metBins, 0., metMax);
  TH1D* hEst_CJVF_MET  = new TH1D("hEst_CJVF_MET",  "", metBins, 0., metMax);
  TH1D* hEst_METP_CJEt = new TH1D("hEst_METP_CJEt", "", cjvBins, 0., cjvMax);
  TH1D* hEst_METF_CJEt = new TH1D("hEst_METF_CJEt", "", cjvBins, 0., cjvMax);

  hEst_CJVP_MET->Add(hData_CJVP_MET, hBG_CJVP_MET, 1., -.1);
  hEst_CJVF_MET->Add(hData_CJVF_MET, hBG_CJVF_MET, 1., -.1);
  hEst_METP_CJEt->Add(hData_METP_CJEt, hBG_METP_CJEt, 1., -.1);
  hEst_METF_CJEt->Add(hData_METF_CJEt, hBG_METF_CJEt, 1., -.1);

  hEst_CJVP_MET->Scale(1/hEst_CJVP_MET->Integral());
  std::cout << "hEst_CJVP_MET integral : " << hEst_CJVP_MET->Integral() << std::endl;
  hEst_CJVF_MET->Scale(1/hEst_CJVF_MET->Integral());
  std::cout << "hEst_CJVF_MET integral : " << hEst_CJVF_MET->Integral() << std::endl;

  hEst_METP_CJEt->Scale(1/hEst_METP_CJEt->Integral());
  std::cout << "hEst_METP_CJEt integral : " << hEst_METP_CJEt->Integral() << std::endl;
  hEst_METF_CJEt->Scale(1/hEst_METF_CJEt->Integral());
  std::cout << "hEst_METF_CJEt integral : " << hEst_METF_CJEt->Integral() << std::endl;


  // make ratio graphs
  TGraphAsymmErrors hEst_CJVRatio_MET(metBins);
  TGraphAsymmErrors hEst_METRatio_CJEt(cjvBins);

  for (int i=1; i<metBins+1; ++i) {

    double r1 = (hEst_CJVP_MET->GetBinContent(i) - hEst_CJVF_MET->GetBinContent(i))/hEst_CJVP_MET->GetBinContent(i);
    double e1 = r1 * sqrt(pow(hEst_CJVP_MET->GetBinError(i)/hEst_CJVP_MET->GetBinContent(i),2) +
			  pow(hEst_CJVF_MET->GetBinError(i)/hEst_CJVF_MET->GetBinContent(i),2));

    hEst_CJVRatio_MET.SetPoint(i-1, hEst_CJVP_MET->GetBinCenter(i), r1);
    hEst_CJVRatio_MET.SetPointError(i-1, 0., 0., e1, e1);
  }

  for (int i=1; i<cjvBins+1; ++i) {

    double r2 = (hEst_METP_CJEt->GetBinContent(i) - hEst_METF_CJEt->GetBinContent(i))/hEst_METP_CJEt->GetBinContent(i);
    double e2 = r2 * sqrt(pow(hEst_METP_CJEt->GetBinError(i)/hEst_METP_CJEt->GetBinContent(i),2) + 
			  pow(hEst_METF_CJEt->GetBinError(i)/hEst_METF_CJEt->GetBinContent(i),2));

    hEst_METRatio_CJEt.SetPoint(i-1, hEst_METP_CJEt->GetBinCenter(i), r2);
    hEst_METRatio_CJEt.SetPointError(i-1, 0., 0., e2, e2);

  }


  // draw control plots
  std::string pdfName;

  gStyle->SetOptStat(0);
  TCanvas canvas;
  canvas.SetCanvasSize(canvas.GetWindowWidth(), 1.2*canvas.GetWindowHeight());

//   canvas.Divide(1,2);

//   TPad pad1("pad1", "", 0., 0.3, 1.0, 1.0);
//   pad1.SetBottomMargin(0.01);
//   pad1.Draw();

//   TPad pad2("pad2", "", 0., 0.0, 1.0, 0.3);
//   pad2.SetTopMargin(1);
//   pad2.SetBottomMargin(0.33);
//   pad2.Draw();
  
  // MET
  //  pad1.cd();

  hEst_CJVP_MET->SetMarkerColor(kRed);
  hEst_CJVP_MET->SetMarkerStyle(22);

  hEst_CJVF_MET->SetMarkerColor(kBlue);
  hEst_CJVF_MET->SetMarkerStyle(23);

  hEst_CJVP_MET->GetYaxis()->SetTitle("arbitrary units");
  hEst_CJVP_MET->GetXaxis()->SetTitle("MET [GeV]");

  hEst_CJVP_MET->Draw();
  hEst_CJVF_MET->Draw("SAME");

  TLegend leg(0.60, 0.7, 0.75, 0.85, "#Delta #phi > 2.6", "NDC");
  leg.AddEntry(hEst_CJVP_MET, "CJV Pass", "P");
  leg.AddEntry(hEst_CJVF_MET, "CJV Fail", "P");
  leg.SetFillColor(0);
  leg.Draw();
  
  pdfName= oDir+std::string("/QCD3_Ctrl_MET.pdf");
  canvas.Print(pdfName.c_str());

  //  pad2.cd();
  
  hEst_CJVRatio_MET.SetMaximum(1.0);
  hEst_CJVRatio_MET.SetMinimum(-1.0);
  hEst_CJVRatio_MET.SetMarkerStyle(8);
  hEst_CJVRatio_MET.SetTitle("");
  hEst_CJVRatio_MET.GetXaxis()->SetTitle("MET [GeV]");
  hEst_CJVRatio_MET.GetYaxis()->SetTitle("(f_{pass CJV} - f_{fail CJV})/f_{pass CJV}");

  hEst_CJVRatio_MET.Draw("AP");
  

  pdfName = oDir+std::string("/QCD3_CtrlRatio_MET.pdf");
  canvas.Print(pdfName.c_str());

  // CJV
  //  pad1.cd();

  hEst_METP_CJEt->SetMarkerColor(kRed);
  hEst_METP_CJEt->SetMarkerStyle(22);

  hEst_METF_CJEt->SetMarkerColor(kBlue);
  hEst_METF_CJEt->SetMarkerStyle(23);

  hEst_METP_CJEt->GetYaxis()->SetTitle("arbitrary units");
  hEst_METP_CJEt->GetXaxis()->SetTitle("cen jet E_{T} [GeV]");

  hEst_METP_CJEt->Draw();
  hEst_METF_CJEt->Draw("SAME");

  TLegend leg2(0.60, 0.7, 0.75, 0.85, "#Delta #phi > 2.6", "NDC");
  leg2.AddEntry(hEst_METP_CJEt, "MET > 130", "P");
  leg2.AddEntry(hEst_METF_CJEt, "MET < 130", "P");
  leg2.SetFillColor(0);
  leg2.Draw();

  pdfName = oDir+std::string("/QCD3_Ctrl_CenJetEt.pdf");
  canvas.Print(pdfName.c_str());

  //  pad2.cd();

  hEst_METRatio_CJEt.SetMaximum(1.5);
  hEst_METRatio_CJEt.SetMinimum(-1.5);
  hEst_METRatio_CJEt.SetMarkerStyle(8);
  hEst_METRatio_CJEt.SetTitle("");
  hEst_METRatio_CJEt.GetXaxis()->SetTitle("cen jet E_{T} [GeV]");
  hEst_METRatio_CJEt.GetYaxis()->SetTitle("(f_{pass MET}-f_{fail MET})/f_{pass MET}");

  hEst_METRatio_CJEt.Draw("AP");
  
  pdfName = oDir+std::string("/QCD3_CtrlRatio_CenJetEt.pdf");
  canvas.Print(pdfName.c_str());

  // save output
  ofile->cd();
  hData_METCJV->Write("", TObject::kOverwrite);
  hBG_METCJV->Write("", TObject::kOverwrite);
  hSignal_METCJV->Write("", TObject::kOverwrite);
  hEst_METCJV->Write("", TObject::kOverwrite);

  hData_HiDPhi_METCJV->Write("", TObject::kOverwrite);
  hBG_HiDPhi_METCJV->Write("", TObject::kOverwrite);
  hSignal_HiDPhi_METCJV->Write("", TObject::kOverwrite);
  hEst_HiDPhi_METCJV->Write("", TObject::kOverwrite);

  hData_CJVP_MET->Write("", TObject::kOverwrite);
  hBG_CJVP_MET->Write("", TObject::kOverwrite);
  hEst_CJVP_MET->Write("", TObject::kOverwrite);

  hData_CJVF_MET->Write("", TObject::kOverwrite);
  hBG_CJVF_MET->Write("", TObject::kOverwrite);
  hEst_CJVF_MET->Write("", TObject::kOverwrite);

  hData_METP_CJEt->Write("", TObject::kOverwrite);
  hBG_METP_CJEt->Write("", TObject::kOverwrite);
  hEst_METP_CJEt->Write("", TObject::kOverwrite);

  hData_METF_CJEt->Write("", TObject::kOverwrite);
  hBG_METF_CJEt->Write("", TObject::kOverwrite);
  hEst_METF_CJEt->Write("", TObject::kOverwrite);

  hEst_CJVRatio_MET.Write("", TObject::kOverwrite);
  hEst_METRatio_CJEt.Write("", TObject::kOverwrite);

  // clean up
  ofile->Close();

}
