#include "pch.h"
#include "Solution.h"
#include "Util.h"
#include <iostream>

Solution::Solution(Problem *problem)
{
	this->problem = problem;
	this->DM = problem->getDM();
	this->n = problem->getN();
	this->m = problem->getM();
	this->p = problem->getP();

	openFacilitiesPerClient = new int *[n];
	for (size_t i = 0; i < n; i++)
	{
		openFacilitiesPerClient[i] = new int[p] {0}; //init by 0
	}

	closedFacilitiesList = new int[m];

	//init closed facilities list as to include all facilities in an ascending order
	for (int i = 0; i < m; i++) {
		closedFacilitiesList[i] = i;
	}

	//init open facilities list
	openFacilitiesList = new int[m];

	openFacilityCount = 0;
	closedFacilityCount = m;
	objValue = 0;
}

Solution::Solution(Problem * problem, int ** openFacilitiesPerClient, int * openFacilitiesList, int * closedFacilitiesList, int openFacilityCount, int closedFacilityCount, double objValue)
{
	this->problem = problem;
	this->DM = problem->getDM();
	this->n = problem->getN();
	this->m = problem->getM();
	this->p = problem->getP();

	this->openFacilitiesPerClient = new int *[n];
	for (size_t i = 0; i < n; i++)
	{
		this->openFacilitiesPerClient[i] = new int[p];
	}

	//init open facilities per client
	for (int i = 0; i < n; i++) {
		Util::arrayCopy(openFacilitiesPerClient[i], this->openFacilitiesPerClient[i], openFacilityCount);
	}

	this->closedFacilitiesList = new int[m];
	//init closed facilities list as to include all facilities in an ascending order
	Util::arrayCopy(closedFacilitiesList, this->closedFacilitiesList, closedFacilityCount);

	this->openFacilitiesList = new int[m];
	//init open facilities list as to include all facilities in an ascending order
	Util::arrayCopy(openFacilitiesList, this->openFacilitiesList, openFacilityCount);

	this->openFacilityCount = openFacilityCount;
	this->closedFacilityCount = closedFacilityCount;
	this->objValue = objValue;
}

Solution::~Solution()
{
	for (size_t i = 0; i < n; i++)
	{
		delete[] openFacilitiesPerClient[i];
	}
	delete[] openFacilitiesPerClient;

	delete[] closedFacilitiesList;
	delete[] openFacilitiesList;
}

double Solution::evaluateFacilityOpening(int facilityToBeOpened) const
{
	double diff = 0;
	if (openFacilityCount != 0) {
		for (int i = 0; i < n; i++) {
			double d = DM[i][facilityToBeOpened] - DM[i][openFacilitiesPerClient[i][0]];
			if (d < 0) {
				diff += d;
			}
		}
	}
	else { //first facility to be added
		for (int i = 0; i < n; i++) {
			diff += DM[i][facilityToBeOpened];
		}
	}

	return diff;
}

void Solution::openFacility(int facilityToBeOpened, double diff)
{
	int index;

	//insert facilityToBeOpened into an appropriate positions for each client's list
	for (int i = 0; i < n; i++) {
		if (openFacilityCount > 0) {
			index = binarySearchForClientDistances(i, openFacilityCount, facilityToBeOpened);
		}
		else {
			index = 0;
		}
		insert(i, index, facilityToBeOpened);
	}

	//find and exclude facilityToBeOpened from the closed facility list
	index = binarySearchForIntArray(closedFacilitiesList, closedFacilityCount, facilityToBeOpened);
	for (int i = index; i < closedFacilityCount - 1; i++) {
		closedFacilitiesList[i] = closedFacilitiesList[i + 1];
	}

	//insert facility into an appropriate position for the openFacilitiesList
	index = binarySearchForIntArray(openFacilitiesList, openFacilityCount, facilityToBeOpened);
	for (int i = openFacilityCount; i > index; i--) {
		openFacilitiesList[i] = openFacilitiesList[i - 1];
	}
	openFacilitiesList[index] = facilityToBeOpened;

	openFacilityCount++;
	closedFacilityCount--;
	objValue += diff;
}

double Solution::evaluateFacilityExchange(int j, int jPrime) const
{
	double diff, totalDiff = 0;

	for (int i = 0; i < n; i++) {
		int f1 = openFacilitiesPerClient[i][0]; //min
		if (f1 == j) {
			int f2 = openFacilitiesPerClient[i][1]; //second min
			diff = Util::min(DM[i][f2] - DM[i][f1], DM[i][jPrime] - DM[i][f1]);
		}
		else {
			diff = Util::min(0, DM[i][jPrime] - DM[i][f1]);
		}
		totalDiff += diff;
	}

	return totalDiff;
}

