/* 
 * Author: Thomas Els
 * 
 * This programm generates long time series (iter = 100'000) of single short 1D random walks(as example: steps = 2, 4, 8 or 12)  
 * , continuous and restricted step width, with help of the rand() function and analyses some statistical data.
 * Data is also outputed into the plots folder.
 */
 
#include <iostream>
#include <ctime>
#include <cmath>
#include <stdlib.h>
#include <fstream>
#include <sstream>

using namespace std;

struct eingabe
{
	double p;
	int steps;
	double s_min;
	double s_max;
	double iter;
	bool cont_step;	
};
struct statistical_data
{
	double mean;
	double mean_square;
	double variance;
	double var_x;
};

double rnd_walk(eingabe *input);		// funktionen deklaration
statistical_data *x_binomial(eingabe *input);
void x_binomial_out(statistical_data *pos_x, eingabe *input);
void distribution(eingabe *input, double scale);
double fRand(double fMin, double fMax);
int get_int(char *message);
bool get_bool(char *message);


int main()
{				
	int steps = 12;				//variablen deklaration
	int iter = 100000;
	double p = 0.5;
	double s_min = -1;
	double s_max = 1;
	double scale = 0.1;
	bool cont_step;
	eingabe input;
	eingabe *pinput;
	
	time_t t;						// rnd aufruf
    time(&t);
    srand((unsigned int) t);
    					
    input.steps = steps;		//input struct def und an pointer übergeben
    input.p = p;
    input.s_min = s_min;
    input.s_max = s_max;
    input.iter = iter;
    input.cont_step = true;		//continuous step width = true, restricted step width = false
    pinput = &input;
    
    steps = get_int("Enter a number for steps of random walk: ");
    cont_step = get_bool("Continuous step width?: ");
    
	//functions aufruf und ausgabe
	input.steps = steps;
	input.cont_step = cont_step;
	pinput = &input;
	x_binomial_out(x_binomial(pinput), pinput);
	distribution(pinput, scale);
	
		
return 0;
}


double rnd_walk(eingabe *input)		// hauptfunktion
{
	double pos_x = 0.0;
	
	if (input->cont_step)
	{	
		for (int i = 0; i < input->steps; i++)
		{
			pos_x += fRand(input->s_min,input->s_max);
		}
	}
	else
	{
		for (int i = 0; i < input->steps; i++)
		{
			if (input->p > fRand(0,1))
			{
				pos_x += input->s_max;
			}
			else
			{
				pos_x += input->s_min;
			}
		}
	}

return pos_x;
}

statistical_data *x_binomial(eingabe *input) //statistische auswertung zur position x
{
	static statistical_data pos_x;
	pos_x.mean = 0.0;
	pos_x.mean_square = 0.0;
	pos_x.var_x = 0.0;
	pos_x.variance = 0.0;
	
	for (int i = 0; i < input->iter; i++)
	{
		pos_x.var_x = rnd_walk(input);
		pos_x.mean += pos_x.var_x;						// + da unabhängige zufallsversuche
		pos_x.mean_square += pos_x.var_x * pos_x.var_x;
	}
	
	pos_x.mean = pos_x.mean / (double)input->iter;
	pos_x.mean_square = pos_x.mean_square / (double)input->iter;
	pos_x.variance += pos_x.mean_square - pos_x.mean * pos_x.mean;

	return &pos_x;
}

void x_binomial_out(statistical_data *pos_x, eingabe *input)
{
	cout << "n = " << input->steps << " -- Mean X_n: " << pos_x->mean << endl;
	cout << "n = " << input->steps << " -- Mean Square X_n: " << pos_x->mean_square << endl;
	cout << "n = " << input->steps << " --  Var X_n: " << pos_x->variance << endl << endl;
}

void distribution(eingabe *input, double scale)
{
	const int SCALING = ( (input->steps) * (int)(1.0/scale) * 2 ) + 1;
	double p_x[SCALING] = {0};
	const double INPUT_MAX = 99999;
	
	if(input->steps < INPUT_MAX)
	{	
		char a[sizeof input->steps] = "";
		sprintf(a, "%05d", input->steps);
		char f_out[] = { 'p', 'l', 'o', 't', 's', '/', 'n', '_', a[0], a[1], a[2], a[3], a[4], '_', 'c', '_', (input->cont_step ? 'y' : 'n'), '.', 'd', 'a', 't','\0' };
	
		fstream output;						//output file
		output.open(f_out, ios::out);
	
		for (int i = 0; i < input->iter; i++)		// loop for making p(x)
		{
			p_x[(int)((rint(rnd_walk(input) * (int)(1.0/scale))) + (input->steps) * (int)(1.0/scale))] += 1;	// + steps because of negative x positions
		}
	
		output << "# x" << "		p(x)" << "		( scale=" << scale << ", continuous=" << input->cont_step << " )" << endl << endl;
		
		for (int i = 0; i < SCALING; i++)		//do output p(x)
		{
			output << (i * scale - (input->steps)) << "	" << p_x[i] / (double)input->iter << "	" << endl;
		}
		
		cout << "distribution for n=" << input->steps << " , scale=" << scale << " -> outputfile: " << f_out << endl << endl;
		output.close();
	}
	else  				// fehler output für n>99999 and 0<p<1
	{
		cout << endl << " -- > distribution for n=" << input->steps << " cannot be put into a file <--" << endl;
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

		if(ss >> out && !(ss >> in) && out > 0 && out < 99999)
		{
			cout << endl; 
			return out;
		}
		//(ss >> out) checks for valid conversion to integer
		//!(ss >> in) checks for unconverted input and rejects it

		cin.clear(); //in case of a cin error, like eof() -- prevent infinite loop
		cerr << "\nInvalid input. Please try again.\n"; //if you get here, it's an error
	}
}

bool get_bool(char *message)
{
	char input;
	bool out;
	string in;

	while(true) {

		cout << message;
		getline(cin,in);
		stringstream ss(in); //covert input to a stream for conversion to int
		ss >> input;
		cout << endl;
		
		switch(input) {
			case 'y':
				return out = true;
				break;
			case 'n':
				return out = false;
				break;
			default:
				cin.clear(); //in case of a cin error, like eof() -- prevent infinite loop
				cerr << "\nInvalid input. Please try again.\n"; //if you get here, it's an error
		}
	}
}
