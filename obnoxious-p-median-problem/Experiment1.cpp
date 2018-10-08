// obnoxious-p-median-problem.cpp : Bu dosya 'main' iþlevi içeriyor. Program yürütme orada baþlayýp biter.
//

#include "pch.h"
//#include <iostream>
//#include <string>
//#include "Problem.h"
//#include "ProblemInstanceReader.h"
//#include "Solution.h"
//#include "IteratedGreedy.h"
//#include <chrono>
//#include <iostream>
//#include <fstream>
//using namespace std;
//
//static void saveToAFile(string EXPERIMENTS_OUTPUT_FOLDER_LOC, string outputFileName, string line) {
//	ofstream outFile;
//	outFile.open(EXPERIMENTS_OUTPUT_FOLDER_LOC + string("\\") + outputFileName, std::ios_base::app);
//	if (outFile.fail()) {
//		cerr << "unable to open file << outputFileName << for writing" << endl;
//		exit(1);
//	}
//	outFile << line << endl;
//	outFile.close();
//}
//
//int main()
//{
//	//Experiment Parameters
//	int SEED;
//	const int NUM_TRY = 10;
//	int MAX_ITER = 1000;
//	bool printInfo = true;
//
//	//instance sets
//	string instanceSet[] = { "pmed29.txt.table.p150.A.txt", "pmed32.txt.table.p175.A.txt", "pmed40.txt.table.p225.A.txt",
//		"pmed20.txt.table.p100.B.txt", "pmed40.txt.table.p225.B.txt" };
//	double BKS[] = { 4141, 4242, 4571, 4067, 4524 };
//	const int instanceSize = 5;
//
//	//File output
//	string EXPERIMENTS_OUTPUT_FOLDER_LOC = "C:\\Users\\osman\\OneDrive\\Belgeler\\Okul\\Academic Works\\Continuing\\obnoxious p-median\\Experiments";
//	string OUTPUT_FILE_NAME = "Exp1_" + to_string(MAX_ITER) + string("iter.txt");
//
//	Problem *p;
//	Solution * S;
//	double results[instanceSize][NUM_TRY][2];
//
//	for (size_t i = 0; i < instanceSize; i++)
//	{
//		if (printInfo) {
//			std::cout << instanceSet[i] << "\n";
//		}
//
//		ProblemInstanceReader * pir = new ProblemInstanceReader(instanceSet[i]);
//		p = pir ->GetProblem();
//
//		double alpha = 0.1;
//		for (size_t a = 0; a < 10; a++)
//		{
//			int d = 5;
//			for (size_t b = 0; b < 10; b++)
//			{
//				SEED = 100;
//				
//				for (size_t j = 0; j < NUM_TRY; j++)
//				{
//					if (printInfo) {
//						std::cout << "\talpha:  " << to_string(alpha) << "\t, p:  " << to_string(d) << "\t, Try " << to_string(j) << "\n";
//					}
//
//					srand(SEED++);
//					IteratedGreedy * ig = new IteratedGreedy(p, d, alpha);
//					ig->setPrintInfo(printInfo);
//
//					auto start = std::chrono::high_resolution_clock::now();
//					S = ig->solve(MAX_ITER);
//					auto finish = std::chrono::high_resolution_clock::now();
//
//					std::chrono::duration<double> elapsed = finish - start;
//					results[i][j][0] = S->getObjValue();
//					results[i][j][1] = elapsed.count();
//
//					string line = string(instanceSet[i]) + string(" ") + to_string(j) + string(" ") + to_string(alpha) + string(" ") + to_string(d) + string(" ") + to_string(results[i][j][0]) + string(" ") + to_string(results[i][j][1]);
//
//					if (printInfo) {
//						std::cout << "\t\tObj: " << to_string(S->getObjValue()) << "\n";
//					}
//
//					saveToAFile(EXPERIMENTS_OUTPUT_FOLDER_LOC, OUTPUT_FILE_NAME, line);
//
//					S = NULL;
//					delete ig;
//				}
//
//				d += 5;
//			}
//
//			alpha += 0.1;
//		}
//		delete pir;
//	}
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
