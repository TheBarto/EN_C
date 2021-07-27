CC=gcc
FLAGS= -g -Wall -pedantic
SOURCES=sources/
HEADERS=headers/
OBJECTS=objects/


main: main.c $(SOURCES)moduloLecturaFicheros.c $(HEADERS)moduloLecturaFicheros.h $(SOURCES)Stack.c $(SOURCES)Queue.c $(SOURCES)LinkedList.c $(HEADERS)Stack.h $(HEADERS)Queue.h $(HEADERS)LinkedList.h $(HEADERS)defines.h
	$(CC) $(FLAGS) -o main main.c $(SOURCES)moduloLecturaFicheros.c $(SOURCES)Stack.c $(SOURCES)Queue.c $(SOURCES)LinkedList.c

#main.o: main.c $(HEADERS)moduloLecturaFicheros.h 
#	$(CC) $(CFLAGS) -c main.c -o main.o -I $(OBJECTS)

#moduloLecturaFicheros.o: $(SOURCES)moduloLecturaFicheros.c $(HEADERS)moduloLecturaFicheros.h $(SOURCES)Stack.c $(SOURCES)Queue.c $(SOURCES)LinkedList.c $(HEADERS)Stack.h $(HEADERS)Queue.h $(HEADERS)LinkedList.h $(HEADERS)defines.h
	#$(CC) $(FLAGS) -c $(SOURCES)moduloLecturaFicheros.c -o $(OBJECTS)moduloLecturaFicheros.o -I $(OBJECTS)

#modulacion.o: modulacion.c modulacion_comun.h gestorPWM.o
#	$(CC) $(FLAGS) -c modulacion.c gestorPWM.o

#gestorPWM.o: gestorPWM.c gestorPWM.h comun.h
#	$(CC) $(FLAGS) -c gestorPWM.c

.PHONY: clean
clean:
	rm *.o main
