#pragma once
#include <string>
#include "Problem.h"
class ProblemInstanceReader
{
public:
	ProblemInstanceReader(std::string fileName);
	~ProblemInstanceReader();
	Problem* GetProblem() const;

private:
	Problem *problem;
};

