#include "TFile.h"
#include "TTree.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TStyle.h"
#include <vector>
#include <string>
#include <iostream>

void phononAbsCount() {
    std::vector<std::string> files = {"Be7_G4RadioDecay_0V_1nm.root","Be7_G4RadioDecay_0V_2nm.root","Be7_G4RadioDecay_0V_5nm.root","Be7_G4RadioDecay_0V_10nm.root","Be7_G4RadioDecay_0V_30nm.root","Be7_G4RadioDecay_0V_50nm.root"};
    std::vector<double> thickness = {1,2,5,10,30,50};
    std::vector<double> absorptionFractions;
    std::vector<int> colors = {kBlue, kRed, kGreen+2, kOrange+1, kMagenta, kCyan};

    for(size_t i=0;i<files.size();i++){
        TFile *f=TFile::Open(files[i].c_str());
        TTree *tB=(TTree*)f->Get("BottomHits"); TTree *tI=(TTree*)f->Get("InnerHits"); TTree *tO=(TTree*)f->Get("OuterHits");
        Long64_t nB=tB->GetEntries(); Long64_t nI=tI->GetEntries(); Long64_t nO=tO->GetEntries();
        absorptionFractions.push_back(double(nB)/double(nB+nI+nO));
        f->Close();
    }

    TCanvas *c=new TCanvas("c","Bottom Phonon Absorption",1200,800); gStyle->SetGridStyle(3); gStyle->SetGridColor(kGray+2); c->SetGrid();

    TGraph *g=new TGraph(thickness.size(),&thickness[0],&absorptionFractions[0]);
    g->SetTitle("Bottom Film Phonon Absorption;Bottom Al Film Thickness [nm];Phonon Absorption Fraction (N bottom / N total)");
    g->SetLineWidth(3); g->SetMarkerStyle(20); g->SetMarkerSize(1.5); g->SetMarkerColor(kBlack); g->SetLineColor(kBlack);
    g->Draw("ALP");

    for(size_t i=0;i<absorptionFractions.size();i++) std::cout<<"Film "<<thickness[i]<<"nm: "<<absorptionFractions[i]<<std::endl;

    c->SaveAs("BottomPhononAbsorption_Count_HQ.png");
}

#include "TFile.h"
#include "TTree.h"
#include <vector>
#include <string>
#include <iostream>

void print_nB_30nm() {
    std::vector<std::string> files = {"Be7_G4RadioDecay_0V_1nm.root",
                                      "Be7_G4RadioDecay_0V_2nm.root",
                                      "Be7_G4RadioDecay_0V_5nm.root",
                                      "Be7_G4RadioDecay_0V_10nm.root",
                                      "Be7_G4RadioDecay_0V_30nm.root",
                                      "Be7_G4RadioDecay_0V_50nm.root"};
    std::vector<double> thickness = {1,2,5,10,30,50};

    for(size_t i=0;i<files.size();i++){
        if(thickness[i]==30){
            TFile *f = TFile::Open(files[i].c_str());
            TTree *tB = (TTree*)f->Get("BottomHits");
            Long64_t nB = tB->GetEntries();
            std::cout << "nB for 30 nm film: " << nB << std::endl;
            f->Close();
            break; // no need to loop further
        }
    }
}

#include "TFile.h"
#include "TTree.h"
#include <vector>
#include <string>
#include <iostream>

void print_hits_30nm() {
    std::vector<std::string> files = {"Be7_G4RadioDecay_0V_1nm.root",
                                      "Be7_G4RadioDecay_0V_2nm.root",
                                      "Be7_G4RadioDecay_0V_5nm.root",
                                      "Be7_G4RadioDecay_0V_10nm.root",
                                      "Be7_G4RadioDecay_0V_30nm.root",
                                      "Be7_G4RadioDecay_0V_50nm.root"};
    std::vector<double> thickness = {1,2,5,10,30,50};

    for(size_t i=0;i<files.size();i++){
        if(thickness[i]==30){
            TFile *f = TFile::Open(files[i].c_str());
            TTree *tB = (TTree*)f->Get("BottomHits");
            TTree *tI = (TTree*)f->Get("InnerHits");
            TTree *tO = (TTree*)f->Get("OuterHits");

            Long64_t nB = tB->GetEntries();
            Long64_t nI = tI->GetEntries();
            Long64_t nO = tO->GetEntries();
            Long64_t nTotal = nB + nI + nO;
            double absorptionFraction = double(nB)/double(nTotal);

            std::cout << "30 nm film hits:" << std::endl;
            std::cout << "Bottom: " << nB << std::endl;
            std::cout << "Inner:  " << nI << std::endl;
            std::cout << "Outer:  " << nO << std::endl;
            std::cout << "Total:  " << nTotal << std::endl;
            std::cout << "Absorption fraction (Bottom / Total): " << absorptionFraction << std::endl;

            f->Close();
            break; // done
        }
    }
}
