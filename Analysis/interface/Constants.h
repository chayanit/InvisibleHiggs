

namespace constants {

  //// Backgrounds ////

  //  Z extrapolation
  static const double ratioZToNuNuZToLL = 5.626;

  // background values and total uncertainty (temporary!)
  static const double nBG_WTau   = 0.;
  static const double stat_WTau  = 0.;


  // systematics (these are fractional values not percentages)
  //   these should not include MC stats !
  static const double syst_effMu      = 0.025;
  static const double syst_lumi       = 0.04;

  // Z->nunu systematics
  static const double syst_Z          = 0.20;    // 20% extrapolation systematics
  static const double syst_Z_JES_m    = -0.045;
  static const double syst_Z_JES_p    = 0.0091;
  static const double syst_Z_JER_m    = 0.034;
  static const double syst_Z_JER_p    = -0.0073;
  static const double syst_Z_UES_m    = -0.0062;
  static const double syst_Z_UES_p    = -0.0023;

  // W->munu
  static const double syst_WMu        = 0.20;    // 20% extrapolation systematics
  static const double syst_WMu_JES_m  = -0.00024;
  static const double syst_WMu_JES_p  = -0.020;
  static const double syst_WMu_JER_m  = 0.042;
  static const double syst_WMu_JER_p  = -0.024;
  static const double syst_WMu_UES_m  = 0.011;
  static const double syst_WMu_UES_p  = 0.0056;

  // W->enu
  static const double syst_WEl         = 0.20;    // 20% extrapolation systematics
  static const double syst_WEl_JES_m   = 0.030;
  static const double syst_WEl_JES_p   = -0.11;
  static const double syst_WEl_JER_m   = 0.046;
  static const double syst_WEl_JER_p   = 0.027;
  static const double syst_WEl_UES_m   = 0.019;
  static const double syst_WEl_UES_p   = -0.0071;

  // W->taunu
  static const double syst_WTau       = 0.22;    // 20% extrapolation + 8% tau ID + 5& lepton contamination
  static const double syst_WTau_JES_m = 0.011;
  static const double syst_WTau_JES_p = 0.015;
  static const double syst_WTau_JER_m = -0.041;
  static const double syst_WTau_JER_p = 0.12;
  static const double syst_WTau_UES_m = 0.017;
  static const double syst_WTau_UES_p = 0.00099;

  // QCD 1
  static const double syst_QCD1        = 1.5;      // QCD 1 is not used any more
  static const double syst_QCD1_JES_m  = 0.0;
  static const double syst_QCD1_JES_p  = 0.0;
  static const double syst_QCD1_JER_m  = 0.0;
  static const double syst_QCD1_JER_p  = 0.0;
  static const double syst_QCD1_UES_m  = 0.0;
  static const double syst_QCD1_UES_p  = 0.0;

  // QCD 3
  static const double syst_QCD3        = 0.35;     // 35% shape systematic
  static const double syst_QCD3_JES_m  = 0.41;
  static const double syst_QCD3_JES_p  = -0.51;
  static const double syst_QCD3_JER_m  = -0.19;
  static const double syst_QCD3_JER_p  = 0.13;
  static const double syst_QCD3_UES_m  = -0.52;
  static const double syst_QCD3_UES_p  = -0.032;

  // ttbar
  static const double syst_TTbar         = 0.07;   // 7% x-section
  static const double syst_TTbar_JES_m   = -0.54;
  static const double syst_TTbar_JES_p   = 0.54;
  static const double syst_TTbar_JER_m   = 0.0;
  static const double syst_TTbar_JER_p   = 0.0;
  static const double syst_TTbar_UES_m   = 0.0;
  static const double syst_TTbar_UES_p   = 0.0;

  // diboson
  static const double syst_Diboson       = 0.18;   // 18% x-section
  static const double syst_Diboson_JES_m = -0.28;
  static const double syst_Diboson_JES_p = 0.28;
  static const double syst_Diboson_JER_m = 0.0;
  static const double syst_Diboson_JER_p = 0.0;
  static const double syst_Diboson_UES_m = 0.0;
  static const double syst_Diboson_UES_p = 0.0;

  // single T
  static const double syst_SingleT       = 0.16;   // 16% x-section
  static const double syst_SingleT_JES_m = -0.43;
  static const double syst_SingleT_JES_p = 0.43;
  static const double syst_SingleT_JER_m = 0.0;
  static const double syst_SingleT_JER_p = 0.0;
  static const double syst_SingleT_UES_m = 0.0;
  static const double syst_SingleT_UES_p = 0.0;

  // DY
  static const double syst_DYLL          = 0.20;   // 20% x-section
  static const double syst_DYLL_JES_m    = -0.81;
  static const double syst_DYLL_JES_p    = 0.81;
  static const double syst_DYLL_JER_m    = 0.0;
  static const double syst_DYLL_JER_p    = 0.0;
  static const double syst_DYLL_UES_m    = 0.0;
  static const double syst_DYLL_UES_p    = 0.0;



  //// Signal properties ////

  // masses
  static const int nM = 6;
  static const double mH[6]  = { 110.,  125.,  150.,  200.,   300.,   400.   };  // GeV

  // cross-sections
  static const double xsH[6] = { 1.809, 1.649, 1.280, 0.8685, 0.4408, 0.2543 };  // pb

  // systematics
  static const double syst_Sig_QCD[6]    = { 0.04,    0.04,    0.04,    0.04,    0.04,   0.04 };
  static const double syst_Sig_PDF[6]    = { 0.041,   0.047,   0.043,   0.047,   0.054,  0.055 };
  static const double syst_Sig_JES_m[6]  = { -0.084,  -0.12,   -0.10,   -0.11,   -0.075, -0.12 };
  static const double syst_Sig_JES_p[6]  = { 0.11,    0.10,    0.091,   0.13,    0.056,  0.10 };
  static const double syst_Sig_JER_m[6]  = { 0.016,   0.025,   0.015,   0.037,   0.0078, 0.017 };
  static const double syst_Sig_JER_p[6]  = { -0.0056, -0.036,  -0.028,  -0.017,  -0.014, -0.037 };
  static const double syst_Sig_UES_m[6]  = { -0.011,  -0.0033, -0.0058, -0.0064, -0.010, -0.00041 };
  static const double syst_Sig_UES_p[6]  = { 0.011,   0.0022,  0.0037,  -0.0014, 0.087,  0.0020 };

}

