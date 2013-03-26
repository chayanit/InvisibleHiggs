
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
  TFile* ofile = TFile::Open( (options.oDir+std::string("/ZBackground.root")).c_str(), "UPDATE");

  // cuts
  Cuts cuts;
  TCut puWeight("puWeight");

  TCut cutQCDNoMET = puWeight * cuts.qcdNoMET();
  TCut cutQCDLoose = puWeight * cuts.qcdLoose();
  TCut cutQCDTightHiDPhi = puWeight * cuts.qcdTightHiDPhi();

  // histograms
  double dphiEdges[4] = { 0., 1.0, 2.6, TMath::Pi() };

  TH1D* hQCD_BG_NoMET_DPhi = new TH1D("hQCD_BG_NoMET_DPhi", "", 3, dphiEdges);  // BG in NoMET region
  TH1D* hQCD_BG_Loose_DPhi = new TH1D("hQCD_BG_Loose_DPhi", "", 3, dphiEdges);  // BG in Loose region
  TH1D* hQCD_BG_Tight_DPhi = new TH1D("hQCD_BG_Tight_DPhi", "", 3, dphiEdges);  // BG in tight region

  TH1D* hQCD_Data_NoMET_DPhi = new TH1D("hQCD_Data_NoMET_DPhi", "", 3, dphiEdges);  // Data in NoMET region
  TH1D* hQCD_Data_Loose_DPhi = new TH1D("hQCD_Data_Loose_DPhi", "", 3, dphiEdges);  // Data in Loose region
  TH1D* hQCD_Data_Tight_DPhi = new TH1D("hQCD_Data_Tight_DPhi", "", 3, dphiEdges);  // Data in tight region

  // loop over MC datasets
  for (unsigned i=0; i<datasets.size(); ++i) {

    Dataset dataset = datasets.getDataset(i);
    
    // check if it's DYJets
    if (dataset.isData) {
      std::cout << "Analysing Data      : " << dataset.name << std::endl;
    }
    else {
      std::cout << "Analysing BG MC     : " << dataset.name << std::endl;
    }

    // get file & tree
    TFile* file = datasets.getTFile(dataset.name);
    TTree* tree = (TTree*) file->Get("invHiggsInfo/InvHiggsInfo");

    // fill tmp histograms for BG estimation
    TH1D* hQCD_NoMET_DPhi = new TH1D("hQCD_NoMET_DPhi", "", 3, dphiEdges);  // this is for the actual BG estimation
    TH1D* hQCD_Loose_DPhi = new TH1D("hQCD_Loose_DPhi", "", 3, dphiEdges);  // this is for the actual BG estimation
    TH1D* hQCD_Tight_DPhi = new TH1D("hQCD_Tight_DPhi", "", 3, dphiEdges);  // this is for the actual BG estimation

    tree->Draw("vbfDPhi>>hQCD_NoMET_DPhi", cutQCDNoMET);
    tree->Draw("vbfDPhi>>hQCD_Loose_DPhi", cutQCDLoose);
    tree->Draw("vbfDPhi>>hQCD_Tight_DPhi", cutQCDTightHiDPhi);

    // weight  to lumi
    double weight = (dataset.isData ? 1. : lumi * dataset.sigma / dataset.nEvents);

    // add to output histograms
    if (dataset.isData) {
      hQCD_Data_NoMET_DPhi->Add(hQCD_NoMET_DPhi);
      hQCD_Data_Loose_DPhi->Add(hQCD_Loose_DPhi);
      hQCD_Data_Tight_DPhi->Add(hQCD_Tight_DPhi);
    }
    else {
      hQCD_BG_NoMET_DPhi->Add(hQCD_NoMET_DPhi, weight);
      hQCD_BG_Loose_DPhi->Add(hQCD_Loose_DPhi, weight);
      hQCD_BG_Tight_DPhi->Add(hQCD_Tight_DPhi, weight);
    }

    std::cout << "  N (met>70, dphi>2.6) : " << weight * hQCD_Loose_DPhi->GetBinContent(3) << std::endl;
    
    delete hQCD_NoMET_DPhi;
    delete hQCD_Loose_DPhi;
    delete hQCD_Tight_DPhi;

    // per-dataset control plots (just an example, add more later)
    ofile->cd();

    std::string hname = std::string("hQCD_Loose_DPhi2_")+dataset.name;
    TH1D* hQCD_Loose_DPhi2 = new TH1D(hname.c_str(), "", 72, 0., TMath::Pi());
    std::string str = std::string("vbfDPhi>>")+hname;
    tree->Draw(str.c_str(), cutQCDLoose);
    hQCD_Loose_DPhi2->Scale(weight);
    hQCD_Loose_DPhi2->Write("",TObject::kOverwrite);

    // clean up
    delete tree;
    file->Close();
   
  }

  // get data-driven background estimates
  std::cout << "Reading Z backgrounds from : " << options.oDir+std::string("/ZBackground.root") << std::endl;
  TFile* zfile = TFile::Open( (options.oDir+std::string("/ZBackground.root")).c_str(), "READ");
