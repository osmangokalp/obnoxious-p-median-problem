#pragma once
#include "Problem.h"

class Solution
{
public:
	Solution(Problem *problem);
	Solution(Problem *problem, int **openFacilitiesPerClient, int *closedFacilitiesList,
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
	int * getCopyOfOpenFacilities();

private:	
	int binarySearchForIntArray(int *arr, int key) const;
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
	int openFacilityCount, closedFacilityCount;
};

