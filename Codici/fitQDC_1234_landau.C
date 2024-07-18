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

void fitQDC_1234_landau() {

    // Prende i dati dal tree
    TFile* file = TFile::Open("./TreeOut.root");
    TTree* tree = (TTree*) file->Get("s1-2-3-4");
    
    // Definisce le variabili a cui accedere
    double q1, q2, q3, q4;
    tree->SetBranchAddress("q1", &q1);
    tree->SetBranchAddress("q2", &q2);
    tree->SetBranchAddress("q3", &q3);
    tree->SetBranchAddress("q4", &q4);

    // Crea gli istogrammi
    int nbins = 1100;
    double xmin = 0, xmax = 5500;
    TH1F *h_q1 = new TH1F("q1", "QDC Channel 0", nbins, xmin, xmax);
    TH1F *h_q2 = new TH1F("q2", "QDC Channel 1", nbins, xmin, xmax);
    TH1F *h_q3 = new TH1F("q3", "QDC Channel 2", nbins, xmin, xmax);
    TH1F *h_q4 = new TH1F("q4", "QDC Channel 3", nbins, xmin, xmax);

    Long64_t tot_entries = tree->GetEntries();

    for(Long64_t iEntry=0; iEntry < tot_entries; ++iEntry){
        tree->GetEntry(iEntry);
        h_q1->Fill(q1);
        h_q2->Fill(q2);
        h_q3->Fill(q3);
        h_q4->Fill(q4);
    }

    // Opzioni di stile
    gStyle->SetOptFit(1111);
    gStyle->SetOptStat(1111);

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
    // ISTOGRAMMI TUTTI INSIEME
    TCanvas *c_all = new TCanvas("c_all", "QDC coincidenza a 4", 1500, 1200);
    h_q1->SetStats(0); 
    h_q2->SetStats(0); 
    h_q3->SetStats(0); 
    h_q4->SetStats(0);
    c_all->SetGrid(0);
    h_q2->SetTitle("QDC coincidenza a 4;QDC;conteggi");
    h_q2->Draw("HIST");
    h_q1->Draw("HIST SAME");
    h_q3->Draw("HIST SAME");
    h_q4->Draw("HIST SAME");

    // Aggiungi legenda
    TLegend *legend = new TLegend(0.75, 0.75, 0.9, 0.9);
    legend->AddEntry(h_q1, "Channel 0", "l");
    legend->AddEntry(h_q2, "Channel 1", "l");
    legend->AddEntry(h_q3, "Channel 2", "l");
    legend->AddEntry(h_q4, "Channel 3", "l");
    legend->Draw();

    c_all->Update();
    c_all->SaveAs("../Dati/QDC/QDC_1234/QDC_all.png");

    double norm1, norm2, MPV, mu, sigmaL, sigmaG, norm_area;
    
    gStyle->SetOptFit(1111);
    gStyle->SetOptStat(1111);
    h_q1->SetStats(false); 
    h_q2->SetStats(false); 
    h_q3->SetStats(false); 
    h_q4->SetStats(false); 

/////////////////////////////////////////////////////////////////////////////////////////////////////
   // Definisce Landau per fit
    TF1 *landau_q1 = new TF1("landau_q1", "landau", xmin, xmax);
    TF1 *landau_q2 = new TF1("landau_q2", "landau", xmin, xmax);
    TF1 *landau_q3 = new TF1("landau_q3", "landau", xmin, xmax);
    TF1 *landau_q4 = new TF1("landau_q4", "landau", xmin, xmax);

    // Parametri iniziali
    norm1  = 800.0;
    MPV    = 1000.;
    sigmaL = 100.;

    landau_q1->SetParameters(norm1, MPV, sigmaL);
    landau_q2->SetParameters(norm1, MPV, sigmaL);
    landau_q3->SetParameters(norm1, MPV, sigmaL);
    landau_q4->SetParameters(norm1, MPV, sigmaL);

    // Fit line color
    landau_q1->SetLineColor(kBlue);
    landau_q2->SetLineColor(kBlue);
    landau_q3->SetLineColor(kBlue);
    landau_q4->SetLineColor(kBlue);

    // Fit degli istogrammi
    h_q1->Fit("landau_q1");
    h_q2->Fit("landau_q2");
    h_q3->Fit("landau_q3");
    h_q4->Fit("landau_q4");

    // Disegna istogrammi + fit
    TCanvas *c1_landau = new TCanvas("c1_landau", "QDC Channel 0 Landau", 1500, 1200);
    h_q1->SetTitle("QDC Channel 0 Landau;QDC;conteggi");
    h_q1->Draw("HIST");
    h_q1->SetStats(true);
    landau_q1->Draw("same");
    c1_landau->SetGrid();
    c1_landau->Update();
    c1_landau->SaveAs("../Dati/QDC/QDC_1234/QDC_Channel0_Landau.png");

    TCanvas *c2_landau = new TCanvas("c2_landau", "QDC Channel 1 Landau", 1500, 1200);
    h_q2->SetTitle("QDC Channel 1 Landau;QDC;conteggi");
    h_q2->Draw("HIST");
    h_q2->SetStats(true);
    landau_q2->Draw("same");
    c2_landau->SetGrid();
    c2_landau->Update();
    c2_landau->SaveAs("../Dati/QDC/QDC_1234/QDC_Channel1_Landau.png");

    TCanvas *c3_landau = new TCanvas("c3_landau", "QDC Channel 2 Landau", 1500, 1200);
    h_q3->SetTitle("QDC Channel 2 Landau;QDC;conteggi");
    h_q3->Draw("HIST");
    h_q2->SetStats(true);
    landau_q3->Draw("same");
    c3_landau->SetGrid();
    c3_landau->Update();
    c3_landau->SaveAs("../Dati/QDC/QDC_1234/QDC_Channel2_Landau.png");

    TCanvas *c4_landau = new TCanvas("c4_landau", "QDC Channel 3 Landau", 1500, 1200);
    h_q4->SetTitle("QDC Channel 3 Landau;QDC;conteggi");
    h_q4->Draw("HIST");
    h_q4->SetStats(true);
    landau_q4->Draw("same");
    c4_landau->SetGrid();
    c4_landau->Update();
    c4_landau->SaveAs("../Dati/QDC/QDC_1234/QDC_Channel3_Landau.png");

/////////////////////////////////////////////////////////////////////////////////////////////////////

    // Salva i risultati in un file ROOT
    TFile outFile("../Dati/QDC/QDC_1234/DataQDC_1234.root", "RECREATE");

    c_all->Write();

    c1_landau->Write();
    c2_landau->Write();
    c3_landau->Write();
    c4_landau->Write();

    outFile.Close();

/////////////////////////////////////////////////////////////////////////////////////////////////////

// ### Istogrammi dei residui ###
TCanvas *c_histores = new TCanvas("c_histores", "Istogramma residui QDC", 1800, 1600);
c_histores->Divide(2, 4);

// Istogramma dei residui per il primo fit (q1)
c_histores->cd(1);
TH1F* h1 = new TH1F("h1", "Istogramma residui Channel0_Landau", 50, -10.0, 10.0);
for (int i = 1; i <= h_q1->GetNbinsX(); ++i) {
    float resid = (h_q1->GetBinContent(i) - landau_q1->Eval(h_q1->GetBinCenter(i))) / TMath::Sqrt(h_q1->GetBinContent(i));
    h1->Fill(resid);
}
h1->SetFillColorAlpha(kRed, 1.0);
h1->SetFillStyle(3002);
TF1* gauss1 = new TF1("gauss1", "gaus", -10.0, 10.0);
gauss1->SetParameters(3.0, 0.0, 1.0);
h1->Fit(gauss1);
h1->Draw("E1");
h1->Draw("bar same");
gStyle->SetOptFit(1111);

// Istogramma dei residui per il terzo fit (q2)
c_histores->cd(3);
TH1F* h3 = new TH1F("h3", "Istogramma residui Channel1_Landau", 50, -10.0, 10.0);
for (int i = 1; i <= h_q2->GetNbinsX(); ++i) {
    float resid = (h_q2->GetBinContent(i) - landau_q2->Eval(h_q2->GetBinCenter(i))) / TMath::Sqrt(h_q2->GetBinContent(i));
    h3->Fill(resid);
}
h3->SetFillColorAlpha(kRed, 1.0);
h3->SetFillStyle(3002);
TF1* gauss3 = new TF1("gauss3", "gaus", -10.0, 10.0);
gauss3->SetParameters(3.0, 0.0, 1.0);
h3->Fit(gauss3);
h3->Draw("E1");
h3->Draw("bar same");
gStyle->SetOptFit(1111);

// Istogramma dei residui per il quinto fit (q3)
c_histores->cd(5);
TH1F* h5 = new TH1F("h5", "Istogramma residui Channel2_Landau", 50, -10.0, 10.0);
for (int i = 1; i <= h_q3->GetNbinsX(); ++i) {
    float resid = (h_q3->GetBinContent(i) - landau_q3->Eval(h_q3->GetBinCenter(i))) / TMath::Sqrt(h_q3->GetBinContent(i));
    h5->Fill(resid);
}
h5->SetFillColorAlpha(kRed, 1.0);
h5->SetFillStyle(3002);
TF1* gauss5 = new TF1("gauss5", "gaus", -10.0, 10.0);
gauss5->SetParameters(3.0, 0.0, 1.0);
h5->Fit(gauss5);
h5->Draw("E1");
h5->Draw("bar same");
gStyle->SetOptFit(1111);

// Istogramma dei residui per il settimo fit (q4)
c_histores->cd(7);
TH1F* h7 = new TH1F("h7", "Istogramma residui Channel3_Landau", 50, -10.0, 10.0);
for (int i = 1; i <= h_q4->GetNbinsX(); ++i) {
    float resid = (h_q4->GetBinContent(i) - landau_q4->Eval(h_q4->GetBinCenter(i))) / TMath::Sqrt(h_q4->GetBinContent(i));
    h7->Fill(resid);
}
h7->SetFillColorAlpha(kRed, 1.0);
h7->SetFillStyle(3002);
TF1* gauss7 = new TF1("gauss7", "gaus", -10.0, 10.0);
gauss7->SetParameters(3.0, 0.0, 1.0);
h7->Fit(gauss7);
h7->Draw("E1");
h7->Draw("bar same");
gStyle->SetOptFit(1111);

// Salva la canvas come file pdf
c_histores->Update();
c_histores->SaveAs("../Dati/QDC/QDC_1234/Residuals_Histograms.pdf");
c_histores->SaveAs("../Dati/QDC/QDC_1234/Residuals_Histograms.pdf");



    return;
}
