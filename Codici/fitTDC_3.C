#include "Riostream.h"
#include <string>
#include "TTree.h"
#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TMath.h"
#include "TLegend.h"

using namespace std;

void fitTDC_3() {
    // Prende i dati dal tree
    TFile* file = TFile::Open("./TreeOut.root");
    TTree* tree = (TTree*) file->Get("s3");

    // Definisce le variabili a cui accedere
    double t1, t2, t3, t4;
    tree->SetBranchAddress("t1", &t1);
    tree->SetBranchAddress("t2", &t2);
    tree->SetBranchAddress("t3", &t3);
    tree->SetBranchAddress("t4", &t4);

    // Crea gli istogrammi
    int nbins = 220;
    double xmin = 400, xmax = 1500;
    TH1F *h_t1 = new TH1F("t1", "Segnale 1", nbins, xmin, xmax);
    TH1F *h_t2 = new TH1F("t2", "Segnale 2", nbins, xmin, xmax);
    TH1F *h_t3 = new TH1F("t3", "Segnale 3", nbins, xmin, xmax);
    TH1F *h_t4 = new TH1F("t4", "Segnale 4", nbins, xmin, xmax);

    Long64_t tot_entries = tree->GetEntries();

    for(Long64_t iEntry = 0; iEntry < tot_entries; ++iEntry) {
        tree->GetEntry(iEntry);
        h_t1->Fill(t1);
        h_t2->Fill(t2);
        h_t3->Fill(t3);
        h_t4->Fill(t4);
    }

    // Opzioni di stile
    gStyle->SetOptStat(false); // Rimuove il box statistico
    gStyle->SetOptFit(false);

    // Set histogram colors and transparency
    h_t1->SetLineColor(kRed+2.);
    h_t1->SetFillColorAlpha(kRed+1., 0.2);
    h_t1->SetLineWidth(1);

    h_t2->SetLineColor(kGreen+2.);
    h_t2->SetFillColorAlpha(kGreen+1., 0.2);
    h_t2->SetLineWidth(1);

    h_t3->SetLineColor(kCyan+2.);
    h_t3->SetFillColorAlpha(kCyan+1., 0.2);
    h_t3->SetLineWidth(1);

    h_t4->SetLineColor(kViolet+2.);
    h_t4->SetFillColorAlpha(kViolet+1., 0.2);
    h_t4->SetLineWidth(1);

    // CANVAS PER TUTTI I CANALI 
    TCanvas *c_all = new TCanvas("c_all", "TDC trigger su S3", 1500, 1800);
    c_all->SetGrid();
    h_t4->SetTitle("TDC trigger su S3;TDC;#frac{dN}{d TDC}");

    h_t4->Draw("E");    // Draw error bars and histogram
    h_t4->Draw("HIST same"); // Draw histogram lines on top

    h_t1->Draw("E same");    // Draw error bars and histogram
    h_t1->Draw("HIST same"); // Draw histogram lines on top

    h_t2->Draw("E same");    // Draw error bars and histogram
    h_t2->Draw("HIST same"); // Draw histogram lines on top

    h_t3->Draw("E same");    // Draw error bars and histogram
    h_t3->Draw("HIST same"); // Draw histogram lines on top

    // Crea la legenda
    TLegend *legend = new TLegend(0.1, 0.9, 0.3, 0.7);
    legend->SetHeader(""); // Remove the white box
    legend->AddEntry(h_t1, "Segnale 1", "l");
    legend->AddEntry(h_t2, "Segnale 2", "l");
    legend->AddEntry(h_t3, "Segnale 3", "l");
    legend->AddEntry(h_t4, "Segnale 4", "l");
    legend->Draw();

    c_all->Update();

    // Salva i risultati in un file ROOT
    TFile outFile("../Dati/TDC/TDC_3/DataTDC_3.root", "RECREATE");

    c_all->Write();

    outFile.Close();

    return;
}
