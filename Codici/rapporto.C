#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

void Rapporto(double &r1, double &r2, double &r3, double &r4) {
    // Nome del file per lo stream di dati
    string file_name = "../Dati/AqqTrigger1234data0506ora1744.txt";
    
    // Inizializza e apri il file di input
    ifstream file_in(file_name);
 
    // Verifica che il file sia aperto
    if (!file_in.is_open()) {
        cout << "Cannot open data file!" << endl;
        return;
    }
    
    // Variabili nel file da leggere
    double q1_l, q1_h, q2_l, q2_h, q3_l, q3_h, q4_l, q4_h, t1, t2, t3, t4, clock;
    
    // Variabili per il rapporto
    double q1_sum = 0, q2_sum = 0, q3_sum = 0, q4_sum = 0;
    
    // Contatore per il numero totale di candidati
    int icand = 0;
    
    // Loop fino alla fine del file, riga per riga
    while (file_in >> q1_l >> q1_h >> q2_l >> q2_h >> q3_l >> q3_h >> q4_l >> q4_h >> t1 >> t2 >> t3 >> t4 >> clock) {
        // Ignora le righe con valori nulli
        if (q1_l == 0 || q2_l == 0 || q3_l == 0 || q4_l == 0 || t1 == 0 || t2 == 0 || t3 == 0 || t4 == 0) {
            continue;
        }

        // Aggiungi ai sommatori
        q1_sum += q1_h / q1_l;
        q2_sum += q2_h / q2_l;
        q3_sum += q3_h / q3_l;
        q4_sum += q4_h / q4_l;

        // Incrementa il contatore per ogni riga letta
        ++icand;
    }
    
    // Chiudi il file di input
    file_in.close();
    
    // Verifica se icand è diverso da zero per evitare divisioni per zero
    if (icand > 0) {
        // Calcola i rapporti medi
        r1 = q1_sum / icand;
        r2 = q2_sum / icand;
        r3 = q3_sum / icand;
        r4 = q4_sum / icand;
    } else {
        // Imposta a zero i rapporti se non ci sono dati validi
        r1 = r2 = r3 = r4 = 0;
    }
    
    // Stampa i risultati
    cout << fixed << setprecision(3);
    cout << "Rapporto1: " << r1 << endl;
    cout << "Rapporto2: " << r2 << endl;
    cout << "Rapporto3: " << r3 << endl;
    cout << "Rapporto4: " << r4 << endl;
    cout << "Numero_candidati: " << icand << endl;
}