//   TH1D* hQCD_Z_NoMET_DPhi = (TH1D*) zfile->Get("hZ_Est_QCDNoMET_DPhi");
//   TH1D* hQCD_Z_Loose_DPhi = (TH1D*) zfile->Get("hZ_Est_QCDLoose_DPhi");
  TH1D* hQCD_Z_Tight_DPhi = (TH1D*) zfile->Get("hZ_Est_S_DPhi");

  if (hQCD_Z_Tight_DPhi == 0) {
    std::cerr << "Could not read Z background histogram" << std::endl;
    std::exit(1);
  }  
  
  std::cout << "Reading W backgrounds from : " << options.oDir+std::string("/WBackground.root") << std::endl;
  TFile* wfile = TFile::Open( (options.oDir+std::string("/WBackground.root")).c_str(), "READ");
//   TH1D* hQCD_W_NoMET_DPhi = (TH1D*) wfile->Get("hW_Est_QCDNoMET_DPhi");
//   TH1D* hQCD_W_Loose_DPhi = (TH1D*) wfile->Get("hW_Est_QCDLoose_DPhi");
  TH1D* hQCD_W_Tight_DPhi = (TH1D*) wfile->Get("hW_Est_S_DPhi");

  if (hQCD_W_Tight_DPhi == 0) {
    std::cerr << "Could not read W background histogram" << std::endl;
    std::exit(1);
  }  

  // create output histograms
  TH1D* hQCD_Est_NoMET_DPhi = new TH1D("hQCD_Est_NoMET_DPhi", "", 3, dphiEdges);
  TH1D* hQCD_Est_Loose_DPhi = new TH1D("hQCD_Est_Loose_DPhi", "", 3, dphiEdges);
  TH1D* hQCD_Est_S_DPhi     = new TH1D("hQCD_Est_S_DPhi",     "", 3, dphiEdges);

  // do the background estimation
  hQCD_Est_NoMET_DPhi->Add(hQCD_Data_NoMET_DPhi, hQCD_BG_NoMET_DPhi, 1., -1.);
  hQCD_Est_Loose_DPhi->Add(hQCD_Data_Loose_DPhi, hQCD_BG_Loose_DPhi, 1., -1.);
  hQCD_Est_S_DPhi->Add(hQCD_Data_Tight_DPhi, hQCD_BG_Tight_DPhi, 1., -1.);

//   hQCD_Est_NoMET_DPhi->Add(hQCD_Z_NoMET_DPhi, -1.);
//   hQCD_Est_Loose_DPhi->Add(hQCD_Z_Loose_DPhi, -1.);
  hQCD_Est_S_DPhi->Add(hQCD_Z_Tight_DPhi, -1.);

