all: run

run: build
	@./main

build: 
	gcc multiply_matrix.c -o main -Wall -lpthread

debug:
	gcc -g --std=c11 multiply_matrix.c -o main -Wall -lpthread
