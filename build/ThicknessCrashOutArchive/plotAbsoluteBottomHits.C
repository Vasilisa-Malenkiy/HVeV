#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TAxis.h"

void plotAbsoluteBottomHits() {

    const int Nfiles = 4;
    double thickness[Nfiles] = {20, 40, 80, 150};

    const char* filenames[Nfiles] = {
        "20nm_0VbfilmTEST.root",
        "40nm_0VbfilmTEST.root",
        "80nm_0VbfilmTEST.root",
        "150nm_0VbfilmTEST.root",
    };

    double nBottom[Nfiles];
    double nInner[Nfiles];
    double nOuter[Nfiles];

    for (int i = 0; i < Nfiles; i++) {
        TFile *f = TFile::Open(filenames[i]);
        if (!f || f->IsZombie()) {
            std::cout << "Warning: Could not open " << filenames[i] << ". Setting counts=0.\n";
            nBottom[i] = 0; nInner[i] = 0; nOuter[i] = 0;
            continue;
        }

        TTree *pB = (TTree*)f->Get("BottomHits");
        TTree *pI = (TTree*)f->Get("InnerHits");
        TTree *pO = (TTree*)f->Get("OuterHits");

        nBottom[i] = pB ? pB->GetEntries() : 0;
        nInner[i]  = pI ? pI->GetEntries() : 0;
        nOuter[i]  = pO ? pO->GetEntries() : 0;

        std::cout << filenames[i] << ":\n";
        std::cout << "  BottomHits = " << nBottom[i] << "\n";
        std::cout << "  InnerHits  = " << nInner[i]  << "\n";
        std::cout << "  OuterHits  = " << nOuter[i]  << "\n";
        std::cout << "  Total      = " << nBottom[i]+nInner[i]+nOuter[i] << "\n\n";

        f->Close();
    }

    // Plot 1: Absolute N_bottom vs thickness
    TGraph *gBottom = new TGraph(Nfiles, thickness, nBottom);
    gBottom->SetTitle("Absolute Bottom Hits vs Thickness;Film Thickness (nm);N_{bottom} (absolute)");
    gBottom->SetLineWidth(3);
    gBottom->SetMarkerStyle(20);
    gBottom->SetMarkerSize(1.8);

    // Plot 2: All three surfaces on same canvas for comparison
    TGraph *gInner = new TGraph(Nfiles, thickness, nInner);
    gInner->SetLineWidth(3);
    gInner->SetMarkerStyle(21);
    gInner->SetMarkerSize(1.8);
    gInner->SetLineColor(kRed);
    gInner->SetMarkerColor(kRed);

    TGraph *gOuter = new TGraph(Nfiles, thickness, nOuter);
    gOuter->SetLineWidth(3);
    gOuter->SetMarkerStyle(22);
    gOuter->SetMarkerSize(1.8);
    gOuter->SetLineColor(kBlue);
    gOuter->SetMarkerColor(kBlue);

    TCanvas *c1 = new TCanvas("c1", "Absolute Bottom Hits", 1400, 1000);
    gBottom->Draw("APL");
    c1->SaveAs("AbsoluteBottomHits_vs_Thickness.png");

    TCanvas *c2 = new TCanvas("c2", "All Surface Hits vs Thickness", 1400, 1000);
    // Draw all three on same axes
    gBottom->SetTitle("Phonon Hits per Surface vs Bottom Film Thickness;Film Thickness (nm);N_{hits}");
    gBottom->Draw("APL");
    gInner->Draw("PL same");
    gOuter->Draw("PL same");

    TLegend *leg = new TLegend(0.65, 0.70, 0.88, 0.88);
    leg->AddEntry(gBottom, "Bottom (varying)", "lp");
    leg->AddEntry(gInner,  "Inner Top (fixed 600nm)", "lp");
    leg->AddEntry(gOuter,  "Outer Top (fixed 600nm)", "lp");
    leg->Draw();

    c2->SaveAs("AllSurfaceHits_vs_Thickness.png");
}
