void TestPlot()
{
    TFile *f = TFile::Open("30nm_BePos100nm.root");
    TTree *t = (TTree*)f->Get("EnergyDeposit");

    TCanvas *c = new TCanvas("c","test",800,600);

    t->Draw("Edep");

    c->Update();
}
