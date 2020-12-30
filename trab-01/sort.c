#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <time.h>
#include "macros/timer.h"

void quicksort(int array[], int lower, int high);
int partition(int array[], int lower, int high);
void swap(int* a, int* b);
void printArray(int* arr, int size);

int main(int argc, char *argv[]) {
    int array[] = {32,12,32323, 321,12,1,0,-1};
    printArray(array, 7);
    quicksort(array, 0, 6);
    printArray(array, 7);
    return 0;
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
    int t = *a;
    *a = *b;
    *b = t;
}

void printArray(int* arr, int size) {    
    for(int i = 0; i < size; i++){
        printf("%d ", arr[i]);
    }
    printf("\n");
}
