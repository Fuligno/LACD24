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

    int count = filenames.size();	// salvo il numero di eventi
    cout << count << endl;

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
    
    // ############# CALCOLO DEGLI ERRORI SUI RITARDI #############
    
    ifstream file("../Dati/TDC/Ritardi_calibrazione_TDC.txt");	// apro il file con tutti i ritardi e le levette usate
    if (!file.is_open()) {
        cout << "Cannot open file!" << endl;
    return;
    }
    vector<double> rit_errors;	// vettore per contenere gli errori sui ritardi
    string filename;
    double rit05, rit1, rit2, rit4, rit8, rit16, rit32;	// errori sul ritardo introdotto da ciascuna levetta
    getline(file, line); // Skip the header line
    while (file.is_open()) {
    
    file >> filename >> rit05 >> rit1 >> rit2 >> rit4 >> rit8 >> rit16 >> rit32;
    if (file.eof()) break;	// se arrivo alla fine del file, esco dal while
    
    double err_value = rit05+rit1+rit2+rit4+rit8+rit16+rit32;
    rit_errors.push_back(err_value);
   }
     
file.close();
    // ############## FINE CALCOLO DEGLI ERRORI SUI RITARDI ##################



///////////////////////////////////////////////////////////////////////////////
    // Crea una canvas
    TCanvas *c_calibr = new TCanvas("c_calibr", "Calibration Fits", 1200, 800);
    c_calibr->cd();
    c_calibr->SetGrid();
    
    // Scatter plot e fit per mean_t1
    TGraphErrors *gr_t1 = new TGraphErrors(x_values.size());
    for (int i = 0; i < x_values.size(); i++) {
        gr_t1->SetPoint(i, x_values[i], means_t1[i]);
        //gr_t1->SetPointError(i, 0, errors_t1[i]);
	   gr_t1->SetPointError(i, rit_errors[i], errors_t1[i]);
    }
    gr_t1->SetMarkerStyle(20);
    gr_t1->SetMarkerColor(kRed);
    gr_t1->SetLineColor(kRed);
    gr_t1->SetTitle("Calibrazione TDC;Ritardi [ns];Conteggi");

    TF1 *linear_t1 = new TF1("linear_t1", "pol1", 0.0, 100.0);	// definisco l'intervallo della funzione di fit tra 0.0 e 100.0 per coprire tutti quanti i punti sperimentali
    linear_t1->SetLineColor(kRed);
    gr_t1->Fit(linear_t1, "Q");
    //gr_t1->SetStats(0);
    gr_t1->Draw("AP");
    linear_t1->Draw("same");

    // Scatter plot e fit per mean_t2
    TGraphErrors *gr_t2 = new TGraphErrors(x_values.size());
    for (int i = 0; i < x_values.size(); i++) {
        gr_t2->SetPoint(i, x_values[i], means_t2[i]);
        //gr_t2->SetPointError(i, 0, errors_t2[i]);
	   gr_t2->SetPointError(i, rit_errors[i], errors_t2[i]);
    }
    gr_t2->SetMarkerStyle(21);
    gr_t2->SetMarkerColor(kGreen);
    gr_t2->SetLineColor(kGreen);

    TF1 *linear_t2 = new TF1("linear_t2", "pol1", 0.0, 100.0);
    linear_t2->SetLineColor(kGreen);
    gr_t2->Fit(linear_t2, "Q");
    //gr_t2->SetStats(0);
    gr_t2->Draw("P same");
    linear_t2->Draw("same");

    // Scatter plot e fit per mean_t3
    TGraphErrors *gr_t3 = new TGraphErrors(x_values.size());
    for (int i = 0; i < x_values.size(); i++) {
        gr_t3->SetPoint(i, x_values[i], means_t3[i]);
        //gr_t3->SetPointError(i, 0, errors_t3[i]);
	   gr_t3->SetPointError(i, rit_errors[i], errors_t3[i]);
    }
    gr_t3->SetMarkerStyle(22);
    gr_t3->SetMarkerColor(kCyan);
    gr_t3->SetLineColor(kCyan);

    TF1 *linear_t3 = new TF1("linear_t3", "pol1", 0.0, 100.0);
    linear_t3->SetLineColor(kCyan);
    gr_t3->Fit(linear_t3, "Q");
    //gr_t3->SetStats(0);
    gr_t3->Draw("P same");
    linear_t3->Draw("same");

    // Scatter plot e fit per mean_t4
    TGraphErrors *gr_t4 = new TGraphErrors(x_values.size());
    for (int i = 0; i < x_values.size(); i++) {
        gr_t4->SetPoint(i, x_values[i], means_t4[i]);
        //gr_t4->SetPointError(i, 0, errors_t4[i]);
	   gr_t4->SetPointError(i, rit_errors[i], errors_t4[i]);
    }
    gr_t4->SetMarkerStyle(23);
    gr_t4->SetMarkerColor(kViolet);
    gr_t4->SetLineColor(kViolet);

    TF1 *linear_t4 = new TF1("linear_t4", "pol1", 0.0, 100.0);
    linear_t4->SetLineColor(kViolet);
    gr_t4->Fit(linear_t4, "Q");
    //gr_t4->SetStats(0);
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
    statsBox->AddText(Form("t1: m = %.3f +/- %.3f, q = %.3f +/- %.3f", m_t1, m_t1_err, q_t1, q_t1_err));
    statsBox->AddText(Form("t2: m = %.3f +/- %.3f, q = %.3f +/- %.3f", m_t2, m_t2_err, q_t2, q_t2_err));
    statsBox->AddText(Form("t3: m = %.3f +/- %.3f, q = %.3f +/- %.3f", m_t3, m_t3_err, q_t3, q_t3_err));
    statsBox->AddText(Form("t4: m = %.3f +/- %.3f, q = %.3f +/- %.3f", m_t4, m_t4_err, q_t4, q_t4_err));

    statsBox->Draw();
    
    TLegend* leg = new TLegend(0.2,0.65,0.5,0.8);		// costruzione della legenda
    leg->AddEntry(gr_t1, "Canale 0");
    leg->AddEntry(gr_t2, "Canale 1");
    leg->AddEntry(gr_t3, "Canale 2");
    leg->AddEntry(gr_t4, "Canale 3");
    leg->Draw();

    // Salva la canvas come file PNG
    c_calibr->Update();
    c_calibr->SaveAs("../Dati/TDC/fit_calibrazioneTDC.png");






    ///////////////////////////////////////////////////////////////////////////
    // Plot dei residui
    
    vector<double> sigma_res1, sigma_res2, sigma_res3, sigma_res4;		// vettore che contiene gli errori sui residui
    double sigma_res_value1, sigma_res_value2, sigma_res_value3, sigma_res_value4;
    cout << "Dimensione vettore ritardi: " << x_values.size() << endl;
    cout << "Dimensione vettore errori ritardi: " << rit_errors.size() << endl;
    cout << "Dimensione vettore valori in y: " << means_t1.size() << endl;
    cout << "Dimensione vettore errori valori in y: " << errors_t1.size() << endl;
    
    // --- parte per calcolare gli errori sui residui ---
    for (int i=0; i < x_values.size(); i++) {
	    sigma_res_value1 = sqrt(errors_t1[i]*errors_t1[i] + rit_errors[i]*linear_t1->Derivative(x_values[i])*rit_errors[i]*linear_t1->Derivative(x_values[i]));	// propagazione degli errori per includere sia errore su x che su y. Serve per calcolare correttamente chiquadro/ngf
	    sigma_res_value2 = sqrt(errors_t2[i]*errors_t2[i] + rit_errors[i]*linear_t2->Derivative(x_values[i])*rit_errors[i]*linear_t2->Derivative(x_values[i]));
	    sigma_res_value3 = sqrt(errors_t3[i]*errors_t3[i] + rit_errors[i]*linear_t3->Derivative(x_values[i])*rit_errors[i]*linear_t3->Derivative(x_values[i]));
	    sigma_res_value4 = sqrt(errors_t4[i]*errors_t4[i] + rit_errors[i]*linear_t4->Derivative(x_values[i])*rit_errors[i]*linear_t4->Derivative(x_values[i]));
	    sigma_res1.push_back(sigma_res_value1);
	    sigma_res2.push_back(sigma_res_value2);
	    sigma_res3.push_back(sigma_res_value3);
	    sigma_res4.push_back(sigma_res_value4);
    }

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
        residui_t1->SetPointError(i, 0, sigma_res1[i]);
	   //residui_t1->SetPointError(i, 0, errors_t1[i]);
    }
    
    TF1 *horiz1 = new TF1("horiz_t1", "pol1", 0.0, 100.0);	// fit a retta dei residui (linea il più possibile orizzontale)
    horiz1->SetLineColor(kRed);
    residui_t1->SetTitle("Residui t1;Ritardi [ns];Residui");
    residui_t1->SetMarkerStyle(20);
    residui_t1->SetMarkerColor(kRed);
    residui_t1->Draw("AP");
    //line_0->Draw("same");
    residui_t1->Fit("horiz_t1");
    horiz1->Draw("same");
    
    TPaveText *eqretta1 = new TPaveText(0.5, 0.1, 0.9, 0.2, "NDC");
    eqretta1->SetBorderSize(1);
    eqretta1->SetFillColor(kWhite);
    eqretta1->SetTextAlign(20);
    eqretta1->SetTextSize(0.03);
    double mr_t1 = horiz1->GetParameter(1);
    double mr_t1_err = horiz1->GetParError(1);
    double qr_t1 = horiz1->GetParameter(0);
    double qr_t1_err = horiz1->GetParError(0);
    eqretta1->AddText(Form("m = %.3f +/- %.3f, q = %.3f +/- %.3f", mr_t1, mr_t1_err, qr_t1, qr_t1_err));
    eqretta1->Draw("same");

    // Residui per t2
    c_residui->cd(2);
    c_residui->cd(2)->SetGrid();
    TGraphErrors *residui_t2 = new TGraphErrors(x_values.size());
    for (int i = 0; i < x_values.size(); i++) {
        double fit_value = linear_t2->Eval(x_values[i]);
        double residual = (means_t2[i] - fit_value);
        residui_t2->SetPoint(i, x_values[i], residual);
        residui_t2->SetPointError(i, 0, sigma_res2[i]);
	   //residui_t2->SetPointError(i, 0, errors_t2[i]);
    }
    
    TF1 *horiz2 = new TF1("horiz_t2", "pol1", 0.0, 100.0);	// fit a retta dei residui
    horiz2->SetLineColor(kBlue);
    residui_t2->SetTitle("Residui t2;Ritardi [ns];Residui");
    residui_t2->SetMarkerStyle(21);
    residui_t2->SetMarkerColor(kBlue);
    residui_t2->Draw("AP");
    //line_0->Draw("same");
    residui_t2->Fit("horiz_t2");
    horiz2->Draw("same");
    
    TPaveText *eqretta2 = new TPaveText(0.5, 0.1, 0.9, 0.2, "NDC");
    eqretta1->SetBorderSize(1);
    eqretta1->SetFillColor(kWhite);
    eqretta1->SetTextAlign(20);
    eqretta1->SetTextSize(0.03);
    double mr_t2 = horiz2->GetParameter(1);
    double mr_t2_err = horiz2->GetParError(1);
    double qr_t2 = horiz2->GetParameter(0);
    double qr_t2_err = horiz2->GetParError(0);
    eqretta2->AddText(Form("m = %.3f +/- %.3f, q = %.3f +/- %.3f", mr_t2, mr_t2_err, qr_t2, qr_t2_err));
    eqretta2->Draw("same");

    // Residui per t3
    c_residui->cd(3);
    c_residui->cd(3)->SetGrid();
    TGraphErrors *residui_t3 = new TGraphErrors(x_values.size());
    for (int i = 0; i < x_values.size(); i++) {
        double fit_value = linear_t3->Eval(x_values[i]);
        double residual = (means_t3[i] - fit_value);
        residui_t3->SetPoint(i, x_values[i], residual);
        residui_t3->SetPointError(i, 0, sigma_res3[i]);
	   //residui_t3->SetPointError(i, 0, errors_t3[i]);
    }
    TF1 *horiz3 = new TF1("horiz_t3", "pol1", 0.0, 100.0);	// fit a retta dei residui
    horiz3->SetLineColor(kGreen);
    residui_t3->SetTitle("Residui t3;Ritardi [ns];Residui");
    residui_t3->SetMarkerStyle(22);
    residui_t3->SetMarkerColor(kGreen);
    residui_t3->Draw("AP");
    //line_0->Draw("same");
    residui_t3->Fit("horiz_t3");
    horiz3->Draw("same");
    
    TPaveText *eqretta3 = new TPaveText(0.5, 0.1, 0.9, 0.2, "NDC");
    eqretta3->SetBorderSize(1);
    eqretta3->SetFillColor(kWhite);
    eqretta3->SetTextAlign(20);
    eqretta3->SetTextSize(0.03);
    double mr_t3 = horiz3->GetParameter(1);
    double mr_t3_err = horiz3->GetParError(1);
    double qr_t3 = horiz3->GetParameter(0);
    double qr_t3_err = horiz3->GetParError(0);
    eqretta3->AddText(Form("m = %.3f +/- %.3f, q = %.3f +/- %.3f", mr_t3, mr_t3_err, qr_t3, qr_t3_err));
    eqretta3->Draw("same");

    // Residui per t4
    c_residui->cd(4);
    c_residui->cd(4)->SetGrid();
    TGraphErrors *residui_t4 = new TGraphErrors(x_values.size());
    for (int i = 0; i < x_values.size(); i++) {
        double fit_value = linear_t4->Eval(x_values[i]);
        double residual = (means_t4[i] - fit_value);
        residui_t4->SetPoint(i, x_values[i], residual);
        residui_t4->SetPointError(i, 0, sigma_res4[i]);
	   //residui_t4->SetPointError(i, 0, errors_t4[i]);
    }
    TF1 *horiz4 = new TF1("horiz_t4", "pol1", 0.0, 100.0);	// fit a retta dei residui
    horiz4->SetLineColor(kMagenta);
    residui_t4->SetTitle("Residui t4;Ritardi [ns];Residui");
    residui_t4->SetMarkerStyle(23);
    residui_t4->SetMarkerColor(kMagenta);
    residui_t4->Draw("AP");
    //line_0->Draw("same");
    residui_t4->Fit("horiz_t4");
    horiz4->Draw("same");
    
    TPaveText *eqretta4 = new TPaveText(0.5, 0.1, 0.9, 0.2, "NDC");
    eqretta4->SetBorderSize(1);
    eqretta4->SetFillColor(kWhite);
    eqretta4->SetTextAlign(20);
    eqretta4->SetTextSize(0.03);
    double mr_t4 = horiz4->GetParameter(1);
    double mr_t4_err = horiz4->GetParError(1);
    double qr_t4 = horiz4->GetParameter(0);
    double qr_t4_err = horiz4->GetParError(0);
    eqretta4->AddText(Form("m = %.3f +/- %.3f, q = %.3f +/- %.3f", mr_t4, mr_t4_err, qr_t4, qr_t4_err));
    eqretta4->Draw("same");

    // Salva la canvas dei residui come file PNG
    c_residui->Update();
    c_residui->SaveAs("../Dati/TDC/calibrazioneTDC_residui.png");
    
    
    
    
    
    
    
    // ### Istogrammi dei residui ###
    TCanvas *c_histores = new TCanvas("c_histores", "Istogramma residui TDC", 1200, 800);
    c_histores->Divide(2, 2);
    
    // Istogramma dei residui per t1
    c_histores->cd(1);
    TH1F* h1 = new TH1F("h1", "Istogramma residui t1", 20, -5.0, 5.0);
    for (int i=0; i < x_values.size(); i++) {
	    float resid_della_ricca1 = (means_t1[i]-linear_t1->Eval(x_values[i]))/sigma_res1[i];
	    h1->Fill(resid_della_ricca1);
    }
    h1->SetFillColorAlpha(kRed, 1.0);	// colore di riempimento dell'istogramma
    h1->SetFillStyle(3002);	// pattern di riempimento
    //TCanvas* c2 = new TCanvas("c2", "Istogramma dei residui TDC", 600, 0, 500, 500);
    TF1* gauss1 = new TF1("gauss1", "gaus", -2.0, 2.0);
    gauss1->SetParameters(3.0, 0.0, 1.0);
    h1->Fit(gauss1);
    h1->Draw("E1");	// disegno le barre di errore
    h1->Draw("bar same");
    //gStyle->SetOptStat(0);
    gStyle->SetOptFit(1111);	// visualizzo le statistiche
    
    // Istogramma dei residui per t2
    c_histores->cd(2);
    TH1F* h2 = new TH1F("h2", "Istogramma residui t2", 20, -5.0, 5.0);
    for (int i=0; i < x_values.size(); i++) {
	    float resid_della_ricca2 = (means_t2[i]-linear_t2->Eval(x_values[i]))/sigma_res2[i];
	    h2->Fill(resid_della_ricca2);
    }
    h2->SetFillColorAlpha(kRed, 1.0);
    h2->SetFillStyle(3002);
    TF1* gauss2 = new TF1("gauss2", "gaus", -2.0, 2.0);
    gauss2->SetParameters(3.0, 0.0, 1.0);
    h2->Fit(gauss2);
    h2->Draw("E1");
    h2->Draw("bar same");
    gStyle->SetOptFit(1111);
    
    // Istogramma dei residui per t3
    c_histores->cd(3);
    TH1F* h3 = new TH1F("h3", "Istogramma residui t3", 20, -5.0, 5.0);
    for (int i=0; i < x_values.size(); i++) {
	    float resid_della_ricca3 = (means_t3[i]-linear_t3->Eval(x_values[i]))/sigma_res3[i];
	    h3->Fill(resid_della_ricca3);
    }
    h3->SetFillColorAlpha(kRed, 1.0);
    h3->SetFillStyle(3002);
    TF1* gauss3 = new TF1("gauss3", "gaus", -2.0, 2.0);
    gauss3->SetParameters(3.0, 0.0, 1.0);
    h3->Fit(gauss3);
    h3->Draw("E1");
    h3->Draw("bar same");
    gStyle->SetOptFit(1111);
    
    // Istogramma dei residui per t4
    c_histores->cd(4);
    TH1F* h4 = new TH1F("h4", "Istogramma residui t4", 20, -5.0, 5.0);
    for (int i=0; i < x_values.size(); i++) {
	    float resid_della_ricca4 = (means_t4[i]-linear_t4->Eval(x_values[i]))/sigma_res4[i];
	    h4->Fill(resid_della_ricca4);
    }
    h4->SetFillColorAlpha(kRed, 1.0);
    h4->SetFillStyle(3002);
    TF1* gauss4 = new TF1("gauss4", "gaus", -2.0, 2.0);
    gauss4->SetParameters(3.0, 0.0, 1.0);
    h4->Fit(gauss4);
    h4->Draw("E1");
    h4->Draw("bar same");
    gStyle->SetOptFit(1111);
    
    // Salva la canvas come file PNG
    c_histores->Update();
    c_histores->SaveAs("../Dati/TDC/calibrazioneTDC_historesidui.png");
}
