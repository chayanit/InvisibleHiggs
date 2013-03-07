
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
#include <fstream>

#include "RooProfileLL.h"
#include "RooAbsPdf.h"
#include "RooStats/HypoTestResult.h"
#include "RooRealVar.h"
#include "RooPlot.h"
#include "RooDataSet.h"
#include "RooTreeDataStore.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TStopwatch.h"

#include "RooStats/ProfileLikelihoodCalculator.h"
#include "RooStats/MCMCCalculator.h"
#include "RooStats/UniformProposal.h"
#include "RooStats/FeldmanCousins.h"
#include "RooStats/NumberCountingPdfFactory.h"
#include "RooStats/ConfInterval.h"
#include "RooStats/PointSetInterval.h"
#include "RooStats/LikelihoodInterval.h"
#include "RooStats/LikelihoodIntervalPlot.h"
#include "RooStats/RooStatsUtils.h"
#include "RooStats/ModelConfig.h"
#include "RooStats/MCMCInterval.h"
#include "RooStats/MCMCIntervalPlot.h"
#include "RooStats/ProposalFunction.h"
#include "RooStats/ProposalHelper.h"
#include "RooFitResult.h"
#include "TGraph2D.h"

// use this order for safety on library loading
using namespace RooFit ;
using namespace RooStats ;

namespace po = boost::program_options;

int main(int argc, char* argv[]) {

  // some variables
  double lumi = 697.;  //pb-1
  std::string iDir("/storage/phjjb/invisibleHiggs/InvHiggsInfo_v8");
  std::string oDir("analysis_v8");
  std::string datasetFile("InvisibleHiggs/Ntuple/data/datasets_v8.txt");

  // program options
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help,h", "Display this message")
    ("outdir,o", po::value<std::string>(), "Output directory")
    ("indir,i", po::value<std::string>(), "Input directory")
    ("datasets,f", po::value<std::string>(), "Datasets file")
    ("lumi,l", po::value<double>(), "Integrated luminosity");

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
  boost::filesystem::path opath(oDir);
  if (!exists(opath)) {
    std::cout << "Creating output directory : " << oDir << std::endl;
    boost::filesystem::create_directory(opath);
  }
  else std::cout << "Writing results to " << oDir << std::endl;

  std::cout << "Integrated luminosity : " << lumi << " pb-1" << std::endl;
  std::cout << std::endl;

  // read background estimates from file
  std::cout << "Backgrounds" << std::endl;
  double bgZjets(0.), err_bgZjets(0.);
  double bgWjets(0.), err_bgWjets(0.);
  double bgQCD(0.), err_bgQCD(0.);

  ifstream file;
  double n, err_n;
  std::string name;

  file.open((oDir+std::string("/zjets.txt").c_str()));
  while (file >> name >> n >> err_n) {
    if (name == "Signal") {
      bgZjets = n;
      err_bgZjets = err_n;
    }
  }
  file.close();

  std::cout << "Z+jets  : " << bgZjets << " +/- " << err_bgZjets << std::endl;

  file.open((oDir+std::string("/wjets.txt")).c_str());
  while (file >> name >> n >> err_n) {
    if (name == "Signal") {
      bgWjets = n;
      err_bgWjets = err_n;
    }
  }
  file.close();

  std::cout << "W+jets  : " << bgWjets << " +/- " << err_bgWjets << std::endl;

  file.open((oDir+std::string("/qcd.txt")).c_str());
  while (file >> name >> n >> err_n) {
    if (name == "Signal") {
      bgQCD = n;
      err_bgQCD = err_n;
    }
  }
  file.close();

  std::cout << "QCD     : " << bgQCD << " +/- " << err_bgQCD << std::endl;

  // total BG
  double bgTot     = bgZjets + bgWjets + bgQCD;
  double err_bgTot = sqrt( pow(err_bgZjets, 2) + pow(err_bgWjets, 2) + pow(err_bgQCD, 2) );

  std::cout << "Total   : " << bgTot << " +/- " << err_bgTot << std::endl;
  std::cout << std::endl;

  // observed
  std::cout << "Observed" << std::endl;
  int nObs = int(bgTot);

//   file.open((oDir+std::string("/obs.txt")).c_str());
//   while (file >> name >> n) {
//     if (name == "Signal") {
//       nObs = n;
//     }
//   }
//   file.close();

  std::cout << "Obs     : " << nObs << std::endl;
  std::cout << std::endl;

  // read signal efficiencies from file
  std::cout << "Signal efficiency" << std::endl;
  double mH[10];
  double effSignal[10];
  double err_effSignal[10];

  file.open((oDir+std::string("/signal.txt")).c_str());
  std::string line;
  getline(file, line);

  int i=0;
  double m, eff, err, a,b,c,d;
  while (file >> name >> m >> eff >> err >> a >> b >> c >> d) {
    mH[i]        = m;
    effSignal[i] = eff;
    err_effSignal[i] = err;
    std::cout << "Signal " << m << " : " << eff << " +/- " << err << std::endl;
  }
  file.close();

  std::cout << std::endl;

  /// calculate expected limit
  std::cout << "Expected limit" << std::endl;
  using namespace RooFit ;
  using namespace RooStats ;

  // model
  std::cout << "Setting up model" << std::endl;
  RooWorkspace* w = new RooWorkspace();

  // total yield
  w->factory("n[0]");

  // BG with nuisance
  w->factory("bg_nom[1.0]");
  w->var("bg_nom")->setVal(bgTot);
  //  w->var("bg_nom")->setConstant(true);
  w->factory("bg_kappa[1.0]");
  w->var("bg_kappa")->setVal(1.0+err_bgTot/bgTot);
  //  w->var("bg_kappa")->setConstant(true);
  w->factory("expr::alpha_bg('pow(bg_kappa,beta_bg)',bg_kappa,beta_bg[0,-5,5])");
  w->factory("prod::nbg(bg_nom,alpha_bg)");
  w->factory("Gaussian::constr_bg(beta_bg,globa_bg[0,-5,5],1)");
  w->factory("glob_bg");
  //  w->var("glob_bg")->setConstant(true);

  // BG + signal
   w->factory("nsig[0]");
//   w->factory("sum::yield(nsig,nbg)");
//   w->factory("Gaussian::model_core(n,yield,expr('sqrt(yield)',yield))");

//   // nuisances
//   w->factory("PROD::model(model_core,constr_bg)");

//   // uniform prior for signal
//   w->factory("Uniform::prior(nsig)");

  std::cout << "Setting limits" << std::endl;

  // set ranges
  w->var("n")->setRange(0,2000);
  //  w->var("nsig")->setRange(0,2000);

//   RooArgSet obs(*(w->var("n")), "obs");

//   RooArgSet globalObs("global_obs");
//   globalObs.add(*(w->var("global_bg")));
  
//   RooArgSet poi(*(w->var("nsig")), "poi");

//   RooArgSet nuis("nuis");
//   nuis.add(*(w->var("beta_bg")));

  w->var("n")->setVal(nObs);
  
  std::cout << "Done" << std::endl;

}
