#include <iostream>
#include <vector>
#include <thread>
#include <fstream>
#include <sstream>
#include <stdlib.h>

using namespace std;

void fileToMatrix(string fileName, vector<vector<int> > &matrixA, vector<vector<int> > &matrixB, vector<int> &products) {
    fstream file;
    file.open(fileName);
    if (file.fail()) {
        cout << "file does not exist" << endl;
    } else {
        int m, n, p;
        file >> m;
        file >> n;
        file >> p;

        matrixA.resize(m);
        for (int i = 0; i < matrixA.size(); i++) {
            matrixA[i].resize(n);
        }
        matrixB.resize(n);
        for (int i = 0; i < matrixB.size(); i++) {
            matrixB[i].resize(p);
        }
        for (int i = 0; i < 3; i++) {
            file.ignore(i, '\n');
        }

        int num = 0;
        string line;
        string myWord;
        for (int i = 0; i < m; i++) {
            getline(file, line);
            istringstream s(line);
            for (int j = 0; j < n; j++) {
                getline(s, myWord, ' ');
                matrixA[i][j] = atoi(myWord.c_str());
            }
        }
        for (int i = 0; i < n; i++) {
            (getline(file, line));
            istringstream s(line);
            for (int j = 0; j < p; j++) {
                getline(s, myWord, ' ');
                matrixB[i][j] = atoi(myWord.c_str());
            }
        }
        for (int i = 0; i < p; i++) {
            (getline(file, line));
            products.push_back(atoi(line.c_str()));
        }
    }
}

void threadedMatrixCalc(vector<vector<int> > matrixA, vector<vector<int> > matrixB, int index, vector<vector<int> > &resultantMatrix) {
    for (int i = 0; i < matrixA[0].size(); i++) {
        for (int j = 0; j < matrixA.size(); j++) {
            resultantMatrix[j][index] = resultantMatrix[j][index] + matrixA[j][i] * matrixB[i][index];
        }
    }
}

void vectorCalc(vector<vector<int> > finalMatrix, vector<int> products, vector<int> &resultVector) {
    resultVector.resize(finalMatrix.size());
    for (int i = 0; i < finalMatrix[0].size(); i++) {
        for (int j = 0; j < finalMatrix.size(); j++) {
            resultVector[j] = resultVector[j] + finalMatrix[j][i] * products[i];
        }
    }
}

void startThreads(vector<vector<int> > matrixA, vector<vector<int> > matrixB, vector<vector<int> > &resultMatrix) {
    vector<thread> threads;
    int x = matrixA.size();
    int y = matrixB[0].size();
    int index;

    resultMatrix.resize(x);
    for (int i = 0; i < x; i++) {
        resultMatrix[i].resize(y);
    }
    for (int i = 0; i < resultMatrix[0].size(); i++) {
        threads.push_back(thread( [&] {threadedMatrixCalc(matrixA, matrixB, i, resultMatrix);}));
        threads[i].join();
    }
}

int main() {
    fstream file;
    string fileName;
    vector<vector<int> > matrixA;
    vector<vector<int> > matrixB;
    vector<vector<int> > resultMatrix;
    vector<int> products;
    vector<int> finalResources;

    cout << "Welcome to the resource requirement calculator." << endl;
    cout << "Please enter a file name containing the data: ";
    cin >> fileName;
    file.open(fileName);
    if (file.fail()) {
        cout << "That file does not exist" << endl;
    } else {
        fileToMatrix(fileName, matrixA, matrixB, products);
        startThreads(matrixA, matrixB, resultMatrix);
        vectorCalc(resultMatrix, products, finalResources);
        cout << "The number of basic resources required is: " << endl;
        for (int i = 0; i < finalResources.size(); i++) {
            cout << finalResources[i] << endl;
        }
        cout << "Thank you!" << endl;
    }
    file.close();
}