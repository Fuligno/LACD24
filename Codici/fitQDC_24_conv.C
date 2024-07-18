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

// Funzione di convoluzione Landau-Gauss
Double_t landauGaussConvolution(Double_t *x, Double_t *par) {
    // par[0] = Width (scale) parameter of Landau density
    // par[1] = Most Probable (MP, location) parameter of Landau density
    // par[2] = Total area (integral -inf to inf, normalization constant)
    // par[3] = Width (sigma) of convoluted Gaussian function
    
    Double_t invsq2pi = 0.3989422804014;   // (2 pi)^(-1/2)
    Double_t mpshift  = -0.22278298;       // Landau maximum location
    
    Double_t np = 1000.0;       // number of convolution steps
    Double_t sc = 10.0;         // convolution extends to +-sc Gaussian sigmas
    
    Double_t xx;
    Double_t mpc;
    Double_t fland;
    Double_t sum = 0.0;
    Double_t xlow,xupp;
    Double_t step;
    Double_t i;
    
    xlow = x[0] - sc * par[3];
    xupp = x[0] + sc * par[3];
    step = (xupp-xlow) / np;
    
    for(i=1.0; i<=np/2; i++) {                              // i da 1 a 500
        xx = xlow + (i-.5) * step;                          // x dello step = centro del bin
        //mpc = (xx - par[1]) / par[0] + mpshift;
        fland = TMath::Landau(xx,par[1],par[0]);            // fland = TMath::Landau(xx,par[1],par[0])/par[0];
        sum += fland * TMath::Gaus(x[0]-xx,0.,par[3]);      // sum += fland * TMath::Gaus(x[0]-xx,0.,par[3])/par[3];
        
        xx = xupp - (i-.5) * step;
        //mpc = (xx - par[1]) / par[0] + mpshift;
        fland = TMath::Landau(xx,par[1],par[0]);            // fland = TMath::Landau(xx,par[1],par[0])/par[0];
        sum += fland * TMath::Gaus(x[0]-xx,0.,par[3]);      // sum += fland * TMath::Gaus(x[0]-xx,0.,par[3])/par[3];
    }
    
    return (par[2] * step * sum * invsq2pi);
}

