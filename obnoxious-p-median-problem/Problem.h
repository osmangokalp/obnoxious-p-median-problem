#pragma once
class Problem
{
public:
	Problem(int n, int m, int p, double **DM);
	~Problem();
	int getN() const;
	int getM() const; 
	int getP() const;
	double** getDM() const;

private:
	int n; //number of clients
	int m; //number of facilities
	int p; //number of facilities to be selected
	double **DM;
};

