#pragma once
#include"Solution.h"
class LocalSearch
{
public:
	LocalSearch();
	~LocalSearch();

	static double LS1(Solution *S);
	static double RLS1(Solution *S);
	static double RLS2(Solution *S);
	static double RLS1_and_RLS2(Solution *S);
};

