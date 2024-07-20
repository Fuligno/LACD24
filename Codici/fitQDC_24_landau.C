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

double landaushort(double* x, double* p){
    if(x[0]>1300 || x[0]< 700){return 0;}
    double fitval;
    double norm = p[0];
    double MPV = p[1];
    double sigma = p[2];
    fitval = norm*TMath::Landau(x[0], MPV, sigma, true);
    return fitval;
}
double graphlandau(double* x, double* p) {
    double fitval;
    double norm = p[0];
    double MPV = p[1];
    double sigma = p[2];
    fitval = norm*TMath::Landau(x[0], MPV, sigma, true);
    return fitval;
}


void fitQDC_24_landau() {

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

    double xmin_fit = 0., xmax_fit = 5000.;
    double_t* par1;
    TF1 *landau_q1 = new TF1("landau_q1", "landau", xmin_fit, xmax_fit);
    TF1 *landau_q2 = new TF1("landau_q2", landaushort, xmin_fit, xmax_fit, 3);
    TF1* func = new TF1("func", graphlandau, xmin_fit, xmax_fit, 3);
    TF1 *landau_q3 = new TF1("landau_q3", "landau", xmin_fit, xmax_fit);
    TF1 *landau_q4 = new TF1("landau_q4", "landau", xmin_fit, xmax_fit);

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
    par1 = landau_q2->GetParameters();
    h_q2->GetListOfFunctions()->Clear();
    //func->SetParameters(par1[0],par1[1],par1[2]);
    func->FixParameter(0, par1[0]);
    func->FixParameter(1, par1[1]);
    func->FixParameter(2, par1[2]);
    h_q2->Fit("func");
    func->SetLineColor(kBlue);
    //func->Draw("same");
    // Disegna istogrammi + fit
    TCanvas *c1_landau = new TCanvas("c1_landau", "Segnale 1", 1500, 1200);
    //h_q1->GetListOfFunctions()->Clear();
    h_q1->SetTitle("QDC Trigger S2-4, Segnale 1;QDC;#frac{dN}{d QDC}");
    h_q1->Draw("E");
    h_q1->Draw("HIST SAME");
    //landau_q1->Draw("same");
    c1_landau->SetGrid();
    c1_landau->Update();

    TCanvas *c2_landau = new TCanvas("c2_landau", "Segnale 2", 1500, 1200);
    //h_q2->GetListOfFunctions()->Clear();
    c2_landau->cd();
    h_q2->SetTitle("QDC Trigger S2-4, Segnale 2;QDC;#frac{dN}{d QDC}");
    h_q2->Draw("E");
    h_q2->Draw("SAME");
    //func->Draw("same");
    //landau_q2->Draw("same");
    h_q2->GetXaxis()->SetRangeUser(0, 5000);
    c2_landau->SetGrid();
    c2_landau->Update();

    TCanvas *c3_landau = new TCanvas("c3_landau", "Segnale 3", 1500, 1200);
    //h_q3->GetListOfFunctions()->Clear();
    h_q3->SetTitle("QDC Trigger S2-4, Segnale 3;QDC;#frac{dN}{d QDC}");
    h_q3->Draw("E");
    h_q3->Draw("HIST SAME");
    //landau_q3->Draw("same");
    c3_landau->SetGrid();
    c3_landau->Update();

    TCanvas *c4_landau = new TCanvas("c4_landau", "Segnale 4", 1500, 1200);
    //h_q4->GetListOfFunctions()->Clear();
    h_q4->SetTitle("QDC Trigger S2-4, Segnale 4;QDC;#frac{dN}{d QDC}");
    h_q4->Draw("E");
    h_q4->Draw("HIST SAME");
    //landau_q4->Draw("same");
    c4_landau->SetGrid();
    c4_landau->Update();

    c1_landau->Write();
    c2_landau->Write();
    c3_landau->Write();
    c4_landau->Write();

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
    TCanvas *c_residui1 = new TCanvas("c_residui1", "QDC Trigger S2-4, Grafico dei Residui segnale 1", 1200, 800);
    c_residui1->cd();
    c_residui1->SetGrid();
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
    residui_q1->SetMarkerSize(.1);
    residui_q1->SetMarkerColor(kRed);
    residui_q1->SetLineColor(kRed);
    //residui_q1->SetStats(false); 
    residui_q1->Draw("AP");
    residui_q1->Fit("horiz_q1");
    horiz1->Draw("same");

// Residui per q2
    TCanvas *c_residui2 = new TCanvas("c_residui2", "QDC Trigger S2-4, Grafico dei Residui segnale 2", 1200, 800);
    c_residui2->cd();
    c_residui2->SetGrid();
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
    residui_q2->SetMarkerSize(.1);
    residui_q2->SetMarkerColor(kGreen);
    residui_q2->SetLineColor(kGreen);
    //residui_q2->SetStats(false); 
    residui_q2->Draw("AP");
    residui_q2->Fit("horiz_q2");
    horiz2->Draw("same");

    // Residui per q3
    TCanvas *c_residui3 = new TCanvas("c_residui3", "QDC Trigger S2-4, Grafico dei Residui segnale 3", 1200, 800);
    c_residui3->cd();
    c_residui3->SetGrid();
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
    residui_q3->SetMarkerSize(.1);
    residui_q3->SetMarkerColor(kCyan);
    residui_q3->SetLineColor(kCyan);
    //residui_q3->SetStats(false); 
    residui_q3->Draw("AP");
    residui_q3->Fit("horiz_q3");
    horiz3->Draw("same");

    // Residui per q4
    TCanvas *c_residui4 = new TCanvas("c_residui4", "QDC Trigger S2-4, Grafico dei Residui segnale 4", 1200, 800);
    c_residui4->cd();
    c_residui4->SetGrid();
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
    residui_q4->SetMarkerSize(.1);
    residui_q4->SetMarkerColor(kViolet);
    residui_q4->SetLineColor(kViolet);
    //residui_q4->SetStats(false); 
    residui_q4->Draw("AP");
    residui_q4->Fit("horiz_q4");
    horiz4->Draw("same");

////////////////////////////////////////////////////////////////////////////////////////////////////

// ### Istogrammi dei residui ###

// Istogramma dei residui per il primo fit (q1)
TCanvas *c_histores1 = new TCanvas("c_histores1", "Istogramma dei pull QDC segnale 1", 1800, 1600);
c_histores1->cd();
TH1F* h1 = new TH1F("Istogramma pull Segnale 1", "Istogramma pull Segnale 1", 31, -20.0, 20.0);
for (int i = 1; i <= h_q1->GetNbinsX(); ++i) {
    float resid = (h_q1->GetBinContent(i) - landau_q1->Eval(h_q1->GetBinCenter(i))) / TMath::Sqrt(h_q1->GetBinContent(i));
    h1->Fill(resid);
}
h1->SetFillColorAlpha(kRed, 1.0);
h1->SetFillStyle(3002);
h1->SetTitle("Istogramma dei pull");
TF1* gauss1 = new TF1("gauss1", "gaus", -10.0, 10.0);
gauss1->SetParameters(3.0, 0.0, 1.0);
h1->Fit(gauss1);
h1->Draw("E1");
h1->Draw("bar same");
gStyle->SetOptFit(111);

// Istogramma dei residui per il terzo fit (q2)
TCanvas *c_histores2 = new TCanvas("c_histores2", "Istogramma dei pull QDC segnale 2", 1800, 1600);
c_histores2->cd();
TH1F* h3 = new TH1F("Istogramma pull Segnale 2", "Istogramma pull Segnale 2", 39, -25.0, 25.0);
for (int i = 1; i <= h_q2->GetNbinsX(); ++i) {
    float resid = (h_q2->GetBinContent(i) - landau_q2->Eval(h_q2->GetBinCenter(i))) / TMath::Sqrt(h_q2->GetBinContent(i));
    h3->Fill(resid);
}
h3->SetFillColorAlpha(kRed, 1.0);
h3->SetFillStyle(3002);
h3->SetTitle("Istogramma dei pull");
TF1* gauss3 = new TF1("gauss3", "gaus", -10.0, 10.0);
gauss3->SetParameters(3.0, 0.0, 1.0);
h3->Fit(gauss3);
h3->Draw("E1");
h3->Draw("bar same");
gStyle->SetOptFit(111);

// Istogramma dei residui per il quinto fit (q3)
TCanvas *c_histores3 = new TCanvas("c_histores3", "Istogramma dei pull QDC segnale 3", 1800, 1600);
c_histores3->cd();
TH1F* h5 = new TH1F("Istogramma pull Segnale 3","Istogramma pull Segnale 3", 31, -20.0, 20.0);
for (int i = 1; i <= h_q3->GetNbinsX(); ++i) {
    float resid = (h_q3->GetBinContent(i) - landau_q3->Eval(h_q3->GetBinCenter(i))) / TMath::Sqrt(h_q3->GetBinContent(i));
    h5->Fill(resid);
}
h5->SetFillColorAlpha(kRed, 1.0);
h5->SetFillStyle(3002);
h5->SetTitle("Istogramma dei pull");
TF1* gauss5 = new TF1("gauss5", "gaus", -10.0, 10.0);
gauss5->SetParameters(3.0, 0.0, 1.0);
h5->Fit(gauss5);
h5->Draw("E1");
h5->Draw("bar same");
gStyle->SetOptFit(111);

// Istogramma dei residui per il settimo fit (q4)
TCanvas *c_histores4 = new TCanvas("c_histores4", "Istogramma dei pull QDC segnale 4", 1800, 1600);
c_histores4->cd();
TH1F* h7 = new TH1F("Istogramma pull Segnale 4","Istogramma pull Segnale 4", 31, -20.0, 20.0);
for (int i = 1; i <= h_q4->GetNbinsX(); ++i) {
    float resid = (h_q4->GetBinContent(i) - landau_q4->Eval(h_q4->GetBinCenter(i))) / TMath::Sqrt(h_q4->GetBinContent(i));
    h7->Fill(resid);
}
h7->SetFillColorAlpha(kRed, 1.0);
h7->SetFillStyle(3002);
h7->SetTitle("Istogramma dei pull");
TF1* gauss7 = new TF1("gauss7", "gaus", -10.0, 10.0);
gauss7->SetParameters(3.0, 0.0, 1.0);
h7->Fit(gauss7);
h7->Draw("E1");
h7->Draw("bar same");
gStyle->SetOptFit(111);

// Salva la canvas come file pdf
c_histores1->Update();
c_histores2->Update();
c_histores3->Update();
c_histores4->Update();

    return;
}