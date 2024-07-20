#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include "Riostream.h"
#include <string>
#include "TTree.h"
#include "TFile.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TPaveText.h"

using namespace std;

void calibrazioneQDC() {
    // Open the input file containing the list of .txt files
    ifstream inputFile("../Dati/QDC/filenameQDC.txt");
    if (!inputFile.is_open()) {
        cerr << "Error opening file filenameQDC.txt" << endl;
        return;
    }

    // Vettori per memorizzare i risultati
    vector<string> filenames;
    vector<double> means_q1_low, errors_q1_low;
    vector<double> means_q2_low, errors_q2_low;
    vector<double> means_q3_low, errors_q3_low;
    vector<double> means_q4_low, errors_q4_low;
    vector<double> means_q1_high, errors_q1_high;
    vector<double> means_q2_high, errors_q2_high;
    vector<double> means_q3_high, errors_q3_high;
    vector<double> means_q4_high, errors_q4_high;
    vector<double> x_values, x1, x2, x3, x4;
    vector<double> x1_high, x2_high, x3_high, x4_high;	// vettori per contenere valori attenuazione solo per alta risoluzione
    vector<double> attvec; // vettore per contenere i valori di attenuazione

    string line;
    while (getline(inputFile, line)) {
        string filePath = "../Dati/QDC/" + line;
        ifstream dataFile(filePath);
        if (!dataFile.is_open()) {
            cerr << "Error opening data file " << line << endl;
            continue;
        }

        // x value
        string x_str = line.substr(line.size() - 7, 3);  
        double A = stod(x_str)/10.0;
        double x_value = pow(10, - A / 20.0);
	   attvec.push_back(A);

        
        vector<double> col1, col2, col3, col4, col5, col6, col7, col8, col9;
        double val1, val2, val3, val4, val5, val6, val7, val8, val9;
        while (dataFile >> val1 >> val2 >> val3 >> val4 >> val5 >> val6 >> val7 >> val8 >> val9) {
            col1.push_back(val1);
            col2.push_back(val2);
            col3.push_back(val3);
            col4.push_back(val4);
            col5.push_back(val5);
            col6.push_back(val6);
            col7.push_back(val7);
            col8.push_back(val8);
            col9.push_back(val9);
        }
        dataFile.close();

        // Function to calculate mean and error
        auto calculateMeanAndError = [](const vector<double>& data, double& mean, double& error) {
            int n = data.size();
            if (n == 0) {
                mean = 0;
                error = 0;
                return;
            }
            double sum = 0;
            for (double val : data) {
                sum += val;
            }
            mean = sum / n;
            double variance = 0;
            for (double val : data) {
                variance += (val - mean) * (val - mean);
            }
            error = sqrt(variance) / (n-1);
        };

        double mean_q1_low, err_q1_low, mean_q2_low, err_q2_low, mean_q3_low, err_q3_low, mean_q4_low, err_q4_low;
        double mean_q1_high, err_q1_high, mean_q2_high, err_q2_high, mean_q3_high, err_q3_high, mean_q4_high, err_q4_high;
        calculateMeanAndError(col1, mean_q1_low,  err_q1_low);
        calculateMeanAndError(col2, mean_q1_high, err_q1_high);
        calculateMeanAndError(col3, mean_q2_low,  err_q2_low);
        calculateMeanAndError(col4, mean_q2_high, err_q2_high);
        calculateMeanAndError(col5, mean_q3_low,  err_q3_low);
        calculateMeanAndError(col6, mean_q3_high, err_q3_high);
        calculateMeanAndError(col7, mean_q4_low,  err_q4_low);
        calculateMeanAndError(col8, mean_q4_high, err_q4_high);

        // Memorizza i risultati nei vettori
        filenames.push_back(line);
        means_q1_low.push_back(mean_q1_low);
        errors_q1_low.push_back(err_q1_low);
        means_q2_low.push_back(mean_q2_low);
        errors_q2_low.push_back(err_q2_low);
        means_q3_low.push_back(mean_q3_low);
        errors_q3_low.push_back(err_q3_low);
        means_q4_low.push_back(mean_q4_low);
        errors_q4_low.push_back(err_q4_low);
        means_q1_high.push_back(mean_q1_high);
        errors_q1_high.push_back(err_q1_high);
        means_q2_high.push_back(mean_q2_high);
        errors_q2_high.push_back(err_q2_high);
        means_q3_high.push_back(mean_q3_high);
        errors_q3_high.push_back(err_q3_high);
        means_q4_high.push_back(mean_q4_high);
        errors_q4_high.push_back(err_q4_high);
        x_values.push_back(x_value);
    }

    // Chiudi il file di input
    inputFile.close();

    // Prepare x1, x2, x3, x4
    for (const auto& x : x_values) {
        x1.push_back(x * 21.593 / 50.0);
        x2.push_back(x * 21.223 / 50.0);
        x3.push_back(x * 20.894 / 50.0);
        x4.push_back(x * 20.924 / 50.0);
    }

    // ############# CALCOLO DEGLI ERRORI SULLE ATTENUAZIONI #############
    
   ifstream file("../Dati/QDC/Attenuazioni_calibrazione_QDC.txt");
    if (!file.is_open()) {
        cout << "Cannot open file!" << endl;
        return;
    }

    vector<double> att_errors1, att_errors2, att_errors3, att_errors4;
    vector<double> A1, A2, A3, A4;
    double A, q0, sigma_q0, var_A, sigma_A;
    double att05, att1, att2, att3, att6, att12, att20;

    // Read the file line by line and calculate errors
    getline(file, line); // Skip the header

    // Channel 1
    q0 = 21.593 / 50;
    sigma_q0 = 0.088 / 50;
    while (file >> A >> att05 >> att1 >> att2 >> att3 >> att6 >> att12 >> att20) {
        var_A = pow(q0,2.0)*(pow(att05,2.0)+pow(att1,2.0)+pow(att2,2.0)+pow(att3,2.0)+pow(att6,2.0)+pow(att12,2.0)+pow(att20,2.0)) * pow(log(10.0)/20,2.0);
        double err_value = sqrt(pow(10, -A / 10.0) * (pow(sigma_q0,2.0) +  var_A));
        att_errors1.push_back(err_value);
        A1.push_back(A);
        sigma_A = sqrt(var_A);
        cout << "sigma_q0 (ch1): " << sigma_q0 << "sigma_A (ch1)" << sigma_A << endl;
    }

    // Reset file stream to read again
    file.clear();
    file.seekg(0, ios::beg);
    getline(file, line); // Skip the header again

    // Channel 2
    q0 = 21.223 / 50;
    sigma_q0 = 0.088 / 50;
   while (file >> A >> att05 >> att1 >> att2 >> att3 >> att6 >> att12 >> att20) {
            var_A = pow(q0,2.0)*(pow(att05,2.0)+pow(att1,2.0)+pow(att2,2.0)+pow(att3,2.0)+pow(att6,2.0)+pow(att12,2.0)+pow(att20,2.0)) * pow(log(10.0)/20,2.0);
        double err_value = sqrt(pow(10, -A / 10.0) * (pow(sigma_q0,2.0) +  var_A));
        att_errors2.push_back(err_value);
        A2.push_back(A);
        sigma_A = sqrt(var_A);
        cout << "sigma_q0 (ch2): " << sigma_q0 << "sigma_A (ch2)" << sigma_A << endl;
    }

    // Reset file stream to read again
    file.clear();
    file.seekg(0, ios::beg);
    getline(file, line); // Skip the header again

    // Channel 3
    q0 = 20.894 / 50;
    sigma_q0 = 0.086 / 50;
   while (file >> A >> att05 >> att1 >> att2 >> att3 >> att6 >> att12 >> att20) {
            var_A = pow(q0,2.0)*(pow(att05,2.0)+pow(att1,2.0)+pow(att2,2.0)+pow(att3,2.0)+pow(att6,2.0)+pow(att12,2.0)+pow(att20,2.0)) * pow(log(10.0)/20,2.0);
        double err_value = sqrt(pow(10, -A / 10.0) * (pow(sigma_q0,2.0) +  var_A));
        att_errors3.push_back(err_value);
        A3.push_back(A);
        sigma_A = sqrt(var_A);
        cout << "sigma_q0 (ch3): " << sigma_q0 << "sigma_A (ch3)" << sigma_A << endl;
    }

    // Reset file stream to read again
    file.clear();
    file.seekg(0, ios::beg);
    getline(file, line); // Skip the header again

    // Channel 4
    q0 = 21.924 / 50;
    sigma_q0 = 0.087 / 50;
    while (file >> A >> att05 >> att1 >> att2 >> att3 >> att6 >> att12 >> att20) {
            var_A = pow(q0,2.0)*(pow(att05,2.0)+pow(att1,2.0)+pow(att2,2.0)+pow(att3,2.0)+pow(att6,2.0)+pow(att12,2.0)+pow(att20,2.0)) * pow(log(10.0)/20,2.0);
        double err_value = sqrt(pow(10, -A / 10.0) * (pow(sigma_q0,2.0) +  var_A));
        att_errors4.push_back(err_value);
        A4.push_back(A);
        sigma_A = sqrt(var_A);
        cout << "sigma_q0 (ch4): " << sigma_q0 << "sigma_A (ch4)" << sigma_A << endl;
    }

    file.close();




    // ############## FINE CALCOLO DEGLI ERRORI SUI RITARDI ##################
    
    
    
    
    
    
//////////////////////////////////////////////////////////////////////////////////    
    
    // Open the text file for writing
    ofstream textFileHigh1("../Dati/QDC/calibrazioneQDC_results_high1.txt");
    ofstream textFileHigh2("../Dati/QDC/calibrazioneQDC_results_high2.txt");
    ofstream textFileHigh3("../Dati/QDC/calibrazioneQDC_results_high3.txt");
    ofstream textFileHigh4("../Dati/QDC/calibrazioneQDC_results_high4.txt");
    ofstream textFileLow1("../Dati/QDC/calibrazioneQDC_results_low1.txt");
    ofstream textFileLow2("../Dati/QDC/calibrazioneQDC_results_low2.txt");
    ofstream textFileLow3("../Dati/QDC/calibrazioneQDC_results_low3.txt");
    ofstream textFileLow4("../Dati/QDC/calibrazioneQDC_results_low4.txt");
    if (!textFileLow1.is_open() || !textFileHigh1.is_open() || !textFileLow2.is_open() || !textFileHigh2.is_open() || !textFileLow3.is_open() || !textFileHigh3.is_open() || !textFileLow4.is_open() || !textFileHigh4.is_open()) {
        cerr << "Error opening text file for writing" << endl;
        return;
    }
    // Write header to the text file
    //textFileLow << "Filename\tMean_q1_low\tErr_q1_low\tMean_q2_low\tErr_q2_low\tMean_q3_low\tErr_q3_low\tMean_q4_low\tErr_q4_low\tx_value\n";
    textFileLow1 << "Att\tCarica\tErrCarica\tMeanq1low\tErrq1low\n";
    textFileLow2 << "Att\tCarica\tErrCarica\tMeanq2low\tErrq2low\n";
    textFileLow3 << "Att\tCarica\tErrCarica\tMeanq3low\tErrq3low\n";
    textFileLow4 << "Att\tCarica\tErrCarica\tMeanq4low\tErrq4low\n";
    //textFileHigh << "Filename\tMean_q1_high\tErr_q1_high\tMean_q2_high\tErr_q2_high\tMean_q3_high\tErr_q3_high\tMean_q4_high\tErr_q4_high\tx_value\n";
    textFileHigh1 << "Att\tCarica\tErrCarica\tMeanq1high\tErrq1high\n";
    textFileHigh2 << "Att\tCarica\tErrCarica\tMeanq2high\tErrq2high\n";
    textFileHigh3 << "Att\tCarica\tErrCarica\tMeanq3high\tErrq3high\n";
    textFileHigh4 << "Att\tCarica\tErrCarica\tMeanq4high\tErrq4high\n";

    for (size_t i = 0; i < filenames.size(); ++i) {
        textFileLow1 << attvec[i] << "\t" << x1[i] << "\t" << att_errors1[i] << "\t"
                << means_q1_low[i] << "\t" << errors_q1_low[i] << "\n";
        textFileLow2 << attvec[i] << "\t" << x2[i] << "\t" << att_errors2[i] << "\t"
                << means_q2_low[i] << "\t" << errors_q2_low[i] << "\n";
        textFileLow3 << attvec[i] << "\t" << x3[i] << "\t" << att_errors3[i] << "\t"
                << means_q3_low[i] << "\t" << errors_q3_low[i] << "\n";
        textFileLow4 << attvec[i] << "\t" << x4[i] << "\t" << att_errors4[i] << "\t"
                << means_q4_low[i] << "\t" << errors_q4_low[i] << "\n";

        textFileHigh1 << attvec[i] << "\t" << x1[i] << "\t" << att_errors1[i] << "\t"
                << means_q1_high[i] << "\t" << errors_q1_high[i] << "\n";
        textFileHigh2 << attvec[i] << "\t" << x2[i] << "\t" << att_errors2[i] << "\t"
                << means_q2_high[i] << "\t" << errors_q2_high[i] << "\n";
        textFileHigh3 << attvec[i] << "\t" << x3[i] << "\t" << att_errors3[i] << "\t"
                << means_q3_high[i] << "\t" << errors_q3_high[i] << "\n";
        textFileHigh4 << attvec[i] << "\t" << x4[i] << "\t" << att_errors4[i] << "\t"
                << means_q4_high[i] << "\t" << errors_q4_high[i] << "\n";
        

    }

    // Finalize text file
    textFileHigh1.close();
    textFileHigh2.close();
    textFileHigh3.close();
    textFileHigh4.close();
    textFileLow1.close();
    textFileLow2.close();
    textFileLow3.close();
    textFileLow4.close();


//////////////////////////////////////////////////////////////////////////////////

    // Crea una canvas
    TCanvas *c_calibr = new TCanvas("c_calibr", "Calibration Fits for Low Resolution", 1200, 800);
    c_calibr->cd();
    c_calibr->SetGrid();

    //////////////////////////////////////////LOW

    // Scatter plot e fit per mean_q1
    TGraphErrors *gr_q1_low = new TGraphErrors(x_values.size());
    for (int i = 0; i < x_values.size(); i++) {
        gr_q1_low->SetPoint(i, x1[i], means_q1_low[i]);
        gr_q1_low->SetPointError(i, att_errors1[i], errors_q1_low[i]);
    }
    gr_q1_low->SetMarkerStyle(20);
    gr_q1_low->SetMarkerColor(kPink+10);
    gr_q1_low->SetLineColor(kPink+10);
    gr_q1_low->SetTitle("Calibrazione QDC;Carica [nC];QDC");
    gr_q1_low->SetMinimum(0);
    gr_q1_low->SetMaximum(4500);

    TF1 *linear_q1_low = new TF1("linear_q1_low", "pol1", 0, 0.4);
    linear_q1_low->SetLineColor(kPink+10);
    linear_q1_low->SetLineWidth(1);
    gr_q1_low->Fit(linear_q1_low, "Q");
    //gr_q1_low->SetStats(false);
    //gStyle->SetOptFit(false);
    //gStyle->SetOptStat(false);
    gr_q1_low->Draw("AP");
    linear_q1_low->Draw("same");

    // Scatter plot e fit per mean_q2
    TGraphErrors *gr_q2_low = new TGraphErrors(x_values.size());
    for (int i = 0; i < x_values.size(); i++) {
        gr_q2_low->SetPoint(i, x2[i], means_q2_low[i]);
        gr_q2_low->SetPointError(i, att_errors2[i], errors_q2_low[i]);
    }
    gr_q2_low->SetMarkerStyle(20);
    gr_q2_low->SetMarkerColor(kAzure+1);
    gr_q2_low->SetLineColor(kAzure+1);

    TF1 *linear_q2_low = new TF1("linear_q2_low", "pol1", 0, 0.4);
    linear_q2_low->SetLineColor(kAzure+1);
    linear_q2_low->SetLineWidth(1);
    gr_q2_low->Fit(linear_q2_low, "Q");
    //gr_q2_low->SetStats(false);
    gr_q2_low->Draw("P same");
    linear_q2_low->Draw("same");

    // Scatter plot e fit per mean_q3
    TGraphErrors *gr_q3_low = new TGraphErrors(x_values.size());
    for (int i = 0; i < x_values.size(); i++) {
        gr_q3_low->SetPoint(i, x3[i], means_q3_low[i]);
        gr_q3_low->SetPointError(i, att_errors3[i], errors_q3_low[i]);
    }
    gr_q3_low->SetMarkerStyle(20);
    gr_q3_low->SetMarkerColor(kTeal);
    gr_q3_low->SetLineColor(kTeal);

    TF1 *linear_q3_low = new TF1("linear_q3v", "pol1", 0, 0.4);
    linear_q3_low->SetLineColor(kTeal);
    linear_q3_low->SetLineWidth(1);
    gr_q3_low->Fit(linear_q3_low, "Q");
    //gr_q3_low->SetStats(false);
    gr_q3_low->Draw("P same");
    linear_q3_low->Draw("same");

    // Scatter plot e fit per mean_q4
    TGraphErrors *gr_q4_low = new TGraphErrors(x_values.size());
    for (int i = 0; i < x_values.size(); i++) {
        gr_q4_low->SetPoint(i, x4[i], means_q4_low[i]);
        gr_q4_low->SetPointError(i, att_errors4[i], errors_q4_low[i]);
    }
    gr_q4_low->SetMarkerStyle(20);
    gr_q4_low->SetMarkerColor(kOrange-3);
    gr_q4_low->SetLineColor(kOrange-3);

    TF1 *linear_q4_low = new TF1("linear_q4_low", "pol1", 0, 0.4);
    linear_q4_low->SetLineColor(kOrange-3);
    linear_q4_low->SetLineWidth(1);
    gr_q4_low->Fit(linear_q4_low, "Q");
    //gr_q4_low->SetStats(false);
    gr_q4_low->Draw("P same");
    linear_q4_low->Draw("same");

    /////////////////////////////////////////////////////////////// HIGH
    // Scatter plot e fit per mean_q1
    TGraphErrors *gr_q1_high = new TGraphErrors(x1_high.size());
    int k = 0;
    for (int i = 0; i < x_values.size(); i++) {
        if (means_q1_high[i]<=3840) {
		  float x1_h = x1[i];
            gr_q1_high->SetPoint(k, x1[i], means_q1_high[i]);
            gr_q1_high->SetPointError(k, att_errors1[i], errors_q1_high[i]);
            k++;
		  x1_high.push_back(x1_h);
        }
    }
    gr_q1_high->SetMarkerStyle(21);
    gr_q1_high->SetMarkerColor(kPink+10);
    gr_q1_high->SetLineColor(kPink+10);

    TF1 *linear_q1_high = new TF1("linear_q1_high", "pol1", 0, 0.4);
    linear_q1_high->SetLineColor(kPink+10);
    linear_q1_high->SetLineWidth(1);
    gr_q1_high->Fit(linear_q1_high, "Q");
    //gr_q1_high->SetStats(false);
    gr_q1_high->Draw("P same");
    linear_q1_high->Draw("same");

    // Scatter plot e fit per mean_q2
    TGraphErrors *gr_q2_high = new TGraphErrors(x2_high.size());
    k = 0;
    for (int i = 0; i < x_values.size(); i++) {
        if (means_q2_high[i]<=3840) {
		  float x2_h = x2[i];
            gr_q2_high->SetPoint(k, x2[i], means_q2_high[i]);
            gr_q2_high->SetPointError(k, att_errors2[i], errors_q2_high[i]);
            k++;
		  x2_high.push_back(x2_h);
        }
    }
    gr_q2_high->SetMarkerStyle(21);
    gr_q2_high->SetMarkerColor(kAzure+1);
    gr_q2_high->SetLineColor(kAzure+1);

    TF1 *linear_q2_high = new TF1("linear_q2_high", "pol1", 0, 0.4);
    linear_q2_high->SetLineColor(kAzure+1);
    linear_q2_high->SetLineWidth(1);
    gr_q2_high->Fit(linear_q2_high, "Q");
    //gr_q2_high->SetStats(false);
    gr_q2_high->Draw("P same");
    linear_q2_high->Draw("same");

    // Scatter plot e fit per mean_q3
    TGraphErrors *gr_q3_high = new TGraphErrors(x3_high.size());
    k = 0;
    for (int i = 0; i < x_values.size(); i++) {
        if (means_q3_high[i]<=3840) {
		  float x3_h = x3[i];
            gr_q3_high->SetPoint(k, x3[i], means_q3_high[i]);
            gr_q3_high->SetPointError(k, att_errors3[i], errors_q3_high[i]);
            k++;
		  x3_high.push_back(x3_h);
        }
    }
    gr_q3_high->SetMarkerStyle(21);
    gr_q3_high->SetMarkerColor(kTeal);
    gr_q3_high->SetLineColor(kTeal);

    TF1 *linear_q3_high = new TF1("linear_q3v", "pol1", 0, 0.4);
    linear_q3_high->SetLineColor(kTeal);
    linear_q3_high->SetLineWidth(1);
    gr_q3_high->Fit(linear_q3_high, "Q");
    //gr_q3_high->SetStats(false);
    gr_q3_high->Draw("P same");
    linear_q3_high->Draw("same");

    // Scatter plot e fit per mean_q4
    TGraphErrors *gr_q4_high = new TGraphErrors(x4_high.size());
    k = 0;
    for (int i = 0; i < x_values.size(); i++) {
        if (means_q4_high[i]<=3840) {
		  float x4_h = x4[i];
            gr_q4_high->SetPoint(k, x4[i], means_q4_high[i]);
            gr_q4_high->SetPointError(k, att_errors4[i], errors_q4_high[i]);
            k++;
		  x4_high.push_back(x4_h);
        }
    }
    gr_q4_high->SetMarkerStyle(21);
    gr_q4_high->SetMarkerColor(kOrange-3);
    gr_q4_high->SetLineColor(kOrange-3);

    TF1 *linear_q4_high = new TF1("linear_q4_high", "pol1", 0, 0.4);
    linear_q4_high->SetLineColor(kOrange-3);
    linear_q4_high->SetLineWidth(1);
    gr_q4_high->Fit(linear_q4_high, "Q");
    //gr_q4_high->SetStats(false);
    gr_q4_high->Draw("P same");
    linear_q4_high->Draw("same");

    /////////////////////////////////////
    // Aggiungi il box con le statistiche
    TPaveText *statsBox = new TPaveText(0.5, 0.1, 0.9, 0.5, "NDC");
    statsBox->SetBorderSize(1);
    statsBox->SetFillColor(kWhite);
    statsBox->SetTextAlign(12);
    statsBox->SetTextSize(0.03);

    // Ottieni i parametri di fit
    double m_q1_low = linear_q1_low->GetParameter(1);
    double m_q1_err_low = linear_q1_low->GetParError(1);
    double q_q1_low = linear_q1_low->GetParameter(0);
    double q_q1_err_low = linear_q1_low->GetParError(0);

    double m_q2_low = linear_q2_low->GetParameter(1);
    double m_q2_err_low = linear_q2_low->GetParError(1);
    double q_q2_low = linear_q2_low->GetParameter(0);
    double q_q2_err_low = linear_q2_low->GetParError(0);

    double m_q3_low = linear_q3_low->GetParameter(1);
    double m_q3_err_low = linear_q3_low->GetParError(1);
    double q_q3_low = linear_q3_low->GetParameter(0);
    double q_q3_err_low = linear_q3_low->GetParError(0);

    double m_q4_low = linear_q4_low->GetParameter(1);
    double m_q4_err_low = linear_q4_low->GetParError(1);
    double q_q4_low = linear_q4_low->GetParameter(0);
    double q_q4_err_low = linear_q4_low->GetParError(0);

    double m_q1_high = linear_q1_high->GetParameter(1);
    double m_q1_err_high = linear_q1_high->GetParError(1);
    double q_q1_high = linear_q1_high->GetParameter(0);
    double q_q1_err_high = linear_q1_high->GetParError(0);

    double m_q2_high = linear_q2_high->GetParameter(1);
    double m_q2_err_high = linear_q2_high->GetParError(1);
    double q_q2_high = linear_q2_high->GetParameter(0);
    double q_q2_err_high = linear_q2_high->GetParError(0);

    double m_q3_high = linear_q3_high->GetParameter(1);
    double m_q3_err_high = linear_q3_high->GetParError(1);
    double q_q3_high = linear_q3_high->GetParameter(0);
    double q_q3_err_high = linear_q3_high->GetParError(0);

    double m_q4_high = linear_q4_high->GetParameter(1);
    double m_q4_err_high = linear_q4_high->GetParError(1);
    double q_q4_high = linear_q4_high->GetParameter(0);
    double q_q4_err_high = linear_q4_high->GetParError(0);

    statsBox->AddText(Form("q1_low: m = %.4f #pm %.4f, q = %.5f #pm %.4f", m_q1_low, m_q1_err_low, q_q1_low, q_q1_err_low));
    statsBox->AddText(Form("q2_low: m = %.4f #pm %.4f, q = %.5f #pm %.4f", m_q2_low, m_q2_err_low, q_q2_low, q_q2_err_low));
    statsBox->AddText(Form("q3_low: m = %.4f #pm %.4f, q = %.5f #pm %.4f", m_q3_low, m_q3_err_low, q_q3_low, q_q3_err_low));
    statsBox->AddText(Form("q4_low: m = %.4f #pm %.4f, q = %.5f #pm %.4f", m_q4_low, m_q4_err_low, q_q4_low, q_q4_err_low));
    statsBox->AddText(Form("q1_high: m = %.4f #pm %.4f, q = %.4f #pm %.4f", m_q1_high, m_q1_err_high, q_q1_high, q_q1_err_high));
    statsBox->AddText(Form("q2_high: m = %.4f #pm %.4f, q = %.4f #pm %.4f", m_q2_high, m_q2_err_high, q_q2_high, q_q2_err_high));
    statsBox->AddText(Form("q3_high: m = %.4f #pm %.4f, q = %.4f #pm %.4f", m_q3_high, m_q3_err_high, q_q3_high, q_q3_err_high));
    statsBox->AddText(Form("q4_high: m = %.4f #pm %.4f, q = %.4f #pm %.4f", m_q4_high, m_q4_err_high, q_q4_high, q_q4_err_high));

    //statsBox->Draw();
    TLegend* leg = new TLegend(0.2,0.65,0.5,0.8);		// costruzione della legenda
    leg->AddEntry(gr_q1_low, "Bassa risoluzione S1");
    leg->AddEntry(gr_q2_low, "Bassa risoluzione S2");
    leg->AddEntry(gr_q3_low, "Bassa risoluzione S3");
    leg->AddEntry(gr_q4_low, "Bassa risoluzione S4");
    leg->AddEntry(gr_q1_high, "Alta risoluzione S1");
    leg->AddEntry(gr_q2_high, "Alta risoluzione S2");
    leg->AddEntry(gr_q3_high, "Alta risoluzione S3");
    leg->AddEntry(gr_q4_high, "Alta risoluzione S4");
    leg->Draw();

    // Salva la canvas come file PNG
    c_calibr->Update();
    c_calibr->SaveAs("../Dati/QDC/fit_calibrazioneQDC.png");

//////////////////////////////////////////////////////////////////////////////////

    TF1 *line_0 = new TF1("line_0", "[0]*x", -1, 100);
    line_0->SetLineColor(kBlack);

// RESIDUI LOW 
    vector<double> sigma_res1_low, sigma_res2_low, sigma_res3_low, sigma_res4_low;		// vettore che contiene gli errori sui residui
    double sigma_res_value1_low, sigma_res_value2_low, sigma_res_value3_low, sigma_res_value4_low;

    // --- parte per calcolare gli errori sui residui ---
    for (int i=0; i < x_values.size(); i++) {
	    sigma_res_value1_low = sqrt(errors_q1_low[i]*errors_q1_low[i] + att_errors1[i]*linear_q1_low->Derivative(A1[i])*att_errors1[i]*linear_q1_low->Derivative(A1[i]));	// propagazione degli errori per includere sia errore su x che su y. Serve per calcolare correttamente chiquadro/ngf
	    sigma_res_value2_low = sqrt(errors_q2_low[i]*errors_q2_low[i] + att_errors2[i]*linear_q2_low->Derivative(A2[i])*att_errors2[i]*linear_q2_low->Derivative(A2[i]));
	    sigma_res_value3_low = sqrt(errors_q3_low[i]*errors_q3_low[i] + att_errors3[i]*linear_q3_low->Derivative(A3[i])*att_errors3[i]*linear_q3_low->Derivative(A3[i]));
	    sigma_res_value4_low = sqrt(errors_q4_low[i]*errors_q4_low[i] + att_errors4[i]*linear_q4_low->Derivative(A4[i])*att_errors4[i]*linear_q4_low->Derivative(A4[i]));
	    sigma_res1_low.push_back(sigma_res_value1_low);
	    sigma_res2_low.push_back(sigma_res_value2_low);
	    sigma_res3_low.push_back(sigma_res_value3_low);
	    sigma_res4_low.push_back(sigma_res_value4_low);
    }

    // Crea una canvas per i residui low
    TCanvas *c_residuals_low = new TCanvas("c_residuals_low", "", 1200, 800);
    c_residuals_low->cd();

    // Primo sottoplot: Residui per mean_q1_low
    TGraphErrors *gr_residuals_q1_low = new TGraphErrors(x_values.size());
    for (int i = 0; i < x_values.size(); i++) {
        double expected_q1_low = linear_q1_low->Eval(x1[i]);
        double residual_q1_low = means_q1_low[i] - expected_q1_low;
        gr_residuals_q1_low->SetPoint(i, x1[i], residual_q1_low);
        gr_residuals_q1_low->SetPointError(i, 0, sigma_res1_low[i]);
    }
    
    TF1 *horiz_q1_low = new TF1("horiz_q1_low", "pol1", 0.0, 100.0);	// fit a retta dei residui (linea il più possibile orizzontale)
    gr_residuals_q1_low->SetMarkerStyle(20);
    gr_residuals_q1_low->SetMarkerColor(kPink+10);
    gr_residuals_q1_low->SetTitle("Residui canale 0 (bassa risoluzione);Carica [nC];Residui");
    gr_residuals_q1_low->Draw("AP");
    gr_residuals_q1_low->Fit("horiz_q1_low");
    horiz_q1_low->Draw("same");
    horiz_q1_low->SetLineColor(kPink+10);

    TPaveText *eqretta_q1_low = new TPaveText(0.5, 0.1, 0.9, 0.2, "NDC");
    eqretta_q1_low->SetBorderSize(1);
    eqretta_q1_low->SetFillColor(kWhite);
    eqretta_q1_low->SetTextAlign(20);
    eqretta_q1_low->SetTextSize(0.03);
    double mr_q1_low = horiz_q1_low->GetParameter(1);
    double mr_q1_low_err = horiz_q1_low->GetParError(1);
    double qr_q1_low = horiz_q1_low->GetParameter(0);
    double qr_q1_low_err = horiz_q1_low->GetParError(0);
    eqretta_q1_low->AddText(Form("m = %.3f #pm %.3f, q = %.3f #pm %.3f", mr_q1_low, mr_q1_low_err, qr_q1_low, qr_q1_low_err));
    eqretta_q1_low->Draw("same");

    c_residuals_low->Update();

    // Secondo sottoplot: Residui per mean_q2_low
    TCanvas *c_residuals_low1 = new TCanvas("c_residuals_low1", "", 1200, 800);
    c_residuals_low1->cd();
    TGraphErrors *gr_residuals_q2_low = new TGraphErrors(x_values.size());
    for (int i = 0; i < x_values.size(); i++) {
        double expected_q2_low = linear_q2_low->Eval(x2[i]);
        double residual_q2_low = means_q2_low[i] - expected_q2_low;
        gr_residuals_q2_low->SetPoint(i, x2[i], residual_q2_low);
        gr_residuals_q2_low->SetPointError(i, 0, sigma_res2_low[i]);
    }

    TF1 *horiz_q2_low = new TF1("horiz_q2_low", "pol1", 0.0, 100.0);	// fit a retta dei residui (linea il più possibile orizzontale)
    gr_residuals_q2_low->SetMarkerStyle(20);
    gr_residuals_q2_low->SetMarkerColor(kAzure+1);
    gr_residuals_q2_low->SetTitle("Residui canale 1 (bassa risoluzione);Carica [nC];Residui");
    gr_residuals_q2_low->Draw("AP");
    gr_residuals_q2_low->Fit("horiz_q2_low");
    horiz_q2_low->Draw("same");
    horiz_q2_low->SetLineColor(kAzure+1);

    TPaveText *eqretta_q2_low = new TPaveText(0.5, 0.1, 0.9, 0.2, "NDC");
    eqretta_q2_low->SetBorderSize(1);
    eqretta_q2_low->SetFillColor(kWhite);
    eqretta_q2_low->SetTextAlign(20);
    eqretta_q2_low->SetTextSize(0.03);
    double mr_q2_low = horiz_q2_low->GetParameter(1);
    double mr_q2_low_err = horiz_q2_low->GetParError(1);
    double qr_q2_low = horiz_q2_low->GetParameter(0);
    double qr_q2_low_err = horiz_q2_low->GetParError(0);
    eqretta_q2_low->AddText(Form("m = %.3f #pm %.3f, q = %.3f #pm %.3f", mr_q2_low, mr_q2_low_err, qr_q2_low, qr_q2_low_err));
    eqretta_q2_low->Draw("same");
    
    // Terzo sottoplot: Residui per mean_q3_low
    TCanvas *c_residuals_low2 = new TCanvas("c_residuals_low2", "", 1200, 800);
    c_residuals_low2->cd();
    TGraphErrors *gr_residuals_q3_low = new TGraphErrors(x_values.size());
    for (int i = 0; i < x_values.size(); i++) {
        double expected_q3_low = linear_q3_low->Eval(x3[i]);
        double residual_q3_low = means_q3_low[i] - expected_q3_low;
        gr_residuals_q3_low->SetPoint(i, x3[i], residual_q3_low);
        gr_residuals_q3_low->SetPointError(i, 0, sigma_res3_low[i]);
    }
    
    TF1 *horiz_q3_low = new TF1("horiz_q3_low", "pol1", 0.0, 100.0);	// fit a retta dei residui (linea il più possibile orizzontale)
    gr_residuals_q3_low->SetMarkerStyle(20);
    gr_residuals_q3_low->SetMarkerColor(kTeal);
    gr_residuals_q3_low->SetTitle("Residui canale 2 (bassa risoluzione);Carica [nC];Residui");
    gr_residuals_q3_low->Draw("AP");
    gr_residuals_q3_low->Fit("horiz_q3_low");
    horiz_q3_low->Draw("same");
    horiz_q3_low->SetLineColor(kTeal);

    TPaveText *eqretta_q3_low = new TPaveText(0.5, 0.1, 0.9, 0.2, "NDC");
    eqretta_q3_low->SetBorderSize(1);
    eqretta_q3_low->SetFillColor(kWhite);
    eqretta_q3_low->SetTextAlign(20);
    eqretta_q3_low->SetTextSize(0.03);
    double mr_q3_low = horiz_q3_low->GetParameter(1);
    double mr_q3_low_err = horiz_q3_low->GetParError(1);
    double qr_q3_low = horiz_q3_low->GetParameter(0);
    double qr_q3_low_err = horiz_q3_low->GetParError(0);
    eqretta_q3_low->AddText(Form("m = %.3f #pm %.3f, q = %.3f #pm %.3f", mr_q3_low, mr_q3_low_err, qr_q3_low, qr_q3_low_err));
    eqretta_q3_low->Draw("same");

    // Quarto sottoplot: Residui per mean_q4_low
    TCanvas *c_residuals_low3 = new TCanvas("c_residuals_low3", "", 1200, 800);
    c_residuals_low3->cd();
    TGraphErrors *gr_residuals_q4_low = new TGraphErrors(x_values.size());
    for (int i = 0; i < x_values.size(); i++) {
        double expected_q4_low = linear_q4_low->Eval(x4[i]);
        double residual_q4_low = means_q4_low[i] - expected_q4_low;
        gr_residuals_q4_low->SetPoint(i, x4[i], residual_q4_low);
        gr_residuals_q4_low->SetPointError(i, 0, sigma_res4_low[i]);
    }
    
    TF1 *horiz_q4_low = new TF1("horiz_q4_low", "pol1", 0.0, 100.0);	// fit a retta dei residui (linea il più possibile orizzontale)
    gr_residuals_q4_low->SetMarkerStyle(20);
    gr_residuals_q4_low->SetMarkerColor(kOrange-3);
    gr_residuals_q4_low->SetTitle("Residui canale 3 (bassa risoluzione);Carica [nC];Residui");
    gr_residuals_q4_low->Draw("AP");
    gr_residuals_q4_low->Fit("horiz_q4_low");
    horiz_q4_low->Draw("same");
    horiz_q4_low->SetLineColor(kOrange-3);

    TPaveText *eqretta_q4_low = new TPaveText(0.5, 0.1, 0.9, 0.2, "NDC");
    eqretta_q4_low->SetBorderSize(1);
    eqretta_q4_low->SetFillColor(kWhite);
    eqretta_q4_low->SetTextAlign(20);
    eqretta_q4_low->SetTextSize(0.03);
    double mr_q4_low = horiz_q4_low->GetParameter(1);
    double mr_q4_low_err = horiz_q4_low->GetParError(1);
    double qr_q4_low = horiz_q4_low->GetParameter(0);
    double qr_q4_low_err = horiz_q4_low->GetParError(0);
    eqretta_q4_low->AddText(Form("m = %.3f #pm %.3f, q = %.3f #pm %.3f", mr_q4_low, mr_q4_low_err, qr_q4_low, qr_q4_low_err));
    eqretta_q4_low->Draw("same");

    // Salva la canvas come file PNG
    c_residuals_low->Update();
    c_residuals_low->SaveAs("../Dati/QDC/residuals_calibrazioneQDC_low.png");


//////////////////////////////////////////////////////////////////////////////////
// RESIDUI HIGH

    vector<double> sigma_res1_high, sigma_res2_high, sigma_res3_high, sigma_res4_high;		// vettore che contiene gli errori sui residui
    double sigma_res_value1_high, sigma_res_value2_high, sigma_res_value3_high, sigma_res_value4_high;

    // --- parte per calcolare gli errori sui residui ---
    for (int i=0; i < x_values.size(); i++) {
	    sigma_res_value1_high = sqrt(errors_q1_high[i]*errors_q1_high[i] + att_errors1[i]*linear_q1_high->Derivative(A1[i])*att_errors1[i]*linear_q1_high->Derivative(A1[i]));	// propagazione degli errori per includere sia errore su x che su y. Serve per calcolare correttamente chiquadro/ngf
	    sigma_res_value2_high = sqrt(errors_q2_high[i]*errors_q2_high[i] + att_errors2[i]*linear_q2_high->Derivative(A2[i])*att_errors2[i]*linear_q2_high->Derivative(A2[i]));
	    sigma_res_value3_high = sqrt(errors_q3_high[i]*errors_q3_high[i] + att_errors3[i]*linear_q3_high->Derivative(A3[i])*att_errors3[i]*linear_q3_high->Derivative(A3[i]));
	    sigma_res_value4_high = sqrt(errors_q4_high[i]*errors_q4_high[i] + att_errors4[i]*linear_q4_high->Derivative(A4[i])*att_errors4[i]*linear_q4_high->Derivative(A4[i]));
	    sigma_res1_high.push_back(sigma_res_value1_high);
	    sigma_res2_high.push_back(sigma_res_value2_high);
	    sigma_res3_high.push_back(sigma_res_value3_high);
	    sigma_res4_high.push_back(sigma_res_value4_high);
    }


    // Crea una canvas per i residui high
    TCanvas *c_residuals_high = new TCanvas("c_residuals_high", "", 1200, 800);
    c_residuals_high->cd();

    // Primo sottoplot: Residui per mean_q1_high
    TGraphErrors *gr_residuals_q1_high = new TGraphErrors(x1_high.size());
    int j = 0;
    for (int i = 0; i < x_values.size(); i++) {
        if (means_q1_high[i] <= 3840) {
		  float x1_h2 = x1[i];
            double expected_q1_high = linear_q1_high->Eval(x1_h2);
            double residual_q1_high = means_q1_high[i] - expected_q1_high;
            gr_residuals_q1_high->SetPoint(j, x1_h2, residual_q1_high);
            gr_residuals_q1_high->SetPointError(j, 0, sigma_res1_high[i]);
            j++;
        }
    }

    TF1 *horiz_q1_high = new TF1("horiz_q1_high", "pol1", 0.0, 100.0);	// fit a retta dei residui (linea il più possibile orizzontale)
    gr_residuals_q1_high->SetMarkerStyle(20);
    gr_residuals_q1_high->SetMarkerColor(kPink+10);
    gr_residuals_q1_high->SetTitle("Residui canale 0 (alta risoluzione);Carica [nC];Residui");
    gr_residuals_q1_high->Draw("AP");
    gr_residuals_q1_high->Fit("horiz_q1_high");
    horiz_q1_high->Draw("same");
    horiz_q1_high->SetLineColor(kPink+10);

    TPaveText *eqretta_q1_high = new TPaveText(0.5, 0.1, 0.9, 0.2, "NDC");
    eqretta_q1_high->SetBorderSize(1);
    eqretta_q1_high->SetFillColor(kWhite);
    eqretta_q1_high->SetTextAlign(20);
    eqretta_q1_high->SetTextSize(0.03);
    double mr_q1_high = horiz_q1_high->GetParameter(1);
    double mr_q1_high_err = horiz_q1_low->GetParError(1);
    double qr_q1_high = horiz_q1_low->GetParameter(0);
    double qr_q1_high_err = horiz_q1_low->GetParError(0);
    eqretta_q1_high->AddText(Form("m = %.3f #pm %.3f, q = %.3f #pm %.3f", mr_q1_high, mr_q1_high_err, qr_q1_high, qr_q1_high_err));
    eqretta_q1_high->Draw("same");

    c_residuals_high->Update();


    // Secondo sottoplot: Residui per mean_q2_high
    TCanvas *c_residuals_high1 = new TCanvas("c_residuals_high1", "Residuals for High Resolution", 1200, 800);
    c_residuals_high1->cd();
    TGraphErrors *gr_residuals_q2_high = new TGraphErrors(x2_high.size());
    j = 0;
    for (int i = 0; i < x_values.size(); i++) {
        if (means_q2_high[i] <= 3840) {
		  float x2_h2 = x2[i];
            double expected_q2_high = linear_q2_high->Eval(x2_h2);
            double residual_q2_high = means_q2_high[i] - expected_q2_high;
            gr_residuals_q2_high->SetPoint(j, x2_h2, residual_q2_high);
            gr_residuals_q2_high->SetPointError(j, 0, sigma_res2_high[i]);
            j++;
        }
    }

TF1 *horiz_q2_high = new TF1("horiz_q2_high", "pol1", 0.0, 100.0);	// fit a retta dei residui (linea il più possibile orizzontale)
    gr_residuals_q2_high->SetMarkerStyle(20);
    gr_residuals_q2_high->SetMarkerColor(kAzure+1);
    gr_residuals_q2_high->SetTitle("Residui canale 1 (alta risoluzione);Carica [nC];Residui");
    gr_residuals_q2_high->Draw("AP");
    gr_residuals_q2_high->Fit("horiz_q2_high");
    horiz_q2_high->Draw("same");
    horiz_q2_high->SetLineColor(kAzure+1);

    TPaveText *eqretta_q2_high = new TPaveText(0.5, 0.1, 0.9, 0.2, "NDC");
    eqretta_q2_high->SetBorderSize(1);
    eqretta_q2_high->SetFillColor(kWhite);
    eqretta_q2_high->SetTextAlign(20);
    eqretta_q2_high->SetTextSize(0.03);
    double mr_q2_high = horiz_q2_high->GetParameter(1);
    double mr_q2_high_err = horiz_q2_low->GetParError(1);
    double qr_q2_high = horiz_q2_low->GetParameter(0);
    double qr_q2_high_err = horiz_q2_low->GetParError(0);
    eqretta_q2_high->AddText(Form("m = %.3f #pm %.3f, q = %.3f #pm %.3f", mr_q2_high, mr_q2_high_err, qr_q2_high, qr_q2_high_err));
    eqretta_q2_high->Draw("same");

    c_residuals_high1->Update();

    // Terzo sottoplot: Residui per mean_q3_high
    TCanvas *c_residuals_high2 = new TCanvas("c_residuals_high2", "Residuals for High Resolution", 1200, 800);
    c_residuals_high2->cd();
    TGraphErrors *gr_residuals_q3_high = new TGraphErrors(x3_high.size());
    j = 0;
    for (int i = 0; i < x_values.size(); i++) {
        if (means_q3_high[i] <= 3840) {
		  float x3_h2 = x3[i];
            double expected_q3_high = linear_q3_high->Eval(x3_h2);
            double residual_q3_high = means_q3_high[i] - expected_q3_high;
            gr_residuals_q3_high->SetPoint(j, x3_h2, residual_q3_high);
            gr_residuals_q3_high->SetPointError(j, 0, sigma_res3_high[i]);
            j++;
        }
    }

TF1 *horiz_q3_high = new TF1("horiz_q3_high", "pol1", 0.0, 100.0);	// fit a retta dei residui (linea il più possibile orizzontale)
    gr_residuals_q3_high->SetMarkerStyle(20);
    gr_residuals_q3_high->SetMarkerColor(kTeal);
    gr_residuals_q3_high->SetTitle("Residui canale 2 (alta risoluzione);Carica [nC];Residui");
    gr_residuals_q3_high->Draw("AP");
    gr_residuals_q3_high->Fit("horiz_q3_high");
    horiz_q3_high->Draw("same");
    horiz_q3_high->SetLineColor(kTeal);

    TPaveText *eqretta_q3_high = new TPaveText(0.5, 0.1, 0.9, 0.2, "NDC");
    eqretta_q3_high->SetBorderSize(1);
    eqretta_q3_high->SetFillColor(kWhite);
    eqretta_q3_high->SetTextAlign(20);
    eqretta_q3_high->SetTextSize(0.03);
    double mr_q3_high = horiz_q3_high->GetParameter(1);
    double mr_q3_high_err = horiz_q3_low->GetParError(1);
    double qr_q3_high = horiz_q3_low->GetParameter(0);
    double qr_q3_high_err = horiz_q3_low->GetParError(0);
    eqretta_q3_high->AddText(Form("m = %.3f #pm %.3f, q = %.3f #pm %.3f", mr_q3_high, mr_q3_high_err, qr_q3_high, qr_q3_high_err));
    eqretta_q3_high->Draw("same");

    c_residuals_high2->Update();

    // Quarto sottoplot: Residui per mean_q4_high
    TCanvas *c_residuals_high3 = new TCanvas("c_residuals_high3", "Residuals for High Resolution", 1200, 800);
    c_residuals_high3->cd();
    TGraphErrors *gr_residuals_q4_high = new TGraphErrors(x4_high.size());
    j = 0;
    for (int i = 0; i < x_values.size(); i++) {
        if (means_q4_high[i] <= 3840) {
		  float x4_h2 = x4[i];
            double expected_q4_high = linear_q4_high->Eval(x4_h2);
            double residual_q4_high = means_q4_high[i] - expected_q4_high;
            gr_residuals_q4_high->SetPoint(j, x4_h2, residual_q4_high);
            gr_residuals_q4_high->SetPointError(j, 0, sigma_res4_high[i]);
            j++;
        }
    }

TF1 *horiz_q4_high = new TF1("horiz_q4_high", "pol1", 0.0, 100.0);	// fit a retta dei residui (linea il più possibile orizzontale)
    gr_residuals_q4_high->SetMarkerStyle(20);
    gr_residuals_q4_high->SetMarkerColor(kOrange-3);
    gr_residuals_q4_high->SetTitle("Residui canale 3 (alta risoluzione);Carica [nC];Residui");
    gr_residuals_q4_high->Draw("AP");
    gr_residuals_q4_high->Fit("horiz_q4_high");
    horiz_q4_high->Draw("same");
    horiz_q4_high->SetLineColor(kOrange-3);

    TPaveText *eqretta_q4_high = new TPaveText(0.5, 0.1, 0.9, 0.2, "NDC");
    eqretta_q4_high->SetBorderSize(1);
    eqretta_q4_high->SetFillColor(kWhite);
    eqretta_q4_high->SetTextAlign(20);
    eqretta_q4_high->SetTextSize(0.03);
    double mr_q4_high = horiz_q4_high->GetParameter(1);
    double mr_q4_high_err = horiz_q4_low->GetParError(1);
    double qr_q4_high = horiz_q4_low->GetParameter(0);
    double qr_q4_high_err = horiz_q4_low->GetParError(0);
    eqretta_q4_high->AddText(Form("m = %.3f #pm %.3f, q = %.3f #pm %.3f", mr_q4_high, mr_q4_high_err, qr_q4_high, qr_q4_high_err));
    eqretta_q4_high->Draw("same");

    c_residuals_high3->Update();

    // Salva la canvas come file PNG
    c_residuals_high->Update();
    c_residuals_high->SaveAs("../Dati/QDC/residuals_calibrazioneQDC_high.png");

    // ### Istogrammi dei RESIDUI LOW ###
    TCanvas *c_histores = new TCanvas("c_histores", "Istogramma residui QDC_high", 1200, 800);
    c_histores->cd();

    // Istogramma dei residui per q1_low
    TH1F* h1 = new TH1F("Canale 0 (bassa risoluzione)", "", 20, -20.0, 20.0);
    for (int i=0; i < x_values.size(); i++) {
	    float resid_della_ricca1 = (means_q1_low[i]-linear_q1_low->Eval(x1[i]))/sigma_res1_low[i];
	    h1->Fill(resid_della_ricca1);
    }
    h1->SetFillColorAlpha(kRed, 1.0);	// colore di riempimento dell'istogramma
    h1->SetFillStyle(3002);	// pattern di riempimento
    h1->SetTitle("Istogramma dei pull; conteggi; pull");

    //TCanvas* c2 = new TCanvas("c2", "Istogramma dei residui QDC", 600, 0, 500, 500);
    TF1* gauss1 = new TF1("gauss1", "gaus", -2.0, 2.0);
    gauss1->SetParameters(3.0, 0.0, 1.0);
    h1->Fit(gauss1);
    h1->Draw("E1");	// disegno le barre di errore
    h1->Draw("bar same");
    //gStyle->SetOptStat(0);
    gStyle->SetOptFit(111);	// visualizzo le statistiche
    c_histores->Update();
    
    // Istogramma dei residui per q2_low
    TCanvas *c_histores2 = new TCanvas("c_histores2", "Istogramma residui QDC_high", 1200, 800);
    c_histores2->cd();
    TH1F* h2 = new TH1F("Canale 1 (bassa risoluzione)", "", 20, -20.0, 20.0);
    for (int i=0; i < x_values.size(); i++) {
	    float resid_della_ricca2 = (means_q2_low[i]-linear_q2_low->Eval(x2[i]))/sigma_res2_low[i];
	    h2->Fill(resid_della_ricca2);
    }
    h2->SetFillColorAlpha(kRed, 1.0);
    h2->SetFillStyle(3002);
    h2->SetTitle("Istogramma dei pull; conteggi; pull");
    TF1* gauss2 = new TF1("gauss2", "gaus", -2.0, 2.0);
    gauss2->SetParameters(3.0, 0.0, 1.0);
    h2->Fit(gauss2);
    h2->Draw("E1");
    h2->Draw("bar same");
    gStyle->SetOptFit(111);
    c_histores2->Update();
    
    // Istogramma dei residui per q3_low
    TCanvas *c_histores3 = new TCanvas("c_histores3", "Istogramma residui QDC_high", 1200, 800);
    c_histores3->cd();
    TH1F* h3 = new TH1F("Canale 2 (bassa risoluzione)", "", 20, -30.0, 30.0);
    for (int i=0; i < x_values.size(); i++) {
	    float resid_della_ricca3 = (means_q3_low[i]-linear_q3_low->Eval(x3[i]))/sigma_res3_low[i];
	    h3->Fill(resid_della_ricca3);
    }
    h3->SetFillColorAlpha(kRed, 1.0);
    h3->SetFillStyle(3002);
    h3->SetTitle("Istogramma dei pull; conteggi; pull");
    TF1* gauss3 = new TF1("gauss3", "gaus", -2.0, 2.0);
    gauss3->SetParameters(3.0, 0.0, 1.0);
    h3->Fit(gauss3);
    h3->Draw("E1");
    h3->Draw("bar same");
    gStyle->SetOptFit(111);
    c_histores3->Update();
    
    // Istogramma dei residui per q4_low
    TCanvas *c_histores4 = new TCanvas("c_histores4", "Istogramma residui QDC_high", 1200, 800);
    c_histores4->cd();
    TH1F* h4 = new TH1F("Canale 3 (bassa risoluzione)", "", 20, -20.0, 20.0);
    for (int i=0; i < x_values.size(); i++) {
	    float resid_della_ricca4 = (means_q4_low[i]-linear_q4_low->Eval(x4[i]))/sigma_res4_low[i];
	    h4->Fill(resid_della_ricca4);
    }
    h4->SetFillColorAlpha(kRed, 1.0);
    h4->SetFillStyle(3002);
    h4->SetTitle("Istogramma dei pull; conteggi; pull");
    TF1* gauss4 = new TF1("gauss4", "gaus", -2.0, 2.0);
    gauss4->SetParameters(3.0, 0.0, 1.0);
    h4->Fit(gauss4);
    h4->Draw("E1");
    h4->Draw("bar same");
    gStyle->SetOptFit(111);
    c_histores4->Update();
    
    // Salva la canvas come file PNG
    c_histores->Update();
    c_histores->SaveAs("../Dati/QDC/calibrazioneQDC_historesiduiLOW.png");

    // ### Istogrammi dei RESIDUI HIGH ###
    TCanvas *c_historesh = new TCanvas("c_historesh", "Istogramma residui QDC_high", 1200, 800);
    c_historesh->cd();
    // Istogramma dei residui per q1_high
    TH1F* h1h = new TH1F("Canale 0 (alta risoluzione)", "", 22, -200.0, 200.0);
    for (int i=0; i < x1_high.size(); i++) {
	    float resid_della_ricca1 = (means_q1_high[i]-linear_q1_high->Eval(x1_high[i]))/sigma_res1_high[i];
	    h1h->Fill(resid_della_ricca1);
    }
    h1h->SetFillColorAlpha(kRed, 1.0);	// colore di riempimento dell'istogramma
    h1h->SetFillStyle(3002);	// pattern di riempimento
    h1h->SetTitle("Istogramma dei pull; conteggi; pull");
    //TCanvas* c2 = new TCanvas("c2", "Istogramma dei residui QDC", 600, 0, 500, 500);
    TF1* gauss1h = new TF1("gauss1h", "gaus", -2.0, 2.0);
    gauss1h->SetParameters(3.0, 0.0, 1.0);
    h1h->Fit(gauss1h);
    h1h->Draw("E1");	// disegno le barre di errore
    h1h->Draw("bar same");
    //gStyle->SetOptStat(0);
    gStyle->SetOptFit(111);	// visualizzo le statistiche
    c_historesh->Update();
    
    // Istogramma dei residui per q2_high
    TCanvas *c_historesh2 = new TCanvas("c_historesh2", "Istogramma residui QDC_high", 1200, 800);
    c_historesh2->cd();
    TH1F* h2h = new TH1F("Canale 1 (alta risoluzione)", "", 22, -200.0, 200.0);
    for (int i=0; i < x2_high.size(); i++) {
	    float resid_della_ricca2 = (means_q2_high[i]-linear_q2_high->Eval(x2_high[i]))/sigma_res2_high[i];
	    h2h->Fill(resid_della_ricca2);
    }
    h2h->SetFillColorAlpha(kRed, 1.0);
    h2h->SetFillStyle(3002);
    h2h->SetTitle("Istogramma dei pull; conteggi; pull");
    TF1* gauss2h = new TF1("gauss2h", "gaus", -2.0, 2.0);
    gauss2h->SetParameters(3.0, 0.0, 1.0);
    h2h->Fit(gauss2h);
    h2h->Draw("E1");
    h2h->Draw("bar same");
    gStyle->SetOptFit(111);
    c_historesh2->Update();
    
    // Istogramma dei residui per q3_high
    TCanvas *c_historesh3 = new TCanvas("c_historesh3", "Istogramma residui QDC_high", 1200, 800);
    c_historesh3->cd();
    TH1F* h3h = new TH1F("Canale 2 (alta risoluzione)", "", 20, -200.0, 200.0);
    for (int i=0; i < x3_high.size(); i++) {
	    float resid_della_ricca3 = (means_q3_high[i]-linear_q3_high->Eval(x3_high[i]))/sigma_res3_high[i];
	    h3h->Fill(resid_della_ricca3);
    }
    h3h->SetFillColorAlpha(kRed, 1.0);
    h3h->SetFillStyle(3002);
    h3h->SetTitle("Istogramma dei pull; conteggi; pull");
    TF1* gauss3h = new TF1("gauss3h", "gaus", -2.0, 2.0);
    gauss3h->SetParameters(3.0, 0.0, 1.0);
    h3h->Fit(gauss3h);
    h3h->Draw("E1");
    h3h->Draw("bar same");
    gStyle->SetOptFit(111);
    c_historesh3->Update();
    
    // Istogramma dei residui per q4_high
    TCanvas *c_historesh4 = new TCanvas("c_historesh4", "Istogramma residui QDC_high", 1200, 800);
    c_historesh4->cd();
    TH1F* h4h = new TH1F("Canale 3 (alta risoluzione)", "", 22, -200.0, 200.0);
    for (int i=0; i < x4_high.size(); i++) {
	    float resid_della_ricca4 = (means_q4_high[i]-linear_q4_high->Eval(x4_high[i]))/sigma_res4_high[i];
	    h4h->Fill(resid_della_ricca4);
    }
    h4h->SetFillColorAlpha(kRed, 1.0);
    h4h->SetFillStyle(3002);
    h4h->SetTitle("Istogramma dei pull; conteggi; pull");
    TF1* gauss4h = new TF1("gauss4h", "gaus", -2.0, 2.0);
    gauss4h->SetParameters(3.0, 0.0, 1.0);
    h4h->Fit(gauss4h);
    h4h->Draw("E1");
    h4h->Draw("bar same");
    gStyle->SetOptFit(111);
    c_historesh4->Update();

    return;
}
