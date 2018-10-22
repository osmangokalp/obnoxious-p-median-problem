#include "pch.h"
#include "IteratedGreedy.h"
#include "LocalSearch.h"
#include "GC2.h"
#include "Util.h"
#include <iostream>
#include <random>

IteratedGreedy::IteratedGreedy(Problem * problem)
{
	this->problem = problem;
	this->printInfo = false;
	this->S = NULL;
	this->SStar = NULL;
}

IteratedGreedy::~IteratedGreedy()
{
	delete SStar;
	delete S;
}

Solution * IteratedGreedy::solve(Solution * sol, int MAX_ITER, double alphaMax, double alphaMin, double alphaInc)
{
	S = sol->cloneSolution();
	SStar = S->cloneSolution();
	Solution *SPrime;

	//stats
	int numOfSuccessiveUnimproved = 0;
	int maxNumOfSuccessiveUnimproved = 0;
	int lastImprovedIter = 0;
	int numOfBestSolutionFound = 0;

	std::default_random_engine generator;
	std::normal_distribution<double> distribution(0.5, 0.2);

	for (size_t i = 0; i < MAX_ITER; i++)
	{
		SPrime = S->cloneSolution();

		if (numOfSuccessiveUnimproved == 0) {
			alpha = alphaMax;  //high level exploitation
		}
		else {
			alpha -= alphaInc; // increase exploration
			if (alpha < alphaMin) {
				alpha = alphaMin;
			}
		}

		double pFactor = distribution(generator);
		while (pFactor < 0.1 || pFactor > 0.9) {
			pFactor = distribution(generator);
		}
		d = floor(pFactor * problem->getP());

		applyDestruction1(SPrime, d);
		applyConstruction1(SPrime, d);

		LocalSearch::RLS1(SPrime);

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