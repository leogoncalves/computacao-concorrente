all: build

test: build
	@./main 1 > tests.md

build: 
	gcc serie.c -o main -Wall -lpthread -lm

debug:
	gcc -g serie.c -o main -Wall -lpthread -lm
