#include "InvisibleHiggs/Analysis/interface/Cuts.h"
#include "InvisibleHiggs/Analysis/interface/Histogrammer.h"
#include "InvisibleHiggs/Analysis/interface/StackPlot.h"
#include "InvisibleHiggs/Analysis/interface/SumDatasets.h"
#include "InvisibleHiggs/Analysis/interface/Datasets.h"

#include "TTree.h"
#include "TMath.h"
#include "TCanvas.h"

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include <iostream>

namespace po = boost::program_options;

int main(int argc, char* argv[]) {

  // some variables
  double lumi = 5000.;  //pb-1
  std::string iDir("");
  std::string oDir("");
  std::string datasetFile("");

  // program options
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help,h",     "Display this message")
    ("outdir,o",   po::value<std::string>(), "Output directory")
    ("indir,i",    po::value<std::string>(), "Input directory")
    ("datasets,f", po::value<std::string>(), "Datasets file")
    ("lumi,l",     po::value<double>(),      "Integrated luminosity");

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(desc).allow_unregistered().run(), vm);  
  po::notify(vm);

  // help
  if (vm.count("help")) {
    std::cout << desc << std::endl;
    std::exit(1);
  }
  
  if (vm.count("outdir"))   oDir = vm["outdir"].as<std::string>();
  if (vm.count("indir"))    iDir=vm["indir"].as<std::string>();
  if (vm.count("datasets")) datasetFile=vm["datasets"].as<std::string>();
  if (vm.count("lumi"))     lumi=vm["lumi"].as<double>();

  // create output directory if it doesn't exist already
  oDir += std::string("/Signal");
  boost::filesystem::path opath(oDir);
  if (!exists(opath)) {
    std::cout << "Creating output directory : " << oDir << std::endl;
    boost::filesystem::create_directory(opath);
  }
  else std::cout << "Writing results to " << oDir << std::endl;

  std::cout << "Integrated luminosity : " << lumi << " pb-1" << std::endl;

  // datasets
  Datasets datasets;
  datasets.readFile(datasetFile);

  // cuts
  Cuts cuts;
  TCut puWeight("puWeight");
  TCut weightedAllCuts = puWeight * cuts.allCuts();

  // plots
  int nSignalPoints=0;
  std::vector<std::string> signalNames;

  for (unsigned i=0; i<datasets.size(); ++i) {

    Dataset dataset = datasets.getDataset(i);

    std::string sigstr("signal");
    if (dataset.name.compare(0,6,sigstr) != 0) continue;

    nSignalPoints++;
    signalNames.push_back(dataset.name);

    std::cout << "Making signal histograms for " << dataset.name << std::endl;

    TFile* ifile = TFile::Open( (iDir+std::string("/")+dataset.name+std::string(".root")).c_str(), "READ");

    if (ifile==0) {
      std::cerr << "No file for " << dataset.name << std::endl;
      continue;
    }

    TFile* ofile = TFile::Open( (oDir+std::string("/")+dataset.name+std::string("_Signal.root")).c_str(), "UPDATE");
    
    TTree* tree = (TTree*) ifile->Get("invHiggsInfo/InvHiggsInfo");

    // parton level
    TH1D* hQ1Pt   = new TH1D("hQ1Pt",   "", 50, 0., 200.);
    TH1D* hQ2Pt   = new TH1D("hQ2Pt",   "", 50, 0., 200.);
    TH1D* hQ1Eta  = new TH1D("hQ1Eta",  "", 50, -5., 5.);
    TH1D* hQ2Eta  = new TH1D("hQ2Eta",  "", 50, -5., 5.);

    TH1D* hHPt    = new TH1D("hHPt",    "", 50, 0., 500.);

    TH1D* hQQM    = new TH1D("hQQM",    "", 50, 0., 3000.);
    TH1D* hQQPt   = new TH1D("hQQPt",   "", 50, 0., 500.);
    TH1D* hQQDEta = new TH1D("hQQDEta", "", 50, 0., 10.);
    TH1D* hQQDPhi = new TH1D("hQQDPhi", "", 50, 0., TMath::Pi());

    tree->Draw("mcQ1Pt>>hQ1Pt");
    tree->Draw("mcQ2Pt>>hQ2Pt");
    tree->Draw("mcQ1Eta>>hQ1Eta");
    tree->Draw("mcQ2Eta>>hQ2Eta");
    tree->Draw("mcHiggsPt>>hHPt");
    tree->Draw("mcVBFM>>hQQM");
    tree->Draw("mcVBFEt>>hQQPt");
    tree->Draw("mcVBFDEta>>hQQDEta");
    tree->Draw("mcVBFDPhi>>hQQDPhi");
    
    hQ1Pt->Write("",TObject::kOverwrite);
    hQ2Pt->Write("",TObject::kOverwrite);
    hQ1Eta->Write("",TObject::kOverwrite);
    hQ2Eta->Write("",TObject::kOverwrite);
    hHPt->Write("",TObject::kOverwrite);
    hQQM->Write("",TObject::kOverwrite);
    hQQPt->Write("",TObject::kOverwrite);
    hQQDEta->Write("",TObject::kOverwrite);
    hQQDPhi->Write("",TObject::kOverwrite);

    // reco level
    TH1D* hJet1Pt  = new TH1D("hJet1Pt", "", 50, 0., 200.);
    TH1D* hJet2Pt  = new TH1D("hJet2Pt", "", 50, 0., 200.);
    TH1D* hJet1Eta = new TH1D("hJet1Eta", "", 50, -5., 5.);
    TH1D* hJet2Eta = new TH1D("hJet2Eta", "", 50, -5., 5.);

    TH1D* hMET     = new TH1D("hMET",    "", 50, 0., 500.);

    TH1D* hJJM     = new TH1D("hJJM",    "", 50, 0., 3000.);
    TH1D* hJJPt    = new TH1D("hJJPt",   "", 50, 0., 500.);
    TH1D* hJJDEta  = new TH1D("hJJDEta", "", 50, 0., 10.);
    TH1D* hJJDPhi  = new TH1D("hJJDPhi", "", 50, 0., TMath::Pi());

    tree->Draw("jet1Pt>>hJet1Pt");
    tree->Draw("jet2Pt>>hJet2Pt");
    tree->Draw("jet1Eta>>hJet1Eta");
    tree->Draw("jet2Eta>>hJet2Eta");
    tree->Draw("met>>hMET");
    tree->Draw("vbfM>>hJJM");
    tree->Draw("vbfEt>>hJJPt");
    tree->Draw("vbfDEta>>hJJDEta");
    tree->Draw("vbfDPhi>>hJJDPhi");

    hJet1Pt->Write("",TObject::kOverwrite);
    hJet2Pt->Write("",TObject::kOverwrite);
    hJet1Eta->Write("",TObject::kOverwrite);
    hJet2Eta->Write("",TObject::kOverwrite);
    hMET->Write("",TObject::kOverwrite);
    hJJM->Write("",TObject::kOverwrite);
    hJJPt->Write("",TObject::kOverwrite);
    hJJDEta->Write("",TObject::kOverwrite);
    hJJDPhi->Write("",TObject::kOverwrite);

    // parton level after all cuts
    TH1D* hQ1PtAllCuts   = new TH1D("hQ1PtAllCuts",   "", 50, 0., 200.);
    TH1D* hQ2PtAllCuts   = new TH1D("hQ2PtAllCuts",   "", 50, 0., 200.);
    TH1D* hQ1EtaAllCuts  = new TH1D("hQ1EtaAllCuts",  "", 50, -5., 5.);
    TH1D* hQ2EtaAllCuts  = new TH1D("hQ2EtaAllCuts",  "", 50, -5., 5.);

    TH1D* hHPtAllCuts    = new TH1D("hHPtAllCuts",    "", 50, 0., 500.);

    TH1D* hQQMAllCuts    = new TH1D("hQQMAllCuts",    "", 50, 0., 3000.);
    TH1D* hQQPtAllCuts   = new TH1D("hQQPtAllCuts",   "", 50, 0., 500.);
    TH1D* hQQDEtaAllCuts = new TH1D("hQQDEtaAllCuts", "", 50, 0., 10.);
    TH1D* hQQDPhiAllCuts = new TH1D("hQQDPhiAllCuts", "", 50, 0., TMath::Pi());

    tree->Draw("mcQ1Pt>>hQ1PtAllCuts", weightedAllCuts);
    tree->Draw("mcQ2Pt>>hQ2PtAllCuts", weightedAllCuts);
    tree->Draw("mcQ1Eta>>hQ1EtaAllCuts", weightedAllCuts);
    tree->Draw("mcQ2Eta>>hQ2EtaAllCuts", weightedAllCuts);
    tree->Draw("mcHiggsPt>>hHPtAllCuts", weightedAllCuts);
    tree->Draw("mcVBFM>>hQQMAllCuts", weightedAllCuts);
    tree->Draw("mcVBFEt>>hQQPtAllCuts", weightedAllCuts);
    tree->Draw("mcVBFDEta>>hQQDEtaAllCuts", weightedAllCuts);
    tree->Draw("mcVBFDPhi>>hQQDPhiAllCuts", weightedAllCuts);

    hQ1PtAllCuts->Write("",TObject::kOverwrite);
    hQ2PtAllCuts->Write("",TObject::kOverwrite);
    hQ1EtaAllCuts->Write("",TObject::kOverwrite);
    hQ2EtaAllCuts->Write("",TObject::kOverwrite);
    hHPtAllCuts->Write("",TObject::kOverwrite);
    hQQMAllCuts->Write("",TObject::kOverwrite);
    hQQPtAllCuts->Write("",TObject::kOverwrite);
    hQQDEtaAllCuts->Write("",TObject::kOverwrite);
    hQQDPhiAllCuts->Write("",TObject::kOverwrite);


    // reco level after all cuts
    TH1D* hJet1PtAllCuts = new TH1D("hJet1PtAllCuts", "", 50, 0., 200.);
    TH1D* hJet2PtAllCuts = new TH1D("hJet2PtAllCuts", "", 50, 0., 200.);
    TH1D* hJet1EtaAllCuts = new TH1D("hJet1EtaAllCuts", "", 50, -5., 5.);
    TH1D* hJet2EtaAllCuts = new TH1D("hJet2EtaAllCuts", "", 50, -5., 5.);

    TH1D* hMETAllCuts    = new TH1D("hMETAllCuts",    "", 50, 0., 500.);

    TH1D* hJJMAllCuts    = new TH1D("hJJMAllCuts",    "", 50, 0., 3000.);
    TH1D* hJJPtAllCuts   = new TH1D("hJJPtAllCuts",   "", 50, 0., 500.);
    TH1D* hJJDEtaAllCuts = new TH1D("hJJDEtaAllCuts", "", 50, 0., 10.);
    TH1D* hJJDPhiAllCuts = new TH1D("hJJDPhiAllCuts", "", 50, 0., TMath::Pi());
    
    tree->Draw("jet1Pt>>hJet1PtAllCuts", weightedAllCuts);
    tree->Draw("jet2Pt>>hQJetPtAllCuts", weightedAllCuts);
    tree->Draw("jet1Eta>>hJet1EtaAllCuts", weightedAllCuts);
    tree->Draw("jet2Eta>>hJet2EtaAllCuts", weightedAllCuts);
    tree->Draw("met>>hMETAllCuts", weightedAllCuts);
    tree->Draw("vbfM>>hJJMAllCuts", weightedAllCuts);
    tree->Draw("vbfEt>>hJJPtAllCuts", weightedAllCuts);
    tree->Draw("vbfDEta>>hJJDEtaAllCuts", weightedAllCuts);
    tree->Draw("vbfDPhi>>hJJDPhiAllCuts", weightedAllCuts);

    hJet1PtAllCuts->Write("",TObject::kOverwrite);
    hJet2PtAllCuts->Write("",TObject::kOverwrite);
    hJet1EtaAllCuts->Write("",TObject::kOverwrite);
    hJet2EtaAllCuts->Write("",TObject::kOverwrite);
    hMETAllCuts->Write("",TObject::kOverwrite);
    hJJMAllCuts->Write("",TObject::kOverwrite);
    hJJPtAllCuts->Write("",TObject::kOverwrite);
    hJJDEtaAllCuts->Write("",TObject::kOverwrite);
    hJJDPhiAllCuts->Write("",TObject::kOverwrite);

    
    // make plots
    TCanvas canvas;
    canvas.SetLogy();

    // quark pt
    hQ1Pt->SetMinimum(1.);
    hQ1Pt->SetLineColor(kBlack);
    hQ1Pt->SetLineStyle(1);
    hQ1Pt->Draw();
    hQ2Pt->SetLineColor(kRed);
    hQ2Pt->SetLineStyle(1);
    hQ2Pt->Draw("SAME");
    hQ1PtAllCuts->SetLineColor(kBlack);
    hQ1PtAllCuts->SetLineStyle(2);
    hQ1PtAllCuts->Draw("SAME");
    hQ2PtAllCuts->SetLineColor(kRed);
    hQ2PtAllCuts->SetLineStyle(2);
    hQ2PtAllCuts->Draw("SAME");

    canvas.Print( (oDir+std::string("/")+dataset.name+std::string("_QuarkPt.pdf")).c_str() );

    // quark eta
    hQ1Eta->SetMinimum(1.);
    hQ1Eta->SetLineColor(kBlack);
    hQ1Eta->SetLineStyle(1);
    hQ1Eta->Draw();
    hQ2Eta->SetLineColor(kRed);
    hQ2Eta->SetLineStyle(1);
    hQ2Eta->Draw("SAME");
    hQ1EtaAllCuts->SetLineColor(kBlack);
    hQ1EtaAllCuts->SetLineStyle(2);
    hQ1EtaAllCuts->Draw("SAME");
    hQ2EtaAllCuts->SetLineColor(kRed);
    hQ2EtaAllCuts->SetLineStyle(2);
    hQ2EtaAllCuts->Draw("SAME");

    canvas.Print( (oDir+std::string("/")+dataset.name+std::string("_QuarkEta.pdf")).c_str() );

    // quark-quark mass
    hQQM->SetMinimum(1.);
    hQQM->SetLineColor(kBlack);
    hQQM->SetLineStyle(1);
    hQQM->Draw();
    hQQMAllCuts->SetLineColor(kBlack);
    hQQMAllCuts->SetLineStyle(2);
    hQQMAllCuts->Draw("SAME");

    canvas.Print( (oDir+std::string("/")+dataset.name+std::string("_QQMass.pdf")).c_str() );

    // quark-quark delta-eta
    hQQDEta->SetMinimum(1.);
    hQQDEta->SetLineColor(kBlack);
    hQQDEta->SetLineStyle(1);
    hQQDEta->Draw();
    hQQDEtaAllCuts->SetLineColor(kBlack);
    hQQDEtaAllCuts->SetLineStyle(2);
    hQQDEtaAllCuts->Draw("SAME");

    canvas.Print( (oDir+std::string("/")+dataset.name+std::string("_QQDEta.pdf")).c_str() );

    // quark-quark delta-phi
    hQQDPhi->SetMinimum(1.);
    hQQDPhi->SetLineColor(kBlack);
    hQQDPhi->SetLineStyle(1);
    hQQDPhi->Draw();
    hQQDPhiAllCuts->SetLineColor(kBlack);
    hQQDPhiAllCuts->SetLineStyle(2);
    hQQDPhiAllCuts->Draw("SAME");

    canvas.Print( (oDir+std::string("/")+dataset.name+std::string("_QQDPhi.pdf")).c_str() );


    // jet pt
    hJet1Pt->SetMinimum(1.);
    hJet1Pt->SetLineColor(kBlack);
    hJet1Pt->SetLineStyle(1);
    hJet1Pt->Draw();
    hJet2Pt->SetLineColor(kRed);
    hJet2Pt->SetLineStyle(1);
    hJet2Pt->Draw("SAME");
    hJet1PtAllCuts->SetLineColor(kBlack);
    hJet1PtAllCuts->SetLineStyle(2);
    hJet1PtAllCuts->Draw("SAME");
    hJet2PtAllCuts->SetLineColor(kRed);
    hJet2PtAllCuts->SetLineStyle(2);
    hJet2PtAllCuts->Draw("SAME");

    canvas.Print( (oDir+std::string("/")+dataset.name+std::string("_JetPt.pdf")).c_str() );

    // jet eta
    hJet1Eta->SetMinimum(1.);
    hJet1Eta->SetLineColor(kBlack);
    hJet1Eta->SetLineStyle(1);
    hJet1Eta->Draw();
    hJet2Eta->SetLineColor(kRed);
    hJet2Eta->SetLineStyle(1);
    hJet2Eta->Draw("SAME");
    hJet1EtaAllCuts->SetLineColor(kBlack);
    hJet1EtaAllCuts->SetLineStyle(2);
    hJet1EtaAllCuts->Draw("SAME");
    hJet2EtaAllCuts->SetLineColor(kRed);
    hJet2EtaAllCuts->SetLineStyle(2);
    hJet2EtaAllCuts->Draw("SAME");
    
    canvas.Print( (oDir+std::string("/")+dataset.name+std::string("_JetEta.pdf")).c_str() );

    // di-jet mass
    hJJM->SetMinimum(1.);
    hJJM->SetLineColor(kBlack);
    hJJM->SetLineStyle(1);
    hJJM->Draw();
    hJJMAllCuts->SetLineColor(kBlack);
    hJJMAllCuts->SetLineStyle(2);
    hJJMAllCuts->Draw("SAME");

    canvas.Print( (oDir+std::string("/")+dataset.name+std::string("_DijetMass.pdf")).c_str() );

    // quark-quark delta-eta
    hJJDEta->SetMinimum(1.);
    hJJDEta->SetLineColor(kBlack);
    hJJDEta->SetLineStyle(1);
    hJJDEta->Draw();
    hJJDEtaAllCuts->SetLineColor(kBlack);
    hJJDEtaAllCuts->SetLineStyle(2);
    hJJDEtaAllCuts->Draw("SAME");

    canvas.Print( (oDir+std::string("/")+dataset.name+std::string("_DijetDEta.pdf")).c_str() );

    // quark-quark delta-phi
    hJJDPhi->SetMinimum(1.);
    hJJDPhi->SetLineColor(kBlack);
    hJJDPhi->SetLineStyle(1);
    hJJDPhi->Draw();
    hJJDPhiAllCuts->SetLineColor(kBlack);
    hJJDPhiAllCuts->SetLineStyle(2);
    hJJDPhiAllCuts->Draw("SAME");

    canvas.Print( (oDir+std::string("/")+dataset.name+std::string("_DijetDPhi.pdf")).c_str() );

 
//     // draw the other stuff
//     canvas.RedrawAxis();
    
//     // draw labels
//     TPaveText cms(0.1, 0.9, 0.4, 0.95, "NDC");
//     cms.SetFillColor(0);
//     cms.SetBorderSize(1);
//     cms.SetLineColor(0);
//     cms.AddText("CMS Preliminary 2012 - 697 pb^{-1}");
//     cms.Draw();
    
//     // draw legend
//     leg.Draw();
    
//     std::string filename = dir_+std::string("/")+hname+std::string(".pdf");
//     std::cout << "Writing file " << filename << std::endl;
    
     ofile->Close();

     ifile->Close();

  }


  // cuts
  std::cout << "Dataset\t\teff(%)\t\tN\t\tpass/total(MC)" << std::endl;

  for (int i=0; i<nSignalPoints; ++i) {

    Dataset dataset = datasets.getDataset(signalNames.at(i));

    TFile* ifile = TFile::Open( (iDir+std::string("/")+dataset.name+std::string(".root")).c_str(), "READ");

    if (ifile==0) {
      std::cerr << "No file for " << dataset.name << std::endl;
      continue;
    }
    
    TTree* tree = (TTree*) ifile->Get("invHiggsInfo/InvHiggsInfo");
    
    TH1D* weight = new TH1D("weight","", 1, 0., 1.);
    tree->Draw("0.5>>weight", weightedAllCuts);

    double nMCPas = weight->GetBinContent(1);
    double nMCTot = double(dataset.nEvents);

    double eff    = double(nMCPas)/dataset.nEvents;
    double effErr = sqrt( nMCPas * (1-nMCPas/nMCTot) ) / nMCTot;  // binomial error

    double n      = lumi * dataset.sigma * eff;
    double nErr   = lumi * dataset.sigma * effErr;


    std::cout << dataset.name << "\t" 
	      << 100.*eff << " +/- " << 100.*effErr << "\t" 
	      << nMCPas << " / " << nMCTot << "\t"
	      << n << " +/- " << nErr << "\t" 
	      << dataset.sigma << std::endl;

    ifile->Close();

  }



}
