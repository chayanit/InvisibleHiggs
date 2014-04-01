{
  ////////////////////////////////////////////////////////////
  // INSTRUCTIONS FOR USE                                   //
  //                                                        //
  // 1. Have ROOT files with hists to be plotted,           //
  //    e.g. ZZ.root,W+jets.root, etc each with a hist hMjj //
  //    You will need to run zBackground and controlPlots   //
  //    once already                                        //
  //     
  // 2. Compile the StackPlot library:                      //
  //    ./makePlotsScripts.sh                               //
  //    (Do this if you change StackPlot.cc or StackPlot.h) //
  //                                                        //
  // 3. Modify this script!                                 //
  //                                                        //
  // 4. Run this script in batch & quit mode:               //
  //      root -q -b plotScript.C                           //
  //                                                        //
  // 5. Watch as plots are made and saved as PDFs           //
  //                                                        //
  // Any questions, ask Robin: robin.aggleton@cern.ch       //
  ////////////////////////////////////////////////////////////

  // Load StackPlot library file
	int load = gSystem->Load("StackPlot_cc.so");
	if (load < 0) {
		std::cerr << " Didn't load StackPlot library correctly - make sure you have compiled!" << std::endl;
		return 1;
	}

  // Location of root files with hists - cahnge these to suit your setup
  std::string signalDir = "/afs/cern.ch/work/r/raggleto/scratch0/InvHiggsV11/CMSSW_5_3_7_patch5/src/InvisibleHiggs/Analysis/controlPlots/ControlPlots/";
	std::string plotDir = "/afs/cern.ch/work/r/raggleto/scratch0/InvHiggsV11/CMSSW_5_3_7_patch5/src/InvisibleHiggs/Analysis/scripts/plotting/";
	
  //////////////////////////////
  // Make signal region plots //
  //////////////////////////////

  // Initialise StackPlot object
  StackPlot signalPlots(signalDir, plotDir);
	
  // Manually set legend position e.g.
  signalPlots.setLegPos(0.63,0.63,0.93,0.89);

  // Manually set lumi to be displayed - NB doesn't rescale anything
  signalPlots.setLumi(19.5);

  // Manually set text size and position e.g.
  // signalPlots.setTextPos(0.38, 0.70, 0.6, 0.89);

  // Add datasets to be plotted
  // The first argument is the name of the ROOT file to be used
  // You can then optionally provide another argument for the name to be displayed on the legend,
  // but if you don't then the name displayed will be the same as the ROOT filename
  // There is also an argument for the colour to be used.
  // The last argument is the plotting option:
  // 0 = add to the BG stack, 1 = data (black points), 2 = signal MC (non-stacked line), 3 = signal MC (line, stacked on top of BG)
  signalPlots.addDataset("tt+DY+VV","t#bar{t}, tW, DY(ll)+jets, VV", kAzure-2, 0);
  signalPlots.addDataset("V+jets", kPink-4,0);
  signalPlots.addDataset("Signal 100%BR","#splitline{VBF m_{H} = 125 GeV,}{B(H#rightarrowinv) = 100%}",kRed, 3);
  signalPlots.addDataset("METABCD",    kBlack, 1);

  // Now that we've added in some datasets, let's plot some hists
  // Note that the ROOT files should have these hists in them, otherwise you get rubbish/seg faults
  // The arguments are:
  //  - hist name in ROOT file
  //  - x title
  //  - y title
  //  - whether to use log y axis (1 = log y axis)
  //  - whether to draw ratio plot below or not (1 = draw ratio plot)
  // Can also manually set y axis max and min e.g.
  
  // signalPlots.setYMax(5E4);
  // signalPlots.setXMax(4.5);
  // signalPlots.setXMin(3.8);
  // signalPlots.setLabel("(ee+#mu#mu)");

  signalPlots.setXMin(1100.);
  signalPlots.setXMax(3500.);
  signalPlots.draw("hMjjSigNoRatio", "M_{jj} [GeV]", "Events / 100 GeV",1,"SIG");

  signalPlots.setXMin(150.);
  signalPlots.setXMax(500.);
  signalPlots.draw("hMETSigNoRatio", "E_{T}^{miss} [GeV]", "Events / 20 GeV",1,"SIG");
  // signalPlots.draw("hDPhiJJSig", "#Delta #phi_{jj}", "Events",1,1);

  //////////////////////////
  // Make Z control plots //
  //////////////////////////

  std::string histDir = "/afs/cern.ch/work/r/raggleto/scratch0/InvHiggsV11/CMSSW_5_3_7_patch5/src/InvisibleHiggs/Analysis/ZBackground2/ZBackground/";
  StackPlot zPlots(histDir, plotDir);
  zPlots.addDataset("tt+VV", "t#bar{t}, tW, VV", kAzure-2, 0);
  zPlots.addDataset("DY+jets", "DY(ll)+jets", kPink-4, 0);
  zPlots.addDataset("METABCD", kBlack, 1);
  
  zPlots.setLegPos(0.70,0.60,0.93,0.89);
  zPlots.setLumi(19.5);
  zPlots.setScaleMCtoData(true); // option to rescale MC to data

  zPlots.setYMax(5.e2);
  zPlots.setXMin(1100.);
  zPlots.setXMax(2600.);
  zPlots.draw("ZCtrlMjj","M_{jj} [GeV]","Events / 100 GeV",1,"RATIO");
  zPlots.setYMax(5.e2);
  zPlots.setXMin(130.);
  zPlots.draw("ZCtrlMET","E_{T}^{miss} [GeV]","Events / 20 GeV",1,"RATIO");
  zPlots.setYMax(80.);
  zPlots.setYMin(0.);
  zPlots.draw("ZCtrlZMass","M_{#mu#mu}  [GeV]","Events / 5 GeV",0,"RATIO");

  ////////////////////
  // Make N-1 plots //
  ////////////////////

  // histDir ="";
  // StackPlot plots(histDir, plotDir);
  // plots.addDataset("Diboson", kViolet-6, 0);
  // plots.addDataset("DYJets+EWK", kPink-4,0);
  // plots.addDataset("SingleT+TTbar", kAzure-2, 0);
  // plots.addDataset("QCD", kGreen+3, 0);
  // plots.addDataset("ZJets+EWK", kOrange-2, 0);
  // plots.addDataset("WNJets+EWK", kGreen-3, 0);
  // plots.addDataset("Signal","Signal 100%BR", kRed, 2);
  // plots.addDataset("METABCD",    kBlack, 1);

  // plots.setLumi(19.5); // doens't rescale anything, just adds a bit of text on the plot
  // std::cout << "plotting hTrigNM1" << std::endl;
  // plots.draw("hTrigNM1", "", "",1,1);
  // plots.draw("hMETFiltNM1", "", "",1,1);
  // plots.draw("hDijetNM1", "Sub-leading jet p_{T} [GeV]", "Events / 5 GeV",1,1);
  // plots.setYMax(5E4);
  // plots.draw("hDEtaJJNM1", "#Delta #eta_{jj}", "Events / 0.16",1,1);
  // plots.draw("hMjjNM1", "M_{jj} [GeV]", "Events / 100 GeV",1,1);
  // plots.draw("hMETNM1", "E_{T}^{miss} [GeV]", "Events / 20 GeV",1,1);
  // plots.draw("hDPhiJMetNM1", "#Delta #phi_{j-#slash{E}_{T}}", "N_{events}",1,1);
  // plots.draw("hDPhiJMetNM1NoDPhiJJ", "#Delta #phi_{j-#slash{E}_{T}}, no #Delta #phi_{jj} cut", "N_{events}",1,1);
  // plots.setYMax(5E5);
  // plots.draw("hDPhiJJNM1", "#Delta #phi_{jj}", "Events / (#pi/25)",1,1);
  // plots.setYMax(1E5);
  // plots.draw("hCenEtNM1", "Central Jet E_{T} [GeV]", "Events / 5 GeV",1,1);
  // plots.draw("hCenEtaNM1", "Central Jet #eta", "N_{events}",1,1);

  // plots.setYMin(1e-1);
  // plots.setYMax(1e2);
  // plots.draw("hEVetoNM1", "E_{T} [GeV]", "N_{events}",1,1);
  // plots.draw("hMuVetoNM1", "p_{T} [GeV]", "N_{events}",1,1);

  // plots.setYMin(1e-2);
  // plots.draw("hDPhiJMetNormNM1", "#Delta #phi_{N}^{min}", "N_{events}",1,1);
  // plots.draw("hDPhiJMetNM1NoDPhiJJ", "#Delta #phi_{j-#slash{E}_{T}}, no #Delta #phi_{jj} cut", "N_{events}",1,1);
  // plots.setYMax(5E5);
  // plots.draw("hDPhiJJNM1", "#Delta #phi_{jj}", "Events / (#pi/25)",1,1);
  // plots.setYMax(1E5);
  // plots.draw("hCenEtNM1", "Central Jet E_{T} [GeV]", "Events / 5 GeV",1,1);
  // plots.draw("hCenEtaNM1", "Central Jet #eta", "N_{events}",1,1);

  // plots.setYMin(1e-1);
  // plots.setYMax(1e2);
  // plots.draw("hEVetoNM1", "E_{T} [GeV]", "N_{events}",1,1);
  // plots.draw("hMuVetoNM1", "p_{T} [GeV]", "N_{events}",1,1);

  // plots.setYMin(1e-2);
  // plots.draw("hDPhiJMetNormNM1", "#Delta #phi_{N}^{min}", "N_{events}",1,1);
  // plots.draw("hDPhiJMetNormNM1NoDPhiJJ", "#Delta #phi_{N}^{min}, no #Delta #phi_{jj} cut", "N_{events}",1,1);

}
