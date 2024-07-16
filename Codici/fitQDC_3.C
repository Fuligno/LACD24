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

void fitQDC_3() {

    // Prende i dati dal tree
    TFile* file = TFile::Open("./TreeOut.root");
    TTree* tree = (TTree*) file->Get("s3_full");
    
    // Definisce le variabili a cui accedere
    double q3;
    tree->SetBranchAddress("q3", &q3);

    // Crea gli istogrammi
    int nbins = 1000;
    double xmin = 0, xmax = 3900;
    TH1F *h_q3 = new TH1F("q3", "QDC Channel 2", nbins, xmin, xmax);

    Long64_t tot_entries = tree->GetEntries();

    for(Long64_t iEntry=0; iEntry < tot_entries; ++iEntry){
        tree->GetEntry(iEntry);
        h_q3->Fill(q3);
    }

    // Normalizza gli istogrammi
    double bin_width = (xmax - xmin) / nbins;
    h_q3->Scale(1.0 / (h_q3->GetEntries() * bin_width));

    // Opzioni di stile
    gStyle->SetOptFit(1111);
    gStyle->SetOptStat(1111);

    // Definisce Landau per fit
    TF1 *landau_q3 = new TF1("landau_q3", "landau", xmin, xmax);

    // Parametri iniziali
    double cost1 = 1.0;
    double MPV = 1000.0;
    double sigma_landau = 100.0;

    landau_q3->SetParameters(cost1, MPV, sigma_landau);

    // Fit degli istogrammi
    h_q3->Fit("landau_q3");

    // Disegna istogrammi + fit
    TCanvas *c3_landau = new TCanvas("c3_landau", "QDC Channel 2 Landau", 800, 600);
    h_q3->Draw();
    c3_landau->Update();
    c3_landau->SaveAs("../Dati/QDC/QDC_3/QDC_Channel2_Landau.png");

    // Definisce Landau + Gauss per fit
    TF1 *gauss_q3 = new TF1("gauss_q3", "landau(3) + gaus(3)", xmin, xmax);

    // Parametri iniziali
    cost1 = 1.0;
    MPV = 1000.0;
    sigma_landau = 100.0;
    double cost2 = 1.0;
    double mean = 1000.0;
    double sigma = 200.0;

    gauss_q3->SetParameters(cost1, MPV, sigma_landau, cost2, mean, sigma);

    h_q3->Fit("gauss_q3");

    // Disegna istogrammi + fit

    TCanvas *c3_gauss = new TCanvas("c3_gauss", "QDC Channel 2 Landau+Gauss", 800, 600);
    h_q3->Draw();
    c3_gauss->Update();
    c3_gauss->SaveAs("../Dati/QDC/QDC_3/QDC_Channel2_Gauss.png");

    // Definisce Landau * Gauss per fit
    TF1Convolution *convolution_q3 = new TF1Convolution("landau_q3", "gauss_q3", -1000, 6000);

    TF1 *landau_gauss_q3 = new TF1("landau_gauss_q3", *convolution_q3, xmin, xmax, convolution_q3->GetNpar());

    // Parametri iniziali
    cost1 = 1.0;
    MPV = 1000.0;
    sigma_landau = 100.0;
    cost2 = 1.0;
    mean = 1000.0;
    sigma = 200.0;

    landau_gauss_q3->SetParameters(cost1, MPV, sigma_landau, cost2, mean, sigma);

    // Fit degli istogrammi
    h_q3->Fit("landau_gauss_q3");

    // Disegna istogrammi + fit
    TCanvas *c3_conv = new TCanvas("c3_conv", "QDC Channel 2 Landau*Gauss", 800, 600);
    h_q3->Draw();
    c3_conv->Update();
    c3_conv->SaveAs("../Dati/QDC/QDC_3/QDC_Channel2_Conv.png");

    // Salva i risultati in un file ROOT
    TFile outFile("../Dati/QDC/QDC_3/DataQDC_3.root", "RECREATE");

    c3_landau->Write();
  
    c3_gauss->Write();

    c3_conv->Write();

    outFile.Close();

    return;
}
