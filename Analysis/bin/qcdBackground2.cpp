
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

  // input datasets
  Datasets datasets(options.iDir);
  datasets.readFile(options.datasetFile);

  std::string oDir_Plot = options.oDir+std::string("/QCDBackground2");

  boost::filesystem::path opath(oDir_Plot);
  if (!exists(opath)) {
    std::cout << "Creating output directory : " << oDir_Plot << std::endl;
    boost::filesystem::create_directory(opath);
  }

  // output file
  TFile* ofile = TFile::Open( (options.oDir+std::string("/QCDBackground2.root")).c_str(), "UPDATE");

  // cuts
  Cuts cuts;
  TCut puWeight("puWeight");
  TCut trigCorrWeight( "(trigCorrWeight>0) ? trigCorrWeight : 1." );
  TCut wWeight = cuts.wWeight();

  TCut Dphi_sig("vbfDPhi<1.0");
  TCut Dphi_qcd("vbfDPhi>2.6");
  TCut MET("met<130.");

  // histograms
  double metEdges_S[3]  = { 0., 130., 2000. };
  double metEdges_C[3]  = { 0., 120., 2000. }; 	//shift by 10 GeV in control region

  TH1D* hQCD_Data_HiDPhi_MET  = new TH1D("hQCD_Data_HiDPhi_MET", "", 2, metEdges_C);
  TH1D* hQCD_Data_LowDPhi_MET = new TH1D("hQCD_Data_LowDPhi_MET", "", 2, metEdges_S);
  TH1D* hQCD_Data_HiDPhi_METshape  = new TH1D("hQCD_Data_HiDPhi_METshape", "", 60, 0., 300.);
  TH1D* hQCD_Data_LowDPhi_METshape = new TH1D("hQCD_Data_LowDPhi_METshape", "", 60, 0., 300.); 

  TH1D* hQCD_BG_HiDPhi_MET  = new TH1D("hQCD_BG_HiDPhi_MET", "", 2, metEdges_C); 	//EW BG subtract in QCD region
  TH1D* hQCD_BG_LowDPhi_MET = new TH1D("hQCD_BG_LowDPhi_MET", "", 2, metEdges_S);  	//EW BG subtract in signal region
  TH1D* hQCD_BG_HiDPhi_METshape   = new TH1D("hQCD_BG_HiDPhi_METshape", "", 60, 0., 300.);
  TH1D* hQCD_BG_LowDPhi_METshape  = new TH1D("hQCD_BG_LowDPhi_METshape", "", 60, 0., 300.);  
  TH1D* hQCD_BG_LowDPhi_METshape2 = new TH1D("hQCD_BG_LowDPhi_METshape2", "", 60, 0., 300.);

  //BG from MC 
  TH1D* hQCD_ZNuNu_HiDPhi_MET  = new TH1D("hQCD_ZNuNu_HiDPhi_MET", "", 2, metEdges_C);
  TH1D* hQCD_ZNuNu_LowDPhi_MET = new TH1D("hQCD_ZNuNu_LowDPhi_MET", "", 2, metEdges_S);

  TH1D* hQCD_Wlv_HiDPhi_MET  = new TH1D("hQCD_Wlv_HiDPhi_MET", "", 2, metEdges_C);
  TH1D* hQCD_Wlv_LowDPhi_MET = new TH1D("hQCD_Wlv_LowDPhi_MET", "", 2, metEdges_S);

  TH1D* hQCD_TTBar_HiDPhi_MET  = new TH1D("hQCD_TTBar_HiDPhi_MET", "", 2, metEdges_C); 
  TH1D* hQCD_TTBar_LowDPhi_MET = new TH1D("hQCD_TTBar_LowDPhi_MET", "", 2, metEdges_S);

  TH1D* hQCD_SingleTSum_HiDPhi_MET  = new TH1D("hQCD_SingleTSum_HiDPhi_MET", "", 2, metEdges_C);
  TH1D* hQCD_SingleTSum_LowDPhi_MET = new TH1D("hQCD_SingleTSum_LowDPhi_MET", "", 2, metEdges_S);

  TH1D* hQCD_DY_HiDPhi_MET  = new TH1D("hQCD_DY_HiDPhi_MET", "", 2, metEdges_C);
  TH1D* hQCD_DY_LowDPhi_MET = new TH1D("hQCD_DY_LowDPhi_MET", "", 2, metEdges_S);

  TH1D* hQCD_Diboson_HiDPhi_MET  = new TH1D("hQCD_Diboson_HiDPhi_MET", "", 2, metEdges_C);
  TH1D* hQCD_Diboson_LowDPhi_MET = new TH1D("hQCD_Diboson_LowDPhi_MET", "", 2, metEdges_S);

  // loop over MC datasets
  for (unsigned i=0; i<datasets.size(); ++i) {

    Dataset dataset = datasets.getDataset(i);

    TCut cutD = cuts.cutDataset(dataset.name);
    
    if (dataset.isData) {
      std::cout << "Analysing Data         : " << dataset.name << std::endl;
    }
    else {
      std::cout << "Analysing BG MC        : " << dataset.name << std::endl;
    }

    // get file & tree
    TFile* file = datasets.getTFile(dataset.name);
    TTree* tree = (TTree*) file->Get("invHiggsInfo/InvHiggsInfo");
 
    TCut cutQCDHiDPhi("");
    TCut cutQCDLowDPhi("");
    TCut cutQCDLowDPhiMET("");

    if (dataset.name == "WJets" ||
        dataset.name == "W1Jets" ||
        dataset.name == "W2Jets" ||
        dataset.name == "W3Jets" ||
        dataset.name == "W4Jets") {
      
      cutQCDHiDPhi     = puWeight * trigCorrWeight * wWeight * (cutD + cuts.qcdNoMET() + Dphi_qcd);
      cutQCDLowDPhi    = puWeight * trigCorrWeight * wWeight * (cutD + cuts.qcdNoMET() + Dphi_sig);
      cutQCDLowDPhiMET = puWeight * trigCorrWeight * wWeight * (cutD + cuts.qcdNoMET() + Dphi_sig + MET);      
    }
    else {
      cutQCDHiDPhi     = puWeight * trigCorrWeight * (cutD + cuts.qcdNoMET() + Dphi_qcd);
      cutQCDLowDPhi    = puWeight * trigCorrWeight * (cutD + cuts.qcdNoMET() + Dphi_sig);
      cutQCDLowDPhiMET = puWeight * trigCorrWeight * (cutD + cuts.qcdNoMET() + Dphi_sig + MET); 
    }


    // fill tmp histograms for BG estimation
    TH1D* hQCD_HiDPhi_MET = new TH1D("hQCD_HiDPhi_MET", "", 2, metEdges_C);  
    TH1D* hQCD_LowDPhi_MET = new TH1D("hQCD_LowDPhi_MET", "", 2, metEdges_S);    

    TH1D* hQCD_HiDPhi_METshape   = new TH1D("hQCD_HiDPhi_METshape", "", 60, 0., 300.);
    TH1D* hQCD_LowDPhi_METshape  = new TH1D("hQCD_LowDPhi_METshape", "", 60, 0., 300.);
    TH1D* hQCD_LowDPhi_METshape2 = new TH1D("hQCD_LowDPhi_METshape2", "", 60, 0., 300.);

    //for MET shape histogram
    if (dataset.isData) {
    tree->Draw("met>>hQCD_HiDPhi_MET", cutQCDHiDPhi);
    tree->Draw("met>>hQCD_LowDPhi_MET", cutQCDLowDPhiMET);
    tree->Draw("met>>hQCD_HiDPhi_METshape", cutQCDHiDPhi);
    tree->Draw("met>>hQCD_LowDPhi_METshape", cutQCDLowDPhiMET);  // MET < 130
    }
    else {
    tree->Draw("met>>hQCD_HiDPhi_MET", cutQCDHiDPhi);
    tree->Draw("met>>hQCD_LowDPhi_MET", cutQCDLowDPhi);
    tree->Draw("met>>hQCD_HiDPhi_METshape", cutQCDHiDPhi);
    tree->Draw("met>>hQCD_LowDPhi_METshape", cutQCDLowDPhiMET);
    tree->Draw("met>>hQCD_LowDPhi_METshape2", cutQCDLowDPhi);
    }

    // weight  to lumi
    double weight = (dataset.isData ? 1. : lumi * dataset.sigma / dataset.nEvents);
 
    hQCD_HiDPhi_MET->Scale(weight);
    hQCD_LowDPhi_MET->Scale(weight);
    hQCD_HiDPhi_METshape->Scale(weight);
    hQCD_LowDPhi_METshape->Scale(weight);
    hQCD_LowDPhi_METshape2->Scale(weight);

    // add to output histograms
    if (dataset.isData) {
	hQCD_Data_HiDPhi_MET->Add(hQCD_HiDPhi_MET);
        hQCD_Data_LowDPhi_MET->Add(hQCD_LowDPhi_MET);
        hQCD_Data_HiDPhi_METshape->Add(hQCD_HiDPhi_METshape);
        hQCD_Data_LowDPhi_METshape->Add(hQCD_LowDPhi_METshape);
    }
    else {
	hQCD_BG_HiDPhi_MET->Add(hQCD_HiDPhi_MET);
	hQCD_BG_LowDPhi_MET->Add(hQCD_LowDPhi_MET);
        hQCD_BG_HiDPhi_METshape->Add(hQCD_HiDPhi_METshape);
        hQCD_BG_LowDPhi_METshape->Add(hQCD_LowDPhi_METshape);
        hQCD_BG_LowDPhi_METshape2->Add(hQCD_LowDPhi_METshape2);
    }

    if (dataset.name.compare(0,3,"Zvv")==0) {
	hQCD_ZNuNu_HiDPhi_MET->Add(hQCD_HiDPhi_MET);
        hQCD_ZNuNu_LowDPhi_MET->Add(hQCD_LowDPhi_MET);
    }

    if (dataset.name == "WJets" ||
        dataset.name == "W1Jets" ||
        dataset.name == "W2Jets" ||
        dataset.name == "W3Jets" ||
        dataset.name == "W4Jets") {
	hQCD_Wlv_HiDPhi_MET->Add(hQCD_HiDPhi_MET);
        hQCD_Wlv_LowDPhi_MET->Add(hQCD_LowDPhi_MET);
    }
    if (dataset.name=="TTBar") {
	hQCD_TTBar_HiDPhi_MET->Add(hQCD_HiDPhi_MET);
        hQCD_TTBar_LowDPhi_MET->Add(hQCD_LowDPhi_MET);
    }

    if (dataset.name.compare(0,7,"SingleT")==0) {
        hQCD_SingleTSum_HiDPhi_MET->Add(hQCD_HiDPhi_MET);
        hQCD_SingleTSum_LowDPhi_MET->Add(hQCD_LowDPhi_MET);
    }

    if (dataset.name.compare(0,2,"DY")==0) {
        hQCD_DY_HiDPhi_MET->Add(hQCD_HiDPhi_MET);
        hQCD_DY_LowDPhi_MET->Add(hQCD_LowDPhi_MET);
    }

    if (dataset.name == "WW" ||
	dataset.name == "WZ" ||
	dataset.name == "ZZ") {
        hQCD_Diboson_HiDPhi_MET->Add(hQCD_HiDPhi_MET);
        hQCD_Diboson_LowDPhi_MET->Add(hQCD_LowDPhi_MET);
    }     
 
    std::cout << "  N (met < 120, dphi>2.6)  : " << hQCD_HiDPhi_MET->GetBinContent(1) << " +/- " << hQCD_HiDPhi_MET->GetBinError(1) << std::endl;
    std::cout << "  N (met > 120, dphi>2.6)  : " << hQCD_HiDPhi_MET->GetBinContent(2) << " +/- " << hQCD_HiDPhi_MET->GetBinError(2) << std::endl;
    std::cout << "  N (met < 130, dphi<1.0)  : " << hQCD_LowDPhi_MET->GetBinContent(1) << " +/- " << hQCD_LowDPhi_MET->GetBinError(1) << std::endl;

    delete hQCD_HiDPhi_MET;
    delete hQCD_LowDPhi_MET;
    delete hQCD_HiDPhi_METshape;
    delete hQCD_LowDPhi_METshape;
    delete hQCD_LowDPhi_METshape2;

    // clean up
    delete tree;
    file->Close();
   
  }

  // create MET shape histograms
  TH1D* hQCD_DataSubBG_HiDPhi_METshape  = new TH1D("hQCD_DataSubBG_HiDPhi_METshape", "", 60, 0., 300.);
  TH1D* hQCD_DataSubBG_LowDPhi_METshape = new TH1D("hQCD_DataSubBG_LowDPhi_METshape","", 60, 0., 300.);

  hQCD_DataSubBG_HiDPhi_METshape->Add(hQCD_Data_HiDPhi_METshape,hQCD_BG_HiDPhi_METshape, 1., -1.);
  hQCD_DataSubBG_LowDPhi_METshape->Add(hQCD_Data_LowDPhi_METshape,hQCD_BG_LowDPhi_METshape, 1., -1.);

  // create output histograms
  TH1D* hQCD_Est_S_MET      = new TH1D("hQCD_Est_S_MET",     "", 2, metEdges_S);
  TH1D* hQCD_Est_C_MET      = new TH1D("hQCD_Est_C_MET",     "", 2, metEdges_C);

  hQCD_Est_S_MET->Add(hQCD_Data_LowDPhi_MET,hQCD_BG_LowDPhi_MET, 1., -1.);
  hQCD_Est_C_MET->Add(hQCD_Data_HiDPhi_MET,hQCD_BG_HiDPhi_MET, 1., -1.);

  double kS     = hQCD_Est_C_MET->GetBinContent(2)/hQCD_Est_C_MET->GetBinContent(1);
  double err_kS = kS * sqrt(pow(hQCD_Est_C_MET->GetBinError(2)/hQCD_Est_C_MET->GetBinContent(2),2) + pow(hQCD_Est_C_MET->GetBinError(1)/hQCD_Est_C_MET->GetBinContent(1),2));

  double nQCD_Est_S_MET = kS * hQCD_Est_S_MET->GetBinContent(1); 
  double err_nQCD_Est_S_MET = nQCD_Est_S_MET * sqrt(pow(err_kS/kS,2) + pow(hQCD_Est_S_MET->GetBinError(1)/hQCD_Est_S_MET->GetBinContent(1),2));

  hQCD_Est_S_MET->SetBinContent(2, nQCD_Est_S_MET);
  hQCD_Est_S_MET->SetBinError(2, err_nQCD_Est_S_MET);

  // print results
  std::cout << std::endl;
  std::cout << "QCD Background Method 2 estimate" << std::endl << std::endl;
  std::cout << std::endl;
  std::cout << "Control region (MET>120, dphi>2.6)" << std::endl;
  std::cout << "   N data      : " << hQCD_Data_HiDPhi_MET->GetBinContent(2) << " +/- " << hQCD_Data_HiDPhi_MET->GetBinError(2) << std::endl;
  std::cout << "   N BG (MC)   : " << hQCD_BG_HiDPhi_MET->GetBinContent(2) << " +/- " << hQCD_BG_HiDPhi_MET->GetBinError(2) << std::endl;
  std::cout << "   N ZNuNu (MC)     : " << hQCD_ZNuNu_HiDPhi_MET->GetBinContent(2) << " +/- " << hQCD_ZNuNu_HiDPhi_MET->GetBinError(2) << std::endl;
  std::cout << "   N Wlv (MC)     : " << hQCD_Wlv_HiDPhi_MET->GetBinContent(2) << " +/- " << hQCD_Wlv_HiDPhi_MET->GetBinError(2) << std::endl;
  std::cout << "   N TTbar (MC)    : " << hQCD_TTBar_HiDPhi_MET->GetBinContent(2) << " +/- " << hQCD_TTBar_HiDPhi_MET->GetBinError(2) << std::endl;
  std::cout << "   N SingleT (MC)  : " << hQCD_SingleTSum_HiDPhi_MET->GetBinContent(2) << " +/- " << hQCD_SingleTSum_HiDPhi_MET->GetBinError(2) << std::endl;
  std::cout << "   N DY (MC)       : " << hQCD_DY_HiDPhi_MET->GetBinContent(2) << " +/- " << hQCD_DY_HiDPhi_MET->GetBinError(2) << std::endl;
  std::cout << "   N Diboson (MC)  : " << hQCD_Diboson_HiDPhi_MET->GetBinContent(2) << " +/- " << hQCD_Diboson_HiDPhi_MET->GetBinError(2) << std::endl;
  std::cout << "   N QCD_C (MET>120)     : " << hQCD_Est_C_MET->GetBinContent(2) << " +/- " << hQCD_Est_C_MET->GetBinError(2) << std::endl;
  std::cout << std::endl;
  std::cout << "Control region (MET<120, dphi>2.6)" << std::endl;
  std::cout << "   N data      : " << hQCD_Data_HiDPhi_MET->GetBinContent(1) << " +/- " << hQCD_Data_HiDPhi_MET->GetBinError(1) << std::endl;
  std::cout << "   N BG (MC)   : " << hQCD_BG_HiDPhi_MET->GetBinContent(1) << " +/- " << hQCD_BG_HiDPhi_MET->GetBinError(1) << std::endl;
  std::cout << "   N ZNuNu (MC)     : " << hQCD_ZNuNu_HiDPhi_MET->GetBinContent(1) << " +/- " << hQCD_ZNuNu_HiDPhi_MET->GetBinError(1) << std::endl;
  std::cout << "   N Wlv (MC)     : " << hQCD_Wlv_HiDPhi_MET->GetBinContent(1) << " +/- " << hQCD_Wlv_HiDPhi_MET->GetBinError(1) << std::endl;
  std::cout << "   N TTbar (MC)    : " << hQCD_TTBar_HiDPhi_MET->GetBinContent(1) << " +/- " << hQCD_TTBar_HiDPhi_MET->GetBinError(1) << std::endl;
  std::cout << "   N SingleT (MC)  : " << hQCD_SingleTSum_HiDPhi_MET->GetBinContent(1) << " +/- " << hQCD_SingleTSum_HiDPhi_MET->GetBinError(1) << std::endl;
  std::cout << "   N DY (MC)       : " << hQCD_DY_HiDPhi_MET->GetBinContent(1) << " +/- " << hQCD_DY_HiDPhi_MET->GetBinError(1) << std::endl;
  std::cout << "   N Diboson (MC)  : " << hQCD_Diboson_HiDPhi_MET->GetBinContent(1) << " +/- " << hQCD_Diboson_HiDPhi_MET->GetBinError(1) << std::endl;
  std::cout << "   N QCD_C (MET<120)     : " << hQCD_Est_C_MET->GetBinContent(1) << " +/- " << hQCD_Est_C_MET->GetBinError(1) << std::endl;
  std::cout << std::endl;
  std::cout <<" Ratio kS" << std::endl;
  std::cout << "   kS   : " << kS << " +/- " << err_kS << std::endl;
  std::cout << std::endl;
  std::cout << "Low MET signal region (MET<130, dphi<1.0)" << std::endl;
  std::cout << "   N data      : " << hQCD_Data_LowDPhi_MET->GetBinContent(1) << " +/- " << hQCD_Data_LowDPhi_MET->GetBinError(1) << std::endl;
  std::cout << "   N BG (MC)   : " << hQCD_BG_LowDPhi_MET->GetBinContent(1) << " +/- " << hQCD_BG_LowDPhi_MET->GetBinError(1) << std::endl;
  std::cout << "   N ZNuNu (MC)     : " << hQCD_ZNuNu_LowDPhi_MET->GetBinContent(1) << " +/- " << hQCD_ZNuNu_LowDPhi_MET->GetBinError(1) << std::endl;
  std::cout << "   N Wlv (MC)     : " << hQCD_Wlv_LowDPhi_MET->GetBinContent(1) << " +/- " << hQCD_Wlv_LowDPhi_MET->GetBinError(1) << std::endl;
  std::cout << "   N TTbar (MC)    : " << hQCD_TTBar_LowDPhi_MET->GetBinContent(1) << " +/- " << hQCD_TTBar_LowDPhi_MET->GetBinError(1) << std::endl;
  std::cout << "   N SingleT (MC)  : " << hQCD_SingleTSum_LowDPhi_MET->GetBinContent(1) << " +/- " << hQCD_SingleTSum_LowDPhi_MET->GetBinError(1) << std::endl;
  std::cout << "   N DY (MC)       : " << hQCD_DY_LowDPhi_MET->GetBinContent(1) << " +/- " << hQCD_DY_LowDPhi_MET->GetBinError(1) << std::endl;
  std::cout << "   N Diboson (MC)  : " << hQCD_Diboson_LowDPhi_MET->GetBinContent(1) << " +/- " << hQCD_Diboson_LowDPhi_MET->GetBinError(1) << std::endl;
  std::cout << "   N QCD_S (MET<130)     : " << hQCD_Est_S_MET->GetBinContent(1) << " +/- " << hQCD_Est_S_MET->GetBinError(1) << std::endl;
  std::cout << std::endl;
  std::cout << "Signal region (MET>130, dphi<1.0)" << std::endl;
  std::cout << "   N QCD (est): " << hQCD_Est_S_MET->GetBinContent(2) << " +/- " << hQCD_Est_S_MET->GetBinError(2) << std::endl;

  // write out histograms
  ofile->cd();
  hQCD_Data_HiDPhi_METshape->Write("", TObject::kOverwrite);
  hQCD_Data_LowDPhi_METshape->Write("", TObject::kOverwrite);
  hQCD_BG_HiDPhi_METshape->Write("", TObject::kOverwrite);
  hQCD_BG_LowDPhi_METshape2->Write("", TObject::kOverwrite);
  hQCD_DataSubBG_HiDPhi_METshape->Write("", TObject::kOverwrite);
  hQCD_DataSubBG_LowDPhi_METshape->Write("", TObject::kOverwrite);

  hQCD_Data_HiDPhi_MET->Write("", TObject::kOverwrite);
  hQCD_Data_LowDPhi_MET->Write("", TObject::kOverwrite);
  hQCD_BG_HiDPhi_MET->Write("", TObject::kOverwrite);
  hQCD_BG_LowDPhi_MET->Write("", TObject::kOverwrite);

  hQCD_Est_C_MET->Write("", TObject::kOverwrite);
  hQCD_Est_S_MET->Write("", TObject::kOverwrite);

  // clean up
  ofile->Close();

}
