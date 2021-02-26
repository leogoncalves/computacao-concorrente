#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void create_control_test_file(char* filename);
void create_file_with_random_numbers(char* filename, long int size, int min, int max);
int generate_random(int min, int max);

int main(int argc, char *argv[]) {
    srand(time(NULL));

    if(argc < 2) {
        printf("Faltam argumentos \n");
        return 1;
    }
    int use_random = atoi(argv[1]);
    
    if(use_random == 1) {
        char* filenameRandomTest = "assets/random_input.bin";
        long int size = 100000;
        int min = 0;
        int max = 5;
        create_file_with_random_numbers(filenameRandomTest, size, min, max);    
    } else {
        char* filenameControlTest = "assets/input.bin";    
        create_control_test_file(filenameControlTest);
    }
}

void create_control_test_file(char* filename) {
    FILE* file = NULL;
    file = fopen(filename, "ab");
    long int size = 30;
    int example[] = {1, 2, 3, 1, 1, 1, 1, 2, 3, 4, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 3, 3, 3, 0, 1, 2, 3, 4, 5, 0};    
    long int i;

    if(file == NULL) {
        printf("Failed to create file");
        exit(EXIT_FAILURE);
    }

    fwrite(&size, sizeof(long int), 1, file);
    
    for(i = 0; i < size; i++) {
        printf("%d ", example[i]);
        fwrite(&example[i], sizeof(int), 1, file);
    }

    fclose(file);
}

void create_file_with_random_numbers(char* filename, long int size, int min, int max) {
    FILE* file = NULL;
    file = fopen(filename, "ab");
    long int i;

    if(file == NULL) {
        printf("Failed to create file");
        exit(EXIT_FAILURE);
    }

    fwrite(&size, sizeof(long int), 1, file);
    
    for(i = 0; i < size; i++) {
        int random_number = generate_random(min, max);
        printf("%d ", random_number);
        fwrite(&random_number, sizeof(int), 1, file);
    }
    printf("\n");

    fclose(file);
}

int generate_random(int min, int max) {  
    int random = rand();
    return random % (max - min + 1 ) + min;
}

