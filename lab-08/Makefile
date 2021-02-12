all: build

test: ReadWriteWithSemaphoresAndLocks ReadWriteWithSemaphores

ReadWriteWithSemaphoresAndLocks: 
	gcc ReadWriteWithSemaphoresAndLocks.c -o ReadWriteWithSemaphoresAndLocks -Wall -lpthread -lm

ReadWriteWithSemaphores:
	gcc ReadWriteWithSemaphores.c -o ReadWriteWithSemaphores -Wall -lpthread -lm

debug:
	gcc -g serie.c -o main -Wall -lpthread -lm
