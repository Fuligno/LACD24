#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TF1.h>

void istogrammiTDC_s24() {
    // Open the ROOT file
    TFile *file = TFile::Open("TreeOut.root");

    // Check if the file was opened successfully
    if (!file || file->IsZombie()) {
        printf("Error: Unable to open file TreeOut.root\n");
        return;
    }

    // Retrieve the TTree named "s2-4"
    TTree *tree_24 = (TTree*)file->Get("s2-4");
    if (!tree_24) {
        printf("Error: Tree not found\n");
        file->Close();
        return;
    }

    // Create histograms for each branch
    int nbins = 1000;
    double xmin = 450, xmax = 1450;
    TH1F *h_t1_s24 = new TH1F("h_t1_s24", "t1_s24", nbins, xmin, xmax);
    TH1F *h_t2_s24 = new TH1F("h_t2_s24", "t2_s24", nbins, xmin, xmax);
    TH1F *h_t3_s24 = new TH1F("h_t3_s24", "t3_s24", nbins, xmin, xmax);
    TH1F *h_t4_s24 = new TH1F("h_t4_s24", "t4_s24", nbins, xmin, xmax);

    // Fill the histograms with data from the TTree
    tree_24->Draw("t1 >> h_t1_s24");
    tree_24->Draw("t2 >> h_t2_s24");
    tree_24->Draw("t3 >> h_t3_s24");
    tree_24->Draw("t4 >> h_t4_s24");

    double bin_width = (xmax - xmin) / nbins;
    double nentries_t1 = h_t1_s24->GetEntries(); 
    double nentries_t2 = h_t2_s24->GetEntries();
    double nentries_t3 = h_t3_s24->GetEntries();
    double nentries_t4 = h_t4_s24->GetEntries();

    h_t1_s24->Scale(1.0 / (bin_width * nentries_t1));
    h_t2_s24->Scale(1.0 / (bin_width * nentries_t2));
    h_t3_s24->Scale(1.0 / (bin_width * nentries_t3));
    h_t4_s24->Scale(1.0 / (bin_width * nentries_t4));

    // Set half-transparent fill styles
    h_t1_s24->SetFillStyle(3003); // Transparent fill
    h_t2_s24->SetFillStyle(3003); // Transparent fill
    h_t3_s24->SetFillStyle(3003); // Transparent fill
    h_t4_s24->SetFillStyle(3003); // Transparent fill

    // Set fill colors
    h_t1_s24->SetFillColor(kOrange);
    h_t2_s24->SetFillColor(kGreen);
    h_t3_s24->SetFillColor(kTeal);
    h_t4_s24->SetFillColor(kBlue);

    // Set line colors
    h_t1_s24->SetLineColor(kOrange);
    h_t2_s24->SetLineColor(kGreen);
    h_t3_s24->SetLineColor(kTeal);
    h_t4_s24->SetLineColor(kBlue);

    // Create a canvas to draw the histograms
    TCanvas *canvas_s24 = new TCanvas("canvas_s24", "Histograms of t1, t2, t3, t4", 1200, 800);
    canvas_s24->Divide(2, 2); // Divide the canvas into a 2x2 grid of pads

    // Set global style options
    gStyle->SetOptFit(1111); // Show fit results
    gStyle->SetOptStat(1111); // Show statistics box

    // Draw histograms on separate pads
    canvas_s24->cd(1); // Move to the first pad
    h_t1_s24->Fit("gaus");
    h_t1_s24->Draw("hist e");

    canvas_s24->cd(2); // Move to the second pad
    h_t2_s24->Fit("gaus");
    h_t2_s24->Draw("hist e");

    canvas_s24->cd(3); // Move to the third pad
    h_t3_s24->Fit("gaus");
    h_t3_s24->Draw("hist e");

    canvas_s24->cd(4); // Move to the fourth pad
    h_t4_s24->Fit("gaus");
    h_t4_s24->Draw("hist e");

    // Save the canvas to a single file
    canvas_s24->SaveAs("../Dati/Plots/istogrammiTDC_s2-4.png");

    // Clean up
    /*file->Close();
    delete canvas_s24;
    delete h_t1_s24;
    delete h_t2_s24;
    delete h_t3_s24;
    delete h_t4_s24;*/
}
