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
void* parallel_montecarlo(void* args);
void simulate(long long int terms, int nthreads);

int main(int argc, char *argv[]) {
    srand(time(NULL));

    radius = 1;
    terms = 24000;
    nthreads = 2;

    // montecarlo(terms, radius);
    simulate(terms, nthreads);
    return 0;
}

void montecarlo(long long int terms, int radius) {
    long long int hits = 0;
    long double x, y;
    double start, finish, elapsed; // tomadores de tempo
    GET_TIME(start);
    for(int i = 0; i < terms; i++) {
        x = (double)rand()/RAND_MAX;
        y = (double)rand()/RAND_MAX;

        if(pow(x,2) + pow(y,2) < pow(radius, 2)) {
            hits++;
        }
    }

    long double approximate_pi =  (4.0 * hits / terms);
    GET_TIME(finish);
    elapsed = finish - start;
    printf("Tempo de execução: %lf segundos \n", elapsed);
    printf("Pi Aproximado: %1.16Lf \nPi(math): %1.16lf \n", approximate_pi, M_PI);
    
}

void* parallel_montecarlo(void* args) {
    long int i, ini, end;
    long double x, y;
    long long int *hits;
    long int id = (long int) args;
    int block = terms/nthreads;
    printf("block size = %d\n", block);
    ini = id * block;
    printf("ini = %ld\n", ini);

    hits = (long long int*) malloc(sizeof(long long int));
    if(hits == NULL) {
        printf("Erro em malloc");
        exit(-1);
    }

    *hits = 0;

    if(id == nthreads - 1) {
        end = terms;    
    } else {
        end = ini + block;
    }

    printf("end = %ld\n", end);

    for(i = ini; i <= end; i++) {
        x = (double)rand()/RAND_MAX;
        y = (double)rand()/RAND_MAX;

        if(pow(x,2) + pow(y,2) < pow(radius,2)) {
            *hits += 1;
        }
    }

    printf("%ld %lld %ld %ld \n", id, *hits, ini, end);

    pthread_exit((void *) hits);

}

void simulate(long long int terms, int nthreads) {
    long int i;
    long long int total_hits = 0;

    long long int *thread_hits;

    double start, finish, elapsed; 
    double total_execution_time = 0.0; 

    GET_TIME(start);
    pthread_t *tid = (pthread_t*)malloc(nthreads * sizeof(pthread_t)); //identificado das threads
        
    if(tid == NULL) {
        printf("--ERRO: malloc de tid");
        exit(-1);
    }    
    GET_TIME(finish);
    elapsed = finish - start;
    // printf("Criação de identificadores de threads: %lf segundos \n", elapsed);

    total_execution_time += elapsed;
    start = 0.0;
    finish = 0.0;

    GET_TIME(start);
    for(i = 0; i < nthreads; i++) {
        if(pthread_create(tid+i, NULL, parallel_montecarlo, (void*) i)){
            printf("--ERRO: pthread_create()\n"); 
            exit(-1);
        }
    }
    GET_TIME(finish);
    elapsed = finish - start;
    // printf("Criação de threads: %lf segundos \n", elapsed);
    total_execution_time += elapsed;
    start = 0.0;
    finish = 0.0;

    GET_TIME(start);
    // Aguarda a finalização das threads
    for(i = 0; i < nthreads ; i++) {
        if(pthread_join(*(tid + i), (void**) &thread_hits)) {
            printf("Erro: pthread_join");
            exit(-1);
        }
        total_hits += *thread_hits;
        free(thread_hits);
    }

    GET_TIME(finish);
    elapsed = finish - start;
    // printf("Tempo de exeução das threads: %lf segundos \n", elapsed);
    total_execution_time += elapsed;
    start = 0.0;
    finish = 0.0;
    printf("%lld %lld \n", total_hits, terms);
    long double approximate_pi =  (long double) total_hits / terms;
    printf("Valor total da soma: %1.16Lf \n", 4*approximate_pi);  
    // printf("Valor de PI: %1.16f \n", M_PI);    
    // printf("Tempo total de execução das threads: %lf segundos \n", elapsed);

}
