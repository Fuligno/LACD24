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
            variance /= n;
            error = sqrt(variance / n);
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

    // ############# CALCOLO DEGLI ERRORI SUI RITARDI #############
    
   ifstream file("../Dati/QDC/Attenuazioni_calibrazione_QDC.txt");
    if (!file.is_open()) {
        cout << "Cannot open file!" << endl;
        return;
    }

    vector<double> att_errors1, att_errors2, att_errors3, att_errors4;
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
        gr_q1_low->SetPoint(i, x1[i], means_q1_low[i]);
        gr_q1_low->SetPointError(i, att_errors1[i], errors_q1_low[i]);
    }
    gr_q1_low->SetMarkerStyle(20);
    gr_q1_low->SetMarkerColor(kRed);
    gr_q1_low->SetLineColor(kRed);
    gr_q1_low->SetTitle("Calibrazione QDC;Carica [nC];Conteggi");
    gr_q1_low->SetMinimum(0);
    gr_q1_low->SetMaximum(4500);

    TF1 *linear_q1_low = new TF1("linear_q1_low", "pol1", 0, x_values.size());
    linear_q1_low->SetLineColor(kRed);
    gr_q1_low->Fit(linear_q1_low, "Q");
    gr_q1_low->SetStats(0);
    gr_q1_low->Draw("AP");
    linear_q1_low->Draw("same");

    // Scatter plot e fit per mean_q2
    TGraphErrors *gr_q2_low = new TGraphErrors(x_values.size());
    for (int i = 0; i < x_values.size(); i++) {
        gr_q2_low->SetPoint(i, x2[i], means_q2_low[i]);
        gr_q2_low->SetPointError(i, att_errors2[i], errors_q2_low[i]);
    }
    gr_q2_low->SetMarkerStyle(21);
    gr_q2_low->SetMarkerColor(kBlue);
    gr_q2_low->SetLineColor(kBlue);

    TF1 *linear_q2_low = new TF1("linear_q2_low", "pol1", 0, x_values.size());
    linear_q2_low->SetLineColor(kBlue);
    gr_q2_low->Fit(linear_q2_low, "Q");
    gr_q2_low->SetStats(0);
    gr_q2_low->Draw("P same");
    linear_q2_low->Draw("same");

    // Scatter plot e fit per mean_q3
    TGraphErrors *gr_q3_low = new TGraphErrors(x_values.size());
    for (int i = 0; i < x_values.size(); i++) {
        gr_q3_low->SetPoint(i, x3[i], means_q3_low[i]);
        gr_q3_low->SetPointError(i, att_errors3[i], errors_q3_low[i]);
    }
    gr_q3_low->SetMarkerStyle(22);
    gr_q3_low->SetMarkerColor(kGreen);
    gr_q3_low->SetLineColor(kGreen);

    TF1 *linear_q3_low = new TF1("linear_q3v", "pol1", 0, x_values.size());
    linear_q3_low->SetLineColor(kGreen);
    gr_q3_low->Fit(linear_q3_low, "Q");
    gr_q3_low->SetStats(0);
    gr_q3_low->Draw("P same");
    linear_q3_low->Draw("same");

    // Scatter plot e fit per mean_q4
    TGraphErrors *gr_q4_low = new TGraphErrors(x_values.size());
    for (int i = 0; i < x_values.size(); i++) {
        gr_q4_low->SetPoint(i, x4[i], means_q4_low[i]);
        gr_q4_low->SetPointError(i, att_errors4[i], errors_q4_low[i]);
    }
    gr_q4_low->SetMarkerStyle(23);
    gr_q4_low->SetMarkerColor(kMagenta);
    gr_q4_low->SetLineColor(kMagenta);

    TF1 *linear_q4_low = new TF1("linear_q4_low", "pol1", 0, x_values.size());
    linear_q4_low->SetLineColor(kMagenta);
    gr_q4_low->Fit(linear_q4_low, "Q");
    gr_q4_low->SetStats(0);
    gr_q4_low->Draw("P same");
    linear_q4_low->Draw("same");

    /////////////////////////////////////////////////////////////// HIGH
    // Scatter plot e fit per mean_q1
    TGraphErrors *gr_q1_high = new TGraphErrors(x_values.size());
    int k = 0;
    for (int i = 0; i < x_values.size(); i++) {
        if (means_q1_high[i]<=3840) {
            gr_q1_high->SetPoint(k, x1[i], means_q1_high[i]);
            gr_q1_high->SetPointError(k, att_errors1[i], errors_q1_high[i]);
            k++;
        }
    }
    gr_q1_high->SetMarkerStyle(20);
    gr_q1_high->SetMarkerColor(kRed);
    gr_q1_high->SetLineColor(kRed);

    TF1 *linear_q1_high = new TF1("linear_q1_high", "pol1", 0, x_values.size());
    linear_q1_high->SetLineColor(kRed);
    gr_q1_high->Fit(linear_q1_high, "Q");
    gr_q1_high->SetStats(0);
    gr_q1_high->Draw("P same");
    linear_q1_high->Draw("same");

    // Scatter plot e fit per mean_q2
    TGraphErrors *gr_q2_high = new TGraphErrors(x_values.size());
    k = 0;
    for (int i = 0; i < x_values.size(); i++) {
        if (means_q2_high[i]<=3840) {
            gr_q2_high->SetPoint(k, x2[i], means_q2_high[i]);
            gr_q2_high->SetPointError(k, att_errors2[i], errors_q2_high[i]);
            k++;
        }
    }
    gr_q2_high->SetMarkerStyle(21);
    gr_q2_high->SetMarkerColor(kBlue);
    gr_q2_high->SetLineColor(kBlue);

    TF1 *linear_q2_high = new TF1("linear_q2_high", "pol1", 0, x_values.size());
    linear_q2_high->SetLineColor(kBlue);
    gr_q2_high->Fit(linear_q2_high, "Q");
    gr_q2_high->SetStats(0);
    gr_q2_high->Draw("P same");
    linear_q2_high->Draw("same");

    // Scatter plot e fit per mean_q3
    TGraphErrors *gr_q3_high = new TGraphErrors(x_values.size());
    k = 0;
    for (int i = 0; i < x_values.size(); i++) {
        if (means_q3_high[i]<=3840) {
            gr_q3_high->SetPoint(k, x3[i], means_q3_high[i]);
            gr_q3_high->SetPointError(k, att_errors3[i], errors_q3_high[i]);
            k++;
        }
    }
    gr_q3_high->SetMarkerStyle(22);
    gr_q3_high->SetMarkerColor(kGreen);
    gr_q3_high->SetLineColor(kGreen);

    TF1 *linear_q3_high = new TF1("linear_q3v", "pol1", 0, x_values.size());
    linear_q3_high->SetLineColor(kGreen);
    gr_q3_high->Fit(linear_q3_high, "Q");
    gr_q3_high->SetStats(0);
    gr_q3_high->Draw("P same");
    linear_q3_high->Draw("same");

    // Scatter plot e fit per mean_q4
    TGraphErrors *gr_q4_high = new TGraphErrors(x_values.size());
    k = 0;
    for (int i = 0; i < x_values.size(); i++) {
        if (means_q4_high[i]<=3840) {
            gr_q4_high->SetPoint(k, x4[i], means_q4_high[i]);
            gr_q4_high->SetPointError(k, att_errors4[i], errors_q4_high[i]);
            k++;
        }
    }
    gr_q4_high->SetMarkerStyle(23);
    gr_q4_high->SetMarkerColor(kMagenta);
    gr_q4_high->SetLineColor(kMagenta);

    TF1 *linear_q4_high = new TF1("linear_q4_high", "pol1", 0, x_values.size());
    linear_q4_high->SetLineColor(kMagenta);
    gr_q4_high->Fit(linear_q4_high, "Q");
    gr_q4_high->SetStats(0);
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

    statsBox->AddText(Form("q1_low: m = %.4f ± %.4f, q = %.5f ± %.4f", m_q1_low, m_q1_err_low, q_q1_low, q_q1_err_low));
    statsBox->AddText(Form("q2_low: m = %.4f ± %.4f, q = %.5f ± %.4f", m_q2_low, m_q2_err_low, q_q2_low, q_q2_err_low));
    statsBox->AddText(Form("q3_low: m = %.4f ± %.4f, q = %.5f ± %.4f", m_q3_low, m_q3_err_low, q_q3_low, q_q3_err_low));
    statsBox->AddText(Form("q4_low: m = %.4f ± %.4f, q = %.5f ± %.4f", m_q4_low, m_q4_err_low, q_q4_low, q_q4_err_low));
    statsBox->AddText(Form("q1_high: m = %.4f ± %.4f, q = %.4f ± %.4f", m_q1_high, m_q1_err_high, q_q1_high, q_q1_err_high));
    statsBox->AddText(Form("q2_high: m = %.4f ± %.4f, q = %.4f ± %.4f", m_q2_high, m_q2_err_high, q_q2_high, q_q2_err_high));
    statsBox->AddText(Form("q3_high: m = %.4f ± %.4f, q = %.4f ± %.4f", m_q3_high, m_q3_err_high, q_q3_high, q_q3_err_high));
    statsBox->AddText(Form("q4_high: m = %.4f ± %.4f, q = %.4f ± %.4f", m_q4_high, m_q4_err_high, q_q4_high, q_q4_err_high));

    statsBox->Draw();

    // Salva la canvas come file PNG
    c_calibr->Update();
    c_calibr->SaveAs("../Dati/QDC/fit_calibrazioneQDC.png");

