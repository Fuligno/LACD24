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

void PlotTDC() {
    // Read the file
    ifstream file("out_calibrazioneTDC.txt");
    if (!file) {
        cout << "Error: Could not open file" << endl;
        return;
    }

    vector<double> x_values;
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
        double x_value = stod(x_str)/10. + 21.2;
        x_values.push_back(x_value);
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
    TCanvas* c1 = new TCanvas("c1", "TDC Calibration", 800, 600);

    // Create the graphs
    TGraphErrors* gr1 = new TGraphErrors(x_values.size(), &x_values[0], &y_values1[0], nullptr, &y_errors1[0]);
    TGraphErrors* gr2 = new TGraphErrors(x_values.size(), &x_values[0], &y_values2[0], nullptr, &y_errors2[0]);
    TGraphErrors* gr3 = new TGraphErrors(x_values.size(), &x_values[0], &y_values3[0], nullptr, &y_errors3[0]);
    TGraphErrors* gr4 = new TGraphErrors(x_values.size(), &x_values[0], &y_values4[0], nullptr, &y_errors4[0]);

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
    TF1* fit1 = new TF1("fit1", "pol1", x_values.front(), x_values.back());
    TF1* fit2 = new TF1("fit2", "pol1", x_values.front(), x_values.back());
    TF1* fit3 = new TF1("fit3", "pol1", x_values.front(), x_values.back());
    TF1* fit4 = new TF1("fit4", "pol1", x_values.front(), x_values.back());
    
    fit1->SetLineColorAlpha(kRed, 0.5);
    fit2->SetLineColorAlpha(kBlue, 0.5);
    fit3->SetLineColorAlpha(kGreen, 0.5);
    fit4->SetLineColorAlpha(kMagenta, 0.5);

    gr1->Fit(fit1, "Q");
    gr2->Fit(fit2, "Q");
    gr3->Fit(fit3, "Q");
    gr4->Fit(fit4, "Q");
    
    // Draw the graphs and fits
    gr1->GetXaxis()->SetTitle("Ritardi [ns]");
    gr1->GetYaxis()->SetTitle("Conteggi");
    gr1->GetYaxis()->SetTitleOffset(1.35);
    gr1->SetTitle("Calibrazione TDC");
    gr1->SetTitle("Calibrazione TDC");
    gr1->Draw("AP");
    gr2->Draw("P");
    gr3->Draw("P");
    gr4->Draw("P");

    // Draw equation of fits
    TPaveText* fitText1 = new TPaveText(0.64, 0.35, 0.89, 0.30, "NDC");
    fitText1->AddText(Form("#color[2]{Ch0: %.2fx + %.2f}", fit1->GetParameter(1), fit1->GetParameter(0)));
    fitText1->SetFillColor(0);
    fitText1->SetTextSize(0.03);
    fitText1->Draw();

    TPaveText* fitText2 = new TPaveText(0.64, 0.30, 0.89, 0.25, "NDC");
    fitText2->AddText(Form("#color[4]{Ch1: %.2fx + %.2f}", fit2->GetParameter(1), fit2->GetParameter(0)));
    fitText2->SetFillColor(0);
    fitText2->SetTextSize(0.03);
    fitText2->Draw();

    TPaveText* fitText3 = new TPaveText(0.64, 0.25, 0.89, 0.20, "NDC");
    fitText3->AddText(Form("#color[3]{Ch2: %.2fx + %.2f}", fit3->GetParameter(1), fit3->GetParameter(0)));
    fitText3->SetFillColor(0);
    fitText3->SetTextSize(0.03);
    fitText3->Draw();

    TPaveText* fitText4 = new TPaveText(0.64, 0.20, 0.89, 0.15, "NDC");
    fitText4->AddText(Form("#color[6]{Ch3: %.2fx + %.2f}", fit4->GetParameter(1), fit4->GetParameter(0)));
    fitText4->SetFillColor(0);
    fitText4->SetTextSize(0.03);
    fitText4->Draw();

    // Remove stats
    gr1->SetStats(0);

    // Save the plot
    c1->SetGrid();
    c1->SaveAs("fit_calibrazioneTDC.png");
}

