#include "pch.h"
#include "RLS1.h"
#include "Util.h"

RLS1::RLS1()
{
}


RLS1::~RLS1()
{
}

double RLS1::search(Solution * S)
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

		if (deltaOpen + deltaClose > 0) {
			improve = true;
		}
	}

	return totalDiff;
}
