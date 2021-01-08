#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <time.h>
#include "macros/timer.h"
#include <math.h>

long long int terms;
int nthreads;
int radius;

void* parallel_montecarlo(void* args);
void montecarlo_threads(long long int terms, int nthreads);
void seed_rand(int thread_id, struct drand48_data *buffer);

int main(int argc, char *argv[]) {
    srand(time(NULL));
    
    if(argc < 3) {
        printf("Faltam argumentos. Informe: \n - A quantidade de termos na serie \n - Quantidades de threads usadas \n");
        exit(-1);
    }

    radius = 1;   
    terms = atoi(argv[1]);
    nthreads = atoi(argv[2]);
    
    
    printf("%lld termos, %d threads \n", terms, nthreads);
    montecarlo_threads(terms, nthreads);
   
    return 0;
}

void* parallel_montecarlo(void* arg) {
    struct drand48_data drand_buffer;
    int thread_id = (int) arg;    
    long int i, ini, end;
    double x, y;
    long long int *hits;
    int block = terms/nthreads;
    
    ini = thread_id * block;
    hits = (long long int*) malloc(sizeof(long long int));

    if(hits == NULL) {
        printf("Erro em malloc");
        exit(-1);
    }

    *hits = 0;

    if(thread_id == nthreads - 1) {
        end = terms;    
    } else {
        end = ini + block;
    }

    seed_rand(thread_id, &drand_buffer);

    for(i = ini; i <= end; i++) {
        
        drand48_r(&drand_buffer, &x);
        drand48_r(&drand_buffer, &y);

        if(pow(x,2) + pow(y,2) < pow(radius, 2)) {
            *hits += 1;
        }
    }

    pthread_exit((void *) hits);
}

void montecarlo_threads(long long int terms, int nthreads) {
    long int i;
    long long int total_hits = 0;
    long long int *thread_hits;
    double start, finish, elapsed; 

    GET_TIME(start);
    pthread_t *tid = (pthread_t*)malloc(nthreads * sizeof(pthread_t)); //identificado das threads
        
    if(tid == NULL) {
        printf("--ERRO: malloc de tid");
        exit(-1);
    }    

    for(i = 0; i < nthreads; i++) {
        if(pthread_create(tid+i, NULL, parallel_montecarlo, (void*) i)){
            printf("--ERRO: pthread_create()\n"); 
            exit(-1);
        }
    }
   
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
    double approximate_pi =   (4.0 * total_hits) / terms;
    printf("%Lf %Lf \n", (long double) total_hits, (long double) terms);
    printf("Valor de Pi (aproximado): %1.16f \n", approximate_pi);  
    printf("Valor de PI (M_PI de math.h): %1.16f \n", M_PI);    
    printf("Tempo total de execução das threads: %lf segundos \n", elapsed);

}

void seed_rand(int thread_id, struct drand48_data *buffer) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    srand48_r(tv.tv_sec * thread_id + tv.tv_usec, buffer);
}
