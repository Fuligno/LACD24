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

void calibrazioneTDC() {
    // Open the input file containing the list of .txt files
    ifstream inputFile("../Dati/TDC/filenameTDC.txt");
    if (!inputFile.is_open()) {
        cerr << "Error opening file filenameTDC.txt" << endl;
        return;
    }

    // Vettori per memorizzare i risultati
    vector<string> filenames;
    vector<double> means_t1, errors_t1;
    vector<double> means_t2, errors_t2;
    vector<double> means_t3, errors_t3;
    vector<double> means_t4, errors_t4;
    vector<double> x_values;

    string line;
    while (getline(inputFile, line)) {
        string filePath = "../Dati/TDC/" + line;
        ifstream dataFile(filePath);
        if (!dataFile.is_open()) {
            cerr << "Error opening data file " << line << endl;
            continue;
        }

        // Extract x value from filename
        string x_str = line.substr(line.size() - 7, 3);  // Use the last three characters before the extension
        double x_value = stod(x_str) / 10.0 + 21.2;

        vector<double> col1, col2, col3, col4;
        double val1, val2, val3, val4, val5;
        while (dataFile >> val1 >> val2 >> val3 >> val4 >> val5) {
            col1.push_back(val1);
            col2.push_back(val2);
            col3.push_back(val3);
            col4.push_back(val4);
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

        double mean_t1, err_t1, mean_t2, err_t2, mean_t3, err_t3, mean_t4, err_t4;
        calculateMeanAndError(col1, mean_t1, err_t1);
        calculateMeanAndError(col2, mean_t2, err_t2);
        calculateMeanAndError(col3, mean_t3, err_t3);
        calculateMeanAndError(col4, mean_t4, err_t4);

        // Memorizza i risultati nei vettori
        filenames.push_back(line);
        means_t1.push_back(mean_t1);
        errors_t1.push_back(err_t1);
        means_t2.push_back(mean_t2);
        errors_t2.push_back(err_t2);
        means_t3.push_back(mean_t3);
        errors_t3.push_back(err_t3);
        means_t4.push_back(mean_t4);
        errors_t4.push_back(err_t4);
        x_values.push_back(x_value);
    }

    // Chiudi il file di input
    inputFile.close();

    // Open the text file for writing
    ofstream textFile("../Dati/TDC/calibrazioneTDC_results.txt");
    if (!textFile.is_open()) {
        cerr << "Error opening text file for writing" << endl;
        return;
    }
    // Write header to the text file
    textFile << "Filename\tMean_t1\tErr_t1\tMean_t2\tErr_t2\tMean_t3\tErr_t3\tMean_t4\tErr_t4\tx_value\n";

    // Scrivi i risultati dai vettori nel file di testo
    for (size_t i = 0; i < filenames.size(); ++i) {
        textFile << filenames[i] << "\t"
                << means_t1[i] << "\t" << errors_t1[i] << "\t"
                << means_t2[i] << "\t" << errors_t2[i] << "\t"
                << means_t3[i] << "\t" << errors_t3[i] << "\t"
                << means_t4[i] << "\t" << errors_t4[i] << "\t"
                << x_values[i] << "\n";
    }

    // Finalize text file
    textFile.close();
///////////////////////////////////////////////////////////////////////////////
    // Crea una canvas
    TCanvas *c_calibr = new TCanvas("c_calibr", "Calibration Fits", 1200, 800);
    c_calibr->cd();
    c_calibr->SetGrid();
    
    // Scatter plot e fit per mean_t1
    TGraphErrors *gr_t1 = new TGraphErrors(x_values.size());
    for (int i = 0; i < x_values.size(); i++) {
        gr_t1->SetPoint(i, x_values[i], means_t1[i]);
        gr_t1->SetPointError(i, 0, errors_t1[i]);
    }
    gr_t1->SetMarkerStyle(20);
    gr_t1->SetMarkerColor(kRed);
    gr_t1->SetLineColor(kRed);
    gr_t1->SetTitle("Calibrazione TDC;Ritardi [ns];Conteggi");

    TF1 *linear_t1 = new TF1("linear_t1", "pol1", 0, x_values.size());
    linear_t1->SetLineColor(kRed);
    gr_t1->Fit(linear_t1, "Q");
    gr_t1->SetStats(0);
    gr_t1->Draw("AP");
    linear_t1->Draw("same");

    // Scatter plot e fit per mean_t2
    TGraphErrors *gr_t2 = new TGraphErrors(x_values.size());
    for (int i = 0; i < x_values.size(); i++) {
        gr_t2->SetPoint(i, x_values[i], means_t2[i]);
        gr_t2->SetPointError(i, 0, errors_t2[i]);
    }
    gr_t2->SetMarkerStyle(21);
    gr_t2->SetMarkerColor(kBlue);
    gr_t2->SetLineColor(kBlue);

    TF1 *linear_t2 = new TF1("linear_t2", "pol1", 0, x_values.size());
    linear_t2->SetLineColor(kBlue);
    gr_t2->Fit(linear_t2, "Q");
    gr_t2->SetStats(0);
    gr_t2->Draw("P same");
    linear_t2->Draw("same");

    // Scatter plot e fit per mean_t3
    TGraphErrors *gr_t3 = new TGraphErrors(x_values.size());
    for (int i = 0; i < x_values.size(); i++) {
        gr_t3->SetPoint(i, x_values[i], means_t3[i]);
        gr_t3->SetPointError(i, 0, errors_t3[i]);
    }
    gr_t3->SetMarkerStyle(22);
    gr_t3->SetMarkerColor(kGreen);
    gr_t3->SetLineColor(kGreen);

    TF1 *linear_t3 = new TF1("linear_t3", "pol1", 0, x_values.size());
    linear_t3->SetLineColor(kGreen);
    gr_t3->Fit(linear_t3, "Q");
    gr_t3->SetStats(0);
    gr_t3->Draw("P same");
    linear_t3->Draw("same");

    // Scatter plot e fit per mean_t4
    TGraphErrors *gr_t4 = new TGraphErrors(x_values.size());
    for (int i = 0; i < x_values.size(); i++) {
        gr_t4->SetPoint(i, x_values[i], means_t4[i]);
        gr_t4->SetPointError(i, 0, errors_t4[i]);
    }
    gr_t4->SetMarkerStyle(23);
    gr_t4->SetMarkerColor(kMagenta);
    gr_t4->SetLineColor(kMagenta);

    TF1 *linear_t4 = new TF1("linear_t4", "pol1", 0, x_values.size());
    linear_t4->SetLineColor(kMagenta);
    gr_t4->Fit(linear_t4, "Q");
    gr_t4->SetStats(0);
    gr_t4->Draw("P same");
    linear_t4->Draw("same");

    // Aggiungi il box con le statistiche
    TPaveText *statsBox = new TPaveText(0.5, 0.1, 0.9, 0.5, "NDC");
    statsBox->SetBorderSize(1);
    statsBox->SetFillColor(kWhite);
    statsBox->SetTextAlign(12);
    statsBox->SetTextSize(0.03);

    // Ottieni i parametri di fit
    double m_t1 = linear_t1->GetParameter(1);
    double m_t1_err = linear_t1->GetParError(1);
    double q_t1 = linear_t1->GetParameter(0);
    double q_t1_err = linear_t1->GetParError(0);

    double m_t2 = linear_t2->GetParameter(1);
    double m_t2_err = linear_t2->GetParError(1);
    double q_t2 = linear_t2->GetParameter(0);
    double q_t2_err = linear_t2->GetParError(0);

    double m_t3 = linear_t3->GetParameter(1);
    double m_t3_err = linear_t3->GetParError(1);
    double q_t3 = linear_t3->GetParameter(0);
    double q_t3_err = linear_t3->GetParError(0);

    double m_t4 = linear_t4->GetParameter(1);
    double m_t4_err = linear_t4->GetParError(1);
    double q_t4 = linear_t4->GetParameter(0);
    double q_t4_err = linear_t4->GetParError(0);

    statsBox->AddText(Form("Fit Statistics:"));
    statsBox->AddText(Form("t1: m = %.4f ± %.4f, q = %.4f ± %.4f", m_t1, m_t1_err, q_t1, q_t1_err));
    statsBox->AddText(Form("t2: m = %.4f ± %.4f, q = %.4f ± %.4f", m_t2, m_t2_err, q_t2, q_t2_err));
    statsBox->AddText(Form("t3: m = %.4f ± %.4f, q = %.4f ± %.4f", m_t3, m_t3_err, q_t3, q_t3_err));
    statsBox->AddText(Form("t4: m = %.4f ± %.4f, q = %.4f ± %.4f", m_t4, m_t4_err, q_t4, q_t4_err));

    statsBox->Draw();

    // Salva la canvas come file PNG
    c_calibr->Update();
    c_calibr->SaveAs("../Dati/TDC/fit_calibrazioneTDC.png");

    ///////////////////////////////////////////////////////////////////////////
    // Plot dei residui

    TF1 *line_0 = new TF1("line_0", "[0]*x", -1, 100);
    line_0->SetLineColor(kBlack);

    TCanvas *c_residui = new TCanvas("c_residui", "Residui TDC", 1200, 800);
    c_residui->Divide(2, 2);

    // Residui per t1
    c_residui->cd(1);
    c_residui->cd(1)->SetGrid();
    TGraphErrors *residui_t1 = new TGraphErrors(x_values.size());
    for (int i = 0; i < x_values.size(); i++) {
        double fit_value = linear_t1->Eval(x_values[i]);
        double residual = (means_t1[i] - fit_value);
        residui_t1->SetPoint(i, x_values[i], residual);
        residui_t1->SetPointError(i, 0, errors_t1[i]);
    }
    residui_t1->SetTitle("Residui t1;Ritardi [ns];Residui");
    residui_t1->SetMarkerStyle(20);
    residui_t1->SetMarkerColor(kRed);
    residui_t1->Draw("AP");
    line_0->Draw("same");

    // Residui per t2
    c_residui->cd(2);
    c_residui->cd(2)->SetGrid();
    TGraphErrors *residui_t2 = new TGraphErrors(x_values.size());
    for (int i = 0; i < x_values.size(); i++) {
        double fit_value = linear_t2->Eval(x_values[i]);
        double residual = (means_t2[i] - fit_value);
        residui_t2->SetPoint(i, x_values[i], residual);
        residui_t2->SetPointError(i, 0, errors_t2[i]);
    }
    residui_t2->SetTitle("Residui t2;Ritardi [ns];Residui");
    residui_t2->SetMarkerStyle(21);
    residui_t2->SetMarkerColor(kBlue);
    residui_t2->Draw("AP");
    line_0->Draw("same");

    // Residui per t3
    c_residui->cd(3);
    c_residui->cd(3)->SetGrid();
    TGraphErrors *residui_t3 = new TGraphErrors(x_values.size());
    for (int i = 0; i < x_values.size(); i++) {
        double fit_value = linear_t3->Eval(x_values[i]);
        double residual = (means_t3[i] - fit_value);
        residui_t3->SetPoint(i, x_values[i], residual);
        residui_t3->SetPointError(i, 0, errors_t3[i]);
    }
    residui_t3->SetTitle("Residui t3;Ritardi [ns];Residui");
    residui_t3->SetMarkerStyle(22);
    residui_t3->SetMarkerColor(kGreen);
    residui_t3->Draw("AP");
    line_0->Draw("same");

    // Residui per t4
    c_residui->cd(4);
    c_residui->cd(4)->SetGrid();
    TGraphErrors *residui_t4 = new TGraphErrors(x_values.size());
    for (int i = 0; i < x_values.size(); i++) {
        double fit_value = linear_t4->Eval(x_values[i]);
        double residual = (means_t4[i] - fit_value);
        residui_t4->SetPoint(i, x_values[i], residual);
        residui_t4->SetPointError(i, 0, errors_t4[i]);
    }
    residui_t4->SetTitle("Residui t4;Ritardi [ns];Residui");
    residui_t4->SetMarkerStyle(23);
    residui_t4->SetMarkerColor(kMagenta);
    residui_t4->Draw("AP");
    line_0->Draw("same");

    // Salva la canvas dei residui come file PNG
    c_residui->Update();
    c_residui->SaveAs("../Dati/TDC/calibrazioneTDC_residui.png");
}
