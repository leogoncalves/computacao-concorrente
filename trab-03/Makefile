all: run

run: build
	@./main

build: 
	gcc main.c -o main -Wall -lpthread

make_binary:
	gcc create_binary_file.c -o create_binary_file -Wall

create_binary_file: make_binary
	@./create_binary_file 1
