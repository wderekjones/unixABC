#ifndef ABCALGO_H

#define ABCALGO_H



#define MAXIMIZATION_PROBLEM 0
#define MINIMIZATION_PROBLEM 1
#define CONSTRAINTS_FAILED 0
#define CONSTRAINTS_PASSED 1


#define MAX_DIM_COUNT 10
#define MAX_COLONY_SIZE 100
#define MAX_FOOD_NUMBER MAX_COLONY_SIZE / 2

#define RANDOMIZE r = ((double)rand() / ((double)(RAND_MAX) + (double)(1)) )

#include <cmath>
#include <vector>

using namespace std;



class foodSource 
{
private: 
	vector<double> dim;
	double objFxValue;
	double fitness;
	double trials;
	double prob;

public:
	foodSource()
	{
		dim.push_back(0);
		objFxValue = 0;
		fitness = 0;
		fitness = 0;
		trials = 0;
		prob = 0;
	}
	vector<double> getDim()
	{
		return dim;
	}

	void setDim(int index, double val)
	{
		if (index <= dim.size())
		{
			dim[index] = val;
		}
	}

	void initDims(int size)
	{
		for (int i = 0; i < size; i++)
		{
			dim.push_back(0);
		}
	}
	double getObjFxValue()
	{
		return objFxValue;
	}

	void setObjFxValue(double objxV)
	{
		objFxValue = objxV;
	}

	double getFitness()
	{
		return fitness;
	}

	void setFitness(double f)
	{
		fitness = f;
	}

	double getTrials()
	{
		return trials;
	}

	void setTrials(double t)
	{
		trials = t;
	}

	double getProb()
	{
		return prob;
	}

	void setProb(double p)
	{
		prob = p;
	}
};


class ABC
{
private:

	int dimCount, colonySize, maxIterations, trialsLimit;

	vector<double> lB;
	vector<double> uB;
	int runTimes;


	vector<foodSource> foods;
	foodSource sol;
	foodSource bestSolution;

	double r;
	int runCount;
	int pCount;
	int probMaxMin;

	int vFOOD_NUMBER;

public:


	double rastriginsBenchmark(foodSource f) {
		double rastValue = 0;
		int dimIndex;
		double u = 0;
		for (dimIndex = 0; dimIndex < dimCount; dimIndex++)
		{
			if (u < INFINITY) {
				u = 10 + (f.getDim()[dimIndex] * f.getDim()[dimIndex] - 10 * cos(2*M_PI*f.getDim()[dimIndex]));
			}
			rastValue += u;
		}

		rastValue = u;
		return rastValue;
	}


	int checkConstraints(foodSource f) {
		int validity = CONSTRAINTS_PASSED;
		return validity;
	}


	void perRunOutput(int r, double result)
	{

		if (r == -1) {
			printf("\nMEAN VALUE: ");
		}
		else {
			printf("\nRUN: ");
			printf("%d", r + 1);
		}
		printf("\n\tCONVERGENCE = ");
		printf("%e", result);
	}


	void calcBounds() {
		int dimIndex;
		for (dimIndex = 0; dimIndex < dimCount; dimIndex++) {
			lB.push_back(-5.12);
			uB.push_back(5.12);
		}

	}

	void initProg() {
		calcBounds();
		for (int i = 0; i < MAX_FOOD_NUMBER; i++)
		{
			foods.push_back(sol);
			foods[i].initDims(dimCount);
		}


	}

	void initABCParameters( int dCt, int cSize, int maxI, int tL, int rT, int nFoods ) {
		dimCount = dCt;
		colonySize = cSize;
		maxIterations = maxI;
		trialsLimit = tL;
		runTimes = rT;

		vFOOD_NUMBER = nFoods;
	}


	void randomizeSeedTimer()
	{
		time_t seconds;
		time(&seconds);
		srand((unsigned int) seconds);
	}
	double calcFitness(double value)
	{
		double fitnessValue = 0;
		if (probMaxMin == 0)
		{
			// maximization problem
			if (value >= 0) {
				fitnessValue = 1 + (value);
			}
			else {
				fitnessValue = 1 / (1 + fabs(value));
			}


		}
		else {
			// minimization problem
			if (value >= 0) {
				fitnessValue = 1 / (value + 1);
			}
			else {
				fitnessValue = 1 + fabs(value);
			}
		}

		return fitnessValue;

	}


	void memorizeBestFoodSource()
	{
		int count;
		for (count = 0; count < vFOOD_NUMBER; count++)
		{
			if (foods[count].getObjFxValue() < bestSolution.getObjFxValue())
			{
				bestSolution = foods[count];
			}

		}

	}


	// initialize a single food source
	void initFood(int index)
	{
		int dimIndex, constraintsPassed = 0;


		while (!constraintsPassed) {

			// generate values of l1, theta, and calc l2 = f(l1, theta)
			for (dimIndex = 0; dimIndex < dimCount; dimIndex++)
			{
				RANDOMIZE;
				foods[index].setDim(dimIndex, (lB[dimIndex] + r * (uB[dimIndex] - lB[dimIndex])));
			}
			// constraints check
			
			if (index <= foods.size())
			{
				constraintsPassed = checkConstraints(foods[index]);
			}
		}

		foods[index].setObjFxValue(rastriginsBenchmark(foods[index]));
	
		foods[index].setFitness(calcFitness(foods[index].getObjFxValue()));
	
		foods[index].setTrials(0);
		
		foods[index].setProb(0);
	}


	void initFoodSources() {
		int foodIndex;
		for (foodIndex = 0; foodIndex < vFOOD_NUMBER; foodIndex++) {
			initFood(foodIndex);
		}
		bestSolution = foods[0];
	}


