#include <iostream>
#include <cmath>
#include "TGraph.h"
#include "TCanvas.h"
#include "TAxis.h"

void printKaplanProb() {
    // Al film parameters (matching AttachGrid)
    const double vSound         = 3.26e12;  // nm/s  (3.26 km/s = 3.26e3 m/s = 3.26e12 nm/s)
    const double phononLifetime = 242e-12;  // s     (242 ps)
    const double filmAbsorption = 0.20;

    // MFP = vSound * phononLifetime at reference energy 2*Delta
    double MFP_ref = vSound * phononLifetime;  // nm

    printf("\nAl parameters:\n");
    printf("  vSound         = %.2e nm/s\n", vSound);
    printf("  phononLifetime = %.0f ps\n", phononLifetime*1e12);
    printf("  MFP @ 2*Delta  = %.2f nm\n", MFP_ref);
    printf("  filmAbsorption = %.2f\n", filmAbsorption);
    printf("  frac (path/thickness in AbsorbPhonon) = 4.0\n\n");

    printf("%-15s  %-18s  %-18s  %-15s\n",
           "Thickness(nm)", "P_kaplan(raw)", "P_total(x0.20)", "MFP/d ratio");
    printf("------------------------------------------------------------------------\n");

    double thicknesses[] = {5, 10, 20, 30, 40, 50, 80, 100, 150, 300, 600};
    int N = sizeof(thicknesses)/sizeof(thicknesses[0]);

    double P_totals[11];

    for (int i = 0; i < N; i++) {
        double d = thicknesses[i];  // nm
        double P_kaplan = 1.0 - std::exp(-4.0 * d / MFP_ref);  // frac=4.0 from HVeVKaplanQP.cc
        double P_total  = filmAbsorption * P_kaplan;
        double ratio    = MFP_ref / d;
        P_totals[i] = P_total;
        printf("%-15.0f  %-18.6f  %-18.6f  %-15.2f\n",
               d, P_kaplan, P_total, ratio);
    }
    printf("\n");

    // Plot P_total vs thickness
    TGraph *g = new TGraph(N, thicknesses, P_totals);
    g->SetTitle("KaplanQP Expected Absorption Probability;Film Thickness (nm);P_{total} = filmAbsorption #times (1 - e^{-4d/MFP})");
    g->SetLineWidth(3);
    g->SetMarkerStyle(20);
    g->SetMarkerSize(1.5);
    g->SetLineColor(kBlue+1);
    g->SetMarkerColor(kBlue+1);

    TCanvas *c = new TCanvas("c", "KaplanQP Absorption Probability", 1400, 1000);
    g->Draw("APL");
    c->SaveAs("KaplanQP_ExpectedProbability.png");
    printf("Plot saved to KaplanQP_ExpectedProbability.png\n");
}
