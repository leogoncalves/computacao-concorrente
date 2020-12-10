all: run

run: build
	@./main

test: build
	# tamanho da matrix | numero de threads | modo verboso ou não
	@./main 500 1 0
	@./main 500 1 0
	@./main 500 1 0
	@./main 500 1 0
	@./main 500 1 0
	
	@./main 500 2 0
	@./main 500 2 0
	@./main 500 2 0
	@./main 500 2 0
	@./main 500 2 0
	
	@./main 500 4 0
	@./main 500 4 0
	@./main 500 4 0
	@./main 500 4 0
	@./main 500 4 0
	
	@./main 1000 1 0
	@./main 1000 1 0
	@./main 1000 1 0
	@./main 1000 1 0
	@./main 1000 1 0
	
	@./main 1000 2 0
	@./main 1000 2 0
	@./main 1000 2 0
	@./main 1000 2 0
	@./main 1000 2 0
	
	@./main 1000 4 0
	@./main 1000 4 0
	@./main 1000 4 0
	@./main 1000 4 0
	@./main 1000 4 0

	@./main 2000 1 0
	@./main 2000 1 0
	@./main 2000 1 0
	@./main 2000 1 0
	@./main 2000 1 0
	
	@./main 2000 2 0
	@./main 2000 2 0
	@./main 2000 2 0
	@./main 2000 2 0
	@./main 2000 2 0

	@./main 2000 4 0
	@./main 2000 4 0
	@./main 2000 4 0
	@./main 2000 4 0
	@./main 2000 4 0


build: 
	gcc multiply_matrix.c -o main -Wall -lpthread

debug:
	gcc -g --std=c11 multiply_matrix.c -o main -Wall -lpthread
