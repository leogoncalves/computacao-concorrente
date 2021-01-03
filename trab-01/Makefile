all: build

test: build
	@./main

build: 
	gcc montecarlo_pi.c -o main -Wall -lpthread -lm

debug:
	gcc -g montecarlo_pi.c -o main -Wall -lpthread -lm
