#include "Riostream.h"
#include <string>
#include <vector>
#include <numeric>

using namespace std;

void ritardiTDC() {
     // Read the file
     ifstream file("Ritardi_calibrazione_TDC.txt");	// apro il file con tutti i ritardi e le levette usate
     if (!file.is_open()) {
         cout << "Cannot open file!" << endl;
	    return;
     }
	
	vector<double> rit_values;	// vettore per contenere i valori dei ritardi
	vector<double> rit_errors;	// vettore per contenere gli errori sui ritardi
     string line;
     string filename;
     double rit05, rit1, rit2, rit4, rit8, rit16, rit32;	// errori sul ritardo introdotto da ciascuna levetta
     getline(file, line); // Skip the header line
	int icand = 0;	// contatore delle righe

     while (file.is_open()) {
	    
	    file >> filename >> rit05 >> rit1 >> rit2 >> rit4 >> rit8 >> rit16 >> rit32;
	    if (file.eof()) break;	// se arrivo alla fine del file, esco dal while
	    
	    string rit_str = filename.substr(3, 3);
	    double rit_value = stod(rit_str)/10.;
	    rit_values.push_back(rit_value);
	    
	    double err_value = rit05+rit1+rit2+rit4+rit8+rit16+rit32;
	    /*if(err_value<0.5){
		    err_value=0.5;
	    }*/
	    rit_errors.push_back(err_value);
	    
	    //printf("cand %i: \t rit_value=%0.3f ns\n", icand, rit_values.at(icand));
	    //printf("cand %i: \t err_rit=%0.3f ns \n", icand, rit_errors.at(icand));
	    ++icand;
    }
	     
	file.close();
	cout << "Total data is:" << rit_errors.size() << endl;
	return;

}