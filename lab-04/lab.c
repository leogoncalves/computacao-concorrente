#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS 4

void* threadA();
void* threadB();
void* threadC();
void* threadD();

int GATE[2] = {0, 0};

pthread_mutex_t mutex;
pthread_cond_t cond;

int main(int argc, char *argv[]) {
    pthread_t threads[NTHREADS];

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create(&threads[0], NULL, threadA, NULL);
    pthread_create(&threads[1], NULL, threadB, NULL);
    pthread_create(&threads[2], NULL, threadC, NULL);
    pthread_create(&threads[3], NULL, threadD, NULL);

    for (int i = 0; i < NTHREADS; i++) {
        pthread_join(threads[i], NULL);
    } 

    printf("Fim \n");

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}

// threads A e B devem rodar antes da C e D
void* threadA() {
    printf("1 - tudo bem? \n");
    GATE[0] = 1;
    pthread_mutex_lock(&mutex);
    if(GATE[0] == 1 && GATE[1] == 1) {
        pthread_cond_broadcast(&cond);
    }
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}
void* threadB() {
    printf("2 - bom dia! \n");
    GATE[1] = 1;
    pthread_mutex_lock(&mutex);
    if(GATE[0] == 1 && GATE[1] == 1) {
        pthread_cond_broadcast(&cond);
    }
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}
void* threadC() {
    pthread_mutex_lock(&mutex);
    if(GATE[0] == 0 || GATE[1] == 0) {
        pthread_cond_wait(&cond, &mutex);   
    }
    pthread_mutex_unlock(&mutex);
    printf("3 - até mais! \n");
    pthread_exit(NULL);
}
void* threadD() {
    pthread_mutex_lock(&mutex);
    if(GATE[0] == 0 || GATE[1] == 0) {
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);
    printf("4 - boa tarde! \n");
    pthread_exit(NULL);
}