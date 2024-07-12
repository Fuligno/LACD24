#include "Riostream.h"
#include <string>
#include "TH1F.h"
#include "TFile.h"

using namespace std;

void letturaQDC_1234() {
    // Variabili per i rapporti precedentemente calcolari
    double rapp1 = 0, rapp2 = 0, rapp3 = 0, rapp4 = 0;

    // Apro file in cui ho salvato i rapporti
    string file_name = "../Dati/QDC/Rapporti.txt";
    ifstream file_rapp(file_name);
 
    // Salva i rapporti in variabili
    if (file_rapp.is_open()) {
        file_rapp >> rapp1;
        file_rapp >> rapp2;
        file_rapp >> rapp3;
        file_rapp >> rapp4;
    } else {
        cerr << "Unable to open file";
        return;
    }

    // Chiudi il file di input
    file_rapp.close();

    //////////////////////////////////////

    // Nome del file per lo stream di dati
    file_name = "../Dati/AqqTrigger1234data0506ora1744.txt";
    // Inizializza e apri il file di input
    ifstream file_in(file_name);
 
    // Verifica che il file sia aperto
    if (!file_in.is_open()) {
        cout << "Cannot open data file!" << endl;
        return;
    }
    
    // Variabili nel file da leggere
    double q1_l, q1_h, q2_l, q2_h, q3_l, q3_h, q4_l, q4_h, t1, t2, t3, t4, clock;
    
    // Contatore per il numero totale di candidati
    int icand = 0;

    // Creazione degli istogrammi
    int nbins = 1000;
    double xmin = 0, xmax = 3900;
    TH1F *h_q1 = new TH1F("q1_h", "QDC Channel 0", nbins, xmin, xmax);
    TH1F *h_q2 = new TH1F("q2_h", "QDC Channel 1", nbins, xmin, xmax);
    TH1F *h_q3 = new TH1F("q3_h", "QDC Channel 2", nbins, xmin, xmax);
    TH1F *h_q4 = new TH1F("q4_h", "QDC Channel 3", nbins, xmin, xmax);
    
    // Loop fino alla fine del file, riga per riga
    while (file_in >> q1_l >> q1_h >> q2_l >> q2_h >> q3_l >> q3_h >> q4_l >> q4_h >> t1 >> t2 >> t3 >> t4 >> clock) {

        // Ignora le righe con valori nulli
        if (t1 == 0 || t2 == 0 || t3 == 0 || t4 == 0) {
            continue;
        }
        if (q1_h >= 3840) {
            q1_h = q1_l * rapp1;
        }
        if (q2_h >= 3840) {
            q2_h = q2_l * rapp2;
        }
        if (q3_h >= 3840) {
            q3_h = q3_l * rapp3;
        }
        if (q4_h >= 3840) {
            q4_h = q4_l * rapp4;
        }

        // Riempie gli istogrammi
        h_q1->Fill(q1_h);
        h_q2->Fill(q2_h);
        h_q3->Fill(q3_h);
        h_q4->Fill(q4_h);

        // Incrementa il contatore per ogni riga letta
        ++icand;
    }
    
    // Chiudi il file di input
    file_in.close();
    
    // Normalizzazione degli istogrammi
    double bin_width = (xmax - xmin) / nbins;
    h_q1->Scale(1.0 / (icand * bin_width));
    h_q2->Scale(1.0 / (icand * bin_width));
    h_q3->Scale(1.0 / (icand * bin_width));
    h_q4->Scale(1.0 / (icand * bin_width));


    // Opzioni di stile
    gStyle->SetOptFit(1111);
    gStyle->SetOptStat(1111);

    // Definisce Landau per fit
    TF1 *landau_q1 = new TF1("landau_q1", "landau", xmin, xmax);
    TF1 *landau_q2 = new TF1("landau_q2", "landau", xmin, xmax);
    TF1 *landau_q3 = new TF1("landau_q3", "landau", xmin, xmax);
    TF1 *landau_q4 = new TF1("landau_q4", "landau", xmin, xmax);

    // Parametri iniziali
    double initial_MPV = 1000.0;
    double initial_constant = 1.0;

    landau_q1->SetParameter(0, initial_constant);
    landau_q1->SetParameter(1, initial_MPV);

    landau_q2->SetParameter(0, initial_constant);
    landau_q2->SetParameter(1, initial_MPV);

    landau_q3->SetParameter(0, initial_constant);
    landau_q3->SetParameter(1, initial_MPV);

    landau_q4->SetParameter(0, initial_constant);
    landau_q4->SetParameter(1, initial_MPV);

    // Fit degli istogrammi
    h_q1->Fit("landau_q1");
    h_q2->Fit("landau_q2");
    h_q3->Fit("landau_q3");
    h_q4->Fit("landau_q4");
    
    // Disegna istogrammi + fit
    TCanvas *c1 = new TCanvas("c1", "QDC Channel 0", 800, 600);
    h_q1->Draw();
    c1->Update();
    c1->SaveAs("../Dati/QDC/QDC_Channel0.png");

    TCanvas *c2 = new TCanvas("c2", "QDC Channel 1", 800, 600);
    h_q2->Draw();
    c2->Update();
    c2->SaveAs("../Dati/QDC/QDC_Channel1.png");

    TCanvas *c3 = new TCanvas("c3", "QDC Channel 2", 800, 600);
    h_q3->Draw();
    c3->Update();
    c3->SaveAs("../Dati/QDC/QDC_Channel2.png");

    TCanvas *c4 = new TCanvas("c4", "QDC Channel 3", 800, 600);
    h_q4->Draw();
    c4->Update();
    c4->SaveAs("../Dati/QDC/QDC_Channel3.png");

    // Salva gli istogrammi in un file ROOT
    TFile outFile("../Dati/QDC/DataQDC_1234.root", "RECREATE");
    h_q1->Write();
    h_q2->Write();
    h_q3->Write();
    h_q4->Write();
    outFile.Close();

    return;
}
