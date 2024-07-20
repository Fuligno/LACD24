#include <TFile.h>
#include <TTree.h>
#include <TH2D.h>
#include <TCanvas.h>

void correlazioni(){
    // Apri il file ROOT
    TFile *file = TFile::Open("TreeOut.root");
    if (!file || file->IsZombie()) {
        std::cerr << "Errore nell'aprire il file tree.root" << std::endl;
        return;
    }
    file->ls();

    // Recupera il TTree dal file
    TTree *tree;
    file->GetObject("s1-2-3-4;1", tree);
    if (!tree) {
        std::cerr << "Errore nel recuperare il TTree dal file" << std::endl;
        file->Close();
        return;
    }

    // Variabili da leggere dal TTree
    double q1, q2, q3, q4, t1, t2, t3, t4, clock;
    tree->SetBranchAddress("q1", &q1);
    tree->SetBranchAddress("q2", &q2);
    tree->SetBranchAddress("q3", &q3);
    tree->SetBranchAddress("q4", &q4);
    tree->SetBranchAddress("t1", &t1);
    tree->SetBranchAddress("t2", &t2);
    tree->SetBranchAddress("t3", &t3);
    tree->SetBranchAddress("t4", &t4);
    tree->SetBranchAddress("clock", &clock);


    // Crea un istogramma 2D

    // Loop sugli eventi del TTree
    long nEntries = tree->GetEntries();
    std::cout << "Numero di voci: " << nEntries << std::endl;

    TH2F *hqt2 = new TH2F("hqt2", "q2 vs t2", 580, 200, 6000, 100, 400, 1400);
    TH2F *hqt3 = new TH2F("hqt3", "q3 vs t3", 580, 200, 6000, 100, 400, 1400);
    TH2F *hqt4 = new TH2F("hqt4", "q4 vs t4", 580, 200, 6000, 100, 400, 1400);
    TH2F *hq1q2 = new TH2F("hq1q2", "q1 vs q2", 580, 200, 6000, 580, 200, 6000);
    TH2F *hq3q4 = new TH2F("hq3q4", "q3 vs q4", 580, 200, 6000, 580, 200, 6000);
    TH2F *ht2LR = new TH2F("ht2LR", "t2 vs t4-t3", 100, 400, 1400, 100, -500, 500);
    TH2F *ht4LR = new TH2F("ht4LR", "t4 vs t4-t3", 100, 400, 1400, 100, -500, 500);
    TH2F *hqLR = new TH2F("hqLR", "q4 vs t4-t3", 580, 200, 6000, 100, -500, 500);

    
    TGraph *grq1 = new TGraph(nEntries);
    TGraph *grq2 = new TGraph(nEntries);
    TGraph *grq3 = new TGraph(nEntries);
    TGraph *grq4 = new TGraph(nEntries);
    for (long i = 0; i < nEntries; ++i) {
        tree->GetEntry(i);
        grq1->SetPoint(i, clock, q1);
        grq2->SetPoint(i, clock, q2);
        grq3->SetPoint(i, clock, q3);
        grq4->SetPoint(i, clock, q4);
        hqt2->Fill(q2, t2);
        hqt3->Fill(q3, t3);
        hqt4->Fill(q4, t4);
        hq1q2->Fill(q1, q2);
        hq3q4->Fill(q3, q4);
        ht2LR->Fill(t2, t4-t3);
        ht4LR->Fill(t4, t4-t3);
        hqLR->Fill(q4, t4-t3);

    }

    double yMin = 200;  // Limite minimo dell'asse Y
    double yMax = 10000; // Limite massimo dell'asse Y
    double xmin = 1000;
    double xmax = 5700000;
    grq1->SetMinimum(yMin);
    grq1->SetMaximum(yMax);
    grq2->SetMinimum(yMin);
    grq2->SetMaximum(yMax);
    grq3->SetMinimum(yMin);
    grq3->SetMaximum(yMax);
    grq4->SetMinimum(yMin);
    grq4->SetMaximum(yMax);
    grq1->GetXaxis()->SetLimits(xmin, xmax);
    grq2->GetXaxis()->SetLimits(xmin, xmax);
    grq3->GetXaxis()->SetLimits(xmin, xmax);
    grq4->GetXaxis()->SetLimits(xmin, xmax);
    grq1->SetLineColor(kRed+1.);
    grq2->SetLineColor(kGreen+1.);
    grq3->SetLineColor(kCyan+1.);
    grq4->SetLineColor(kViolet+1.);


    std::cout << "Istogramma riempito" << std::endl;

    // Crea un canvas per disegnare l'istogramma
    TCanvas *c1 = new TCanvas("c1", "Istogramma 2D", 800, 600);
    c1->cd();
    hqt2->Draw("colz");

    TCanvas *c2 = new TCanvas("c2", "Istogramma 2D", 800, 600);
    c2->cd();
    hqt3->Draw("colz");

    TCanvas *c3 = new TCanvas("c3", "Istogramma 2D", 800, 600);
    c3->cd();
    hqt4->Draw("colz");

    TCanvas *c4 = new TCanvas("c4", "Istogramma 2D", 800, 600);
    c4->cd();
    hqLR->Draw("colz");

    TCanvas *c5 = new TCanvas("c5", "Istogramma 2D", 800, 600);
    c5->cd();
    ht2LR->Draw("colz");

    TCanvas *c6 = new TCanvas("c6", "Istogramma 2D", 800, 600);
    c6->cd();
    ht4LR->Draw("colz");

    TCanvas *c7 = new TCanvas("c7", "Istogramma 2D", 800, 600);
    c7->cd();
    hq1q2->Draw("colz");

    TCanvas *c8 = new TCanvas("c8", "Istogramma 2D", 800, 600);
    c8->cd();
    hq3q4->Draw("colz");


    TCanvas *c9 = new TCanvas("c9", "4 Graphs in One Canvas", 800, 800);
    c9->Divide(1, 4,0,0);
    for (int i = 1; i <= 4; ++i) {
        c9->cd(i);
        if (i == 1) {
            grq1->Draw("APL");
            grq1->SetTitle("");
            grq1->GetYaxis()->SetTitle("QDC"); 
            grq1->GetXaxis()->SetLabelSize(0); // Rimuovere le etichette dell'asse X per i pad superiori
            grq1->GetXaxis()->SetTitle("");
            c9->cd(i)->SetGrid();
        } else if (i == 2) {
            grq2->Draw("APL");
            grq2->SetTitle("");
            grq2->GetXaxis()->SetLabelSize(0); // Rimuovere le etichette dell'asse X per i pad superiori
            grq2->GetXaxis()->SetTitle("");
            c9->cd(i)->SetGrid();
        } else if (i == 3) {
            grq3->Draw("APL");
            grq3->SetTitle("");
            grq3->GetXaxis()->SetLabelSize(0); // Rimuovere le etichette dell'asse X per i pad superiori
            grq3->GetXaxis()->SetTitle("");
            c9->cd(i)->SetGrid();
        } else if (i == 4) {
            grq4->Draw("APL");
            grq4->SetTitle("");
            grq4->GetXaxis()->SetTitle("Time [s]");
            c9->cd(i)->SetGrid();
        }

    }

    // Aggiornare il canvas
    c9->Update();

    // Salva il canvas in un file immagine
    //canvas->SaveAs("histogram2D.png");

    

}
