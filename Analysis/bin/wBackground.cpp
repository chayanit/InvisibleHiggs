
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

  Datasets datasets(options.iDir);
  datasets.readFile(options.datasetFile);

  // output file
  TFile* ofile = TFile::Open( (options.oDir+std::string("/WBackground.root")).c_str(), "UPDATE");

  // cuts
  Cuts cuts;
  TCut puWeight("puWeight");

  TCut cutWMu_Gen_C = puWeight * (cuts.wMuGen() + cuts.wMuVBF());
  TCut cutWMu_Gen0P_C = puWeight * (cuts.wMuGen0P() + cuts.wMuVBF());
  TCut cutWMu_Gen_S = puWeight * (cuts.wMuGen() + cuts.allCutsNoDPhi());
  TCut cutWMu_Gen0P_S = puWeight * (cuts.wMuGen0P() + cuts.allCutsNoDPhi());
  TCut cutWMu_C = puWeight * (cuts.wMuVBF());
  TCut cutWMu_S = puWeight * (cuts.allCutsNoDPhi());

  TCut cutWEl_Gen_C = puWeight * (cuts.wElGen() + cuts.wElVBF());
  TCut cutWEl_Gen0P_C = puWeight * (cuts.wElGen0P() + cuts.wElVBF());
  TCut cutWEl_Gen_S = puWeight * (cuts.wElGen() + cuts.allCutsNoDPhi());
  TCut cutWEl_Gen0P_S = puWeight * (cuts.wElGen0P() + cuts.allCutsNoDPhi());
  TCut cutWEl_C = puWeight * (cuts.wElVBF());
  TCut cutWEl_S = puWeight * (cuts.allCutsNoDPhi());

  // histograms
  double dphiEdges[4] = { 0., 1.0, 2.6, TMath::Pi() };

  TH1D* hWMu_MCC_DPhi = new TH1D("hWMu_MCC_DPhi", "", 3, dphiEdges);  // W+jets MC ctrl region
  TH1D* hWMu_MCS_DPhi = new TH1D("hWMu_MCS_DPhi", "", 3, dphiEdges);  // W+jets MC sgnl region
  TH1D* hWMu_BGC_DPhi = new TH1D("hWMu_BGC_DPhi", "", 3, dphiEdges);  // background MC ctrl region
  //  TH1D* hWMu_BGS_DPhi = new TH1D("hWMu_BGS_DPhi", "", 3, dphiEdges);  // background MC sgnl region
  TH1D* hWMu_DataC_DPhi = new TH1D("hWMu_DataC_DPhi", "", 3, dphiEdges);  // Data ctrl region

  TH1D* hWEl_MCC_DPhi = new TH1D("hWEl_MCC_DPhi", "", 3, dphiEdges);  // W+jets MC ctrl region
  TH1D* hWEl_MCS_DPhi = new TH1D("hWEl_MCS_DPhi", "", 3, dphiEdges);  // W+jets MC sgnl region
  TH1D* hWEl_BGC_DPhi = new TH1D("hWEl_BGC_DPhi", "", 3, dphiEdges);  // background MC ctrl region
  //  TH1D* hWEl_BGS_DPhi = new TH1D("hWEl_BGS_DPhi", "", 3, dphiEdges);  // background MC sgnl region
  TH1D* hWEl_DataC_DPhi = new TH1D("hWEl_DataC_DPhi", "", 3, dphiEdges);  // Data ctrl region

  // loop over MC datasets
  for (unsigned i=0; i<datasets.size(); ++i) {

    Dataset dataset = datasets.getDataset(i);
    
    // check it's  W+Jets
    bool isWInclusive = false;
    bool isWNJets = false;
    if (dataset.name == "WJets") {
      isWInclusive = true;
      std::cout << "Analysing W MC     : " << dataset.name << std::endl;
    }
    else if (dataset.name == "W1Jets" || 
	     dataset.name == "W2Jets" || 
	     dataset.name == "W3Jets" || 
	     dataset.name == "W4Jets") {
      isWNJets = true;
      std::cout << "Analysing W MC     : " << dataset.name << std::endl;
    }
    else if (dataset.isData) {
      std::cout << "Analysing Data     : " << dataset.name << std::endl;
    }
    else {
      std::cout << "Analysing BG MC    : " << dataset.name << std::endl;
    }

    TFile* file = datasets.getTFile(dataset.name);
    TTree* tree = (TTree*) file->Get("invHiggsInfo/InvHiggsInfo");

    // tmp histograms
    TH1D* hWMu_C_DPhi = new TH1D("hWMu_C_DPhi", "", 3, dphiEdges);  // W+jets MC ctrl region
    TH1D* hWMu_S_DPhi = new TH1D("hWMu_S_DPhi", "", 3, dphiEdges);  // W+jets MC sgnl region
    TH1D* hWEl_C_DPhi = new TH1D("hWEl_C_DPhi", "", 3, dphiEdges);  // W+jets MC ctrl region
    TH1D* hWEl_S_DPhi = new TH1D("hWEl_S_DPhi", "", 3, dphiEdges);  // W+jets MC sgnl region

    double weight = lumi * dataset.sigma / dataset.nEvents;

    if (isWInclusive) {
      tree->Draw("vbfDPhi>>hWMu_C_DPhi", cutWMu_Gen0P_C);
      tree->Draw("vbfDPhi>>hWMu_S_DPhi", cutWMu_Gen0P_S);
      tree->Draw("vbfDPhi>>hWEl_C_DPhi", cutWEl_Gen0P_C);
      tree->Draw("vbfDPhi>>hWEl_S_DPhi", cutWEl_Gen0P_S);
      hWMu_MCC_DPhi->Add(hWMu_C_DPhi, weight);
      hWMu_MCS_DPhi->Add(hWMu_S_DPhi, weight);
      hWEl_MCC_DPhi->Add(hWEl_C_DPhi, weight);
      hWEl_MCS_DPhi->Add(hWEl_S_DPhi, weight);
    }
    else if (isWNJets) {
      tree->Draw("vbfDPhi>>hWMu_C_DPhi", cutWMu_Gen_C);
      tree->Draw("vbfDPhi>>hWMu_S_DPhi", cutWMu_Gen_S);
      tree->Draw("vbfDPhi>>hWEl_C_DPhi", cutWEl_Gen_C);
      tree->Draw("vbfDPhi>>hWEl_S_DPhi", cutWEl_Gen_S);
      hWMu_MCC_DPhi->Add(hWMu_C_DPhi, weight);
      hWMu_MCS_DPhi->Add(hWMu_S_DPhi, weight);
      hWEl_MCC_DPhi->Add(hWEl_C_DPhi, weight);
      hWEl_MCS_DPhi->Add(hWEl_S_DPhi, weight);
    }
    else if (dataset.isData) {
      tree->Draw("vbfDPhi>>hWMu_C_DPhi", cutWMu_C);
      tree->Draw("vbfDPhi>>hWEl_C_DPhi", cutWEl_C);
      hWMu_DataC_DPhi->Add(hWMu_C_DPhi);
      hWEl_DataC_DPhi->Add(hWEl_C_DPhi);
    }
    else {  // must be a BG dataset
      tree->Draw("vbfDPhi>>hWMu_C_DPhi", cutWMu_C);
      //      tree->Draw("vbfDPhi>>hWMu_S_DPhi", cutWMu_S);
      tree->Draw("vbfDPhi>>hWEl_C_DPhi", cutWEl_C);
      //      tree->Draw("vbfDPhi>>hWEl_S_DPhi", cutWEl_S);
      hWMu_BGC_DPhi->Add(hWMu_C_DPhi, weight);
      //      hWMu_BGS_DPhi->Add(hWMu_S_DPhi, weight);
      hWEl_BGC_DPhi->Add(hWEl_C_DPhi, weight);
      //      hWEl_BGS_DPhi->Add(hWEl_S_DPhi, weight);
    }

    // debug output
    std::cout << "  N ctrl region (dphi<1) : " << hWMu_C_DPhi->GetBinContent(1) << " +/- " << hWMu_C_DPhi->GetBinError(1) << std::endl;
    
    delete hWMu_C_DPhi;
    delete hWMu_S_DPhi;
    delete hWEl_C_DPhi;
    delete hWEl_S_DPhi;

    // per-dataset control plots (just an example, add more later)
    ofile->cd();

    std::string hname = std::string("hWMu_WmT_")+dataset.name;
    TH1D* hWMu_WmT = new TH1D(hname.c_str(), "", 40, 0., 120.);
    std::string str = std::string("wMt>>")+hname;
    tree->Draw(str.c_str(), cutWMu_C);
    hWMu_WmT->Write("",TObject::kOverwrite);

    hname = std::string("hWEl_WmT_")+dataset.name;
    TH1D* hWEl_WmT = new TH1D(hname.c_str(), "", 40, 0., 120.);
    str = std::string("wMt>>")+hname;
    tree->Draw(str.c_str(), cutWEl_C);
    hWEl_WmT->Write("",TObject::kOverwrite);

    delete tree;
    file->Close();
   
  }


  // create histograms with the background estimate
  TH1D* hWMu_R_DPhi    = new TH1D("hWMu_R_DPhi", "", 3, dphiEdges);  // ratio of sngl/ctrl
  TH1D* hWMu_EstC_DPhi = new TH1D("hWMu_EstC_DPhi", "", 3, dphiEdges); // estimated W in ctrl region
  TH1D* hWMu_EstS_DPhi = new TH1D("hWMu_EstS_DPhi", "", 3, dphiEdges); // estimated W in signal region

  TH1D* hWEl_R_DPhi    = new TH1D("hWEl_R_DPhi", "", 3, dphiEdges);
  TH1D* hWEl_EstC_DPhi = new TH1D("hWEl_EstC_DPhi", "", 3, dphiEdges);
  TH1D* hWEl_EstS_DPhi = new TH1D("hWEl_EstS_DPhi", "", 3, dphiEdges);

  hWMu_R_DPhi->Divide(hWMu_MCS_DPhi, hWMu_MCC_DPhi, 1., 1.);
  hWMu_EstC_DPhi->Add(hWMu_DataC_DPhi, hWMu_BGC_DPhi, 1., -1.);
  hWMu_EstS_DPhi->Multiply(hWMu_EstC_DPhi, hWMu_R_DPhi, 1., 1.);

  hWEl_R_DPhi->Divide(hWEl_MCS_DPhi, hWEl_MCC_DPhi, 1., 1.);
  hWEl_EstC_DPhi->Add(hWEl_DataC_DPhi, hWEl_BGC_DPhi, 1., -1.);
  hWEl_EstS_DPhi->Multiply(hWEl_EstC_DPhi, hWEl_R_DPhi, 1., 1.);

  TH1D* hW_Est_S_DPhi = new TH1D("hW_Est_S_DPhi", "", 3, dphiEdges); 
  hW_Est_S_DPhi->Add(hWMu_EstS_DPhi, hWEl_EstS_DPhi, 1., 1.);


  std::cout << std::endl;
  std::cout << "W->mu channel (dphi>2.6)" << std::endl;
  std::cout << "  W+jets MC  ctrl region : " << hWMu_MCC_DPhi->GetBinContent(3) << std::endl;
  std::cout << "  W+jets MC  sgnl region : " << hWMu_MCS_DPhi->GetBinContent(3) << std::endl;
  std::cout << "  Background ctrl region : " << hWMu_BGC_DPhi->GetBinContent(3) << std::endl;
  //  std::cout << "  Background sgnl region : " << hWMu_BGS_DPhi->GetBinContent(3) << std::endl;
  std::cout << "  Data ctrl region       : " << hWMu_DataC_DPhi->GetBinContent(3) << std::endl;
  std::cout << std::endl;
  std::cout << "  W in ctrl region       : " << hWMu_EstC_DPhi->GetBinContent(3) << std::endl;
  std::cout << "  N_S(MC)/N_C(MC)        : " << hWMu_R_DPhi->GetBinContent(3) << std::endl;
  std::cout << "  W in sgnl region       : " << hWMu_EstS_DPhi->GetBinContent(3) << std::endl;
  std::cout << std::endl << std::endl;
  std::cout << "W->el channel (dphi>2.6)" << std::endl;
  std::cout << "  W+jets MC  ctrl region : " << hWEl_MCC_DPhi->GetBinContent(3) << std::endl;
  std::cout << "  W+jets MC  sgnl region : " << hWEl_MCS_DPhi->GetBinContent(3) << std::endl;
  std::cout << "  Background ctrl region : " << hWEl_BGC_DPhi->GetBinContent(3) << std::endl;
  //  std::cout << "  Background sgnl region : " << hWEl_BGS_DPhi->GetBinContent(3) << std::endl;
  std::cout << "  Data ctrl region       : " << hWEl_DataC_DPhi->GetBinContent(3) << std::endl;
  std::cout << std::endl;
  std::cout << "  W in ctrl region       : " << hWEl_EstC_DPhi->GetBinContent(3) << std::endl;
  std::cout << "  N_S(MC)/N_C(MC)        : " << hWEl_R_DPhi->GetBinContent(3) << std::endl;
  std::cout << "  W in sgnl region       : " << hWEl_EstS_DPhi->GetBinContent(3) << std::endl;
  std::cout << std::endl << std::endl;
  std::cout << "W->mu channel (dphi<1.0)" << std::endl;
  std::cout << "  W+jets MC  ctrl region : " << hWMu_MCC_DPhi->GetBinContent(1) << std::endl;
  std::cout << "  W+jets MC  sgnl region : " << hWMu_MCS_DPhi->GetBinContent(1) << std::endl;
  std::cout << "  Background ctrl region : " << hWMu_BGC_DPhi->GetBinContent(1) << std::endl;
  //  std::cout << "  Background sgnl region : " << hWMu_BGS_DPhi->GetBinContent(1) << std::endl;
  std::cout << "  Data ctrl region       : " << hWMu_DataC_DPhi->GetBinContent(1) << std::endl;
  std::cout << std::endl;
  std::cout << "  W in ctrl region       : " << hWMu_EstC_DPhi->GetBinContent(1) << std::endl;
  std::cout << "  N_S(MC)/N_C(MC)        : " << hWMu_R_DPhi->GetBinContent(1) << std::endl;
  std::cout << "  W in sgnl region       : " << hWMu_EstS_DPhi->GetBinContent(1) << std::endl;
  std::cout << std::endl << std::endl;
  std::cout << "W->el channel (dphi<1.0)" << std::endl;
  std::cout << "  W+jets MC  ctrl region : " << hWEl_MCC_DPhi->GetBinContent(1) << std::endl;
  std::cout << "  W+jets MC  sgnl region : " << hWEl_MCS_DPhi->GetBinContent(1) << std::endl;
  std::cout << "  Background ctrl region : " << hWEl_BGC_DPhi->GetBinContent(1) << std::endl;
  //  std::cout << "  Background sgnl region : " << hWEl_BGS_DPhi->GetBinContent(1) << std::endl;
  std::cout << "  Data ctrl region       : " << hWEl_DataC_DPhi->GetBinContent(1) << std::endl;
  std::cout << std::endl;
  std::cout << "  W in ctrl region       : " << hWEl_EstC_DPhi->GetBinContent(1) << std::endl;
  std::cout << "  N_S(MC)/N_C(MC)        : " << hWEl_R_DPhi->GetBinContent(1) << std::endl;
  std::cout << "  W in sgnl region       : " << hWEl_EstS_DPhi->GetBinContent(1) << std::endl;
  std::cout << std::endl;
  std::cout << "Total W (dphi<1.0)" << std::endl;
  std::cout << "  W in sgnl region       : " << hW_Est_S_DPhi->GetBinContent(1) << std::endl;


  // store histograms
  ofile->cd();
    
  hWMu_MCC_DPhi->Write("",TObject::kOverwrite);
  hWMu_MCS_DPhi->Write("",TObject::kOverwrite);
  hWMu_BGC_DPhi->Write("",TObject::kOverwrite);
  //  hWMu_BGS_DPhi->Write("",TObject::kOverwrite);
  hWMu_DataC_DPhi->Write("",TObject::kOverwrite);
  hWMu_R_DPhi->Write("",TObject::kOverwrite);
  hWMu_EstC_DPhi->Write("",TObject::kOverwrite);
  hWMu_EstS_DPhi->Write("",TObject::kOverwrite);

  hWEl_MCC_DPhi->Write("",TObject::kOverwrite);
  hWEl_MCS_DPhi->Write("",TObject::kOverwrite);
  hWEl_BGC_DPhi->Write("",TObject::kOverwrite);
  //  hWEl_BGS_DPhi->Write("",TObject::kOverwrite);
  hWEl_DataC_DPhi->Write("",TObject::kOverwrite);
  hWEl_R_DPhi->Write("",TObject::kOverwrite);
  hWEl_EstC_DPhi->Write("",TObject::kOverwrite);
  hWEl_EstS_DPhi->Write("",TObject::kOverwrite);

  hW_Est_S_DPhi->Write("",TObject::kOverwrite);
      
  ofile->Close();    

}
