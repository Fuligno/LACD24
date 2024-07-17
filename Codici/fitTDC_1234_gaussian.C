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

// Definizione della funzione doppia gaussiana
double fitf2(double* x, double* p) {
    double norm1 = p[0], norm2 = p[1], mu = p[2], sigma1 = p[3], sigma2 = p[4];
    double arg1 = (x[0] - mu) / sigma1;
    double arg2 = (x[0] - mu) / sigma2;
    return norm1 * TMath::Exp(-0.5 * arg1 * arg1) + norm2 * TMath::Exp(-0.5 * arg2 * arg2);
}

// Definizione della funzione tripla gaussiana
double fitf3(double* x, double* p) {
    double norm1 = p[0], norm2 = p[1], norm3 = p[2], mu = p[3], sigma1 = p[4], sigma2 = p[5], sigma3 = p[6];
    double arg1 = (x[0] - mu) / sigma1;
    double arg2 = (x[0] - mu) / sigma2;
    double arg3 = (x[0] - mu) / sigma3;
    return norm1 * TMath::Exp(-0.5 * arg1 * arg1) + norm2 * TMath::Exp(-0.5 * arg2 * arg2) + norm3 * TMath::Exp(-0.5 * arg3 * arg3);
}

void fitTDC_1234_gaussian() {
    // Prende i dati dal tree
    TFile* file = TFile::Open("./TreeOut.root");
    TTree* tree = (TTree*) file->Get("s1-2-3-4");

    // Definisce le variabili a cui accedere
    double t1, t2, t3, t4;
    tree->SetBranchAddress("t1", &t1);
    tree->SetBranchAddress("t2", &t2);
    tree->SetBranchAddress("t3", &t3);
    tree->SetBranchAddress("t4", &t4);

    // Crea gli istogrammi
    int nbins = 220;
    double xmin = 400, xmax = 1500;
    TH1F *h_t1 = new TH1F("t1", "TDC Channel 0", nbins, xmin, xmax);
    TH1F *h_t2 = new TH1F("t2", "TDC Channel 1", nbins, xmin, xmax);
    TH1F *h_t3 = new TH1F("t3", "TDC Channel 2", nbins, xmin, xmax);
    TH1F *h_t4 = new TH1F("t4", "TDC Channel 3", nbins, xmin, xmax);

    Long64_t tot_entries = tree->GetEntries();

    for(Long64_t iEntry = 0; iEntry < tot_entries; ++iEntry) {
        tree->GetEntry(iEntry);
        h_t1->Fill(t1);
        h_t2->Fill(t2);
        h_t3->Fill(t3);
        h_t4->Fill(t4);
    }

    // Opzioni di stile
    gStyle->SetOptFit(1111);
    gStyle->SetOptStat(1111);

    // Definisce la funzione gaussiana, doppia gaussiana e tripla gaussiana per il fit
    TF1 *gauss_t1 = new TF1("gauss_t1", "gaus", xmin, xmax);
    TF1 *gauss_t2 = new TF1("gauss_t2", "gaus", xmin, xmax);
    TF1 *gauss_t3 = new TF1("gauss_t3", "gaus", xmin, xmax);
    TF1 *gauss_t4 = new TF1("gauss_t4", "gaus", xmin, xmax);

    TF1 *dgauss_t1 = new TF1("dgauss_t1", fitf2, xmin, xmax, 5);
    TF1 *dgauss_t2 = new TF1("dgauss_t2", fitf2, xmin, xmax, 5);
    TF1 *dgauss_t3 = new TF1("dgauss_t3", fitf2, xmin, xmax, 5);
    TF1 *dgauss_t4 = new TF1("dgauss_t4", fitf2, xmin, xmax, 5);

    TF1 *tgauss_t1 = new TF1("tgauss_t1", fitf3, xmin, xmax, 7);
    TF1 *tgauss_t2 = new TF1("tgauss_t2", fitf3, xmin, xmax, 7);
    TF1 *tgauss_t3 = new TF1("tgauss_t3", fitf3, xmin, xmax, 7);
    TF1 *tgauss_t4 = new TF1("tgauss_t4", fitf3, xmin, xmax, 7);

    // Parametri iniziali
    double cost1 = 5000, cost2 = 8000, cost3 = 10000;
    double mu = 900.0;
    double sigma1 = 60.0, sigma2 = 50.0, sigma3 = 20.0;

    gauss_t1->SetParameters(cost1, mu, sigma1);
    gauss_t2->SetParameters(cost1, mu, sigma1);
    gauss_t3->SetParameters(cost1, mu, sigma1);
    gauss_t4->SetParameters(cost1, mu, sigma1);
    dgauss_t1->SetParameters(cost1, cost2, mu, sigma1, sigma2);
    dgauss_t2->SetParameters(cost1, cost2, mu, sigma1, sigma2);
    dgauss_t3->SetParameters(cost1, cost2, mu, sigma1, sigma2);
    dgauss_t4->SetParameters(cost1, cost2, mu, sigma1, sigma2);
    tgauss_t1->SetParameters(cost1, cost2, cost3, mu, sigma1, sigma2, sigma3);
    tgauss_t2->SetParameters(cost1, cost2, cost3, mu, sigma1, sigma2, sigma3);
    tgauss_t3->SetParameters(cost1, cost2, cost3, mu, sigma1, sigma2, sigma3);
    tgauss_t4->SetParameters(cost1, cost2, cost3, mu, sigma1, sigma2, sigma3);

    // Set histogram colors and transparency
    h_t1->SetLineColor(kRed+1.);
    h_t1->SetFillColorAlpha(kRed, 0.2);
    h_t1->SetLineWidth(2);

    h_t2->SetLineColor(kGreen+1.);
    h_t2->SetFillColorAlpha(kGreen, 0.2);
    h_t2->SetLineWidth(2);

    h_t3->SetLineColor(kCyan+1.);
    h_t3->SetFillColorAlpha(kCyan, 0.2);
    h_t3->SetLineWidth(2);

    h_t4->SetLineColor(kViolet+1.);
    h_t4->SetFillColorAlpha(kViolet, 0.2);
    h_t4->SetLineWidth(2);

    // Fit line color
    gauss_t1->SetLineColor(kBlue);
    gauss_t2->SetLineColor(kBlue);
    gauss_t3->SetLineColor(kBlue);
    gauss_t4->SetLineColor(kBlue);
    dgauss_t1->SetLineColor(kBlue);
    dgauss_t2->SetLineColor(kBlue);
    dgauss_t3->SetLineColor(kBlue);
    dgauss_t4->SetLineColor(kBlue);
    tgauss_t1->SetLineColor(kBlue);
    tgauss_t2->SetLineColor(kBlue);
    tgauss_t3->SetLineColor(kBlue);
    tgauss_t4->SetLineColor(kBlue);

    // CANVAS PER TUTTI I CANALI 
    TCanvas *c_all = new TCanvas("c_all", "TDC coincidenza a 4", 1500, 1800);
    c_all->SetGrid();
    h_t1->SetTitle("Fit TDC coincidenza a 4;ch;Conteggi");
    h_t1->Draw("HIST");
    h_t2->Draw("HIST same");
    h_t3->Draw("HIST same");
    h_t4->Draw("HIST same");
    c_all->Update();
    c_all->SaveAs("../Dati/TDC/TDC_1234/TDC_all.png");

    // Disegna istogrammi + fit
    /* // GATE
    TCanvas *c1_gauss = new TCanvas("c1_gauss", "TDC Channel 0 Gauss", 1500, 1800);
    c1_gauss->SetGrid();
    h_t1->Fit("gauss_t1");
    h_t1->Draw("HIST");
    gauss_t1->Draw("same");
    c1_gauss->Update();
    c1_gauss->SaveAs("../Dati/TDC/TDC_1234/TDC_Channel0_Gauss.png");

    TCanvas *c1_dgauss = new TCanvas("c1_dgauss", "TDC Channel 0 DoubleGauss", 1500, 1800);
    c1_dgauss->SetGrid();
    h_t1->Fit("dgauss_t1");
    h_t1->Draw("HIST");
    dgauss_t1->Draw("same");
    c1_dgauss->Update();
    c1_dgauss->SaveAs("../Dati/TDC/TDC_1234/TDC_Channel0_doubleGauss.png");

    TCanvas *c1_tgauss = new TCanvas("c1_tgauss", "TDC Channel 0 TripleGauss", 1500, 1800);
    c1_tgauss->SetGrid();
    h_t1->Fit("tgauss_t1");
    h_t1->Draw("HIST");
    tgauss_t1->Draw("same");
    c1_tgauss->Update();
    c1_tgauss->SaveAs("../Dati/TDC/TDC_1234/TDC_Channel0_tripleGauss.png");
    */

    ///////

    TCanvas *c2_gauss = new TCanvas("c2_gauss", "TDC Channel 1 Gauss", 1500, 1800);
    c2_gauss->SetGrid();
    h_t2->SetTitle("Fit TDC (gauss) CH1;ch;Conteggi");
    h_t2->Fit("gauss_t2");
    h_t2->Draw("HIST");
    gauss_t2->Draw("same");
    c2_gauss->Update();
    c2_gauss->SaveAs("../Dati/TDC/TDC_1234/TDC_Channel1_Gauss.png");

    TCanvas *c2_dgauss = new TCanvas("c2_dgauss", "TDC Channel 1 DoubleGauss", 1500, 1800);
    c2_dgauss->SetGrid();
    h_t2->SetTitle("Fit TDC (doppio gauss) CH1;ch;Conteggi");
    h_t2->Fit("dgauss_t2");
    h_t2->Draw("HIST");
    dgauss_t2->Draw("same");
    c2_dgauss->Update();
    c2_dgauss->SaveAs("../Dati/TDC/TDC_1234/TDC_Channel1_doubleGauss.png");

    TCanvas *c2_tgauss = new TCanvas("c2_tgauss", "TDC Channel 1 TripleGauss", 1500, 1800);
    c2_tgauss->SetGrid();
    h_t2->SetTitle("Fit TDC (triplo gauss) CH1;ch;Conteggi");
    h_t2->Fit("tgauss_t2");
    h_t2->Draw("HIST");
    tgauss_t2->Draw("same");
    c2_tgauss->Update();
    c2_tgauss->SaveAs("../Dati/TDC/TDC_1234/TDC_Channel1_tripleGauss.png");

    ///////

    TCanvas *c3_gauss = new TCanvas("c3_gauss", "TDC Channel 2 Gauss", 1500, 1800);
    c3_gauss->SetGrid();
    h_t3->SetTitle("Fit TDC (gauss) CH1;ch;Conteggi");
    h_t3->Fit("gauss_t3");
    h_t3->Draw("HIST");
    gauss_t3->Draw("same");
    c3_gauss->Update();
    c3_gauss->SaveAs("../Dati/TDC/TDC_1234/TDC_Channel2_Gauss.png");

    TCanvas *c3_dgauss = new TCanvas("c3_dgauss", "TDC Channel 2 DoubleGauss", 1500, 1800);
    c3_dgauss->SetGrid();
    h_t3->SetTitle("Fit TDC (doppio gauss) CH1;ch;Conteggi");
    h_t3->Fit("dgauss_t3");
    h_t3->Draw("HIST");
    dgauss_t3->Draw("same");
    c3_dgauss->Update();
    c3_dgauss->SaveAs("../Dati/TDC/TDC_1234/TDC_Channel2_doubleGauss.png");

    TCanvas *c3_tgauss = new TCanvas("c3_tgauss", "TDC Channel 2 TripleGauss", 1500, 1800);
    c3_tgauss->SetGrid();
    h_t3->SetTitle("Fit TDC (triplo gauss) CH1;ch;Conteggi");
    h_t3->Fit("tgauss_t3");
    h_t3->Draw("HIST");
    tgauss_t3->Draw("same");
    c3_tgauss->Update();
    c3_tgauss->SaveAs("../Dati/TDC/TDC_1234/TDC_Channel2_tripleGauss.png");

    ///////

    TCanvas *c4_gauss = new TCanvas("c4_gauss", "TDC Channel 3 Gauss", 1500, 1800);
    c4_gauss->SetGrid();
    h_t4->SetTitle("Fit TDC (gauss) CH1;ch;Conteggi");
    h_t4->Fit("gauss_t4");
    h_t4->Draw("HIST");
    gauss_t4->Draw("same");
    c4_gauss->Update();
    c4_gauss->SaveAs("../Dati/TDC/TDC_1234/TDC_Channel3_Gauss.png");

    TCanvas *c4_dgauss = new TCanvas("c4_dgauss", "TDC Channel 3 DoubleGauss", 1500, 1800);
    c4_dgauss->SetGrid();
    h_t4->SetTitle("Fit TDC (doppio gauss) CH1;ch;Conteggi");
    h_t4->Fit("dgauss_t4");
    h_t4->Draw("HIST");
    dgauss_t4->Draw("same");
    c4_dgauss->Update();
    c4_dgauss->SaveAs("../Dati/TDC/TDC_1234/TDC_Channel3_doubleGauss.png");

    TCanvas *c4_tgauss = new TCanvas("c4_tgauss", "TDC Channel 3 TripleGauss", 1500, 1800);
    c4_tgauss->SetGrid();
    h_t4->SetTitle("Fit TDC (triplo gauss) CH1;ch;Conteggi");
    h_t4->Fit("tgauss_t4");
    h_t4->Draw("HIST");
    tgauss_t4->Draw("same");
    c4_tgauss->Update();
    c4_tgauss->SaveAs("../Dati/TDC/TDC_1234/TDC_Channel3_tripleGauss.png");

    ///////

    // Salva i risultati in un file ROOT
    TFile outFile("../Dati/TDC/TDC_1234/DataTDC_1234.root", "RECREATE");

    c_all->Write();

    //c1_gauss->Write();
    c2_gauss->Write();
    c3_gauss->Write();
    c4_gauss->Write();

    //c1_dgauss->Write();
    c2_dgauss->Write();
    c3_dgauss->Write();
    c4_dgauss->Write();

    //c1_tgauss->Write();
    c2_tgauss->Write();
    c3_tgauss->Write();
    c4_tgauss->Write();

    outFile.Close();

    return;
}
