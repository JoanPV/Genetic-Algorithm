
Author: Joan Pareras Velasco
NIU: 1618913
version: 2.0
Last_revision: 07-01-2022

----------------------------------------------------------------------------------

GenAlgorithm
|
|---data					-> To save outputs if needed
|
|---src
|	|---Crossovermutation.h -> Contains functions for crossover and mutation
|	|---functions.h			-> Contains definitions of parameters and functions 
|	|---main.c				-> Main program. -> program to execute
|	|---random.h			-> Contains functions that generates random numbers
|	|---RKF78.c				-> Runge-kutta Ferhberg 7-8 to make ODE predictions
|	|---RKF78.h				-> Runge-kutta Ferhberg 7-8 definitions
|
|---README.txt
|---Genetic_Algorithm.pdf

----------------------------------------------------------------------------------

Instructions to execute:

Change the parameters if needed in functions.h:
	
	...
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
	
Also uncomment the File related lines if wanted the convergence values in a file.
	
Then execute the main.c code as 
	gcc -Wall main.c -o main.exe -lm
	./main.exe

-----------------------------------------------------------------------------------

The program will print:

	iteration: %%%%%, fittest: %%%%%%%%%, absolute fitness: %%%%%%%
	
	fittest is the fittest value found on that iteration
	absolute fittest is the fittest value found in all iterations

