void fitQDC_24_conv() {

    // Prende i dati dal tree
    TFile* file = TFile::Open("./TreeOut.root");
    TTree* tree = (TTree*) file->Get("s2-4");
    
    // Definisce le variabili a cui accedere
    double q1, q2, q3, q4;
    tree->SetBranchAddress("q1", &q1);
    tree->SetBranchAddress("q2", &q2);
    tree->SetBranchAddress("q3", &q3);
    tree->SetBranchAddress("q4", &q4);

    // Crea gli istogrammi
    int nbins = 1100;
    double xmin = 0., xmax = 5500;
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
    gStyle->SetOptFit(1111111);
    gStyle->SetOptStat(1111);

    // Set histogram colors and transparency
    h_q1->SetLineColor(kRed+1.);
    h_q1->SetFillColorAlpha(kRed, 0.2);
    h_q1->SetLineWidth(1);

    h_q2->SetLineColor(kGreen+1.);
    h_q2->SetFillColorAlpha(kGreen, 0.2);
    h_q2->SetLineWidth(1);

    h_q3->SetLineColor(kCyan+1.);
    h_q3->SetFillColorAlpha(kCyan, 0.2);
    h_q3->SetLineWidth(1);

    h_q4->SetLineColor(kViolet+1.);
    h_q4->SetFillColorAlpha(kViolet, 0.2);
    h_q4->SetLineWidth(1);

    // Creazione delle funzioni di fit per ogni istogramma
    double xmin_fit = 600.,xmax_fit=4000.;
    TF1 *landau_gauss_q1 = new TF1("landau_gauss_q1", landauGaussConvolution, xmin_fit, xmax_fit, 4);
    TF1 *landau_gauss_q2 = new TF1("landau_gauss_q2", landauGaussConvolution, xmin_fit, xmax_fit, 4);
    TF1 *landau_gauss_q3 = new TF1("landau_gauss_q3", landauGaussConvolution, xmin_fit, xmax_fit, 4);
    TF1 *landau_gauss_q4 = new TF1("landau_gauss_q4", landauGaussConvolution, xmin_fit, xmax_fit, 4);

    // Inizializzazione dei parametri per ogni funzione
    double initWidth = 100;
    double initMPV = 1000;  
    double initArea = h_q2->Integral();
    double initSigma = 100.;  

    landau_gauss_q1->SetParameters(initWidth, initMPV, initArea, initSigma);
    landau_gauss_q2->SetParameters(initWidth, initMPV, initArea, initSigma);
    landau_gauss_q3->SetParameters(initWidth, initMPV, initArea, initSigma);
    landau_gauss_q4->SetParameters(initWidth, initMPV, initArea, initSigma);

    double min_area=(0.5*h_q2->Integral()), max_area=(6.*h_q2->Integral());
    //double min_sigma=0., max_sigma=1000.;
    landau_gauss_q1->SetParLimits(2, min_area, max_area);
    landau_gauss_q2->SetParLimits(2, min_area, max_area);
    landau_gauss_q3->SetParLimits(2, min_area, max_area);
    landau_gauss_q4->SetParLimits(2, min_area, max_area);

    //landau_gauss_q3->SetParLimits(2, min_sigma, max_sigma);

    // Impostazione dei nomi dei parametri
    landau_gauss_q1->SetParNames("Width", "MPV", "Area", "Sigma");
    landau_gauss_q2->SetParNames("Width", "MPV", "Area", "Sigma");
    landau_gauss_q3->SetParNames("Width", "MPV", "Area", "Sigma");
    landau_gauss_q4->SetParNames("Width", "MPV", "Area", "Sigma");

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

    h_q1->Fit("landau_gauss_q1", "R");  // "R" per usare il range della funzione
    h_q2->Fit("landau_gauss_q2", "R");
    h_q3->Fit("landau_gauss_q3", "R");
    h_q4->Fit("landau_gauss_q4", "R");

    // Disegna istogrammi + fit
    TCanvas *c1_conv = new TCanvas("c1_conv", "Segnale 1", 1500, 1200);
    h_q1->SetTitle("Trigger S2-4, Segnale 1;QDC;#frac{dN}{d QDC}");
    h_q1->Draw("E");
    h_q1->Draw("HIST SAME");
    landau_gauss_q1->Draw("same");
    c1_conv->SetGrid();
    c1_conv->Update();

    TCanvas *c2_conv = new TCanvas("c2_conv", "Segnale 2", 1500, 1200);
    h_q2->SetTitle("Trigger S2-4, Segnale 2;QDC;#frac{dN}{d QDC}");
    h_q2->Draw("E");
    h_q2->Draw("HIST SAME");
    landau_gauss_q2->Draw("same");
    c2_conv->SetGrid();
    c2_conv->Update();

    TCanvas *c3_conv = new TCanvas("c3_conv", "Segnale 3", 1500, 1200);
    h_q3->SetTitle("Trigger S2-4, Segnale 3;QDC;#frac{dN}{d QDC}");
    h_q3->Draw("E");
    h_q3->Draw("HIST SAME");
    landau_gauss_q3->Draw("same");
    c3_conv->SetGrid();
    c3_conv->Update();

    TCanvas *c4_conv = new TCanvas("c4_conv", "Segnale 4", 1500, 1200);
    h_q4->SetTitle("Trigger S2-4, Segnale 4;QDC;#frac{dN}{d QDC}");
    h_q4->Draw("E");
    h_q4->Draw("HIST SAME");
    landau_gauss_q4->Draw("same");
    c4_conv->SetGrid();
    c4_conv->Update();

    c1_conv->Write();
    c2_conv->Write();
    c3_conv->Write();
    c4_conv->Write();

    return;
}