
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

int main(int argc, char* argv[]) {

  TH1::SetDefaultSumw2();

  ProgramOptions options(argc, argv);

  double lumi = options.lumi;

  std::cout << "Integrated luminosity : " << lumi << " pb-1" << std::endl;

  // input datasets
  Datasets datasets(options.iDir);
  datasets.readFile(options.datasetFile);

  // output file
  TFile* ofile = TFile::Open( (options.oDir+std::string("/ZBackground.root")).c_str(), "UPDATE");

  // cuts
  Cuts cuts;
  TCut puWeight("puWeight");

  TCut cutZMuMu_C = puWeight * cuts.zMuMuVBF();

  // histograms
  double dphiEdges[4] = { 0., 1.0, 2.6, TMath::Pi() };

  TH1D* hZ_DY_C_DPhi = new TH1D("hZ_DY_C_DPhi", "", 3, dphiEdges);  // Z+jets MC ctrl region
  TH1D* hZ_BG_C_DPhi = new TH1D("hZ_BG_C_DPhi", "", 3, dphiEdges);  // background MC ctrl region
  TH1D* hZ_Data_C_DPhi = new TH1D("hZ_Data_C_DPhi", "", 3, dphiEdges);  // Data ctrl region

  // loop over MC datasets
  for (unsigned i=0; i<datasets.size(); ++i) {

    Dataset dataset = datasets.getDataset(i);
    
    // check if it's DYJets
    bool isDY = false;
    if (dataset.name.compare(0,2,"DY")==0) {
      isDY = true;
      std::cout << "Analysing DY->ll MC : " << dataset.name << std::endl;
    }
    else if (dataset.isData) {
      std::cout << "Analysing Data      : " << dataset.name << std::endl;
    }
    else {
      std::cout << "Analysing BG MC     : " << dataset.name << std::endl;
    }

    // get file & tree
    TFile* file = datasets.getTFile(dataset.name);
    TTree* tree = (TTree*) file->Get("invHiggsInfo/InvHiggsInfo");

    // fill tmp histograms for BG estimation
    TH1D* hZ_C_DPhi = new TH1D("hZ_C_DPhi", "", 3, dphiEdges);  // this is for the actual BG estimation

    tree->Draw("vbfDPhi>>hZ_C_DPhi", cutZMuMu_C);

    // weight  to lumi
    double weight = (dataset.isData ? 1. : lumi * dataset.sigma / dataset.nEvents);

    // add to output histograms
    if (dataset.isData) {
      hZ_Data_C_DPhi->Add(hZ_C_DPhi);
    }
    else if (isDY) {
      hZ_DY_C_DPhi->Add(hZ_C_DPhi, weight);
    }
    else {
      hZ_BG_C_DPhi->Add(hZ_C_DPhi, weight);
    }

    std::cout << "  N ctrl (dphi>2.6) : " << weight * hZ_C_DPhi->GetBinContent(3) << std::endl;
    
    delete hZ_C_DPhi;

    // per-dataset control plots (just an example, add more later)
    ofile->cd();

    std::string hname = std::string("hZ_mZ_")+dataset.name;
    TH1D* hZ_mZ = new TH1D(hname.c_str(), "", 30, 60., 120.);
    std::string str = std::string("zMass>>")+hname;
    tree->Draw(str.c_str(), cutZMuMu_C);
    hZ_mZ->Scale(weight);
    hZ_mZ->Write("",TObject::kOverwrite);

    // clean up
    delete tree;
    file->Close();
   
  }

  // numbers - calculate these from MC in this program later!
  double ratioBF = 5.626;  //  MCFM + NLO
  double eps_mumu = 0.290;
  double eps_s_vbf = 0.0194;
  double eps_c_vbf = 0.0315;

  double f = (ratioBF * eps_s_vbf) / (eps_mumu * eps_c_vbf);

  std::cout << "Efficiencies" << std::endl;
  std::cout << "  eps_mumu   : " << eps_mumu << std::endl;
  std::cout << "  eps_s_vbf   : " << eps_s_vbf << std::endl;
  std::cout << "  eps_c_vbf   : " << eps_c_vbf << std::endl;
  std::cout << "  ratio       : " << eps_s_vbf/eps_c_vbf << std::endl;
  std::cout << std::endl <<std::endl;

  TH1D* hZ_Est_C_DPhi = new TH1D("hZ_Est_C_DPhi", "", 3, dphiEdges); // estimated W in ctrl region
  TH1D* hZ_Est_S_DPhi = new TH1D("hZ_Est_S_DPhi", "", 3, dphiEdges); // estimated W in bkgrnd region
  
  hZ_Est_C_DPhi->Add(hZ_Data_C_DPhi, hZ_BG_C_DPhi, 1., -1.);
  hZ_Est_S_DPhi->Add(hZ_Est_C_DPhi, f);

  std::cout << std::endl;
  std::cout << "dphi>2.6" << std::endl;
  std::cout << "  DY+jets MC ctrl region : " << hZ_DY_C_DPhi->GetBinContent(3) << " +/- " << hZ_DY_C_DPhi->GetBinError(3) << std::endl;
  std::cout << "  Background ctrl region : " << hZ_BG_C_DPhi->GetBinContent(3) << " +/- " << hZ_BG_C_DPhi->GetBinError(3) << std::endl;
  std::cout << "  Data ctrl region       : " << hZ_Data_C_DPhi->GetBinContent(3) << " +/- " << hZ_Data_C_DPhi->GetBinError(3) << std::endl;
  std::cout << std::endl;
  std::cout << "  Z in ctrl region       : " << hZ_Est_C_DPhi->GetBinContent(3) << " +/- " << hZ_Est_C_DPhi->GetBinError(3) << std::endl;
  std::cout << "  Z in sgnl region       : " << hZ_Est_S_DPhi->GetBinContent(3) << " +/- " << hZ_Est_S_DPhi->GetBinError(3) << std::endl;
  std::cout << std::endl << std::endl;
  std::cout << "dphi<1.0" << std::endl;
  std::cout << "  DY+jets MC ctrl region : " << hZ_DY_C_DPhi->GetBinContent(1) << " +/- " << hZ_DY_C_DPhi->GetBinError(1) << std::endl;
  std::cout << "  Background ctrl region : " << hZ_BG_C_DPhi->GetBinContent(1) << " +/- " << hZ_BG_C_DPhi->GetBinError(1) << std::endl;
  std::cout << "  Data ctrl region       : " << hZ_Data_C_DPhi->GetBinContent(1) << " +/- " << hZ_Data_C_DPhi->GetBinError(1) << std::endl;
  std::cout << std::endl;
  std::cout << "  Z in ctrl region       : " << hZ_Est_C_DPhi->GetBinContent(1) << " +/- " << hZ_Est_C_DPhi->GetBinError(1) << std::endl;
  std::cout << "  Z in sgnl region       : " << hZ_Est_S_DPhi->GetBinContent(1) << " +/- " << hZ_Est_S_DPhi->GetBinError(1) << std::endl;
  std::cout << std::endl << std::endl;


  //store histograms
  ofile->cd();
  hZ_DY_C_DPhi->Write("",TObject::kOverwrite);
  hZ_BG_C_DPhi->Write("",TObject::kOverwrite);
  hZ_Data_C_DPhi->Write("",TObject::kOverwrite);
//   hZ_R_DPhi->Write("",TObject::kOverwrite);
  hZ_Est_C_DPhi->Write("",TObject::kOverwrite);
  hZ_Est_S_DPhi->Write("",TObject::kOverwrite);

  ofile->Close();    

}