	void sendEmployees() {
		int foodIndex, paramToChange, neighbour, constraintsPassed;
		for (foodIndex = 0; foodIndex < vFOOD_NUMBER; foodIndex++) {
			// random determine parameter to be changed
			RANDOMIZE;
			paramToChange = (int)(r * (dimCount));
			// random determine solution to produce a mutant solution of the solution [foodIndex]
			// also randomly selected neighbour must not be the [foodIndex] solution itself
			RANDOMIZE;
			neighbour = (int)(r * vFOOD_NUMBER);
			while (neighbour == foodIndex) {
				RANDOMIZE;
				neighbour = (int)(r * vFOOD_NUMBER);
			}

			// mutate and check gnerated parameter's boundaries 

			sol = foods[foodIndex]; constraintsPassed = 0;
			while (!constraintsPassed) {
				RANDOMIZE;
				sol.setDim(paramToChange,foods[foodIndex].getDim()[paramToChange] + (foods[foodIndex].getDim()[paramToChange] - foods[neighbour].getDim()[paramToChange]) * (r - 0.5) * 2);
				if (sol.getDim()[paramToChange] < lB[paramToChange]) {
					sol.setDim(paramToChange,lB[paramToChange]);
				}
				else if (sol.getDim()[paramToChange] > uB[paramToChange]) {
					sol.setDim(paramToChange,uB[paramToChange]);
				}
				constraintsPassed = checkConstraints(sol);

			}

			// calc objective function value and fitness for the mutation
			sol.setObjFxValue(rastriginsBenchmark(sol));
			sol.setFitness(calcFitness(sol.getObjFxValue()));

			// greedily check if mutation is better than original

			if (sol.getFitness() > foods[foodIndex].getFitness()) {
				// if mutation is better,

				foods[foodIndex] = sol; // replace original with mutation
				foods[foodIndex].setTrials(0); // reset trials count
			}
			else {
				// failure, so increment trials count
				foods[foodIndex].setTrials(foods[foodIndex].getTrials()+1);
			}


		}


	}



	// calcualte probability for onlooker selection
	void calcProbabilities() {
		int foodIndex;
		double sumFitness = 0;

		// find sum fitness
		for (foodIndex = 0; foodIndex < vFOOD_NUMBER; foodIndex++) {
			sumFitness += foods[foodIndex].getFitness();
		}
		// calc probabilities using: prob_i = fit_i / sum (fit_i)
		for (foodIndex = 0; foodIndex < vFOOD_NUMBER; foodIndex++) {
			foods[foodIndex].setProb(0.9 * (foods[foodIndex].getFitness() / sumFitness) + 0.1);
		}
	}


	// onlookers phase

	void sendOnlookers() {
		int foodIndex, onlookerIndex, paramToChange, neighbour, constraintsPassed;
		foodIndex = 0;
		onlookerIndex = 0;
		while (onlookerIndex < vFOOD_NUMBER) {
			RANDOMIZE;
			if (r < foods[foodIndex].getProb()) {
				onlookerIndex++;

				RANDOMIZE;
				paramToChange = (int)(r * (dimCount));

				RANDOMIZE;
				neighbour = (int)(r * vFOOD_NUMBER);
				while (neighbour == foodIndex) {
					RANDOMIZE;
					neighbour = (int)(r * vFOOD_NUMBER);
				}

				sol = foods[foodIndex]; constraintsPassed = 0;
				while (!constraintsPassed) {
					RANDOMIZE;
					sol.setDim(paramToChange,foods[foodIndex].getDim()[paramToChange] + (foods[foodIndex].getDim()[paramToChange] - foods[neighbour].getDim()[paramToChange]) * (r - 0.5) * 2);
					if (sol.getDim()[paramToChange] < lB[paramToChange]) {
						sol.setDim(paramToChange,lB[paramToChange]);
					}
					else if (sol.getDim()[paramToChange] > uB[paramToChange]) {
						sol.setDim(paramToChange,uB[paramToChange]);
					}
					constraintsPassed = checkConstraints(sol);
				}

				sol.setObjFxValue(rastriginsBenchmark(sol));
				sol.setFitness(calcFitness(sol.getObjFxValue()));

				if (sol.getFitness() > foods[foodIndex].getFitness()) {
					foods[foodIndex] = sol;
					foods[foodIndex].setTrials(0);
				}
				else {
					foods[foodIndex].setTrials(foods[foodIndex].getTrials() +1 );
				}

			}
			foodIndex++;
			if (foodIndex >= vFOOD_NUMBER - 1) {
				foodIndex = 0;
			}

		}

	}



	void sendScouts() {
		int maxTrialsIndex, foodIndex;
		maxTrialsIndex = 0;
		for (foodIndex = 0; foodIndex <vFOOD_NUMBER; foodIndex++) {
			if (foods[foodIndex].getTrials() > foods[maxTrialsIndex].getTrials()) {
				maxTrialsIndex = foodIndex;
			}
		}

		if (foods[maxTrialsIndex].getTrials() > trialsLimit) {
			initFood(maxTrialsIndex);
		}
	}


	foodSource runABC(int dumpData)
	{
		ABC::initFoodSources();
		ABC::memorizeBestFoodSource();
		int iteration;

		for (iteration = 0; iteration < maxIterations; iteration++)
		{
			ABC::sendEmployees();
			ABC::calcProbabilities();
			ABC::sendOnlookers(); 
			//ABC::memorizeBestFoodSource();
			ABC::sendScouts();
			ABC::memorizeBestFoodSource();

		}

		runCount++;
		return bestSolution;
	}

};

#endif

