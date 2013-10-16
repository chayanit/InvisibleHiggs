// Macro to create TH2s from lepton weight text files, like for trigger weights
// Still a work in progress: need to add in other text files & push them to git, and add in check in makeHists() to check file exists
//
// Robin Aggleton 2013

void makeHists(std::string filename){
    ifstream infile;
    infile.open(filename.c_str());
    std::string line;
    double lastPtMin(0.), lastPtMax(0.), lastEtaMin(-1.), lastEtaMax(0.);
    std::vector<double> ptBins;
    std::vector<double> etaBins;
    std::vector<double> SF;
    std::vector<double> SFerrUp;
    std::vector<double> SFerrDown;
    int nPtBins(0),nEtaBins(0);
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
    // for(unsigned i = 0; i < ptBins.size(); i++)
        // std::cout << ptBins[i] << std::endl;
    // for(unsigned i = 0; i < etaBins.size(); i++)
        // std::cout << etaBins[i] << std::endl;
    std::cout << "Number of pt bins: " << nPtBins <<std::endl;
    std::cout << "Number of eta bins: " << nEtaBins <<std::endl;

    int lastindex = filename.find_last_of("."); 
    string histName = filename.substr(0, lastindex);

    TH2D histSF(histName.c_str(),";p_{T};#eta",nPtBins,&ptBins[0],nEtaBins,&etaBins[0]);
    TH2D histSFup((histName+"_errUp").c_str(),";p_{T};#eta",nPtBins,&ptBins[0],nEtaBins,&etaBins[0]);
    TH2D histSFdown((histName+"_errDown").c_str(),";p_{T};#eta",nPtBins,&ptBins[0],nEtaBins,&etaBins[0]);
    for (int a = 0; a < etaBins.size()-1 ; a++) {
        for (int b = 0; b < ptBins.size()-1; b++){
            std::cout << SF[b+(a*(ptBins.size()-1))] << "   ";
            histSF.SetBinContent(b+1,a+1,SF[b+(a*nPtBins)]);
            histSFup.SetBinContent(b+1,a+1,SF[b+(a*nPtBins)]+SFerrUp[b+(a*nPtBins)]);
            histSFdown.SetBinContent(b+1,a+1,SF[b+(a*nPtBins)]-SFerrDown[b+(a*nPtBins)]);
        }
        std::cout << std::endl;
    }

    TCanvas c1;
    histSF.Draw("TEXT90COLZ");
    c1.SaveAs((histName+".pdf").c_str());
    TFile *file = new TFile((histName+".root").c_str(),"RECREATE");
    histSF.Write();
    histSFup.Write();
    histSFdown.Write();
}

void MakeLeptonWeightHistos(){
    gStyle->SetOptStat("n");
    makeHists("ele_tight_id.txt");
}