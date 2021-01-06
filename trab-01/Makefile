all: build_montecarlo_thread build_montecarlo_sequencial build_leibniz_serie_approximate_pi

test: build
	@./main

build_montecarlo_thread: 
	gcc montecarlo_thread.c -o montecarlo_thread -Wall -lpthread -lm

debug_montecarlo_thread:
	gcc -g montecarlo_thread.c -o montecarlo_thread_debug -Wall -lpthread -lm

build_montecarlo_sequencial: 
	gcc montecarlo_sequencial.c -o montecarlo_sequencial -Wall -lpthread -lm

debug_montecarlo_sequencial:
	gcc -g montecarlo_sequencial.c -o montecarlo_sequencial_debug -Wall -lpthread -lm

build_leibniz_serie_approximate_pi: 
	gcc leibniz_serie_approximate_pi.c -o leibniz_pi -Wall -lpthread -lm

debug_leibniz_serie_approximate_pi:
	gcc -g leibniz_serie_approximate_pi.c -o leibniz_pi_debug -Wall -lpthread -lm