# Problema do leitor escritor

Para executar a aplicação, basta clonar o repositório, ir até a pasta lab-04 e executar em um shell unix

> $ make

Dois arquivos serão compilados:

- without_priority
- writer_priority

Cada um deles recebe 3 parâmetros na entrada: A quantidade de elementos, o número de threads de leitura e o número de threads de escrita
Então, para executar, faça algo como:

> $ ./without_priority [quantidade de elementos] [threads leitoras] [threads escritoras]
