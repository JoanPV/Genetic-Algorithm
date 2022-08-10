#include <math.h>
#include <values.h>
#include "RKF78.c"
#include "random.h"

// Fixed parameters
#define THETA 1000
#define BETA 0.000024382635446

// Runge-kutta parameters
#define HMAX 1.0
#define HMIN 1.e-6
#define RKTOL 1.e-8

#define MAX_POPULATION 20000        // size of population
#define MAX_ITER 10000              // maximum of iterations
#define NUM_GEN 6                   // number of genes per individual
#define NUM_YEARS 12                // number of years

#define CROSSOVER_OPTION 1          // Crossover option (1 = one point, 2 = two point, 3 = uniform)
#define CROSSOVER_PROBABILITY 0.95  // Probability of crossover
#define PROB_UNI 0.5                // Probability of uniform crossover

#define MUTATION_PROBABILITY 0.08   // Mutation probability

#define TOURNAMENT_INDIVIDUALS 50   // Tournament selection size
#define TOURNAMENT_PROBABILITY 0.8  // Tournament probability

#define MAX_REPEAT_ITER 200         // Ending condition


typedef struct {
    double phi;
    double beta;
    double lambda;
    double mu;
    double sigma;
    double delta;
} ODE_Parameters;

typedef struct {
    unsigned long phi;
    unsigned long lambda;
    unsigned long mu;
    unsigned long sigma;
    unsigned long delta;
} binary_Parameters;

typedef struct{
    binary_Parameters parameters;
    double fitness;
} individual;

typedef struct{
	int individual;
	double fitness;
} Tournament_individual;

// Definition of PSI
double ElliotSigmoid(double x, double sigma, double delta) {
    return sigma * (x - delta) / (THETA + sigma * fabs(x - delta));
}

double Edir(double x, double mu, double sigma, double delta){
    double res = mu * (THETA + sigma*delta) / (2.*THETA + sigma*delta);
    return res = (res * (1. - x/delta) + x/delta) * ElliotSigmoid(x, sigma, delta);
}

double Psi(double x, double mu, double sigma, double delta){
    double result;
    if (x <= delta){
        result = (1. - Edir(x, mu, sigma, delta)) / (1. - Edir(0., mu, sigma, delta));
    }
    else{
        result = (1. - ElliotSigmoid(x, sigma, delta)) / (1. - Edir(0., mu, sigma, delta));
    }
    return result;
}

void MigrationODE(double t, double x, double *der, void *Params){
    ODE_Parameters *par = (ODE_Parameters *) Params; // Pointer cast to save typing and thinking
    *der = par->phi * x - par->beta*x*x - par->lambda*Psi(x, par->mu, par->sigma, par->delta);
}


int Generate_EDO_Prediction(double *xt, double x0, unsigned short number_of_years, ODE_Parameters *pars){
	/*
		Generates values prediction from the ODE 
	*/
    register unsigned ty;
    xt[0] = x0; // Storing IC x(0)
    for(ty = 1; ty < number_of_years; ty++){
        xt[ty] = 0.0;
    }
    double t = 0.0, err, h = 1.e-3;
    for(ty = 1; ty < number_of_years; ty++){ 
        int status;
        while(t+h < ty) {
            status = RKF78(&t, &x0, &h, &err, HMIN, HMAX, RKTOL, pars, MigrationODE);
            if(status){
                return status;
            }
        } 
        h = ty - t;
        status = RKF78(&t, &x0, &h, &err, HMIN, HMAX, RKTOL, pars, MigrationODE);
        if(status){
            return status;
        }
        xt[ty] = x0;
    }
    return 0;
}


unsigned long random_long (unsigned long max) {
	/*
		Generates random value between 0 and max
	*/
	return (unsigned long)(uniform()*(max-1));
}

void random_seed(){
	/*
		Initialize random seed
	*/
	randomize();
}

double rand_uniform_value(){
	/*
		Gives uniform number between (0,1)
	*/
	return uniform();
}

binary_Parameters Initial_population(){
	/*
		Generates random individual
	*/
    unsigned long phi    = random_long((int)pow(2,34));
    unsigned long lambda = random_long((int)pow(2,25));
    unsigned long mu     = random_long((int)pow(2,25));
    unsigned long sigma  = random_long((int)pow(2,17));
    unsigned long delta  = random_long((int)pow(2,15));

    binary_Parameters parameters = {phi, lambda, mu, sigma, delta};
    return parameters;
}

ODE_Parameters to_double(binary_Parameters p){
	/*
		Converts parameters from unsigned int to double
	*/
	double lambda  = (double)((p.lambda)*3000. / (pow(2, 25)-1)); 	
	double mu      = (double)((p.mu)*20.       / (pow(2, 25)-1)); 	
	double sigma   = (double)((p.sigma)*1000.  / (pow(2, 17)-1)); 
	double delta   = (double)((p.delta)*25000. / (pow(2, 15)-1));
	double phi     = (double)((p.phi)*100.35   / (pow(2, 34)-1)) - 100.;

    ODE_Parameters parameters = {phi, BETA, lambda, mu, sigma, delta};
    return parameters;
}

double quadraticerror(double * x, double * y, int num_years) {
	/*
		Computes the quadratic error between x and y array values.
	*/
    double fitness = 0.0;
    for (int i = 0 ; i < num_years ; i++){
        fitness += (double)((x[i] - y[i]) * (double)(x[i] - y[i]));
    }
    return fitness;
}

int compare_elements (const void *a, const void *b){
	individual *x = (individual *) a;
	individual *y = (individual *) b;
	if (x->fitness < y->fitness) return -1;
	else if (x->fitness > y->fitness) return 1;
	return 0;
}

int tour_fit_comparison (const void *a, const void *b){
	/*
		Compares a and b and returns integer values indicating the bigger.
	*/
	Tournament_individual *x = (Tournament_individual *) a;
	Tournament_individual *y = (Tournament_individual *) b;
	if (x->fitness < y->fitness) return -1;
	else if (x->fitness > y->fitness) return 1;
	return 0;
}

individual * TournamentSelection(individual * list, int tournament_size, int population, double probability){
	/*
		Returns a new population selected from precious generation by Tournament selection.
	*/
	// Initialize new population
    individual * new_generation = (individual *) malloc(population * sizeof(individual));
    if (new_generation == NULL){
        printf("ERROR: not allocated in memory.\n");
    }
    // Generate new population
    for (int i = 0 ; i < population ; i++){
        Tournament_individual * tour_list = (Tournament_individual *)malloc(tournament_size*sizeof(Tournament_individual));
        // Tournament selection of size tournament size
        for (int j = 0 ; j < tournament_size ; j++){
            tour_list[j].individual = random_long(population-1);
            tour_list[j].fitness = list[tour_list[j].individual].fitness;
        }
        // sort tournament individuals by fitness
        qsort(tour_list, tournament_size, sizeof(Tournament_individual), tour_fit_comparison);
        
        int exponent = 0;
        int end_condition = 0;
        // Select fittest individual with certain probability
        do {
        	if (uniform() < probability * pow((1-probability), exponent)){
        		new_generation[i] = list[tour_list[exponent].individual];
        		exponent++;
        		end_condition = 1;
        	}
        } while (end_condition == 0);
        free(tour_list);
    }
    return new_generation;
}























