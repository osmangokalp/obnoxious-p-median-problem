#pragma once
#include "Problem.h"
#include <string>

class Solution
{
public:
	Solution(Problem *problem);
	Solution(Problem *problem, int **openFacilitiesPerClient, int * openFacilitiesList, int *closedFacilitiesList,
		int openFacilityCount, int closedFacilityCount, double objValue);
	~Solution();
	double evaluateFacilityOpening(int facilityToBeOpened) const;
	void openFacility(int facilityToBeOpened, double diff);
	double evaluateFacilityExchange(int j, int jPrime) const;
	double evaluateFacilityClosing(int facilityToBeClosed) const;
	void closeFacility(int facilityToBeClosed, double diff);
	int selectMostFrequentAtFirst() const;
	Solution * cloneSolution() const;
	int getOpenFacilityCount() const;
	int getClosedFacilityCount() const;
	int * getClosedFacilitiesList() const;
	double getObjValue() const;
	int * getOpenFacilitiesList();
	std::string toString() const;

private:	
	int binarySearchForIntArray(int *arr, int len, int key) const;
	int binarySearchForClientDistances(int i, int length, int facilityToBeOpened) const;
	void insert(int i, int index, int facilityToBeOpened);
	void remove(int i, int index);

	//Variables for the problem
	int n;
	int m;
	int p;
	double **DM;
	double objValue;
	Problem *problem;

	//Variables for the algorithm
	int **openFacilitiesPerClient;
	int *closedFacilitiesList;
	int *openFacilitiesList;
	int openFacilityCount, closedFacilityCount;
};

