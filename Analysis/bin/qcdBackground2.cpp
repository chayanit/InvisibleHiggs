
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

  // output file
  TFile* ofile = TFile::Open( (options.oDir+std::string("/QCDBackground_M2.root")).c_str(), "UPDATE");

  // cuts
  Cuts cuts;
  TCut puWeight("puWeight");
  //TCut trigCorrWeight("trigCorrWeight");
  TCut trigCorrWeight("1.");
  TCut Dphi_sig("vbfDPhi<1.0");
  TCut Dphi_qcd("vbfDPhi>2.6");

  TCut cutQCDNoMET   = puWeight * trigCorrWeight * cuts.qcdNoMET();
  TCut cutQCDHiDPhi  = puWeight * trigCorrWeight * (cuts.qcdNoMET() + Dphi_qcd);
  TCut cutQCDLowDPhi = puWeight * trigCorrWeight * (cuts.qcdNoMET() + Dphi_sig);

  //std::cout << cutQCDTightHiDPhi << std::endl;

  // histograms
  //double dphiEdges[4] = { 0., 1.0, 2.6, TMath::Pi() };
  double metEdges_S[3]  = { 0., 130., 300. };
  double metEdges_C[3]  = { 0., 120., 300. }; 	//shift by 10 GeV 

  TH1D* hQCD_BG_HiDPhi_MET  = new TH1D("hQCD_BG_HiDPhi_MET", "", 2, metEdges_C); 		//EW subtract in QCD region
  TH1D* hQCD_BG_LowDPhi_MET = new TH1D("hQCD_BG_LowDPhi_MET", "", 2, metEdges_S);  	//EW subtract in signal region

  TH1D* hQCD_TTBar_HiDPhi_MET  = new TH1D("hQCD_TTBar_HiDPhi_MET", "", 2, metEdges_C); 
  TH1D* hQCD_TTBar_LowDPhi_MET = new TH1D("hQCD_TTBar_LowDPhi_MET", "", 2, metEdges_S);

  TH1D* hQCD_SingleTSum_HiDPhi_MET  = new TH1D("hQCD_SingleTSum_HiDPhi_MET", "", 2, metEdges_C);
  TH1D* hQCD_SingleTSum_LowDPhi_MET = new TH1D("hQCD_SingleTSum_LowDPhi_MET", "", 2, metEdges_S);

  TH1D* hQCD_DY_HiDPhi_MET  = new TH1D("hQCD_DY_HiDPhi_MET", "", 2, metEdges_C);
  TH1D* hQCD_DY_LowDPhi_MET = new TH1D("hQCD_DY_LowDPhi_MET", "", 2, metEdges_S);

  TH1D* hQCD_Diboson_HiDPhi_MET  = new TH1D("hQCD_Diboson_HiDPhi_MET", "", 2, metEdges_C);
  TH1D* hQCD_Diboson_LowDPhi_MET = new TH1D("hQCD_Diboson_LowDPhi_MET", "", 2, metEdges_S);

  TH1D* hQCD_Data_HiDPhi_MET  = new TH1D("hQCD_Data_HiDPhi_MET", "", 2, metEdges_C);
  TH1D* hQCD_Data_LowDPhi_MET = new TH1D("hQCD_Data_LowDPhi_MET", "", 2, metEdges_S);

  // loop over MC datasets
  for (unsigned i=0; i<datasets.size(); ++i) {

    Dataset dataset = datasets.getDataset(i);
    
    if (dataset.isData) {
      std::cout << "Analysing Data         : " << dataset.name << std::endl;
    }
    else {
      std::cout << "Analysing BG MC        : " << dataset.name << std::endl;
    }

    // get file & tree
    TFile* file = datasets.getTFile(dataset.name);
    TTree* tree = (TTree*) file->Get("invHiggsInfo/InvHiggsInfo");

    // fill tmp histograms for BG estimation
    TH1D* hQCD_HiDPhi_MET = new TH1D("hQCD_HiDPhi_MET", "", 2, metEdges_C);  
    TH1D* hQCD_LowDPhi_MET = new TH1D("hQCD_LowDPhi_MET", "", 2, metEdges_S);    
 
    //TH1D* hQCD_Loose2_DPhi = new TH1D("hQCD_Loose2_DPhi", "", 3, dphiEdges);  // 
    //TH1D* hQCD_Loose_DPhi = new TH1D("hQCD_Loose_DPhi", "", 3, dphiEdges);  // 
    //TH1D* hQCD_Tight_DPhi = new TH1D("hQCD_Tight_DPhi", "", 3, dphiEdges);  // this is for the actual BG estimation

    tree->Draw("met>>hQCD_HiDPhi_MET", cutQCDHiDPhi);
    tree->Draw("met>>hQCD_LowDPhi_MET", cutQCDLowDPhi);

    //tree->Draw("vbfDPhi>>hQCD_Loose2_DPhi", cutQCDLoose2);
    //tree->Draw("vbfDPhi>>hQCD_Loose_DPhi", cutQCDLoose);
    //tree->Draw("vbfDPhi>>hQCD_Tight_DPhi", cutQCDTightHiDPhi);

    // weight  to lumi
    double weight = (dataset.isData ? 1. : lumi * dataset.sigma / dataset.nEvents);
 
    hQCD_HiDPhi_MET->Scale(weight);
    hQCD_LowDPhi_MET->Scale(weight);

    //hQCD_NoMET_DPhi->Scale(weight);
    //hQCD_Loose2_DPhi->Scale(weight);
    //hQCD_Loose_DPhi->Scale(weight);
    //hQCD_Tight_DPhi->Scale(weight);

    // add to output histograms
    if (dataset.isData) {
	hQCD_Data_HiDPhi_MET->Add(hQCD_HiDPhi_MET);
        hQCD_Data_LowDPhi_MET->Add(hQCD_LowDPhi_MET);

      //hQCD_Data_NoMET_DPhi->Add(hQCD_NoMET_DPhi);
      //hQCD_Data_Loose2_DPhi->Add(hQCD_Loose2_DPhi);
      //hQCD_Data_Loose_DPhi->Add(hQCD_Loose_DPhi);
      //hQCD_Data_Tight_DPhi->Add(hQCD_Tight_DPhi);
    }
    else {
	hQCD_BG_HiDPhi_MET->Add(hQCD_HiDPhi_MET);
	hQCD_BG_LowDPhi_MET->Add(hQCD_LowDPhi_MET);

      //hQCD_BG_NoMET_DPhi->Add(hQCD_NoMET_DPhi);
      //hQCD_BG_Loose2_DPhi->Add(hQCD_Loose2_DPhi);
      //hQCD_BG_Loose_DPhi->Add(hQCD_Loose_DPhi);
      //hQCD_BG_Tight_DPhi->Add(hQCD_Tight_DPhi);
    }

    if (dataset.name=="TTBar") {
	hQCD_TTBar_HiDPhi_MET->Add(hQCD_HiDPhi_MET);
        hQCD_TTBar_LowDPhi_MET->Add(hQCD_LowDPhi_MET);

      //hQCD_TTBar_NoMET_DPhi->Add(hQCD_NoMET_DPhi);
      //hQCD_TTBar_Loose2_DPhi->Add(hQCD_Loose2_DPhi);
      //hQCD_TTBar_Loose_DPhi->Add(hQCD_Loose_DPhi);
      //hQCD_TTBar_Tight_DPhi->Add(hQCD_Tight_DPhi);
    }

    if (dataset.name.compare(0,7,"SingleT")==0) {
        hQCD_SingleTSum_HiDPhi_MET->Add(hQCD_HiDPhi_MET);
        hQCD_SingleTSum_LowDPhi_MET->Add(hQCD_LowDPhi_MET);
 
      //hQCD_SingleTSum_NoMET_DPhi->Add(hQCD_NoMET_DPhi);
      //hQCD_SingleTSum_Loose2_DPhi->Add(hQCD_Loose2_DPhi);
      //hQCD_SingleTSum_Loose_DPhi->Add(hQCD_Loose_DPhi);
      //hQCD_SingleTSum_Tight_DPhi->Add(hQCD_Tight_DPhi);
    }

    if (dataset.name.compare(0,2,"DY")==0) {
        hQCD_DY_HiDPhi_MET->Add(hQCD_HiDPhi_MET);
        hQCD_DY_LowDPhi_MET->Add(hQCD_LowDPhi_MET);

      //hQCD_DY_NoMET_DPhi->Add(hQCD_NoMET_DPhi);
      //hQCD_DY_Loose2_DPhi->Add(hQCD_Loose2_DPhi);
      //hQCD_DY_Loose_DPhi->Add(hQCD_Loose_DPhi);
      //hQCD_DY_Tight_DPhi->Add(hQCD_Tight_DPhi);
    }

    if (dataset.name == "WW" ||
	dataset.name == "WZ" ||
	dataset.name == "ZZ") {
        hQCD_Diboson_HiDPhi_MET->Add(hQCD_HiDPhi_MET);
        hQCD_Diboson_LowDPhi_MET->Add(hQCD_LowDPhi_MET);

      //hQCD_Diboson_NoMET_DPhi->Add(hQCD_NoMET_DPhi);
      //hQCD_Diboson_Loose2_DPhi->Add(hQCD_Loose2_DPhi);
      //hQCD_Diboson_Loose_DPhi->Add(hQCD_Loose_DPhi);
      //hQCD_Diboson_Tight_DPhi->Add(hQCD_Tight_DPhi);
    }     
 
    std::cout << "  N (met < 120, dphi>2.6)  : " << hQCD_HiDPhi_MET->GetBinContent(1) << std::endl;
    std::cout << "  N (met > 120, dphi>2.6)  : " << hQCD_HiDPhi_MET->GetBinContent(2) << std::endl;
    std::cout << "  N (met < 130, dphi<1.0)  : " << hQCD_LowDPhi_MET->GetBinContent(1) << std::endl;
    //std::cout << "  N (met>35, dphi>2.6) : " << hQCD_Loose2_DPhi->GetBinContent(3) << std::endl;
    //std::cout << "  N (met>35, dphi<1.0) : " << hQCD_Loose2_DPhi->GetBinContent(1) << std::endl;
    //std::cout << "  N (met>70, dphi>2.6) : " << hQCD_Loose_DPhi->GetBinContent(3) << std::endl;
    //std::cout << "  N (met>70, dphi<1.0) : " << hQCD_Loose_DPhi->GetBinContent(1) << std::endl;
    //std::cout << "  N (met>130,dphi>2.6) : " << hQCD_Tight_DPhi->GetBinContent(3) << std::endl;

    delete hQCD_HiDPhi_MET;
    delete hQCD_LowDPhi_MET;

    //delete hQCD_NoMET_DPhi;
    //delete hQCD_Loose_DPhi;
    //delete hQCD_Loose2_DPhi;
    //delete hQCD_Tight_DPhi;

    // per-dataset control plots (just an example, add more later)
    ofile->cd();

    //std::string hname = std::string("hQCD_Loose_DPhi2_")+dataset.name;
    //TH1D* hQCD_Loose_DPhi2 = new TH1D(hname.c_str(), "", 72, 0., TMath::Pi());
    //std::string str = std::string("vbfDPhi>>")+hname;
    //tree->Draw(str.c_str(), cutQCDLoose);
    //hQCD_Loose_DPhi2->Scale(weight);
    //hQCD_Loose_DPhi2->Write("",TObject::kOverwrite);

    std::string hname = std::string("hQCD_MET_HiDPhi_")+dataset.name;
    TH1D* hQCD_MET_HiDPhi = new TH1D(hname.c_str(), "", 60, 0., 300.);
    std::string str = std::string("met>>")+hname;
    tree->Draw(str.c_str(), cutQCDHiDPhi);
    hQCD_MET_HiDPhi->Scale(weight); 
    hQCD_MET_HiDPhi->Write("",TObject::kOverwrite);

    hname = std::string("hQCD_MET_LowDPhi_")+dataset.name;
    TH1D* hQCD_MET_LowDPhi = new TH1D(hname.c_str(), "", 60, 0., 300.);
    str = std::string("met>>")+hname;
    tree->Draw(str.c_str(), cutQCDLowDPhi);
    hQCD_MET_LowDPhi->Scale(weight);
    hQCD_MET_LowDPhi->Write("",TObject::kOverwrite);

    // clean up
    delete tree;
    file->Close();
   
  }

  // get data-driven background estimates
  //std::cout << "Reading Z backgrounds from : " << options.oDir+std::string("/ZBackground.root") << std::endl;
  //TFile* zfile = TFile::Open( (options.oDir+std::string("/ZBackground.root")).c_str(), "READ");
  //TH1D* hQCD_Z_NoMET_DPhi  = (TH1D*) zfile->Get("hZ_Est_NoMETS_DPhi");
  //TH1D* hQCD_Z_Loose2_DPhi = (TH1D*) zfile->Get("hZ_Est_Loose2S_DPhi");
  //TH1D* hQCD_Z_Loose_DPhi  = (TH1D*) zfile->Get("hZ_Est_LooseS_DPhi");
  //TH1D* hQCD_Z_Tight_DPhi  = (TH1D*) zfile->Get("hZ_Est_S_DPhi");

  //if (hQCD_Z_Tight_DPhi == 0) {
  //  std::cerr << "Could not read Z background histogram" << std::endl;
  //  std::exit(1);
  //}

  //std::cout << "  N (met>0, dphi>2.6)  : " << hQCD_Z_NoMET_DPhi->GetBinContent(3) << std::endl;
  //std::cout << "  N (met>0, dphi<1.0)  : " << hQCD_Z_NoMET_DPhi->GetBinContent(1) << std::endl;
  //std::cout << "  N (met>35, dphi>2.6) : " << hQCD_Z_Loose2_DPhi->GetBinContent(3) << std::endl;
  //std::cout << "  N (met>35, dphi>1.0) : " << hQCD_Z_Loose2_DPhi->GetBinContent(1) << std::endl;
  //std::cout << "  N (met>70, dphi>2.6) : " << hQCD_Z_Loose_DPhi->GetBinContent(3) << std::endl;
  //std::cout << "  N (met>70, dphi>1.0) : " << hQCD_Z_Loose_DPhi->GetBinContent(1) << std::endl;
  //std::cout << "  N (met>130,dphi>2.6) : " << hQCD_Z_Tight_DPhi->GetBinContent(3) << std::endl;
  std::cout << std::endl;
  
  //std::cout << "Reading W backgrounds from : " << options.oDir+std::string("/WBackground.root") << std::endl;
  //TFile* wfile = TFile::Open( (options.oDir+std::string("/WBackground.root")).c_str(), "READ");
  //TH1D* hQCD_W_NoMET_DPhi  = (TH1D*) wfile->Get("hW_Est_NoMETS_DPhi");
  //TH1D* hQCD_W_Loose2_DPhi = (TH1D*) wfile->Get("hW_Est_Loose2S_DPhi");
  //TH1D* hQCD_W_Loose_DPhi  = (TH1D*) wfile->Get("hW_Est_LooseS_DPhi");
  //TH1D* hQCD_W_Tight_DPhi  = (TH1D*) wfile->Get("hW_Est_S_DPhi");

  //if (hQCD_W_Tight_DPhi == 0) {
  //  std::cerr << "Could not read W background histogram" << std::endl;
  //  std::exit(1);
  //}  

  //std::cout << "  N (met>0, dphi>2.6)  : " << hQCD_W_NoMET_DPhi->GetBinContent(3) << std::endl;
  //std::cout << "  N (met>0, dphi>1.0)  : " << hQCD_W_NoMET_DPhi->GetBinContent(1) << std::endl;
  //std::cout << "  N (met>35, dphi>2.6) : " << hQCD_W_Loose2_DPhi->GetBinContent(3) << std::endl;
  //std::cout << "  N (met>35, dphi>1.0) : " << hQCD_W_Loose2_DPhi->GetBinContent(1) << std::endl;
  //std::cout << "  N (met>70, dphi>2.6) : " << hQCD_W_Loose_DPhi->GetBinContent(3) << std::endl;
  //std::cout << "  N (met>70, dphi>1.0) : " << hQCD_W_Loose_DPhi->GetBinContent(1) << std::endl;
  //std::cout << "  N (met>130,dphi>2.6) : " << hQCD_W_Tight_DPhi->GetBinContent(3) << std::endl;
  //std::cout << std::endl;
  

  // create output histograms
  //TH1D* hQCD_Est_NoMET_DPhi  = new TH1D("hQCD_Est_NoMET_DPhi", "", 3, dphiEdges);
  //TH1D* hQCD_Est_Loose2_DPhi = new TH1D("hQCD_Est_Loose2_DPhi", "", 3, dphiEdges);
  //TH1D* hQCD_Est_Loose_DPhi  = new TH1D("hQCD_Est_Loose_DPhi", "", 3, dphiEdges);
  //TH1D* hQCD_Est_S_DPhi      = new TH1D("hQCD_Est_S_DPhi",     "", 3, dphiEdges);

  // do the background estimation
  //hQCD_Est_NoMET_DPhi->Add(hQCD_Data_NoMET_DPhi, hQCD_BG_NoMET_DPhi, 1., -1.);
  //hQCD_Est_Loose2_DPhi->Add(hQCD_Data_Loose2_DPhi, hQCD_BG_Loose2_DPhi, 1., -1.);
  //hQCD_Est_Loose_DPhi->Add(hQCD_Data_Loose_DPhi, hQCD_BG_Loose_DPhi, 1., -1.);
  //hQCD_Est_S_DPhi->Add(hQCD_Data_Tight_DPhi, hQCD_BG_Tight_DPhi, 1., -1.);

  //hQCD_Est_NoMET_DPhi->Add(hQCD_Z_NoMET_DPhi, -1.);
  //hQCD_Est_Loose2_DPhi->Add(hQCD_Z_Loose2_DPhi, -1.);
  //hQCD_Est_Loose_DPhi->Add(hQCD_Z_Loose_DPhi, -1.);
  //hQCD_Est_S_DPhi->Add(hQCD_Z_Tight_DPhi, -1.);

  //hQCD_Est_NoMET_DPhi->Add(hQCD_W_NoMET_DPhi, -1.);
  //hQCD_Est_Loose2_DPhi->Add(hQCD_W_Loose2_DPhi, -1.);
  //hQCD_Est_Loose_DPhi->Add(hQCD_W_Loose_DPhi, -1.);
  //hQCD_Est_S_DPhi->Add(hQCD_W_Tight_DPhi, -1.);

  // calculate ratios
  //double rNoMET      = hQCD_Est_NoMET_DPhi->GetBinContent(1) / hQCD_Est_NoMET_DPhi->GetBinContent(3);
  //double err_rNoMET  = rNoMET * sqrt(pow(hQCD_Est_NoMET_DPhi->GetBinError(1)/hQCD_Est_NoMET_DPhi->GetBinContent(1),2) + pow(hQCD_Est_NoMET_DPhi->GetBinError(3)/hQCD_Est_NoMET_DPhi->GetBinContent(3),2));
  //double rLoose2     = hQCD_Est_Loose2_DPhi->GetBinContent(1) / hQCD_Est_Loose2_DPhi->GetBinContent(3);
  //double err_rLoose2 = rLoose2 * sqrt(pow(hQCD_Est_Loose2_DPhi->GetBinError(1)/hQCD_Est_Loose2_DPhi->GetBinContent(1),2) + pow(hQCD_Est_Loose2_DPhi->GetBinError(3)/hQCD_Est_Loose2_DPhi->GetBinContent(3),2));
  //double rLoose      = hQCD_Est_Loose_DPhi->GetBinContent(1) / hQCD_Est_Loose_DPhi->GetBinContent(3);
  //double err_rLoose  = rLoose * sqrt(pow(hQCD_Est_Loose_DPhi->GetBinError(1)/hQCD_Est_Loose_DPhi->GetBinContent(1),2) + pow(hQCD_Est_Loose_DPhi->GetBinError(3)/hQCD_Est_Loose_DPhi->GetBinContent(3),2));
  
  // linear extrapolation using MET>0 and MET>70 bins
  //double rTight = (130. * (rLoose - rNoMET)/70.) + rNoMET;
  //double err_rTight = rTight * sqrt(pow(err_rNoMET/rNoMET,2) + pow(err_rLoose/rLoose,2));

  // predict signal
  //double nQCD_Est_S_HiDPhi = rTight * hQCD_Est_S_DPhi->GetBinContent(3);
  //double err_nQCD_Est_S_HiDPhi = nQCD_Est_S_HiDPhi * sqrt(pow(err_rTight/rTight,2)+pow(hQCD_Est_S_DPhi->GetBinError(3)/hQCD_Est_S_DPhi->GetBinContent(3),2));

  // set bins in output histogram
  //hQCD_Est_S_DPhi->SetBinContent(1, nQCD_Est_S_HiDPhi);
  //hQCD_Est_S_DPhi->SetBinError(1, err_nQCD_Est_S_HiDPhi);

  /*
  // print results
  std::cout << std::endl;
  std::cout << "QCD Background estimate" << std::endl << std::endl;
  std::cout << "No MET region (MET>0, dphi>2.6)" << std::endl;
  std::cout << "   N data          : " << hQCD_Data_NoMET_DPhi->GetBinContent(3) << " +/- " << hQCD_Data_NoMET_DPhi->GetBinError(3) << std::endl;
  std::cout << "   N Z (data)      : " << hQCD_Z_NoMET_DPhi->GetBinContent(3) << " +/- " << hQCD_Z_NoMET_DPhi->GetBinError(3) << std::endl;
  std::cout << "   N W (data)      : " << hQCD_W_NoMET_DPhi->GetBinContent(3) << " +/- " << hQCD_W_NoMET_DPhi->GetBinError(3) << std::endl;
  std::cout << "   N BG (MC)       : " << hQCD_BG_NoMET_DPhi->GetBinContent(3) << " +/- " << hQCD_BG_NoMET_DPhi->GetBinError(3) << std::endl;
  std::cout << "   N TTbar (MC)    : " << hQCD_TTBar_NoMET_DPhi->GetBinContent(3) << " +/- " << hQCD_TTBar_NoMET_DPhi->GetBinError(3) << std::endl;
  std::cout << "   N SingleT (MC)  : " << hQCD_SingleTSum_NoMET_DPhi->GetBinContent(3) << " +/- " << hQCD_SingleTSum_NoMET_DPhi->GetBinError(3) << std::endl;
  std::cout << "   N DY (MC)       : " << hQCD_DY_NoMET_DPhi->GetBinContent(3) << " +/- " << hQCD_DY_NoMET_DPhi->GetBinError(3) << std::endl;
  std::cout << "   N Diboson (MC)  : " << hQCD_Diboson_NoMET_DPhi->GetBinContent(3) << " +/- " << hQCD_Diboson_NoMET_DPhi->GetBinError(3) << std::endl;
  std::cout << std::endl;
  
  std::cout << "No MET region (MET>0, dphi<1.0)" << std::endl;
  std::cout << "   N data          : " << hQCD_Data_NoMET_DPhi->GetBinContent(1) << " +/- " << hQCD_Data_NoMET_DPhi->GetBinError(1) << std::endl;
  std::cout << "   N Z (data)      : " << hQCD_Z_NoMET_DPhi->GetBinContent(1) << " +/- " << hQCD_Z_NoMET_DPhi->GetBinError(1) << std::endl;
  std::cout << "   N W (data)      : " << hQCD_W_NoMET_DPhi->GetBinContent(1) << " +/- " << hQCD_W_NoMET_DPhi->GetBinError(1) << std::endl;
  std::cout << "   N BG (MC)       : " << hQCD_BG_NoMET_DPhi->GetBinContent(1) << " +/- " << hQCD_BG_NoMET_DPhi->GetBinError(1) << std::endl;
  std::cout << "   N TTbar (MC)    : " << hQCD_TTBar_NoMET_DPhi->GetBinContent(1) << " +/- " << hQCD_TTBar_NoMET_DPhi->GetBinError(1) << std::endl;
  std::cout << "   N SingleT (MC)  : " << hQCD_SingleTSum_NoMET_DPhi->GetBinContent(1) << " +/- " << hQCD_SingleTSum_NoMET_DPhi->GetBinError(1) << std::endl;
  std::cout << "   N DY (MC)       : " << hQCD_DY_NoMET_DPhi->GetBinContent(1) << " +/- " << hQCD_DY_NoMET_DPhi->GetBinError(1) << std::endl;
  std::cout << "   N Diboson (MC)  : " << hQCD_Diboson_NoMET_DPhi->GetBinContent(1) << " +/- " << hQCD_Diboson_NoMET_DPhi->GetBinError(1) << std::endl;
  std::cout << std::endl;

  std::cout << "V Loose region (MET>35, dphi>2.6)" << std::endl;
  std::cout << "   N data          : " << hQCD_Data_Loose2_DPhi->GetBinContent(3) << " +/- " << hQCD_Data_Loose2_DPhi->GetBinError(3) << std::endl;
  std::cout << "   N Z (data)      : " << hQCD_Z_Loose2_DPhi->GetBinContent(3) << " +/- " << hQCD_Z_Loose2_DPhi->GetBinError(3) << std::endl;
  std::cout << "   N W (data)      : " << hQCD_W_Loose2_DPhi->GetBinContent(3) << " +/- " << hQCD_W_Loose2_DPhi->GetBinError(3) << std::endl;
  std::cout << "   N BG (MC)       : " << hQCD_BG_Loose2_DPhi->GetBinContent(3) << " +/- " << hQCD_BG_Loose2_DPhi->GetBinError(3) << std::endl;
  std::cout << "   N TTbar (MC)    : " << hQCD_TTBar_Loose2_DPhi->GetBinContent(3) << " +/- " << hQCD_TTBar_Loose2_DPhi->GetBinError(3) << std::endl;
  std::cout << "   N SingleT (MC)  : " << hQCD_SingleTSum_Loose2_DPhi->GetBinContent(3) << " +/- " << hQCD_SingleTSum_Loose2_DPhi->GetBinError(3) << std::endl;
  std::cout << "   N DY (MC)       : " << hQCD_DY_Loose2_DPhi->GetBinContent(3) << " +/- " << hQCD_DY_Loose2_DPhi->GetBinError(3) << std::endl;
  std::cout << "   N Diboson (MC)  : " << hQCD_Diboson_Loose2_DPhi->GetBinContent(3) << " +/- " << hQCD_Diboson_Loose2_DPhi->GetBinError(3) << std::endl;
  std::cout << std::endl;

  std::cout << "V Loose region (MET>35, dphi<1.0)" << std::endl;
  std::cout << "   N data          : " << hQCD_Data_Loose2_DPhi->GetBinContent(1) << " +/- " << hQCD_Data_Loose2_DPhi->GetBinError(1) << std::endl;
  std::cout << "   N Z (data)      : " << hQCD_Z_Loose2_DPhi->GetBinContent(1) << " +/- " << hQCD_Z_Loose2_DPhi->GetBinError(1) << std::endl;
  std::cout << "   N W (data)      : " << hQCD_W_Loose2_DPhi->GetBinContent(1) << " +/- " << hQCD_W_Loose2_DPhi->GetBinError(1) << std::endl;
  std::cout << "   N BG (MC)       : " << hQCD_BG_Loose2_DPhi->GetBinContent(1) << " +/- " << hQCD_BG_Loose2_DPhi->GetBinError(1) << std::endl;
  std::cout << "   N TTbar (MC)    : " << hQCD_TTBar_Loose2_DPhi->GetBinContent(1) << " +/- " << hQCD_TTBar_Loose2_DPhi->GetBinError(1) << std::endl;
  std::cout << "   N SingleT (MC)  : " << hQCD_SingleTSum_Loose2_DPhi->GetBinContent(1) << " +/- " << hQCD_SingleTSum_Loose2_DPhi->GetBinError(1) << std::endl;
  std::cout << "   N DY (MC)       : " << hQCD_DY_Loose2_DPhi->GetBinContent(1) << " +/- " << hQCD_DY_Loose2_DPhi->GetBinError(1) << std::endl;
  std::cout << "   N Diboson (MC)  : " << hQCD_Diboson_Loose2_DPhi->GetBinContent(1) << " +/- " << hQCD_Diboson_Loose2_DPhi->GetBinError(1) << std::endl;
  std::cout << std::endl;

  std::cout << "Loose region (MET>70, dphi>2.6)" << std::endl;
  std::cout << "   N data          : " << hQCD_Data_Loose_DPhi->GetBinContent(3) << " +/- " << hQCD_Data_Loose_DPhi->GetBinError(3) << std::endl;
  std::cout << "   N Z (data)      : " << hQCD_Z_Loose_DPhi->GetBinContent(3) << " +/- " << hQCD_Z_Loose_DPhi->GetBinError(3) << std::endl;
  std::cout << "   N W (data)      : " << hQCD_W_Loose_DPhi->GetBinContent(3) << " +/- " << hQCD_W_Loose_DPhi->GetBinError(3) << std::endl;
  std::cout << "   N BG (MC)       : " << hQCD_BG_Loose_DPhi->GetBinContent(3) << " +/- " << hQCD_BG_Loose_DPhi->GetBinError(3) << std::endl;
  std::cout << "   N TTbar (MC)    : " << hQCD_TTBar_Loose_DPhi->GetBinContent(3) << " +/- " << hQCD_TTBar_Loose_DPhi->GetBinError(3) << std::endl;
  std::cout << "   N SingleT (MC)  : " << hQCD_SingleTSum_Loose_DPhi->GetBinContent(3) << " +/- " << hQCD_SingleTSum_Loose_DPhi->GetBinError(3) << std::endl;
  std::cout << "   N DY (MC)       : " << hQCD_DY_Loose_DPhi->GetBinContent(3) << " +/- " << hQCD_DY_Loose_DPhi->GetBinError(3) << std::endl;
  std::cout << "   N Diboson (MC)  : " << hQCD_Diboson_Loose_DPhi->GetBinContent(3) << " +/- " << hQCD_Diboson_Loose_DPhi->GetBinError(3) << std::endl;
  std::cout << std::endl;

  std::cout << "Loose region (MET>70, dphi<1.0)" << std::endl;
  std::cout << "   N data          : " << hQCD_Data_Loose_DPhi->GetBinContent(1) << " +/- " << hQCD_Data_Loose_DPhi->GetBinError(1) << std::endl;
  std::cout << "   N Z (data)      : " << hQCD_Z_Loose_DPhi->GetBinContent(1) << " +/- " << hQCD_Z_Loose_DPhi->GetBinError(1) << std::endl;
  std::cout << "   N W (data)      : " << hQCD_W_Loose_DPhi->GetBinContent(1) << " +/- " << hQCD_W_Loose_DPhi->GetBinError(1) << std::endl;
  std::cout << "   N BG (MC)       : " << hQCD_BG_Loose_DPhi->GetBinContent(1) << " +/- " << hQCD_BG_Loose_DPhi->GetBinError(1) << std::endl;
  std::cout << "   N TTbar (MC)    : " << hQCD_TTBar_Loose_DPhi->GetBinContent(1) << " +/- " << hQCD_TTBar_Loose_DPhi->GetBinError(1) << std::endl;
  std::cout << "   N SingleT (MC)  : " << hQCD_SingleTSum_Loose_DPhi->GetBinContent(1) << " +/- " << hQCD_SingleTSum_Loose_DPhi->GetBinError(1) << std::endl;
  std::cout << "   N DY (MC)       : " << hQCD_DY_Loose_DPhi->GetBinContent(1) << " +/- " << hQCD_DY_Loose_DPhi->GetBinError(1) << std::endl;
  std::cout << "   N Diboson (MC)  : " << hQCD_Diboson_Loose_DPhi->GetBinContent(1) << " +/- " << hQCD_Diboson_Loose_DPhi->GetBinError(1) << std::endl;
  std::cout << std::endl;

  std::cout << std::endl;

  std::cout <<" Ratios" << std::endl;
  std::cout << "   R(MET>0)   : " << rNoMET << " +/- " << err_rNoMET << std::endl;
  std::cout << "   R(MET>35)  : " << rLoose2 << " +/- " << err_rLoose2 << std::endl;
  std::cout << "   R(MET>70)  : " << rLoose << " +/- " << err_rLoose << std::endl;
  std::cout << "   R(MET>130) : " << rTight << " +/- " << err_rTight << std::endl;
  std::cout << std::endl;

  std::cout << "Control region (MET>130, dphi>2.6)" << std::endl;
  std::cout << "   N data      : " << hQCD_Data_Tight_DPhi->GetBinContent(3) << " +/- " << hQCD_Data_Tight_DPhi->GetBinError(3) << std::endl;
  std::cout << "   N Z (data)  : " << hQCD_Z_Tight_DPhi->GetBinContent(3) << " +/- " << hQCD_Z_Tight_DPhi->GetBinError(3) << std::endl;
  std::cout << "   N W (data)  : " << hQCD_W_Tight_DPhi->GetBinContent(3) << " +/- " << hQCD_W_Tight_DPhi->GetBinError(3) << std::endl;
  std::cout << "   N BG (MC)   : " << hQCD_BG_Tight_DPhi->GetBinContent(3) << " +/- " << hQCD_BG_Tight_DPhi->GetBinError(3) << std::endl;
  std::cout << "   N QCD (est) : " << hQCD_Est_S_DPhi->GetBinContent(3) << " +/- " << hQCD_Est_S_DPhi->GetBinError(3) << std::endl;
  std::cout << std::endl;
  std::cout << "Signal region (MET>130, dphi<1.0)" << std::endl;
  std::cout << "   N QCD (est): " << hQCD_Est_S_DPhi->GetBinContent(1) << " +/- " << hQCD_Est_S_DPhi->GetBinError(1) << std::endl;
  */

  // write out histograms
  ofile->cd();
  hQCD_Data_HiDPhi_MET->Write("", TObject::kOverwrite);
  hQCD_Data_LowDPhi_MET->Write("", TObject::kOverwrite);
  hQCD_BG_HiDPhi_MET->Write("", TObject::kOverwrite);
  hQCD_BG_LowDPhi_MET->Write("", TObject::kOverwrite);

  //hQCD_Data_NoMET_DPhi->Write("", TObject::kOverwrite);
  //hQCD_Data_Loose_DPhi->Write("", TObject::kOverwrite);
  //hQCD_Data_Tight_DPhi->Write("", TObject::kOverwrite);
  //hQCD_BG_NoMET_DPhi->Write("", TObject::kOverwrite);
  //hQCD_BG_Loose_DPhi->Write("", TObject::kOverwrite);
  //hQCD_BG_Tight_DPhi->Write("", TObject::kOverwrite);
  //hQCD_Z_Tight_DPhi->Write("", TObject::kOverwrite);
  //hQCD_W_Tight_DPhi->Write("", TObject::kOverwrite);
  //hQCD_Est_NoMET_DPhi->Write("", TObject::kOverwrite);
  //hQCD_Est_Loose_DPhi->Write("", TObject::kOverwrite);
  //hQCD_Est_S_DPhi->Write("", TObject::kOverwrite);

  // clean up
  ofile->Close();

}
