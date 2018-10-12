#pragma once
#include <string>
class Util
{
public:
	Util();
	~Util();
	static void arrayCopy(int *src, int *dest, int len);
	static double min(double a, double b);
	static void shuffle(int *arr, int length);
	static std::string getCurrentDateAndTime();
};

