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
    
    

    for (long i = 0; i < nEntries; ++i) {
        tree->GetEntry(i);

        hqt2->Fill(q2, t2);
        hqt3->Fill(q3, t3);
        hqt4->Fill(q4, t4);
        hq1q2->Fill(q1, q2);
        hq3q4->Fill(q3, q4);
        ht2LR->Fill(t2, t4-t3);
        ht4LR->Fill(t4, t4-t3);
        hqLR->Fill(q4, t4-t3);

    }
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


    // Salva il canvas in un file immagine
    //canvas->SaveAs("histogram2D.png");

    

}
