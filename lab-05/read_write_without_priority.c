#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t mutex;
pthread_cond_t cond_writer, cond_reader;
int ARRAY_SIZE;
int reader_threads;
int writer_threads;
int reader = 0;
int writer = 0;

typedef struct {
    int tid;    
    int* array;
} thread_arguments;

void writeUp();
void writeDown();
void readUp();
void readDown();

void *readers(void* args);
void *writers(void* args);
void fillArrayZeros(int *array);

int main(int argc, char *argv[]) {
    if(argc < 3) {
        printf("Faltam argumentos. Informe: \n - A quantidade de elementos \n - Quantidades de threads leitoras \n - Quantidades de threads escritoras \n");
        exit(-1);
    }

    // Quantidade de elementos no array
    ARRAY_SIZE = atoi(argv[1]);

    // Quantidade de threads leitoras
    reader_threads = atoi(argv[2]);

    // quantidade de threads escritoras 
    writer_threads = atoi(argv[3]);

    // Total de threads no programa
    int total_threads = reader_threads + writer_threads;

    // Variávei auxiliar
    int i;

    // Inicializa variável de condição
    pthread_mutex_init(&mutex, NULL);

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

    pthread_mutex_destroy(&mutex);
    
    return 0;
}


void *readers(void* args) {
    while(1){
        readUp();
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
        readDown();
        pthread_exit(NULL);
    }
}

void *writers(void* args) {
    while(1){
        writeUp();
        thread_arguments *params = (thread_arguments*) args;
        int id = params->tid;
        int *array = params->array;
        
        printf("Escrevemos com a thread %d \n", id);
        
        array[0] = id;
        array[ARRAY_SIZE - 1] = id;
        for(int i = 1; i < ARRAY_SIZE - 1; i++) {        
            array[i] = id * 2;
            
        }
        writeDown();
        pthread_exit(NULL);
    }
}

void fillArrayZeros(int *array){
    for(int i = 0; i < ARRAY_SIZE; i++) {        
        array[i] = 0;
    }
}


void writeUp(){
    pthread_mutex_lock(&mutex);
    while((reader > 0) || (writer > 0)){
        pthread_cond_wait(&cond_writer, &mutex);        
    }
    writer++;
    pthread_mutex_unlock(&mutex);
}
void writeDown(){
    pthread_mutex_lock(&mutex);
    writer--;
    pthread_cond_signal(&cond_writer);
    pthread_cond_broadcast(&cond_reader);
    pthread_mutex_unlock(&mutex);
}
void readUp(){
    pthread_mutex_lock(&mutex);
    while(writer > 0) {
        pthread_cond_wait(&cond_reader, &mutex);
    }
    reader++;
    pthread_mutex_unlock(&mutex);
}
void readDown(){
    pthread_mutex_lock(&mutex);
    reader--;
    if(reader == 0) {
        pthread_cond_signal(&cond_writer);
    }
    pthread_mutex_unlock(&mutex);
}