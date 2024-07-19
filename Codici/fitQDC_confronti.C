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

void fitQDC_confronti() {

    // Prende i dati dal tree_1234
    TFile* file = TFile::Open("./treeOut.root");
    TTree* tree_1234 = (TTree*) file->Get("s1-2-3-4");
    TTree* tree_24 = (TTree*) file->Get("s2-4");
    TTree* tree_3 = (TTree*) file->Get("s3");
    
    // Definisce le variabili a cui accedere
    double q1, q2, q3, q4, m1, m2, m3, m4, n1, n2, n3, n4;
    tree_1234->SetBranchAddress("q1", &q1);
    tree_1234->SetBranchAddress("q2", &q2);
    tree_1234->SetBranchAddress("q3", &q3);
    tree_1234->SetBranchAddress("q4", &q4);

    tree_24->SetBranchAddress("q1", &m1);
    tree_24->SetBranchAddress("q2", &m2);
    tree_24->SetBranchAddress("q3", &m3);
    tree_24->SetBranchAddress("q4", &m4);

    tree_3->SetBranchAddress("q1", &n1);
    tree_3->SetBranchAddress("q2", &n2);
    tree_3->SetBranchAddress("q3", &n3);
    tree_3->SetBranchAddress("q4", &n4);

    // Crea gli istogrammi
    int nbins = 1100;
    double xmin = 0, xmax = 5500;
    TH1F *h_q1 = new TH1F("Segnale 1", "QDC Channel 0", nbins, xmin, xmax);
    TH1F *h_q2 = new TH1F("Segnale 2", "QDC Channel 1", nbins, xmin, xmax);
    TH1F *h_q3 = new TH1F("Segnale 3", "QDC Channel 2", nbins, xmin, xmax);
    TH1F *h_q4 = new TH1F("Segnale 4", "QDC Channel 3", nbins, xmin, xmax);

    TH1F *h_m1 = new TH1F("Segnale 1", "QDC Channel 0", nbins, xmin, xmax);
    TH1F *h_m2 = new TH1F("Segnale 2", "QDC Channel 1", nbins, xmin, xmax);
    TH1F *h_m3 = new TH1F("Segnale 3", "QDC Channel 2", nbins, xmin, xmax);
    TH1F *h_m4 = new TH1F("Segnale 4", "QDC Channel 3", nbins, xmin, xmax);

    TH1F *h_n1 = new TH1F("Segnale 1", "QDC Channel 0", nbins, xmin, xmax);
    TH1F *h_n2 = new TH1F("Segnale 2", "QDC Channel 1", nbins, xmin, xmax);
    TH1F *h_n3 = new TH1F("Segnale 3", "QDC Channel 2", nbins, xmin, xmax);
    TH1F *h_n4 = new TH1F("Segnale 4", "QDC Channel 3", nbins, xmin, xmax);


    Long64_t tot_entries1234 = tree_1234->GetEntries();
    Long64_t tot_entries24 = tree_24->GetEntries();
    Long64_t tot_entries3 = tree_3->GetEntries();

    for(Long64_t iEntry=0; iEntry < tot_entries1234; ++iEntry){
        tree_1234->GetEntry(iEntry);
        h_q1->Fill(q1);
        h_q2->Fill(q2);
        h_q3->Fill(q3);
        h_q4->Fill(q4);
    }

    

    for(Long64_t iEntry=0; iEntry < tot_entries24; ++iEntry){
        tree_24->GetEntry(iEntry);
        h_m1->Fill(m1);
        h_m2->Fill(m2);
        h_m3->Fill(m3);
        h_m4->Fill(m4);
    }

    

    for(Long64_t iEntry=0; iEntry < tot_entries3; ++iEntry){
        tree_3->GetEntry(iEntry);
        h_n1->Fill(n1);
        h_n2->Fill(n2);
        h_n3->Fill(n3);
        h_n4->Fill(n4);
    }

    double bin_width = (xmax - xmin) / nbins;
    h_q1->Scale(1.0 / (h_q1->GetEntries() * bin_width));
    h_q2->Scale(1.0 / (h_q2->GetEntries() * bin_width));
    h_q3->Scale(1.0 / (h_q3->GetEntries() * bin_width));
    h_q4->Scale(1.0 / (h_q4->GetEntries() * bin_width));

    h_m1->Scale(1.0 / (h_m1->GetEntries() * bin_width));
    h_m2->Scale(1.0 / (h_m2->GetEntries() * bin_width));
    h_m3->Scale(1.0 / (h_m3->GetEntries() * bin_width));
    h_m4->Scale(1.0 / (h_m4->GetEntries() * bin_width));

    h_n1->Scale(1.0 / (h_n1->GetEntries() * bin_width));
    h_n2->Scale(1.0 / (h_n2->GetEntries() * bin_width));
    h_n3->Scale(1.0 / (h_n3->GetEntries() * bin_width));
    h_n4->Scale(1.0 / (h_n4->GetEntries() * bin_width));


    // Set histogram colors and transparency
    h_q1->SetLineColor(kSpring);
    h_q1->SetLineWidth(1);

    h_q2->SetLineColor(kSpring);
    h_q2->SetLineWidth(1);

    h_q3->SetLineColor(kSpring);
    h_q3->SetLineWidth(1);

    h_q4->SetLineColor(kSpring);
    h_q4->SetLineWidth(1);
    //
    h_m1->SetLineColor(kPink);
    h_m1->SetLineWidth(1);

    h_m2->SetLineColor(kPink);
    h_m2->SetLineWidth(1);

    h_m3->SetLineColor(kPink);
    h_m3->SetLineWidth(1);

    h_m4->SetLineColor(kPink);
    h_m4->SetLineWidth(1);
    //
    h_n1->SetLineColor(kAzure);
    h_n1->SetLineWidth(1);

    h_n2->SetLineColor(kAzure);
    h_n2->SetLineWidth(1);

    h_n3->SetLineColor(kAzure);
    h_n3->SetLineWidth(1);

    h_n4->SetLineColor(kAzure);
    h_n4->SetLineWidth(1);

/////////////////////////////////////////////////////////////////////////////////////////////////////
    // CANVAS PER TUTTI I CANALI 
    TCanvas *c_all = new TCanvas("QDC, confronto tra segnali da acquisizioni con trigger diversi", "QDC, confronto tra segnali da acquisizioni con trigger diversi", 1500, 1800);
    c_all->Divide(2,2);
    c_all->cd(1)->SetGrid();
    c_all->cd(2)->SetGrid();
    c_all->cd(3)->SetGrid();
    c_all->cd(4)->SetGrid();

    c_all->cd(1);
    h_q1->SetTitle("Segnale 1;QDC;#frac{dN}{d QDC}");
    h_q1->Draw("HIST"); // Draw histogram lines on top
    h_m1->Draw("HIST same"); // Draw histogram lines on top
    h_n1->Draw("HIST same"); // Draw histogram lines on top
    
    // Crea la legenda
    TLegend *legend1 = new TLegend(0.7, 0.9, 0.9, 0.7);
    //legend1->SetHeader(""); // Remove the white box
    legend1->AddEntry(h_q1, "Trigger S1-2-3-4", "l");
    legend1->AddEntry(h_m1, "Trigger S2-4", "l");
    legend1->AddEntry(h_n1, "Trigger S3", "l");
    legend1->Draw();

    c_all->cd(2);
    h_q2->SetTitle("Segnale 2;QDC;#frac{dN}{d QDC}");
    h_q2->Draw("HIST"); // Draw histogram lines on top
    h_m2->Draw("HIST same"); // Draw histogram lines on top
    h_n2->Draw("HIST same"); // Draw histogram lines on top

    // Crea la legenda
    TLegend *legend2 = new TLegend(0.7, 0.9, 0.9, 0.7);
    //legend2->SetHeader(""); // Remove the white box
    legend2->AddEntry(h_q2, "Trigger S1-2-3-4", "l");
    legend2->AddEntry(h_m2, "Trigger S2-4", "l");
    legend2->AddEntry(h_n2, "Trigger S3", "l");
    legend2->Draw();

    c_all->cd(3);
    h_q3->SetTitle("Segnale 3;QDC;#frac{dN}{d QDC}");
    h_q3->Draw("HIST"); // Draw histogram lines on top
    h_m3->Draw("HIST same"); // Draw histogram lines on top
    h_n3->Draw("HIST same"); // Draw histogram lines on top

    // Crea la legenda
    TLegend *legend3 = new TLegend(0.7, 0.9, 0.9, 0.7);
    //legend3->SetHeader(""); // Remove the white box
    legend3->AddEntry(h_q3, "Trigger S1-2-3-4", "l");
    legend3->AddEntry(h_m3, "Trigger S2-4", "l");
    legend3->AddEntry(h_n3, "Trigger S3", "l");
    legend3->Draw();

    c_all->cd(4);
    h_q4->SetTitle("Segnale 4;QDC;#frac{dN}{d QDC}");
    h_q4->Draw("HIST"); // Draw histogram lines on top
    h_m4->Draw("HIST same"); // Draw histogram lines on top
    h_n4->Draw("HIST same"); // Draw histogram lines on top

    // Crea la legenda
    TLegend *legend4 = new TLegend(0.7, 0.9, 0.9, 0.7);
    //legend4->SetHeader(""); // Remove the white box
    legend4->AddEntry(h_q4, "Trigger S1-2-3-4", "l");
    legend4->AddEntry(h_m4, "Trigger S2-4", "l");
    legend4->AddEntry(h_n4, "Trigger S3", "l");
    legend4->Draw();
    
    h_q1->SetStats(false); 
    h_q2->SetStats(false); 
    h_q3->SetStats(false); 
    h_q4->SetStats(false); 

    h_m1->SetStats(false); 
    h_m2->SetStats(false); 
    h_m3->SetStats(false); 
    h_m4->SetStats(false); 

    h_n1->SetStats(false); 
    h_n2->SetStats(false); 
    h_n3->SetStats(false); 
    h_n4->SetStats(false); 


    // Fit degli istogrammi
    h_q1->GetListOfFunctions()->Clear();
    h_q2->GetListOfFunctions()->Clear();
    h_q3->GetListOfFunctions()->Clear();
    h_q4->GetListOfFunctions()->Clear();

    h_m1->GetListOfFunctions()->Clear();
    h_m2->GetListOfFunctions()->Clear();
    h_m3->GetListOfFunctions()->Clear();
    h_m4->GetListOfFunctions()->Clear();

    h_n1->GetListOfFunctions()->Clear();
    h_n2->GetListOfFunctions()->Clear();
    h_n3->GetListOfFunctions()->Clear();
    h_n4->GetListOfFunctions()->Clear();

    c_all->Update();
    c_all->Write();
    
    return;
}
