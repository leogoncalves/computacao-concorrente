#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define NTHREADS  2
#define SIZE_VECTOR 10

typedef struct {
    int thread_id;
    int *vector;
} thread_arguments;

void* increment_element_in_array(void* arg);
void printArray (int vector[]);

int main(void) {

    
    pthread_t tid[NTHREADS];
    int thread;
    int *vector = malloc(sizeof(int) * SIZE_VECTOR);
    *vector = 0;
    thread_arguments *args;

    printf("Estado inicial do vetor de %d elementos \n", SIZE_VECTOR);
    printArray(vector);

    for(thread = 0; thread < NTHREADS; thread++) {
        printf("aloca e preenche argumentos para a thread %d \n", thread);
        args = malloc(sizeof(thread_arguments));
        if (args == NULL) {
            printf("--ERRO: malloc()\n"); 
            exit(-1);
        }

        args->thread_id = thread;
        args->vector = vector;

        printf("Cria a thread %d\n", thread);

        if(pthread_create(&tid[thread], NULL, increment_element_in_array, (void *) args)) {
            printf("Erro: pthread_create");
            exit(-1);
        }
    }

    for(thread = 0; thread < NTHREADS; thread++) {
        if(pthread_join(tid[thread], NULL)) {
            printf("Erro: pthread_join");
            exit(-1);
        }
    }

    printf("Estado final do vetor de %d elementos \n", SIZE_VECTOR);
    printArray(vector);

    printf("Terminamos \n");

    return 0;
}

void* increment_element_in_array(void* arg) {
    
    thread_arguments *args = (thread_arguments*) arg;
    int thread_id = args->thread_id;       
    
    for(int i = thread_id; i < SIZE_VECTOR; i += 2) {        
        args->vector[i] = 1;
    }

    free(args);
    pthread_exit(NULL);
}

void printArray (int vector[]) {
    printf("[");
    for(int i = 0; i < SIZE_VECTOR; i++) {
        printf(" %d, ", vector[i]);
    }
    printf("]\n");
}