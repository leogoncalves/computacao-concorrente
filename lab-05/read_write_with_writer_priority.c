#include <pthread.h>
#include <semaphore.h> 
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t mutexA;
pthread_mutex_t mutexB;

int ARRAY_SIZE;
int reader_threads;
int writer_threads;
int read_counter, writer_counter;
sem_t read_mutex, write_mutex, read, write;


typedef struct {
    int tid;    
    int* array;
} thread_arguments;

void *readers(void* args);
void *writers(void* args);
void fillArrayZeros(int *array);

int main(int argc, char *argv[]) {

    // Inicializa variável de condição
    pthread_mutex_init(&mutexA, NULL);

    read_counter = 0;
    writer_counter = 0;
    
    sem_init(&read_mutex, 0, 1);
    sem_init(&write_mutex, 0, 1);
    sem_init(&read, 0, 1);
    sem_init(&write, 0, 1);

    // Quantidade de elementos no array
    ARRAY_SIZE = 10;

    // Quantidade de threads leitoras
    reader_threads = 5;

    // quantidade de threads escritoras 
    writer_threads = 5;

    // Total de threads no programa
    int total_threads = reader_threads + writer_threads;

    // Variávei auxiliar
    int i;    

    // aloca um array de threads
    pthread_t *threads = (pthread_t*) malloc(total_threads * sizeof(pthread_t)) ;    

    // Caso tenha erro na criação das threads, lança o erro
    if(threads == NULL) {
        printf("--ERRO: malloc de tid");
        exit(-1);
    }

    // aloca um array de elementos
    int *array = (int*) malloc(ARRAY_SIZE * sizeof(int));

    // inicializa array com zeros
    fillArrayZeros(array);

    // aloca memória para parametros que serão passados por threads
    thread_arguments *args = (thread_arguments*) malloc(total_threads * sizeof(thread_arguments));
    if (args == NULL) {
        printf("--ERRO: malloc()\n"); 
        exit(-1);
    }

    // argumentos para threads
    for(i = 0; i < total_threads; i++) {        
        args[i].tid = i;
        args[i].array = array;
    }

    // cria threads leitores
    for(i = 0; i < reader_threads; i++) {
        if(pthread_create(&threads[i], NULL, readers, (void*)(&args[i]))){
            printf("--ERRO: reader_threads pthread_create()\n"); 
            exit(-1);
        }
    }

    // cria threads escritoras    
    for(i = reader_threads; i < total_threads; i++) {
        if(pthread_create(&threads[i], NULL, writers, (void*)(&args[i]))){
            printf("--ERRO: writer_threads pthread_create()\n"); 
            exit(-1);
        }
    }

    for(i = 0; i < total_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutexA);
    
    return 0;
}


void *readers(void* args) {    
    while(1) {
        sem_wait(&read); // leitor
        pthread_mutex_lock(&mutexA);
        read_counter++;
        if(read_counter == 1) {
            sem_wait(&writer);
        }
        pthread_mutex_unlock(&mutexA);
        sem_post(&read);

        thread_arguments *params = (thread_arguments*) args;
        int id = params->tid;
        int *array = params->array;
        int sum = 0;
        printf("Leitura da thread %d: \n", id);
        for(int i = 0; i < ARRAY_SIZE; i++) {        
            printf("%d ", array[i]);
            sum += array[i];
        }
        printf("\nMédia dos valores: %lf\n\n", (1.0 * sum)/ARRAY_SIZE);

        pthread_mutex_lock(&mutexA);
        read_counter--;
        if(read_counter == 0) {
            sem_post(&read);
        }
        pthread_mutex_unlock(&mutexA);
        pthread_exit(NULL);
    } 
}

void *writers(void* args) {
    while(1) {
        pthread_mutex_lock(&mutexB);
        writer_counter++;
        if(writer_counter == 1) {
            sem_wait(&write);
        }
        pthread_mutex_unlock(&mutexB);

        sem_wait(&write);
        thread_arguments *params = (thread_arguments*) args;
        int id = params->tid;
        int *array = params->array;
        
        printf("Escrevemos com a thread %d \n", id);
        
        array[0] = id;
        array[ARRAY_SIZE - 1] = id;
        for(int i = 1; i < ARRAY_SIZE - 1; i++) {        
            array[i] = id * 2;
            
        }
        sem_post(&write);

        pthread_mutex_lock(&mutexB);
        writer_counter--;
        if(writer_counter == 0) {
            sem_post(&write);
        }
        pthread_mutex_unlock(&mutexB);
        
        pthread_exit(NULL);
    }    
}

void fillArrayZeros(int *array){
    for(int i = 0; i < ARRAY_SIZE; i++) {        
        array[i] = 0;
    }
}
