all: build

test: build
	@./main

build: 
	gcc sort.c -o main -Wall -lpthread -lm

debug:
	gcc -g sort.c -o main -Wall -lpthread -lm
