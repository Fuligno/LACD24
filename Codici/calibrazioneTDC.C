#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <iomanip> 

using namespace std;   

void calibrazioneTDC() {
    // Open the file that contains the list of .txt files
    ifstream fileList("filenameTDC.txt");
    if (!fileList.is_open()) {
        cerr << "Unable to open file list!" << endl;  // Error message if file list can't be opened
        return;
    }

    string line;                           // Variable to store each line from the file
    vector<string> fileNames;              // Vector to store the list of file names

    // Read the file names from the list
    while (getline(fileList, line)) {
        fileNames.push_back(line);         // Add each file name to the vector
    }
    fileList.close();                      // Close the file list

    const int nColumns = 4;                // We only use the first 4 columns
    ofstream outFile("out_calibrazioneTDC.txt");  // Create an output file
    if (!outFile.is_open()) {
        cerr << "Unable to open output file!" << endl;  // Error message if output file can't be opened
        return;
    }

    // Write the header for the output file
    outFile << setprecision(4) << fixed;   // Set precision to 4 significant digits
    outFile << "File\tMean 1\tError 1\tMean 2\tError 2\tMean 3\tError 3\tMean 4\tError 4\n";  // Write the header

    // Loop through each file and process it
    for (const string& fileName : fileNames) {
        ifstream dataFile("../Dati/TDC/"+fileName);       // Open the data file
        if (!dataFile.is_open()) {
            cerr << "Unable to open data file: " << fileName << endl;  // Error message if data file can't be opened
            continue;
        }

        string dataLine;                   // Variable to store each line of data
        vector<double> sum(nColumns, 0.0); // Vector to store the sum of each column
        vector<double> sumSquares(nColumns, 0.0); // Vector to store the sum of squares of each column
        int rowCount = 0;                  // Counter for the number of rows

        // Read data from the file
        while (getline(dataFile, dataLine)) {
            istringstream ss(dataLine);    // Create a string stream from the line
            double value;                  // Variable to store each value
            for (int i = 0; i < nColumns; ++i) {
                ss >> value;               // Extract the value from the string stream
                sum[i] += value;           // Accumulate the sum
                sumSquares[i] += value * value; // Accumulate the sum of squares
            }
            ++rowCount;                    // Increment the row counter
        }
        dataFile.close();                  // Close the data file

        // Calculate means and errors for this file
        vector<double> means(nColumns), errors(nColumns);  // Vectors to store means and errors

        for (int i = 0; i < nColumns; ++i) {
            double mean = sum[i] / rowCount;                   // Calculate mean
            double variance = (sumSquares[i] / rowCount) - (mean * mean); // Calculate variance
            double error = sqrt(variance / rowCount);          // Calculate error
            means[i] = mean;                                   // Store mean
            errors[i] = error;                                 // Store error
        }

        // Write the means and errors for this file to the output file
        outFile << fileName << "\t";          // Write the file name
        for (int i = 0; i < nColumns; ++i) {
            outFile << means[i] << "\t" << errors[i] << "\t"; // Write mean and error for each column
        }
        outFile << "\n";                      // Move to the next line for the next file
    }

    outFile.close();                          // Close the output file
}


