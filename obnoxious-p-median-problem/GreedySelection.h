#pragma once
#include "Solution.h"

class GreedySelection
{
public:
	GreedySelection();
	~GreedySelection();
	static void select(Solution *sol, double alpha, int &selected, double &diff, int selectMode);
	static Solution * constructSolution(Problem * problem, double alpha, int selectMode);

private:
	static void selectRandomPlusGreedy(Solution *sol, double alpha, int &selected, double &diff);
	static void selectGRASP(Solution *sol, double alpha, int &selected, double &diff);
};

