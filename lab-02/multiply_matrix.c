#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <time.h>
#include "macros/timer.h"

#define MAX_VALUE 10
#define MIN_VALUE 1

typedef struct {
    int id, size, row;
    int nthreads;
    int* matrixA;
    int* matrixB;
    int* matrixC;
} thread_arguments;

int random_number(int min, int max);

int* allocate_space_to_matrix(int rows, int cols);
void populate_matrix(int* matrix, int rows, int cols);
void* multiply_matrix(void* arg);
void print_matrix(int* matrix, int rows, int cols);
void free_matrix(int *matrix);
int offset(int size, int row, int column);
void multiply(int matrix_size, int MAX_THREADS);

int main(void) {
    srand(time(NULL));
    int matrix_size = 2;
    int MAX_THREADS = 1;
    
    multiply(matrix_size, MAX_THREADS);
    
    return 0;
}

int *allocate_space_to_matrix(int rows, int cols) {
    int *matrix = (int*)malloc(rows * cols * sizeof(int));
    if(matrix == NULL) {
        printf("Erro na alocação de espaço da matrix");
        exit(-1);
    }
    return matrix;
}

void populate_matrix(int* matrix, int rows, int cols) {
    int i,j;
    for(i = 0; i < rows; i++)
        for(j = 0; j < cols; j++)
            *(matrix + i*cols+j) = random_number(MIN_VALUE, MAX_VALUE);
}

void print_matrix(int* matrix, int rows, int cols) {
    int i,j;
    for(i = 0; i < rows; i++){
        for(j = 0; j < cols; j++) {
            printf("%d ", *(matrix + i*rows+j));
        }
        printf("\t\n");
    }
}

int offset(int size, int row, int column) {
    return size * row + column;
}

void* multiply_matrix(void* arg) {
    thread_arguments *args = (thread_arguments*) arg;

    for(int row = args->row; row < args->size; row+= args->nthreads) {
        for(int col = 0; col < args->size; col++) {
            for(int idx = 0; idx < args->size; idx++) {        
                int x = args->matrixA[offset(args->size, row, idx)];
                int y = args->matrixB[offset(args->size, idx, col)];
                int *z = &args->matrixC[offset(args->size, row, col)];
                *z += x * y;
                
            }
        }
    }
    pthread_exit(NULL);
}

int random_number(int min, int max) {
    int random = rand();
    return random % (max - min + 1 ) + min;
}

void free_matrix(int *matrix) {
    free(matrix);
}


void multiply(int matrix_size, int MAX_THREADS) {
    int row, nthreads;
    double start, finish, elapsed;
    double total_execution_time = 0.0;
    int VERBOSE = 1;

    printf("Execução com Dimensão %d e %d threads \n", matrix_size, MAX_THREADS);
    
    GET_TIME(start);
    int* matrixA = allocate_space_to_matrix(matrix_size, matrix_size);
    populate_matrix(matrixA, matrix_size, matrix_size);
    
    if(VERBOSE){
        print_matrix(matrixA, matrix_size, matrix_size);
        printf("\n");
    }
    
    int* matrixB = allocate_space_to_matrix(matrix_size, matrix_size);
    populate_matrix(matrixB, matrix_size, matrix_size);
    
    if(VERBOSE){
        print_matrix(matrixB, matrix_size, matrix_size);
        printf("\n");
    }
    
    
    int* matrixC = allocate_space_to_matrix(matrix_size, matrix_size);

    if(VERBOSE){
        print_matrix(matrixC, matrix_size, matrix_size);
        printf("\n");
    }
    
    GET_TIME(finish);
    
    elapsed = finish - start;
    printf("Alocação e criação de matrizes: %lf segundos \n", elapsed);
    total_execution_time += elapsed;
    start = 0.0;
    finish = 0.0;

    
    GET_TIME(start);
    pthread_t *threads = (pthread_t*) malloc(MAX_THREADS * sizeof(pthread_t)) ;    
    thread_arguments *args = (thread_arguments*) malloc(MAX_THREADS * sizeof(thread_arguments));
    if (args == NULL) {
        printf("--ERRO: malloc()\n"); 
        exit(-1);
    }

    nthreads = 0;
    for(row = 0; row < MAX_THREADS; row++) {        
        args[nthreads].id = nthreads;            
        args[nthreads].nthreads = MAX_THREADS;            
        args[nthreads].size = matrix_size;            
        args[nthreads].row = row;
        args[nthreads].matrixA = matrixA;
        args[nthreads].matrixB = matrixB;
        args[nthreads].matrixC = matrixC;
        nthreads++;
    }
    GET_TIME(finish);
    elapsed = finish - start;
    printf("Alocação de threads e recursos: %lf segundos \n", elapsed);
    
    total_execution_time += elapsed;
    start = 0.0;
    finish = 0.0;

    GET_TIME(start);    
    nthreads = 0;
    for(row = 0; row < MAX_THREADS; row++) {
        if(pthread_create(&threads[nthreads], NULL, multiply_matrix, (void*)(&args[nthreads]))){
            printf("--ERRO: pthread_create()\n"); 
            exit(-1);
        }
        nthreads += 1;
    }

    // Aguarda a finalização das threads
    for(int i = 0; i < MAX_THREADS ; i++) {
        if(pthread_join(threads[i], NULL)) {
            printf("Erro: pthread_join");
            exit(-1);
        }
    }
    GET_TIME(finish);
    elapsed = finish - start;
    printf("Execução da multiplicação de matrizes: %lf segundos \n", elapsed);
    total_execution_time += elapsed;
    start = 0.0;
    finish = 0.0;

    if(VERBOSE){
        print_matrix(matrixC, matrix_size, matrix_size);
        printf("\n");
    }


    GET_TIME(start);
    free(args);
    free(matrixA);
    free(matrixB);
    free(matrixC);
    GET_TIME(finish);
    
    elapsed = finish - start;
    printf("Liberação de recursos: %lf segundos \n", elapsed);
    total_execution_time += elapsed;
    start = 0.0;
    finish = 0.0;

    printf("Tempo total gasto: %lf \n", total_execution_time);
}