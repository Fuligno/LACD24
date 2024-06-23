//Questo programma calcola il rapporto tra gli integrali di alta e bassa risoluzione
//per ogni canale dalle coincidenze 1-2-3-4, escludendo i valori nulli

#include "Riostream.h"
#include <string>
#include "TFile.h"
#include "TTree.h"

using namespace std;

void lettura() {
    // Variabili nel file da leggere
    double q1_l, q1_h, q2_l, q2_h, q3_l, q3_h, q4_l, q4_h, t1, t2, t3, t4, clock;
    double q1, q2, q3, q4;

    //valori da prendere dalla macro rapporto.C
    double conv1=8, conv2=8, conv3=8, conv4=8;

    TFile* file_out = new TFile("TreeOut.root", "RECREATE");

    // Dati con il trigger sui 4 canali
	TTree *treeS1234 = new TTree("s1-2-3-4", "QDC & TDC of Trigger with S1+S2+S3+S4");
	treeS1234->Branch("q1", &q1, "q1/D");
    treeS1234->Branch("q2", &q2, "q2/D");
    treeS1234->Branch("q3", &q3, "q3/D");
    treeS1234->Branch("q4", &q4, "q4/D");
    treeS1234->Branch("t1", &t1, "t1/D");
    treeS1234->Branch("t2", &t2, "t2/D");
    treeS1234->Branch("t3", &t3, "t3/D");
    treeS1234->Branch("t4", &t4, "t4/D");
    treeS1234->Branch("clock", &clock, "clock/D");

    // Dati con trigger sui canali 1 e 4, dei soli canali triggerati
    TTree *treeS14_full = new TTree("s1-4_full", "QDC & TDC of Trigger with S1+S4");
	treeS14_full->Branch("q1", &q1, "q1/D");
    treeS14_full->Branch("q4", &q4, "q4/D");
    treeS14_full->Branch("t1", &t1, "t1/D");
    treeS14_full->Branch("t4", &t4, "t4/D");
    treeS14_full->Branch("clock", &clock, "clock/D");

    // Dati con trigger sui canali 1 e 4, con segnale presente anche sui canali non triggerati
    TTree *treeS14 = new TTree("s1-4", "QDC & TDC of Trigger with S1+S4");
	treeS14->Branch("q1", &q1, "q1/D");
    treeS14->Branch("q2", &q2, "q2/D");
    treeS14->Branch("q3", &q3, "q3/D");
    treeS14->Branch("q4", &q4, "q4/D");
    treeS14->Branch("t1", &t1, "t1/D");
    treeS14->Branch("t2", &t2, "t2/D");
    treeS14->Branch("t3", &t3, "t3/D");
    treeS14->Branch("t4", &t4, "t4/D");
    treeS14->Branch("clock", &clock, "clock/D");

    // Dati con trigger sul canale 3, del solo canale triggerato
    TTree *treeS3_full = new TTree("s3_full", "QDC & TDC of Trigger with S3");
	treeS3_full->Branch("q3", &q3, "q3/D");
    treeS3_full->Branch("t3", &t3, "t3/D");
    treeS3_full->Branch("clock", &clock, "clock/D");

    // Dati con trigger sul canale 3, con segnale presente anche sui canali non triggerati
    TTree *treeS3 = new TTree("s3", "QDC & TDC of Trigger with S3");
	treeS3->Branch("q1", &q1, "q1/D");
    treeS3->Branch("q2", &q2, "q2/D");
    treeS3->Branch("q3", &q3, "q3/D");
    treeS3->Branch("q4", &q4, "q4/D");
    treeS3->Branch("t1", &t1, "t1/D");
    treeS3->Branch("t2", &t2, "t2/D");
    treeS3->Branch("t3", &t3, "t3/D");
    treeS3->Branch("t4", &t4, "t4/D");
    treeS3->Branch("clock", &clock, "clock/D");

    // Nome del file per lo stream di dati Trigger S3
    string file3 = "../Dati/AqqTrigger3data0306ora1747.txt";
    // Inizializza e apri il file di input
    ifstream file_in3(file3);

    // Nome del file per lo stream di dati Trigger S1+S4
    string file2 = "../Dati/AqqTrigger14data0406ora1728.txt";
    // Inizializza e apri il file di input
    ifstream file_in2(file2);

    // Nome del file per lo stream di dati Trigger S1+S2+S3+S4
    string file1 = "../Dati/AqqTrigger1234data0506ora1744.txt";
    // Inizializza e apri il file di input
    ifstream file_in1(file1);
 
    // Verifica che i file siano aperto
    if (!file_in3.is_open()) {
        cout << "Cannot open data file S3!" << endl;
        return;
    }
    if (!file_in2.is_open()) {
        cout << "Cannot open data file S1-4!" << endl;
        return;
    }
    if (!file_in1.is_open()) {
        cout << "Cannot open data file S1-2-3-4!" << endl;
        return;
    }
    
    // Loop fino alla fine del file, riga per riga
    while (file_in1 >> q1_l >> q1_h >> q2_l >> q2_h >> q3_l >> q3_h >> q4_l >> q4_h >> t1 >> t2 >> t3 >> t4 >> clock) {

        // Ignora le righe con valori nulli
        if (q1_h == 0 || q2_h == 0  || q3_h == 0 || q4_h == 0 || t1 == 0 || t2 == 0 || t3 == 0 || t4 == 0) {
            continue;
        }

        // questi if andrebbero messi in una macro carina
        if(q1_h > 3839){
            q1 = q1_l * conv1;
        } else {
            q1 = q1_h;
        }
        if(q2_h > 3839){
            q2 = q2_l * conv2;
        } else {
            q2 = q2_h;
        }
        if(q3_h > 3839){
            q3 = q3_l * conv3;
        } else {
            q3 = q3_h;
        }
        if(q4_h > 3839){
            q4 = q4_l * conv4;
        } else {
            q4 = q4_h;
        }
        // fare macro per convertire i valori da file in valori in carica e tempo prima di scriverli
        treeS1234->Fill();
    }
    // Chiudi il file di input
    file_in1.close();
    
    // Loop fino alla fine del file, riga per riga
    while (file_in2 >> q1_l >> q1_h >> q2_l >> q2_h >> q3_l >> q3_h >> q4_l >> q4_h >> t1 >> t2 >> t3 >> t4 >> clock) {

        // Ignora le righe con valori nulli
        if (q1_h == 0 || q4_h == 0 || t1 == 0 || t4 == 0) {
            continue;
        }

        // questi if andrebbero messi in una macro carina
        if(q1_h > 3839){
            q1 = q1_l * conv1;
        } else {
            q1 = q1_h;
        }
        if(q4_h > 3839){
            q4 = q4_l * conv4;
        } else {
            q4 = q4_h;
        }
        // fare macro per convertire i valori da file in valori in carica e tempo prima di scriverli
        treeS14_full->Fill();
        // guarda se lo scintillatore 2 ha visto un evento
        if (q2_h == 0 || q3_h == 0 || t2 == 0 || t3 == 0) {
            continue;
        }
        // questi if andrebbero messi in una macro carina
        if(q2_h > 3839){
            q2 = q2_l * conv2;
        } else {
            q2 = q2_h;
        }
        if(q3_h > 3839){
            q3 = q3_l * conv3;
        } else {
            q3 = q3_h;
        }
        // fare macro per convertire i valori da file in valori in carica e tempo prima di scriverli
        treeS14->Fill();
    }
    // Chiudi il file di input
    file_in2.close();
    
    // Loop fino alla fine del file, riga per riga
    while (file_in3 >> q1_l >> q1_h >> q2_l >> q2_h >> q3_l >> q3_h >> q4_l >> q4_h >> t1 >> t2 >> t3 >> t4 >> clock) {

        // Ignora le righe con valori nulli
        if (q3_h == 0 || t3 == 0) {
            continue;
        }

        // questi if andrebbero messi in una macro carina
        if(q3_h > 3839){
            q3 = q3_l * conv3;
        } else {
            q3 = q3_h;
        }
        
        // fare macro per convertire i valori da file in valori in carica e tempo prima di scriverli
        treeS3_full->Fill();
        // guarda se lo scintillatore 2 ha visto un evento
        if (q1_h == 0 || q2_h == 0 || q4_h == 0 || t1 == 0 || t2 == 0 || t4 == 0) {
            continue;
        }
        // questi if andrebbero messi in una macro carina
        if(q1_h > 3839){
            q1 = q1_l * conv1;
        } else {
            q1 = q1_h;
        }
        if(q2_h > 3839){
            q2 = q2_l * conv2;
        } else {
            q2 = q2_h;
        }
        if(q4_h > 3839){
            q4 = q4_l * conv4;
        } else {
            q4 = q4_h;
        }
        // fare macro per convertire i valori da file in valori in carica e tempo prima di scriverli
        treeS3->Fill();
    }
    // Chiudi il file di input
    file_in3.close();

    
    // Stampa i risultati
    treeS1234->Write();
    treeS14_full->Write();
    treeS14->Write();
    treeS3_full->Write();
    treeS3->Write();
    delete file_out;
    
    return;
}
