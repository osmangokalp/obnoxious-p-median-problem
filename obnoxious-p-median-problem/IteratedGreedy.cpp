#include "pch.h"
#include "IteratedGreedy.h"
#include "LocalSearch.h"
#include "GreedySelection.h"
#include "Util.h"
#include <iostream>
#include <random>
#include <chrono>

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

Solution * IteratedGreedy::solve(Solution * sol, int MAX_ITER, double MAX_TIME, double alphaMin, double dPercentMean, int GREEDY_SELECTION_MODE, int ALPHA_MODE, int LS_MODE, int D_MODE)
{
	auto start = std::chrono::high_resolution_clock::now();

	S = sol->cloneSolution();
	SStar = S->cloneSolution();
	Solution *SPrime;
	this -> alpha = alphaMin;
	d = floor(dPercentMean * problem->getP());
	if (d == 0) {
		d = 1;
	}

	//stats
	int numOfSuccessiveUnimproved = 0;
	int maxNumOfSuccessiveUnimproved = 0;
	int lastImprovedIter = 0;
	int numOfBestSolutionFound = 0;

	std::default_random_engine generator;
	//std::normal_distribution<double> n_dist_dPercent(dPercentMean, 0.2);
	//std::normal_distribution<double> n_dist_alpha(alphaMin, 0.2);
	std::uniform_real_distribution<double> u_dist_alpha(0.0, 0.8);
	std::uniform_real_distribution<double> u_dist_dPercent(0.3, 0.5);

	iterCounter = 1;
	timeElapsed = 0.0;

	while (iterCounter <= MAX_ITER && timeElapsed <= MAX_TIME)
	{
		SPrime = S->cloneSolution();

		if (ALPHA_MODE == 1) { //random
			alpha = u_dist_alpha(generator);
			/*alpha = n_dist_alpha(generator);
			while (alpha > 1.0 || alpha < 0.0) {
				alpha = n_dist_alpha(generator);
			}*/
		}

		if (D_MODE == 1) {
			/*double dPercent = n_dist_dPercent(generator);
			while (dPercent < 0.1 || dPercent > 0.9) {
				dPercent = n_dist_dPercent(generator);
			}*/
			double dPercent = u_dist_dPercent(generator);
			d = floor(dPercent * problem->getP());
			if (d == 0) {
				d = 1;
			}
		}

		applyDestruction(SPrime, d);
		applyConstruction(SPrime, d, GREEDY_SELECTION_MODE);

		LocalSearch::search(SPrime, LS_MODE);

		//acceptance
		if (SPrime->getObjValue() > S->getObjValue()) {
			delete S;
			S = SPrime;
			lastImprovedIter = iterCounter;
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

		auto finish = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = finish - start;
		timeElapsed = elapsed.count();

		iterCounter++;
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

double IteratedGreedy::getElapsedTime() const
{
	return timeElapsed;
}

int IteratedGreedy::getIterCounter() const
{
	return iterCounter;
}

void IteratedGreedy::applyConstruction(Solution * SPrime, int d, int GREEDY_SELECTION_MODE) const
{
	int selected;
	double diff;

	//construction
	for (int j = 0; j < d; j++) {
		GreedySelection::select(SPrime, alpha, selected, diff, GREEDY_SELECTION_MODE);
		SPrime->openFacility(selected, diff);
	}
}

//remove randomly selected d open facilities
void IteratedGreedy::applyDestruction(Solution * SPrime, int d) const
{
	int *out = selectRandomFromOpenFacilities(SPrime, d);
	double diff;

	//destruction
	for (int j = 0; j < d; j++) {
		SPrime->evaluateAndCloseFacility(out[j]);
	}

	delete[] out;
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