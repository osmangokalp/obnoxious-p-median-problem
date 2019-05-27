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
	string EXPERIMENTS_FOLDER_LOC = "D:\\Experiments\\obnoxious-p-median";

	//Experiment Parameters
	int SEED;
	const int NUM_TRY = 50;
	int MAX_ITER = 0;
	double MAX_TIME = 0;
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
	double alphaInitial = 0.0;
	double alpha = 0.79;
	double pFactor = 0.61;
	int GREEDY_SELECTION_MODE = 1; //0: rpg, 1:GRASP
	int ALPHA_MODE = 0; //0: static, 1: u[alpha, 1.0] random
	int LS_MODE = 3; //0: no LS, 1: RLS1, 2: RLS2, 3: composite LS
	int D_MODE = 0; //0: static, 1: u(pFactor - 0.1, pFactor + 0.1) random
	int iterFactor = 10;
	double timeFactor = 999999;

	//for(pFactor = 0.1; pFactor <= 0.9; pFactor += 0.1) {
	//for (alpha = 0.6; alpha <= 1.0; alpha += 0.1) {
	//for (LS_MODE = 0; LS_MODE <= 3; LS_MODE++) {
	stringstream stream, stream2, stream3, stream4;
	stream << fixed << setprecision(2) << alpha;
	string alphaStr = stream.str();
	stream2 << fixed << setprecision(2) << pFactor;
	string pFactorStr = stream2.str();
	stream3 << fixed << setprecision(2) << alphaInitial;
	string alphaInitialStr = stream3.str();
	stream4 << fixed << setprecision(2) << timeFactor;
	string timeFactorStr = stream4.str();

	//File output
	//string OUTPUT_FILE_NAME = instancesToBeSolved + "_alpha_pFactor_parameter_analysis_" + "_iterF_" + to_string(iterFactor) +"_timeF_" + timeFactorStr + string(".txt");
	string OUTPUT_FILE_NAME = instancesToBeSolved + "_new_alpha_" + alphaStr + "_pFactor_" + pFactorStr + "_iterF_" + to_string(iterFactor) + "_timeF_" + timeFactorStr + string(".txt");

	for (size_t i = 0; i < instanceSize; i++)
	{
		if (printInfo) {
			std::cout << instanceSet[i] << "\n";
		}

		ProblemInstanceReader * pir = new ProblemInstanceReader(instanceSet[i]);
		p = pir->GetProblem();

		MAX_ITER = iterFactor * p->getP();
		MAX_TIME = timeFactor * p->getP();

		SEED = 2019;

		for (size_t tryNo = 0; tryNo < NUM_TRY; tryNo++)
		{
			if (printInfo) {
				std::cout << "\tTry " << to_string(tryNo) << "\n";
			}

			srand(SEED++);
			IteratedGreedy * ig = new IteratedGreedy(p);
			ig->setPrintInfo(printInfo);

			S = GreedySelection::constructSolution(p, alphaInitial, GREEDY_SELECTION_MODE);
			LocalSearch::search(S, LS_MODE);
			Solution * S_Optimized = ig->solve(S, MAX_ITER, MAX_TIME, alpha, pFactor, GREEDY_SELECTION_MODE, ALPHA_MODE, LS_MODE, D_MODE);

			double f = S_Optimized->getObjValue();

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
				saveToAFile(EXPERIMENTS_FOLDER_LOC + "\\NewBest", "NewBest_" + instanceSet[i], data);
			}

			string line = string(instanceSet[i]) + string(" ") + to_string(tryNo) + string(" ") + to_string(f) + string(" ")
				+ to_string(ig->getElapsedTime()) + string(" ") + to_string(ig->getIterCounter() - 1)
				+ string(" ") + to_string(GREEDY_SELECTION_MODE)
				+ string(" ") + to_string(ALPHA_MODE) + string(" ") + to_string(LS_MODE) + string(" ") + to_string(D_MODE)
				+ string(" ") + alphaInitialStr + string(" ") + alphaStr + string(" ") + pFactorStr;

			if (printInfo) {
				std::cout << "\t\tObj: " << to_string(S_Optimized->getObjValue()) << "\n";
			}

			saveToAFile(EXPERIMENTS_FOLDER_LOC, OUTPUT_FILE_NAME, line);

			delete S;
			delete ig;
		}

		delete pir;
		//}
	}
	delete[] instanceSet;
	delete[] BKS;
}

//// Programý çalýþtýr: Ctrl + F5 veya Hata Ayýkla > Hata Ayýklamadan Baþlat menüsü
//// Programda hata ayýkla: F5 veya Hata Ayýkla > Hata Ayýklamayý Baþlat menüsü
//
//// Kullanmaya Baþlama Ýpuçlarý: 
////   1. Dosyalarý eklemek/yönetmek için Çözüm Gezgini penceresini kullanýn
////   2. Kaynak denetimine baðlanmak için Takým Gezgini penceresini kullanýn
////   3. Derleme çýktýsýný ve diðer iletileri görmek için Çýktý penceresini kullanýn
////   4. Hatalarý görüntülemek için Hata Listesi penceresini kullanýn
////   5. Yeni kod dosyalarý oluþturmak için Projeye Git > Yeni Öðe ekle veya varolan kod dosyalarýný projeye eklemek için Proje > Var Olan Öðeyi Ekle adýmlarýný izleyin
////   6. Bu projeyi daha sonra yeniden açmak için Dosya > Aç > Proje'ye gidip .sln uzantýlý dosyayý seçin


