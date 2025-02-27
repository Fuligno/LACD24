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
    TH1F *h_q1 = new TH1F("Segnale 1", "QDC Channel 0", nbins, xmin, xmax);
    TH1F *h_q2 = new TH1F("Segnale 2", "QDC Channel 1", 1000., xmin, 5000.);
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

    // Opzioni di stile
    gStyle->SetOptFit(111);
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

   // Definisce Landau per fit

    h_q1->SetStats(true); 
    h_q2->SetStats(true); 
    h_q3->SetStats(true); 
    h_q4->SetStats(true); 

    TF1 *landau_q1 = new TF1("landau_q1", "landau", xmin, xmax);
    TF1 *landau_q2 = new TF1("landau_q2", "landau", xmin, xmax);
    TF1 *landau_q3 = new TF1("landau_q3", "landau", xmin, xmax);
    TF1 *landau_q4 = new TF1("landau_q4", "landau", xmin, xmax);

    double norm1, norm2, MPV, mu, sigmaL, sigmaG, norm_area;
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // canvas con 3 pannelli: fit, residui, pulls
    
    // segnale 1
    TCanvas * canv1 = new TCanvas("canv1", "Segnale1", 1800, 1600);
    canv1->SetTitle("QDC Trigger S1-2-3-4, Segnale1");
    canv1->Divide(3, 1);
    canv1->cd(1);
    h_q1->SetTitle("Fit;QDC;#frac{dN}{d QDC}");
    h_q1->Draw("E");
    h_q1->Draw("HIST SAME");
    landau_q1->Draw("same");
    canv1->cd(1)->SetGrid();
    canv1->cd(1)->Update();
    
    //segnale 2
    TCanvas * canv2 = new TCanvas("canv2", "Segnale2", 1800, 1600);
    canv2->SetTitle("QDC Trigger S1-2-3-4, Segnale2");
    canv2->Divide(3, 1);
    canv2->cd(1);
    h_q2->SetTitle("Fit;QDC;#frac{dN}{d QDC}");
    h_q2->Draw("E");
    h_q2->Draw("HIST SAME");
    landau_q2->Draw("same");
    canv2->cd(1)->SetGrid();
    canv2->cd(1)->Update();

    //segnale 3
    TCanvas * canv3 = new TCanvas("canv3", "Segnale3", 1800, 1600);
    canv3->SetTitle("QDC Trigger S1-2-3-4, Segnale3");
    canv3->Divide(3, 1);
    canv3->cd(1);
    h_q3->SetTitle("Fit;QDC;#frac{dN}{d QDC}");
    h_q3->Draw("E");
    h_q3->Draw("HIST SAME");
    landau_q3->Draw("same");
    canv3->cd(1)->SetGrid();
    canv3->cd(1)->Update();

    //segnale 4
    TCanvas * canv4 = new TCanvas("canv4", "Segnale4", 1800, 1600);
    canv4->SetTitle("QDC Trigger S1-2-3-4, Segnale4");
    canv4->Divide(3, 1);
    canv4->cd(1);
    h_q4->SetTitle("Fit;QDC;#frac{dN}{d QDC}");
    h_q4->Draw("E");
    h_q4->Draw("HIST SAME");
    landau_q4->Draw("same");
    canv4->cd(1)->SetGrid();
    canv4->cd(1)->Update();

    canv1->cd(1)->Write();
    canv2->cd(1)->Write();
    canv3->cd(1)->Write();
    canv4->cd(1)->Write();

/////////////////////////////////////////////////////////////////////////////////////////////////////

