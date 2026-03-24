#include <map>
#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TGraph.h"
#include "TCanvas.h"

Int_t eventid;
std::map<int,int> NB, NI, NO;

double computeFraction(TTree *pB, TTree *pI, TTree *pO, std::map<int,int> &NB, std::map<int,int> &NI, std::map<int,int> &NO, Int_t &eventid){
    NB.clear(); NI.clear(); NO.clear();
    if(pB){ pB->SetBranchAddress("eventid",&eventid); for(int i=0;i<pB->GetEntries();i++){pB->GetEntry(i); NB[eventid]++;} }
    if(pI){ pI->SetBranchAddress("eventid",&eventid); for(int i=0;i<pI->GetEntries();i++){pI->GetEntry(i); NI[eventid]++;} }
    if(pO){ pO->SetBranchAddress("eventid",&eventid); for(int i=0;i<pO->GetEntries();i++){pO->GetEntry(i); NO[eventid]++;} }
    double sum=0.0; int Nevt=0;
    for(auto &kv:NB){ int ev=kv.first; int ntot=NB[ev]+NI[ev]+NO[ev]; if(ntot==0) continue; sum+=(double)NB[ev]/ntot; Nevt++; }
    return (Nevt>0)?sum/Nevt:0.0;
}

void plotBottomFraction(){

    const int Nfiles = 13;
    double thickness[Nfiles] = {1,5,10,15,20,25,30,35,40,45,50,55,60};
    double frac[Nfiles];

    const char* filenames[Nfiles] = {
        "1nm_0Vbfilm.root",
        "5nm_0Vbfilm.root",
        "10nm_0Vbfilm.root",
        "15nm_0Vbfilm.root",
        "20nm_0Vbfilm.root",
        "25nm_0Vbfilm.root",
        "30nm_0Vbfilm.root",
        "35nm_0Vbfilm.root",
        "40nm_0Vbfilm.root",
        "45nm_0Vbfilm.root",
        "50nm_0Vbfilm.root",
        "55nm_0Vbfilm.root",
        "60nm_0Vbfilm.root"
    };

    for(int i=0;i<Nfiles;i++){
        TFile *f = TFile::Open(filenames[i]);
        if(!f || f->IsZombie()){
            std::cout << "Warning: Could not open file " << filenames[i] << ". Setting fraction=0.\n";
            frac[i] = 0;
            continue;
        }

        TTree *pB = (TTree*)f->Get("BottomHits");
        TTree *pI = (TTree*)f->Get("InnerHits");
        TTree *pO = (TTree*)f->Get("OuterHits");

        frac[i] = computeFraction(pB,pI,pO,NB,NI,NO,eventid);
        f->Close();
    }

    TGraph *g = new TGraph(Nfiles, thickness, frac);
    g->SetTitle("Bottom Film Phonon Absorption;Film Thickness (nm);<N_{bottom}/N_{total}>");
    g->SetLineWidth(4);
    g->SetMarkerStyle(20);
    g->SetMarkerSize(1.8);

    TCanvas *c = new TCanvas("c","Bottom Phonon Absorption vs Thickness",1400,1000);
    g->Draw("APL");
    c->SaveAs("BottomPhononFraction_vs_Thickness.png");
}
