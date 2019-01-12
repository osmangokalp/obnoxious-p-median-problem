#include "pch.h"
#include "GreedySelection.h"
#include "Util.h"
#include "Problem.h"
#include <iostream>

GreedySelection::GreedySelection()
{
}


GreedySelection::~GreedySelection()
{
}

void GreedySelection::selectRandomPlusGreedy(Solution *sol, double alpha, int & selected, double & diff)
{
	int ofc = sol->getOpenFacilityCount();
	int cfc = sol->getClosedFacilityCount();
	if (ofc < 1) {
		std::cerr << "Solution must have at least one open facility";
	}

	int *CL = new int[cfc];
	Util::arrayCopy(sol->getClosedFacilitiesList(), CL, cfc);
	int size = (int)(cfc * alpha);

	if (size == 0) {
		size = 1;
	}

	int *RCL = new int[size];

	int remainingCandidateCount = cfc;
	for (int i = 0; i < size; i++) {
		int randomIndex = (rand() % remainingCandidateCount);
		RCL[i] = CL[randomIndex];

		//shift array
		for (int j = randomIndex; j < remainingCandidateCount - 1; j++) {
			CL[j] = CL[j + 1];
		}

		remainingCandidateCount--;
	}

	double maxDiff = -DBL_MAX;
	int sel = -1;

	for (int i = 0; i < size; i++) {
		double d = sol->evaluateFacilityOpening(RCL[i]);
		if (d > maxDiff) {
			maxDiff = d;
			sel = RCL[i];
		}
	}

	delete[] CL;
	delete[] RCL;

	selected = sel;
	diff = maxDiff;
}

void GreedySelection::selectGRASP(Solution *sol, double alpha, int & selected, double & diff)
{
	int ofc = sol->getOpenFacilityCount();
	int cfc = sol->getClosedFacilityCount();
	if (ofc < 1) {
		std::cerr << "Solution must have at least one open facility";
	}

	int *CL = new int[cfc];
	Util::arrayCopy(sol->getClosedFacilitiesList(), CL, cfc);
	int *RCL = new int[cfc];

	double gMin = INT_MAX;
	double gMax = INT_MIN;
	double *delta = new double[ofc + cfc];

	for (int i = 0; i < cfc; i++) {
		delta[CL[i]] = sol->evaluateFacilityOpening(CL[i]);
		if (delta[CL[i]] < gMin) {
			gMin = delta[CL[i]];
		}
		if (delta[CL[i]] > gMax) {
			gMax = delta[CL[i]];
		}
	}

	int RCL_length = 0;
	for (int i = 0; i < cfc; i++) {
		if (delta[CL[i]] >= gMin + alpha * (gMax - gMin)) {
			RCL[RCL_length++] = CL[i];
		}
	}

	int sel = -1;
	int randomIndex = (rand() % RCL_length);
	sel = RCL[randomIndex];
	double d = delta[RCL[randomIndex]];

	delete[] CL;
	delete[] RCL;
	delete[] delta;

	selected = sel;
	diff = d;
}

void GreedySelection::select(Solution * sol, double alpha, int & selected, double & diff, int selectMode)
{
	if (selectMode == 0) {
		selectRandomPlusGreedy(sol, alpha, selected, diff);
	}
	else {
		selectGRASP(sol, alpha, selected, diff);
	}
}

Solution * GreedySelection::constructSolution(Problem * problem, double alpha, int selectMode)
{
	Solution *sol = new Solution(problem);

	//open first facility randomly
	int selected = (rand() % problem->getM());
	double diff = sol->evaluateFacilityOpening(selected);
	sol->openFacility(selected, diff);

	while (sol->getOpenFacilityCount() != problem->getP()) {
		if (selectMode == 0) {
			GreedySelection::selectRandomPlusGreedy(sol, alpha, selected, diff);
		}
		else {
			GreedySelection::selectGRASP(sol, alpha, selected, diff);
		}
		sol->openFacility(selected, diff);
	}

	return sol;
}

