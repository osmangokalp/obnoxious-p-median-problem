#pragma once
#include "Solution.h"

class GC2
{
public:
	GC2();
	~GC2();
	static void selectCandidate(Solution *sol, double alpha, int &selected, double &diff);
};