// RESIDUI

 vector<double> sigma_res1, sigma_res2, sigma_res3, sigma_res4;		// vettore che contiene gli errori sui residui
    double sigma_res_value1, sigma_res_value2, sigma_res_value3, sigma_res_value4;
    
    // --- parte per calcolare gli errori sui residui ---
    for (int i=0; i < h_q1->GetNbinsX(); i++) {
	    sigma_res_value1 = TMath::Sqrt(h_q1->GetBinContent(i));
        sigma_res1.push_back(sigma_res_value1);
    }
	for (int i=0; i < h_q2->GetNbinsX(); i++) {
	    sigma_res_value2 = TMath::Sqrt(h_q2->GetBinContent(i));
        sigma_res2.push_back(sigma_res_value2);
    }
    for (int i=0; i < h_q3->GetNbinsX(); i++) {
	    sigma_res_value3 = TMath::Sqrt(h_q3->GetBinContent(i));
        sigma_res3.push_back(sigma_res_value3);
    }
    for (int i=0; i < h_q4->GetNbinsX(); i++) {
	    sigma_res_value4 = TMath::Sqrt(h_q4->GetBinContent(i));
        sigma_res4.push_back(sigma_res_value4);
    }

    // Residui per q1
    canv1->cd(2);
    canv1->cd(2)->SetGrid();
    TGraphErrors *residui_q1 = new TGraphErrors(h_q1->GetNbinsX());
    for (int i = 0; i < h_q1->GetNbinsX(); i++) {
        double fit_value = landau_q1->Eval(h_q1->GetBinCenter(i));
        double residual = (h_q1->GetBinContent(i) - fit_value);
        residui_q1->SetPoint(i, h_q1->GetBinCenter(i), residual);
        residui_q1->SetPointError(i, 0, sigma_res1[i]);
    }
    
    TF1 *horiz1 = new TF1("horiz_q1", "pol1", 0.0, 100.0);	// fit a retta dei residui (linea il più possibile orizzontale)
    horiz1->SetLineColor(kBlue);
    residui_q1->SetTitle("Residui;QDC;Residui");
    residui_q1->SetMarkerStyle(1);
    //residui_q1->SetMarkerSize(.1);
    residui_q1->SetMarkerColor(kRed);
    residui_q1->SetLineColor(kRed);
    //residui_q1->SetStats(false); 
    residui_q1->Draw("AP");
    residui_q1->Fit("horiz_q1");
    horiz1->Draw("same");
    
    TPaveText *eqretta1 = new TPaveText(0.5, 0.1, 0.9, 0.2, "NDC");
    eqretta1->SetBorderSize(1);
    eqretta1->SetFillColor(kWhite);
    eqretta1->SetTextAlign(20);
    eqretta1->SetTextSize(0.03);
    double mr_q1 = horiz1->GetParameter(1);
    double mr_q1_err = horiz1->GetParError(1);
    double qr_q1 = horiz1->GetParameter(0);
    double qr_q1_err = horiz1->GetParError(0);
    eqretta1->AddText(Form("m = %.3f +/- %.3f, q = %.3f +/- %.3f", mr_q1, mr_q1_err, qr_q1, qr_q1_err));
    eqretta1->Draw("same");


