// obnoxious-p-median-problem.cpp : Bu dosya 'main' işlevi içeriyor. Program yürütme orada başlayıp biter.
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

static void saveToTheFile(string EXPERIMENTS_OUTPUT_FOLDER_LOC, string outputFileName, string line) {
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
	//Experiment Parameters
	int SEED;
	const int NUM_TRY = 10;
	int MAX_ITER = 500;
	bool printInfo = true;
	
	//instance sets
	string instanceSet[] = { "pmed29.txt.table.p150.A.txt", "pmed32.txt.table.p175.A.txt", "pmed40.txt.table.p225.A.txt",
		"pmed20.txt.table.p100.B.txt", "pmed40.txt.table.p225.B.txt" };
	double BKS[] = { 4141, 4242, 4571, 4067, 4524 };
	const int instanceSize = 5;

	//File output
	string EXPERIMENTS_OUTPUT_FOLDER_LOC = "C:\\Users\\osman\\OneDrive\\Belgeler\\Okul\\Academic Works\\Continuing\\obnoxious p-median\\Experiments";
	string OUTPUT_FILE_NAME = "results" + to_string(MAX_ITER) + string("iter.txt");

	Problem *p;
	Solution * S;
	double results[instanceSize][NUM_TRY][2];

	for (size_t i = 0; i < instanceSize; i++)
	{
		SEED = 100;
		ProblemInstanceReader pir(instanceSet[i]);
		p = pir.GetProblem();
		for (size_t j = 0;  j < NUM_TRY;  j++)
		{
			srand(SEED++);
			IteratedGreedy ig(p, 50, 0.35);

			auto start = std::chrono::high_resolution_clock::now();
			S = ig.solve(MAX_ITER);
			auto finish = std::chrono::high_resolution_clock::now();

			std::chrono::duration<double> elapsed = finish - start;
			results[i][j][0] = S->getObjValue();
			results[i][j][1] = elapsed.count();

			string line = string(instanceSet[i]) + string(" ") + to_string(j) + string(" ") + to_string(results[i][j][0]) + string(" ") + to_string(results[i][j][1]);
			if (printInfo) {
				std::cout << line << "\n";
			}

			saveToTheFile(EXPERIMENTS_OUTPUT_FOLDER_LOC, OUTPUT_FILE_NAME, line);

			delete S;
		}
	}

}

// Programı çalıştır: Ctrl + F5 veya Hata Ayıkla > Hata Ayıklamadan Başlat menüsü
// Programda hata ayıkla: F5 veya Hata Ayıkla > Hata Ayıklamayı Başlat menüsü

// Kullanmaya Başlama İpuçları: 
//   1. Dosyaları eklemek/yönetmek için Çözüm Gezgini penceresini kullanın
//   2. Kaynak denetimine bağlanmak için Takım Gezgini penceresini kullanın
//   3. Derleme çıktısını ve diğer iletileri görmek için Çıktı penceresini kullanın
//   4. Hataları görüntülemek için Hata Listesi penceresini kullanın
//   5. Yeni kod dosyaları oluşturmak için Projeye Git > Yeni Öğe ekle veya varolan kod dosyalarını projeye eklemek için Proje > Var Olan Öğeyi Ekle adımlarını izleyin
//   6. Bu projeyi daha sonra yeniden açmak için Dosya > Aç > Proje'ye gidip .sln uzantılı dosyayı seçin
