CC=gcc
FLAGS= -g -Wall -pedantic

main: main.c modulacion.o
	$(CC) $(FLAGS) main.c modulacion.o -o main

modulacion.o: modulacion.c modulacion_comun.h gestorPWM.o
	$(CC) $(FLAGS) -c modulacion.c gestorPWM.o

gestorPWM.o: gestorPWM.c gestorPWM.h comun.h
	$(CC) $(FLAGS) -c gestorPWM.c

.PHONY: clean
clean:
	rm *.o main