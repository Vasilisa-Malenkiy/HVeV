// EnergyOverlay.C
// Run with: root -l EnergyOverlay.C

// EnergyAndPhononScan.C
// Run with: root -l EnergyAndPhononScan.C

#include <map>
#include <vector>
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TGraph.h"

void EnergyAndPhononScan()
{
  const int N = 6;

  const char* files[N] = {
    "5nm_0Vbfilm.root",
    "10nm_0Vbfilm.root",
    "15nm_0Vbfilm.root",
    "20nm_0Vbfilm.root",
    "25nm_0Vbfilm.root",
    "30nm_0Vbfilm.root"
  };

  double thickness_nm[N] = {5,10,15,20,25,30};

  int colors[N] = {
    kBlack, kRed, kBlue, kGreen+2, kMagenta, kOrange+7
  };

  // Output file
  TFile *fout = new TFile("EnergyAndPhononResults.root","RECREATE");

  // =======================
  // ENERGY OVERLAY CANVAS
  // =======================
  TCanvas *cE = new TCanvas(
    "c_TotalEnergy",
    "Total Energy Deposition",
    900,650
  );

  TLegend *leg = new TLegend(0.12,0.65,0.32,0.88);

  // =======================
  // PHONON FRACTION STORAGE
  // =======================
  std::vector<double> tvec;
  std::vector<double> fracvec;

  for (int i = 0; i < N; i++) {

    TFile *f = TFile::Open(files[i]);
    if (!f || f->IsZombie()) continue;

    // ---------- ENERGY ----------
    TTree *tB = (TTree*)f->Get("BottomHits");
    TTree *tI = (TTree*)f->Get("InnerHits");
    TTree *tO = (TTree*)f->Get("OuterHits");

    Int_t eventid;
    Double_t edep;
    std::map<int,double> E;

    auto fillEnergy = [&](TTree *t){
      if (!t) return;
      t->SetBranchAddress("eventid",&eventid);
      t->SetBranchAddress("Edep",&edep);
      for (Long64_t j=0;j<t->GetEntries();j++){
        t->GetEntry(j);
        E[eventid] += edep;
      }
    };

    fillEnergy(tB);
    fillEnergy(tI);
    fillEnergy(tO);

    TH1D *h = new TH1D(
      Form("h_%dnm",(int)thickness_nm[i]),
      "Total Energy;Total E_{dep};Events",
      100,0,130
    );

    for (auto &kv : E)
      h->Fill(kv.second);

    h->SetLineColor(colors[i]);
    h->SetLineWidth(2);

    if (i==0) {
      h->SetMaximum(37);
      h->Draw();
    } else {
      h->Draw("SAME");
    }

    leg->AddEntry(h,Form("%.0f nm",thickness_nm[i]),"l");

    fout->cd();
    h->Write();

    // ---------- PHONONS ----------
    TTree *pB = (TTree*)f->Get("BottomPhonons");
    TTree *pI = (TTree*)f->Get("InnerPhonons");
    TTree *pO = (TTree*)f->Get("OuterPhonons");

    double Nbottom = 0;
    double Ntotal  = 0;

    if (pB) Nbottom += pB->GetEntries();
    if (pI) Ntotal  += pI->GetEntries();
    if (pO) Ntotal  += pO->GetEntries();
    Ntotal += Nbottom;

    double frac = (Ntotal > 0) ? Nbottom / Ntotal : 0.0;

    tvec.push_back(thickness_nm[i]);
    fracvec.push_back(frac);
  }

  leg->Draw();
  fout->cd();
  cE->Write();

  // =======================
  // PHONON FRACTION GRAPH
  // =======================
  TCanvas *cP = new TCanvas(
    "c_PhononFraction",
    "Bottom Film Phonon Absorption",
    800,600
  );

  TGraph *g = new TGraph(
    tvec.size(),
    tvec.data(),
    fracvec.data()
  );

  g->SetTitle(
    "Bottom Film Phonon Absorption;"
    "Bottom Film Thickness (nm);"
    "N_{phonon}^{bottom} / N_{phonon}^{total}"
  );

  g->SetMarkerStyle(20);
  g->SetMarkerSize(1.2);
  g->SetLineWidth(2);

  g->Draw("APL");

  fout->cd();
  g->Write();
  cP->Write();

  fout->Close();
}
