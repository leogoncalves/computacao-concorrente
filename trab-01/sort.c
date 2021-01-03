#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <time.h>
#include "macros/timer.h"
#include <math.h>

typedef struct {
    int *array;
    int lower, high;
} thread_arguments;

void quicksort(int array[], int lower, int high);
int partition(int array[], int lower, int high);
void swap(int* a, int* b);
void printArray(int* arr, int size);
int random_number(int min, int max);
void sequential_quicksort();
void quickSortIterative(int arr[], int l, int h);

int main(int argc, char *argv[]) {
    srand(time(NULL));
    sequential_quicksort();
    return 0;
}

void sequential_quicksort() {
    double start, finish, elapsed;
    int size = 10000000;

    int *array = (int*) malloc(size * sizeof(int));
    for(int i = 0; i < size; i++) {
        *(array + i) = random_number(0, 8096);
    }
    // printArray(array, size);
    GET_TIME(start);
    quicksort(array, 0, size - 1);
    GET_TIME(finish);
    elapsed = finish - start;
    // printArray(array, size);
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

void quickSortIterative(int arr[], int l, int h) 
{ 
    // Create an auxiliary stack 
    int stack[h - l + 1]; 
  
    // initialize top of stack 
    int top = -1; 
  
    // push initial values of l and h to stack 
    stack[++top] = l; 
    stack[++top] = h; 
  
    // Keep popping from stack while is not empty 
    while (top >= 0) { 
        // Pop h and l 
        h = stack[top--]; 
        l = stack[top--]; 
  
        // Set pivot element at its correct position 
        // in sorted array 
        int p = partition(arr, l, h); 
  
        // If there are elements on left side of pivot, 
        // then push left side to stack 
        if (p - 1 > l) { 
            stack[++top] = l; 
            stack[++top] = p - 1; 
        } 
  
        // If there are elements on right side of pivot, 
        // then push right side to stack 
        if (p + 1 < h) { 
            stack[++top] = p + 1; 
            stack[++top] = h; 
        } 
    } 
} 

void *qsortThread(void* args) {
    thread_arguments *targs = (thread_arguments*) args;
}