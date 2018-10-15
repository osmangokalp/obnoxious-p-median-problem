// obnoxious-p-median-problem.cpp : Bu dosya 'main' iþlevi içeriyor. Program yürütme orada baþlayýp biter.
//

#include "pch.h"
#include <iostream>
#include "Problem.h"
#include "Util.h"
#include "ProblemInstanceReader.h"
#include "Solution.h"
#include "GC2.h"
#include "IteratedGreedy.h"
#include "IGxSA.h"
#include "LS1.h"
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

	string instancesToBeSolved = "limited_instances.txt";

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

	//ALgorithm parameters
	double alpha = 0.3;
	double dFactor = 0.5;

	//File output
	string OUTPUT_FILE_NAME = "Exp2_Deneme_" + to_string(MAX_ITER) + string("iter.txt");

	for (size_t i = 0; i < instanceSize; i++)
	{
		if (printInfo) {
			std::cout << instanceSet[i] << "\n";
		}

		ProblemInstanceReader * pir = new ProblemInstanceReader(instanceSet[i]);
		p = pir->GetProblem();

	/*	alpha = 0.0;
		for (size_t a = 0; a < 9; a++)
		{
			alpha += 0.1;
			dFactor = 0.0;
			for (size_t b = 0; b < 9; b++)
			{
				dFactor += 0.1;*/
				int d = floor(dFactor * p->getP());
				SEED = 2018;

				for (size_t j = 0; j < NUM_TRY; j++)
				{
					if (printInfo) {
						std::cout << "\talpha:  " << to_string(alpha) << "\t, p:  " << to_string(d) << "\t, Try " << to_string(j) << "\n";
					}

					srand(SEED++);
					IteratedGreedy * ig = new IteratedGreedy(p, d, alpha);
					ig->setPrintInfo(printInfo);

					auto start = std::chrono::high_resolution_clock::now();

					S = GC2::constructSolution(p, alpha);
					//LS1::search(S);
					Solution * S_Optimized = ig->solve(S, MAX_ITER);

					auto finish = std::chrono::high_resolution_clock::now();

					std::chrono::duration<double> elapsed = finish - start;
					double f = S_Optimized->getObjValue();
					double t = elapsed.count();

					if (f > BKS[i]) { //new best solution found 
						string data = instanceSet[i] + "\n";
						data += S_Optimized->toString();
						data += "\n";
						data += "alpha: " + to_string(alpha) + "\n";
						data += "dFactor: " + to_string(dFactor) + "\n";
						data += "SEED: " + to_string(SEED) + "\n";
						data += Util::getCurrentDateAndTime() + "\n";
						if (Util::validateSolution(S_Optimized, f, p)) {
							data += "VALID SOLUTION\n";
						}
						else {
							data += "INVALID SOLUTION !!!\n";
						}
						saveToAFile(EXPERIMENTS_FOLDER_LOC, "NewBestSolutions.txt", data);
					}

					string line = string(instanceSet[i]) + string(" ") + to_string(BKS[i]) + string(" ") + to_string(j) + string(" ") + to_string(alpha) + string(" ") + to_string(dFactor) + string(" ") + to_string(f) + string(" ") + to_string(t);

					if (printInfo) {
						std::cout << "\t\tObj: " << to_string(S_Optimized->getObjValue()) << "\n";
					}

					saveToAFile(EXPERIMENTS_FOLDER_LOC, OUTPUT_FILE_NAME, line);

					delete S;
					delete ig;
				}
		//	}
		//}

		delete pir;
	}
	delete[] instanceSet;
	delete[] BKS;
}

// Programý çalýþtýr: Ctrl + F5 veya Hata Ayýkla > Hata Ayýklamadan Baþlat menüsü
// Programda hata ayýkla: F5 veya Hata Ayýkla > Hata Ayýklamayý Baþlat menüsü

// Kullanmaya Baþlama Ýpuçlarý: 
//   1. Dosyalarý eklemek/yönetmek için Çözüm Gezgini penceresini kullanýn
//   2. Kaynak denetimine baðlanmak için Takým Gezgini penceresini kullanýn
//   3. Derleme çýktýsýný ve diðer iletileri görmek için Çýktý penceresini kullanýn
//   4. Hatalarý görüntülemek için Hata Listesi penceresini kullanýn
//   5. Yeni kod dosyalarý oluþturmak için Projeye Git > Yeni Öðe ekle veya varolan kod dosyalarýný projeye eklemek için Proje > Var Olan Öðeyi Ekle adýmlarýný izleyin
//   6. Bu projeyi daha sonra yeniden açmak için Dosya > Aç > Proje'ye gidip .sln uzantýlý dosyayý seçin
