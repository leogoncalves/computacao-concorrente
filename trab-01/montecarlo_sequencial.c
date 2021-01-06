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

int main(int argc, char *argv[]) {
    srand(time(NULL));
    
    radius = 1;   
    terms = 1000000;
    
    for(int i = 0; i < 1000; i++) {
        montecarlo(terms, radius);
        printf("\n");
    }
    
    return 0;
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

    long double approximate_pi =  (long double) 4 * hits/terms;
    GET_TIME(finish);
    elapsed = finish - start;
    printf("Valor de Pi (aproximado): %1.16Lf \n", approximate_pi);
    printf("Valor de PI (M_PI de math.h): %1.16f \n", M_PI);    
    printf("Tempo de execução: %lf segundos \n", elapsed);
    printf("\n");
    
}