//////////////////////////////////////////////////////////////////////////////////

    TF1 *line_0 = new TF1("line_0", "[0]*x", -1, 100);
    line_0->SetLineColor(kBlack);

// RESIDUI LOW 

    // Crea una canvas per i residui low
    TCanvas *c_residuals_low = new TCanvas("c_residuals_low", "Residuals for Low Resolution", 1200, 800);
    c_residuals_low->Divide(2, 2); // Divide in 4 sottoplot (2x2)

    // Primo sottoplot: Residui per mean_q1_low
    c_residuals_low->cd(1);
    c_residuals_low->cd(1)->SetGrid();
    TGraphErrors *gr_residuals_q1_low = new TGraphErrors(x_values.size());
    for (int i = 0; i < x_values.size(); i++) {
        double expected_q1_low = linear_q1_low->Eval(x1[i]);
        double residual_q1_low = means_q1_low[i] - expected_q1_low;
        gr_residuals_q1_low->SetPoint(i, x1[i], residual_q1_low);
        gr_residuals_q1_low->SetPointError(i, 0, errors_q1_low[i]);
    }
    gr_residuals_q1_low->SetMarkerStyle(20);
    gr_residuals_q1_low->SetMarkerColor(kRed);
    gr_residuals_q1_low->SetLineColor(kRed);
    gr_residuals_q1_low->SetTitle("Residui q1 (Low Resolution);Carica [nC];Residui");
    gr_residuals_q1_low->Draw("AP");
    line_0->Draw("l same");

    // Secondo sottoplot: Residui per mean_q2_low
    c_residuals_low->cd(2); // Seleziona il secondo sottoplot
    c_residuals_low->cd(2)->SetGrid();
    TGraphErrors *gr_residuals_q2_low = new TGraphErrors(x_values.size());
    for (int i = 0; i < x_values.size(); i++) {
        double expected_q2_low = linear_q2_low->Eval(x2[i]);
        double residual_q2_low = means_q2_low[i] - expected_q2_low;
        gr_residuals_q2_low->SetPoint(i, x2[i], residual_q2_low);
        gr_residuals_q2_low->SetPointError(i, 0, errors_q2_low[i]);
    }
    gr_residuals_q2_low->SetMarkerStyle(21);
    gr_residuals_q2_low->SetMarkerColor(kBlue);
    gr_residuals_q2_low->SetLineColor(kBlue);
    gr_residuals_q2_low->SetTitle("Residui q2 (Low Resolution);Carica [nC];Residui");
    gr_residuals_q2_low->Draw("AP");
    line_0->Draw("l same");

    // Terzo sottoplot: Residui per mean_q3_low
    c_residuals_low->cd(3); // Seleziona il terzo sottoplot
    c_residuals_low->cd(3)->SetGrid();
    TGraphErrors *gr_residuals_q3_low = new TGraphErrors(x_values.size());
    for (int i = 0; i < x_values.size(); i++) {
        double expected_q3_low = linear_q3_low->Eval(x3[i]);
        double residual_q3_low = means_q3_low[i] - expected_q3_low;
        gr_residuals_q3_low->SetPoint(i, x3[i], residual_q3_low);
        gr_residuals_q3_low->SetPointError(i, 0, errors_q3_low[i]);
    }
    gr_residuals_q3_low->SetMarkerStyle(22);
    gr_residuals_q3_low->SetMarkerColor(kGreen);
    gr_residuals_q3_low->SetLineColor(kGreen);
    gr_residuals_q3_low->SetTitle("Residui q3 (Low Resolution);Carica [nC];Residui");
    gr_residuals_q3_low->Draw("AP");
    line_0->Draw("l same");

    // Quarto sottoplot: Residui per mean_q4_low
    c_residuals_low->cd(4); // Seleziona il quarto sottoplot
    c_residuals_low->cd(4)->SetGrid();
    TGraphErrors *gr_residuals_q4_low = new TGraphErrors(x_values.size());
    for (int i = 0; i < x_values.size(); i++) {
        double expected_q4_low = linear_q4_low->Eval(x4[i]);
        double residual_q4_low = means_q4_low[i] - expected_q4_low;
        gr_residuals_q4_low->SetPoint(i, x4[i], residual_q4_low);
        gr_residuals_q4_low->SetPointError(i, 0, errors_q4_low[i]);
    }
    gr_residuals_q4_low->SetMarkerStyle(23);
    gr_residuals_q4_low->SetMarkerColor(kMagenta);
    gr_residuals_q4_low->SetLineColor(kMagenta);
    gr_residuals_q4_low->SetTitle("Residui q4 (Low Resolution);Carica [nC];Residui");
    gr_residuals_q4_low->Draw("AP");
    line_0->Draw("l same");

    // Salva la canvas come file PNG
    c_residuals_low->Update();
    c_residuals_low->SaveAs("../Dati/QDC/residuals_calibrazioneQDC_low.png");


