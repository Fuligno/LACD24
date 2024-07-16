#include "Riostream.h"
#include <string>

using namespace std;

double q1max, q2max, q3max, q4max;		// valori massimi della QDC a bassa risoluzione per ciascun canale

void maxq_l() {
	double q1_l, q1_h, q2_l, q2_h, q3_l, q3_h, q4_l, q4_h, t1, t2, t3, t4, clock;
	string dati = "../Dati/AqqTrigger3data0306ora1747.txt";	// leggo il file della coincidenza a 4
	ifstream file_in(dati);
     if (file_in.is_open()) {
         cout << "Data file is open!" << endl;
     }
	q1max = 0.0;	// inizializzo le variabili
	q2max = 0.0;
	q3max = 0.0;
	q4max = 0.0;
	
     while (file_in >> q2_l >> q2_h >> q1_l >> q1_h >> q3_l >> q3_h >> q4_l >> q4_h >> t2 >> t1 >> t3 >> t4 >> clock) {
         // Ignora le righe con valori nulli
         if (q1_h == 0 || q2_h == 0  || q3_h == 0 || q4_h == 0 || t1 == 0 || t2 == 0 || t3 == 0 || t4 == 0) {
             continue;
         }
	    if (q1_l>q1max){	// aggiorno il valore massimo man mano che leggo il file
		    q1max=q1_l;
	    }
	    if (q2_l>q2max){
		    q2max=q2_l;
	    }
	    if (q3_l>q3max){
		    q3max=q3_l;
	    }
	    if (q4_l>q4max){
		    q4max=q4_l;
	    }
	    if (file_in.eof()) break;
    	}
	
	file_in.close();
	cout << "Max q1 low: " << q1max << endl;	// mi stampo i valori massimi
	cout << "Max q2 low: " << q2max << endl;
	cout << "Max q3 low: " << q3max << endl;
	cout << "Max q4 low: " << q4max << endl;
	
}