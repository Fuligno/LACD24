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

void calibrazioneQDC_unico() {
    // Open the input file containing the list of .txt files
    ifstream inputFile("../Dati/QDC/filenameQDC.txt");
    if (!inputFile.is_open()) {
        cerr << "Error opening file filenameQDC.txt" << endl;
        return;
    }

    double R1 =7.843, R2=7.978, R3=7.885, R4=7.908;
    double errR1 =0.148, errR2=0.235, errR3=0.273, errR4=0.434;

    vector<double> errLH1, errLH2, errLH3, errLH4;

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


vector<double> LH1(means_q1_low);
vector<double> LH2(means_q2_low);
vector<double> LH3(means_q3_low);
vector<double> LH4(means_q4_low);

for (int i = 0; i < 5; i++) {
    LH1.push_back(R1 * means_q1_low[i]);
    LH2.push_back(R2 * means_q2_low[i]);
    LH3.push_back(R3 * means_q3_low[i]);
    LH4.push_back(R4 * means_q4_low[i]);
    errLH1.push_back(sqrt(pow(errR1, 2) + pow(errors_q1_low[i], 2)));
    errLH2.push_back(sqrt(pow(errR2, 2) + pow(errors_q2_low[i], 2)));
    errLH3.push_back(sqrt(pow(errR3, 2) + pow(errors_q3_low[i], 2)));
    errLH4.push_back(sqrt(pow(errR4, 2) + pow(errors_q4_low[i], 2)));
}
for (int i = 5; i < 12; i++) {
    LH1.push_back(means_q1_high[i]);
    LH2.push_back(means_q2_high[i]);
    LH3.push_back(means_q3_high[i]);
    LH4.push_back(means_q4_high[i]);
    errLH1.push_back(errors_q1_high[i]);
    errLH2.push_back(errors_q2_high[i]);
    errLH3.push_back(errors_q3_high[i]);
    errLH4.push_back(errors_q4_high[i]);
}

for (int i = 0; i < 12; i++) {
    cout << "////////////////////////////////" << endl;
    cout << "HR: " << means_q1_high[i] << endl;
    cout << "LH: " << LH1[i] << endl;
}
    // Open the text file for writing
    ofstream textFileHigh("../Dati/QDC/calibrazioneQDC_results_high.txt");
    ofstream textFileLow("../Dati/QDC/calibrazioneQDC_results_low.txt");
    if (!textFileLow.is_open() || !textFileHigh.is_open()) {
        cerr << "Error opening text file for writing" << endl;
        return;
    }
    // Write header to the text file
    textFileLow << "Filename\tMean_q1_low\tErr_q1_low\tMean_q2_low\tErr_q2_low\tMean_q3_low\tErr_q3_low\tMean_q4_low\tErr_q4_low\tx_value\n";
    textFileHigh << "Filename\tMean_q1_high\tErr_q1_high\tMean_q2_high\tErr_q2_high\tMean_q3_high\tErr_q3_high\tMean_q4_high\tErr_q4_high\tx_value\n";

    for (size_t i = 0; i < filenames.size(); ++i) {
        textFileLow << filenames[i] << "\t"
                << means_q1_low[i] << "\t" << errors_q1_low[i] << "\t"
                << means_q2_low[i] << "\t" << errors_q2_low[i] << "\t"
                << means_q3_low[i] << "\t" << errors_q3_low[i] << "\t"
                << means_q4_low[i] << "\t" << errors_q4_low[i] << "\t"
                << x_values[i] << "\n";

        textFileHigh << filenames[i] << "\t"
                << means_q1_high[i] << "\t" << errors_q1_high[i] << "\t"
                << means_q2_high[i] << "\t" << errors_q2_high[i] << "\t"
                << means_q3_high[i] << "\t" << errors_q3_high[i] << "\t"
                << means_q4_high[i] << "\t" << errors_q4_high[i] << "\t"
                << x_values[i] << "\n";
        

    }

    // Finalize text file
    textFileHigh.close();
    textFileLow.close();

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


 // Crea una canvas
    TCanvas *c_calibr = new TCanvas("c_calibr", "Calibration Fits for Low Resolution", 1200, 800);
    c_calibr->cd();
    c_calibr->SetGrid();

    //////////////////////////////////////////LOW

    // Scatter plot e fit per mean_q1
    TGraphErrors *gr_q1_low = new TGraphErrors(x_values.size());
    for (int i = 0; i < x_values.size(); i++) {
        gr_q1_low->SetPoint(i, x1[i], LH1[i]);
        gr_q1_low->SetPointError(i, att_errors1[i], errLH1[i]);
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
    gStyle->SetOptFit(false);
    gStyle->SetOptStat(false);
    gr_q1_low->Draw("AP");
    linear_q1_low->Draw("same");

    // Scatter plot e fit per mean_q2
    TGraphErrors *gr_q2_low = new TGraphErrors(x_values.size());
    for (int i = 0; i < x_values.size(); i++) {
        gr_q2_low->SetPoint(i, x2[i], LH2[i]);
        gr_q2_low->SetPointError(i, att_errors2[i], errLH2[i]);
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
        gr_q3_low->SetPoint(i, x3[i], LH3[i]);
        gr_q3_low->SetPointError(i, att_errors3[i], errLH3[i]);
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
        gr_q4_low->SetPoint(i, x4[i], LH4[i]);
        gr_q4_low->SetPointError(i, att_errors4[i], errLH4[i]);
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
    TCanvas *c_residuals_low = new TCanvas("c_residuals_low", "Residuals for Low Resolution", 1200, 800);

    // Primo sottoplot: Residui per mean_q1_low
    c_residuals_low->cd();
    TGraphErrors *gr_residuals_q1_low = new TGraphErrors(x_values.size());
    for (int i = 0; i < x_values.size(); i++) {
        double expected_q1_low = linear_q1_low->Eval(x1[i]);
        double residual_q1_low = LH1[i] - expected_q1_low;
        gr_residuals_q1_low->SetPoint(i, x1[i], residual_q1_low);
        gr_residuals_q1_low->SetPointError(i, 0, sigma_res1_low[i]);
    }
    
    TF1 *horiz_q1_low = new TF1("horiz_q1_low", "pol1", 0.0, 100.0);	// fit a retta dei residui (linea il più possibile orizzontale)
    gr_residuals_q1_low->SetMarkerStyle(20);
    gr_residuals_q1_low->SetMarkerColor(kPink+10);
    gr_residuals_q1_low->SetTitle("Residui canale 0;Carica [nC];Residui");
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

    // Secondo sottoplot: Residui per mean_q2_low
    TCanvas *c_residuals_low1 = new TCanvas("c_residuals_low1", "Residuals for Low Resolution", 1200, 800);
    c_residuals_low1->cd(); // Seleziona il secondo sottoplot
    TGraphErrors *gr_residuals_q2_low = new TGraphErrors(x_values.size());
    for (int i = 0; i < x_values.size(); i++) {
        double expected_q2_low = linear_q2_low->Eval(x2[i]);
        double residual_q2_low = LH2[i] - expected_q2_low;
        gr_residuals_q2_low->SetPoint(i, x2[i], residual_q2_low);
        gr_residuals_q2_low->SetPointError(i, 0, sigma_res2_low[i]);
    }

    TF1 *horiz_q2_low = new TF1("horiz_q2_low", "pol1", 0.0, 100.0);	// fit a retta dei residui (linea il più possibile orizzontale)
    gr_residuals_q2_low->SetMarkerStyle(20);
    gr_residuals_q2_low->SetMarkerColor(kAzure+1);
    gr_residuals_q2_low->SetTitle("Residui canale 1;Carica [nC];Residui");
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
    TCanvas *c_residuals_low2 = new TCanvas("c_residuals_low2", "Residuals for Low Resolution", 1200, 800);
    c_residuals_low2->cd(); // Seleziona il secondo sottoplot
    TGraphErrors *gr_residuals_q3_low = new TGraphErrors(x_values.size());
    for (int i = 0; i < x_values.size(); i++) {
        double expected_q3_low = linear_q3_low->Eval(x3[i]);
        double residual_q3_low = LH3[i] - expected_q3_low;
        gr_residuals_q3_low->SetPoint(i, x3[i], residual_q3_low);
        gr_residuals_q3_low->SetPointError(i, 0, sigma_res3_low[i]);
    }
    
    TF1 *horiz_q3_low = new TF1("horiz_q3_low", "pol1", 0.0, 100.0);	// fit a retta dei residui (linea il più possibile orizzontale)
    gr_residuals_q3_low->SetMarkerStyle(20);
    gr_residuals_q3_low->SetMarkerColor(kTeal);
    gr_residuals_q3_low->SetTitle("Residui canale 2;Carica [nC];Residui");
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
    TCanvas *c_residuals_low3 = new TCanvas("c_residuals_low3", "Residuals for Low Resolution", 1200, 800);
    c_residuals_low3->cd(); // Seleziona il secondo sottoplot
    TGraphErrors *gr_residuals_q4_low = new TGraphErrors(x_values.size());
    for (int i = 0; i < x_values.size(); i++) {
        double expected_q4_low = linear_q4_low->Eval(x4[i]);
        double residual_q4_low = LH4[i] - expected_q4_low;
        gr_residuals_q4_low->SetPoint(i, x4[i], residual_q4_low);
        gr_residuals_q4_low->SetPointError(i, 0, sigma_res4_low[i]);
    }
    
    TF1 *horiz_q4_low = new TF1("horiz_q4_low", "pol1", 0.0, 100.0);	// fit a retta dei residui (linea il più possibile orizzontale)
    gr_residuals_q4_low->SetMarkerStyle(20);
    gr_residuals_q4_low->SetMarkerColor(kOrange-3);
    gr_residuals_q4_low->SetTitle("Residui canale 3;Carica [nC];Residui");
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

// ### Istogrammi dei RESIDUI LOW ###
    TCanvas *c_histores = new TCanvas("c_histores", "Istogramma residui QDC_low", 1200, 800);
    
    // Istogramma dei residui per q1_low
    c_histores->cd();
    TH1F* h1 = new TH1F("h1", "Istogramma residui q1_low", 20, -20.0, 20.0);
    for (int i=0; i < x_values.size(); i++) {
	    float resid_della_ricca1 = (LH1[i]-linear_q1_low->Eval(x1[i]))/sigma_res1_low[i];
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
    gStyle->SetOptFit(1111);	// visualizzo le statistiche
    
    // Istogramma dei residui per q2_low
    TCanvas *c_histores1 = new TCanvas("c_histores1", "Istogramma residui QDC_low", 1200, 800);
    c_histores1->cd();
    TH1F* h2 = new TH1F("h2", "Istogramma residui q2_low", 20, -20.0, 20.0);
    for (int i=0; i < x_values.size(); i++) {
	    float resid_della_ricca2 = (LH2[i]-linear_q2_low->Eval(x2[i]))/sigma_res2_low[i];
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
    gStyle->SetOptFit(1111);
    
    // Istogramma dei residui per q3_low
    TCanvas *c_histores2 = new TCanvas("c_histores2", "Istogramma residui QDC_low", 1200, 800);
    c_histores2->cd();
    TH1F* h3 = new TH1F("h3", "Istogramma residui q3_low", 20, -20.0, 20.0);
    for (int i=0; i < x_values.size(); i++) {
	    float resid_della_ricca3 = (LH3[i]-linear_q3_low->Eval(x3[i]))/sigma_res3_low[i];
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
    gStyle->SetOptFit(1111);
    
    // Istogramma dei residui per q4_low
    TCanvas *c_histores3 = new TCanvas("c_histores3", "Istogramma residui QDC_low", 1200, 800);
    c_histores3->cd();
    TH1F* h4 = new TH1F("h4", "Istogramma residui q4_low", 20, -20.0, 20.0);
    for (int i=0; i < x_values.size(); i++) {
	    float resid_della_ricca4 = (LH4[i]-linear_q4_low->Eval(x4[i]))/sigma_res4_low[i];
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
    gStyle->SetOptFit(1111);
    
    // Salva la canvas come file PNG
    c_histores->Update();
    c_histores->SaveAs("../Dati/QDC/calibrazioneQDC_historesiduiLOW.png");
    return;
}
