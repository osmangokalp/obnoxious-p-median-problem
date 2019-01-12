#pragma once
#include "Problem.h"
#include <string>

class Solution
{
public:
	Solution(Problem *problem);
	Solution(Problem *problem, int *sigma, int * openFacilitiesList, int *closedFacilitiesList,
		int openFacilityCount, int closedFacilityCount, double objValue);
	~Solution();
	double evaluateFacilityOpening(int facilityToBeOpened) const;
	void openFacility(int facilityToBeOpened, double diff);
	double evaluateFacilityClosing(int facilityToBeClosed) const;
	void closeFacility(int facilityToBeClosed, double diff);
	void evaluateAndCloseFacility(int facilityToBeClosed);
	Solution * cloneSolution() const;
	int getOpenFacilityCount() const;
	int getClosedFacilityCount() const;
	int * getClosedFacilitiesList() const;
	double getObjValue() const;
	int * getOpenFacilitiesList();
	std::string toString() const;

private:	
	//Variables for the problem
	int n;
	int m;
	int p;
	double **DM;
	double objValue;
	Problem *problem;

	//Variables for the algorithm
	int *closedFacilitiesList;
	int *openFacilitiesList;
	int *deltaF;
	int openFacilityCount, closedFacilityCount;
};

