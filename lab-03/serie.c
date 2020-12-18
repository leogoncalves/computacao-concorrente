#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <time.h>
#include <math.h>
#include "macros/timer.h"

long long int terms;
int nthreads;

void* sum_alternated_series(void* arg);
void task(long long int terms, int nthreads);
void test_performance(int nthreads);

int main(int argc, char *argv[]) {

    if(argc < 2) {
        printf("Faltam argumentos. Informe: \n - Se deseja rodar os casos de teste (0 para não executar, 1 para executar) \n - A quantidade de termos na serie \n - Quantidades de threads usadas \n");
        exit(-1);
    }

    int RUN_TESTS = atoi(argv[1]);

    if(RUN_TESTS == 1) {
        test_performance(1);
        test_performance(2);
        test_performance(4);
    } else {
        terms = atoll(argv[2]);
        nthreads = atoi(argv[3]);
        task(terms, nthreads);
    }

    
    
    
    return 0;
}

void test_performance(int num_threads) {    
    int i, block_size;
    
    terms = pow(10, 3);
    nthreads = num_threads;
    block_size = terms/nthreads;     
    printf("| terms = %lld | nthreads=%d |Tam_Bloco = %d \n\n", terms, nthreads, block_size);
    for(i = 0; i < 5; i++) {
        task(terms, nthreads);
        printf("\n");
    }
    
    terms = pow(10, 5);
    nthreads = num_threads;
    block_size = terms/nthreads;     
    printf("| terms = %lld | nthreads=%d |Tam_Bloco = %d \n\n", terms, nthreads, block_size);
    nthreads = num_threads;
    for(i = 0; i < 5; i++) {
        task(terms, nthreads);
        printf("\n");
    }

    terms = pow(10, 7);
    nthreads = num_threads;
    block_size = terms/nthreads;     
    printf("| terms = %lld | nthreads=%d |Tam_Bloco = %d \n\n", terms, nthreads, block_size);
    nthreads = num_threads;
    for(i = 0; i < 5; i++) {
        task(terms, nthreads);
        printf("\n");
    }

    terms = pow(10, 9);
    nthreads = num_threads;
    block_size = terms/nthreads;     
    printf("| terms = %lld | nthreads=%d |Tam_Bloco = %d \n\n", terms, nthreads, block_size);
    nthreads = num_threads;
    for(i = 0; i < 5; i++) {
        task(terms, nthreads);
        printf("\n");
    }
    
}

void* sum_alternated_series(void* arg) {
    long int i, ini, end; // inicio e fim de bloco
    long double *sum;; // somador local
    long int id = (long int) arg; // id da thread
    int block_size = terms/nthreads; // tamanho do bloco
    ini = id * block_size;

    sum = (long double*) malloc(sizeof(long double));
    if(sum == NULL) {
        printf("Erro no malloc");
        exit(-1);
    }
    *sum = 0;
    // caso tenha resto na divisao, tratamos
    if(id == nthreads - 1) {
        end = block_size;    
    } else {
        end = ini + block_size;
    }    
    
    for(i = end; i >= ini ; i--) {
        *sum += pow(-1, i) * (1.0/(2*i+1));
    }
    
    pthread_exit((void *) sum);
}

void task(long long int terms, int nthreads) {
    long int i; // variavel de controle de laços
    long double sum_total = 0.0; // soma dos valores retornados de cada thred
    
    long double *sum_in_thread; // valor da soma de uma thread


    double start, finish, elapsed; // tomadores de tempo
    double total_execution_time = 0.0; // soma da main
    
    GET_TIME(start);
    pthread_t *tid = (pthread_t*)malloc(nthreads * sizeof(pthread_t)); //identificado das threads
        
    if(tid == NULL) {
        printf("--ERRO: malloc de tid");
        exit(-1);
    }    
    GET_TIME(finish);
    elapsed = finish - start;
    printf("Criação de identificadores de threads: %lf segundos \n", elapsed);

    total_execution_time += elapsed;
    start = 0.0;
    finish = 0.0;

    GET_TIME(start);
    for(i = 0; i < nthreads; i++) {
        if(pthread_create(tid+i, NULL, sum_alternated_series, (void*) i)){
            printf("--ERRO: pthread_create()\n"); 
            exit(-1);
        }
    }
    GET_TIME(finish);
    elapsed = finish - start;
    printf("Criação de threads: %lf segundos \n", elapsed);
    total_execution_time += elapsed;
    start = 0.0;
    finish = 0.0;

    GET_TIME(start);
    // Aguarda a finalização das threads
    for(i = 0; i < nthreads ; i++) {
        if(pthread_join(*(tid + i), (void**) &sum_in_thread)) {
            printf("Erro: pthread_join");
            exit(-1);
        }
        sum_total += *sum_in_thread;
        free(sum_in_thread);
    }
    GET_TIME(finish);
    elapsed = finish - start;
    printf("Tempo de exeução das threads: %lf segundos \n", elapsed);
    total_execution_time += elapsed;
    start = 0.0;
    finish = 0.0;
    printf("Valor total da soma: %1.16Lf \n", 4 * sum_total);  
    printf("Valor de PI: %1.16f \n", M_PI);    
    long double error = (4 * sum_total) - M_PI;
    printf("Erro estimado: %1.16Lf \n", error);    
    printf("Tempo total de execução: %lf segundos \n", total_execution_time);

    free(tid);
}