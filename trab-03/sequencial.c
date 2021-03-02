#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include "macros/timer.h"

#define DEBUG 0

pthread_mutex_t mutex;
typedef struct {
    // position | max | sequence_value
    long long int sequence_of_identical_values[3];
    long long int amount_triples;
    long long int occurrences_of_sequence;
} Response;

typedef struct {
    int *array;
    int array_size;
    Response* response;
} threadArguments;

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

void* ThreadA(void *args);
void* ThreadB(void *args);
void* ThreadC(void *args);

Response* initialize_response();

int main(int argc, char *argv[]) {

    if(argc < 3){
        printf("Falha ao executar o programa. Faltam argumentos. \n");
        printf("Informe: \n");
        printf("- Um valor para M (Tamanho do Bloco) \n");
        printf("- Um valor para N (Tamanho do Buffer) \n");
        printf("Ex: \n");
        printf("$ ./main 1000 100 \n");
        return 1;
    }
    
    double start, finish, elapsed; // tomadores de tempo
    
    /*
        Inicializar nossa estrutura utilizada para 
        armazenar as respostas
    */
    Response *response = initialize_response();

    /*
        Inicializa o mutex que será utilizado
        para controlar as nossas seções críticas
    */
    pthread_mutex_init(&mutex, NULL);

    char* filename = "input.bin";   
    
    /*
        Tamanho do arquivo
    */
    long long int size = get_size(filename);
    if(DEBUG) {
        debug_binary_file(filename);
    }
    
    /*
        Tamanho do Bloco (chunk do arquivo)
        1 KB = 1024 bytes
        1 MB = 1024 Kylobytes
        1 GB = 1024 Megabytes

        M = 1024 * 1024 * 1 = 1MB

    */
    long long int M = atoll(argv[1]); //1024 * 1024 * 1;

    /*
        Tamanho do buffer a ser utilizado pela aplicação
    */
    long long int N = atoll(argv[2]); //1024;

    /*
        Variável de controle para
        offset do arquivo (posição do cursor no arquivo)
    */
    long long int offsetP = 0;

    /*
        Tamanho do buffer definido pelo usuário
    */
    long long int buffer_size = N;
    

    /*
        Buffer que será utilizado como elemento a ser consumido.
        Vamos passar elementos que estão no bufferP para o 
        bufferT
    */
    int* bufferP = (int*) malloc(M * sizeof(int));
    set_buffer(bufferP, M, -1);

    /*
        Buffer que vai receber os valores de bufferP 
        e será utilizado como consumidor
    */
    int* bufferT = (int*) malloc(buffer_size * sizeof(int));    
    set_buffer(bufferT, buffer_size, -1);
    

    /*
        Controla os blocos do arquivo para leitura de chunks
        de arquivo
    */
    long long int amount_blocks_P = ceil(sizeof(int) * size / M); 
    long long int i = 0;

    /*
        Controla os blocos do arquivo para leitura chunk
        para o buffer
    */
    long long int amount_blocks_T = ceil(M / N);    
    long long int j = 0;

    if(DEBUG) {
        printf("[DEBUG] amount_blocks_P: %lld\n", amount_blocks_P);
        printf("[DEBUG] amount_blocks_T: %lld\n", amount_blocks_T);
        printf("[DEBUG] Offset Inicial: %lld", offsetP);
    }
    GET_TIME(start);
    while(i <= amount_blocks_P) {
        
        if(DEBUG) {
            printf("[DEBUG] Le bloco %lld do arquivo pra chunk\n", i);
        }
        
        set_buffer(bufferP, M, -1);
        read_from_binary_file(filename, bufferP, M, offsetP);
        
        while(j < amount_blocks_T) {
            set_buffer(bufferT, buffer_size, -1);
            
            if(DEBUG) {
                printf("[DEBUG]  Le do arquivo para buffer: Bloco j =  %lld\n", j);
            }
            
            for(int k = 0; k < buffer_size; k++) {
                bufferT[k] = bufferP[k + (j * buffer_size)];
            }
            
            show_buffer(bufferT, buffer_size);

            threadArguments *targs = (threadArguments *) malloc(sizeof(threadArguments));
            targs->array = bufferT;
            targs->array_size = buffer_size;
            targs->response = response;
            
            find_identical_value_sequence(bufferT, buffer_size, response);
            find_continous_sequence_of_same_value(bufferT, buffer_size, response);
            find_number_of_match_sequence(bufferT, buffer_size, response);
            
            j++;
            
        }
        
        offsetP += M;
        i++;
    }
    GET_TIME(finish);
    
    showResponse(response);
    elapsed = finish - start;
    printf("Tempo gasto: %lf segundos \n", elapsed);

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

    // Início da Região crítica
    pthread_mutex_lock(&mutex);
    if(max > response->sequence_of_identical_values[1]) {
        response->sequence_of_identical_values[0] = position;
        response->sequence_of_identical_values[1] = max;
        response->sequence_of_identical_values[2] = sequence_value;
    }
    pthread_mutex_unlock(&mutex);
    // Fim da Região crítica

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
    // Início da Região crítica
    pthread_mutex_lock(&mutex);
    response->amount_triples += count;
    pthread_mutex_unlock(&mutex);
    // Fim da Região crítica
    
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
            pthread_mutex_lock(&mutex);
            total_occurence += 1;
            pthread_mutex_unlock(&mutex);
            if(DEBUG) {
                printf("[DEBUG] Ocorrencias da sequencia <012345> na posição: %d \n", i);
            }
        }
    }
    // Início da Região crítica
    pthread_mutex_lock(&mutex);
    response->occurrences_of_sequence += total_occurence;
    pthread_mutex_unlock(&mutex);
    // Fim da Região crítica
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
        for(int i = 0; i < array_size; i++) {
            printf("%d ", *(array + i));
        }
        printf("\n");
    }    
}

void set_buffer(int* array, long long int array_size, int value) {
    for(int i = 0; i < array_size; i++) {
        array[i] = value;
    }
}

Response* initialize_response() {
    Response *response = (Response*) malloc(sizeof(Response));
    for(int i = 0; i < 3; i++){
        response->sequence_of_identical_values[i] = 0;
    }
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

void* ThreadA(void* args) {
    threadArguments *targs = (threadArguments*) args;
    find_identical_value_sequence(targs->array, targs->array_size, targs->response);
    pthread_exit(NULL);
}

void* ThreadB(void* args) {
    threadArguments *targs = (threadArguments*) args;
    find_continous_sequence_of_same_value(targs->array, targs->array_size, targs->response);
    pthread_exit(NULL);
}

void* ThreadC(void* args) {
    threadArguments *targs = (threadArguments*) args;
    find_number_of_match_sequence(targs->array, targs->array_size, targs->response);
    pthread_exit(NULL);
}
