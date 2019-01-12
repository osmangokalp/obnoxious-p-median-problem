#pragma once
#include "Problem.h"
#include "Solution.h"

class IteratedGreedy
{
public:
	IteratedGreedy(Problem *problem);
	~IteratedGreedy();
	Solution * solve(Solution * sol, int MAX_ITER, double alpha, double dPercent, int GREEDY_SELECTION_MODE, int ALPHA_MODE, int LS_MODE, int D_MODE);
	void setPrintInfo(bool b);

private:
	int d; //destruction size
	double alpha; //for construction heuristic
	Problem *problem;
	bool printInfo;
	Solution * SStar;
	Solution * S;

	void applyConstruction(Solution *SPrime, int d, int GREEDY_SELECTION_MODE) const;
	void applyDestruction(Solution *SPrime, int d) const;
	int * selectRandomFromOpenFacilities(Solution *sol, int d) const;
};

