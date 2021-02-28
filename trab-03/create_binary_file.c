#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void create_control_test_file(char* filename);
void create_file_with_random_numbers(char* filename, long long int size, int min, int max);
int generate_random(int min, int max);

int main(int argc, char *argv[]) {
    srand(time(NULL));

    if(argc < 2) {
        printf("Faltam argumentos \n");
        printf("Informe quantos elementos deseja alocar no arquivo \n");
        return 1;
    }
    
    /*
        Cria arquivo com números aleatórios de 0 a 5
    */
    char* filename = "input.bin";
    int min = 0;
    int max = 5;
    long long int size =  atol(argv[1]);
    create_file_with_random_numbers(filename, size, min, max);    
}

void create_file_with_random_numbers(char* filename, long long int size, int min, int max) {
    FILE* file = NULL;
    file = fopen(filename, "ab");
    long long int i;

    if(file == NULL) {
        printf("Failed to create file");
        exit(EXIT_FAILURE);
    }

    fwrite(&size, sizeof(long long int), 1, file);
    
    for(i = 0; i < size; i++) {
        int random_number = generate_random(min, max);
        fwrite(&random_number, sizeof(int), 1, file);
    }

    fclose(file);
}

int generate_random(int min, int max) {  
    int random = rand();
    return random % (max - min + 1 ) + min;
}

