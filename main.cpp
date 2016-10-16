#include<iostream>



#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <time.h>
#include <math.h>

#include "abcalgo.h"


using namespace std;


/*
void outputToFile(vector<double> vct)
{
	for(size_t i = 0; i < vct.size();++i)
   	{

      		for(size_t j = 0; j < vct[0].size(); j+=2)
      		{
        	 	std::cout << std::scientific << std::setprecision(5)
                  		<< std::setw(13) << vct[i][j]
                  		<< std::fixed << std::setprecision(4)
                  		<< std::setw(7) << vct[i][j+1];
      		}
      	std::cout << std::endl;
	}
}*/

int main(int argC, char* argV[])
{
	ABC a;


	double dCt = 15; // number of dimensions
	double cS = 10; // size of colony (number of bees)
	double mIt = 10000; // maximum number of iterations per run
	double tL = 100; // trials limit (how long to stay at a food source)
	double rT = 10; // run times
	double vF_N = 20; // number of food sources

	a.initABCParameters(dCt, cS, mIt, tL, rT, vF_N);

	clock_t start, end;
	double cpuTimeUsed;

	a.initProg();

	start = clock();
	int runs;


	vector<foodSource> results;
	vector<double> values;
	double optimalObjFxValue = 1;
	
	for (runs = 0; runs < 10; runs++)
	{
		results.push_back(a.runABC(0));
		values.push_back(results[runs].getObjFxValue());
		a.perRunOutput(runs, results[runs].getObjFxValue());
		if (results[runs].getObjFxValue() < optimalObjFxValue)
		{
			optimalObjFxValue = results[runs].getObjFxValue();
		}
	}

	end = clock();

	vector<double> outputs;
	vector<foodSource> testSources;
	int colonyLimit = 10;
	cout<<endl;
	for( int i = 1 ; i < colonyLimit + 1; i++)
	{
		a.initABCParameters(dCt, i , mIt, tL, rT, vF_N);
		testSources.push_back(a.runABC(0));
		outputs.push_back(testSources[i].getObjFxValue());
		cout<<outputs[i]<<endl;
	}

	
	cpuTimeUsed = ((double) (end-start)) / CLOCKS_PER_SEC;
	printf("\n\nTOTAL RUNTIME (secs): ");
	printf("%lf", cpuTimeUsed);
	printf("\nMEAN RUNTIME (secs): ");
	printf("%lf", cpuTimeUsed / 10);
	printf("\nObjective Function Value of Optimal Solution: ");
	printf("%lf", optimalObjFxValue);
	cout << endl;
	
	


	return 0;
}
