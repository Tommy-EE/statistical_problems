/* 
 * Author: Thomas Els
 * 
 * This programm generates long time series (M = 100'000) of single short measurements(as example: N = 10, 20 or 40) 
 * of equally distributed random numbers (from 0 to 1) with help of the rand() function and
 * analyses the estimated variance and gives an "unbiased" estimator for the variance
 */
 
#include <iostream>
#include <ctime>
#include <cmath>
#include <stdlib.h>
#include <fstream>
#include <sstream>

using namespace std;

double fRand(double fMin, double fMax);
double variance(int N);
void time_series(double* variance_array, int M, int N);
int get_int(char *message);


int main(int argc, char **argv) //main
{
	int N;
	int M = 100000;
	double var_est = 0.0;
	double variance_array[M] = {0.0};
	
   time_t t;		// rnd aufruf
   time(&t);
	srand((unsigned int) t);
	
	cout << "Statistical systematic deviations (Bias) analysis for time series of rand() function" << endl << "--------------------------" << endl << endl;
	
	N = get_int("Enter a number for short measurements: ");
	
	var_est = 0.0;
	time_series(variance_array, M, N);
	
	for(int j = 0; j < M; j++)
	{
		var_est += variance_array[j];
	}
	var_est /= M;
	
	cout << endl << "For N = " << N << ": " << endl;
	cout << " -- variance estimation: " << var_est << "	actual variance: " << 1/12.0*(1-1/(double)N)<< endl;
	
	return 0;
}


double variance(int N)
{
	double random_number[N] = {0};
	double mean = 0.0;
	double var = 0.0;
	
	for(int i = 0; i < N; i++)
	{
		random_number[i] = fRand(0,1);
		mean += random_number[i];
	}
	
	mean /= N;
	
	for(int i = 0; i < N; i++)
	{
		var += (random_number[i] - mean) * (random_number[i] - mean);
	}
	
	var /= N;
	
	return var;
}

inline void time_series(double* variance_array, int M, int N)
{
	for(int i = 0; i < M; i++)
	{
			variance_array[i] = variance(N);
	}
}

double fRand(double fMin, double fMax)				//random funktion
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

int get_int(char *message)
{
	int out;
	string in;

	while(true) {

		cout << message;
		getline(cin,in);
		stringstream ss(in); //covert input to a stream for conversion to int

		if(ss >> out && !(ss >> in) && out > 0) return out;
		//(ss >> out) checks for valid conversion to integer
		//!(ss >> in) checks for unconverted input and rejects it

		cin.clear(); //in case of a cin error, like eof() -- prevent infinite loop
		cerr << "\nInvalid input. Please try again.\n"; //if you get here, it's an error
	}
}
