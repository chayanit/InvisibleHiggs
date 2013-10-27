#include "InvisibleHiggs/Analysis/interface/ProgramOptions.h"
#include "InvisibleHiggs/Analysis/interface/Cuts.h"
#include "InvisibleHiggs/Analysis/interface/Histogrammer.h"
#include "InvisibleHiggs/Analysis/interface/StackPlot.h"
#include "InvisibleHiggs/Analysis/interface/SumDatasets.h"
#include "InvisibleHiggs/Analysis/interface/Datasets.h"

#include "TTree.h"
#include "TMath.h"
#include "TH1D.h"
#include "TH2D.h"

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {

  TH1::SetDefaultSumw2();

  ProgramOptions options(argc, argv);

  double lumi = options.lumi;
  std::string oDir = options.oDir+std::string("/ControlPlots");

  std::cout << "Integrated luminosity : " << lumi << " pb-1" << std::endl;

  boost::filesystem::path opath(oDir);
  if (!exists(opath)) {
    std::cout << "Creating output directory : " << oDir << std::endl;
    boost::filesystem::create_directory(opath);
  }
  else std::cout << "Writing results to " << oDir << std::endl;

  // datasets
  Datasets datasets(options.iDir);
  datasets.readFile(options.datasetFile);

  // cuts
  Cuts cuts;
  TCut ctrl("");
  ctrl += cuts.cut("trigger");     // trigger
  ctrl += cuts.cut("metFilter");  // met filters
  ctrl += cuts.cut("dijet");       // dijets
  ctrl += cuts.cut("sgnEtaJJ");   // fwd/bkwd
  ctrl += cuts.cut("EVeto");       // lepton vetoes
  ctrl += cuts.cut("MuVeto");

  // For lepton weights
  TCut elVetoWeight = cuts.elVetoWeight(options.leptCorr);
  TCut muVetoWeight = cuts.muVetoWeight(options.leptCorr);
  TCut leptonVetoWeight = elVetoWeight * muVetoWeight;

  TCut puWeight("puWeight"); // PU weights
  // TCut trigCorr("trigCorrWeight"); // Trigger data/mc corrections
  TCut trigCorr( "(trigCorrWeight>0) ? trigCorrWeight : 1." );

  // loop over datasets
  for (unsigned i=0; i<datasets.size(); ++i) {

    Dataset dataset = datasets.getDataset(i);

    std::cout << "Making histograms for " << dataset.name << " " << (dataset.isData?"data":"MC") << std::endl;

    TFile* ifile = datasets.getTFile(dataset.name);
    TTree* tree = (TTree*) ifile->Get("invHiggsInfo/InvHiggsInfo");

    TFile* ofile = TFile::Open( (oDir+std::string("/")+dataset.name+std::string(".root")).c_str(), "RECREATE");

    // create histograms
    // TH1D* hjet1pt      = new TH1D("hjet1pt",    "", 100, 0.,  200.);
    TH1D* hjet1pt         = new TH1D("hjet1pt",    "", 75, 0.,  2000.);
    TH1D* hjet1eta        = new TH1D("hjet1eta",   "", 50, -5.,  5.);
    TH1D* hjet1phi        = new TH1D("hjet1phi",   "", 72, -1*TMath::Pi(), TMath::Pi());
    TH2D* hjet1etaphi     = new TH2D("hjet1etaphi","", 100, -5., 5., 72, -1*TMath::Pi(), TMath::Pi());

    // TH1D* hjet2pt      = new TH1D("hjet2pt",    "", 100, 0.,  200.);
    TH1D* hjet2pt         = new TH1D("hjet2pt",    "", 75, 0.,  1000.);
    TH1D* hjet2eta        = new TH1D("hjet2eta",   "", 50, -5.,  5.);
    TH1D* hjet2phi        = new TH1D("hjet2phi",   "", 72, -1*TMath::Pi(), TMath::Pi());
    TH2D* hjet2etaphi     = new TH2D("hjet2etaphi","", 100, -5., 5., 72, -1*TMath::Pi(), TMath::Pi());

    TH1D* hjetdeta        = new TH1D("hjetdeta",   "", 50,  0.,  10.);
    // TH1D* hjetdphi     = new TH1D("hjetdphi",   "", 72,  0.,  TMath::Pi());
    TH1D* hjetdphi        = new TH1D("hjetdphi",   "", 50,  0.,  TMath::Pi());
    TH1D* hJetMetDPhiNorm = new TH1D("hJetMetDPhiNorm",   "", 50,  0.,  100.);
    TH1D* hJetMetDPhi     = new TH1D("hJetMetDPhi",   "", 50,  0.,  TMath::Pi());
    // TH1D* hmjj         = new TH1D("hmjj",       "", 100, 0.,  3000.);
    TH1D* hmjj            = new TH1D("hmjj",       "", 50, 0.,  4000.);
    // TH1D* hmet         = new TH1D("hmet",       "", 100, 0.,  200.);
    TH1D* hmet            = new TH1D("hmet",       "", 50, 0.,  800.);
    TH1D* hmetphi         = new TH1D("hmetphi",   "", 72, -1*TMath::Pi(), TMath::Pi());


    // set up cuts

    // Additional cuts specific to DYJetsToLL (cut on Zpt <100 to avoid double counting with the PtZ-100 sample)
    TCut cutD = cuts.cutDataset(dataset.name);

    // Additional WJets corrections (so can use inclusive & exclusive samples)
    TCut wWeight("");
    if (dataset.name == "WJets"  ||
        dataset.name == "W1Jets" || 
        dataset.name == "W2Jets" || 
        dataset.name == "W3Jets" || 
        dataset.name == "W4Jets") {
      std::cout << "     using wWeight" << std::endl;
      wWeight =  cuts.wWeight();
    } 

    TCut ctrl1 = puWeight * wWeight * trigCorr * leptonVetoWeight * (cutD + ctrl + cuts.cut("Mjj"));  // Mjj
    TCut ctrl2 = puWeight * wWeight * trigCorr * leptonVetoWeight * (cutD + ctrl + cuts.cut("MET"));  // MET

    // fill histograms
    tree->Draw("jet1Pt>>hjet1pt", ctrl2);
    tree->Draw("jet1Eta>>hjet1eta", ctrl2);
    tree->Draw("jet1Phi>>hjet1phi", ctrl2);
    tree->Draw("jet1Phi:jet1Eta>>hjet1etaphi", ctrl2);

    tree->Draw("jet2Pt>>hjet2pt", ctrl2);
    tree->Draw("jet2Eta>>hjet2eta", ctrl2);
    tree->Draw("jet2Phi>>hjet2phi", ctrl2);
    tree->Draw("jet2Phi:jet2Eta>>hjet2etaphi", ctrl2);

    tree->Draw("vbfDEta>>hjetdeta", ctrl2);
    tree->Draw("vbfDPhi>>hjetdphi", ctrl2);
    tree->Draw("jmDPhiNMin>>hJetMetDPhiNorm",ctrl2);
    tree->Draw("jmDPhi>>hJetMetDPhi",ctrl2);
    tree->Draw("vbfM>>hmjj", ctrl2);
    tree->Draw("met>>hmet", ctrl1);
    tree->Draw("metPhi>>hmetphi", ctrl1);
    // scale MC to lumi
    if (!dataset.isData) {
      double weight = lumi * dataset.sigma / dataset.nEvents;
      hjet1pt->Scale(weight);
      hjet1eta->Scale(weight);
      hjet1phi->Scale(weight);
      hjet1etaphi->Scale(weight);
      hjet2pt->Scale(weight);
      hjet2eta->Scale(weight);
      hjet2phi->Scale(weight);
      hjet2etaphi->Scale(weight);
      hjetdeta->Scale(weight);
      hjetdphi->Scale(weight);
      hJetMetDPhiNorm->Scale(weight);  
      hJetMetDPhi->Scale(weight);  
      hmjj->Scale(weight);
      hmet->Scale(weight);
      hmetphi->Scale(weight);
    }

    // write histograms
    hjet1pt->Write("",TObject::kOverwrite);
    hjet1eta->Write("",TObject::kOverwrite);
    hjet1phi->Write("",TObject::kOverwrite);
    hjet1etaphi->Write("",TObject::kOverwrite);
    hjet2pt->Write("",TObject::kOverwrite);
    hjet2eta->Write("",TObject::kOverwrite);
    hjet2phi->Write("",TObject::kOverwrite);
    hjet2etaphi->Write("",TObject::kOverwrite);
    hjetdeta->Write("",TObject::kOverwrite);
    hjetdphi->Write("",TObject::kOverwrite);
    hJetMetDPhiNorm->Write("",TObject::kOverwrite);
    hJetMetDPhi->Write("",TObject::kOverwrite);
    hmjj->Write("",TObject::kOverwrite);
    hmet->Write("",TObject::kOverwrite);
    hmetphi->Write("",TObject::kOverwrite);

    ofile->Close();    
    ifile->Close();

  }

  // list histograms for dataset summing
  std::vector<std::string> hists;
  hists.push_back("hjet1pt");
  hists.push_back("hjet1eta");
  hists.push_back("hjet1phi");
  hists.push_back("hjet2pt");
  hists.push_back("hjet2eta");
  hists.push_back("hjet2phi");
  hists.push_back("hjetdeta");
  hists.push_back("hjetdphi");
  hists.push_back("hJetMetDPhiNorm");
  hists.push_back("hJetMetDPhi");
  hists.push_back("hmjj");
  hists.push_back("hmet");
  hists.push_back("hmetphi");


  // sum QCD histograms
  std::vector<std::string> qcdDatasets;
  qcdDatasets.push_back(std::string("QCD_Pt30to50"));
  qcdDatasets.push_back(std::string("QCD_Pt50to80"));
  qcdDatasets.push_back(std::string("QCD_Pt80to120"));
  qcdDatasets.push_back(std::string("QCD_Pt120to170"));
  qcdDatasets.push_back(std::string("QCD_Pt170to300"));
  qcdDatasets.push_back(std::string("QCD_Pt300to470"));
  qcdDatasets.push_back(std::string("QCD_Pt470to600"));
  qcdDatasets.push_back(std::string("QCD_Pt600to800"));
  qcdDatasets.push_back(std::string("QCD_Pt800to1000"));
  qcdDatasets.push_back(std::string("QCD_Pt1000to1400"));
  qcdDatasets.push_back(std::string("QCD_Pt1400to1800"));
  qcdDatasets.push_back(std::string("QCD_Pt1800"));
  SumDatasets(oDir, qcdDatasets, hists, "QCD");

  // sum Z+jets datasets
  std::vector<std::string> zjetsDatasets;
  zjetsDatasets.push_back(std::string("Zvv_50to100"));
  zjetsDatasets.push_back(std::string("Zvv_100to200"));
  zjetsDatasets.push_back(std::string("Zvv_200to400"));
  zjetsDatasets.push_back(std::string("Zvv_400toinf"));
  SumDatasets(oDir, zjetsDatasets, hists, "ZJets");

  // sum W+jets datasets
  std::vector<std::string> wjetsDatasets;
  wjetsDatasets.push_back(std::string("WJets"));
  wjetsDatasets.push_back(std::string("W1Jets"));
  wjetsDatasets.push_back(std::string("W2Jets"));
  wjetsDatasets.push_back(std::string("W3Jets"));
  wjetsDatasets.push_back(std::string("W4Jets"));
  SumDatasets(oDir, wjetsDatasets, hists, "WNJets");

  // sum single top datasets
  std::vector<std::string> topDatasets;
  topDatasets.push_back(std::string("SingleT_t"));
  topDatasets.push_back(std::string("SingleTbar_t"));
  topDatasets.push_back(std::string("SingleT_s"));
  topDatasets.push_back(std::string("SingleTbar_s"));
  topDatasets.push_back(std::string("SingleT_tW"));
  topDatasets.push_back(std::string("SingleTbar_tW"));
  topDatasets.push_back(std::string("TTBar"));
  SumDatasets(oDir, topDatasets, hists, "SingleT+TTbar");

  // sum DY contributions
  std::cout << "Summing histograms for DYJetsToLL" << std::endl;
  std::vector<std::string> dyjets;
  dyjets.push_back("DYJetsToLL");
  dyjets.push_back("DYJetsToLL_PtZ-100");
  dyjets.push_back("DYJetsToLL_EWK");
  SumDatasets(oDir,dyjets,hists,"DYJets");

  // sum single top datasets
  std::vector<std::string> dibDatasets;
  dibDatasets.push_back(std::string("WW"));
  dibDatasets.push_back(std::string("WZ"));
  dibDatasets.push_back(std::string("ZZ"));
  SumDatasets(oDir, dibDatasets, hists, "DiBoson");

  // make plots
  std::cout << "Making plots" << std::endl;
  StackPlot plots(oDir);
  plots.setLegPos(0.69,0.77,0.98,0.97);

  // plots.setLabel("CMS Preliminary 2012 #int L = 19.56 fb^{-1}");

  // plots.addDataset("QCD",        kBlue, 0);
  // plots.addDataset("WNJets",      kGreen, 0);
  // plots.addDataset("ZJets",      kOrange, 0);
  // plots.addDataset("TTbar",      kMagenta, 0);
  // plots.addDataset("SingleT",  kMagenta-2, 0);
  // plots.addDataset("DiBoson",    kRed-2, 0);

  // plots.addDataset("signalM120", kRed, 2);
  // 
  plots.addDataset("DiBoson", kViolet-6, 0);
  plots.addDataset("DYJets", kPink-4,0);
  plots.addDataset("SingleT+TTbar", kAzure-2, 0);
  plots.addDataset("ZJets", kOrange-2, 0);
  plots.addDataset("WNJets", kBlue+1, 0);
  plots.addDataset("QCD", kGreen+3, 0);
  plots.addDataset("SignalM125_POWHEG", kRed, 2);
  plots.addDataset("METABCD",    kBlack, 1);

  // draw plots
  plots.dumpInfo(std::cout);

  plots.draw("hjet1pt", "Leading jet p_{T} [GeV]", "N_{events}");
  plots.draw("hjet1eta", "Leading jet #eta", "N_{events}");
  plots.draw("hjet1phi", "Leading jet #phi", "N_{events}");
  plots.draw("hjet2pt", "Sub-leading jet p_{T} [GeV]", "N_{events}");
  plots.draw("hjet2eta", "Sub-leading jet #eta", "N_{events}");
  plots.draw("hjet2phi", "Sub-leading jet #phi", "N_{events}");
  plots.draw("hjetdeta", "#Delta #eta_{jj}", "N_{events}");
  plots.draw("hmjj", "M_{jj} [GeV]", "N_{events}");
  plots.draw("hmet", "E_{T}^{miss} [GeV]", "N_{events}");
  plots.draw("hmetphi", "#phi (E_{T}^{miss}) [GeV]", "N_{events}");
  plots.setYMin(1e-1);
  plots.draw("hjetdphi", "#Delta #phi_{jj} [GeV]", "N_{events}");
  plots.draw("hJetMetDPhi", "#Delta #phi_{j-#slash{E}_{T}} [GeV]", "N_{events}");
  plots.draw("hJetMetDPhiNorm", "#Delta #phi_{N}^{min} [GeV]", "N_{events}");
}
