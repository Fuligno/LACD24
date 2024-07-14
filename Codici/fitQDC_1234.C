#include "Riostream.h"
#include <string>
#include "TTree.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TF1.h" 
#include "TF1Convolution.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"


using namespace std;

void fitQDC_1234() {

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
    int nbins = 1000;
    double xmin = 0, xmax = 3900;
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

    // Normalizza gli istogrammi
    double bin_width = (xmax - xmin) / nbins;
    h_q1->Scale(1.0 / (h_q1->GetEntries() * bin_width));
    h_q2->Scale(1.0 / (h_q2->GetEntries() * bin_width));
    h_q3->Scale(1.0 / (h_q3->GetEntries() * bin_width));
    h_q4->Scale(1.0 / (h_q4->GetEntries() * bin_width));

    // Opzioni di stile
    gStyle->SetOptFit(1111);
    gStyle->SetOptStat(1111);

    // Definisce Landau per fit
    TF1 *landau_q1 = new TF1("landau_q1", "landau", xmin, xmax);
    TF1 *landau_q2 = new TF1("landau_q2", "landau", xmin, xmax);
    TF1 *landau_q3 = new TF1("landau_q3", "landau", xmin, xmax);
    TF1 *landau_q4 = new TF1("landau_q4", "landau", xmin, xmax);

    // Parametri iniziali
    double cost1 = 1.0;
    double MPV = 1000.0;
    double sigma_landau = 100.0;

    landau_q1->SetParameters(cost1, MPV, sigma_landau);
    landau_q2->SetParameters(cost1, MPV, sigma_landau);
    landau_q3->SetParameters(cost1, MPV, sigma_landau);
    landau_q4->SetParameters(cost1, MPV, sigma_landau);

    // Fit degli istogrammi
    h_q1->Fit("landau_q1");
    h_q2->Fit("landau_q2");
    h_q3->Fit("landau_q3");
    h_q4->Fit("landau_q4");

    // Disegna istogrammi + fit
    TCanvas *c1_landau = new TCanvas("c1_landau", "QDC Channel 0 Landau", 800, 600);
    h_q1->Draw();
    c1_landau->Update();
    c1_landau->SaveAs("../Dati/QDC/QDC_Channel0_Landau.png");

    TCanvas *c2_landau = new TCanvas("c2_landau", "QDC Channel 1 Landau", 800, 600);
    h_q2->Draw();
    c2_landau->Update();
    c2_landau->SaveAs("../Dati/QDC/QDC_Channel1_Landau.png");

    TCanvas *c3_landau = new TCanvas("c3_landau", "QDC Channel 2 Landau", 800, 600);
    h_q3->Draw();
    c3_landau->Update();
    c3_landau->SaveAs("../Dati/QDC/QDC_Channel2_Landau.png");

    TCanvas *c4_landau = new TCanvas("c4_landau", "QDC Channel 3 Landau", 800, 600);
    h_q4->Draw();
    c4_landau->Update();
    c4_landau->SaveAs("../Dati/QDC/QDC_Channel3_Landau.png");

    // Definisce Gauss per fit
    TF1 *gauss_q1 = new TF1("gauss_q1", "gaus", xmin, xmax);
    TF1 *gauss_q2 = new TF1("gauss_q2", "gaus", xmin, xmax);
    TF1 *gauss_q3 = new TF1("gauss_q3", "gaus", xmin, xmax);
    TF1 *gauss_q4 = new TF1("gauss_q4", "gaus", xmin, xmax);

    // Parametri iniziali
    double cost2 = 1.0;
    double mean = 1000.0;
    double sigma = 200.0;

    gauss_q1->SetParameters(cost2, mean, sigma);
    gauss_q2->SetParameters(cost2, mean, sigma);
    gauss_q3->SetParameters(cost2, mean, sigma);
    gauss_q4->SetParameters(cost2, mean, sigma);

    // Fit degli istogrammi
    h_q1->Fit("gauss_q1");
    h_q2->Fit("gauss_q2");
    h_q3->Fit("gauss_q3");
    h_q4->Fit("gauss_q4");
    
    // Disegna istogrammi + fit
    TCanvas *c1_gauss = new TCanvas("c1_gauss", "QDC Channel 0 Gauss", 800, 600);
    h_q1->Draw();
    c1_gauss->Update();
    c1_gauss->SaveAs("../Dati/QDC/QDC_Channel0_Gauss.png");

    TCanvas *c2_gauss = new TCanvas("c2_gauss", "QDC Channel 1 Gauss", 800, 600);
    h_q2->Draw();
    c2_gauss->Update();
    c2_gauss->SaveAs("../Dati/QDC/QDC_Channel1_Gauss.png");

    TCanvas *c3_gauss = new TCanvas("c3_gauss", "QDC Channel 2 Gauss", 800, 600);
    h_q3->Draw();
    c3_gauss->Update();
    c3_gauss->SaveAs("../Dati/QDC/QDC_Channel2_Gauss.png");

    TCanvas *c4_gauss = new TCanvas("c4_gauss", "QDC Channel 3 Gauss", 800, 600);
    h_q4->Draw();
    c4_gauss->Update();
    c4_gauss->SaveAs("../Dati/QDC/QDC_Channel3_Gauss.png");

    // Definisce Landau * Gauss per fit
    TF1 *landau_gaus_q1 = new TF1("landau_gaus_q1", "landau(0) + gaus(3)", xmin, xmax);
    TF1 *landau_gaus_q2 = new TF1("landau_gaus_q2", "landau(0) + gaus(3)", xmin, xmax);
    TF1 *landau_gaus_q3 = new TF1("landau_gaus_q3", "landau(0) + gaus(3)", xmin, xmax);
    TF1 *landau_gaus_q4 = new TF1("landau_gaus_q4", "landau(0) + gaus(3)", xmin, xmax);

    // Parametri iniziali
    cost1 = 1.0;
    MPV = 1000.0;
    sigma_landau = 100.0;
    cost2 = 1.0;
    mean = 1000.0;
    sigma = 200.0;

    landau_gaus_q1->SetParameters(cost1, MPV, sigma_landau, cost2, mean, sigma);
    landau_gaus_q2->SetParameters(cost1, MPV, sigma_landau, cost2, mean, sigma);
    landau_gaus_q3->SetParameters(cost1, MPV, sigma_landau, cost2, mean, sigma);
    landau_gaus_q4->SetParameters(cost1, MPV, sigma_landau, cost2, mean, sigma);

    // Fit degli istogrammi
    h_q1->Fit("landau_gaus_q1");
    h_q2->Fit("landau_gaus_q2");
    h_q3->Fit("landau_gaus_q3");
    h_q4->Fit("landau_gaus_q4");

    // Disegna istogrammi + fit
    TCanvas *c1_conv = new TCanvas("c1_conv", "QDC Channel 0 Conv", 800, 600);
    h_q1->Draw();
    c1_conv->Update();
    c1_conv->SaveAs("../Dati/QDC/QDC_Channel0_Conv.png");

    TCanvas *c2_conv = new TCanvas("c2_conv", "QDC Channel 1 Conv", 800, 600);
    h_q2->Draw();
    c2_conv->Update();
    c2_conv->SaveAs("../Dati/QDC/QDC_Channel1_Conv.png");

    TCanvas *c3_conv = new TCanvas("c3_conv", "QDC Channel 2 Conv", 800, 600);
    h_q3->Draw();
    c3_conv->Update();
    c3_conv->SaveAs("../Dati/QDC/QDC_Channel2_Conv.png");

    TCanvas *c4_conv = new TCanvas("c4_conv", "QDC Channel 3 Conv", 800, 600);
    h_q4->Draw();
    c4_conv->Update();
    c4_conv->SaveAs("../Dati/QDC/QDC_Channel3_Conv.png");

    // Salva i risultati in un file ROOT
    TFile outFile("../Dati/QDC/DataQDC_1234.root", "RECREATE");

    c1_landau->Write();
    c2_landau->Write();
    c3_landau->Write();
    c4_landau->Write();

    c1_gauss->Write();
    c2_gauss->Write();
    c3_gauss->Write();
    c4_gauss->Write();

    c1_conv->Write();
    c2_conv->Write();
    c3_conv->Write();
    c4_conv->Write();

    outFile.Close();

    return;
}
