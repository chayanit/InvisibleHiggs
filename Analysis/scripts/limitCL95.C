//////////////////////////////////////////////////////////////////
//
// counting_limit.C
//
// This is an example how to use roostats_cl95.C
// to compute a limit on the signal cross section
// for a counting experiment
//
// How to change various options from default values
//
// Gena Kukartsev, May 2012
//
// Usage:
//        - start root
//        - compile and run this routine
//
//          [1] .L counting_limit.C+
//          [2] limit("bayesian")
//          [3] limit("mcmc")
//          [4] limit("cls")
//

#include <iostream>

#include "StatisticalTools/RooStatsRoutines/root/roostats_cl95.C"



void getLimitCL95(std::string oDir, 
		  double ilum = 195600., 
		  double slum = 558.) {
  //
  // compute limit in a counting experiment
  //

  // read backgrounds etc from target dir
  std::cout << "Backgrounds" << std::endl;
  double bgZjets(0.), err_bgZjets(0.);
  double bgWjets(0.), err_bgWjets(0.);
  double bgQCD(0.), err_bgQCD(0.);

  ifstream file;
  double n, err_n;
  std::string name;

  file.open( (oDir+std::string("/zjets.txt")).c_str() );
  while (file >> name >> n >> err_n) {
    if (name == "Signal") {
      bgZjets = n;
      err_bgZjets = err_n;
    }
  }
  file.close();

  std::cout << "Z+jets  : " << bgZjets << " +/- " << err_bgZjets << std::endl;

  file.open( (oDir+std::string("/wjets.txt")).c_str() );
  while (file >> name >> n >> err_n) {
    if (name == "Signal") {
      bgWjets = n;
      err_bgWjets = err_n;
    }
  }
  file.close();

  std::cout << "W+jets  : " << bgWjets << " +/- " << err_bgWjets << std::endl;

  file.open( (oDir+std::string("/qcd.txt")).c_str() );
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
  std::vector<double> mH, xsH, effSignal, err_effSignal;

  file.open( (oDir+std::string("/signal.txt")).c_str() );
  std::string line;
  getline(file, line);

  int i=0;
  double m, eff, err, a,b,c,d,x;
  while (file >> name >> m >> eff >> err >> a >> b >> c >> d >> x) {
    mH.push_back( m );
    xsH.push_back( x );
    effSignal.push_back( eff );
    err_effSignal.push_back( err );
    std::cout << "Signal " << m << " xs=" << x << " : " << eff << " +/- " << err << std::endl;
  }
  file.close();

  std::cout << std::endl;

  std::cout << "Going to compute limit for mH=" << mH[0] << ".  VBF x-section=" << xsH[0] << std::endl;
  std::cout << std::endl;
  

  // optional: set some parameters
  SetParameter("Optimize", false);
  SetParameter("CorrelatedLumiSyst", false);
  SetParameter("MakePlot", true);
  SetParameter("GaussianStatistics", false);

  SetParameter("NClsSteps", 10);
  SetParameter("NToys", 1000);
  SetParameter("CalculatorType", 0); // 0 for frequentist
  SetParameter("TestStatType", 3); // LHC-style 1-sided profile likelihood
  SetParameter("Verbosity", 3);
  SetParameter("RandomSeed", 0);

  SetParameter("ConfidenceLevel", 0.95);
  SetParameter("NToysRatio", 2.0);

  
  // will work with any method, example shown for Bayesian
  LimitResult expected = GetExpectedLimit(ilum, slum,
					  effSignal[0], err_effSignal[0],
					  bgTot, err_bgTot,
					  100,
					  "bayesian");
    
  std::cout << " expected limit (median) " << expected.GetExpectedLimit() << std::endl;
  std::cout << " expected limit (-1 sig) " << expected.GetOneSigmaLowRange() << std::endl;
  std::cout << " expected limit (+1 sig) " << expected.GetOneSigmaHighRange() << std::endl;
  std::cout << " expected limit (-2 sig) " << expected.GetTwoSigmaLowRange() << std::endl;
  std::cout << " expected limit (+2 sig) " << expected.GetTwoSigmaHighRange() << std::endl;

  // write summary file
  ofstream oFile;
  oFile.open( (oDir+std::string("/RooStatsCL95.txt")).c_str() );

  oFile << "Lumi=" << ilum << " +/- " << slum << std::endl;
  oFile << std::endl;
  oFile << "Backgrounds" << std::endl;
  oFile << "Z+jets       : " << bgZjets << " +/- " << err_bgZjets << std::endl;
  oFile << "W+jets       : " << bgWjets << " +/- " << err_bgWjets << std::endl;
  oFile << "QCD          : " << bgQCD   << " +/- " << err_bgQCD << std::endl;
  oFile << "Total        : " << bgTot << " +/- " << err_bgTot << std::endl;
  oFile << std::endl;
  oFile << "Observed     : " << nObs << std::endl;
  oFile << std::endl;
  oFile << "Signal point" << std::endl;
  oFile << "mH=" << mH[0] << ".  VBF x-section=" << xsH[0] << std::endl;
  oFile << "eff=" << effSignal[0] << " +/- " << err_effSignal[0] << std::endl;
  oFile << "yield (100% inv BF)=" << int(xsH[0]*ilum*effSignal[0]) << std::endl;
  oFile << std::endl;
  oFile << "Results" << std::endl;
  oFile << "  expected limit (median) " << expected.GetExpectedLimit() << std::endl;
  oFile << "  expected limit (-1 sig) " << expected.GetOneSigmaLowRange() << std::endl;
  oFile << "  expected limit (+1 sig) " << expected.GetOneSigmaHighRange() << std::endl;
  oFile << "  expected limit (-2 sig) " << expected.GetTwoSigmaLowRange() << std::endl;
  oFile << "  expected limit (+2 sig) " << expected.GetTwoSigmaHighRange() << std::endl;
  oFile << std::endl;

  oFile.close();

  return;
}
