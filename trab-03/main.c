#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>

#define DEBUG 0

typedef struct {
    // position | max | sequence_value
    long long int sequence_of_identical_values[3];
    long long int amount_triples;
    long long int occurrences_of_sequence;
} Response;

int three_equals_values (int a, int b, int c);
long long int get_size(char* filename);

void show_buffer(int* array, long long int array_size);
void set_buffer(int* array, long long int array_size, int value);
void showResponse(Response *response);
void read_from_binary_file(char* filename, int* buffer, long long int buffer_size, long long int offset);
void find_identical_value_sequence(int* array, int array_size, Response* response);
void find_continous_sequence_of_same_value(int* array, int array_size, Response* response);
void find_number_of_match_sequence(int *array, int array_size, Response* response);
void debug_binary_file(char* filename);
Response* initialize_response();

int main(int argc, char *argv[]) {

    Response *response = initialize_response();

    // char* filenameControlTest = "input.bin";   
    char* filenameControlTest = "random_input.bin";   
    // char* filenameRandomTest = "random_input.bin";
    
    /*
        Tamanho do arquivo
    */
    long long int size = get_size(filenameControlTest);
    if(DEBUG) {
        debug_binary_file(filenameControlTest);
    }
    debug_binary_file(filenameControlTest);
    /*
        Tamanho do Bloco (chunk do arquivo)
        1 KB = 1024 bytes
        1 MB = 1024 Kylobytes
        1 GB = 1024 Megabytes

        M = 1024 * 1024 * 1 = 1MB

    */
    long long int M = 1024 * 1024 * 1;

    /*
        Tamanho do buffer a ser utilizado pela aplicação
    */
    long long int N = 1024;

    
    long long int offset = 0;
    long long int buffer_size = N;
    int* bufferT = (int*) malloc(buffer_size * sizeof(int));
    set_buffer(bufferT, buffer_size, -1);
    
    long long int i = 0;
    long long int amount_blocks = ceil(M / N);
    printf("amount_blocks: %lld \n", amount_blocks);
    while(i != amount_blocks) {
        /*
            Aloca memória dinamicamente, a 
            depender do tamanho do bloco
            necessário
        */
        read_from_binary_file(filenameControlTest, bufferT, buffer_size, offset);
        find_identical_value_sequence(bufferT, buffer_size, response);
        find_continous_sequence_of_same_value(bufferT, buffer_size, response);
        find_number_of_match_sequence(bufferT, buffer_size, response);
        
        offset += buffer_size;        
        set_buffer(bufferT, buffer_size, -1);
        i++;
    }
    showResponse(response);
}

void showResponse(Response *response) {
    
    printf("- Maior sequencia de valores identicos: \n");
    printf("\t- Posição: %lld \n", response->sequence_of_identical_values[0]);
    printf("\t- Tamanho: %lld \n", response->sequence_of_identical_values[1]);
    printf("\t- Valor da sequencia que se repete: %lld \n", response->sequence_of_identical_values[2]);

    printf("- Quantidade de triplas que se repetem: %lld \n", response->amount_triples);

    printf("- Quantidade de ocorrencias da sequencia <012345>: %lld \n", response->occurrences_of_sequence);
}

/*
    Encontra a maior subsequencia de dígitos iguais
    e a posição de início da sua ocorrência
    
    @param array Ponteiro para array de inteiros
    @param array_size Quantidade de elementos no array
*/
void find_identical_value_sequence(int* array, int array_size, Response* response) {
    int count = 1; 
    int max = 1;
    int sequence_value = -1;
    int position = -1;

    for(int i = 0; i < array_size; i++) {
        if(array[i] == -1) {
            break;
        }
        if(array[i] == array[i+1]) {
            count++;
        } else {
            if(count > max) {
                position = i - count + 1;
                max = count;
                sequence_value = array[i];
            }
            count = 1;
        }
    }

    if(max > response->sequence_of_identical_values[1]) {
        response->sequence_of_identical_values[0] = position;
        response->sequence_of_identical_values[1] = max;
        response->sequence_of_identical_values[2] = sequence_value;
    }

    if(DEBUG) {
        printf("[DEBUG] Maior sequencia de valores identicos: \n");
        printf("\t- Posição: %d \n", position);
        printf("\t- Tamanho: %d \n", max);
        printf("\t- Valor da sequencia que se repete: %d \n", sequence_value);
    }
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
void find_continous_sequence_of_same_value(int* array, int array_size, Response* response) {
    int count = 0;
    int i = 0;
    while(i < array_size) {
        if(array[i] == -1) {
            break;
        }
        if(three_equals_values(array[i], array[i + 1], array[i + 2])) {            
            count++;
            i+=2;
        }
        i += 1;
    }
    response->amount_triples += count;
    if(DEBUG) {
        printf("[DEBUG] Quantidade de triplas que se repetem: %d \n", count);
    }
    
}

/*
    Encontra a quantidade de ocorrencias da 
    subsequencia <012345> em um vetor

    @param array Ponteiro para array de inteiros
    @param array_size Quantidade de elementos no array
*/
void find_number_of_match_sequence(int *array, int array_size, Response* response) {

    if(DEBUG) {
        printf("[DEBUG] Buffer em find_number_of_match_sequence \n");
        show_buffer(array, array_size);
    }
    
    // Parametros imutaveis
    int DEFAULT_SIZE_MATCH = 6;
    int PATTERN_MATCH[] = {0, 1, 2, 3, 4, 5};
    long long int total_occurence = 0;
    
    for(int i = 0; i <= array_size - DEFAULT_SIZE_MATCH; i++ ) {
        if(array[i] == -1) {
            break;
        }
        int j;
        for(j = 0; j < DEFAULT_SIZE_MATCH; j++) {
            if(array[i+j] != PATTERN_MATCH[j])
                break;
        }
        if(j == DEFAULT_SIZE_MATCH) {
            total_occurence += 1;
            if(DEBUG) {
                printf("[DEBUG] Ocorrencias da sequencia <012345> na posição: %d \n", i);
            }
        }
    }
    response->occurrences_of_sequence = total_occurence;
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

        
    if(DEBUG) {
        printf("[DEBUG] Offset em read_from_binary_file: %lld \n", offset);
    }

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
    if(DEBUG) {
        printf("[DEBUG] show_buffer \n");
    }
    for(int i = 0; i < array_size; i++) {
        printf("%d ", *(array + i));
    }
    printf("\n");
}

void set_buffer(int* array, long long int array_size, int value) {
    for(int i = 0; i < array_size; i++) {
        array[i] = value;
    }
}

Response* initialize_response() {
    Response *response = (Response*) malloc(sizeof(Response));
    for(int i = 0; i < 3; i++)
        response->sequence_of_identical_values[i] = 0;
    response->amount_triples = 0;
    response->occurrences_of_sequence = 0;

    return response;
}

void debug_binary_file(char* filename) {
    printf("[DEBUG] ARQUIVO BINARIO GERADO\n");
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
    long long int size = get_size(filename);
    int *buffer = (int*) malloc(size * sizeof(int));
    
    fseek(file, sizeof(long long int), 1);

    for(long long int i = 0; i < size; i++) {
        fread(&buffer[i], sizeof(int), 1, file);
    }
    show_buffer(buffer, size);
    printf("\n");

    fclose(file);
}
