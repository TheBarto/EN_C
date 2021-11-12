/*
 * gestorADC.c
 *
 *  Created on: 12 oct. 2021
 *      Author: afuenteo
 */

#include "../headers/gestorADC.h"

uint8_t obtener_numero_ADC(const char* nombre, uint8_t* adc)
{

	for(pins_t* pPins = &table[0]; pPins->name; pPins++)
	{

		char* pin_name = (char *)pPins->name;
		char* name = (char *)nombre;

		while((*pin_name != 0) && (*name != 0) && (*name == *pin_name))
		{
			name++;
			pin_name++;
		}

		if(!*name && !*pin_name)
		{
			*adc = pPins->ain;
			return OK;
		}
		else
		{
			pin_name = (char *)pPins->key;
			name = (char *)nombre;

			while((*pin_name != 0) && (*name != 0) && (*name == *pin_name))
			{
				name++;
				pin_name++;
			}

			if(!*name && !*pin_name)
			{
				*adc = pPins->ain;
				return OK;
			}
		}
	}

	return E_ARG_INVALIDO;
}

/* Inicializamos todos los puertos ADC */
void inicializar_puertos_ADC()
{

	if(adc_setup() != OK)
	{
		printf("Error al inicializar los puertos ADC del sistema. Las posibles causas son: \n");
		printf("\t - Una capa hardware que entre en conflicto con los mapeos de los pines cargados\n");
		printf("\t - Un objeto arbol de dispositivos cargado con el mismo nombre para un fragmento: helper\n");
	}

	return;
}

float leer_valor_ADC(uint8_t* puerto)
{
	float valor = leer_valor_crudo_ADC(puerto);

	//Modificador del valor segun el tipo de hardware
#ifdef BBBVERSION41
	valor /= 4095.0;
#else
	valor /= 1800.0;
#endif

	return valor;
}

float leer_valor_crudo_ADC(uint8_t* puerto)
{
	uint8_t valor_puerto = 0;
	float valor_leido = 0;

	if(!adc_initialized)
	{
		printf("Primero se debe de inicializar los puertos ADC antes de operar con ellos.\n");
	}

	if(obtener_numero_ADC((char *)puerto, &valor_puerto) != E_ARG_INVALIDO)
	{
		printf("Puerto erroneo.\n");
	}

	if(read_value(valor_puerto, &valor_leido) != OK)
	{
		printf("Error al leer del puerto. Puerto invalido o bloqueado.\n");
	}

	return valor_leido;
}
