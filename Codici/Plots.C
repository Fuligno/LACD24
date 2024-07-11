#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TPad.h>

void Plots() {
    // Open the ROOT file
    TFile *file = TFile::Open("TreeOut.root");

    // Check if the file was opened successfully
    if (!file || file->IsZombie()) {
        printf("Error: Unable to open file TreeOut.root\n");
        return;
    }

    // Retrieve the TTree named "s1234"
    TTree *tree = (TTree*)file->Get("s1-2-3-4");
    if (!tree) {
        printf("Error: Tree s1-2-3-4 not found\n");
        file->Close();
        return;
    }

    // Create a h_t3_s1234ogram for the branch "t3"
    // Specify the number of bins (1000), and range [600, 1300]
    TH1F *h_t1_s1234 = new TH1F("h_t1_s1234", "t1_s1234", 1000, 970, 985);
    TH1F *h_t2_s1234 = new TH1F("h_t2_s1234", "t2_s1234", 1000, 550, 1250);
    TH1F *h_t3_s1234 = new TH1F("h_t3_s1234", "t3_s1234", 1000, 650, 1300);
    TH1F *h_t4_s1234 = new TH1F("h_t4_s1234", "t4_s1234", 1000, 400, 1050);

    // Fill the h_t3_s1234ogram with data from the "t3" branch
    tree->Draw("t1 >> h_t1_s1234");
    tree->Draw("t2 >> h_t2_s1234");
    tree->Draw("t3 >> h_t3_s1234");
    tree->Draw("t4 >> h_t4_s1234");

    // Create a canvas to draw the h_t3_s1234ogram
    TCanvas *canvas_s1234 = new TCanvas("canvas", "histograms_s1234", 800, 600);
    /*canvas_s1234->Divide(1,2);
    TPad *upperPad = (TPad*)canvas_s1234->cd(1);
    upperPad->Divide(2,1);
    TPad *lowerPad = (TPad*)canvas_s1234->cd(1);
    lowerPad->Divide(2,1);*/

    // Draw the histograms on the canvas
    //canvas_s1234->cd();
    //upperPad->cd(1);
    h_t1_s1234->Draw("hist");
    h_t1_s1234->SetFillColor(44);
    h_t1_s1234->Fit("gaus");
    canvas_s1234->SaveAs("../Dati/histogram_t1_s1234.png");

    h_t2_s1234->Draw("hist");
    h_t2_s1234->SetFillColor(44);
    h_t2_s1234->Fit("gaus");
    canvas_s1234->SaveAs("../Dati/histogram_t2_s1234.png");

    h_t3_s1234->Draw("hist");
    h_t3_s1234->SetFillColor(44);
    h_t3_s1234->Fit("gaus");
    canvas_s1234->SaveAs("../Dati/histogram_t3_s1234.png");
    
    h_t4_s1234->Draw("hist");
    h_t4_s1234->SetFillColor(44);
    h_t4_s1234->Fit("gaus");
    canvas_s1234->SaveAs("../Dati/histogram_t4_s1234.png");

    // Clean up
    file->Close();
    delete canvas_s1234;
    delete h_t1_s1234;
    delete h_t2_s1234;
    delete h_t3_s1234;
    delete h_t4_s1234;
}
