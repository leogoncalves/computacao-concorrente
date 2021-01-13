all: run

run: build
	@./main

build: 
	gcc lab.c -o main -Wall -lpthread

debug:
	gcc -g lab.c -o main -Wall -lpthread -lm