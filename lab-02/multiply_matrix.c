#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <time.h>
#include "macros/timer.h"

#define NTHREADS 8
#define MAX_VALUE 10
#define MIN_VALUE 1

typedef struct {
    int thread_id, cols, rows;    
    int* matrixA, matrixB, matrixC;
} thread_arguments;

int random_number(int min, int max);

int* allocate_space_to_matrix(int rows, int cols);
void populate_matrix(int* matrix, int rows, int cols);
void print_matrix(int* matrix, int rows, int cols);
void free_matrix(int *matrix);

int main(void) {
    srand(time(NULL));
    int matrix_size = 4;
    double start, finish, elapsed;
    
    GET_TIME(start);
    int* matrix = allocate_space_to_matrix(matrix_size, matrix_size);
    GET_TIME(finish);
    elapsed = finish - start;
    printf("The code to be timed took %e seconds\n", elapsed);

    GET_TIME(start);
    populate_matrix(matrix, matrix_size, matrix_size);
    GET_TIME(finish);
    elapsed = finish - start;
    printf("The code to be timed took %e seconds\n", elapsed);

    GET_TIME(start);
    print_matrix(matrix, matrix_size, matrix_size);
    GET_TIME(finish);
    elapsed = (finish - start);
    printf("The code to be timed took %e seconds\n", elapsed);
    
    // pthread_t tid[NTHREADS];
    // int thread;
    
    // thread_arguments *args;
    // args = malloc(sizeof(thread_arguments));
    //     if (args == NULL) {
    //         printf("--ERRO: malloc()\n"); 
    //         exit(-1);
    //     }
    
    // Inicia threads e passam o trabalho para elas
    // for(thread = 0; thread < NTHREADS; thread++) {
    //     printf("Preenche argumentos para a thread %d \n", thread);
    //     args->thread_id = thread;
    //     printf("Cria a thread %d\n", thread);

    //     if(pthread_create(&tid[thread], NULL, increment_element_in_array, (void *) args)) {
    //         printf("Erro: pthread_create");
    //         exit(-1);
    //     }
    // }

    // Aguarda a finalização das threads
    // for(thread = 0; thread < NTHREADS; thread++) {
    //     if(pthread_join(tid[thread], NULL)) {
    //         printf("Erro: pthread_join");
    //         exit(-1);
    //     }
    // }

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

int random_number(int min, int max) {
    int random = rand();
    return random % (max - min + 1 ) + min;
}

void free_matrix(int *matrix) {
    free(matrix);
}