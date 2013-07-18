

namespace constants {

  // Higgs masses and cross-sections
  static const int nM = 6;
  static const double mH[6]  = { 110.,  125.,  150.,  200.,   300.,   400.   };  // GeV
  static const double xsH[6] = { 1.809, 1.649, 1.280, 0.8685, 0.4408, 0.2543 };  // pb

  // systematics (fractional values)
  static const double syst_lumi = 0.04;

  static const double syst_JES_Z     = 0.03;
  static const double syst_JES_WMu   = 0.04;
  static const double syst_JES_WEl   = 0.14;
  static const double syst_JES_WTau  = 0.00;
  static const double syst_JES_Other = 0.40;

  static const double syst_QCD1 = 1.5;
  static const double syst_QCD3 = 0.35;

  // background values and total uncertainty (temporary!)
  static const double nBG_WTau   = 95.0;
  static const double errBG_WTau = 30.;

}

