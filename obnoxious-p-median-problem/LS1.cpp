#include "pch.h"
#include "LS1.h"
#include "Util.h"
#include <iostream>

LS1::LS1()
{
}

LS1::~LS1()
{
}

double LS1::search(Solution * S)
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
						std::cerr <<"d1 + d2 != diff at facility exchange!";
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
