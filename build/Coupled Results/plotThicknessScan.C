#include <map>
#include <iostream>
#include <cmath>
#include "TFile.h"
#include "TTree.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TF1.h"
#include "TAxis.h"

Int_t eventid;
Double_t Edep;

// Compute per-event averaged E_bottom / E_total for one file
double computeEnergyFraction(TTree *pB, TTree *pI, TTree *pO, double &rms) {

    std::map<int,double> EB, EI, EO;

    if(pB){
        pB->SetBranchAddress("eventid",&eventid);
        pB->SetBranchAddress("Edep",&Edep);
        for(int i=0;i<pB->GetEntries();i++){ pB->GetEntry(i); EB[eventid]+=Edep; }
    }
    if(pI){
        pI->SetBranchAddress("eventid",&eventid);
        pI->SetBranchAddress("Edep",&Edep);
        for(int i=0;i<pI->GetEntries();i++){ pI->GetEntry(i); EI[eventid]+=Edep; }
    }
    if(pO){
        pO->SetBranchAddress("eventid",&eventid);
        pO->SetBranchAddress("Edep",&Edep);
        for(int i=0;i<pO->GetEntries();i++){ pO->GetEntry(i); EO[eventid]+=Edep; }
    }

    // Per-event fraction
    std::vector<double> fracs;
    for(auto &kv : EB){
        int ev = kv.first;
        double Etot = EB[ev] + EI[ev] + EO[ev];
        if(Etot == 0) continue;
        fracs.push_back(EB[ev] / Etot);
    }

    if(fracs.empty()){ rms=0; return 0; }

    // Mean
    double mean = 0;
    for(auto &f : fracs) mean += f;
    mean /= fracs.size();

    // RMS for error bar
    double var = 0;
    for(auto &f : fracs) var += (f-mean)*(f-mean);
    rms = std::sqrt(var/fracs.size()) / std::sqrt(fracs.size()); // standard error on mean

    return mean;
}

void plotThicknessScan() {

    const int Nfiles = 4;
    double thickness[Nfiles] = {10, 20, 35, 50};
    double frac[Nfiles];
    double err[Nfiles];

    const char* filenames[Nfiles] = {
        "10nmALBFT_merged.root",
        "20nmALBFT_merged.root",
        "35nmALBFT_merged.root",
        "50nmALBFT_merged.root"
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

    // --- Overlay analytical KaplanQP prediction ---
    const double vSound         = 3.26e12;  // nm/s
    const double phononLifetime = 242e-12;  // s
    const double filmAbsorption = 0.20;
    const double MFP            = vSound * phononLifetime;  // nm

    const int Ntheo = 100;
    double theoX[Ntheo], theoY[Ntheo];
    for(int i=0; i<Ntheo; i++){
        theoX[i] = 1. + i * (200./Ntheo);  // 1 to 200 nm
        double P = filmAbsorption * (1. - std::exp(-4.*theoX[i]/MFP));
        theoY[i] = P;  // this is P_absorb per hit, not the fraction
    }

    // --- Plot simulation result with error bars ---
    TGraphErrors *gSim = new TGraphErrors(Nfiles, thickness, frac,
                                          nullptr, err);
    gSim->SetTitle("Bottom Film Energy Fraction vs KaplanQP Thickness;"
                   "filmThickness (nm);"
                   "<E_{bottom} / E_{total absorbed}>");
    gSim->SetLineWidth(3);
    gSim->SetMarkerStyle(20);
    gSim->SetMarkerSize(1.8);
    gSim->SetLineColor(kBlack);
    gSim->SetMarkerColor(kBlack);

    TCanvas *c = new TCanvas("c","Thickness Scan",1400,1000);
    gSim->Draw("APL");
    c->SaveAs("ThicknessScan_EnergyFraction.png");

    std::cout << "\nPlot saved to ThicknessScan_EnergyFraction.png\n";
    std::cout << "\nExpected P_absorb per hit from KaplanQP formula:\n";
    std::cout << "  MFP = " << MFP << " nm\n";
    for(int i=0; i<Nfiles; i++){
        double d = thickness[i];
        double P = filmAbsorption * (1. - std::exp(-4.*d/MFP));
        std::cout << "  " << d << " nm -> P_total = " << P << "\n";
    }
}
