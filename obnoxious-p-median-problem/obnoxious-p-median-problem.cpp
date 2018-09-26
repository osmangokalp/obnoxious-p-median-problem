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
using namespace std;

int main()
{
	int SEED = 103;
	srand(SEED);
	int instance = 0;
	int MAX_ITER = 1000;
	int d = 50;
	double alpha = 0.35;

	string testInsances[] = { "pmed29.txt.table.p150.A.txt", "pmed32.txt.table.p175.A.txt", "pmed40.txt.table.p225.A.txt",
		"pmed20.txt.table.p100.B.txt", "pmed40.txt.table.p225.B.txt" };
	double BKS[] = { 4141, 4242, 4571, 4067, 4524 };
	

	ProblemInstanceReader pir(testInsances[instance]);
	Problem *p = pir.GetProblem();

	IteratedGreedy ig(p, d,alpha);

	auto start = std::chrono::high_resolution_clock::now();

	ig.solve(MAX_ITER);

	auto finish = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> elapsed = finish - start;
	std::cout << "Elapsed time: " << elapsed.count() << " s\n";

	std::getchar();
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
