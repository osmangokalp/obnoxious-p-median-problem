#pragma once
#include "Problem.h"
#include "Solution.h"

class IteratedGreedy
{
public:
	IteratedGreedy(Problem *problem);
	~IteratedGreedy();
	Solution * solve(Solution * sol, int MAX_ITER, double alphaMax, double alphaMin, double alphaInc);
	void setPrintInfo(bool b);

private:
	int d; //destruction size
	double alpha; //for construction heuristic
	Problem *problem;
	bool printInfo;
	Solution * SStar;
	Solution * S;

	void applyConstruction1(Solution *SPrime, int d) const;
	void applyDestruction1(Solution *SPrime, int d) const;
	void applyDestruction2(Solution *SPrime, int d) const;
	int * selectRandomFromOpenFacilities(Solution *sol, int d) const;
};