//   hQCD_Est_NoMET_DPhi->Add(hQCD_W_NoMET_DPhi, -1.);
//   hQCD_Est_Loose_DPhi->Add(hQCD_W_Loose_DPhi, -1.);
//  hQCD_Est_S_DPhi->Add(hQCD_W_Tight_DPhi, -1.);

  // calculate ratios
  double rNoMET = hQCD_Est_NoMET_DPhi->GetBinContent(1) / hQCD_Est_NoMET_DPhi->GetBinContent(3);
  double err_rNoMET = 0.;
  double rLoose = hQCD_Est_Loose_DPhi->GetBinContent(1) / hQCD_Est_Loose_DPhi->GetBinContent(3);
  double err_rLoose = 0.;
  
  double rTight = rLoose;  // for now
  double err_rTight = err_rLoose;

  // predict signal
  double nQCD_Est_S_HiDPhi = rTight * hQCD_Est_S_DPhi->GetBinContent(3);
  double err_nQCD_Est_S_HiDPhi = 0.;

  // set bins in output histogram
  hQCD_Est_S_DPhi->SetBinContent(1, nQCD_Est_S_HiDPhi);
  hQCD_Est_S_DPhi->SetBinError(1, err_nQCD_Est_S_HiDPhi);

  // print results
  std::cout << std::endl;
  std::cout << "QCD Background estimate" << std::endl << std::endl;
  std::cout << "No MET region (MET>0, dphi>2.6)" << std::endl;
  std::cout << "   N data     : " << hQCD_Data_NoMET_DPhi->GetBinContent(3) << " +/- " << hQCD_Data_NoMET_DPhi->GetBinError(3) << std::endl;
  std::cout << "   N BG (MC)  : " << hQCD_BG_NoMET_DPhi->GetBinContent(3) << " +/- " << hQCD_BG_NoMET_DPhi->GetBinError(3) << std::endl;
//   std::cout << "   N BG (Z)   : " << hQCD_Z_NoMET_DPhi->GetBinContent(3) << " +/- " << hQCD_Z_NoMET_DPhi->GetBinError(3) << std::endl;
//   std::cout << "   N BG (W)   : " << hQCD_W_NoMET_DPhi->GetBinContent(3) << " +/- " << hQCD_W_NoMET_DPhi->GetBinError(3) << std::endl;
  std::cout << "   N QCD (est): " << hQCD_Est_S_DPhi->GetBinContent(3) << " +/- " << hQCD_Est_S_DPhi->GetBinError(3) << std::endl;
  std::cout << "No MET region (MET>0, dphi<1.0)" << std::endl;
  std::cout << "   N data     : " << hQCD_Data_NoMET_DPhi->GetBinContent(1) << " +/- " << hQCD_Data_NoMET_DPhi->GetBinError(1) << std::endl;
  std::cout << "   N BG (MC)  : " << hQCD_BG_NoMET_DPhi->GetBinContent(1) << " +/- " << hQCD_BG_NoMET_DPhi->GetBinError(1) << std::endl;
//   std::cout << "   N BG (Z)   : " << hQCD_Z_NoMET_DPhi->GetBinContent(1) << " +/- " << hQCD_Z_NoMET_DPhi->GetBinError(1) << std::endl;
//   std::cout << "   N BG (W)   : " << hQCD_W_NoMET_DPhi->GetBinContent(1) << " +/- " << hQCD_W_NoMET_DPhi->GetBinError(1) << std::endl;
  std::cout << "   N QCD (est): " << hQCD_Est_S_DPhi->GetBinContent(1) << " +/- " << hQCD_Est_S_DPhi->GetBinError(1) << std::endl;
  std::cout << std::endl;

  std::cout << "Loose region (MET>70, dphi>2.6)" << std::endl;
  std::cout << "   N data     : " << hQCD_Data_Loose_DPhi->GetBinContent(3) << " +/- " << hQCD_Data_Loose_DPhi->GetBinError(3) << std::endl;
  std::cout << "   N BG (MC)  : " << hQCD_BG_Loose_DPhi->GetBinContent(3) << " +/- " << hQCD_BG_Loose_DPhi->GetBinError(3) << std::endl;
