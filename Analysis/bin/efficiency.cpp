
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

namespace po = boost::program_options;

int main(int argc, char* argv[]) {

  TH1::SetDefaultSumw2();

  ProgramOptions options(argc, argv);

  double lumi = options.lumi;
  std::string iDir = options.iDir;
  std::string oDir = options.oDir;

  // input datasets
  Datasets datasets(iDir);
  datasets.readFile(options.datasetFile);

  // cuts
  Cuts cuts;
  unsigned nCuts = cuts.nCuts();

  // output file
  TFile* ofile = TFile::Open( (options.oDir+std::string("/Efficiency.root")).c_str(), "UPDATE");

  // output histograms (for datasets with several components)
  TH1D* hQCD     = new TH1D("hCutFlow_QCD", "", nCuts, 0., nCuts);
  TH1D* hZNuNu   = new TH1D("hCutFlow_ZNuNu", "", nCuts, 0., nCuts);
  TH1D* hWLNu    = new TH1D("hCutFlow_WLNu", "", nCuts, 0., nCuts);
  TH1D* hWMuNu   = new TH1D("hCutFlow_WMuNu", "", nCuts, 0., nCuts);
  TH1D* hWElNu   = new TH1D("hCutFlow_WElNu", "", nCuts, 0., nCuts);
  TH1D* hWTauNu  = new TH1D("hCutFlow_WTauNu", "", nCuts, 0., nCuts);
  TH1D* hSingleT = new TH1D("hCutFlow_SingleTSum", "", nCuts, 0., nCuts);
  TH1D* hDiboson = new TH1D("hCutFlow_Diboson", "", nCuts, 0., nCuts);

  for (unsigned i=0; i<datasets.size(); ++i) {

    Dataset dataset = datasets.getDataset(i);

    if (dataset.isData) continue;

    std::cout << "Cut flow for " << dataset.name << std::endl;

    TFile* ifile = datasets.getTFile(dataset.name);
    TTree* tree = (TTree*) ifile->Get("invHiggsInfo/InvHiggsInfo");

    //cuts
    TCut cutDSet     = cuts.cutDataset(dataset.name);
    TCut puWeight("puWeight");
    TCut wWeight("");
    if (dataset.name.compare(0,1,"W")) wWeight = cuts.wWeight();
     
    // tmp histogram for counting.  Need a hist so we can use PU weight

    // do cutflow
    std::string hname = std::string("hCutFlow_")+dataset.name;
    TH1D* hCutFlow = new TH1D(hname.c_str(), "", nCuts, 0., nCuts);
    hname = std::string("hCutFlowMu_")+dataset.name;
    TH1D* hCutFlowMu = new TH1D(hname.c_str(), "", nCuts, 0., nCuts);
    hname = std::string("hCutFlowEl_")+dataset.name;
    TH1D* hCutFlowEl = new TH1D(hname.c_str(), "", nCuts, 0., nCuts);
    hname = std::string("hCutFlowTau_")+dataset.name;
    TH1D* hCutFlowTau = new TH1D(hname.c_str(), "", nCuts, 0., nCuts);

    for (unsigned c=0; c<nCuts; ++c) {

      TCut cut = puWeight * wWeight * (cutDSet + cuts.cutflow(c));
      TCut cutMu = puWeight * wWeight * (cutDSet + cuts.wMuGen() + cuts.cutflow(c));
      TCut cutEl = puWeight * wWeight * (cutDSet + cuts.wElGen() + cuts.cutflow(c));
      TCut cutTau = puWeight * wWeight * (cutDSet + cuts.wTauGen() + cuts.cutflow(c));

      //      std::cout << cut << std::endl;

      TH1D* h = new TH1D("h","", 1, 0., 1.);
      tree->Draw("0.5>>h", cut);
      hCutFlow->SetBinContent(c+1, h->GetBinContent(1));
      hCutFlow->SetBinError(c+1, h->GetBinError(1));

      TH1D* hMu = new TH1D("hMu","", 1, 0., 1.);
      tree->Draw("0.5>>hMu", cutMu);
      hCutFlowMu->SetBinContent(c+1, hMu->GetBinContent(1));
      hCutFlowMu->SetBinError(c+1, hMu->GetBinError(1));

      TH1D* hEl = new TH1D("hEl","", 1, 0., 1.);
      tree->Draw("0.5>>hEl", cutEl);
      hCutFlowEl->SetBinContent(c+1, hEl->GetBinContent(1));
      hCutFlowEl->SetBinError(c+1, hEl->GetBinError(1));

      TH1D* hTau = new TH1D("hTau","", 1, 0., 1.);
      tree->Draw("0.5>>hTau", cutTau);
      hCutFlowTau->SetBinContent(c+1, hTau->GetBinContent(1));
      hCutFlowTau->SetBinError(c+1, hTau->GetBinError(1));

      delete h;
      delete hMu;
      delete hEl;
      delete hTau;
    }

    hCutFlow->Scale( lumi * dataset.sigma / dataset.nEvents );
    hCutFlowMu->Scale( lumi * dataset.sigma / dataset.nEvents );
    hCutFlowEl->Scale( lumi * dataset.sigma / dataset.nEvents );
    hCutFlowTau->Scale( lumi * dataset.sigma / dataset.nEvents );

    // sum binned datasets
    if (dataset.name.compare(0,3,"QCD")==0) hQCD->Add(hCutFlow);
    if (dataset.name.compare(0,3,"Zvv")==0) hZNuNu->Add(hCutFlow);
    if (dataset.name.compare(0,1,"W")==0) {
      hWLNu->Add(hCutFlow);
      hWMuNu->Add(hCutFlowMu);
      hWElNu->Add(hCutFlowEl);
      hWTauNu->Add(hCutFlowTau);
    }
    if (dataset.name.compare(0,7,"SingleT")==0) hSingleT->Add(hCutFlow);
    if (dataset.name.compare(0,2,"WW")==0 ||
	dataset.name.compare(0,2,"WZ")==0 ||
	dataset.name.compare(0,2,"ZZ")==0 )
      hDiboson->Add(hCutFlow);

    ofile->cd();
    hCutFlow->Write("", TObject::kOverwrite);

    ifile->Close();

  }

  ofile->cd();
  hQCD->Write("",TObject::kOverwrite);  
  hZNuNu->Write("",TObject::kOverwrite);  
  hWLNu->Write("",TObject::kOverwrite);  
  hWMuNu->Write("",TObject::kOverwrite);  
  hWElNu->Write("",TObject::kOverwrite);  
  hWTauNu->Write("",TObject::kOverwrite);  
  hSingleT->Write("",TObject::kOverwrite);  
  hDiboson->Write("",TObject::kOverwrite);  

  // write TeX file
  std::cout << "Writing cut flow TeX file" << std::endl;

  ofstream texFile;
  texFile.open(options.oDir+std::string("/cutflow.tex"));

  texFile << "Cut & N($Z\\rightarrow\\nu\\nu) & N($W\\rightarrow l\\nu) & N(QCD) & N($t\\bar{t}$) & N(single $t$) & N(diboson) & N(signal $m_H=120$~\\GeV \\\\" << std::endl;

  TH1D* hTTbar = (TH1D*) ofile->Get("hCutFlow_TTBar");
  TH1D* hSignal = (TH1D*) ofile->Get("hCutFlow_SignalM125_POWHEG");

  // cutflow table
  for (unsigned i=0; i<nCuts; ++i) {

    texFile << cuts.cutName(i) << " & ";
    texFile << "$" << hZNuNu->GetBinContent(i+1) << " \\pm " << hZNuNu->GetBinError(i+1) << "$ & ";
    texFile << "$" << hWLNu->GetBinContent(i+1) << " \\pm " << hWLNu->GetBinError(i+1) << "$ & ";
    texFile << "$" << hQCD->GetBinContent(i+1) << " \\pm " << hQCD->GetBinError(i+1) << "$ & ";
    texFile << "$" << hTTbar->GetBinContent(i+1) << " \\pm " << hTTbar->GetBinError(i+1) << "$ & ";
    texFile << "$" << hSingleT->GetBinContent(i+1) << " \\pm " << hSingleT->GetBinError(i+1) << "$ & ";
    texFile << "$" << hDiboson->GetBinContent(i+1) << " \\pm " << hDiboson->GetBinError(i+1) << "$ & ";
    texFile << "$" << hSignal->GetBinContent(i+1) << " \\pm " << hSignal->GetBinError(i+1) << "$ \\\\ " << std::endl;

  }

  texFile << std::endl << std::endl;
  texFile.close();


  // signal efficiencies
  texFile.open(options.oDir+std::string("/signalEff.tex"));
  texFile << "Dataset & eff(%) & N \\\\" << std::endl;

  for (unsigned i=0; i<datasets.size(); ++i) {

    Dataset dataset = datasets.getDataset(i);

    if (!(dataset.name.compare(0,6,"Signal") ==0 ||
	  dataset.name.compare(0,6,"GluGlu") == 0 )) continue;

    std::string hname = std::string("hCutFlow_")+dataset.name;
    TH1D* weight = (TH1D*) ofile->Get(hname.c_str());

    double n      = weight->GetBinContent(nCuts);
    double err_n  = weight->GetBinError(nCuts);

    double eff    = n / (lumi * dataset.sigma);
    double err_eff = err_n  / (lumi *dataset.sigma);

    //    double n      = lumi * dataset.sigma * eff;
    //    double nErr   = lumi * dataset.sigma * effErr;


    texFile << dataset.name << " & $" 
	    << 100*eff << " \\pm " << 100*err_eff << "$ & $"
	    << n << " \\pm " << err_n << "$ \\\\" 
	    << std::endl;
    
  } 
  

  ofile->Close();
  texFile.close();

}
