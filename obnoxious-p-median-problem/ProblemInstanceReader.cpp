#include "pch.h"
#include "ProblemInstanceReader.h"
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

const string INSTANCE_FOLDER_NAME = "OpM_LIB_2016";

ProblemInstanceReader::ProblemInstanceReader(string fileName)
{
	ifstream inFile;
	string line, delimiter;
	int n, m, p;

	inFile.open(INSTANCE_FOLDER_NAME + string("\\") + fileName);

	if (inFile.fail()) {
		cerr << "unable to open file << fileName << for reading" << endl;
		exit(1);
	}

	//read first line
	getline(inFile, line);

	delimiter = "=";

	//read n
	getline(inFile, line);
	line = line.substr(line.find(delimiter) + 1, line.length());
	n = stoi(line);

	//read m
	getline(inFile, line);
	line = line.substr(line.find(delimiter) + 1, line.length());
	m = stoi(line);

	//read p
	getline(inFile, line);
	line = line.substr(line.find(delimiter) + 1, line.length());
	p = stoi(line);

	//read until table prefix
	while (getline(inFile, line))
	{
		if (line.compare(0, 5, "table") == 0) {
			break;
		}
	}

	//concat all lines into data string
	string data = "";
	while (getline(inFile, line)) {
		data += line;
	}

	//tokenize data
	char * chArr = new char[data.length() + 1];
	strcpy_s(chArr, data.length() + 1, data.c_str());
	char separators[] = "{} ,\t\n";
	char *token, *next_token1;
	token = strtok_s(chArr, separators, &next_token1);

	double** DM = new double*[n];
	for (size_t i = 0; i < n; i++)
	{
		DM[i] = new double[m];
	}

	int clientCounter = 0;
	int facilityCounter = 0;

	//fill Distance Matrix(DM) 
	while ((token != NULL))
	{
		if (token != NULL)
		{
			if (clientCounter == n) {
				clientCounter = 0;
				facilityCounter++;
			}

			double d = stod(token);
			DM[clientCounter][facilityCounter] = d;
			clientCounter++;

			token = strtok_s(NULL, separators, &next_token1);
		}
	}

	//create Problem object
	problem = new Problem(n, m, p, DM);
}


ProblemInstanceReader::~ProblemInstanceReader()
{
	delete problem;
}

Problem * ProblemInstanceReader::GetProblem() const
{
	return problem;
}


