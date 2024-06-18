#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <iomanip>

using namespace std;    

void calibrazioneQDC() {
    // Open the file that contains the list of .txt files
    ifstream fileList("filenameQDC.txt");
    if (!fileList.is_open()) {
        cerr << "Unable to open file list!" << endl;  // Error message if file list can't be opened
        return;
    }

    string line;
    vector<string> fileNames;

    // Read the file names from the list
    while (getline(fileList, line)) {
        fileNames.push_back(line);  // Add each file name to the vector
    }
    fileList.close();  // Close the file list

    const int nColumns = 8;  // We only use the first 8 columns
    // Initialize vectors to hold sums and sum of squares for high resolution data
    vector<vector<double>> sum_high(fileNames.size(), vector<double>(nColumns, 0.0));
    vector<vector<double>> sumSquares_high(fileNames.size(), vector<double>(nColumns, 0.0));
    vector<int> rowCount_high(fileNames.size(), 0);  // Row counts for high resolution data

    // Initialize vectors to hold sums and sum of squares for low resolution data
    vector<vector<double>> sum_low(fileNames.size(), vector<double>(nColumns, 0.0));
    vector<vector<double>> sumSquares_low(fileNames.size(), vector<double>(nColumns, 0.0));
    vector<int> rowCount_low(fileNames.size(), 0);  // Row counts for low resolution data

    // Loop through each file and process it
    for (size_t fileIndex = 0; fileIndex < fileNames.size(); ++fileIndex) {
        ifstream dataFile("../Dati/QDC/"+fileNames[fileIndex]);
        if (!dataFile.is_open()) {
            cerr << "Unable to open data file: " << fileNames[fileIndex] << endl;  // Error if data file can't be opened
            continue;
        }

        string dataLine;
        // Read each line of the data file
        while (getline(dataFile, dataLine)) {
            istringstream ss(dataLine);
            double value;
            int column = 0;
            // Read values from the line and accumulate sums and sums of squares
            while (ss >> value && column < nColumns) {
                if (column % 2 == 0) { // Low resolution values (columns 0, 2, 4, 6)
                    sum_low[fileIndex][column] += value;
                    sumSquares_low[fileIndex][column] += value * value;
                } else { // High resolution values (columns 1, 3, 5, 7)
                    sum_high[fileIndex][column] += value;
                    sumSquares_high[fileIndex][column] += value * value;
                }
                column++;
            }
            rowCount_low[fileIndex]++;  // Increment row count for low resolution
            rowCount_high[fileIndex]++; // Increment row count for high resolution
        }
        dataFile.close();  // Close the data file
    }

    // Write data for high resolution to a separate file
    ofstream outFile_high("out_calibrazioneQDC_high.txt");
    if (!outFile_high.is_open()) {
        cerr << "Unable to open high resolution output file!" << endl;  // Error if output file can't be opened
        return;
    }

    // Set precision to 4 decimal places
    outFile_high << fixed << setprecision(4);

    // Write column headers for high resolution file
    outFile_high << "File\tMean_HR_Chan0\tError_HR_Chan0\tMean_HR_Chan1\tError_HR_Chan1\tMean_HR_Chan2\tError_HR_Chan2\tMean_HR_Chan3\tError_HR_Chan3\n";

    // Write data for each file (high resolution)
    for (size_t fileIndex = 0; fileIndex < fileNames.size(); ++fileIndex) {
        outFile_high << fileNames[fileIndex] << "\t";  // Write file name

        for (int i = 0; i < 4; ++i) {
            // High resolution values (columns 1, 3, 5, 7)
            int col = i * 2 + 1;
            double meanHigh = sum_high[fileIndex][col] / rowCount_high[fileIndex];  // Calculate mean
            double varianceHigh = (sumSquares_high[fileIndex][col] / rowCount_high[fileIndex]) - (meanHigh * meanHigh);  // Calculate variance
            double errorHigh = sqrt(varianceHigh / rowCount_high[fileIndex]);  // Calculate error

            // Write mean and error for each channel (high resolution)
            outFile_high << meanHigh << "\t" << errorHigh << "\t";
        }

        outFile_high << "\n"; // Move to the next line for the next file
    }

    outFile_high.close();  // Close the high resolution output file

    // Write data for low resolution to a separate file
    ofstream outFile_low("out_calibrazioneQDC_low.txt");
    if (!outFile_low.is_open()) {
        cerr << "Unable to open low resolution output file!" << endl;  // Error if output file can't be opened
        return;
    }

    // Set precision to 4 decimal places
    outFile_low << fixed << setprecision(4);

    // Write column headers for low resolution file
    outFile_low << "File\tMean_LR_Chan0\tError_LR_Chan0\tMean_LR_Chan1\tError_LR_Chan1\tMean_LR_Chan2\tError_LR_Chan2\tMean_LR_Chan3\tError_LR_Chan3\n";

    // Write data for each file (low resolution)
    for (size_t fileIndex = 0; fileIndex < fileNames.size(); ++fileIndex) {
        outFile_low << fileNames[fileIndex] << "\t";  // Write file name

        for (int i = 0; i < 4; ++i) {
            // Low resolution values (columns 0, 2, 4, 6)
            int col = i * 2;
            double meanLow = sum_low[fileIndex][col] / rowCount_low[fileIndex];  // Calculate mean
            double varianceLow = (sumSquares_low[fileIndex][col] / rowCount_low[fileIndex]) - (meanLow * meanLow);  // Calculate variance
            double errorLow = sqrt(varianceLow / rowCount_low[fileIndex]);  // Calculate error

            // Write mean and error for each channel (low resolution)
            outFile_low << meanLow << "\t" << errorLow << "\t";
        }

        outFile_low << "\n"; // Move to the next line for the next file
    }

    outFile_low.close();  // Close the low resolution output file
}