//   std::cout << "   N BG (Z)   : " << hQCD_Z_Loose_DPhi->GetBinContent(3) << " +/- " << hQCD_Z_Loose_DPhi->GetBinError(3) << std::endl;
//   std::cout << "   N BG (W)   : " << hQCD_W_Loose_DPhi->GetBinContent(3) << " +/- " << hQCD_W_Loose_DPhi->GetBinError(3) << std::endl;
  std::cout << "   N QCD (est): " << hQCD_Est_S_DPhi->GetBinContent(3) << " +/- " << hQCD_Est_S_DPhi->GetBinError(3) << std::endl;
  std::cout << "No MET region (MET>0, dphi<1.0)" << std::endl;
  std::cout << "   N data     : " << hQCD_Data_Loose_DPhi->GetBinContent(1) << " +/- " << hQCD_Data_Loose_DPhi->GetBinError(1) << std::endl;
  std::cout << "   N BG (MC)  : " << hQCD_BG_Loose_DPhi->GetBinContent(1) << " +/- " << hQCD_BG_Loose_DPhi->GetBinError(1) << std::endl;
//   std::cout << "   N BG (Z)   : " << hQCD_Z_Loose_DPhi->GetBinContent(1) << " +/- " << hQCD_Z_Loose_DPhi->GetBinError(1) << std::endl;
//   std::cout << "   N BG (W)   : " << hQCD_W_Loose_DPhi->GetBinContent(1) << " +/- " << hQCD_W_Loose_DPhi->GetBinError(1) << std::endl;
  std::cout << "   N QCD (est): " << hQCD_Est_S_DPhi->GetBinContent(1) << " +/- " << hQCD_Est_S_DPhi->GetBinError(1) << std::endl;
  std::cout << std::endl;
  std::cout <<" Ratios" << std::endl;
  std::cout << "   R(MET>0)   : " << rNoMET << " +/- " << err_rNoMET << std::endl;
  std::cout << "   R(MET>70)  : " << rLoose << " +/- " << err_rLoose << std::endl;
  std::cout << std::endl;
  std::cout << "Control region (MET>130, dphi>2.6)" << std::endl;
  std::cout << "   N data     : " << hQCD_Data_Tight_DPhi->GetBinContent(3) << " +/- " << hQCD_Data_Tight_DPhi->GetBinError(3) << std::endl;
  std::cout << "   N BG (MC)  : " << hQCD_BG_Tight_DPhi->GetBinContent(3) << " +/- " << hQCD_BG_Tight_DPhi->GetBinError(3) << std::endl;
  std::cout << "   N BG (Z)   : " << hQCD_Z_Tight_DPhi->GetBinContent(3) << " +/- " << hQCD_Z_Tight_DPhi->GetBinError(3) << std::endl;
  std::cout << "   N BG (W)   : " << hQCD_W_Tight_DPhi->GetBinContent(3) << " +/- " << hQCD_W_Tight_DPhi->GetBinError(3) << std::endl;
  std::cout << "   N QCD (est): " << hQCD_Est_S_DPhi->GetBinContent(3) << " +/- " << hQCD_Est_S_DPhi->GetBinError(3) << std::endl;
  std::cout << std::endl;
  std::cout << "Signal region (MET>130, dphi<1.0)" << std::endl;
  std::cout << "   N QCD (est): " << hQCD_Est_S_DPhi->GetBinContent(1) << " +/- " << hQCD_Est_S_DPhi->GetBinError(1) << std::endl;

  // write out histograms
  ofile->cd();
  hQCD_Data_NoMET_DPhi->Write("", TObject::kOverwrite);
  hQCD_Data_Loose_DPhi->Write("", TObject::kOverwrite);
  hQCD_Data_Tight_DPhi->Write("", TObject::kOverwrite);
  hQCD_BG_NoMET_DPhi->Write("", TObject::kOverwrite);
  hQCD_BG_Loose_DPhi->Write("", TObject::kOverwrite);
  hQCD_BG_Tight_DPhi->Write("", TObject::kOverwrite);
  hQCD_Z_Tight_DPhi->Write("", TObject::kOverwrite);
  hQCD_W_Tight_DPhi->Write("", TObject::kOverwrite);
  hQCD_Est_NoMET_DPhi->Write("", TObject::kOverwrite);
  hQCD_Est_Loose_DPhi->Write("", TObject::kOverwrite);
  hQCD_Est_S_DPhi->Write("", TObject::kOverwrite);

  // clean up
  ofile->Close();

}
