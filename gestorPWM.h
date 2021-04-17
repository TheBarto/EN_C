#ifndef GESTORPWM_H
#define GESTORPWM_H

#include "comun.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct PWM
{
	uint16_t frecuencia;
	char nombre[20];
	uint8_t polaridad;
	uint8_t cicloTrabajo;
}PWM;

#define PFRECUENCIA(x) (x->frecuencia)
#define PNOMBRE(x) (x->nombre)
#define PPOLARIDAD(x) (x->polaridad)
#define PCICLOTRABAJO(x) (x->cicloTrabajo)


int8_t obtenerClavePWM(const char* nombre, char* clavePuerto);

int8_t iniciar_puerto_PWM(const char* nombre, uint16_t* frecuencia, uint8_t* polaridad, uint8_t* cicloTrabajo);
int8_t parar_puerto_PWM(const char* nombre);
int8_t cambiar_ciclo_trabajo(const char* nombre, uint8_t* cicloTrabajo);
int8_t cambiar_frecuencia(const char* nombre, uint8_t* frecuencia);

#endif