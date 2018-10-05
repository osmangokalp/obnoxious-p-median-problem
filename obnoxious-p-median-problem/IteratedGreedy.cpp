#include "pch.h"
#include "IteratedGreedy.h"
#include "LS1.h"
#include "GC2.h"
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

Solution * IteratedGreedy::solve(int MAX_ITER)
{
	S = constructSolution();
	LS1::search(S);
	SStar = S->cloneSolution();
	Solution *SPrime;

	//stats
	int numOfSuccessiveUnimproved = 0;
	int maxNumOfSuccessiveUnimproved = 0;
	int numOfCallsToApplyDestruction1 = 0;
	int numOfCallsToApplyDestruction2 = 0;

	for (size_t i = 0; i < MAX_ITER; i++)
	{
		SPrime = S->cloneSolution();

		if (numOfSuccessiveUnimproved == 0) {
			applyDestruction2(SPrime, d);
			numOfCallsToApplyDestruction2++;
		}
		else {
			applyDestruction1(SPrime, d);
			numOfCallsToApplyDestruction1++;
		}
		applyConstruction1(SPrime, d);

		LS1::search(SPrime);

		//acceptance
		if (SPrime->getObjValue() > S->getObjValue()) {
			delete S;
			S = SPrime;
			if (S->getObjValue() > SStar->getObjValue()) {
				delete SStar;
				SStar = S->cloneSolution();
			}

			///*if (printInfo) {
			//	std::cout << "\t\tITER: " << i << "\t" << S->getObjValue() << "\n";
			//}*/
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
		std::cout << "\t\tmaxNumOfSuccessiveUnimproved: " << maxNumOfSuccessiveUnimproved << "\n";
		std::cout << "\t\tnumOfCallsToApplyDestruction1: " << numOfCallsToApplyDestruction1 << "\n";
		std::cout << "\t\tnumOfCallsToApplyDestruction2: " << numOfCallsToApplyDestruction2 << "\n";
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

	delete out;
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
	int *openFacilities = sol->getCopyOfOpenFacilities();
	int openFacilitiesCount = sol->getOpenFacilityCount();

	int counter = 0;

	while (counter < d) {
		int randIndex =(rand() % openFacilitiesCount);
		int candidate = openFacilities[randIndex];
		bool reSelect = false;
		for (int i = 0; i < counter; i++) {
			if (toBeClosed[i] == candidate) {
				reSelect = true;
				break;
			}
		}
		if (!reSelect) {
			toBeClosed[counter++] = candidate;
		}
	}

	delete openFacilities;

	return toBeClosed;
}

Solution * IteratedGreedy::constructSolution() const
{
	Solution *sol = new Solution(problem);

	//open first facility randomly
	int selected = (rand() % problem->getM());
	double diff = sol->evaluateFacilityOpening(selected);
	sol->openFacility(selected, diff);

	while (sol->getOpenFacilityCount() != problem->getP()) {
		GC2::selectCandidate(sol, alpha, selected, diff);
		sol->openFacility(selected, diff);
	}

	return sol;
}