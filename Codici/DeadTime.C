//Questo programma calcola il rapporto tra gli integrali di alta e bassa risoluzione
//per ogni canale dalle coincidenze 1-2-3-4, escludendo i valori nulli

#include "Riostream.h"
#include <string>

using namespace std;
double tauD(double &n, double &m, double &T){
    return (n-m)*T/(n*m);
}
double tauErr(double &tau, double &N, double &T){
    return sqrt(tau*T/(N*N));
}

void DeadTime() {
    // Nome del file per lo stream di dati
    double N1, M1 = 0, N2, M2 = 0, N3, M3 = 0;
    double T1, T2, T3;
    double eff1 = 0, eff2 = 0, eff3 = 0;
    double tau1, tau2, tau3;
    double tau_err1, tau_err2, tau_err3; 
    double T=3600000;
    double m3 = 83002, n3 = 199437; 
    double m2 = 22171, n2 = 23425;
    double m1 = 18509, n1 = 19190;

    tau1 = tauD(n1, m1, T);
    tau2 = tauD(n2, m2, T);
    tau3 = tauD(n3, m3, T);
    tau_err1 = tauErr(tau1, n1, T);
    tau_err2 = tauErr(tau2, n2, T);
    tau_err3 = tauErr(tau3, n3, T);




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

    // Nome del file per lo stream di dati Trigger S1+S2+S3+S4
    string file4 = "../Dati/coincidenzeSoftware_trigger3.txt";
    // Inizializza e apri il file di input
    ifstream file_in4(file4);
    if (!file_in4.is_open()) {
        cout << "Cannot open data file S1-2-3-4!" << endl;
        return;
    }

 
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
    
    // Variabili nel file da leggere
    double q1_l, q1_h, q2_l, q2_h, q3_l, q3_h, q4_l, q4_h, t1, t2, t3, t4, clock;
    double nEv1 = 0;
    // Loop fino alla fine del file, riga per riga
    while (file_in1 >> q1_l >> q1_h >> q2_l >> q2_h >> q3_l >> q3_h >> q4_l >> q4_h >> t1 >> t2 >> t3 >> t4 >> clock) {
        nEv1 +=1;
        if(nEv1== 1){T = clock;}
        T1 = clock - T;
        // Ignora le righe con valori nulli
        if (q1_h == 0 || q2_h == 0  || q3_h == 0 || q4_h == 0 || t1 == 0 || t2 == 0 || t3 == 0 || t4 == 0) {
            continue;
        }
        M1 +=1;
    }
    eff1 = m1/n1;
    // Chiudi il file di input
    file_in1.close();
    double nEv2 = 0;
     // Loop fino alla fine del file, riga per riga
    while (file_in2 >> q1_l >> q1_h >> q2_l >> q2_h >> q3_l >> q3_h >> q4_l >> q4_h >> t1 >> t2 >> t3 >> t4 >> clock) {
        nEv2 +=1;
        if(nEv2 == 1){T = clock;}
        T2 = clock - T;
        // Ignora le righe con valori nulli
        if (q1_h == 0 || q2_h == 0  || q3_h == 0 || q4_h == 0 || t1 == 0 || t2 == 0 || t3 == 0 || t4 == 0) {
            continue;
        }
        M2 +=1;
    }
    eff2 = m2/n2;
    // Chiudi il file di input
    file_in2.close();
    double nEv3 = 0;
     // Loop fino alla fine del file, riga per riga
    while (file_in3 >> q1_l >> q1_h >> q2_l >> q2_h >> q3_l >> q3_h >> q4_l >> q4_h >> t1 >> t2 >> t3 >> t4 >> clock) {
        nEv3 +=1;
        if(nEv3 == 1){T = clock;}
        T3 = clock - T;
        // Ignora le righe con valori nulli
        if (q1_h == 0 || q2_h == 0  || q3_h == 0 || q4_h == 0 || t1 == 0 || t2 == 0 || t3 == 0 || t4 == 0) {
            continue;
        }
            M3 +=1;
            
    }
    eff3 = m3/n3;
    // Chiudi il file di input
    file_in3.close();
    double f=0;
    while (file_in4 >> q1_l >> q1_h >> q2_l >> q2_h >> q3_l >> q3_h >> q4_l >> q4_h >> t1 >> t2 >> t3 >> t4 >> clock) {
        // Ignora le righe con valori nulli
        if (q1_h == 0 || q2_h == 0  || q3_h == 0 || q4_h == 0 || t1 == 0 || t2 == 0 || t3 == 0 || t4 == 0) {
            continue;
        }
        f +=1;
            
    }
    f = f/8973;
    double err = 32281./34638;
    N1 = M1/eff1;
    N2 = M2/eff2;
    N3 = M3*f*err/eff3;
    
    
    // Stampa i risultati su terminale
    cout << fixed << setprecision(4);
    cout << "tau_D s1234:\t" << tau1 << "\t sigma: " << tau_err1 << endl;
    cout << "tau_D s24:\t" << tau2 << "\t sigma: " << tau_err2 << endl;
    cout << "tau_D s3:\t" << tau3 << "\t sigma: " << tau_err3 << endl;
    cout << "N s1234:\t" << N1 << "\t" << N1/T1 << "\t" << M1 << endl;
    cout << "N s24:\t\t" << N2 << "\t" << N2/T2 <<  "\t" << M2 << endl;
    cout << "N s3:\t\t" << N3 << "\t" << N3/T3 <<  "\t" << M3 << endl;
    cout << "f: " << f << "\t" << M1/nEv1 << endl;
    cout << "epsilon: " << err << endl;
    cout << "eff1: " << eff1 << endl;
    cout << "eff2: " << eff2 << endl;
    cout << "eff3: " << eff3 << endl;


                      
}
