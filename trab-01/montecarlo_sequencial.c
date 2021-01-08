#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <time.h>
#include "macros/timer.h"
#include <math.h>

long long int terms;
int nthreads;
int radius;

void montecarlo(long long int terms, int radius);
void simulate(int magnitude);

int main(int argc, char *argv[]) {
    srand(time(NULL));
    
    if(argc < 2) {
        printf("Faltam argumentos. Informe a quantidade de termos na serie\n\n");
        exit(-1);
    }

    radius = 1;   
    terms = atoll(argv[1]);

    printf("Monte Carlo Sequencial com %lld termos \n", terms);
    montecarlo(terms, radius);    

    return 0;
}

void simulate(int magnitude) {
    terms = pow(10, magnitude);
    radius = 1;   
    printf("Monte Carlo Sequencial com %d termos \n", magnitude);
    for(int i = 0; i < 5; i++) {
        montecarlo(terms, radius);
    }
}

void montecarlo(long long int terms, int radius) {
    long long int hits = 0;
    long double x, y;
    double start, finish, elapsed;
    GET_TIME(start);
    for(int i = 0; i < terms; i++) {
        x = (long double)rand()/RAND_MAX;
        y = (long double)rand()/RAND_MAX;
        if(pow(x,2) + pow(y,2) <= pow(radius, 2)) {
            hits++;
        }
    }

    long double approximate_pi =  (long double) 4.0 * (hits*1.0)/terms;
    GET_TIME(finish);
    elapsed = finish - start;
    printf("Valor de Pi (aproximado): %1.16Lf \n", approximate_pi);
    printf("Valor de PI (M_PI de math.h): %1.16f \n", M_PI);    
    printf("Tempo de execução: %lf segundos \n", elapsed);
    printf("\n");
    
}
