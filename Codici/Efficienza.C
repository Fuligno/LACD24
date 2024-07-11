//Questo programma calcola il rapporto tra gli integrali di alta e bassa risoluzione
//per ogni canale dalle coincidenze 1-2-3-4, escludendo i valori nulli

#include "Riostream.h"
#include <string>

using namespace std;

void Efficienza() {
    // Nome del file per lo stream di dati
    string file_name = "../Dati/AqqTrigger14data0406ora1728.txt";
    // Inizializza e apri il file di input
    ifstream file_in(file_name);
 
    // Verifica che il file sia aperto
    if (!file_in.is_open()) {
        cout << "Cannot open data file!" << endl;
        return;
    }
    
    // Variabili nel file da leggere
    double q1_l, q1_h, q2_l, q2_h, q3_l, q3_h, q4_l, q4_h, t1, t2, t3, t4, clock;
    
    
    // Contatore per il numero totale di eventi
    int nEventi = 0;
    int eff3 = 0;
    
    // Loop fino alla fine del file, riga per riga
    while (file_in >> q1_l >> q1_h >> q2_l >> q2_h >> q3_l >> q3_h >> q4_l >> q4_h >> t1 >> t2 >> t3 >> t4 >> clock) {

        // Ignora le righe con valori nulli
        if (q1_l == 0 || q4_l == 0 || t1 == 0 || t4 == 0) {
            continue;
        }
        // guarda se lo scintillatore 3 ha visto un evento
        if (q3_h != 0 && t3 != 0){
            eff3 +=1;
        }

        // Incrementa il contatore per ogni riga letta
        ++nEventi;
    }
    // Chiudi il file di input
    file_in.close();
    
    // Calcola le efficienze
    double ef3 = eff3*1. / nEventi ;
    double err = sqrt((ef3*(1-ef3))/nEventi);
    
    // Stampa i risultati su terminale
    cout << fixed << setprecision(4);
    cout << "Efficienza3: " << ef3 << "\t sigma: " << err << endl;

    // Stampa i risultati su file
    ofstream outFile("../Dati/QDC/Efficienze.txt");  
    if (!outFile.is_open()) {
        cerr << "Unable to open output file!" << endl;  
        return;
    }
    // Write the header for the output file
    outFile << setprecision(4) << fixed;   // Set precision to 4 significant digits
    outFile << "ef3\t sigma\n";  
    outFile << ef3 << "\t" << err;                         
    outFile.close();  
}