// Residui per q2
    canv2->cd(2);
    canv2->cd(2)->SetGrid();
    TGraphErrors *residui_q2 = new TGraphErrors(h_q2->GetNbinsX());
    for (int i = 0; i < h_q2->GetNbinsX(); i++) {
        double fit_value = landau_q2->Eval(h_q2->GetBinCenter(i));
        double residual = (h_q2->GetBinContent(i) - fit_value);
        residui_q2->SetPoint(i, h_q2->GetBinCenter(i), residual);
        residui_q2->SetPointError(i, 0, sigma_res2[i]);
    }
    
    TF1 *horiz2 = new TF1("horiz_q2", "pol1", 0.0, 100.0);	// fit a retta dei residui (linea il più possibile orizzontale)
    horiz2->SetLineColor(kBlue);
    residui_q2->SetTitle("Residui;QDC;Residui");
    residui_q2->SetMarkerStyle(1);
    //residui_q2->SetMarkerSize(.1);
    residui_q2->SetMarkerColor(kGreen);
    residui_q2->SetLineColor(kGreen);
    //residui_q2->SetStats(false); 
    residui_q2->Draw("AP");
    residui_q2->Fit("horiz_q2");
    horiz2->Draw("same");
    
    /*TPaveText *eqretta2 = new TPaveText(0.5, 0.1, 0.9, 0.2, "NDC");
    eqretta2->SetBorderSize(1);
    eqretta2->SetFillColor(kWhite);
    eqretta2->SetTextAlign(20);
    eqretta2->SetTextSize(0.03);
    double mr_q2 = horiz2->GetParameter(1);
    double mr_q2_err = horiz2->GetParError(1);
    double qr_q2 = horiz2->GetParameter(0);
    double qr_q2_err = horiz2->GetParError(0);
    eqretta2->AddText(Form("m = %.3f +/- %.3f, q = %.3f +/- %.3f", mr_q2, mr_q2_err, qr_q2, qr_q2_err));
    eqretta2->Draw("same");*/

    // Residui per q3
    canv3->cd(2);
    canv3->cd(2)->SetGrid();
    TGraphErrors *residui_q3 = new TGraphErrors(h_q3->GetNbinsX());
    for (int i = 0; i < h_q3->GetNbinsX(); i++) {
        double fit_value = landau_q3->Eval(h_q3->GetBinCenter(i));
        double residual = (h_q3->GetBinContent(i) - fit_value);
        residui_q3->SetPoint(i, h_q3->GetBinCenter(i), residual);
        residui_q3->SetPointError(i, 0, sigma_res3[i]);
    }
    
    TF1 *horiz3 = new TF1("horiz_q3", "pol1", 0.0, 100.0);	// fit a retta dei residui (linea il più possibile orizzontale)
    horiz3->SetLineColor(kBlue);
    residui_q3->SetTitle("Residui;QDC;Residui");
    residui_q3->SetMarkerStyle(1);
    //residui_q3->SetMarkerSize(.1);
    residui_q3->SetMarkerColor(kCyan);
    residui_q3->SetLineColor(kCyan);
    //residui_q3->SetStats(false); 
    residui_q3->Draw("AP");
    residui_q3->Fit("horiz_q3");
    horiz3->Draw("same");
    
    /*TPaveText *eqretta3 = new TPaveText(0.5, 0.1, 0.9, 0.2, "NDC");
    eqretta3->SetBorderSize(1);
    eqretta3->SetFillColor(kWhite);
    eqretta3->SetTextAlign(20);
    eqretta3->SetTextSize(0.03);
    double mr_q3 = horiz3->GetParameter(1);
    double mr_q3_err = horiz3->GetParError(1);
    double qr_q3 = horiz3->GetParameter(0);
    double qr_q3_err = horiz3->GetParError(0);
    eqretta3->AddText(Form("m = %.3f +/- %.3f, q = %.3f +/- %.3f", mr_q3, mr_q3_err, qr_q3, qr_q3_err));
    eqretta3->Draw("same");*/

    // Residui per q4
    canv4->cd(2);
    canv4->cd(2)->SetGrid();
    TGraphErrors *residui_q4 = new TGraphErrors(h_q4->GetNbinsX());
    for (int i = 0; i < h_q4->GetNbinsX(); i++) {
        double fit_value = landau_q4->Eval(h_q4->GetBinCenter(i));
        double residual = (h_q4->GetBinContent(i) - fit_value);
        residui_q4->SetPoint(i, h_q4->GetBinCenter(i), residual);
        residui_q4->SetPointError(i, 0, sigma_res4[i]);
    }
    
    TF1 *horiz4 = new TF1("horiz_q4", "pol1", 0.0, 100.0);	// fit a retta dei residui (linea il più possibile orizzontale)
    horiz4->SetLineColor(kBlue);
    residui_q4->SetTitle("Residui;QDC;Residui");
    residui_q4->SetMarkerStyle(1);
    //residui_q4->SetMarkerSize(.1);
    residui_q4->SetMarkerColor(kViolet);
    residui_q4->SetLineColor(kViolet);
    //residui_q4->SetStats(false); 
    residui_q4->Draw("AP");
    residui_q4->Fit("horiz_q4");
    horiz4->Draw("same");
    
    /*TPaveText *eqretta4 = new TPaveText(0.5, 0.1, 0.9, 0.2, "NDC");
    eqretta4->SetBorderSize(1);
    eqretta4->SetFillColor(kWhite);
    eqretta4->SetTextAlign(20);
    eqretta4->SetTextSize(0.03);
    double mr_q4 = horiz4->GetParameter(1);
    double mr_q4_err = horiz4->GetParError(1);
    double qr_q4 = horiz4->GetParameter(0);
    double qr_q4_err = horiz4->GetParError(0);
    eqretta4->AddText(Form("m = %.3f +/- %.3f, q = %.3f +/- %.3f", mr_q4, mr_q4_err, qr_q4, qr_q4_err));
    eqretta4->Draw("same");*/


