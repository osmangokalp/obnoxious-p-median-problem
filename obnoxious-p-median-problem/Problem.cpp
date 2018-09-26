#include "pch.h"
#include "Problem.h"


Problem::Problem(int n, int m, int p, double ** DM)
{
	this->n = n;
	this->m = m;
	this->p = p;
	this->DM = DM;
}

Problem::~Problem()
{
	for (int i = 0; i < n; i++)
	{
		delete[] DM[i];
	}
	delete[] DM;
}

int Problem::getN() const
{
	return n;
}

int Problem::getM() const
{
	return m;
}

int Problem::getP() const
{
	return p;
}

double ** Problem::getDM() const
{
	return DM;
}
