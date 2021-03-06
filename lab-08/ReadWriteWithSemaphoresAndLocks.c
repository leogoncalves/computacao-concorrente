#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>

pthread_mutex_t mutex;
sem_t write;
int writing = 0;

/*
    struct thread_arguments
    int tid
    Identificador único da thread
    int  *array;
    Vetor compartilhado entre as threads;
*/
typedef struct {
    int tid;
    int array_size;
    int *array;
} thread_arguments;

/*
    Método passado para threads leitoras;
*/
void *reader(void *args);

/*
    Método passado para threads escritoras;
*/
void *writer(void *args);

/*
    Método para inicialização de semáforos
*/
void initializeSemaphoresAndMutex();

/*
    Realiza a operação de leitura 
*/
void make_read(int tid, int* array, int array_size);

/*
    Realiza a operação de escrita
*/
void make_write(int tid, int* array, int array_size);


/*
    Operação apenas para gastar o tempo da thread
*/
void delay();

int main(int argc, char *argv[]) {
    
    initializeSemaphoresAndMutex();
    if(argc < 3) {
        printf("Faltam argumentos. Informe: \n");
        printf("- Quantidade de threads leitoras \n");
        printf("- Quantidade de threads escritoas \n");
        printf("- Tamanho do buffer \n");
        printf("- Todos os elementos devem ser inteiros \n");
        exit(-1);
    }
    int thread_readers = atoi(argv[1]);
    int thread_writers = atoi(argv[2]);
    int array_size = atoi(argv[3]);
    int total_threads = thread_readers + thread_writers;
    

    pthread_t *threads = (pthread_t*) malloc(total_threads * sizeof(pthread_t));
    thread_arguments *args = (thread_arguments*) malloc(total_threads * sizeof(thread_arguments));

    int *array = (int *)malloc(array_size * sizeof(int));

    for(int i = 0; i < thread_readers; i++) {
        args[i].tid = i;
        args[i].array = array;
        args[i].array_size = array_size;
    }

    for(int i = 0; i < thread_writers; i++) {
        args[i + thread_readers].tid = i + thread_readers;
        args[i + thread_readers].array = array;
        args[i + thread_readers].array_size = array_size;
    }

    for(int i = 0; i < thread_readers; i++) {
        if(pthread_create(&threads[i], NULL, reader, (void*)(&args[i]))){
            printf("--ERRO: pthread_create()\n"); 
            exit(-1);
        }
    }

    for(int i = 0; i < thread_writers; i++) {
        if(pthread_create(&threads[i+thread_readers], NULL, writer, (void*)(&args[i + thread_readers]))){
            printf("--ERRO: pthread_create()\n"); 
            exit(-1);
        }
    }

    // Aguarda a finalização das threads
    for(int i = 0; i < total_threads ; i++) {
        if(pthread_join(threads[i], NULL)) {
            printf("Erro: pthread_join");
            exit(-1);
        }
    }

    
}

void initializeSemaphoresAndMutex() {
    sem_init(&write, 0, 1);
    pthread_mutex_init(&mutex, NULL);
}

void *reader(void *args) {
    while(1) {
        delay();
        thread_arguments *data = (thread_arguments*) args;
        sem_wait(&write);        
        make_read(data->tid, data->array, data->array_size);        
        sem_post(&write);
    }
}

void *writer(void *args) {
    while(1) {
        delay();
        thread_arguments *data = (thread_arguments*) args;
        pthread_mutex_lock(&mutex);
        writing += 1;
        if(writing == 1) {
            sem_wait(&write);
        }
        make_write(data->tid, data->array, data->array_size);
        pthread_mutex_unlock(&mutex);

        pthread_mutex_lock(&mutex);
        writing -= 1;
        if(writing == 0) {
            sem_post(&write);
        }
        pthread_mutex_unlock(&mutex);
    }
}

void make_read(int tid, int* array, int array_size) {
    printf("Thread %d: Começamos a operação de leitura\n", tid);
    for(int i = 0; i < array_size; i++) {
        printf("%d ", *(array + i));
    }
    printf("\n");
    printf("Terminamos a operação de leitura\n");
}


void make_write(int tid, int* array, int array_size) {
    printf("Thread %d: Começamos a operação de escrita\n", tid);
    for(int i = 0; i < array_size; i++) {
        *(array + i) = tid * 2;
    }
    printf("Terminamos a operação de escrita\n");
}

void delay() {
    int i = 0;
    while(i < 100000000)
        i++;
}