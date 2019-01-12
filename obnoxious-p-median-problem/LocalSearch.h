#pragma once
#include"Solution.h"
class LocalSearch
{
public:
	LocalSearch();
	~LocalSearch();

	static double search(Solution *S, int mode);

private:
	static double RLS1(Solution *S);
	static double RLS2(Solution *S);
	static double compositeLS(Solution *S);
};

