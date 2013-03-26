
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
  std::string oDir = options.oDir+std::string("/Signal");

  // input datasets
  Datasets datasets(iDir);
  datasets.readFile(options.datasetFile);

  // output file
  TFile* ofile = TFile::Open( (options.oDir+std::string("/Efficiency.root")).c_str(), "UPDATE");

  // output histograms (for datasets with several components)
  TH1D* hQCD     = new TH1D("hCutFlow_QCD", "", 7, 0., 7.);
  TH1D* hZNuNu   = new TH1D("hCutFlow_ZNuNu", "", 7, 0., 7.);
  TH1D* hWLNu    = new TH1D("hCutFlow_WLNu", "", 7, 0., 7.);
  TH1D* hSingleT = new TH1D("hCutFlow_SingleT", "", 7, 0., 7.);
  TH1D* hDiboson = new TH1D("hCutFlow_Diboson", "", 7, 0., 7.);


  // cuts
  Cuts cuts;
  TCut puWeight("puWeight");

  TCut cutTrg   = puWeight * (cuts.cut("trigger") + cuts.cut("metFilter"));
  TCut cutVeto  = puWeight * (cuts.cut("trigger") + cuts.cut("metFilter") + cuts.cut("EVeto") + cuts.cut("MuVeto"));
  TCut cutDijet = puWeight * (cuts.cut("trigger") + cuts.cut("metFilter") + cuts.cut("EVeto") + cuts.cut("MuVeto") + cuts.cut("dijet") + cuts.cut("sgnEtaJJ"));
  TCut cutDEta  = puWeight * (cuts.cut("trigger") + cuts.cut("metFilter") + cuts.cut("EVeto") + cuts.cut("MuVeto") + cuts.cut("dijet") + cuts.cut("sgnEtaJJ") + cuts.cut("dEtaJJ"));
  TCut cutMET   = puWeight * (cuts.cut("trigger") + cuts.cut("metFilter") + cuts.cut("EVeto") + cuts.cut("MuVeto") + cuts.cut("dijet") + cuts.cut("sgnEtaJJ") + cuts.cut("dEtaJJ") + cuts.cut("met"));
  TCut cutMjj   = puWeight * (cuts.cut("trigger") + cuts.cut("metFilter") + cuts.cut("EVeto") + cuts.cut("MuVeto") + cuts.cut("dijet") + cuts.cut("sgnEtaJJ") + cuts.cut("dEtaJJ") + cuts.cut("met") + cuts.cut("Mjj"));
  TCut cutDPhi  = puWeight * cuts.allCuts();

  for (unsigned i=0; i<datasets.size(); ++i) {

    Dataset dataset = datasets.getDataset(i);

    if (dataset.isData) continue;

    TFile* ifile = datasets.getTFile(dataset.name);
    TTree* tree = (TTree*) ifile->Get("invHiggsInfo/InvHiggsInfo");
    
    // tmp histogram for counting.  Need a hist so we can use PU weight
    TH1D* weight = new TH1D("weight","", 1, 0., 1.);

    // do cutflow
    std::string hname = std::string("hCutFlow_")+dataset.name;
    TH1D* hCutFlow = new TH1D(hname.c_str(), "", 7, 0., 7.);

    weight->SetBinContent(1, 0.);
    weight->SetBinError(1, 0.);
    tree->Draw("0.5>>weight", cutTrg);
    hCutFlow->SetBinContent(1, weight->GetBinContent(1));
    hCutFlow->SetBinError(1, weight->GetBinError(1));

    weight->SetBinContent(1, 0.);
    weight->SetBinError(1, 0.);
    tree->Draw("0.5>>weight", cutVeto);
    hCutFlow->SetBinContent(2, weight->GetBinContent(1));
    hCutFlow->SetBinError(2, weight->GetBinError(1));

    weight->SetBinContent(1, 0.);
    weight->SetBinError(1, 0.);
    tree->Draw("0.5>>weight", cutDijet);
    hCutFlow->SetBinContent(3, weight->GetBinContent(1));
    hCutFlow->SetBinError(3, weight->GetBinError(1));

    weight->SetBinContent(1, 0.);
    weight->SetBinError(1, 0.);
    tree->Draw("0.5>>weight", cutDEta);
    hCutFlow->SetBinContent(4, weight->GetBinContent(1));
    hCutFlow->SetBinError(4, weight->GetBinError(1));

    weight->SetBinContent(1, 0.);
    weight->SetBinError(1, 0.);
    tree->Draw("0.5>>weight", cutMET);
    hCutFlow->SetBinContent(5, weight->GetBinContent(1));
    hCutFlow->SetBinError(5, weight->GetBinError(1));

    weight->SetBinContent(1, 0.);
    weight->SetBinError(1, 0.);
    tree->Draw("0.5>>weight", cutMjj);
    hCutFlow->SetBinContent(6, weight->GetBinContent(1));
    hCutFlow->SetBinError(6, weight->GetBinError(1));

    weight->SetBinContent(1, 0.);
    weight->SetBinError(1, 0.);
    tree->Draw("0.5>>weight", cutDPhi);
    hCutFlow->SetBinContent(7, weight->GetBinContent(1));
    hCutFlow->SetBinError(7, weight->GetBinError(1));

    hCutFlow->Scale( lumi * dataset.sigma / dataset.nEvents );

    // sum binned datasets
    if (dataset.name.compare(0,3,"QCD")==0) hQCD->Add(hCutFlow, 1.);
    if (dataset.name.compare(0,3,"Zvv")==0) hZNuNu->Add(hCutFlow, 1.);
    if (dataset.name.compare(0,1,"W")==0) hWLNu->Add(hCutFlow, 1.);
    if (dataset.name.compare(0,6,"SingleT")==0) hSingleT->Add(hCutFlow, 1.);
    if (dataset.name.compare(0,2,"WW")==0 ||
	dataset.name.compare(0,2,"WZ")==0 ||
	dataset.name.compare(0,2,"ZZ")==0 )
      hDiboson->Add(hCutFlow, 1.);

    ofile->cd();
    hCutFlow->Write("",TObject::kOverwrite);
    
    ifile->Close();

//     delete hCutFlow;
//     delete weight;
//     delete tree;
//     delete ifile;

  }

  hQCD->Write("",TObject::kOverwrite);  
  hZNuNu->Write("",TObject::kOverwrite);  
  hWLNu->Write("",TObject::kOverwrite);  
  hSingleT->Write("",TObject::kOverwrite);  
  hDiboson->Write("",TObject::kOverwrite);  

  // write TeX file
  ofstream effFile;
  effFile.open(options.oDir+std::string("/efficiency.txt"));

  effFile << "Cut & N($Z\\rightarrow\\nu\\nu) & N($W\\rightarrow l\\nu) & N(QCD) & N($t\\bar{t}$) & N(single $t$) & N(diboson) & N(signal $m_H=120$~\\GeV \\\\" << std::endl;

  std::vector<std::string> cutNames;
  cutNames.push_back("Trigger");
  cutNames.push_back("Lepton vetoes");
  cutNames.push_back("Dijet $p_T>50$~\\GeV");
  cutNames.push_back("$\\detajj > 4.2$");
  cutNames.push_back("$\\met > 130$~\\GeV");
  cutNames.push_back("$\\mjj > 1200$~\\GeV");
  cutNames.push_back("$\\dphijj < 1.0$");

  TH1D* hTTbar = (TH1D*) ofile->Get("hCutFlow_TTbar");
  TH1D* hSignal = (TH1D*) ofile->Get("hCutFlow_signalM120");

  // cutflow table
  for (unsigned i=1; i<8; ++i) {

    effFile << cutNames.at(i-1) << " & ";
    effFile << "$" << hZNuNu->GetBinContent(i) << " \\pm " << hZNuNu->GetBinError(i) << "$ & ";
    effFile << "$" << hWLNu->GetBinContent(i) << " \\pm " << hWLNu->GetBinError(i) << "$ & ";
    effFile << "$" << hQCD->GetBinContent(i) << " \\pm " << hQCD->GetBinError(i) << "$ & ";
    effFile << "$" << hTTbar->GetBinContent(i) << " \\pm " << hTTbar->GetBinError(i) << "$ & ";
    effFile << "$" << hSingleT->GetBinContent(i) << " \\pm " << hSingleT->GetBinError(i) << "$ & ";
    effFile << "$" << hDiboson->GetBinContent(i) << " \\pm " << hDiboson->GetBinError(i) << "$ & ";
    effFile << "$" << hSignal->GetBinContent(i) << " \\pm " << hSignal->GetBinError(i) << "$ \\\\ " << std::endl;

  }

  effFile << std::endl << std::endl;

  // signal efficiencies
  std::cout << "Dataset\t\teff(%)\t\tN\t\tpass/total(MC)" << std::endl;

  for (unsigned i=0; i<datasets.size(); ++i) {

    Dataset dataset = datasets.getDataset(i);

    std::string sigstr("signal");
    if (dataset.name.compare(0,6,sigstr) != 0) continue;

    TFile* ifile = datasets.getTFile(dataset.name);
    TTree* tree = (TTree*) ifile->Get("invHiggsInfo/InvHiggsInfo");
    
    TH1D* weight = new TH1D("weight","", 1, 0., 1.);
    tree->Draw("0.5>>weight", cutDPhi);

    double nMCPas = weight->GetBinContent(1);
    double nMCTot = double(dataset.nEvents);

    double eff    = double(nMCPas)/dataset.nEvents;
    double effErr = sqrt( nMCPas * (1-nMCPas/nMCTot) ) / nMCTot;  // binomial error

    double n      = lumi * dataset.sigma * eff;
    double nErr   = lumi * dataset.sigma * effErr;


    effFile << dataset.name << "\t" 
	    << 100.*eff << " +/- " << 100.*effErr << "\t" 
	    << nMCPas << " / " << nMCTot << "\t"
	    << n << " +/- " << nErr << "\t" 
	    << dataset.sigma << std::endl;
    
    ifile->Close();

  } 
  

  ofile->Close();
  effFile.close();

}
