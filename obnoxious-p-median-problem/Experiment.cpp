// obnoxious-p-median-problem.cpp : Bu dosya 'main' iþlevi içeriyor. Program yürütme orada baþlayýp biter.
//

#include "pch.h"
#include <iostream>
#include "Problem.h"
#include "Util.h"
#include "ProblemInstanceReader.h"
#include "Solution.h"
#include "GreedySelection.h"
#include "IteratedGreedy.h"
#include "LocalSearch.h"
#include <chrono>
#include <fstream>
#include <iomanip> // setprecision
#include <sstream> // stringstream
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

	string instancesToBeSolved = "setB.txt";

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
	double alphaInitial = 0.5;
	double alpha = 0.5;
	double dPercent = 0.5;
	int GREEDY_SELECTION_MODE = 1; //0: rpg, 1:GRASP
	int ALPHA_MODE = 1; //0: static, 1: u[0.5,1] random
	int LS_MODE = 3; //0: no LS, 1: RLS1, 2: RLS2, 3: composite LS
	int D_MODE = 1; //0: static, 1: n(0.5, 0.2) random
	int iterFactor = 5;

	//for(dPercent = 0.1; dPercent <= 0.9; dPercent += 0.1)
	//for (alpha = 0.0; alpha <= 1.0; alpha += 0.1) {

		stringstream stream, stream2, stream3;
		stream << fixed << setprecision(2) << alpha;
		string alphaStr = stream.str();
		stream2 << fixed << setprecision(2) << dPercent;
		string dPercentStr = stream2.str();
		stream3 << fixed << setprecision(2) << alphaInitial;
		string alphaInitialStr = stream3.str();

		//File output
		string OUTPUT_FILE_NAME = instancesToBeSolved + "_px" + to_string(iterFactor) + "iter_GM" + to_string(GREEDY_SELECTION_MODE)
			+ "_AM" + to_string(ALPHA_MODE) + "_LM" + to_string(LS_MODE) + "_DM" + to_string(D_MODE) 
			+ "_alphaInitial" + alphaInitialStr + "_alpha" + alphaStr + "_dPercent" + dPercentStr + string(".txt");

		for (size_t i = 0; i < instanceSize; i++)
		{
			if (printInfo) {
				std::cout << instanceSet[i] << "\n";
			}

			ProblemInstanceReader * pir = new ProblemInstanceReader(instanceSet[i]);
			p = pir->GetProblem();

			MAX_ITER = p->getP() * iterFactor;

			SEED = 2019;

			for (size_t j = 0; j < NUM_TRY; j++)
			{
				if (printInfo) {
					std::cout << "\tTry " << to_string(j) << "\n";
				}

				srand(SEED++);
				IteratedGreedy * ig = new IteratedGreedy(p);
				ig->setPrintInfo(printInfo);

				auto start = std::chrono::high_resolution_clock::now();

				S = GreedySelection::constructSolution(p, alphaInitial, GREEDY_SELECTION_MODE);
				LocalSearch::search(S, LS_MODE);
				Solution * S_Optimized = ig->solve(S, MAX_ITER, alpha, dPercent, GREEDY_SELECTION_MODE, ALPHA_MODE, LS_MODE, D_MODE);

				auto finish = std::chrono::high_resolution_clock::now();

				std::chrono::duration<double> elapsed = finish - start;
				double f = S_Optimized->getObjValue();
				double t = elapsed.count();

				if (f > BKS[i]) { //new best solution found 
					string data = instanceSet[i] + "\n";
					data += S_Optimized->toString();
					data += "\n";
					data += "alpha: " + to_string(alpha) + "\n";
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
	//}
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
