#include "pch.h"
#include "LocalSearch.h"
#include "Util.h"
#include <iostream>


LocalSearch::LocalSearch()
{
}


LocalSearch::~LocalSearch()
{
}

double LocalSearch::LS1(Solution * S)
{
	double totalDiff = 0;
	int openFacilitiesCount = S->getOpenFacilityCount();
	int closedFacilitiesCount = S->getClosedFacilityCount();

	bool improve = true;
	while (improve) {
		improve = false;
		int *openFacilities = new int[openFacilitiesCount];
		Util::arrayCopy(S->getOpenFacilitiesList(), openFacilities, openFacilitiesCount);
		int *closedFacilities = new int[closedFacilitiesCount];
		Util::arrayCopy(S->getClosedFacilitiesList(), closedFacilities, closedFacilitiesCount);
		Util::shuffle(openFacilities, openFacilitiesCount);
		Util::shuffle(closedFacilities, closedFacilitiesCount);
		int index1 = 0, index2 = 0;

		while (index1 < openFacilitiesCount && !improve) {
			while (index2 < closedFacilitiesCount && !improve) {
				double diff = S->evaluateFacilityExchange(openFacilities[index1], closedFacilities[index2]);
				if (diff > 0) {
					double d1 = S->evaluateFacilityClosing(openFacilities[index1]);
					S->closeFacility(openFacilities[index1], d1);
					double d2 = S->evaluateFacilityOpening(closedFacilities[index2]);
					S->openFacility(closedFacilities[index2], d2);

					if (d1 + d2 != diff) {
						std::cerr << "d1 + d2 != diff at facility exchange!";
					}

					totalDiff += diff;
					improve = true;
				}
				index2++;
			}
			index1++;
		}
		delete[] openFacilities;
		delete[] closedFacilities;
	}

	return totalDiff;
}

//first remove best, then add best
double LocalSearch::RLS1(Solution * S)
{
	double totalDiff = 0;
	int openFacilitiesCount = S->getOpenFacilityCount();
	int closedFacilitiesCount = S->getClosedFacilityCount();
	int *openFacilities = S->getOpenFacilitiesList();
	int *closedFacilities = S->getClosedFacilitiesList();

	bool improve = true;
	while (improve) {
		improve = false;

		double deltaClose = -1;
		int toBeClosed = -1;
		for (size_t i = 0; i < openFacilitiesCount; i++)
		{
			double d = S->evaluateFacilityClosing(openFacilities[i]);
			if (d > deltaClose) {
				deltaClose = d;
				toBeClosed = openFacilities[i];
			}
		}

		S->closeFacility(toBeClosed, deltaClose);
		closedFacilitiesCount++;
		openFacilitiesCount--;

		double deltaOpen = -999999999;
		int toBeOpened = -1;
		for (size_t i = 0; i < closedFacilitiesCount; i++)
		{
			double d = S->evaluateFacilityOpening(closedFacilities[i]);
			if (d > deltaOpen) {
				deltaOpen = d;
				toBeOpened = closedFacilities[i];
			}
		}

		S->openFacility(toBeOpened, deltaOpen);
		closedFacilitiesCount--;
		openFacilitiesCount++;

		double diff = deltaOpen + deltaClose;
		if (diff > 0) {
			improve = true;
			totalDiff += diff;
		}
	}

	return totalDiff;
}

//first add best, then remove best
double LocalSearch::RLS2(Solution * S)
{
	double totalDiff = 0;
	int openFacilitiesCount = S->getOpenFacilityCount();
	int closedFacilitiesCount = S->getClosedFacilityCount();
	int *openFacilities = S->getOpenFacilitiesList();
	int *closedFacilities = S->getClosedFacilitiesList();

	bool improve = true;
	while (improve) {
		improve = false;

		double deltaOpen = -999999999;
		int toBeOpened = -1;
		for (size_t i = 0; i < closedFacilitiesCount; i++)
		{
			double d = S->evaluateFacilityOpening(closedFacilities[i]);
			if (d > deltaOpen) {
				deltaOpen = d;
				toBeOpened = closedFacilities[i];
			}
		}

		S->openFacility(toBeOpened, deltaOpen);
		closedFacilitiesCount--;
		openFacilitiesCount++;

		double deltaClose = -1;
		int toBeClosed = -1;
		for (size_t i = 0; i < openFacilitiesCount; i++)
		{
			double d = S->evaluateFacilityClosing(openFacilities[i]);
			if (d > deltaClose) {
				deltaClose = d;
				toBeClosed = openFacilities[i];
			}
		}

		S->closeFacility(toBeClosed, deltaClose);
		closedFacilitiesCount++;
		openFacilitiesCount--;

		double diff = deltaOpen + deltaClose;
		if (diff > 0) {
			improve = true;
			totalDiff += diff;
		}
	}

	return totalDiff;
}

double LocalSearch::RLS1_and_RLS2(Solution * S)
{
	bool improved = true;
	double totalDiff = 0;

	while (improved) {
		improved = false;
		double diff = LocalSearch::RLS1(S);
		if (diff > 0) {
			improved = true;
			totalDiff += diff;
		}
		diff = LocalSearch::RLS2(S);
		if (diff > 0) {
			improved = true;
			totalDiff += diff;
		}
	}

	return totalDiff;
}
