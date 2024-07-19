#include "Riostream.h"
#include <string>
#include "TTree.h"
#include "TFile.h"
#include "TH1F.h"
#include "TF1.h" 
#include "TCanvas.h"
#include "TStyle.h"
#include "TMath.h"

using namespace std;

void fitQDC_3() {

    // Prende i dati dal tree
    TFile* file = TFile::Open("./TreeOut.root");
    TTree* tree = (TTree*) file->Get("s3");
    
    // Definisce le variabili a cui accedere
    double q1, q2, q3, q4;
    tree->SetBranchAddress("q1", &q1);
    tree->SetBranchAddress("q2", &q2);
    tree->SetBranchAddress("q3", &q3);
    tree->SetBranchAddress("q4", &q4);

    // Crea gli istogrammi
    int nbins = 1100;
    double xmin = 0, xmax = 5500;
    TH1F *h_q1 = new TH1F("Segnale 1", "QDC Channel 0", nbins, xmin, xmax);
    TH1F *h_q2 = new TH1F("Segnale 2", "QDC Channel 1", nbins, xmin, xmax);
    TH1F *h_q3 = new TH1F("Segnale 3", "QDC Channel 2", nbins, xmin, xmax);
    TH1F *h_q4 = new TH1F("Segnale 4", "QDC Channel 3", nbins, xmin, xmax);

    Long64_t tot_entries = tree->GetEntries();

    for(Long64_t iEntry=0; iEntry < tot_entries; ++iEntry){
        tree->GetEntry(iEntry);
        h_q1->Fill(q1);
        h_q2->Fill(q2);
        h_q3->Fill(q3);
        h_q4->Fill(q4);
    }

    // Set histogram colors and transparency
    h_q1->SetLineColor(kRed+1.);
    h_q1->SetFillColorAlpha(kRed, 0.2);
    h_q1->SetLineWidth(2);

    h_q2->SetLineColor(kGreen+1.);
    h_q2->SetFillColorAlpha(kGreen, 0.2);
    h_q2->SetLineWidth(2);

    h_q3->SetLineColor(kCyan+1.);
    h_q3->SetFillColorAlpha(kCyan, 0.2);
    h_q3->SetLineWidth(2);

    h_q4->SetLineColor(kViolet+1.);
    h_q4->SetFillColorAlpha(kViolet, 0.2);
    h_q4->SetLineWidth(2);

/////////////////////////////////////////////////////////////////////////////////////////////////////
    // CANVAS PER TUTTI I CANALI 
    TCanvas *c_all = new TCanvas("QDC con trigger S3", "QDC con trigger S3", 1500, 1800);
    c_all->SetGrid();
    h_q2->SetTitle("QDC con trigger S3;QDC;#frac{dN}{d QDC}");

    h_q2->Draw("E");    // Draw error bars and histogram
    h_q2->Draw("HIST same"); // Draw histogram lines on top

    h_q1->Draw("E same");    // Draw error bars and histogram
    h_q1->Draw("HIST same"); // Draw histogram lines on top

    h_q4->Draw("E same");    // Draw error bars and histogram
    h_q4->Draw("HIST same"); // Draw histogram lines on top

    h_q3->Draw("E same");    // Draw error bars and histogram
    h_q3->Draw("HIST same"); // Draw histogram lines on top

    // Crea la legenda
    TLegend *legend = new TLegend(0.7, 0.9, 0.9, 0.7);
    legend->SetHeader(""); // Remove the white box
    legend->AddEntry(h_q1, "Segnale 1", "l");
    legend->AddEntry(h_q2, "Segnale 2", "l");
    legend->AddEntry(h_q3, "Segnale 3", "l");
    legend->AddEntry(h_q4, "Segnale 4", "l");
    legend->Draw();
    
    h_q1->SetStats(false); 
    h_q2->SetStats(false); 
    h_q3->SetStats(false); 
    h_q4->SetStats(false); 

    // Fit degli istogrammi
    h_q1->GetListOfFunctions()->Clear();
    h_q2->GetListOfFunctions()->Clear();
    h_q3->GetListOfFunctions()->Clear();
    h_q4->GetListOfFunctions()->Clear();
    c_all->Update();
    c_all->Write();
    
    return;
}
