
#include "InvisibleHiggs/Analysis/interface/Cuts.h"
#include "InvisibleHiggs/Analysis/interface/Histogrammer.h"
#include "InvisibleHiggs/Analysis/interface/StackPlot.h"
#include "InvisibleHiggs/Analysis/interface/SumDatasets.h"
#include "InvisibleHiggs/Analysis/interface/Datasets.h"

#include <iostream>

int main(int argc, char* argv[]) {

  double lumi = 697.;  //pb-1
  std::string mcDir("/storage/phjjb/invisibleHiggs/InvHiggsInfo_Summer12_v6b");
  std::string dataDir("/storage/phjjb/invisibleHiggs/InvHiggsInfo_Data2012_v6b");
  std::string odir("Summer12_v6b");

  Datasets datasets;
  datasets.readFile("InvisibleHiggs/Ntuple/data/datasets_v6.txt");

  // cuts
  Cuts cuts;
  cuts.addCut("trigger",    "hltResult1>0.");
  cuts.addCut("dijet",      "jet1Pt>40.&&jet2Pt>40.");
  cuts.addCut("jetFB",      "(jet1Eta*jet2Eta)<0.");
  cuts.addCut("dEta",       "abs(jet1Eta-jet2Eta)>4.2");
  cuts.addCut("dijetMass",  "vbfM>1200.");
  cuts.addCut("MET",        "met>110.");
  cuts.addCut("jetMETdPhi", "min(abs(abs(abs(metPhi-jet1Phi)-TMath::Pi())-TMath::Pi()), abs(abs(abs(metPhi-jet2Phi)-TMath::Pi())-TMath::Pi()))>0.5");
  cuts.addCut("EVeto",      "ele1Pt<10.");
  cuts.addCut("MuVeto",     "mu1Pt<10.");


  // MC histograms
  Histogrammer histogrammer(odir, mcDir, cuts);

  for (unsigned i=0; i<datasets.size(); ++i) {
    Dataset d = datasets.getDataset(i);
    histogrammer.addDataset(d.name, lumi*d.sigma/d.nEvents);
  }

  histogrammer.addDataset("MET_Run2012A", 1., dataDir);

  histogrammer.loop();

  // sum QCD histograms
  std::vector<std::string> qcdDatasets;
  qcdDatasets.push_back(std::string("QCD_Pt-30to50"));
  qcdDatasets.push_back(std::string("QCD_Pt-50to80"));
  qcdDatasets.push_back(std::string("QCD_Pt-80to120"));
  qcdDatasets.push_back(std::string("QCD_Pt-120to170"));
  qcdDatasets.push_back(std::string("QCD_Pt-170to300"));
  qcdDatasets.push_back(std::string("QCD_Pt-300to470"));
  qcdDatasets.push_back(std::string("QCD_Pt-470to600"));
  qcdDatasets.push_back(std::string("QCD_Pt-600to800"));
  qcdDatasets.push_back(std::string("QCD_Pt-800to1000"));
  qcdDatasets.push_back(std::string("QCD_Pt-1000to1400"));
  qcdDatasets.push_back(std::string("QCD_Pt-1400to1800"));
  qcdDatasets.push_back(std::string("QCD_Pt-1800"));

  std::vector<std::string> hists;
  hists.push_back("hjet1pt");
  hists.push_back("hjet2pt");
  hists.push_back("hjetdeta");
  hists.push_back("hvbfm");
  hists.push_back("hmet");
  hists.push_back("hjetdphi");

  SumDatasets(odir, qcdDatasets, hists, "QCD");


  // make plots
  StackPlot plots(odir);
  plots.addDataset("QCD_Pt-30to50",     kBlue-9, 0);
  plots.addDataset("QCD_Pt-50to80",     kBlue-8, 0);
  plots.addDataset("QCD_Pt-80to120",    kBlue-7, 0);
  plots.addDataset("QCD_Pt-120to170",   kBlue-6, 0);
  plots.addDataset("QCD_Pt-170to300",   kBlue-5, 0);
  plots.addDataset("QCD_Pt-300to470",   kBlue-4, 0);
  plots.addDataset("QCD_Pt-470to600",   kBlue-3, 0);
  plots.addDataset("QCD_Pt-600to800",   kBlue-2, 0);
  plots.addDataset("QCD_Pt-800to1000",  kBlue-1, 0);
  plots.addDataset("QCD_Pt-1000to1400", kBlue,   0);
  plots.addDataset("QCD_Pt-1400to1800", kBlue+1, 0);
  plots.addDataset("QCD_Pt-1800",       kBlue+2, 0);
  plots.addDataset("WJetsToLNu",        kOrange, 0);
  plots.addDataset("ZJetsToNuNu_50_HT_100",  kGreen-4, 0);
  plots.addDataset("ZJetsToNuNu_100_HT_200", kGreen-3, 0);
  plots.addDataset("ZJetsToNuNu_200_HT_400", kGreen-2, 0);
  plots.addDataset("ZJetsToNuNu_400_HT_inf", kGreen-1, 0);

  plots.addDataset("VBF_HToZZTo4Nu_M120", kRed, 2);

  plots.addDataset("MET_Run2012A", kBlack, 1);


  //histogrammer.addDataset("VBF_HToZZTo4Nu_M120", lumi*1.632/99675, baseDir, version);
  //histogrammer.addDataset("VBF_HToZZTo4Nu_M150", lumi*1.251/99408, baseDir, version);
  //histogrammer.addDataset("VBF_HToZZTo4Nu_M200", lumi*0.8441/100118, baseDir, version);
  //histogrammer.addDataset("VBF_HToZZTo4Nu_M300", lumi*0.4408/99682, baseDir, version);
  //histogrammer.addDataset("VBF_HToZZTo4Nu_M400", lumi*0.2543/100083, baseDir, version);

  plots.dumpInfo(std::cout);

  plots.draw("hjet1pt", "E_{T} [GeV]", "N_{events}");

  plots.draw("hjet2pt", "E_{T} [GeV]", "N_{events}");

  plots.setYMin(1e-5);
  plots.setYMax(1e1);
  plots.draw("hjetdeta", "#Delta #eta_{jj}", "N_{events}");

  plots.setYMin(1e-5);
  plots.setYMax(1e1);
  plots.draw("hvbfm", "M_{jj} [GeV]", "N_{events}");

  plots.setYMin(1e-5);
  plots.setYMax(1e1);
  plots.draw("hmet", "E_{T}^{miss} [GeV]", "N_{events}");

  plots.draw("hjetdphi", "#Delta #phi_{jj} [GeV]", "N_{events}");

  plots.draw("hjmdphi", "#Delta #phi_{jetMET} [GeV]", "N_{events}");

  plots.draw("hseljet1pt", "E_{T} [GeV]", "N_{events}");

  plots.draw("hseljet2pt", "E_{T} [GeV]", "N_{events}");

  plots.setYMin(1e-5);
  plots.setYMax(1e1);
  plots.draw("hseljetdeta", "#Delta #eta_{jj}", "N_{events}");

  plots.setYMin(1e-5);
  plots.setYMax(1e1);
  plots.draw("hselvbfm", "M_{jj} [GeV]", "N_{events}");

  plots.setYMin(1e-5);
  plots.setYMax(1e1);
  plots.draw("hselmet", "E_{T}^{miss} [GeV]", "N_{events}");

  plots.draw("hseljetdphi", "#Delta #phi_{jj} [GeV]", "N_{events}");

  plots.draw("hseljmdphi", "#Delta #phi_{jetMET} [GeV]", "N_{events}");


}
