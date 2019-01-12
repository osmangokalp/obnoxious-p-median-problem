#include "pch.h"
#include "Solution.h"
#include "Util.h"
#include <iostream>
#include <string>
using namespace std;

Solution::Solution(Problem *problem)
{
	this->problem = problem;
	this->DM = problem->getDM();
	this->n = problem->getN();
	this->m = problem->getM();
	this->p = problem->getP();

	closedFacilitiesList = new int[m];

	//init closed facilities list as to include all facilities
	for (int i = 0; i < m; i++) {
		closedFacilitiesList[i] = i;
	}

	//create open facilities list
	openFacilitiesList = new int[m];

	//create delta facilities (delta facility: min distance facility for a particular client)
	deltaF = new int[n];

	openFacilityCount = 0;
	closedFacilityCount = m;
	objValue = 0;
}

/*Used to create a new solution from data of an existing solution*/
Solution::Solution(Problem * problem, int * delta, int * openFacilitiesList, int * closedFacilitiesList, int openFacilityCount, int closedFacilityCount, double objValue)
{
	this->problem = problem;
	this->DM = problem->getDM();
	this->n = problem->getN();
	this->m = problem->getM();
	this->p = problem->getP();

	this->closedFacilitiesList = new int[m];
	Util::arrayCopy(closedFacilitiesList, this->closedFacilitiesList, closedFacilityCount);

	this->openFacilitiesList = new int[m];
	Util::arrayCopy(openFacilitiesList, this->openFacilitiesList, openFacilityCount);

	this->deltaF = new int[n];
	Util::arrayCopy(delta, this->deltaF, n);

	this->openFacilityCount = openFacilityCount;
	this->closedFacilityCount = closedFacilityCount;
	this->objValue = objValue;
}

Solution::~Solution()
{
	delete[] closedFacilitiesList;
	delete[] openFacilitiesList;
	delete[] deltaF;
}

double Solution::evaluateFacilityOpening(int facilityToBeOpened) const
{
	double diff = 0;
	if (openFacilityCount != 0) {
		for (int i = 0; i < n; i++) {
			double d = DM[i][facilityToBeOpened] - DM[i][deltaF[i]];
			if (d < 0) {
				diff += d;
			}
		}
	}
	else { //evaluate adding the first facility
		for (int i = 0; i < n; i++) {
			diff += DM[i][facilityToBeOpened];
		}
	}

	return diff;
}

void Solution::openFacility(int facilityToBeOpened, double diff)
{

	//find and exclude facilityToBeOpened from the closed facility list
	int index;

	//search
	for (int i = 0; i < closedFacilityCount; i++) {
		if (closedFacilitiesList[i] == facilityToBeOpened) {
			index = i;
			break;
		}
	}

	//delete
	for (int i = index; i < closedFacilityCount - 1; i++) {
		closedFacilitiesList[i] = closedFacilitiesList[i + 1];
	}


	//insert facility into the openFacilitiesList
	openFacilitiesList[openFacilityCount] = facilityToBeOpened;

	//update delta
	if (openFacilityCount == 0) {
		for (int i = 0; i < n; i++) {
			deltaF[i] = facilityToBeOpened;
		}
	}
	else {
		for (int i = 0; i < n; i++) {
			if (DM[i][facilityToBeOpened] < DM[i][deltaF[i]]) {
				deltaF[i] = facilityToBeOpened;
			}
		}
	}

	openFacilityCount++;
	closedFacilityCount--;
	objValue += diff;
}

double Solution::evaluateFacilityClosing(int facilityToBeClosed) const
{
	double diff = 0;

	if (openFacilityCount < 2) {
		std::cerr << "To evaluate closing, the number of opened facilities must be at least 2!\n";
	}

	for (int i = 0; i < n; i++) {
		if (deltaF[i] == facilityToBeClosed) { // first element to be removed
			int secondMin = INT_MAX;
			int secondMinF = -1;
			for (int j = 0; j < openFacilityCount; j++) {
				int f = openFacilitiesList[j];
				if (f != facilityToBeClosed) {
					if (DM[i][f] < secondMin) {
						secondMin = DM[i][f];
						secondMinF = f;
					}
				}
			}
			diff += secondMin - DM[i][facilityToBeClosed];
		}

	}

	return diff;
}

