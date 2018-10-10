#include "pch.h"
#include "IGxSA.h"
#include "LS1.h"
#include "GC2.h"
#include "Util.h"
#include <iostream>

IGxSA::IGxSA(Problem * problem, int d, double alpha)
{
	this->problem = problem;
	this->d = d;
	this->alpha = alpha;
	this->printInfo = false;
	this->S = NULL;
	this->SStar = NULL;
}

IGxSA::~IGxSA()
{
	delete SStar;
	delete S;
}

Solution * IGxSA::solve(Solution * sol, int MAX_ITER)
{
	S = sol->cloneSolution();
	SStar = S->cloneSolution();
	Solution *SPrime;
	double thresh = SStar ->getObjValue() / 5;
	double threshFactor = 0.95;

	//stats
	int numOfSuccessiveUnimproved = 0;
	int maxNumOfSuccessiveUnimproved = 0;
	int numOfCallsToApplyDestruction1 = 0;
	int numOfCallsToApplyDestruction2 = 0;
	int lastImprovedIter = 0;

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

		//LS1::search(SPrime);

		//acceptance
		if (SPrime->getObjValue() + thresh > S->getObjValue()) {
			delete S;
			S = SPrime;
			
			if (S->getObjValue() > SStar->getObjValue()) {
				delete SStar;
				SStar = S->cloneSolution();
				lastImprovedIter = i;
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
		thresh *= threshFactor;
	}

	if (printInfo) {
		std::cout << "\t\tlastImprovedIter: " << lastImprovedIter << "\n";
		std::cout << "\t\tmaxNumOfSuccessiveUnimproved: " << maxNumOfSuccessiveUnimproved << "\n";
		std::cout << "\t\tnumOfCallsToApplyDestruction1: " << numOfCallsToApplyDestruction1 << "\n";
		std::cout << "\t\tnumOfCallsToApplyDestruction2: " << numOfCallsToApplyDestruction2 << "\n";
	}

	return SStar;
}

void IGxSA::setPrintInfo(bool b)
{
	this->printInfo = b;
}

void IGxSA::applyConstruction1(Solution * SPrime, int d) const
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
void IGxSA::applyDestruction1(Solution * SPrime, int d) const
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
void IGxSA::applyDestruction2(Solution * SPrime, int d) const
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

int * IGxSA::selectRandomFromOpenFacilities(Solution * sol, int d) const
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