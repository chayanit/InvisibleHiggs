

namespace constants {

  ///// Higgs masses and cross-sections
  static const int nM = 6;
  static const double mH[6]  = { 110.,  125.,  150.,  200.,   300.,   400.   };  // GeV
  static const double xsH[6] = { 1.809, 1.649, 1.280, 0.8685, 0.4408, 0.2543 };  // pb


  //// Z values
  static const double ratioZToNuNuZToLL = 5.626;


  //// background values and total uncertainty (temporary!)
  static const double nBG_WTau   = 76.0;
  static const double stat_WTau  = 0.32;


  //// systematics (these are fractional values not percentages)
  ////   these should not include MC stats !
  static const double syst_effMu  = 0.025;

  static const double syst_lumi   = 0.04;
  static const double syst_Z      = 0.03;
  static const double syst_WMu    = 0.04;
  static const double syst_WEl    = 0.14;
  static const double syst_WTau   = 0.25;
  static const double syst_QCD1   = 1.5;
  static const double syst_QCD3   = 0.35;
  static const double syst_Other  = 0.40;

  // systematics - Signal
  static const double syst_Sig_JES[6] = {0.110, 0.121, 0.108, 0.131, 0.077, 0.122};   // really JES + JER + UES
  static const double syst_Sig_PDF[6] = {0.041, 0.047, 0.043, 0.047, 0.054, 0.055};

}

