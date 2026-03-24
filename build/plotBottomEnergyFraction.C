#include <map>
#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TLegend.h"

Int_t eventid;
Double_t Edep;

std::map<int,double> EB, EI, EO;

double computeEnergyFraction(TTree *pB, TTree *pI, TTree *pO){

    EB.clear(); EI.clear(); EO.clear();

    // ---------- Bottom ----------
    if(pB){
        pB->SetBranchAddress("eventid",&eventid);
        pB->SetBranchAddress("Edep",&Edep);
        for(int i=0;i<pB->GetEntries();i++){
            pB->GetEntry(i);
            EB[eventid] += Edep;
        }
    }

    // ---------- Inner ----------
    if(pI){
        pI->SetBranchAddress("eventid",&eventid);
        pI->SetBranchAddress("Edep",&Edep);
        for(int i=0;i<pI->GetEntries();i++){
            pI->GetEntry(i);
            EI[eventid] += Edep;
        }
    }

    // ---------- Outer ----------
    if(pO){
        pO->SetBranchAddress("eventid",&eventid);
        pO->SetBranchAddress("Edep",&Edep);
        for(int i=0;i<pO->GetEntries();i++){
            pO->GetEntry(i);
            EO[eventid] += Edep;
        }
    }

    // ---------- Compute event-averaged fraction ----------
    double sumFraction = 0.0;
    int Nevt = 0;

    for(auto &kv : EB){
        int ev = kv.first;
        double Ebottom = EB[ev];
        double Etot = EB[ev] + EI[ev] + EO[ev];
        if(Etot == 0) continue;
        sumFraction += Ebottom / Etot;
        Nevt++;
    }

    return (Nevt > 0) ? sumFraction / Nevt : 0.0;
}


void plotBottomEnergyFraction(){

    const int Nfiles = 3;
    double thickness[Nfiles] = {10, 30, 50};
    double frac[Nfiles];

    const char* filenames[Nfiles] = {
        "10nmALBFT.root",
        "30nmALBFT.root",
        "50nmALBFT.root"
    };

    for(int i=0;i<Nfiles;i++){
        TFile *f = TFile::Open(filenames[i]);
        if(!f || f->IsZombie()){
            std::cout << "Warning: Could not open file "
                      << filenames[i] << ". Setting fraction=0.\n";
            frac[i] = 0;
            continue;
        }

        TTree *pB = (TTree*)f->Get("BottomHits");
        TTree *pI = (TTree*)f->Get("InnerHits");
        TTree *pO = (TTree*)f->Get("OuterHits");

        frac[i] = computeEnergyFraction(pB, pI, pO);

        std::cout << filenames[i] << ": <E_bottom/E_total> = " << frac[i] << "\n";

        f->Close();
    }

    TGraph *g = new TGraph(Nfiles, thickness, frac);
    g->SetTitle("Bottom Film Energy Fraction (Decoupled);KaplanQP filmThickness (nm);<E_{bottom}/E_{total absorbed}>");
    g->SetLineWidth(4);
    g->SetMarkerStyle(20);
    g->SetMarkerSize(1.8);
    g->SetLineColor(kBlack);
    g->SetMarkerColor(kBlack);

    TCanvas *c = new TCanvas("c","Bottom Energy Fraction vs Thickness",1400,1000);
    g->Draw("APL");
    c->SaveAs("BottomEnergyFraction_vs_Thickness_ALBFT.png");
    std::cout << "Plot saved to BottomEnergyFraction_vs_Thickness_ALBFT.png\n";
}
