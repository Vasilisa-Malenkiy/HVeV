// BePositionScan.C
// Run with: root -l BePositionScan.C

#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TGraph.h"

void BePositionScan()
{
    const int N = 3;

    const char* files[N] = {
        "30nm_BePos100nm.root",
        "30nm_BePos200nm.root",
        "30nm_BePos300nm.root"
    };

    double position_nm[N] = {100,200,300};
    double frac[N] = {0};

    int colors[N] = {kRed, kBlue, kGreen+2};

    // =============================
    // ENERGY HISTOGRAM OVERLAY
    // =============================

    TCanvas *cE = new TCanvas("cE","Energy vs Counts",900,650);
    TLegend *leg = new TLegend(0.65,0.7,0.88,0.88);

    bool firstDraw = true;

    for (int i=0;i<N;i++)
    {
        TFile *f = TFile::Open(files[i]);
        if(!f || f->IsZombie()){
            std::cout<<"Could not open "<<files[i]<<std::endl;
            continue;
        }

        std::cout<<"Opened "<<files[i]<<std::endl;

        TTree *EdepTree = (TTree*)f->Get("EnergyDeposit");
        if(!EdepTree){
            std::cout<<"No EnergyDeposit tree!"<<std::endl;
            f->Close();
            continue;
        }

        TH1D *h = new TH1D(
            Form("h_%d",(int)position_nm[i]),
            "Energy Deposition;Energy;Counts",
            120,0,130
        );

        h->SetDirectory(0);   // ← IMPORTANT FIX

        Double_t Edep;
        EdepTree->SetBranchAddress("Edep",&Edep);

        Long64_t nEntries = EdepTree->GetEntries();
        for(Long64_t j=0;j<nEntries;j++){
            EdepTree->GetEntry(j);
            h->Fill(Edep);
        }

        h->SetLineColor(colors[i]);
        h->SetLineWidth(3);

        if(firstDraw){
            h->Draw();
            firstDraw = false;
        }
        else{
            h->Draw("SAME");
        }

        leg->AddEntry(h,Form("Be @ %d nm",(int)position_nm[i]),"l");

        // ===== PHONON FRACTION =====
        TTree *b   = (TTree*)f->Get("BottomHits");
        TTree *in  = (TTree*)f->Get("InnerHits");
        TTree *out = (TTree*)f->Get("OuterHits");

        double Nb = b ? b->GetEntries() : 0;
        double Ni = in ? in->GetEntries() : 0;
        double No = out ? out->GetEntries() : 0;

        double Ntot = Nb + Ni + No;
        frac[i] = (Ntot>0) ? Nb/Ntot : 0.0;

        std::cout<<"Bottom fraction = "<<frac[i]<<std::endl;

        f->Close();
    }

    leg->Draw();
    cE->Update();
    cE->SaveAs("EnergyOverlay_vs_BePosition.png");

    // =============================
    // FRACTION GRAPH
    // =============================

    TCanvas *cP = new TCanvas("cP","Bottom Absorption",800,600);

    TGraph *g = new TGraph(N, position_nm, frac);

    g->SetTitle("Bottom Film Phonon Absorption;Be Position (nm);Bottom/Total Hits");
    g->SetMarkerStyle(20);
    g->SetMarkerSize(1.5);
    g->SetLineWidth(3);

    g->Draw("APL");

    cP->Update();
    cP->SaveAs("BottomPhononFraction_vs_BePosition.png");
}

// Automatically execute when file is run
BePositionScan();
