#include "pch.h"
#include "Util.h"
#include <cstdlib>
#include <string>
#include <ctime>
#include <iostream>

using namespace std;

Util::Util()
{
}


Util::~Util()
{
}

void Util::arrayCopy(int * src, int * dest, int len)
{
	for (size_t i = 0; i < len; i++)
	{
		dest[i] = src[i];
	}
}

double Util::min(double a, double b)
{
	double min = a;
	if (min > b) {
		min = b;
	}
	return min;
}

void Util::shuffle(int * arr, int length)
{
	for (int i = length - 1; i > 0; i--) {
		int index = (rand() % (i + 1));

		int a = arr[index];
		arr[index] = arr[i];
		arr[i] = a;
	}
}

std::string Util::getCurrentDateAndTime() {
	struct tm newtime;
	char am_pm[] = "AM";
	__time64_t long_time;
	char timebuf[26];
	errno_t err;

	// Get time as 64-bit integer.  
	_time64(&long_time);
	// Convert to local time.  
	err = _localtime64_s(&newtime, &long_time);
	if (err)
	{
		printf("Invalid argument to _localtime64_s.");
		exit(1);
	}
	if (newtime.tm_hour > 12)        // Set up extension.   
		strcpy_s(am_pm, sizeof(am_pm), "PM");
	if (newtime.tm_hour > 12)        // Convert from 24-hour   
		newtime.tm_hour -= 12;    // to 12-hour clock.   
	if (newtime.tm_hour == 0)        // Set hour to 12 if midnight.  
		newtime.tm_hour = 12;

	// Convert to an ASCII representation.   
	err = asctime_s(timebuf, 26, &newtime);
	if (err)
	{
		printf("Invalid argument to asctime_s.");
		exit(1);
	}

	string result = timebuf;
	result += am_pm;

	return result;
}

bool Util::validateSolution(Solution * S, double obj, Problem * problem)
{
	bool multipleFacility = false;

	int * openFacilities = S->getOpenFacilitiesList();
	int openFacilityCount = S->getOpenFacilityCount();

	if (openFacilityCount != problem ->getP()) {
		return false;
	}


	int * freq = new int[problem->getM()] {0};
	for (size_t i = 0; i < openFacilityCount; i++)
	{
		if (++freq[openFacilities[i]] > 1) {
			for (size_t j = 0; j < openFacilityCount; j++)
			{
				std::cout << to_string(openFacilities[j]) + ", ";
			}
			std::cout << "\n";
			for (size_t j = 0; j < openFacilityCount; j++)
			{
				std::cout << to_string(freq[j]) + ", ";
			}
			std::cout << "\n" << to_string(freq[openFacilities[i]]);
			delete[] freq;
			return false; //multiple copies of facility
		}
	}
	delete[] freq;

	double total = 0.0;
	for (size_t i = 0; i < problem ->getN(); i++)
	{
		double min = DBL_MAX;
		for (size_t j = 0; j < openFacilityCount; j++)
		{
			if (problem->getDM()[i][openFacilities[j]] < min) {
				min = problem->getDM()[i][openFacilities[j]];
			}
		}
		total += min;
	}

	if (total == obj) {
		return true;
	}
	else {
		return false;
	}
}