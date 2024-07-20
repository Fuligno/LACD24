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

void fitTDC_confronti() {

    // Prende i dati dal tree_1234
    TFile* file = TFile::Open("./treeOut.root");
    TTree* tree_1234 = (TTree*) file->Get("s1-2-3-4");
    TTree* tree_24 = (TTree*) file->Get("s2-4");
    TTree* tree_3 = (TTree*) file->Get("s3");
    
    // Definisce le variabili a cui accedere
    double q1, q2, q3, q4, m1, m2, m3, m4, n1, n2, n3, n4;
    tree_1234->SetBranchAddress("t4", &q4);
    tree_24->SetBranchAddress("t4", &m4);
    tree_3->SetBranchAddress("t4", &n4);

    // Crea gli istogrammi
    int nbins = 220;
    double xmin = 400, xmax = 1500;
    TH1F *h_q4 = new TH1F("Segnale 4", "QDC Channel 3", nbins, xmin, xmax);
    TH1F *h_m4 = new TH1F("Segnale 4", "QDC Channel 3", nbins, xmin, xmax);
    TH1F *h_n4 = new TH1F("Segnale 4", "QDC Channel 3", nbins, xmin, xmax);


    Long64_t tot_entries1234 = tree_1234->GetEntries();
    Long64_t tot_entries24 = tree_24->GetEntries();
    Long64_t tot_entries3 = tree_3->GetEntries();

    for(Long64_t iEntry=0; iEntry < tot_entries1234; ++iEntry){
        tree_1234->GetEntry(iEntry);
        h_q4->Fill(q4);
    }

    for(Long64_t iEntry=0; iEntry < tot_entries24; ++iEntry){
        tree_24->GetEntry(iEntry);
        h_m4->Fill(m4);
    }

    for(Long64_t iEntry=0; iEntry < tot_entries3; ++iEntry){
        tree_3->GetEntry(iEntry);
        h_n4->Fill(n4);
    }

    double bin_width = (xmax - xmin) / nbins;
    h_q4->Scale(1.0 / (h_q4->GetEntries() * bin_width));
    h_m4->Scale(1.0 / (h_m4->GetEntries() * bin_width));
    h_n4->Scale(1.0 / (h_n4->GetEntries() * bin_width));


    // Set histogram colors and transparency
    h_q4->SetLineColor(kSpring);
    h_q4->SetFillColorAlpha(kSpring+1., 0.2);
    h_q4->SetLineWidth(2);
    //
    h_m4->SetLineColor(kOrange);
    h_m4->SetFillColorAlpha(kOrange+1., 0.2);
    h_m4->SetLineWidth(2);
    //
    h_n4->SetLineColor(kAzure);
    h_n4->SetFillColorAlpha(kAzure+1., 0.2);
    h_n4->SetLineWidth(2);

/////////////////////////////////////////////////////////////////////////////////////////////////////
    // CANVAS PER TUTTI I CANALI 
    TCanvas *c_all = new TCanvas("TDC, confronto tra segnali da acquisizioni con trigger diversi", "QDC, confronto tra segnali da acquisizioni con trigger diversi", 1500, 1800);
    c_all->SetGrid();
    h_n4->SetTitle("Segnale 4;TDC;#frac{dN}{d TDC}");
    h_n4->Draw("HIST"); // Draw histogram lines on top
    h_m4->Draw("HIST same"); // Draw histogram lines on top
    h_q4->Draw("HIST same"); // Draw histogram lines on top

    // Crea la legenda
    TLegend *legend4 = new TLegend(0.7, 0.9, 0.9, 0.7);
    //legend4->SetHeader(""); // Remove the white box
    legend4->AddEntry(h_q4, "Trigger S1-2-3-4", "l");
    legend4->AddEntry(h_m4, "Trigger S2-4", "l");
    legend4->AddEntry(h_n4, "Trigger S3", "l");
    legend4->Draw();
    
    h_q4->SetStats(false); 
    h_m4->SetStats(false); 
    h_n4->SetStats(false); 

    // Fit degli istogrammi
    h_q4->GetListOfFunctions()->Clear();
    h_m4->GetListOfFunctions()->Clear();
    h_n4->GetListOfFunctions()->Clear();

    c_all->Update();
    c_all->Write();
    
    return;
}
