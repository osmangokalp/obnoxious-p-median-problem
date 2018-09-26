#include "pch.h"
#include "Util.h"
#include <cstdlib>

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
