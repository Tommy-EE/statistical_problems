/* 
 * Author: Thomas Els
 * 
 * This programm generates long time series (M = 100'000) of single short measurements(N = 10, 20 & 40) 
 * of equally distributed random numbers (0-1) with the help of the rand() function and
 * analyses the estimated variance and gives an "unbiased" estimator for the variance
 */
 
#include <iostream>
#include <ctime>
#include <cmath>
#include <stdlib.h>
#include <fstream>

using namespace std;

double fRand(double fMin, double fMax);
double variance(int N);
void time_series(double* variance_array, int M, int N);


int main(int argc, char **argv) //main
{
	int N = 10;
	int M = 100000;
	double var_est = 0.0;
	double variance_array[M] = {0.0};
	
   time_t t;		// rnd aufruf
   time(&t);
	srand((unsigned int) t);
	
	cout << "Statistical systematic deviations (Bias) analysis for time series of rand() function" << endl << "--------------------------" << endl << endl;
	
	for(N = 10; N <= 40; N += N)
	{
		var_est = 0.0;
		time_series(variance_array, M, N);
	
		for(int j = 0; j < M; j++)
		{
			var_est += variance_array[j];
		}
		var_est /= M;
		cout << "for N=" << N << "	variance estimation: " << var_est << "	actual variance: " << 1/12.0*(1-1/(double)N)<< endl;
	}
	
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
