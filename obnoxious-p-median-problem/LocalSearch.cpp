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

double LocalSearch::search(Solution * S, int mode)
{
	switch (mode)
	{
	case 0:
		return 0.0;
		break;
	case 1:
		return RLS1(S);
		break;
	case 2:
		return RLS2(S);
		break;
	case 3:
		return compositeLS(S);
		break;
	default:
		break;
	}
}

//first remove best, then add best
double LocalSearch::RLS1(Solution * S)
{
	int openFacilitiesCount = S->getOpenFacilityCount();
	int closedFacilitiesCount = S->getClosedFacilityCount();
	int *openFacilities = S->getOpenFacilitiesList();
	int *closedFacilities = S->getClosedFacilitiesList();


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

	return diff;
}

//first add best, then remove best
double LocalSearch::RLS2(Solution * S)
{
	int openFacilitiesCount = S->getOpenFacilityCount();
	int closedFacilitiesCount = S->getClosedFacilityCount();
	int *openFacilities = S->getOpenFacilitiesList();
	int *closedFacilities = S->getClosedFacilitiesList();


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

	return diff;
}

double LocalSearch::compositeLS(Solution * S)
{
	bool improved = true;
	double totalDiff = 0;

	while (improved) {
		improved = false;
		double diff;

		diff = LocalSearch::RLS1(S);
		while (diff > 0) {
			improved = true;
			totalDiff += diff;
			diff = LocalSearch::RLS1(S);
		}

		diff = LocalSearch::RLS2(S);
		while (diff > 0) {
			improved = true;
			totalDiff += diff;
			diff = LocalSearch::RLS2(S);
		}
	}

	return totalDiff;
}
