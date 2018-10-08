#include "pch.h"
#include "GC2.h"
#include "Util.h"
#include <iostream>

GC2::GC2()
{
}


GC2::~GC2()
{
}

void GC2::selectCandidate(Solution *sol, double alpha, int & selected, double & diff)
{
	int ofc = sol->getOpenFacilityCount();
	int cfc = sol->getClosedFacilityCount();
	if (ofc < 1) {
		std::cerr << "Solution must have at least one open facility";
	}

	int *CL = new int[cfc];
	Util::arrayCopy(sol->getClosedFacilitiesList(), CL, cfc);
	int size = (int)(cfc * alpha);
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
