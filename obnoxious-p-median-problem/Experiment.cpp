// obnoxious-p-median-problem.cpp : Bu dosya 'main' i�levi i�eriyor. Program y�r�tme orada ba�lay�p biter.
//

#include "pch.h"
#include <iostream>
#include "Problem.h"
#include "Util.h"
#include "ProblemInstanceReader.h"
#include "Solution.h"
#include "GC2.h"
#include "IteratedGreedy.h"
#include "LocalSearch.h"
#include <chrono>
#include <fstream>
using namespace std;

static void saveToAFile(string EXPERIMENTS_OUTPUT_FOLDER_LOC, string outputFileName, string line) {
	ofstream outFile;
	outFile.open(EXPERIMENTS_OUTPUT_FOLDER_LOC + string("\\") + outputFileName, std::ios_base::app);
	if (outFile.fail()) {
		cerr << "unable to open file << outputFileName << for writing" << endl;
		exit(1);
	}
	outFile << line << endl;
	outFile.close();
}

int main()
{
	string EXPERIMENTS_FOLDER_LOC = "D:\\Experiments\\obnoxious p-median";

	//Experiment Parameters
	int SEED;
	const int NUM_TRY = 10;
	int MAX_ITER = 1000;
	bool printInfo = true;

	string instancesToBeSolved = "representative_instances.txt";

	ifstream inFile;
	string line, delimiter;
	inFile.open(EXPERIMENTS_FOLDER_LOC + string("\\") + instancesToBeSolved);

	if (inFile.fail()) {
		cerr << "unable to open file << instancesToBeSolved << for reading" << endl;
		exit(1);
	}

	//read first line ( # of instances)
	getline(inFile, line);
	int instanceSize = stoi(line);

	string * instanceSet = new string[instanceSize];
	double * BKS = new double[instanceSize];

	delimiter = ",";
	int counter = 0;
	while (getline(inFile, line)) {
		instanceSet[counter] = line.substr(0, line.find(delimiter));
		BKS[counter] = stod(line.substr(line.find(delimiter) + 1, line.length()));
		counter++;
	}

	inFile.close();

	Problem *p;
	Solution * S;

	//Algorithm parameters
	double alphaMax = 0.9;
	double alphaMin = 0.1;
	double alphaDecreaseFactor = 0.9;

	//File output
	string OUTPUT_FILE_NAME = "Exp_Deneme12_px10" + string("iter.txt");

	for (size_t i = 0; i < instanceSize; i++)
	{
		if (printInfo) {
			std::cout << instanceSet[i] << "\n";
		}

		ProblemInstanceReader * pir = new ProblemInstanceReader(instanceSet[i]);
		p = pir->GetProblem();

		MAX_ITER = p->getP() * 10;

		SEED = 2018;

		for (size_t j = 0; j < NUM_TRY; j++)
		{
			if (printInfo) {
				std::cout << "\tTry " << to_string(j) << "\n";
			}

			srand(SEED++);
			IteratedGreedy * ig = new IteratedGreedy(p);
			ig->setPrintInfo(printInfo);

			auto start = std::chrono::high_resolution_clock::now();

			S = GC2::constructSolution(p, alphaMax);
			LocalSearch::compositeLS(S);
			Solution * S_Optimized = ig->solve(S, MAX_ITER, alphaMax, alphaMin, alphaDecreaseFactor);

			auto finish = std::chrono::high_resolution_clock::now();

			std::chrono::duration<double> elapsed = finish - start;
			double f = S_Optimized->getObjValue();
			double t = elapsed.count();

			if (f > BKS[i]) { //new best solution found 
				string data = instanceSet[i] + "\n";
				data += S_Optimized->toString();
				data += "\n";
				data += "alphaMax: " + to_string(alphaMax) + "\n";
				data += "alphaMin: " + to_string(alphaMin) + "\n";
				data += "alphaInc: " + to_string(alphaDecreaseFactor) + "\n";
				data += "SEED: " + to_string(SEED) + "\n";
				data += Util::getCurrentDateAndTime() + "\n";
				if (Util::validateSolution(S_Optimized, f, p)) {
					data += "VALID SOLUTION\n";
				}
				else {
					data += "INVALID SOLUTION !!!\n";
				}
				saveToAFile(EXPERIMENTS_FOLDER_LOC, "NewBest_" + instanceSet[i], data);
			}

			string line = string(instanceSet[i]) + string(" ") + to_string(j) + string(" ") + to_string(f) + string(" ") + to_string(t);

			if (printInfo) {
				std::cout << "\t\tObj: " << to_string(S_Optimized->getObjValue()) << "\n";
			}

			saveToAFile(EXPERIMENTS_FOLDER_LOC, OUTPUT_FILE_NAME, line);

			delete S;
			delete ig;
		}

		delete pir;
	}
	delete[] instanceSet;
	delete[] BKS;
}

// Program� �al��t�r: Ctrl + F5 veya Hata Ay�kla > Hata Ay�klamadan Ba�lat men�s�
// Programda hata ay�kla: F5 veya Hata Ay�kla > Hata Ay�klamay� Ba�lat men�s�

// Kullanmaya Ba�lama �pu�lar�: 
//   1. Dosyalar� eklemek/y�netmek i�in ��z�m Gezgini penceresini kullan�n
//   2. Kaynak denetimine ba�lanmak i�in Tak�m Gezgini penceresini kullan�n
//   3. Derleme ��kt�s�n� ve di�er iletileri g�rmek i�in ��kt� penceresini kullan�n
//   4. Hatalar� g�r�nt�lemek i�in Hata Listesi penceresini kullan�n
//   5. Yeni kod dosyalar� olu�turmak i�in Projeye Git > Yeni ��e ekle veya varolan kod dosyalar�n� projeye eklemek i�in Proje > Var Olan ��eyi Ekle ad�mlar�n� izleyin
//   6. Bu projeyi daha sonra yeniden a�mak i�in Dosya > A� > Proje'ye gidip .sln uzant�l� dosyay� se�in
