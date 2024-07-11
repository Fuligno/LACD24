//Questo programma calcola il rapporto tra gli integrali di alta e bassa risoluzione
//per ogni canale dalle coincidenze 1-2-3-4, escludendo i valori nulli

#include "Riostream.h"
#include <string>
#include "TFile.h"
#include "TTree.h"
#include "Rapporto.C"

using namespace std;

double carica(double &ql, double &qh, double &conv){
    // questa macro prende il valore ad alta risoluzione se valido, altrimenti
    // converte quello a bassa in alta usando l'opportuno fattore di conversione
    double q;
    if(qh<3840){
        q = qh;
    } else {
        q = ql * conv;
    }
    return q;
}

void lettura() {

//------------------------------Dichiarazione variabili e apertura file------------------------------------
    // Variabili nel file da leggere
    double q1_l, q1_h, q2_l, q2_h, q3_l, q3_h, q4_l, q4_h, t1, t2, t3, t4, clock;
    double q1, q2, q3, q4;



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

    // Dati con trigger sui canali 2 e 4, dei soli canali triggerati
    TTree *treeS24_full = new TTree("s2-4_full", "QDC & TDC of Trigger with S2+S4");
	treeS24_full->Branch("q2", &q2, "q2/D");
    treeS24_full->Branch("q4", &q4, "q4/D");
    treeS24_full->Branch("t2", &t2, "t2/D");
    treeS24_full->Branch("t4", &t4, "t4/D");
    treeS24_full->Branch("clock", &clock, "clock/D");

    // Dati con trigger sui canali 2 e 4, con segnale presente anche sui canali non triggerati
    TTree *treeS24 = new TTree("s2-4", "QDC & TDC of Trigger with S2+S4");
	treeS24->Branch("q1", &q1, "q1/D");
    treeS24->Branch("q2", &q2, "q2/D");
    treeS24->Branch("q3", &q3, "q3/D");
    treeS24->Branch("q4", &q4, "q4/D");
    treeS24->Branch("t1", &t1, "t1/D");
    treeS24->Branch("t2", &t2, "t2/D");
    treeS24->Branch("t3", &t3, "t3/D");
    treeS24->Branch("t4", &t4, "t4/D");
    treeS24->Branch("clock", &clock, "clock/D");

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

    // Nome del file per lo stream di dati Trigger S2+S4
    string file2 = "../Dati/AqqTrigger14data0406ora1728.txt";
    // Inizializza e apri il file di input
    ifstream file_in2(file2);

    // Nome del file per lo stream di dati Trigger S1+S2+S3+S4
    string file1 = "../Dati/AqqTrigger1234data0506ora1744.txt";
    // Inizializza e apri il file di input
    ifstream file_in1(file1);
 
    // Verifica che i file siano aperti
    if (!file_in3.is_open()) {
        cout << "Cannot open data file S3!" << endl;
        return;
    }
    if (!file_in2.is_open()) {
        cout << "Cannot open data file S2-4!" << endl;
        return;
    }
    if (!file_in1.is_open()) {
        cout << "Cannot open data file S1-2-3-4!" << endl;
        return;
    }
    
//------------------------------Conversione bassa -> alta risoluzione della QDC------------------------------------
    //valori da prendere dalla macro Rapporto.C
    double conv1, conv2, conv3, conv4;
    Rapporto(conv2, conv1, conv3, conv4);


/*
Nota: sia per il rapporto che per tutti i cicli di lettura dei file, c'è sempre il 2 prima dell'1, così da avere
degli output coerenti con i numeri dei detector: infatti il segnale del detector 1 e 2 erano invertiti sui cavi 
2 e 1 durante la presa dati e quindi nelle colonne del file c'è sempre prima la colonna relativa al segnale 2 e 
poi la colonna del segnale 1. Questo problema non è stato identificato prima poiché era un problema dei cavi
alla sorgente.
*/

//------------------------------Lettura file S1+2+3+4------------------------------------
    // Loop fino alla fine del file, riga per riga
    while (file_in1 >> q2_l >> q2_h >> q1_l >> q1_h >> q3_l >> q3_h >> q4_l >> q4_h >> t2 >> t1 >> t3 >> t4 >> clock) {
        // Ignora le righe con valori nulli
        if (q1_h == 0 || q2_h == 0  || q3_h == 0 || q4_h == 0 || t1 == 0 || t2 == 0 || t3 == 0 || t4 == 0) {
            continue;
        }
        q1 = carica(q1_l, q1_h, conv1);
        q2 = carica(q2_l, q2_h, conv2);
        q3 = carica(q3_l, q3_h, conv3);
        q4 = carica(q4_l, q4_h, conv4);

        treeS1234->Fill();
    }
    // Chiudi il file di input
    file_in1.close();

//------------------------------Lettura file S2+4------------------------------------    
    // Loop fino alla fine del file, riga per riga
    while (file_in2 >> q2_l >> q2_h >> q1_l >> q1_h >> q3_l >> q3_h >> q4_l >> q4_h >> t2 >> t1 >> t3 >> t4 >> clock) {
        // Ignora le righe con valori nulli
        if (q2_h == 0 || q4_h == 0 || t2 == 0 || t4 == 0) {
            continue;
        }
        q2 = carica(q2_l, q2_h, conv2);
        q4 = carica(q4_l, q4_h, conv4);

        treeS24_full->Fill();

        // guarda se gli scintillatori 1 e 3 hanno visto un evento
        if (q1_h == 0 || q3_h == 0 || t1 == 0 || t3 == 0) {
            continue;
        }
        q1 = carica(q1_l, q1_h, conv1);
        q3 = carica(q3_l, q3_h, conv3);
 
        treeS24->Fill();
    }
    // Chiudi il file di input
    file_in2.close();

//------------------------------Lettura file S3------------------------------------   
    // Loop fino alla fine del file, riga per riga
    while (file_in3 >> q2_l >> q2_h >> q1_l >> q1_h >> q3_l >> q3_h >> q4_l >> q4_h >> t2 >> t1 >> t3 >> t4 >> clock) {

        // Ignora le righe con valori nulli
        if (q3_h == 0 || t3 == 0) {
            continue;
        }
        q3 = carica(q3_l, q3_h, conv3);

        treeS3_full->Fill();
        // guarda se gli scintillatori 1, 3 e 4 hanno visto un evento
        if (q1_h == 0 || q2_h == 0 || q4_h == 0 || t1 == 0 || t2 == 0 || t4 == 0) {
            continue;
        }
        q1 = carica(q1_l, q1_h, conv1);
        q2 = carica(q2_l, q2_h, conv2);
        q4 = carica(q4_l, q4_h, conv4);

        treeS3->Fill();
    }
    // Chiudi il file di input
    file_in3.close();

//------------------------------Salvataggio su file dei risultati------------------------------------       
    treeS1234->Write();
    treeS24_full->Write();
    treeS24->Write();
    treeS3_full->Write();
    treeS3->Write();
    delete file_out;
    
}
