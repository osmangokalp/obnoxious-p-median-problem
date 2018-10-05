#pragma once
#include "Problem.h"
#include "Solution.h"

class IteratedGreedy
{
public:
	IteratedGreedy(Problem *problem, int d, double alpha);
	~IteratedGreedy();
	Solution * solve(int MAX_ITER);
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
	Solution * constructSolution() const;
};

