#include <map>
#include <iostream>
#include <cmath>
#include <vector>
#include "TFile.h"
#include "TTree.h"
#include "TGraphErrors.h"
#include "TCanvas.h"

Int_t eventid;
Double_t Edep;

double computeEnergyFraction(TTree *pB, TTree *pI, TTree *pO, double &rms) {
    std::map<int,double> EB, EI, EO;
    if(pB){ pB->SetBranchAddress("eventid",&eventid); pB->SetBranchAddress("Edep",&Edep);
        for(int i=0;i<pB->GetEntries();i++){ pB->GetEntry(i); EB[eventid]+=Edep; } }
    if(pI){ pI->SetBranchAddress("eventid",&eventid); pI->SetBranchAddress("Edep",&Edep);
        for(int i=0;i<pI->GetEntries();i++){ pI->GetEntry(i); EI[eventid]+=Edep; } }
    if(pO){ pO->SetBranchAddress("eventid",&eventid); pO->SetBranchAddress("Edep",&Edep);
        for(int i=0;i<pO->GetEntries();i++){ pO->GetEntry(i); EO[eventid]+=Edep; } }

    std::vector<double> fracs;
    for(auto &kv : EB){
        int ev = kv.first;
        double Etot = EB[ev] + EI[ev] + EO[ev];
        if(Etot == 0) continue;
        fracs.push_back(EB[ev] / Etot);
    }
    if(fracs.empty()){ rms=0; return 0; }
    double mean = 0;
    for(auto &f : fracs) mean += f;
    mean /= fracs.size();
    double var = 0;
    for(auto &f : fracs) var += (f-mean)*(f-mean);
    rms = std::sqrt(var/fracs.size()) / std::sqrt(fracs.size());
    return mean;
}

void APSScanTest() {

    const int Nfiles = 3;
    double thickness[Nfiles] = {10, 20, 50};
    double frac[Nfiles];
    double err[Nfiles];

    const char* filenames[Nfiles] = {
        "10nmAtPS.root",
        "20nmAtPS.root",
        "50nmAtPS.root"
    };

    for(int i=0; i<Nfiles; i++){
        TFile *f = TFile::Open(filenames[i]);
        if(!f || f->IsZombie()){
            std::cout << "Warning: Could not open " << filenames[i] << "\n";
            frac[i]=0; err[i]=0;
            continue;
        }
        TTree *pB = (TTree*)f->Get("BottomHits");
        TTree *pI = (TTree*)f->Get("InnerHits");
        TTree *pO = (TTree*)f->Get("OuterHits");
        frac[i] = computeEnergyFraction(pB, pI, pO, err[i]);
        std::cout << filenames[i] << ":\n";
        std::cout << "  <E_bottom/E_total> = " << frac[i]
                  << " +/- " << err[i] << "\n\n";
        f->Close();
    }

    // --- Analytical KaplanQP prediction ---
    const double vSound         = 3.26e12;  // nm/s
    const double phononLifetime = 242e-12;  // s
    const double filmAbsorption = 0.20;
    const double MFP            = vSound * phononLifetime;  // nm

    // --- Plot ---
    TGraphErrors *gSim = new TGraphErrors(Nfiles, thickness, frac, nullptr, err);
    gSim->SetTitle("Bottom Film Energy Fraction vs Thickness (Uncoupled);"
                   "KaplanQP filmThickness (nm), geometry fixed at 1nm;"
                   "<E_{bottom} / E_{total absorbed}>");
    gSim->SetLineWidth(3);
    gSim->SetMarkerStyle(20);
    gSim->SetMarkerSize(1.8);
    gSim->SetLineColor(kBlue+1);
    gSim->SetMarkerColor(kBlue+1);

    TCanvas *c = new TCanvas("c", "Uncoupled Thickness Scan", 1400, 1000);
    gSim->Draw("APL");
    c->SaveAs("LALALA.png");

    std::cout << "\nPlot saved to LALALA.png\n";
    std::cout << "\nExpected P_absorb per hit from KaplanQP formula:\n";
    std::cout << "  MFP = " << MFP << " nm\n";
    for(int i=0; i<Nfiles; i++){
        double d = thickness[i];
        double P = filmAbsorption * (1. - std::exp(-4.*d/MFP));
        std::cout << "  " << d << " nm -> P_total = " << P << "\n";
    }
}
