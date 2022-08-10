#define THETA 1000
#define BETA 0.000024382635446

#define HMAX 1.0
#define HMIN 1.e-6
#define RKTOL 1.e-8

#define MAX_POPULATION 10000
#define MAX_ITER 10000
#define NUM_GEN 6
#define NUM_YEARS 12

#define CROSSOVER_OPTION 1
#define CROSSOVER_PROBABILITY 0.95
#define PROB_UNI 0.3
#define MUTATION_PROBABILITY 0.005

#define TOURNAMENT_INDIVIDUALS 30

#define FITNESS_END 1000000


typedef struct ODE_Parameters {
    double phi;
    double beta;
    double lambda;
    double mu;
    double sigma;
    double delta;
} ODE_Parameters;

typedef struct binary_Parameters {
    unsigned long phi;
    unsigned long lambda;
    unsigned long mu;
    unsigned long sigma;
    unsigned long delta;
} binary_Parameters;

typedef struct{
    binary_Parameters parameters;
    double fitness;
    int order;
} individual;