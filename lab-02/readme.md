# Lab 02 - Multiplicação de matrizes concorrente

## Opções disponíveis (via Makefile)

Para compilar, faça

> $ make build

Para executar a bateria de testes (execução com matrizes de 500, 1000 e 2000 posições, com 1, 2 e 4 threads)
**ATENÇÃO: A execução da bateria de testes levar mais de 15 minutos**

> $ make test

Para executar um programa de teste utilizando uma matriz de 500 posições e 4 threads, sem visualizar as matrizes

> $ make

## Rodando o programa

As execuções do programa precisam receber três parâmetros:

- Dimensão da matriz: um inteiro qualquer maior do que zero
- Número de threads que serão utilizadas: um inteiro qualquer maior do que zero
- Modo de visualização (verboso ou não): Para fins de demonstração. Use 1 para visualizar as matrizes geradas ou 0 para não imprimir as matrizes.

## Exemplo de execução

```sh
# Execução de matriz de 500 elementos,
# com 4 threads e sem visualizar as matrizes geradas
$ ./main 500 4 0
```

A saída será parecida com a mostrada abaixo

```
Execução com Dimensão 500 e 4 threads
Alocação e criação de matrizes: 0.005078 segundos
Alocação de threads e recursos: 0.000000 segundos
Execução da multiplicação de matrizes: 0.289785 segundos
Liberação de recursos: 0.000174 segundos
Tempo total gasto: 0.295037
```

## Sobre execução, testes e ganhos

É possível acompanhar o ganho de execução do algoritmo [nessa planilha](https://docs.google.com/spreadsheets/d/1JYsK0vEt_RA4nlVsRUPTi6_80HiTGipJoxDuzG-CAhw/edit?usp=sharing).

Os dados utilizados para o teste estão disponíveis [aqui](./out.md).
