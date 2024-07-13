#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TStyle.h>
#include <TApplication.h>

void istogrammiTDC_s1234() {
    // Open the ROOT file
    TFile *file = TFile::Open("TreeOut.root");

    // Check if the file was opened successfully
    if (!file || file->IsZombie()) {
        printf("Error: Unable to open file TreeOut.root\n");
        return;
    }
    // Retrieve the TTree named "s1-2-3-4"
    TTree *tree_1234 = (TTree*)file->Get("s1-2-3-4");
    if (!tree_1234) {
        printf("Error: Tree not found\n");
        file->Close();
        return;
    }

    // Create histograms for each branch
    int nbins = 1000;
    double xmin = 450, xmax = 1450;
    
    TH1F *h_t1_s1234 = new TH1F("h_t1_s1234", "t1_s1234", nbins, xmin, xmax);
    TH1F *h_t2_s1234 = new TH1F("h_t2_s1234", "t2_s1234", nbins, xmin, xmax);
    TH1F *h_t3_s1234 = new TH1F("h_t3_s1234", "t3_s1234", nbins, xmin, xmax);
    TH1F *h_t4_s1234 = new TH1F("h_t4_s1234", "t4_s1234", nbins, xmin, xmax);

    // Fill the histograms with data from the TTree
    tree_1234->Draw("t1 >> h_t1_s1234");
    tree_1234->Draw("t2 >> h_t2_s1234");
    tree_1234->Draw("t3 >> h_t3_s1234");
    tree_1234->Draw("t4 >> h_t4_s1234");

    double bin_width = (xmax - xmin) / nbins;
    double nentries_t1 = h_t1_s1234->GetEntries(); 
    double nentries_t2 = h_t2_s1234->GetEntries();
    double nentries_t3 = h_t3_s1234->GetEntries();
    double nentries_t4 = h_t4_s1234->GetEntries();

    h_t1_s1234->Scale(1.0 / (bin_width * nentries_t1));
    h_t2_s1234->Scale(1.0 / (bin_width * nentries_t2));
    h_t3_s1234->Scale(1.0 / (bin_width * nentries_t3));
    h_t4_s1234->Scale(1.0 / (bin_width * nentries_t4));

    // Set half-transparent fill styles
    h_t1_s1234->SetFillStyle(3003); // Transparent fill
    h_t2_s1234->SetFillStyle(3003); // Transparent fill
    h_t3_s1234->SetFillStyle(3003); // Transparent fill
    h_t4_s1234->SetFillStyle(3003); // Transparent fill

    // Set fill colors
    h_t1_s1234->SetFillColor(kOrange);
    h_t2_s1234->SetFillColor(kGreen);
    h_t3_s1234->SetFillColor(kTeal);
    h_t4_s1234->SetFillColor(kBlue);

    // Set line colors
    h_t1_s1234->SetLineColor(kOrange);
    h_t2_s1234->SetLineColor(kGreen);
    h_t3_s1234->SetLineColor(kTeal);
    h_t4_s1234->SetLineColor(kBlue);

    // Create a canvas to draw the histograms
    TCanvas *canvas_s1234 = new TCanvas("canvas_s1234", "Histograms of t1, t2, t3, t4", 1200, 800);
    canvas_s1234->Divide(2, 2); // Divide the canvas into a 2x2 grid of pads

    // Draw histograms on separate pads with Gaussian fit and statistics box
    canvas_s1234->cd(1); // Move to the first pad
    h_t1_s1234->Fit("gaus");
    h_t1_s1234->Draw("hist e");
    gStyle->SetOptFit(1111); // Show fit results
    gStyle->SetOptStat(1111); // Show statistics box

    canvas_s1234->cd(2); // Move to the second pad
    h_t2_s1234->Fit("gaus");
    h_t2_s1234->Draw("hist e");
    gStyle->SetOptFit(1111); // Show fit results
    gStyle->SetOptStat(1111); // Show statistics box

    canvas_s1234->cd(3); // Move to the third pad
    h_t3_s1234->Fit("gaus");
    h_t3_s1234->Draw("hist e");
    gStyle->SetOptFit(1111); // Show fit results
    gStyle->SetOptStat(1111); // Show statistics box

    canvas_s1234->cd(4); // Move to the fourth pad
    h_t4_s1234->Fit("gaus");
    h_t4_s1234->Draw("hist e");
    gStyle->SetOptFit(1111); // Show fit results
    gStyle->SetOptStat(1111); // Show statistics box

    // Save the canvas to a single file
    canvas_s1234->SaveAs("../Dati/Plots/istogrammiTDC_s1-2-3-4.png");

    // Clean up
    /*file->Close();
    delete canvas_s1234;
    delete h_t1_s1234;
    delete h_t2_s1234;
    delete h_t3_s1234;
    delete h_t4_s1234;*/
}
