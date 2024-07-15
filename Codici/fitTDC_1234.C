#include "Riostream.h"
#include <string>
#include "TTree.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TF1.h" 
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"

using namespace std;

void fitTDC_1234() {

    // Prende i dati dal tree
    TFile* file = TFile::Open("./TreeOut.root");
    TTree* tree = (TTree*) file->Get("s1-2-3-4");
    
    // Definisce le variabili a cui accedere
    double t1, t2, t3, t4;
    tree->SetBranchAddress("t1", &t1);
    tree->SetBranchAddress("t2", &t2);
    tree->SetBranchAddress("t3", &t3);
    tree->SetBranchAddress("t4", &t4);

    // Crea gli istogrammi
    int nbins = 1500;
    double xmin = 400, xmax = 1500;
    TH1F *h_t1 = new TH1F("t1", "TDC Channel 0", nbins, xmin, xmax);
    TH1F *h_t2 = new TH1F("t2", "TDC Channel 1", nbins, xmin, xmax);
    TH1F *h_t3 = new TH1F("t3", "TDC Channel 2", nbins, xmin, xmax);
    TH1F *h_t4 = new TH1F("t4", "TDC Channel 3", nbins, xmin, xmax);

    Long64_t tot_entries = tree->GetEntries();

    for(Long64_t iEntry=0; iEntry < tot_entries; ++iEntry){
        tree->GetEntry(iEntry);
        h_t1->Fill(t1);
        h_t2->Fill(t2);
        h_t3->Fill(t3);
        h_t4->Fill(t4);
    }

    // Normalizza gli istogrammi
    double bin_width = (xmax - xmin) / nbins;
    h_t1->Scale(1.0 / (h_t1->GetEntries() * bin_width));
    h_t2->Scale(1.0 / (h_t2->GetEntries() * bin_width));
    h_t3->Scale(1.0 / (h_t3->GetEntries() * bin_width));
    h_t4->Scale(1.0 / (h_t4->GetEntries() * bin_width));

    // Opzioni di stile
    gStyle->SetOptFit(1111);
    gStyle->SetOptStat(1111);

    // Definisce Gauss per fit
    TF1 *gauss_t1 = new TF1("gauss_t1", "gaus", xmin, xmax);
    TF1 *gauss_t2 = new TF1("gauss_t2", "gaus", xmin, xmax);
    TF1 *gauss_t3 = new TF1("gauss_t3", "gaus", xmin, xmax);
    TF1 *gauss_t4 = new TF1("gauss_t4", "gaus", xmin, xmax);

    // Imposta il colore della linea del fit a rosso
    gauss_t1->SetLineColor(kRed);
    gauss_t2->SetLineColor(kRed);
    gauss_t3->SetLineColor(kRed);
    gauss_t4->SetLineColor(kRed);

    // Parametri iniziali
    double cost1 = 0.5;
    double MPV = 900.0;
    double sigma_gauss = 200.0;

    gauss_t1->SetParameters(cost1, MPV, sigma_gauss);
    gauss_t2->SetParameters(cost1, MPV, sigma_gauss);
    gauss_t3->SetParameters(cost1, MPV, sigma_gauss);
    gauss_t4->SetParameters(cost1, MPV, sigma_gauss);

    // Fit degli istogrammi
    h_t1->Fit("gauss_t1");
    h_t2->Fit("gauss_t2");
    h_t3->Fit("gauss_t3");
    h_t4->Fit("gauss_t4");

    // Stampa i risultati del fit per il controllo
    gauss_t1->Print();
    gauss_t2->Print();
    gauss_t3->Print();
    gauss_t4->Print();

    // Disegna istogrammi + fit
    TCanvas *c1_gauss = new TCanvas("c1_gauss", "TDC Channel 0 Gauss", 800, 600);
    h_t1->Draw();
    gauss_t1->Draw("same");
    c1_gauss->Update();
    c1_gauss->SaveAs("../Dati/TDC/TDC_1234/TDC_Channel0_Gauss.png");

    TCanvas *c2_gauss = new TCanvas("c2_gauss", "TDC Channel 1 Gauss", 800, 600);
    h_t2->Draw();
    gauss_t2->Draw("same");
    c2_gauss->Update();
    c2_gauss->SaveAs("../Dati/TDC/TDC_1234/TDC_Channel1_Gauss.png");

    TCanvas *c3_gauss = new TCanvas("c3_gauss", "TDC Channel 2 Gauss", 800, 600);
    h_t3->Draw();
    gauss_t3->Draw("same");
    c3_gauss->Update();
    c3_gauss->SaveAs("../Dati/TDC/TDC_1234/TDC_Channel2_Gauss.png");

    TCanvas *c4_gauss = new TCanvas("c4_gauss", "TDC Channel 3 Gauss", 800, 600);
    h_t4->Draw();
    gauss_t4->Draw("same");
    c4_gauss->Update();
    c4_gauss->SaveAs("../Dati/TDC/TDC_1234/TDC_Channel3_Gauss.png");

    // Salva i risultati in un file ROOT
    TFile outFile("../Dati/TDC/TDC_1234/DataTDC_1234.root", "RECREATE");

    c1_gauss->Write();
    c2_gauss->Write();
    c3_gauss->Write();
    c4_gauss->Write();

    outFile.Close();

    return;
}
