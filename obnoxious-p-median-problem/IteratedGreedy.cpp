#include "pch.h"
#include "IteratedGreedy.h"
#include "LS1.h"
#include "RLS1.h"
#include "GC2.h"
#include "Util.h"
#include <iostream>

IteratedGreedy::IteratedGreedy(Problem * problem, int d, double alpha)
{
	this->problem = problem;
	this->d = d;
	this->alpha = alpha;
	this->printInfo = false;
	this->S = NULL;
	this->SStar = NULL;
}

IteratedGreedy::~IteratedGreedy()
{
	delete SStar;
	delete S;
}

Solution * IteratedGreedy::solve(Solution * sol, int MAX_ITER)
{
	S = sol->cloneSolution();
	SStar = S->cloneSolution();
	Solution *SPrime;

	double alphaMin = 0.1;
	double alphaMax = 0.9;

	//stats
	int numOfSuccessiveUnimproved = 0;
	int maxNumOfSuccessiveUnimproved = 0;
	int lastImprovedIter = 0;
	int numOfBestSolutionFound = 0;

	for (size_t i = 0; i < MAX_ITER; i++)
	{		
		SPrime = S->cloneSolution();
		double pFactor = (((double)rand() / (RAND_MAX)) / 2.0) + 0.3; //rand[0.3,0.8]
		d = floor(pFactor * problem->getP());

		if (numOfSuccessiveUnimproved == 0) {
			//applyDestruction2(SPrime, d);
			alpha = alphaMax;
		}
		else {
			//applyDestruction1(SPrime, d);
			alpha -= 0.05;
			if (alpha < alphaMin) {
				alpha = alphaMin;
			}
		}

		applyDestruction1(SPrime, d);
		applyConstruction1(SPrime, d);

		RLS1::search(SPrime);

		//acceptance
		if (SPrime->getObjValue() > S->getObjValue()) {
			delete S;
			S = SPrime;
			lastImprovedIter = i;
			if (S->getObjValue() > SStar->getObjValue()) {
				delete SStar;
				SStar = S->cloneSolution();
				numOfBestSolutionFound++;
			}

			numOfSuccessiveUnimproved = 0;
		}
		else {
			numOfSuccessiveUnimproved++;
			if (numOfSuccessiveUnimproved > maxNumOfSuccessiveUnimproved) {
				maxNumOfSuccessiveUnimproved = numOfSuccessiveUnimproved;
			}
			delete SPrime;
		}
	}

	if (printInfo) {
		std::cout << "\t\tlastImprovedIter: " << lastImprovedIter << "\n";
		std::cout << "\t\tmaxNumOfSuccessiveUnimproved: " << maxNumOfSuccessiveUnimproved << "\n";
		std::cout << "\t\tnumOfBestSolutionFound: " << numOfBestSolutionFound << "\n";
	}

	return SStar;
}

void IteratedGreedy::setPrintInfo(bool b)
{
	this->printInfo = b;
}

void IteratedGreedy::applyConstruction1(Solution * SPrime, int d) const
{
	int selected;
	double diff;

	//construction
	for (int j = 0; j < d; j++) {
		GC2::selectCandidate(SPrime, alpha, selected, diff);
		SPrime->openFacility(selected, diff);
	}
}

//remove randomly selected d open facilities
void IteratedGreedy::applyDestruction1(Solution * SPrime, int d) const
{
	int *out = selectRandomFromOpenFacilities(SPrime, d);
	double diff;

	//destruction
	for (int j = 0; j < d; j++) {
		diff = SPrime->evaluateFacilityClosing(out[j]);
		SPrime->closeFacility(out[j], diff);
	}

	delete[] out;
}

//remove the most frequent facility d times
void IteratedGreedy::applyDestruction2(Solution * SPrime, int d) const
{
	int selected;
	double diff;

	//destruction
	for (int j = 0; j < d; j++) {
		selected = SPrime->selectMostFrequentAtFirst();
		diff = SPrime->evaluateFacilityClosing(selected);
		SPrime->closeFacility(selected, diff);
	}
}

int * IteratedGreedy::selectRandomFromOpenFacilities(Solution * sol, int d) const
{
	int *toBeClosed = new int[d];
	int openFacilitiesCount = sol->getOpenFacilityCount();
	int *openFacilities = new int[openFacilitiesCount];
	Util::arrayCopy(S->getOpenFacilitiesList(), openFacilities, openFacilitiesCount);

	int remainingCount = openFacilitiesCount;
	for (int i = 0; i < d; i++) {
		int randomIndex = (rand() % remainingCount);
		toBeClosed[i] = openFacilities[randomIndex];

		//shift array
		for (int j = randomIndex; j < remainingCount - 1; j++) {
			openFacilities[j] = openFacilities[j + 1];
		}

		remainingCount--;
	}

	delete[] openFacilities;

	return toBeClosed;
}