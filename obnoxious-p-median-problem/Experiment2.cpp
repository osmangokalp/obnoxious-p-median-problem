// obnoxious-p-median-problem.cpp : Bu dosya 'main' iþlevi içeriyor. Program yürütme orada baþlayýp biter.
//

#include "pch.h"
#include <iostream>
#include <string>
#include "Problem.h"
#include "ProblemInstanceReader.h"
#include "Solution.h"
#include "IteratedGreedy.h"
#include <chrono>
#include <iostream>
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
	string EXPERIMENTS_FOLDER_LOC = "C:\\Users\\osman\\OneDrive\\Belgeler\\Okul\\Academic Works\\Continuing\\obnoxious p-median\\Experiments";

	//Experiment Parameters
	int SEED = 1919;
	const int NUM_TRY = 1;
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

	//File output
	string OUTPUT_FILE_NAME = "Exp2_" + to_string(MAX_ITER) + string("iter.txt");

	Problem *p;
	Solution * S;

	for (size_t i = 0; i < instanceSize; i++)
	{
		if (printInfo) {
			std::cout << instanceSet[i] << "\n";
		}

		ProblemInstanceReader * pir = new ProblemInstanceReader(instanceSet[i]);
		p = pir->GetProblem();

		double alpha = 0.35;
		int d = 45;

		for (size_t j = 0; j < NUM_TRY; j++)
		{
			if (printInfo) {
				std::cout << "\talpha:  " << to_string(alpha) << "\t, p:  " << to_string(d) << "\t, Try " << to_string(j) << "\n";
			}

			srand(SEED++);
			IteratedGreedy * ig = new IteratedGreedy(p, d, alpha);
			ig->setPrintInfo(printInfo);

			auto start = std::chrono::high_resolution_clock::now();
			S = ig->solve(MAX_ITER);
			auto finish = std::chrono::high_resolution_clock::now();

			std::chrono::duration<double> elapsed = finish - start;
			double f = S->getObjValue();
			double t = elapsed.count();

			string line = string(instanceSet[i]) + string(" ") + to_string(j) + string(" ") + to_string(alpha) + string(" ") + to_string(d) + string(" ") + to_string(f) + string(" ") + to_string(t);

			if (printInfo) {
				std::cout << "\t\tObj: " << to_string(S->getObjValue()) << "\n";
			}

			saveToAFile(EXPERIMENTS_FOLDER_LOC, OUTPUT_FILE_NAME, line);

			S = NULL;
			delete ig;
		}

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
