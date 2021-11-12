CC=gcc
FLAGS= -g -Wall -Wpedantic
SOURCES=sources/
HEADERS=headers/
OBJECTS=objects/
OBJS=$(OBJECTS)main.o $(OBJECTS)LinkedList.o $(OBJECTS)Queue.o $(OBJECTS)Stack.o $(OBJECTS)moduloLecturaFicheros.o $(OBJECTS)modulacion.o $(OBJECTS)gestorADC.o $(OBJECTS)gestorPWM.o $(OBJECTS)gestorGPIO.o $(OBJECTS)driverADC.o $(OBJECTS)driverPWM.o $(OBJECTS)driverGPIO.o $(OBJECTS)comun.o $(OBJECTS)c_pinmux.o

all: drivers gestores modulaciones lecturaFicheros main.o main

lecturaFicheros: linkedList.o queue.o stack.o moduloLecturaFicheros.o

modulaciones: gestores modulacion.o

gestores: drivers gestorADC.o gestorPWM.o gestorGPIO.o

drivers: comun.o pinmux.o driverADC.o driverPWM.o driverGPIO.o

main: $(OBJS)
	$(CC) $(CFLAGS) -o main $(OBJS) -lpthread

main.o: main.c $(OBJECTS)moduloLecturaFicheros.o $(OBJECTS)modulacion.o
	$(CC) $(CFLAGS) -c main.c -o $(OBJECTS)main.o 

moduloLecturaFicheros.o: $(SOURCES)moduloLecturaFicheros.c $(HEADERS)moduloLecturaFicheros.h $(OBJECTS)Stack.o $(OBJECTS)Queue.o $(HEADERS)modulacion_comun.h
	$(CC) $(FLAGS) -c $(SOURCES)moduloLecturaFicheros.c -o $(OBJECTS)moduloLecturaFicheros.o 

stack.o: $(SOURCES)Stack.c $(HEADERS)Stack.h $(HEADERS)defines.h $(OBJECTS)LinkedList.o
	$(CC) $(FLAGS) -c $(SOURCES)Stack.c -o $(OBJECTS)Stack.o
	
queue.o: $(SOURCES)Queue.c $(HEADERS)Queue.h $(HEADERS)defines.h $(OBJECTS)LinkedList.o
	$(CC) $(FLAGS) -c $(SOURCES)Queue.c -o $(OBJECTS)Queue.o

linkedList.o: $(SOURCES)LinkedList.c $(HEADERS)LinkedList.h $(HEADERS)defines.h
	$(CC) $(FLAGS) -c $(SOURCES)LinkedList.c -o $(OBJECTS)LinkedList.o

modulacion.o: $(SOURCES)modulacion.c $(HEADERS)modulacion_comun.h $(OBJECTS)gestorPWM.o $(OBJECTS)gestorADC.o $(OBJECTS)gestorGPIO.o
	$(CC) $(FLAGS) -c $(SOURCES)modulacion.c -o $(OBJECTS)modulacion.o

gestorGPIO.o: $(SOURCES)gestorGPIO.c $(HEADERS)gestorGPIO.h $(OBJECTS)comun.o $(OBJECTS)driverGPIO.o $(OBJECTS)c_pinmux.o
	$(CC) $(FLAGS) -c $(SOURCES)gestorGPIO.c -o $(OBJECTS)gestorGPIO.o	

gestorPWM.o: $(SOURCES)gestorPWM.c $(HEADERS)gestorPWM.h $(OBJECTS)comun.o $(OBJECTS)driverPWM.o
	$(CC) $(FLAGS) -c $(SOURCES)gestorPWM.c -o $(OBJECTS)gestorPWM.o

gestorADC.o: $(SOURCES)gestorADC.c $(HEADERS)gestorADC.h $(OBJECTS)comun.o $(OBJECTS)driverADC.o
	$(CC) $(FLAGS) -c $(SOURCES)gestorADC.c -o $(OBJECTS)gestorADC.o

driverPWM.o: $(SOURCES)c_pwm.c $(HEADERS)c_pwm.h $(OBJECTS)comun.o $(OBJECTS)c_pinmux.o
	$(CC) $(FLAGS) -c $(SOURCES)c_pwm.c -o $(OBJECTS)driverPWM.o

driverADC.o: $(SOURCES)c_adc.c $(HEADERS)c_adc.h $(OBJECTS)comun.o
	$(CC) $(FLAGS) -c $(SOURCES)c_adc.c -o $(OBJECTS)driverADC.o

driverGPIO.o: $(SOURCES)event_gpio.c $(HEADERS)event_gpio.h $(OBJECTS)comun.o
	$(CC) $(FLAGS) -c $(SOURCES)event_gpio.c -o $(OBJECTS)driverGPIO.o

pinmux.o: $(SOURCES)c_pinmux.c $(HEADERS)c_pinmux.h $(OBJECTS)comun.o
	$(CC) $(FLAGS) -c $(SOURCES)c_pinmux.c -o $(OBJECTS)c_pinmux.o

comun.o: $(SOURCES)comun.c $(HEADERS)comun.h
	$(CC) $(FLAGS) -c $(SOURCES)comun.c -o $(OBJECTS)comun.o

.PHONY: clean
clean:
	rm $(OBJECTS)*.o main
