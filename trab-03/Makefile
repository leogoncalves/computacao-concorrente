all: build

build: clean create_binary_file main

main: clean
	gcc -g main.c -o main -Wall -lpthread -lm

create_binary_file: clean
	gcc create_binary_file.c -o create_binary_file -Wall

clean:
	rm -f main create_binary_file input.bin

test:
	@./create_binary_file 1000000
	@./main