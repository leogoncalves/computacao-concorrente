#include <stdio.h>
#include <pthread.h>

/*
    Semáforos

    @mutal_exclusion
    @priority
    @write
*/
sem_t mutal_exclusion;
sem_t priority;
sem_t write;

/*
*/
int reading = 0;
int writing = 0;



/*
*/
void *reader(void *args);

/*
*/
void *writer(void *args);

/*
*/
void initializeSemaphores();

/*
*/
void make_read();

/*
*/
void make_write();

int main() {
    initializeSemaphores();
    return 0;
}

void initializeSemaphores() {
    sem_init(&mutal_exclusion, 0, 1);
    sem_init(&write, 0, 1);
    sem_init(&priority, 0, 1);
}

void *reader(void *args){
    while(1) {
        sem_wait(&mutal_exclusion);
        while(writing > 0) {
            sem_post(&mutal_exclusion);
            sem_wait(&priority);
            sem_post(&priority);
            sem_wait(&mutal_exclusion);
        }
        reading += 1;
        if(reading == 1) {
            sem_wait(&write);
        }
        sem_post(&mutal_exclusion);
        make_read();
        reading -= 1;
        if(reading == 0) {
            sem_post(&write);
        }
        sem_post(&mutal_exclusion);
    }
}

void *writer(void *args) {
    while(1) {
        sem_wait(&mutal_exclusion);
        writing += 1;        
        if(writing == 1) {
            sem_wait(&priority);
        }
        sem_post(&mutal_exclusion);
        sem_wait(&write);
        make_write();
        sem_post();
        sem_wait();
        writing -= 1;
        if(writing == 0) {
            sem_post(&priority);
        }
        sem_post(&mutal_exclusion);
    }
}

void make_read() {

}


void make_write() {

}