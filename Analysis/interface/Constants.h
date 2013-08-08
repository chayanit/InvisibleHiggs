

namespace constants {

  ///// Higgs masses and cross-sections
  static const int nM = 6;
  static const double mH[6]  = { 110.,  125.,  150.,  200.,   300.,   400.   };  // GeV
  static const double xsH[6] = { 1.809, 1.649, 1.280, 0.8685, 0.4408, 0.2543 };  // pb


  //// Z values
  static const double ratioZToNuNuZToLL = 5.626;


  //// background values and total uncertainty (temporary!)
  static const double nBG_WTau   = 64.6;
  static const double stat_WTau  = 0.31;


  //// systematics (these are fractional values not percentages)
  ////   these should not include MC stats !
  static const double syst_effMu  = 0.025;   // 2.5% muon efficiency

  static const double syst_lumi    = 0.04;   // 4% lumi
  static const double syst_Z       = 0.21;   // 5% JES/JER + 20% MCFM/MG ratio
  static const double syst_WMu     = 0.20;   // 4% JES/JER + 20% MCFM/MG ratio
  static const double syst_WEl     = 0.23;   // 11% JES/JER + 20% MCFM/MG ratio
  static const double syst_WTau    = 0.27;   // 16% JES/JER + 8% tau ID + 5% W-e cont + 20% MCFM/MG ratio
  static const double syst_QCD1    = 1.5;    // 150% uncertainty from 
  static const double syst_QCD3    = 0.83;   // 75% JES/JER + 35% MET shape
  static const double syst_TTbar   = 0.54;   // 54% JES/JER + 7% x-section
  static const double syst_Diboson = 0.33;   // 28% JES/JER + 18% x-section
  static const double syst_SingleT = 0.46;   // 43% JES/JER + 16% x-section
  static const double syst_DYLL    = 0.83;   // 81% JES/JER + 20% x-section

  // systematics - Signal
  static const double syst_Sig_JES[6] = {0.110, 0.121, 0.108, 0.131, 0.077, 0.122};   // really JES + JER + UES
  static const double syst_Sig_PDF[6] = {0.041, 0.047, 0.043, 0.047, 0.054, 0.055};

}

