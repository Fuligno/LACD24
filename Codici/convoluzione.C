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

void convoluzione() {

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

    // Definisce Landau * Gauss per fit
     // Crea le convoluzioni
    TF1Convolution *landau_gauss_conv = new TF1Convolution(landau, gaus, -500., 7000., false);
    //landau_gauss_conv->SetRange(-1000., 10000.);
    
    // Crea le funzioni di fit con la convoluzione
    TF1 *landau_gauss_q1 = new TF1("landau_gauss_q1", *landau_gauss_conv, xmin, xmax, landau_gauss_conv->GetNpar());
    TF1 *landau_gauss_q2 = new TF1("landau_gauss_q2", *landau_gauss_conv, xmin, xmax, landau_gauss_conv->GetNpar());
    TF1 *landau_gauss_q3 = new TF1("landau_gauss_q3", *landau_gauss_conv, xmin, xmax, landau_gauss_conv->GetNpar());
    TF1 *landau_gauss_q4 = new TF1("landau_gauss_q4", *landau_gauss_conv, xmin, xmax, landau_gauss_conv->GetNpar());

    // Parametri iniziali
    double MPV = 1000., minMVP = 500.,    maxMVP = 4000.;
    double sigmaL = 200., min_sigmaL = 0.1, max_sigmaL = 10000.;
    double mu = 1200.,   min_mu = 500.,    max_mu = 4000.;
    double sigmaG = 200., min_sigmaG = 0.1, max_sigmaG = 1000.;
    double norm1 = 10., min_norm1 = 1.,   max_norm1 =  10000.;

    landau_gauss_q1->SetParameters(MPV, sigmaL, mu, sigmaG, norm1);
    landau_gauss_q2->SetParameters(MPV, sigmaL, mu, sigmaG, norm1);
    landau_gauss_q3->SetParameters(MPV, sigmaL, mu, sigmaG, norm1);
    landau_gauss_q4->SetParameters(MPV, sigmaL, mu, sigmaG, norm1);
    
    /*landau_gauss_q1->SetParLimits(0, minMVP, maxMVP);                   // MPV
    landau_gauss_q1->SetParLimits(1, min_sigmaL, max_sigmaL);           // sigmaL
    landau_gauss_q1->SetParLimits(2, min_mu, max_mu);                   // mu
    landau_gauss_q1->SetParLimits(3, min_sigmaG, max_sigmaG);           // sigmaL
    landau_gauss_q1->SetParLimits(4, min_norm1, max_norm1);             // norm1

    landau_gauss_q2->SetParLimits(0, minMVP, maxMVP);                   // MPV
    landau_gauss_q2->SetParLimits(1, min_sigmaL, max_sigmaL);           // sigmaL
    landau_gauss_q2->SetParLimits(2, min_mu, max_mu);                   // mu
    landau_gauss_q2->SetParLimits(3, min_sigmaG, max_sigmaG);           // sigmaL
    landau_gauss_q2->SetParLimits(4, min_norm1, max_norm1);             // norm1

    landau_gauss_q3->SetParLimits(0, minMVP, maxMVP);                   // MPV
    landau_gauss_q3->SetParLimits(1, min_sigmaL, max_sigmaL);           // sigmaL
    landau_gauss_q3->SetParLimits(2, min_mu, max_mu);                   // mu
    landau_gauss_q3->SetParLimits(3, min_sigmaG, max_sigmaG);           // sigmaL
    landau_gauss_q3->SetParLimits(4, min_norm1, max_norm1);             // norm1
    
    landau_gauss_q4->SetParLimits(0, minMVP, maxMVP);                   // MPV
    landau_gauss_q4->SetParLimits(1, min_sigmaL, max_sigmaL);           // sigmaL
    landau_gauss_q4->SetParLimits(2, min_mu, max_mu);                   // mu
    landau_gauss_q4->SetParLimits(3, min_sigmaG, max_sigmaG);           // sigmaL
    landau_gauss_q4->SetParLimits(4, min_norm1, max_norm1);             // norm1*/
    

    // Fit line color
    landau_gauss_q1->SetLineColor(kBlue);
    landau_gauss_q2->SetLineColor(kBlue);
    landau_gauss_q3->SetLineColor(kBlue);
    landau_gauss_q4->SetLineColor(kBlue);

    // Fit degli istogrammi
    h_q1->GetListOfFunctions()->Clear();
    h_q2->GetListOfFunctions()->Clear();
    h_q3->GetListOfFunctions()->Clear();
    h_q4->GetListOfFunctions()->Clear();

    h_q1->Fit("landau_gauss_q1");
    h_q2->Fit("landau_gauss_q2");
    h_q3->Fit("landau_gauss_q3");
    h_q4->Fit("landau_gauss_q4");

    // Disegna istogrammi + fit
    TCanvas *c1_conv = new TCanvas("c1_conv", "QDC Channel 0 Landau*Gauss", 1500, 1200);
    h_q1->SetTitle("QDC Channel 0 Landau*Gauss;QDC;conteggi");
    h_q1->Draw("HIST");
    landau_gauss_q1->Draw("same");
    c1_conv->SetGrid();
    c1_conv->Update();
    c1_conv->SaveAs("../Dati/QDC/QDC_3/QDC_Channel0_Conv.png");

    TCanvas *c2_conv = new TCanvas("c2_conv", "QDC Channel 1 Landau*Gauss", 1500, 1200);
    h_q2->SetTitle("QDC Channel 1 Landau*Gauss;QDC;conteggi");
    h_q2->Draw("HIST");
    landau_gauss_q2->Draw("same");
    c2_conv->SetGrid();
    c2_conv->Update();
    c2_conv->SaveAs("../Dati/QDC/QDC_3/QDC_Channel1_Conv.png");

    TCanvas *c3_conv = new TCanvas("c3_conv", "QDC Channel 2 Landau*Gauss", 1500, 1200);
    h_q3->SetTitle("QDC Channel 2 Landau*Gauss;QDC;conteggi");
    h_q3->Draw("HIST");
    landau_gauss_q3->Draw("same");
    c3_conv->SetGrid();
    c3_conv->Update();
    c3_conv->SaveAs("../Dati/QDC/QDC_3/QDC_Channel2_Conv.png");

    TCanvas *c4_conv = new TCanvas("c4_conv", "QDC Channel 3 Landau*Gauss", 1500, 1200);
    h_q4->SetTitle("QDC Channel 3 Landau*Gauss;QDC;conteggi");
    h_q4->Draw("HIST");
    landau_gauss_q4->Draw("same");
    c4_conv->SetGrid();
    c4_conv->Update();
    c4_conv->SaveAs("../Dati/QDC/QDC_3/QDC_Channel3_Conv.png");


    // Salva i risultati in un file ROOT
    TFile outFile("../Dati/QDC/QDC_3/DataQDC_3.root", "RECREATE");

    c1_conv->Write();
    c2_conv->Write();
    c3_conv->Write();
    c4_conv->Write();

    outFile.Close();

    return;
}