#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>

#define DEBUG 1

void find_identical_value_sequence(int* array, int array_size);
int three_equals_values (int a, int b, int c);
void find_continous_sequence_of_same_value(int* array, int array_size);
void find_number_of_match_sequence(int *array, int array_size);

void read_from_binary_file(char* filename, int* buffer, long long int buffer_size, long long int offset);
long long int get_size(char* filename);
void show_buffer(int* array, long long int array_size);

int main(int argc, char *argv[]) {

    char* filenameControlTest = "input.bin";   
    // char* filenameRandomTest = "random_input.bin";
    
    /*
        Definir estratégia para o tamanho padrão 
        do buffer baseado na quantidade de elementos
        informado no arquivo
    */
    // long long int size = get_size(filenameControlTest);

    /*
        Tamanho do Bloco
    */
    // long long int M;

    /*
        Tamanho do buffer a ser utilizado pela aplicação
    */
    // long long int N;


    /*
        Buffer que será utilizado pelas threads
    */
    
    // long long int total_blocks = ceil(size / M);


    int i = 0;
    while(i < 3) {
        /*
            Aloca memória dinamicamente, a 
            depender do tamanho do bloco
            necessário
        */   
        long long int offset = 0;
        long long int buffer_size = 10;
        int* bufferT = (int*) malloc(buffer_size * sizeof(int));
        
        offset = buffer_size*i + offset;
        printf("offset: %lld \n", offset);
        
        read_from_binary_file(filenameControlTest, bufferT, buffer_size, offset);
        find_identical_value_sequence(bufferT, buffer_size);
        find_continous_sequence_of_same_value(bufferT, buffer_size);
        find_number_of_match_sequence(bufferT, buffer_size);
        
        i++;
    }
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
            count++;
            i+=2;
        }
        i += 1;
    }
    printf("Quantidade de triplas que se repetem: %d \n", count);
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

/*
    Realiza a leitura de um arquivo binario
    1 int = 4 bytes

    @param filename Caminho relativo para o arquivo que será lido
    @buffer Vetor de inteiros que será utilizado como buffer
*/
void read_from_binary_file(char* filename, int* buffer, long long int buffer_size, long long int offset) {
    FILE *file = NULL;
    
    file = fopen(filename, "rb");
    
    if(!file) {
        perror("fopen: Falha ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    /*
        Desconsidera o primeiro elemento lido do arquiv
        Ele é o tamanho da entrada e não estamos querendo
        nada com ele
    */
    fseek(file, sizeof(long long int), 1);

    /*
        Offset para posição do arquivo que queremos ler
    */
    fseek(file, offset * sizeof(int), 1);

    for(int i = 0; i < buffer_size; i++) {
        fread(&buffer[i], sizeof(int), 1, file);
    }
    
    if(DEBUG) {
        show_buffer(buffer, buffer_size);
    }

    fclose(file);
}


long long int get_size(char* filename) {
    
    FILE *file = NULL;

    file = fopen(filename, "rb");
    if(!file) {
        perror("fopen: Falha ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    long long int size;    
    fread(&size, sizeof(long long int), 1, file);
    fclose(file);

    return size;
}


/*  
    Método auxiliar para imprimir um vetor de inteiros

    @param array Vetor de inteiros
    @param array_size Tamanhho do vetor de inteiros
*/
void show_buffer(int* array, long long int array_size) {
    
    for(int i = 0; i < array_size; i++) {
        printf("%d ", *(array + i));
    }
    printf("\n");
}
    