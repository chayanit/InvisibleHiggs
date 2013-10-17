// Macro to create TH2s from lepton weight text files, like for trigger weights
// They can then be used in Ntuple code
//
// Robin Aggleton 2013

void makeHists(std::string filename, TFile* outputFile){
    ifstream infile;
    infile.open(filename.c_str());
    if (!infile.fail()) { //check if file opened correctly
        std::string line;
        double lastPtMin(0.), lastPtMax(0.), lastEtaMin(-1.), lastEtaMax(0.);
        std::vector<double> ptBins;
        std::vector<double> etaBins;
        std::vector<double> SF;
        std::vector<double> SFerrUp;
        std::vector<double> SFerrDown;
        int nPtBins(0),nEtaBins(0);

        // Read in each line of txt file and process
        // We want the pT and eta bins out, as well as storign the scale factors, and the +/- errors on it
        while (std::getline(infile, line))
        {
            std::stringstream ss(line);
            double ptMin, ptMax, etaMin, etaMax, sf, sfErrP, sfErrM;
            if (ss >> ptMin >> ptMax >> etaMin >> etaMax >> sf >> sfErrP >> sfErrM) {
                SF.push_back(sf);
                SFerrUp.push_back(sfErrP);
                SFerrDown.push_back(sfErrM);
                if (ptMin > lastPtMin) {
                    lastPtMin = ptMin;
                    nPtBins++;
                    ptBins.push_back(ptMin);
                }
                if (etaMin > lastEtaMin) {
                    lastEtaMin = etaMin;
                    nEtaBins++;
                    etaBins.push_back(etaMin);
                }
                lastPtMax  = ptMax;
                lastEtaMax = etaMax;
            }
        }    
        ptBins.push_back(lastPtMax);
        etaBins.push_back(lastEtaMax);
        infile.close();

        // Make a sensible hist name out of the filename - chop off the trailing .txt
        int lastindex = filename.find_last_of("."); 
        string histName = filename.substr(0, lastindex);

        // Now make 2D hists with the above pT and eta bins, and fill them with scale factors (central, error up and error down)
        TH2D histSF(histName.c_str(),";p_{T};#eta",nPtBins,&ptBins[0],nEtaBins,&etaBins[0]);
        TH2D histSFup((histName+"_errUp").c_str(),";p_{T};#eta",nPtBins,&ptBins[0],nEtaBins,&etaBins[0]);
        TH2D histSFdown((histName+"_errDown").c_str(),";p_{T};#eta",nPtBins,&ptBins[0],nEtaBins,&etaBins[0]);
        for (int a = 0; a < etaBins.size()-1 ; a++) {
            for (int b = 0; b < ptBins.size()-1; b++){
                // std::cout << SF[b+(a*(ptBins.size()-1))] << "   ";
                histSF.SetBinContent(b+1,a+1,SF[b+(a*nPtBins)]);
                histSFup.SetBinContent(b+1,a+1,SF[b+(a*nPtBins)]+SFerrUp[b+(a*nPtBins)]);
                histSFdown.SetBinContent(b+1,a+1,SF[b+(a*nPtBins)]-SFerrDown[b+(a*nPtBins)]);
            }
            // std::cout << std::endl;
        }

        // Uncomment the following 3 lines if you want a pdf of the scale factor
        // Makes it very slow though!
        // TCanvas c1;
        // histSF.Draw("TEXT90COLZ");
        // c1.SaveAs((histName+".pdf").c_str());

        // Save TH2s to file
        // TFile *file = new TFile((histName+".root").c_str(),"RECREATE");
        outputFile->cd();
        histSF.Write();
        histSFup.Write();
        histSFdown.Write();
        // file->Close();
    } else 
        std::cout << "ERROR: No such file " << filename << std::endl;
}

void MakeLeptonWeightHistos(){
    gStyle->SetOptStat("n");
    std::vector<std::string> fileList;
    TFile *outputFile = new TFile("leptonWeights.root","RECREATE");
    makeHists("ele_tight_id.txt",outputFile);
    makeHists("ele_veto_id_data_eff.txt",outputFile);
    makeHists("ele_veto_id_mc_eff.txt",outputFile);
    makeHists("mu_loose_id_SF.txt",outputFile);
    makeHists("mu_loose_id_data_eff.txt",outputFile);
    makeHists("mu_loose_id_mc_eff.txt",outputFile);
    makeHists("mu_loose_iso_SF.txt",outputFile);
    makeHists("mu_loose_iso_data_eff.txt",outputFile);
    makeHists("mu_loose_iso_mc_eff.txt",outputFile);
    makeHists("mu_tight_id_SF.txt",outputFile);
    makeHists("mu_tight_id_data_eff.txt",outputFile);
    makeHists("mu_tight_id_mc_eff.txt",outputFile);
    makeHists("mu_tight_iso_SF.txt",outputFile);
    makeHists("mu_tight_iso_data_eff.txt",outputFile);
    makeHists("mu_tight_iso_mc_eff.txt",outputFile);
    outputFile->Close();
}