// obnoxious-p-median-problem.cpp : Bu dosya 'main' iþlevi içeriyor. Program yürütme orada baþlayýp biter.
//

//#include "pch.h"
//#include <iostream>
//#include "Problem.h"
//#include "Util.h"
//#include "ProblemInstanceReader.h"
//#include "Solution.h"
//#include "GreedySelection.h"
//#include "IteratedGreedy.h"
//#include "LocalSearch.h"
//#include <fstream>
//#include <iomanip> // setprecision
//#include <sstream> // stringstream
//using namespace std;
//
//int main(int argc, char *argv[])
//{
//	//string info = "";
//	//for (int i = 1; i < argc; i++)
//	//{
//	//	std::string s(argv[i]);
//	//	info += (" " + s);
//	//}
//	//std::cout << info;
//
//	//Experiment Parameters
//	int MAX_ITER = 0;
//	double MAX_TIME = 0;
//	bool printInfo = false;
//
//	string instanceToBeSolved = "-1";
//	int SEED = 0;
//	double alpha = -1;
//	double dPercent = -1;
//
//
//	for (int i = 1; i < argc; i++)
//	{
//		std::string s(argv[i]);
//		string subS = s.substr(0, 4);
//		const char  *subSC = subS.c_str();
//		if (strcmp(argv[i], "--seed") == 0)
//		{
//			SEED = std::stoi(argv[i + 1]);
//		}
//		else if (strcmp(subSC, "--in") == 0)
//		{
//			instanceToBeSolved = s.substr(11, s.length());
//		}
//		else if (strcmp(argv[i], "--alpha") == 0)
//		{
//			alpha = std::stod(argv[i + 1]);
//		}
//		else if (strcmp(argv[i], "--dPercent") == 0)
//		{
//			dPercent = std::stod(argv[i + 1]);
//		}
//	}
//	   	  
//	Problem *p;
//	Solution * S;
//
//	//Algorithm parameters
//	double alphaInitial = 0.0;
//	int GREEDY_SELECTION_MODE = 1; //0: rpg, 1:GRASP
//	int ALPHA_MODE = 0; //0: static, 1: u[alpha, 1.0] random
//	int LS_MODE = 3; //0: no LS, 1: RLS1, 2: RLS2, 3: composite LS
//	int D_MODE = 0; //0: static, 1: u(pFactor - 0.1, pFactor + 0.1) random
//	int iterFactor = 5;
//	double timeFactor = 999999;
//
//	
//	ProblemInstanceReader * pir = new ProblemInstanceReader(instanceToBeSolved);
//	/*std::cout << "Ýrace deneme!";*/
//	p = pir->GetProblem();
//
//	MAX_ITER = iterFactor * p->getP();
//	MAX_TIME = timeFactor * p->getP();
//
//	srand(SEED);
//	IteratedGreedy * ig = new IteratedGreedy(p);
//	ig->setPrintInfo(printInfo);
//
//	S = GreedySelection::constructSolution(p, alphaInitial, GREEDY_SELECTION_MODE);
//	LocalSearch::search(S, LS_MODE);
//	Solution * S_Optimized = ig->solve(S, MAX_ITER, MAX_TIME, alpha, dPercent, GREEDY_SELECTION_MODE, ALPHA_MODE, LS_MODE, D_MODE);
//
//	double f = -1 * S_Optimized->getObjValue();
//
//	std::cout << to_string(f);
//
//	delete S;
//	delete ig;
//
//	delete pir;
//
//
//}

// Programý çalýþtýr: Ctrl + F5 veya Hata Ayýkla > Hata Ayýklamadan Baþlat menüsü
// Programda hata ayýkla: F5 veya Hata Ayýkla > Hata Ayýklamayý Baþlat menüsü

// Kullanmaya Baþlama Ýpuçlarý: 
//   1. Dosyalarý eklemek/yönetmek için Çözüm Gezgini penceresini kullanýn
//   2. Kaynak denetimine baðlanmak için Takým Gezgini penceresini kullanýn
//   3. Derleme çýktýsýný ve diðer iletileri görmek için Çýktý penceresini kullanýn
//   4. Hatalarý görüntülemek için Hata Listesi penceresini kullanýn
//   5. Yeni kod dosyalarý oluþturmak için Projeye Git > Yeni Öðe ekle veya varolan kod dosyalarýný projeye eklemek için Proje > Var Olan Öðeyi Ekle adýmlarýný izleyin
//   6. Bu projeyi daha sonra yeniden açmak için Dosya > Aç > Proje'ye gidip .sln uzantýlý dosyayý seçin
