#include <stdio.h>
#include <stdlib.h>
#include "functions.h"
#include "Crossovermutation.h"

int main () {

	// FILE if needed to plot convergence
	// FILE * fip = NULL;
	// fip = fopen("../data/file.csv", "w");

    printf("\nStarting execution\n");
    printf("Population size is: %d individuals \n", MAX_POPULATION);
    printf("Execution with crossover probability: %f \n", CROSSOVER_PROBABILITY);
    printf("Crossover method: %i\n", CROSSOVER_OPTION);
    printf("Execution with mutation probability: %f \n", MUTATION_PROBABILITY);
    printf("Mutation method: BitFlipMutation\n" );
    printf("\n\n");

    double Colleced_data[NUM_YEARS] = {15329.,14177.,13031.,9762.,11271.,8688.,7571.,6983.,4778.,2067.,1586.,793.};

    // Initiallizing random seed
    random_seed(); 

    // Initiallizing population
    individual *actual_population;
    actual_population = (individual*)malloc(MAX_POPULATION * sizeof(individual));
    if (actual_population == NULL){
        printf("ERROR: not allocated in memory.\n");
    }
    for (int i = 0 ; i < MAX_POPULATION ; i++ ) {
		actual_population[i].parameters = Initial_population();	
    }
    
    // Initializing needed variables
    int count = 0;
    int same_count = 0;
    int exit_while = 0;
    binary_Parameters absolute_best_parameters;
    double *xt = (double*) malloc(NUM_YEARS*sizeof(double));
    double absolute_best_fitness = DBL_MAX;

    // Intitilizing iterative procedure
    while ((count < MAX_ITER) & (exit_while == 0)){
        
        double best_fitness = DBL_MAX;
    	binary_Parameters best_individual;
        // Fitness of actual population and actual best fitted 
        for (int i = 0 ; i < MAX_POPULATION ; i++){
            ODE_Parameters double_parameteters = to_double(actual_population[i].parameters);
            Generate_EDO_Prediction(xt, Colleced_data[0], NUM_YEARS, &double_parameteters);
            actual_population[i].fitness = quadraticerror(Colleced_data, xt, NUM_YEARS);
            if (actual_population[i].fitness < best_fitness){
            	best_fitness = actual_population[i].fitness;
            	best_individual = actual_population[i].parameters;
            }
        }
        
        // Best absolute fitted individual in all iterations
        if (best_fitness < absolute_best_fitness){
			absolute_best_fitness = best_fitness;
			absolute_best_parameters = best_individual;
			same_count = 0;
		}
		
		// Count how many times has been best absolute fitted
		else{
			same_count++;
		}
        
        // Ending condition
		if (same_count > MAX_REPEAT_ITER){
			exit_while = 1;
		}

        printf("Iteration: %i,\t fittest: %9.2f,\t absolute fittest: %9.2f\n", count, best_fitness, absolute_best_fitness);
        //fprintf(fip, "%i;%f;%f\n", count, best_fitness, absolute_best_fitness);

		// SELECTION - TOURNAMENT SELECTION
        actual_population = TournamentSelection(actual_population, TOURNAMENT_INDIVIDUALS, MAX_POPULATION, TOURNAMENT_PROBABILITY);
		
		// Parents from selected population
        for (int i = 0 ; i < MAX_POPULATION ; i += 2){
            binary_Parameters parent1 = actual_population[i].parameters;
            binary_Parameters parent2 = actual_population[i+1].parameters;
            binary_Parameters child1;
            binary_Parameters child2;

            // CROSSOVER -> 3 different options of crossover
            if (rand_uniform_value() < CROSSOVER_PROBABILITY){
                switch (CROSSOVER_OPTION)
                {
                case 1:
                    OnePointCrossover(parent1.phi,    parent2.phi,    &child1.phi,    &child2.phi,    34);
                    OnePointCrossover(parent1.lambda, parent2.lambda, &child1.lambda, &child2.lambda, 25);
                    OnePointCrossover(parent1.mu,     parent2.mu,     &child1.mu,     &child2.mu,     25);
                    OnePointCrossover(parent1.sigma,  parent2.sigma,  &child1.sigma,  &child2.sigma,  17);
                    OnePointCrossover(parent1.delta,  parent2.delta,  &child1.delta,  &child2.delta,  15);
                    break;
                
                case 2:
                    TwoPointCrossover(parent1.phi,    parent2.phi,    &child1.phi,    &child2.phi,    34);
                    TwoPointCrossover(parent1.lambda, parent2.lambda, &child1.lambda, &child2.lambda, 25);
                    TwoPointCrossover(parent1.mu,     parent2.mu,     &child1.mu,     &child2.mu,     25);
                    TwoPointCrossover(parent1.sigma,  parent2.sigma,  &child1.sigma,  &child2.sigma,  17);
                    TwoPointCrossover(parent1.delta,  parent2.delta,  &child1.delta,  &child2.delta,  15);
                    break;
                
                case 3: 
                    UniformCrossover(parent1.phi,    parent2.phi,    &child1.phi,    &child2.phi,    PROB_UNI, 34);
                    UniformCrossover(parent1.lambda, parent2.lambda, &child1.lambda, &child2.lambda, PROB_UNI, 25);
                    UniformCrossover(parent1.mu,     parent2.mu,     &child1.mu,     &child2.mu,     PROB_UNI, 25);
                    UniformCrossover(parent1.sigma,  parent2.sigma,  &child1.sigma,  &child2.sigma,  PROB_UNI, 17);
                    UniformCrossover(parent1.delta,  parent2.delta,  &child1.delta,  &child2.delta,  PROB_UNI, 15);
                    break;
                }
            }
			
			// If not crossover childrens = parents
            else{
                child1 = parent1;
                child2 = parent2;
            }

            // MUTATION - BIT FLIP MUTATION
            BitFlipMutation(&child1.phi   , MUTATION_PROBABILITY, 34);
            BitFlipMutation(&child1.lambda, MUTATION_PROBABILITY, 25);
            BitFlipMutation(&child1.mu    , MUTATION_PROBABILITY, 25);
            BitFlipMutation(&child1.sigma , MUTATION_PROBABILITY, 17);
            BitFlipMutation(&child1.delta , MUTATION_PROBABILITY, 15);

            BitFlipMutation(&child2.phi   , MUTATION_PROBABILITY, 34);
            BitFlipMutation(&child2.lambda, MUTATION_PROBABILITY, 25);
            BitFlipMutation(&child2.mu    , MUTATION_PROBABILITY, 25);
            BitFlipMutation(&child2.sigma , MUTATION_PROBABILITY, 17);
            BitFlipMutation(&child2.delta , MUTATION_PROBABILITY, 15);

            // ADDITION TO NEW GENERATION
            actual_population[i].parameters = child1;
            actual_population[i+1].parameters = child2;
        }
        count++;
    }
    
    // Convert obtained solution to double
    ODE_Parameters best_parameters = to_double(absolute_best_parameters);

    printf("Best parameters:\n");
    printf("Beta:   %1.15f\n", BETA);
    printf("Phi:    %3.8f\n", best_parameters.phi);
    printf("Lambda: %4.4f\n", best_parameters.lambda);
    printf("Mu:     %2.6f\n", best_parameters.mu);
    printf("Sigma:  %4.2f\n", best_parameters.sigma);
    printf("Delta:  %5.1f\n", best_parameters.delta);
    printf("With quadratic error: %f\n", absolute_best_fitness);

	//fclose(fip);
	free(actual_population);
    return 0;
}














