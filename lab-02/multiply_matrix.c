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

int main(void) {
    srand(time(NULL));
    int matrix_size = 2;
    int MAX_THREADS = 1;
    int row, nthreads;

    int* matrixA = allocate_space_to_matrix(matrix_size, matrix_size);
    populate_matrix(matrixA, matrix_size, matrix_size);
    print_matrix(matrixA, matrix_size, matrix_size);
    printf("\n");
    
    int* matrixB = allocate_space_to_matrix(matrix_size, matrix_size);
    populate_matrix(matrixB, matrix_size, matrix_size);
    print_matrix(matrixB, matrix_size, matrix_size);
    printf("\n");
    
    int* matrixC = allocate_space_to_matrix(matrix_size, matrix_size);
    print_matrix(matrixC, matrix_size, matrix_size);
    printf("\n");

    
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
    free(args);

    print_matrix(matrixC, matrix_size, matrix_size);

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