void Solution::closeFacility(int facilityToBeClosed, double diff)
{
	int index = -1;

	if (openFacilityCount < 2) {
		std::cerr << "Close operation requires at least 2 opened facility!\n";
	}

	//find and exclude facilityToBeOpened from the closed facility list

	//find
	for (int i = 0; i < openFacilityCount; i++) {
		if (openFacilitiesList[i] == facilityToBeClosed) {
			index = i;
			break;
		}
	}

	//exclude
	for (int i = index; i < openFacilityCount - 1; i++) {
		openFacilitiesList[i] = openFacilitiesList[i + 1];
	}

	openFacilityCount--;

	//insert facility into the closedFacilitiesList
	closedFacilitiesList[closedFacilityCount++] = facilityToBeClosed;

	//update delta
	for (int i = 0; i < n; i++) {
		if (deltaF[i] == facilityToBeClosed) { // first element to be removed
			int secondMin = INT_MAX;
			int secondMinF = -1;
			for (int j = 0; j < openFacilityCount; j++) {
				int f = openFacilitiesList[j];
				if (DM[i][f] < secondMin) {
					secondMin = DM[i][f];
					secondMinF = f;
				}
			}
			deltaF[i] = secondMinF;
		}

	}
	
	objValue += diff;
}

void Solution::evaluateAndCloseFacility(int facilityToBeClosed) {
	int index = -1;
	double diff = 0;

	if (openFacilityCount < 2) {
		std::cerr << "Close operation requires at least 2 opened facility!\n";
	}

	//find and exclude facilityToBeOpened from the closed facility list

	//find
	for (int i = 0; i < openFacilityCount; i++) {
		if (openFacilitiesList[i] == facilityToBeClosed) {
			index = i;
			break;
		}
	}

	//exclude
	for (int i = index; i < openFacilityCount - 1; i++) {
		openFacilitiesList[i] = openFacilitiesList[i + 1];
	}

	openFacilityCount--;

	//insert facility into the closedFacilitiesList
	closedFacilitiesList[closedFacilityCount++] = facilityToBeClosed;

	//calculate diff and update delta
	for (int i = 0; i < n; i++) {
		if (deltaF[i] == facilityToBeClosed) { // first element to be removed
			int secondMin = INT_MAX;
			int secondMinF = -1;
			for (int j = 0; j < openFacilityCount; j++) {
				int f = openFacilitiesList[j];
				if (DM[i][f] < secondMin) {
					secondMin = DM[i][f];
					secondMinF = f;
				}
			}
			deltaF[i] = secondMinF;
			diff += secondMin - DM[i][facilityToBeClosed];
		}
	}

	objValue += diff;
}

Solution * Solution::cloneSolution() const
{
	return new Solution(problem, deltaF, openFacilitiesList, closedFacilitiesList,
		openFacilityCount, closedFacilityCount, objValue);
}

int Solution::getOpenFacilityCount() const
{
	return openFacilityCount;
}

int Solution::getClosedFacilityCount() const
{
	return closedFacilityCount;
}

int * Solution::getClosedFacilitiesList() const
{
	return closedFacilitiesList;
}

double Solution::getObjValue() const
{
	return objValue;
}

int * Solution::getOpenFacilitiesList()
{
	return openFacilitiesList;
}

string Solution::toString() const
{
	string s = "";
	for (size_t i = 0; i < openFacilityCount; i++)
	{
		s += to_string(openFacilitiesList[i]) + " ";
	}
	s += "\nObj. Value: " + to_string(getObjValue());
	return s;
}
