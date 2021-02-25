#include <stdlib.h>
#include <stdio.h>

#define DEBUG 0

void find_identical_value_sequence(int* array, int array_size);
int three_equals_values (int a, int b, int c);
void find_continous_sequence_of_same_value(int* array, int array_size);
void find_number_of_match_sequence(int *array, int array_size);

int main(void) {
    int array_size = 30;
    int example[] = {30, 1, 2, 3, 1, 1, 1, 1, 2, 3, 4, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 3, 3, 3, 0, 1, 2, 3, 4, 5, 0};    
    find_identical_value_sequence((int *)example, array_size); // 11 7 5
    find_continous_sequence_of_same_value((int *)example, array_size); // 5
    find_number_of_match_sequence((int *)example, array_size);

}

/*
    Encontra a maior subsequencia de dígitos iguais
    e a posição de início da sua ocorrência
    
    @param array Ponteiro para array de inteiros
    @param array_size Quantidade de elementos no array
*/
void find_identical_value_sequence(int* array, int array_size) {
    int count = 1; 
    int max = 1;
    int sequence_value = -1;
    int position = -1;

    for(int i = 0; i < array_size; i++) {
        if(array[i] == array[i+1]) {
            count++;
        } else {
            if(count > max) {
                max = count;
                sequence_value = array[i];
                position = i - count;
            }
            count = 1;
        }
    }

    printf("Maior sequencia de valores identicos: \n");
    printf("\t- Posição: %d \n", position);
    printf("\t- Tamanho: %d \n", max);
    printf("\t- Valor da sequencia que se repete: %d \n", sequence_value);
}

/*
    Verifica se três valores inteiros são iguais.
    Caso a avaliação seja verdadeira, retorna 1;
    Caso a avaliação seja falsa, retorna 0;
    
    @param a
    @param b
    @param c
*/
int three_equals_values (int a, int b, int c) {
    if( a == b && a == c) {
        return 1;
    }
    return 0;
}

/*
    Retorna a quantidade de ocorrências de sequencias de 
    três dígitos iguais em um array. 

    @param array Ponteiro para array de inteiros
    @param array_size Quantidade de elementos no array
*/
void find_continous_sequence_of_same_value(int* array, int array_size) {
    int count = 0;
    int i = 0;
    while(i < array_size) {
        if(three_equals_values(array[i], array[i + 1], array[i + 2])) {
            if(DEBUG) {
                printf("%d: %d %d %d\n", i, array[i], array[i + 1], array[i + 2]);
            }
            count++;
            i+=2;
        }
        i += 1;
    }
    printf("Quantidade de triplas: %d \n", count);
}

/*
    Encontra a quantidade de ocorrencias da 
    subsequencia <012345> em um vetor

    @param array Ponteiro para array de inteiros
    @param array_size Quantidade de elementos no array
*/
void find_number_of_match_sequence(int *array, int array_size) {
    
    // Parametros imutaveis
    int DEFAULT_SIZE_MATCH = 6;
    int PATTERN_MATCH[] = {0, 1, 2, 3, 4, 5};
    
    for(int i = 0; i <= array_size - DEFAULT_SIZE_MATCH; i++ ) {
        int j;
        for(j = 0; j < DEFAULT_SIZE_MATCH; j++) {
            if(array[i+j] != PATTERN_MATCH[j])
                break;
        }
        if(j == DEFAULT_SIZE_MATCH) {
            printf("Quantidade de ocorrencias da sequencia <012345>: %d \n", i);
        }
    }
}