double Solution::evaluateFacilityClosing(int facilityToBeClosed) const
{
	double diff = 0;

	if (openFacilityCount < 2) {
		std::cerr << "To evaluate closing, the number of opened facilities must be at least 2!\n";
	}

	for (int i = 0; i < n; i++) {
		if (openFacilitiesPerClient[i][0] == facilityToBeClosed) {
			diff += DM[i][openFacilitiesPerClient[i][1]] - DM[i][openFacilitiesPerClient[i][0]];
		}
	}

	return diff;
}

void Solution::closeFacility(int facilityToBeClosed, double diff)
{
	int index;

	if (openFacilityCount < 1) {
		std::cerr << "Close operation requires at least one opened facility!\n";
	}

	//find and remove facility from the open facilities
	for (int i = 0; i < n; i++) {
		index = binarySearchForClientDistances(i, openFacilityCount, facilityToBeClosed);

		if (openFacilitiesPerClient[i][index] != facilityToBeClosed) { //if it is not the facility that we are searching for
			bool found = false;

			//search left
			for (int j = index - 1; j > -1; j--) {
				if (DM[i][openFacilitiesPerClient[i][j]] != DM[i][facilityToBeClosed]) { //not found, break the loop
					break;
				}
				if (openFacilitiesPerClient[i][j] == facilityToBeClosed) { //found
					found = true;
					index = j;
				}
			}

			if (!found) {
				//search right
				for (int j = index + 1; j < openFacilityCount; j++) {
					if (DM[i][openFacilitiesPerClient[i][j]] != DM[i][facilityToBeClosed]) { //not found, break the loop
						std::cerr << "Facility could not found!\n";
					}
					if (openFacilitiesPerClient[i][j] == facilityToBeClosed) { //found
						index = j;
						break;
					}
				}
			}
		}
		remove(i, index);
	}

	//insert facility into an appropriate position for the closedFacilitiesList
	index = binarySearchForIntArray(closedFacilitiesList, closedFacilityCount, facilityToBeClosed);
	for (int i = closedFacilityCount; i > index; i--) {
		closedFacilitiesList[i] = closedFacilitiesList[i - 1];
	}
	closedFacilitiesList[index] = facilityToBeClosed;

	//find and exclude facilityToBeOpened from the open facility list
	index = binarySearchForIntArray(openFacilitiesList, openFacilityCount, facilityToBeClosed);
	for (int i = index; i < openFacilityCount - 1; i++) {
		openFacilitiesList[i] = openFacilitiesList[i + 1];
	}

	openFacilityCount--;
	closedFacilityCount++;
	objValue += diff;
}

int Solution::selectMostFrequentAtFirst() const
{
	int *frequency = new int[m] {0};

	int max = 0;
	int selected = -1;
	for (int i = 0; i < n; i++) {
		int f = openFacilitiesPerClient[i][0];
		if (++frequency[f] > max) {
			max = frequency[f];
			selected = f;
		}

	}

	delete[] frequency;

	return selected;
}

int Solution::binarySearchForIntArray(int *arr, int len, int key) const {
	int low = 0;
	int high = len - 1;

	while (low <= high) {
		int mid = (low + high) / 2;
		int midVal = arr[mid];

		if (midVal < key) {
			low = mid + 1;
		}
		else if (midVal > key) {
			high = mid - 1;
		}
		else {
			return mid; // key found
		}
	}
	return low;  // key not found
}

int Solution::binarySearchForClientDistances(int i, int length, int facilityToBeOpened) const {
	int *arr = openFacilitiesPerClient[i];
	int low = 0;
	int high = length - 1;

	while (low <= high) {
		int mid = (low + high) / 2;
		double midVal = DM[i][arr[mid]];

		if (midVal < DM[i][facilityToBeOpened]) {
			low = mid + 1;
		}
		else if (midVal > DM[i][facilityToBeOpened]) {
			high = mid - 1;
		}
		else {
			return mid; // key found. Return key position
		}
	}
	return low;  // key not found. Return insertion position
};

Solution * Solution::cloneSolution() const
{
	return new Solution(problem, openFacilitiesPerClient, openFacilitiesList, closedFacilitiesList,
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

void Solution::insert(int i, int index, int facilityToBeOpened)
{
	int *arr = openFacilitiesPerClient[i];
	for (int j = openFacilityCount - 1; j >= index; j--) {
		arr[j + 1] = arr[j];
	}
	arr[index] = facilityToBeOpened;
}

void Solution::remove(int i, int index)
{
	int *arr = openFacilitiesPerClient[i];
	for (int j = index; j < openFacilityCount - 1; j++) {
		arr[j] = arr[j + 1];
	}
}
