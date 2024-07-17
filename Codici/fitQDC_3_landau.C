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

/*// Definizione della funzione landau + gauss
double fitf_sum1(double* x, double* p) {
    double norm1 = p[0], norm2 = p[1], MPV = p[2], mu = p[3], sigmaL = p[4], sigmaG = p[5];
    double argG = (x[0] - mu) / sigmaG;
    return norm1 * TMath::Landau(x[0], MPV, sigmaL) + norm2 * TMath::Exp(-0.5 * argG * argG);
}*/

void fitQDC_3_landau() {

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

/////////////////////////////////////////////////////////////////////////////////////////////////////
    // ISTOGRAMMI TUTTI INSIEME
    TCanvas *c_all = new TCanvas("c_all", "QDC coincidenza a 1", 1500, 1200);
    h_q1->SetStats(0); 
    h_q2->SetStats(0); 
    h_q3->SetStats(0); 
    h_q4->SetStats(0);
    c_all->SetGrid(0);
    h_q2->SetTitle("QDC coincidenza a 1;QDC;conteggi");
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
    c_all->SaveAs("../Dati/QDC/QDC_3/QDC_all.png");

    double norm1, norm2, MPV, mu, sigmaL, sigmaG, norm_area;
    
    gStyle->SetOptFit(1111);
    gStyle->SetOptStat(1111);
    h_q1->SetStats(1); 
    h_q2->SetStats(1); 
    h_q3->SetStats(1); 
    h_q4->SetStats(1); 

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
    landau_q2->SetParameters(800., 1000., 100.);
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
    landau_q1->Draw("same");
    c1_landau->SetGrid();
    c1_landau->Update();
    c1_landau->SaveAs("../Dati/QDC/QDC_3/QDC_Channel0_Landau.png");

    TCanvas *c2_landau = new TCanvas("c2_landau", "QDC Channel 1 Landau", 1500, 1200);
    h_q2->SetTitle("QDC Channel 1 Landau;QDC;conteggi");
    h_q2->Draw("HIST");
    landau_q2->Draw("same");
    c2_landau->SetGrid();
    c2_landau->Update();
    c2_landau->SaveAs("../Dati/QDC/QDC_3/QDC_Channel1_Landau.png");

    TCanvas *c3_landau = new TCanvas("c3_landau", "QDC Channel 2 Landau", 1500, 1200);
    h_q3->SetTitle("QDC Channel 2 Landau;QDC;conteggi");
    h_q3->Draw("HIST");
    landau_q3->Draw("same");
    c3_landau->SetGrid();
    c3_landau->Update();
    c3_landau->SaveAs("../Dati/QDC/QDC_3/QDC_Channel2_Landau.png");

    TCanvas *c4_landau = new TCanvas("c4_landau", "QDC Channel 3 Landau", 1500, 1200);
    h_q4->SetTitle("QDC Channel 3 Landau;QDC;conteggi");
    h_q4->Draw("HIST");
    landau_q4->Draw("same");
    c4_landau->SetGrid();
    c4_landau->Update();
    c4_landau->SaveAs("../Dati/QDC/QDC_3/QDC_Channel3_Landau.png");
/////////////////////////////////////////////////////////////////////////////////////////////////////
    // Definisce Landau * Gauss per fit
     // Crea le convoluzioni
    TF1Convolution *landau_gauss_conv = new TF1Convolution("landau", "gaus", xmin, xmax, false);
    landau_gauss_conv->SetRange(-1000., 6000.);
    //cout << landau_gauss_conv->GetNpar() << endl;
    
    // Crea le funzioni di fit con la convoluzione
    TF1 *landau_gauss_q1 = new TF1("landau_gauss_q1", *landau_gauss_conv, xmin, xmax, landau_gauss_conv->GetNpar());
    TF1 *landau_gauss_q2 = new TF1("landau_gauss_q2", *landau_gauss_conv, xmin, xmax, landau_gauss_conv->GetNpar());
    TF1 *landau_gauss_q3 = new TF1("landau_gauss_q3", *landau_gauss_conv, xmin, xmax, landau_gauss_conv->GetNpar());
    TF1 *landau_gauss_q4 = new TF1("landau_gauss_q4", *landau_gauss_conv, xmin, xmax, landau_gauss_conv->GetNpar());

    // Parametri iniziali
    norm1 = 45.;
    MPV = 600., mu = 500.;
    sigmaL = 80., sigmaG = 80.;
    landau_gauss_q1->SetParameters(norm1, MPV, sigmaL, mu, sigmaG);
    landau_gauss_q2->SetParameters(norm1, MPV, sigmaL, mu, sigmaG);
    landau_gauss_q3->SetParameters(norm1, MPV, sigmaL, mu, sigmaG);
    landau_gauss_q4->SetParameters(norm1, MPV, sigmaL, mu, sigmaG);

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

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*
   // Definisce Landau + Gauss per fit
    h_q1->SetStats(1); 
    h_q2->SetStats(1); 
    h_q3->SetStats(1); 
    h_q4->SetStats(1); 
    TF1 *gauss_q1 = new TF1("gauss_q1", fitf_sum1, xmin, xmax, 6);
    TF1 *gauss_q2 = new TF1("gauss_q2", fitf_sum1, xmin, xmax, 6);
    TF1 *gauss_q3 = new TF1("gauss_q3", fitf_sum1, xmin, xmax, 6);
    TF1 *gauss_q4 = new TF1("gauss_q4", fitf_sum1, xmin, xmax, 6);

    // Parametri iniziali
    norm1  = 4000.,  norm2  = 1000.; 
    MPV    = 1000.,  mu     = 800.; 
    sigmaL = 50.;    sigmaG = 50.;

    gauss_q1->SetParameters(norm1, norm2, MPV, mu, sigmaL, sigmaG);
    gauss_q2->SetParameters(norm1, norm2, MPV, mu, sigmaL, sigmaG);
    gauss_q3->SetParameters(norm1, norm2, MPV, mu, sigmaL, sigmaG);
    gauss_q4->SetParameters(norm1, norm2, MPV, mu, sigmaL, sigmaG);

    // Fit line color
    gauss_q1->SetLineColor(kBlue);
    gauss_q2->SetLineColor(kBlue);
    gauss_q3->SetLineColor(kBlue);
    gauss_q4->SetLineColor(kBlue);

    h_q1->Fit("gauss_q1");
    h_q2->Fit("gauss_q2");
    h_q3->Fit("gauss_q3");
    h_q4->Fit("gauss_q4");
    
    // Disegna istogrammi + fit
    TCanvas *c1_gauss = new TCanvas("c1_gauss", "QDC Channel 0 Landau+Gauss", 1500, 1200);
    h_q1->SetTitle("QDC Channel 0 Landau+Gauss;QDC;conteggi");
    h_q1->Draw("HIST");
    gauss_q1->Draw("same");
    c1_gauss->SetGrid();
    c1_gauss->Update();
    c1_gauss->SaveAs("../Dati/QDC/QDC_3/QDC_Channel0_Gauss.png");

    TCanvas *c2_gauss = new TCanvas("c2_gauss", "QDC Channel 1 Landau+Gauss", 1500, 1200);
    h_q2->SetTitle("QDC Channel 1 Landau+Gauss;QDC;conteggi");
    h_q2->Draw("HIST");
    gauss_q2->Draw("same");
    c2_gauss->SetGrid();
    c2_gauss->Update();
    c2_gauss->SaveAs("../Dati/QDC/QDC_3/QDC_Channel1_Gauss.png");

    TCanvas *c3_gauss = new TCanvas("c3_gauss", "QDC Channel 2 Landau+Gauss", 1500, 1200);
    h_q3->SetTitle("QDC Channel 2 Landau+Gauss;QDC;conteggi");
    h_q3->Draw("HIST");
    gauss_q3->Draw("same");
    c3_gauss->SetGrid();
    c3_gauss->Update();
    c3_gauss->SaveAs("../Dati/QDC/QDC_3/QDC_Channel2_Gauss.png");

    TCanvas *c4_gauss = new TCanvas("c4_gauss", "QDC Channel 3 Landau+Gauss", 1500, 1200);
    h_q4->SetTitle("QDC Channel 3 Landau+Gauss;QDC;conteggi");
    h_q4->Draw("HIST");
    gauss_q4->Draw("same");
    c4_gauss->SetGrid();
    c4_gauss->Update();
    c4_gauss->SaveAs("../Dati/QDC/QDC_3/QDC_Channel3_Gauss.png");
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////

    // Salva i risultati in un file ROOT
    TFile outFile("../Dati/QDC/QDC_3/DataQDC_3.root", "RECREATE");

    c_all->Write();

    c1_conv->Write();
    c2_conv->Write();
    c3_conv->Write();
    c4_conv->Write();

    c1_landau->Write();
    c2_landau->Write();
    c3_landau->Write();
    c4_landau->Write();

    /*c1_gauss->Write();
    c2_gauss->Write();
    c3_gauss->Write();
    c4_gauss->Write();*/

    

    outFile.Close();

    return;
}