//////////////////////////////////////////////////////////////////////////////////
// RESIDUI HIGH

    // Crea una canvas per i residui high
    TCanvas *c_residuals_high = new TCanvas("c_residuals_high", "Residuals for High Resolution", 1200, 800);
    c_residuals_high->Divide(2, 2); // Divide in 4 sottoplot (2x2)

    // Primo sottoplot: Residui per mean_q1_high
    c_residuals_high->cd(1); // Seleziona il primo sottoplot
    c_residuals_high->cd(1)->SetGrid();
    TGraphErrors *gr_residuals_q1_high = new TGraphErrors(x_values.size());
    int j = 0;
    for (int i = 0; i < x_values.size(); i++) {
        if (means_q1_high[i] <= 3840) {
            double expected_q1_high = linear_q1_high->Eval(x1[i]);
            double residual_q1_high = means_q1_high[i] - expected_q1_high;
            gr_residuals_q1_high->SetPoint(j, x1[i], residual_q1_high);
            gr_residuals_q1_high->SetPointError(j, 0, errors_q1_high[i]);
            j++;
        }
    }
    gr_residuals_q1_high->SetMarkerStyle(20);
    gr_residuals_q1_high->SetMarkerColor(kRed);
    gr_residuals_q1_high->SetLineColor(kRed);
    gr_residuals_q1_high->SetTitle("Residui q1 (High Resolution);Carica [nC];Residui");
    gr_residuals_q1_high->Draw("AP");
    line_0->Draw("l same");

    // Secondo sottoplot: Residui per mean_q2_high
    c_residuals_high->cd(2); // Seleziona il secondo sottoplot
    c_residuals_high->cd(2)->SetGrid();
    TGraphErrors *gr_residuals_q2_high = new TGraphErrors(x_values.size());
    j = 0;
    for (int i = 0; i < x_values.size(); i++) {
        if (means_q2_high[i] <= 3840) {
            double expected_q2_high = linear_q2_high->Eval(x2[i]);
            double residual_q2_high = means_q2_high[i] - expected_q2_high;
            gr_residuals_q2_high->SetPoint(j, x2[i], residual_q2_high);
            gr_residuals_q2_high->SetPointError(j, 0, errors_q2_high[i]);
            j++;
        }
    }
    gr_residuals_q2_high->SetMarkerStyle(21);
    gr_residuals_q2_high->SetMarkerColor(kBlue);
    gr_residuals_q2_high->SetLineColor(kBlue);
    gr_residuals_q2_high->SetTitle("Residui q2 (High Resolution);Carica [nC];Residui");
    gr_residuals_q2_high->Draw("AP");
    line_0->Draw("l same");

    // Terzo sottoplot: Residui per mean_q3_high
    c_residuals_high->cd(3); // Seleziona il terzo sottoplot
    c_residuals_high->cd(3)->SetGrid();
    TGraphErrors *gr_residuals_q3_high = new TGraphErrors(x_values.size());
    j = 0;
    for (int i = 0; i < x_values.size(); i++) {
        if (means_q3_high[i] <= 3840) {
            double expected_q3_high = linear_q3_high->Eval(x3[i]);
            double residual_q3_high = means_q3_high[i] - expected_q3_high;
            gr_residuals_q3_high->SetPoint(j, x3[i], residual_q3_high);
            gr_residuals_q3_high->SetPointError(j, 0, errors_q3_high[i]);
            j++;
        }
    }
    gr_residuals_q3_high->SetMarkerStyle(22);
    gr_residuals_q3_high->SetMarkerColor(kGreen);
    gr_residuals_q3_high->SetLineColor(kGreen);
    gr_residuals_q3_high->SetTitle("Residui q3 (High Resolution);Carica [nC];Residui");
    gr_residuals_q3_high->Draw("AP");
    line_0->Draw("l same");

    // Quarto sottoplot: Residui per mean_q4_high
    c_residuals_high->cd(4); // Seleziona il quarto sottoplot
    c_residuals_high->cd(4)->SetGrid();
    TGraphErrors *gr_residuals_q4_high = new TGraphErrors(x_values.size());
    j = 0;
    for (int i = 0; i < x_values.size(); i++) {
        if (means_q4_high[i] <= 3840) {
            double expected_q4_high = linear_q4_high->Eval(x4[i]);
            double residual_q4_high = means_q4_high[i] - expected_q4_high;
            gr_residuals_q4_high->SetPoint(j, x4[i], residual_q4_high);
            gr_residuals_q4_high->SetPointError(j, 0, errors_q4_high[i]);
            j++;
        }
    }
    gr_residuals_q4_high->SetMarkerStyle(23);
    gr_residuals_q4_high->SetMarkerColor(kMagenta);
    gr_residuals_q4_high->SetLineColor(kMagenta);
    gr_residuals_q4_high->SetTitle("Residui q4 (High Resolution);Carica [nC];Residui");
    gr_residuals_q4_high->Draw("AP");
    line_0->Draw("l same");

    // Salva la canvas come file PNG
    c_residuals_high->Update();
    c_residuals_high->SaveAs("../Dati/QDC/residuals_calibrazioneQDC_high.png");

    return;
}