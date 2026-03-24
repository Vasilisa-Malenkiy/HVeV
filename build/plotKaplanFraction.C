#include <map>
#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TLegend.h"

Int_t eventid;
std::map<int,int> NB, NI, NO;

double computeFraction(TTree *pB, TTree *pI, TTree *pO,
                       std::map<int,int> &NB, std::map<int,int> &NI, std::map<int,int> &NO,
                       Int_t &eventid) {
    NB.clear(); NI.clear(); NO.clear();
    if(pB){ pB->SetBranchAddress("eventid",&eventid); for(int i=0;i<pB->GetEntries();i++){pB->GetEntry(i); NB[eventid]++;} }
    if(pI){ pI->SetBranchAddress("eventid",&eventid); for(int i=0;i<pI->GetEntries();i++){pI->GetEntry(i); NI[eventid]++;} }
    if(pO){ pO->SetBranchAddress("eventid",&eventid); for(int i=0;i<pO->GetEntries();i++){pO->GetEntry(i); NO[eventid]++;} }
    double sum=0.0; int Nevt=0;
    for(auto &kv:NB){ int ev=kv.first; int ntot=NB[ev]+NI[ev]+NO[ev]; if(ntot==0) continue; sum+=(double)NB[ev]/ntot; Nevt++; }
    return (Nevt>0)?sum/Nevt:0.0;
}

void plotKaplanFraction() {

    const int Nfiles = 3;
    double thickness[Nfiles] = {10, 30, 50};

    const char* filenames[Nfiles] = {
        "10nmALBFT.root",
        "30nmALBFT.root",
        "50nmALBFT.root"
    };

    double frac[Nfiles];
    double nBottom[Nfiles];
    double nInner[Nfiles];
    double nOuter[Nfiles];

    for(int i = 0; i < Nfiles; i++){
        TFile *f = TFile::Open(filenames[i]);
        if(!f || f->IsZombie()){
            std::cout << "Warning: Could not open " << filenames[i] << ". Setting values=0.\n";
            frac[i] = 0; nBottom[i] = 0; nInner[i] = 0; nOuter[i] = 0;
            continue;
        }

        TTree *pB = (TTree*)f->Get("BottomHits");
        TTree *pI = (TTree*)f->Get("InnerHits");
        TTree *pO = (TTree*)f->Get("OuterHits");

        nBottom[i] = pB ? pB->GetEntries() : 0;
        nInner[i]  = pI ? pI->GetEntries() : 0;
        nOuter[i]  = pO ? pO->GetEntries() : 0;

        frac[i] = computeFraction(pB, pI, pO, NB, NI, NO, eventid);

        std::cout << filenames[i] << ":\n";
        std::cout << "  BottomHits = " << nBottom[i] << "\n";
        std::cout << "  InnerHits  = " << nInner[i]  << "\n";
        std::cout << "  OuterHits  = " << nOuter[i]  << "\n";
        std::cout << "  Total      = " << nBottom[i]+nInner[i]+nOuter[i] << "\n";
        std::cout << "  Fraction   = " << frac[i] << "\n\n";

        f->Close();
    }

    // Plot 1: N_bottom/N_total fraction vs filmThickness
    TGraph *gFrac = new TGraph(Nfiles, thickness, frac);
    gFrac->SetTitle("Bottom Film Phonon Fraction (Decoupled);KaplanQP filmThickness (nm);<N_{bottom}/N_{total}>");
    gFrac->SetLineWidth(3);
    gFrac->SetMarkerStyle(20);
    gFrac->SetMarkerSize(1.8);

    TCanvas *c1 = new TCanvas("c1", "Kaplan Fraction", 1400, 1000);
    gFrac->Draw("APL");
    c1->SaveAs("KaplanFraction_vs_FilmThickness.png");

    // Plot 2: Absolute hits per surface — Inner/Outer should be flat
    TGraph *gBottom = new TGraph(Nfiles, thickness, nBottom);
    gBottom->SetLineWidth(3); gBottom->SetMarkerStyle(20); gBottom->SetMarkerSize(1.8);
    gBottom->SetLineColor(kBlack); gBottom->SetMarkerColor(kBlack);

    TGraph *gInner = new TGraph(Nfiles, thickness, nInner);
    gInner->SetLineWidth(3); gInner->SetMarkerStyle(21); gInner->SetMarkerSize(1.8);
    gInner->SetLineColor(kRed); gInner->SetMarkerColor(kRed);

    TGraph *gOuter = new TGraph(Nfiles, thickness, nOuter);
    gOuter->SetLineWidth(3); gOuter->SetMarkerStyle(22); gOuter->SetMarkerSize(1.8);
    gOuter->SetLineColor(kBlue); gOuter->SetMarkerColor(kBlue);

    TCanvas *c2 = new TCanvas("c2", "Absolute Hits per Surface", 1400, 1000);
    gBottom->SetTitle("Absolute Hits per Surface (Decoupled);KaplanQP filmThickness (nm);N_{hits}");
    gBottom->Draw("APL");
    gInner->Draw("PL same");
    gOuter->Draw("PL same");

    TLegend *leg = new TLegend(0.65, 0.70, 0.88, 0.88);
    leg->AddEntry(gBottom, "Bottom (KaplanQP varying)", "lp");
    leg->AddEntry(gInner,  "Inner Top (fixed 600nm)", "lp");
    leg->AddEntry(gOuter,  "Outer Top (fixed 600nm)", "lp");
    leg->Draw();

    c2->SaveAs("AbsoluteHits_Decoupled.png");
}
