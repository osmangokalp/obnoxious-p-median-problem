#pragma once
#include"Solution.h"
class LocalSearch
{
public:
	LocalSearch();
	~LocalSearch();

	static double exchange(Solution *S);
	static double RLS1(Solution *S);
	static double RLS2(Solution *S);
	static double removeFreqWorstAddBest(Solution *S);
	static double avgPositionPriorityLS(Solution *S);
	static double compositeLS(Solution *S);
};

