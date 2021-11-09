#ifndef GESTORPWM_H
#define GESTORPWM_H

#include "../headers/comun.h"
#include "../headers/c_pwm.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define	FRECUENCIA_DEFECTO		((float) 2000.0)
#define CICLO_TRABAJO_DEFECTO	((float) 0.0)
#define POLARIDAD_DEFECTO		((float) 0.0)

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

int8_t iniciar_puerto_PWM(const char* nombre, float cicloTrabajo, float frecuencia, float polaridad);
int8_t parar_puerto_PWM(const char* nombre);
int8_t cambiar_ciclo_trabajo(const char* nombre, float cicloTrabajo);
int8_t cambiar_frecuencia(const char* nombre, float frecuencia);

#endif
