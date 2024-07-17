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

void fitQDC_24() {

    // Prende i dati dal tree
    TFile* file = TFile::Open("./TreeOut.root");
    TTree* tree = (TTree*) file->Get("s2-4_full");
    
    // Definisce le variabili a cui accedere
    double q2, q4;
    tree->SetBranchAddress("q2", &q2);
    tree->SetBranchAddress("q4", &q4);

    // Crea gli istogrammi
    int nbins = 1000;
    double xmin = 0, xmax = 3900;
    TH1F *h_q2 = new TH1F("q2", "QDC Channel 1", nbins, xmin, xmax);
    TH1F *h_q4 = new TH1F("q4", "QDC Channel 3", nbins, xmin, xmax);

    Long64_t tot_entries = tree->GetEntries();

    for(Long64_t iEntry=0; iEntry < tot_entries; ++iEntry){
        tree->GetEntry(iEntry);
        h_q2->Fill(q2);
        h_q4->Fill(q4);
    }

    // Normalizza gli istogrammi
    double bin_width = (xmax - xmin) / nbins;
    h_q2->Scale(1.0 / (h_q2->GetEntries() * bin_width));
    h_q4->Scale(1.0 / (h_q4->GetEntries() * bin_width));

    // Opzioni di stile
    gStyle->SetOptFit(1111);
    gStyle->SetOptStat(1111);

    // Definisce Landau per fit
    TF1 *landau_q2 = new TF1("landau_q2", "landau", xmin, xmax);
    TF1 *landau_q4 = new TF1("landau_q4", "landau", xmin, xmax);

    // Parametri iniziali
    double cost1 = 1.0;
    double MPV = 1000.0;
    double sigma_landau = 100.0;

    landau_q2->SetParameters(cost1, MPV, sigma_landau);
    landau_q4->SetParameters(cost1, MPV, sigma_landau);

    // Fit degli istogrammi
    h_q2->Fit("landau_q2");
    h_q4->Fit("landau_q4");

    TCanvas *c2_landau = new TCanvas("c2_landau", "QDC Channel 1 Landau", 800, 600);
    h_q2->Draw();
    c2_landau->Update();
    c2_landau->SaveAs("../Dati/QDC/QDC_24/QDC_Channel1_Landau.png");

    TCanvas *c4_landau = new TCanvas("c4_landau", "QDC Channel 3 Landau", 800, 600);
    h_q4->Draw();
    c4_landau->Update();
    c4_landau->SaveAs("../Dati/QDC/QDC_24/QDC_Channel3_Landau.png");

    // Definisce Landau + Gauss per fit
    TF1 *gauss_q2 = new TF1("gauss_q2", "landau(3) + gaus(3)", xmin, xmax);
    TF1 *gauss_q4 = new TF1("gauss_q4", "landau(3) + gaus(3)", xmin, xmax);

    // Parametri iniziali
    cost1 = 1.0;
    MPV = 1000.0;
    sigma_landau = 100.0;
    double cost2 = 1.0;
    double mean = 1000.0;
    double sigma = 200.0;

    gauss_q2->SetParameters(cost1, MPV, sigma_landau, cost2, mean, sigma);
    gauss_q4->SetParameters(cost1, MPV, sigma_landau, cost2, mean, sigma);

    h_q2->Fit("gauss_q2");
    h_q4->Fit("gauss_q4");
    
    // Disegna istogrammi + fit
    TCanvas *c2_gauss = new TCanvas("c2_gauss", "QDC Channel 1 Landau+Gauss", 800, 600);
    h_q2->Draw();
    c2_gauss->Update();
    c2_gauss->SaveAs("../Dati/QDC/QDC_24/QDC_Channel1_Gauss.png");

    TCanvas *c4_gauss = new TCanvas("c4_gauss", "QDC Channel 3 Landau+Gauss", 800, 600);
    h_q4->Draw();
    c4_gauss->Update();
    c4_gauss->SaveAs("../Dati/QDC/QDC_24/QDC_Channel3_Gauss.png");
    /*
    // Definisce Landau * Gauss per fit
    TF1Convolution *convolution_q2 = new TF1Convolution("landau_q2", "gauss_q2", -1000, 6000);
    TF1Convolution *convolution_q4 = new TF1Convolution("landau_q4", "gauss_q4", -1000, 6000);

    TF1 *landau_gauss_q2 = new TF1("landau_gauss_q2", *convolution_q2, xmin, xmax, convolution_q2->GetNpar());
    TF1 *landau_gauss_q4 = new TF1("landau_gauss_q4", *convolution_q4, xmin, xmax, convolution_q4->GetNpar());

    // Parametri iniziali
    cost1 = 1.0;
    MPV = 1000.0;
    sigma_landau = 100.0;
    cost2 = 1.0;
    mean = 1000.0;
    sigma = 200.0;

    landau_gauss_q2->SetParameters(cost1, MPV, sigma_landau, cost2, mean, sigma);
    landau_gauss_q4->SetParameters(cost1, MPV, sigma_landau, cost2, mean, sigma);

    // Fit degli istogrammi
    h_q2->Fit("landau_gauss_q2");
    h_q4->Fit("landau_gauss_q4");

    // Disegna istogrammi + fit
    TCanvas *c2_conv = new TCanvas("c2_conv", "QDC Channel 1 Landau*Gauss", 800, 600);
    h_q2->Draw();
    c2_conv->Update();
    c2_conv->SaveAs("../Dati/QDC/QDC_24/QDC_Channel1_Conv.png");

    TCanvas *c4_conv = new TCanvas("c4_conv", "QDC Channel 3 Landau*Gauss", 800, 600);
    h_q4->Draw();
    c4_conv->Update();
    c4_conv->SaveAs("../Dati/QDC/QDC_24/QDC_Channel3_Conv.png");

    // Salva i risultati in un file ROOT
    TFile outFile("../Dati/QDC/QDC_24/DataQDC_24.root", "RECREATE");

    c2_landau->Write();
    c4_landau->Write();

    c2_gauss->Write();
    c4_gauss->Write();

    c2_conv->Write();
    c4_conv->Write();

    outFile.Close();
*/
    return;
}
