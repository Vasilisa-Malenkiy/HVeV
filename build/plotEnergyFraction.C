#include <map>
#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TLegend.h"

void plotEnergyFraction() {

    const int Nfiles = 3;
    double thickness[Nfiles] = {10, 30, 50};

    const char* filenames[Nfiles] = {
        "10nmALBFT.root",
        "30nmALBFT.root",
        "50nmALBFT.root"
    };

    double fracBottom[Nfiles];   // E_bottom / E_generated
    double fracInner[Nfiles];    // E_inner  / E_generated
    double fracOuter[Nfiles];    // E_outer  / E_generated

    for (int i = 0; i < Nfiles; i++) {
        TFile *f = TFile::Open(filenames[i]);
        if (!f || f->IsZombie()) {
            std::cout << "Warning: Could not open " << filenames[i] << "\n";
            fracBottom[i] = fracInner[i] = fracOuter[i] = 0;
            continue;
        }

        TTree *tBot   = (TTree*)f->Get("BottomHits");
        TTree *tInner = (TTree*)f->Get("InnerHits");
        TTree *tOuter = (TTree*)f->Get("OuterHits");
        TTree *tPrim  = (TTree*)f->Get("PrimaryPhonon");
        TTree *tLuke  = (TTree*)f->Get("LukePhonon");

        // --- Sum energy deposited in each surface ---
        Double_t Edep = 0.;
        double E_bottom = 0., E_inner = 0., E_outer = 0.;

        if (tBot) {
            tBot->SetBranchAddress("Edep", &Edep);
            for (int j = 0; j < tBot->GetEntries(); j++) {
                tBot->GetEntry(j);
                E_bottom += Edep;
            }
        }
        if (tInner) {
            tInner->SetBranchAddress("Edep", &Edep);
            for (int j = 0; j < tInner->GetEntries(); j++) {
                tInner->GetEntry(j);
                E_inner += Edep;
            }
        }
        if (tOuter) {
            tOuter->SetBranchAddress("Edep", &Edep);
            for (int j = 0; j < tOuter->GetEntries(); j++) {
                tOuter->GetEntry(j);
                E_outer += Edep;
            }
        }

        // --- Sum total phonon energy generated (primary + Luke) ---
        Double_t energy = 0.;
        double E_generated = 0.;

        if (tPrim) {
            tPrim->SetBranchAddress("energy", &energy);
            for (int j = 0; j < tPrim->GetEntries(); j++) {
                tPrim->GetEntry(j);
                E_generated += energy;
            }
        }
        if (tLuke) {
            tLuke->SetBranchAddress("energy", &energy);
            for (int j = 0; j < tLuke->GetEntries(); j++) {
                tLuke->GetEntry(j);
                E_generated += energy;
            }
        }

        fracBottom[i] = (E_generated > 0) ? E_bottom / E_generated : 0;
        fracInner[i]  = (E_generated > 0) ? E_inner  / E_generated : 0;
        fracOuter[i]  = (E_generated > 0) ? E_outer  / E_generated : 0;

        std::cout << filenames[i] << ":\n";
        std::cout << "  E_generated = " << E_generated << " eV\n";
        std::cout << "  E_bottom    = " << E_bottom    << " eV\n";
        std::cout << "  E_inner     = " << E_inner     << " eV\n";
        std::cout << "  E_outer     = " << E_outer     << " eV\n";
        std::cout << "  frac_bottom = " << fracBottom[i] << "\n";
        std::cout << "  frac_inner  = " << fracInner[i]  << "\n";
        std::cout << "  frac_outer  = " << fracOuter[i]  << "\n\n";

        f->Close();
    }

    // --- Plot energy fractions vs filmThickness ---
    TGraph *gBot   = new TGraph(Nfiles, thickness, fracBottom);
    TGraph *gInner = new TGraph(Nfiles, thickness, fracInner);
    TGraph *gOuter = new TGraph(Nfiles, thickness, fracOuter);

    gBot->SetLineWidth(3); gBot->SetMarkerStyle(20); gBot->SetMarkerSize(1.5);
    gBot->SetLineColor(kBlack); gBot->SetMarkerColor(kBlack);

    gInner->SetLineWidth(3); gInner->SetMarkerStyle(21); gInner->SetMarkerSize(1.5);
    gInner->SetLineColor(kRed); gInner->SetMarkerColor(kRed);

    gOuter->SetLineWidth(3); gOuter->SetMarkerStyle(22); gOuter->SetMarkerSize(1.5);
    gOuter->SetLineColor(kBlue); gOuter->SetMarkerColor(kBlue);

    gBot->SetTitle("Energy Fraction per Surface vs Bottom Film KaplanQP Thickness;KaplanQP filmThickness (nm);E_{surface} / E_{generated}");

    TCanvas *c = new TCanvas("c", "Energy Fraction", 1400, 1000);
    gBot->Draw("APL");
    gInner->Draw("PL same");
    gOuter->Draw("PL same");

    TLegend *leg = new TLegend(0.60, 0.70, 0.88, 0.88);
    leg->AddEntry(gBot,   "Bottom (KaplanQP varying)", "lp");
    leg->AddEntry(gInner, "Inner Top (fixed 600nm)",   "lp");
    leg->AddEntry(gOuter, "Outer Top (fixed 600nm)",   "lp");
    leg->Draw();

    c->SaveAs("EnergyFraction_vs_FilmThickness.png");
    std::cout << "Plot saved to EnergyFraction_vs_FilmThickness.png\n";
}
