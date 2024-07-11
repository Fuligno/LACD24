#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <TCanvas.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TLegend.h>
#include <TPaveText.h>

using namespace std;

void Plot_calibrazioneQDC_low() {
    // Read the file
    ifstream file("../Dati/QDC/out_calibrazioneQDC_low.txt");
    if (!file) {
        cout << "Error: Could not open file" << endl;
        return;
    }

    vector<double> x_values1, x_values2, x_values3, x_values4;
    vector<double> y_values1, y_values2, y_values3, y_values4;
    vector<double> y_errors1, y_errors2, y_errors3, y_errors4;
    string line;
    getline(file, line); // Skip the header line

    while (getline(file, line)) {
        istringstream iss(line);
        string filename;
        double mean1, error1, mean2, error2, mean3, error3, mean4, error4;
        iss >> filename >> mean1 >> error1 >> mean2 >> error2 >> mean3 >> error3 >> mean4 >> error4;

        string x_str = filename.substr(3, 3);
        double x_value1 = 21.593 * pow(10, -stod(x_str) / 200.);
        double x_value2 = 21.223 * pow(10, -stod(x_str) / 200.);
        double x_value3 = 20.894 * pow(10, -stod(x_str) / 200.);
        double x_value4 = 20.924 * pow(10, -stod(x_str) / 200.);
        
        x_values1.push_back(x_value1);
        x_values2.push_back(x_value2);
        x_values3.push_back(x_value3);
        x_values4.push_back(x_value4);
        y_values1.push_back(mean1);
        y_errors1.push_back(error1);
        y_values2.push_back(mean2);
        y_errors2.push_back(error2);
        y_values3.push_back(mean3);
        y_errors3.push_back(error3);
        y_values4.push_back(mean4);
        y_errors4.push_back(error4);
    }

    file.close();

    // Create the canvas
    TCanvas* c1 = new TCanvas("c1", "QDC Calibration", 800, 600);

    // Create the graphs
    TGraphErrors* gr1 = new TGraphErrors(x_values1.size(), &x_values1[0], &y_values1[0], nullptr, &y_errors1[0]);
    TGraphErrors* gr2 = new TGraphErrors(x_values2.size(), &x_values2[0], &y_values2[0], nullptr, &y_errors2[0]);
    TGraphErrors* gr3 = new TGraphErrors(x_values3.size(), &x_values3[0], &y_values3[0], nullptr, &y_errors3[0]);
    TGraphErrors* gr4 = new TGraphErrors(x_values4.size(), &x_values4[0], &y_values4[0], nullptr, &y_errors4[0]);

    // Set graph styles
    gr1->SetMarkerStyle(20);
    gr1->SetMarkerColor(kRed);
    gr1->SetMarkerSize(1.5);
    gr1->SetLineColor(kRed);
    gr2->SetMarkerStyle(21);
    gr2->SetMarkerSize(1.5);
    gr2->SetMarkerColor(kBlue);
    gr2->SetLineColor(kBlue);
    gr3->SetMarkerStyle(22);
    gr3->SetMarkerSize(1.5);
    gr3->SetMarkerColor(kGreen);
    gr3->SetLineColor(kGreen);
    gr4->SetMarkerStyle(23);
    gr4->SetMarkerColor(kMagenta);
    gr4->SetMarkerSize(1.5);
    gr4->SetLineColor(kMagenta);

    // Create the linear fits
    TF1* fit1 = new TF1("fit1", "pol1", x_values1.front(), x_values1.back());
    TF1* fit2 = new TF1("fit2", "pol1", x_values2.front(), x_values2.back());
    TF1* fit3 = new TF1("fit3", "pol1", x_values3.front(), x_values3.back());
    TF1* fit4 = new TF1("fit4", "pol1", x_values4.front(), x_values4.back());
    
    fit1->SetLineColor(kRed);
    fit2->SetLineColor(kBlue);
    fit3->SetLineColor(kGreen);
    fit4->SetLineColor(kMagenta);

    gr1->Fit(fit1, "Q");
    gr2->Fit(fit2, "Q");
    gr3->Fit(fit3, "Q");
    gr4->Fit(fit4, "Q");

    // Draw the graphs and fits
    gr1->GetXaxis()->SetTitle("Carica [nC]");
    gr1->GetYaxis()->SetTitle("Conteggi");
    gr1->GetYaxis()->SetTitleOffset(1.35);
    gr1->SetTitle("Calibrazione QDC (low resolution)");
    gr1->Draw("AP");
    gr2->Draw("P");
    gr3->Draw("P");
    gr4->Draw("P");


    // Draw equation of fits
    TPaveText* fitText1 = new TPaveText(0.63, 0.35, 0.89, 0.30, "NDC");
    fitText1->AddText(Form("Ch0: %.2fx + %.2f", fit1->GetParameter(1), fit1->GetParameter(0)));
    fitText1->SetFillColor(0);
    fitText1->SetTextSize(0.03);
    fitText1->SetTextColor(kRed);
    fitText1->Draw();

    TPaveText* fitText2 = new TPaveText(0.63, 0.30, 0.89, 0.25, "NDC");
    fitText2->AddText(Form("Ch1: %.2fx + %.2f", fit2->GetParameter(1), fit2->GetParameter(0)));
    fitText2->SetFillColor(0);
    fitText2->SetTextSize(0.03);
    fitText2->SetTextColor(kGreen);
    fitText2->Draw();

    TPaveText* fitText3 = new TPaveText(0.63, 0.25, 0.89, 0.20, "NDC");
    fitText3->AddText(Form("Ch2: %.2fx + %.2f", fit3->GetParameter(1), fit3->GetParameter(0)));
    fitText3->SetFillColor(0);
    fitText3->SetTextSize(0.03);
    fitText3->SetTextColor(kBlue);
    fitText3->Draw();

    TPaveText* fitText4 = new TPaveText(0.63, 0.20, 0.89, 0.15, "NDC");
    fitText4->AddText(Form("Ch3: %.2fx + %.2f", fit4->GetParameter(1), fit4->GetParameter(0)));
    fitText4->SetFillColor(0);
    fitText4->SetTextSize(0.03);
    fitText4->SetTextColor(kMagenta);
    fitText4->Draw();

    // Remove stats
    gr1->SetStats(0);

    // Save the plot
    c1->SetGrid();
    c1->SaveAs("fit_calibrazioneQDC_low.png");
}

