
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
  TCut cutSpesh = //cuts.zMuMuGenMass() +
    cuts.cut("trigger")+
    cuts.cut("metFilter")+
    //cuts.cut("EVeto")+
    cuts.cut("dijet")+
    cuts.cut("sgnEtaJJ")+
    cuts.cut("dEtaJJ")+
    cuts.cut("Mjj");
  TCut cutLoDPhi = cuts.cut("dPhiJJ"); 
  TCut cutHiDPhi("vbfDPhi>2.6"); 

  TCut puWeight("puWeight");
  TCut trigWeight( "(trigCorrWeight>0.) ? trigCorrWeight : 1." );

  // For lepton weights
  TCut elVetoWeight = cuts.elVetoWeight(options.leptCorr);
  TCut muVetoWeight = cuts.muVetoWeight(options.leptCorr);

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
  TH1D* hDataHi_CJVP_MET  = new TH1D("hDataHi_CJVP_MET",  "", metBins, 0., metMax);
  TH1D* hBGHi_CJVP_MET    = new TH1D("hBGHi_CJVP_MET",    "", metBins, 0., metMax);
  TH1D* hDataHi_CJVF_MET  = new TH1D("hDataHi_CJVF_MET",  "", metBins, 0., metMax);
  TH1D* hBGHi_CJVF_MET    = new TH1D("hBGHi_CJVF_MET",    "", metBins, 0., metMax);

  TH1D* hDataHi_METP_CJEt = new TH1D("hDataHi_METP_CJEt", "", cjvBins, 0., cjvMax);
  TH1D* hBGHi_METP_CJEt   = new TH1D("hBGHi_METP_CJEt",   "", cjvBins, 0., cjvMax);
  TH1D* hDataHi_METF_CJEt = new TH1D("hDataHi_METF_CJEt", "", cjvBins, 0., cjvMax);
  TH1D* hBGHi_METF_CJEt   = new TH1D("hBGHi_METF_CJEt",   "", cjvBins, 0., cjvMax);

  TH1D* hDataLo_CJVP_MET  = new TH1D("hDataLo_CJVP_MET",  "", 13, 0., 130.);
  TH1D* hBGLo_CJVP_MET    = new TH1D("hBGLo_CJVP_MET",    "", 13, 0., 130.);
  TH1D* hDataLo_CJVF_MET  = new TH1D("hDataLo_CJVF_MET",  "", 13, 0., 130.);
  TH1D* hBGLo_CJVF_MET    = new TH1D("hBGLo_CJVF_MET",    "", 13, 0., 130.);

  TH1D* hDataLo_METP_CJEt = new TH1D("hDataLo_METP_CJEt", "", 17, 30., 200.);
  TH1D* hBGLo_METP_CJEt   = new TH1D("hBGLo_METP_CJEt",   "", 17, 30., 200.);
  TH1D* hDataLo_METF_CJEt = new TH1D("hDataLo_METF_CJEt", "", 17, 30., 200.);
  TH1D* hBGLo_METF_CJEt   = new TH1D("hBGLo_METF_CJEt",   "", 17, 30., 200.);

  // loop over MC datasets
  for (unsigned i=0; i<datasets.size(); ++i) {

    Dataset dataset = datasets.getDataset(i);
    TFile* file = datasets.getTFile(dataset.name);
    TTree* tree = (TTree*) file->Get("invHiggsInfo/InvHiggsInfo");

    TCut cutD = cuts.cutDataset(dataset.name);

    TH2D* hTmp_METCJV        = new TH2D("hTmp_METCJV", "", 2, metEdges, 2, cjvEdges);
    TH2D* hTmp_HiDPhi_METCJV = new TH2D("hTmp_HiDPhi_METCJV", "", 2, metEdges, 2, cjvEdges);
    TH1D* hTmp_ZGen	     = new TH1D("hTmp_ZGen", "", 1, 0, 1);

    TH1D* hTmpHi_CJVP_MET  = new TH1D("hTmpHi_CJVP_MET",  "", metBins, 0., metMax);
    TH1D* hTmpHi_CJVF_MET  = new TH1D("hTmpHi_CJVF_MET",  "", metBins, 0., metMax);    
    TH1D* hTmpHi_METP_CJEt = new TH1D("hTmpHi_METP_CJEt", "", cjvBins, 0., cjvMax);
    TH1D* hTmpHi_METF_CJEt = new TH1D("hTmpHi_METF_CJEt", "", cjvBins, 0., cjvMax);

    TH1D* hTmpLo_CJVP_MET  = new TH1D("hTmpLo_CJVP_MET",  "", 13, 0., 130.);
    TH1D* hTmpLo_CJVF_MET  = new TH1D("hTmpLo_CJVF_MET",  "", 13, 0., 130.);
    TH1D* hTmpLo_METP_CJEt = new TH1D("hTmpLo_METP_CJEt", "", 17, 30., 200.);
    TH1D* hTmpLo_METF_CJEt = new TH1D("hTmpLo_METF_CJEt", "", 17, 30., 200.);
   
    double weight = (dataset.isData ? 1. : lumi * dataset.sigma / dataset.nEvents);
    //if(dataset.name == "EWK_ZvvFake") weight *=  (constants::sigma_Zvv_EWK/constants::sigma_Zuu_EWK);

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
      TCut ctrlHiMETP = cutHi + TCut("met>130");
      TCut ctrlHiMETF = cutHi + TCut("met>50&&met<130");
      TCut ctrlHiCJVP = cutHi + TCut("cenJetEt<30.");
      TCut ctrlHiCJVF = cutHi + TCut("cenJetEt>30.");

      tree->Draw("met>>hTmpHi_CJVP_MET",          ctrlHiCJVP);
      tree->Draw("met>>hTmpHi_CJVF_MET",          ctrlHiCJVF);
      tree->Draw("cenJetEt>>hTmpHi_METP_CJEt",    ctrlHiMETP);
      tree->Draw("cenJetEt>>hTmpHi_METF_CJEt",    ctrlHiMETF);

      hDataHi_CJVP_MET->Add(hTmpHi_CJVP_MET, 1.);
      hDataHi_CJVF_MET->Add(hTmpHi_CJVF_MET, 1.);
      hDataHi_METP_CJEt->Add(hTmpHi_METP_CJEt, 1.);
      hDataHi_METF_CJEt->Add(hTmpHi_METF_CJEt, 1.);

      TCut ctrlLoMETP = cutLo + TCut("met>130");
      TCut ctrlLoMETF = cutLo + TCut("met<130");
      TCut ctrlLoCJVP = cutLo + TCut("cenJetEt<30.");
      TCut ctrlLoCJVF = cutLo + TCut("cenJetEt>30.");

      tree->Draw("met>>hTmpLo_CJVP_MET",          ctrlLoCJVP);
      tree->Draw("met>>hTmpLo_CJVF_MET",          ctrlLoCJVF);
      tree->Draw("cenJetEt>>hTmpLo_METP_CJEt",    ctrlLoMETP);
      tree->Draw("cenJetEt>>hTmpLo_METF_CJEt",    ctrlLoMETF);

      hDataLo_CJVP_MET->Add(hTmpLo_CJVP_MET, 1.);
      hDataLo_CJVF_MET->Add(hTmpLo_CJVF_MET, 1.);
      hDataLo_METP_CJEt->Add(hTmpLo_METP_CJEt, 1.);
      hDataLo_METF_CJEt->Add(hTmpLo_METF_CJEt, 1.);

    }
    else if (dataset.name=="SignalM125_POWHEG") {
      std::cout << "Analysing Signal MC        : " << dataset.name << std::endl;
      
      TCut cutLo = puWeight * elVetoWeight * muVetoWeight * (cutBase + cutLoDPhi);
      TCut cutHi = puWeight * elVetoWeight * muVetoWeight * (cutBase + cutHiDPhi);

      tree->Draw("cenJetEt:met>>hTmp_METCJV",        cutLo);
      tree->Draw("cenJetEt:met>>hTmp_HiDPhi_METCJV", cutHi);

      hTmp_METCJV->Scale(weight);
      hTmp_HiDPhi_METCJV->Scale(weight);

      hSignal_METCJV->Add(hTmp_METCJV, 1.);
      hSignal_HiDPhi_METCJV->Add(hTmp_HiDPhi_METCJV, 1.);

    }
    else {
      std::cout << "Analysing BG MC        : " << dataset.name << std::endl;

      TCut cutGen("");      
      TCut cutLo(""); 
      TCut cutHi("");

      if (dataset.name == "EWK_ZvvFake") {
	cutGen = puWeight * cutD; 
	cutLo  = puWeight * trigWeight * (cutD + cutSpesh + cutLoDPhi);
	cutHi  = puWeight * trigWeight * (cutD + cutSpesh + cutHiDPhi);

        tree->Draw("cenJetEt:metNoMuon>>hTmp_METCJV",        cutLo);
        tree->Draw("cenJetEt:metNoMuon>>hTmp_HiDPhi_METCJV", cutHi);
 	tree->Draw("0.5>>hTmp_ZGen", cutGen);

	hTmp_ZGen->Scale(weight);
      }
      else {
	cutLo = puWeight * trigWeight * elVetoWeight * muVetoWeight * (cutD + cutBase + cutLoDPhi);
	cutHi = puWeight * trigWeight * elVetoWeight * muVetoWeight * (cutD + cutBase + cutHiDPhi); 

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
      }

      if (dataset.name == "EWK_ZvvFake") weight *= ((lumi/1000.) * constants::sigma_Zvv_EWK)/hTmp_ZGen->GetBinContent(1); 

      hTmp_METCJV->Scale(weight);
      hTmp_HiDPhi_METCJV->Scale(weight);

      hBG_METCJV->Add(hTmp_METCJV, 1.);
      hBG_HiDPhi_METCJV->Add(hTmp_HiDPhi_METCJV, 1.);

      // control plots
      TCut ctrlHiMETP(""); 
      TCut ctrlHiMETF(""); 
      if (dataset.name == "EWK_ZvvFake") { 
	ctrlHiMETP = cutHi + TCut("metNoMuon>130");
	ctrlHiMETF = cutHi + TCut("metNoMuon>50&&metNoMuon<130");
      }
      else {
	ctrlHiMETP = cutHi + TCut("met>130");
	ctrlHiMETF = cutHi + TCut("met>50&&met<130");
      }
      TCut ctrlHiCJVP = cutHi + TCut("cenJetEt<30.");
      TCut ctrlHiCJVF = cutHi + TCut("cenJetEt>30.");

      tree->Draw("met>>hTmpHi_CJVP_MET",          ctrlHiCJVP);
      tree->Draw("met>>hTmpHi_CJVF_MET",          ctrlHiCJVF);
      tree->Draw("cenJetEt>>hTmpHi_METP_CJEt",    ctrlHiMETP);
      tree->Draw("cenJetEt>>hTmpHi_METF_CJEt",    ctrlHiMETF);

      hTmpHi_CJVP_MET->Scale(weight);
      hTmpHi_CJVF_MET->Scale(weight);
      hTmpHi_METP_CJEt->Scale(weight);
      hTmpHi_METF_CJEt->Scale(weight);

      hBGHi_CJVP_MET->Add(hTmpHi_CJVP_MET, 1.);
      hBGHi_CJVF_MET->Add(hTmpHi_CJVF_MET, 1.);
      hBGHi_METP_CJEt->Add(hTmpHi_METP_CJEt, 1.);
      hBGHi_METF_CJEt->Add(hTmpHi_METF_CJEt, 1.);

      TCut ctrlLoMETP("");
      TCut ctrlLoMETF(""); 
      if (dataset.name == "EWK_ZvvFake") {
        ctrlHiMETP = cutLo + TCut("metNoMuon>130");
        ctrlHiMETF = cutLo + TCut("metNoMuon>50&&metNoMuon<130");
      }
      else {
        ctrlHiMETP = cutLo + TCut("met>130");
        ctrlHiMETF = cutLo + TCut("met>50&&met<130");
      }
      TCut ctrlLoCJVP = cutLo + TCut("cenJetEt<30.");
      TCut ctrlLoCJVF = cutLo + TCut("cenJetEt>30.");

      tree->Draw("met>>hTmpLo_CJVP_MET",          ctrlLoCJVP);
      tree->Draw("met>>hTmpLo_CJVF_MET",          ctrlLoCJVF);
      tree->Draw("cenJetEt>>hTmpLo_METP_CJEt",    ctrlLoMETP);
      tree->Draw("cenJetEt>>hTmpLo_METF_CJEt",    ctrlLoMETF);

      hTmpLo_CJVP_MET->Scale(weight);
      hTmpLo_CJVF_MET->Scale(weight);
      hTmpLo_METP_CJEt->Scale(weight);
      hTmpLo_METF_CJEt->Scale(weight);

      hBGLo_CJVP_MET->Add(hTmpLo_CJVP_MET, 1.);
      hBGLo_CJVF_MET->Add(hTmpLo_CJVF_MET, 1.);
      hBGLo_METP_CJEt->Add(hTmpLo_METP_CJEt, 1.);
      hBGLo_METF_CJEt->Add(hTmpLo_METF_CJEt, 1.);

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
  TH1D* hEstHi_CJVP_MET  = new TH1D("hEstHi_CJVP_MET",  "", metBins, 0., metMax);
  TH1D* hEstHi_CJVF_MET  = new TH1D("hEstHi_CJVF_MET",  "", metBins, 0., metMax);
  TH1D* hEstHi_METP_CJEt = new TH1D("hEstHi_METP_CJEt", "", cjvBins, 0., cjvMax);
  TH1D* hEstHi_METF_CJEt = new TH1D("hEstHi_METF_CJEt", "", cjvBins, 0., cjvMax);

  hEstHi_CJVP_MET->Add(hDataHi_CJVP_MET, hBGHi_CJVP_MET, 1., -.1);
  hEstHi_CJVF_MET->Add(hDataHi_CJVF_MET, hBGHi_CJVF_MET, 1., -.1);
  hEstHi_METP_CJEt->Add(hDataHi_METP_CJEt, hBGHi_METP_CJEt, 1., -.1);
  hEstHi_METF_CJEt->Add(hDataHi_METF_CJEt, hBGHi_METF_CJEt, 1., -.1);

  hEstHi_CJVP_MET->Scale(1/hEstHi_CJVP_MET->Integral());
  std::cout << "hEst_CJVP_MET integral : " << hEstHi_CJVP_MET->Integral() << std::endl;
  hEstHi_CJVF_MET->Scale(1/hEstHi_CJVF_MET->Integral());
  std::cout << "hEst_CJVF_MET integral : " << hEstHi_CJVF_MET->Integral() << std::endl;

  hEstHi_METP_CJEt->Scale(1/hEstHi_METP_CJEt->Integral());
  std::cout << "hEst_METP_CJEt integral : " << hEstHi_METP_CJEt->Integral() << std::endl;
  hEstHi_METF_CJEt->Scale(1/hEstHi_METF_CJEt->Integral());
  std::cout << "hEst_METF_CJEt integral : " << hEstHi_METF_CJEt->Integral() << std::endl;

  TH1D* hEstLo_CJVP_MET  = new TH1D("hEstLo_CJVP_MET",  "", 13, 0., 130.);
  TH1D* hEstLo_CJVF_MET  = new TH1D("hEstLo_CJVF_MET",  "", 13, 0., 130.);
  TH1D* hEstLo_METP_CJEt = new TH1D("hEstLo_METP_CJEt", "", 17, 30., 200.);
  TH1D* hEstLo_METF_CJEt = new TH1D("hEstLo_METF_CJEt", "", 17, 30., 200.);

  hEstLo_CJVP_MET->Add(hDataLo_CJVP_MET, hBGLo_CJVP_MET, 1., -.1);
  hEstLo_CJVF_MET->Add(hDataLo_CJVF_MET, hBGLo_CJVF_MET, 1., -.1);
  hEstLo_METP_CJEt->Add(hDataLo_METP_CJEt, hBGLo_METP_CJEt, 1., -.1);
  hEstLo_METF_CJEt->Add(hDataLo_METF_CJEt, hBGLo_METF_CJEt, 1., -.1);

  hEstLo_CJVP_MET->Scale(1/hEstLo_CJVP_MET->Integral());
  std::cout << "hEst_CJVP_MET integral : " << hEstLo_CJVP_MET->Integral() << std::endl;
  hEstLo_CJVF_MET->Scale(1/hEstLo_CJVF_MET->Integral());
  std::cout << "hEst_CJVF_MET integral : " << hEstLo_CJVF_MET->Integral() << std::endl;

  hEstLo_METP_CJEt->Scale(1/hEstLo_METP_CJEt->Integral());
  std::cout << "hEst_METP_CJEt integral : " << hEstLo_METP_CJEt->Integral() << std::endl;
  hEstLo_METF_CJEt->Scale(1/hEstLo_METF_CJEt->Integral());
  std::cout << "hEst_METF_CJEt integral : " << hEstLo_METF_CJEt->Integral() << std::endl;

  // make ratio graphs
  TGraphAsymmErrors hEstHi_CJVRatio_MET(metBins);
  TGraphAsymmErrors hEstHi_METRatio_CJEt(cjvBins);

  for (int i=1; i<metBins+1; ++i) {

    double r1 = (hEstHi_CJVP_MET->GetBinContent(i) - hEstHi_CJVF_MET->GetBinContent(i))/hEstHi_CJVP_MET->GetBinContent(i);
    double e1 = r1 * sqrt(pow(hEstHi_CJVP_MET->GetBinError(i)/hEstHi_CJVP_MET->GetBinContent(i),2) +
			  pow(hEstHi_CJVF_MET->GetBinError(i)/hEstHi_CJVF_MET->GetBinContent(i),2));

    hEstHi_CJVRatio_MET.SetPoint(i-1, hEstHi_CJVP_MET->GetBinCenter(i), r1);
    hEstHi_CJVRatio_MET.SetPointError(i-1, 0., 0., e1, e1);
  }

  for (int i=1; i<cjvBins+1; ++i) {

    double r2 = (hEstHi_METP_CJEt->GetBinContent(i) - hEstHi_METF_CJEt->GetBinContent(i))/hEstHi_METP_CJEt->GetBinContent(i);
    double e2 = r2 * sqrt(pow(hEstHi_METP_CJEt->GetBinError(i)/hEstHi_METP_CJEt->GetBinContent(i),2) + 
			  pow(hEstHi_METF_CJEt->GetBinError(i)/hEstHi_METF_CJEt->GetBinContent(i),2));

    hEstHi_METRatio_CJEt.SetPoint(i-1, hEstHi_METP_CJEt->GetBinCenter(i), r2);
    hEstHi_METRatio_CJEt.SetPointError(i-1, 0., 0., e2, e2);

  }

  TGraphAsymmErrors hEstLo_CJVRatio_MET(13);
  TGraphAsymmErrors hEstLo_METRatio_CJEt(17);

  for (int i=1; i<14; ++i) {

    double r1 = (hEstLo_CJVP_MET->GetBinContent(i) - hEstLo_CJVF_MET->GetBinContent(i))/hEstLo_CJVP_MET->GetBinContent(i);
    double e1 = r1 * sqrt(pow(hEstLo_CJVP_MET->GetBinError(i)/hEstLo_CJVP_MET->GetBinContent(i),2) +
			  pow(hEstLo_CJVF_MET->GetBinError(i)/hEstLo_CJVF_MET->GetBinContent(i),2));

    hEstLo_CJVRatio_MET.SetPoint(i-1, hEstLo_CJVP_MET->GetBinCenter(i), r1);
    hEstLo_CJVRatio_MET.SetPointError(i-1, 0., 0., e1, e1);
  }

  for (int i=1; i<18; ++i) {

    double r2 = (hEstLo_METP_CJEt->GetBinContent(i) - hEstLo_METF_CJEt->GetBinContent(i))/hEstLo_METP_CJEt->GetBinContent(i);
    double e2 = r2 * sqrt(pow(hEstLo_METP_CJEt->GetBinError(i)/hEstLo_METP_CJEt->GetBinContent(i),2) + 
			  pow(hEstLo_METF_CJEt->GetBinError(i)/hEstLo_METF_CJEt->GetBinContent(i),2));

    hEstLo_METRatio_CJEt.SetPoint(i-1, hEstLo_METP_CJEt->GetBinCenter(i), r2);
    hEstLo_METRatio_CJEt.SetPointError(i-1, 0., 0., e2, e2);

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

  hEstHi_CJVP_MET->SetMarkerColor(kRed);
  hEstHi_CJVP_MET->SetMarkerStyle(22);

  hEstHi_CJVF_MET->SetMarkerColor(kBlue);
  hEstHi_CJVF_MET->SetMarkerStyle(23);

  hEstHi_CJVP_MET->GetYaxis()->SetTitle("arbitrary units");
  hEstHi_CJVP_MET->GetXaxis()->SetTitle("MET [GeV]");

  hEstHi_CJVP_MET->Draw();
  hEstHi_CJVF_MET->Draw("SAME");

  TLegend leg(0.60, 0.7, 0.75, 0.85, "#Delta#phi>2.6", "NDC");
  leg.AddEntry(hEstHi_CJVP_MET, "CJV Pass", "P");
  leg.AddEntry(hEstHi_CJVF_MET, "CJV Fail", "P");
  leg.SetFillColor(0);
  leg.Draw();
  
  pdfName= oDir+std::string("/QCD3_CtrlHi_MET.pdf");
  canvas.Print(pdfName.c_str());

  //  pad2.cd();
  
  hEstHi_CJVRatio_MET.SetMaximum(1.0);
  hEstHi_CJVRatio_MET.SetMinimum(-1.0);
  hEstHi_CJVRatio_MET.SetMarkerStyle(8);
  hEstHi_CJVRatio_MET.SetTitle("");
  hEstHi_CJVRatio_MET.GetXaxis()->SetTitle("MET [GeV]");
  hEstHi_CJVRatio_MET.GetYaxis()->SetTitle("(f_{pass CJV} - f_{fail CJV})/f_{pass CJV}");

  hEstHi_CJVRatio_MET.Draw("AP");
  

  pdfName = oDir+std::string("/QCD3_CtrlHi_MET_Ratio.pdf");
  canvas.Print(pdfName.c_str());

  // CJV
  //  pad1.cd();

  hEstHi_METP_CJEt->SetMarkerColor(kRed);
  hEstHi_METP_CJEt->SetMarkerStyle(22);

  hEstHi_METF_CJEt->SetMarkerColor(kBlue);
  hEstHi_METF_CJEt->SetMarkerStyle(23);

  hEstHi_METP_CJEt->GetYaxis()->SetTitle("arbitrary units");
  hEstHi_METP_CJEt->GetXaxis()->SetTitle("cen jet E_{T} [GeV]");

  hEstHi_METP_CJEt->Draw();
  hEstHi_METF_CJEt->Draw("SAME");

  TLegend leg2(0.60, 0.7, 0.75, 0.85, "#Delta#phi>2.6", "NDC");
  leg2.AddEntry(hEstHi_METP_CJEt, "MET > 130", "P");
  leg2.AddEntry(hEstHi_METF_CJEt, "MET < 130", "P");
  leg2.SetFillColor(0);
  leg2.Draw();

  pdfName = oDir+std::string("/QCD3_CtrlHi_CenJetEt.pdf");
  canvas.Print(pdfName.c_str());

  //  pad2.cd();

  hEstHi_METRatio_CJEt.SetMaximum(1.5);
  hEstHi_METRatio_CJEt.SetMinimum(-1.5);
  hEstHi_METRatio_CJEt.SetMarkerStyle(8);
  hEstHi_METRatio_CJEt.SetTitle("");
  hEstHi_METRatio_CJEt.GetXaxis()->SetTitle("cen jet E_{T} [GeV]");
  hEstHi_METRatio_CJEt.GetYaxis()->SetTitle("(f_{pass MET}-f_{fail MET})/f_{pass MET}");

  hEstHi_METRatio_CJEt.Draw("AP");
  
  pdfName = oDir+std::string("/QCD3_CtrlHi_CenJetEt_Ratio.pdf");
  canvas.Print(pdfName.c_str());


  // MET dphi<1.0
  //  pad1.cd();

  hEstLo_CJVP_MET->SetMarkerColor(kRed);
  hEstLo_CJVP_MET->SetMarkerStyle(22);

  hEstLo_CJVF_MET->SetMarkerColor(kBlue);
  hEstLo_CJVF_MET->SetMarkerStyle(23);

  hEstLo_CJVP_MET->GetYaxis()->SetTitle("arbitrary units");
  hEstLo_CJVP_MET->GetXaxis()->SetTitle("MET [GeV]");

  hEstLo_CJVP_MET->Draw();
  hEstLo_CJVF_MET->Draw("SAME");

  TLegend leg3(0.60, 0.7, 0.75, 0.85, "#Delta#phi<1.0", "NDC");
  leg3.AddEntry(hEstLo_CJVP_MET, "CJV Pass", "P");
  leg3.AddEntry(hEstLo_CJVF_MET, "CJV Fail", "P");
  leg3.SetFillColor(0);
  leg3.Draw();
  
  pdfName= oDir+std::string("/QCD3_CtrlLo_MET.pdf");
  canvas.Print(pdfName.c_str());

  //  pad2.cd();
  
  hEstLo_CJVRatio_MET.SetMaximum(1.0);
  hEstLo_CJVRatio_MET.SetMinimum(-1.0);
  hEstLo_CJVRatio_MET.SetMarkerStyle(8);
  hEstLo_CJVRatio_MET.SetTitle("");
  hEstLo_CJVRatio_MET.GetXaxis()->SetTitle("MET [GeV]");
  hEstLo_CJVRatio_MET.GetYaxis()->SetTitle("(f_{pass CJV} - f_{fail CJV})/f_{pass CJV}");

  hEstLo_CJVRatio_MET.Draw("AP");
  

  pdfName = oDir+std::string("/QCD3_CtrlLo_MET_Ratio.pdf");
  canvas.Print(pdfName.c_str());

  // CJV
  //  pad1.cd();

  hEstLo_METP_CJEt->SetMarkerColor(kRed);
  hEstLo_METP_CJEt->SetMarkerStyle(22);

  hEstLo_METF_CJEt->SetMarkerColor(kBlue);
  hEstLo_METF_CJEt->SetMarkerStyle(23);

  hEstLo_METP_CJEt->GetYaxis()->SetTitle("arbitrary units");
  hEstLo_METP_CJEt->GetXaxis()->SetTitle("cen jet E_{T} [GeV]");

  hEstLo_METP_CJEt->Draw();
  hEstLo_METF_CJEt->Draw("SAME");

  TLegend leg4(0.60, 0.7, 0.75, 0.85, "#Delta#phi<1.0", "NDC");
  leg4.AddEntry(hEstLo_METP_CJEt, "MET > 130", "P");
  leg4.AddEntry(hEstLo_METF_CJEt, "MET < 130", "P");
  leg4.SetFillColor(0);
  leg4.Draw();

  pdfName = oDir+std::string("/QCD3_CtrlLo_CenJetEt.pdf");
  canvas.Print(pdfName.c_str());

  //  pad2.cd();

  hEstLo_METRatio_CJEt.SetMaximum(5.0);
  hEstLo_METRatio_CJEt.SetMinimum(-5.0);
  hEstLo_METRatio_CJEt.SetMarkerStyle(8);
  hEstLo_METRatio_CJEt.SetTitle("");
  hEstLo_METRatio_CJEt.GetXaxis()->SetTitle("cen jet E_{T} [GeV]");
  hEstLo_METRatio_CJEt.GetYaxis()->SetTitle("(f_{pass MET}-f_{fail MET})/f_{pass MET}");

  hEstLo_METRatio_CJEt.Draw("AP");
  
  pdfName = oDir+std::string("/QCD3_CtrlLo_CenJetEt_Ratio.pdf");
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

  hDataHi_CJVP_MET->Write("", TObject::kOverwrite);
  hBGHi_CJVP_MET->Write("", TObject::kOverwrite);
  hEstHi_CJVP_MET->Write("", TObject::kOverwrite);

  hDataHi_CJVF_MET->Write("", TObject::kOverwrite);
  hBGHi_CJVF_MET->Write("", TObject::kOverwrite);
  hEstHi_CJVF_MET->Write("", TObject::kOverwrite);

  hDataHi_METP_CJEt->Write("", TObject::kOverwrite);
  hBGHi_METP_CJEt->Write("", TObject::kOverwrite);
  hEstHi_METP_CJEt->Write("", TObject::kOverwrite);

  hDataHi_METF_CJEt->Write("", TObject::kOverwrite);
  hBGHi_METF_CJEt->Write("", TObject::kOverwrite);
  hEstHi_METF_CJEt->Write("", TObject::kOverwrite);

  hEstHi_CJVRatio_MET.Write("", TObject::kOverwrite);
  hEstHi_METRatio_CJEt.Write("", TObject::kOverwrite);

  hDataLo_CJVP_MET->Write("", TObject::kOverwrite);
  hBGLo_CJVP_MET->Write("", TObject::kOverwrite);
  hEstLo_CJVP_MET->Write("", TObject::kOverwrite);

  hDataLo_CJVF_MET->Write("", TObject::kOverwrite);
  hBGLo_CJVF_MET->Write("", TObject::kOverwrite);
  hEstLo_CJVF_MET->Write("", TObject::kOverwrite);

  hDataLo_METP_CJEt->Write("", TObject::kOverwrite);
  hBGLo_METP_CJEt->Write("", TObject::kOverwrite);
  hEstLo_METP_CJEt->Write("", TObject::kOverwrite);

  hDataLo_METF_CJEt->Write("", TObject::kOverwrite);
  hBGLo_METF_CJEt->Write("", TObject::kOverwrite);
  hEstLo_METF_CJEt->Write("", TObject::kOverwrite);

  hEstLo_CJVRatio_MET.Write("", TObject::kOverwrite);
  hEstLo_METRatio_CJEt.Write("", TObject::kOverwrite);


  // clean up
  ofile->Close();

}
