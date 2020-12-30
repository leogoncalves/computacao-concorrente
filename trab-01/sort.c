#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <time.h>
#include "macros/timer.h"
#include <math.h>

void quicksort(int array[], int lower, int high);
int partition(int array[], int lower, int high);
void swap(int* a, int* b);
void printArray(int* arr, int size);
int random_number(int min, int max);
void sequential();

int main(int argc, char *argv[]) {
    srand(time(NULL));
    sequential();
    
    return 0;
}

void sequential() {
    double start, finish, elapsed;
    int size = 50; // pow(10,8);

    int *array = (int*) malloc(size * sizeof(int));
    for(int i = 0; i < size; i++) {
        *(array + i) = random_number(0, 8096);
    }
    printArray(array, size);
    GET_TIME(start);
    quicksort(array, 0, size - 1);
    GET_TIME(finish);
    elapsed = finish - start;
    printArray(array, size);
    printf("Tempo para ordenar os resultados: %lf segundos \n", elapsed);
}

int random_number(int min, int max) {
    int random = rand();
    return random % (max - min + 1 ) + min;
}

void quicksort(int array[], int lower, int high) {
    if(lower < high) {
        int pivot = partition(array, lower, high);
        quicksort(array, lower, pivot - 1);
        quicksort(array, pivot + 1, high);
    }
}

int partition(int array[], int lower, int high){
    int pivot = array[high];
    int i = lower - 1;    
    for(int j = lower; j <= high - 1; j++){
        if(array[j] < pivot) {
            i = i+1;
            swap(&array[i], &array[j]);            
        }
    }
    swap(&array[i+1], &array[high]);        
    
    return i+1;
}

void swap(int* a, int* b){
    int aux = *a;
    *a = *b;
    *b = aux;
}

void printArray(int* arr, int size) {    
    for(int i = 0; i < size; i++){
        printf("%d ", arr[i]);
    }
    printf("\n\n");
}