////////////////////////////////////////////////////////////////////////////////////////////////////

// Istogramma dei residui per il primo fit (q1)
canv1->cd(3);
TH1F* h1 = new TH1F("Istogramma pull Segnale 1", "Istogramma pull Segnale 1", 31, -20.0, 20.0);
for (int i = 1; i <= h_q1->GetNbinsX(); ++i) {
    float resid = (h_q1->GetBinContent(i) - landau_q1->Eval(h_q1->GetBinCenter(i))) / TMath::Sqrt(h_q1->GetBinContent(i));
    h1->Fill(resid);
}
h1->SetFillColorAlpha(kRed, 1.0);
h1->SetFillStyle(3002);
h1->SetTitle("Istogramma dei pull;pull;conteggi");
TF1* gauss1 = new TF1("gauss1", "gaus", -10.0, 10.0);
gauss1->SetParameters(3.0, 0.0, 1.0);
h1->Fit(gauss1);
h1->Draw("E1");
h1->Draw("bar same");
gStyle->SetOptFit(111);

// Istogramma dei residui per il terzo fit (q2)
canv2->cd(3);
TH1F* h3 = new TH1F("Istogramma pull Segnale 2", "Istogramma pull Segnale 2", 31, -20.0, 20.0);
for (int i = 1; i <= h_q2->GetNbinsX(); ++i) {
    float resid = (h_q2->GetBinContent(i) - landau_q2->Eval(h_q2->GetBinCenter(i))) / TMath::Sqrt(h_q2->GetBinContent(i));
    h3->Fill(resid);
}
h3->SetFillColorAlpha(kRed, 1.0);
h3->SetFillStyle(3002);
h3->SetTitle("Istogramma dei pull;pull;conteggi");
TF1* gauss3 = new TF1("gauss3", "gaus", -10.0, 10.0);
gauss3->SetParameters(3.0, 0.0, 1.0);
h3->Fit(gauss3);
h3->Draw("E1");
h3->Draw("bar same");
gStyle->SetOptFit(111);

// Istogramma dei residui per il quinto fit (q3)
canv3->cd(3);
TH1F* h5 = new TH1F("Istogramma pull Segnale 3","Istogramma pull Segnale 3", 31, -20.0, 20.0);
for (int i = 1; i <= h_q3->GetNbinsX(); ++i) {
    float resid = (h_q3->GetBinContent(i) - landau_q3->Eval(h_q3->GetBinCenter(i))) / TMath::Sqrt(h_q3->GetBinContent(i));
    h5->Fill(resid);
}
h5->SetFillColorAlpha(kRed, 1.0);
h5->SetFillStyle(3002);
h5->SetTitle("Istogramma dei pull;pull;conteggi");
TF1* gauss5 = new TF1("gauss5", "gaus", -10.0, 10.0);
gauss5->SetParameters(3.0, 0.0, 1.0);
h5->Fit(gauss5);
h5->Draw("E1");
h5->Draw("bar same");
gStyle->SetOptFit(111);

// Istogramma dei residui per il settimo fit (q4)
canv4->cd(3);
TH1F* h7 = new TH1F("Istogramma pull Segnale 4","Istogramma pull Segnale 4", 31, -20.0, 20.0);
for (int i = 1; i <= h_q4->GetNbinsX(); ++i) {
    float resid = (h_q4->GetBinContent(i) - landau_q4->Eval(h_q4->GetBinCenter(i))) / TMath::Sqrt(h_q4->GetBinContent(i));
    h7->Fill(resid);
}
h7->SetFillColorAlpha(kRed, 1.0);
h7->SetFillStyle(3002);
h7->SetTitle("Istogramma dei pull;pull;conteggi");
TF1* gauss7 = new TF1("gauss7", "gaus", -10.0, 10.0);
gauss7->SetParameters(3.0, 0.0, 1.0);
h7->Fit(gauss7);
h7->Draw("E1");
h7->Draw("bar same");
gStyle->SetOptFit(111);

// Salva la canvas come file pdf
canv1->Update();
canv2->Update();
canv3->Update();
canv4->Update();
    return;
}
