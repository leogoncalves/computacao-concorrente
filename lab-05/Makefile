all: read_write_without_priority read_write_writer_priority

read_write_without_priority: 
	gcc read_write_without_priority.c -o without_priority -Wall -lpthread

debug_read_write_without_priority:
	gcc -g read_write_without_priority.c -o debug_without_priority -Wall -lpthread -lm

read_write_writer_priority: 
	gcc read_write_with_writer_priority.c -o writer_priority -Wall -lpthread

debug_read_write_writer_priority:
	gcc -g read_write_with_writer_priority.c -o debug_writer_priority -Wall -lpthread -lm