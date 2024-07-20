//Questo programma calcola il rapporto tra gli integrali di alta e bassa risoluzione
//per ogni canale dalle coincidenze 1-2-3-4, escludendo i valori nulli

#include "Riostream.h"
#include <string>

using namespace std;

void rapporto() {
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
    
    //Errore sulla media a posteriori
    double variance1=0;
    double variance2=0;
    double variance3=0;
    double variance4=0;
    double sigma1;
    double sigma2;
    double sigma3;
    double sigma4;

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

        //Errore sulla media a posteriori
        variance1 += (q1_h / q1_l - 7.843)*(q1_h / q1_l - 7.843);
        variance2 += (q2_h / q2_l - 7.978)*(q2_h / q2_l - 7.978);
        variance3 += (q3_h / q3_l - 7.885)*(q3_h / q3_l - 7.885);
        variance4 += (q4_h / q4_l - 7.908)*(q4_h / q4_l - 7.908);

        // Incrementa il contatore per ogni riga letta
        ++icand;
    }

    // Chiudi il file di input
    file_in.close();
    
    // Calcola i rapporti medi
    double q1 = q1_sum / icand ;
    double q2 = q2_sum / icand ;
    double q3 = q3_sum / icand ;
    double q4 = q4_sum / icand ;
    
    // Stampa i risultati
    cout << fixed << setprecision(3);
    cout << "Rapporto 1: " << q1 << endl;
    cout << "Rapporto 2: " << q2 << endl;
    cout << "Rapporto 3: " << q3 << endl;
    cout << "Rapporto 4: " << q4 << endl;
    cout << "Numero di candidati: " << icand << endl;

    //Errore sulla media a posteriori
    variance1 = 1.0/icand*variance1;
    variance2 = 1.0/icand*variance2;
    variance3 = 1.0/icand*variance3;
    variance4 = 1.0/icand*variance4;

    sigma1 = sqrt(variance1);
    sigma2 = sqrt(variance2);
    sigma3 = sqrt(variance3);
    sigma4 = sqrt(variance4);
    
    cout << fixed << setprecision(3);
    cout << "Sigma 1: " << sigma1 << endl;
    cout << "Sigma 2: " << sigma2 << endl;
    cout << "Sigma 3: " << sigma3 << endl;
    cout << "Sigma 4: " << sigma4 << endl;
    
    return;
}
