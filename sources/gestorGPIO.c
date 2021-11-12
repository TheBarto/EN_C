/*
 * gestorGPIO.c
 *
 *  Created on: 12 oct. 2021
 *      Author: afuenteo
 */

#include "../headers/gestorGPIO.h"

uint8_t inicializar_modulo()
{

	for(uint8_t i = 0; i < 120; i++)
	{
		direccion_GPIO[i] = -1;
	}

	configuracion_modulo = 1;

	return 0;
}

/* Buscamos por nombre y clave */
uint8_t obtener_numero_gpio(const char* nombre, uint16_t* gpio)
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
			*gpio = pPins->gpio;
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
				*gpio = pPins->gpio;
				return OK;
			}
		}
	}

	return E_ARG_INVALIDO;
}

void iniciar_GPIO(const char* canal, uint8_t direccion, uint8_t pull_up_down, uint16_t espera, uint8_t inicial)
{

	uint16_t gpio = 0;
	uint8_t pud = PUD_OFF;
	uint8_t ret = -1;

	if(!configuracion_modulo)
	{
		inicializar_modulo();
	}


	if((direccion != INPUT) && (direccion != OUTPUT))
	{
		printf("Direccion indicada a la funcion es incorrecta. Tiene que ser ENTRADA o SALIDA.\n");
		return;
	}

	if(direccion == OUTPUT)
	{
		pud = PUD_OFF;
	}

	if((pud != PUD_OFF) && (pud != PUD_DOWN) && (pud != PUD_UP))
	{
		printf("Valor inválido para el modo del puerto GPIO. El estado debe ser PUD_UP (ALTO), PUD_DOWN (BAJO) o PUD_OFF (APAGADO).\n");
		return;
	}

	if(obtener_numero_gpio(canal, &gpio) != OK)
	{
		return;
	}

	gpio_export(gpio);

	/* Este retraso es para dar tiempo al sistema a fijar los permisos de los ficheros
	 * para poder usar el puerto GPIO. */
	if(espera > 0)
	{
#ifdef __unix__
		nanosleep((struct timespec[]){{0, espera*1000000L}}, NULL); //dormimos nanosegundos. El valor de espera en milisegundos.
#endif
	}

	if(gpio_set_direction(gpio, direccion) != 0)
	{
		printf("Error al fijar la dirección del GPIO (ENTRADA o SALIDA). Falta un archivo o los permisos son insuficientes.\n");
		return;
	}

	if(direccion == OUTPUT)
	{
		if(gpio_set_value(gpio, inicial) != OK)
		{
			printf("Error al fijar el valor al GPIO (ENTRADA o SALIDA). Falta un archivo o los permisos son insuficientes.\n");
			return;
		}
	}
	else
	{
		if(pud == PUD_DOWN)
		{
			ret = set_pin_mode(canal, "gpio_pd");
		}
		else if(pud == PUD_UP)
		{
			ret = set_pin_mode(canal, "gpio_pu");
		}
		else
		{
			ret = set_pin_mode(canal, "gpio");
		}
	}

	if(ret != OK)
	{
		printf("Error al fijar el modo al GPIO (PUD_UP, PUD_DOWN o PUD_OFF). Falta un archivo o los permisos son insuficientes.\n");
		return;
	}

	direccion_GPIO[gpio] = direccion;

	return;
}

void limpiar_GPIO()
{
	event_cleanup();

	return;
}

void salida_GPIO(const char* canal, uint8_t value)
{

	uint16_t gpio = 0;

	if(obtener_numero_gpio(canal, &gpio) != OK)
	{
		return;
	}

	if((!configuracion_modulo) || (direccion_GPIO[gpio] != OUTPUT))
	{
		printf("El puerto GPIO no se ha establecido como un puerto de salida (OUTPUT).\n");
		return;
	}

	gpio_set_value(gpio, value);

	return;

}

uint32_t entrada_GPIO(const char* canal)
{

	uint16_t gpio = 0;
	uint32_t valor = 0xFFFFFFFF;

	if(obtener_numero_gpio(canal, &gpio) != OK)
	{
		return valor;
	}

	if((!configuracion_modulo) || ((direccion_GPIO[gpio] != OUTPUT) && (direccion_GPIO[gpio] != INPUT)))
	{
		printf("El puerto GPIO no se ha inicializado. Inicialice el puerto primero.\n");
		return valor;
	}

	gpio_set_value(gpio, valor);

	return valor;
}

void fijar_aviso_GPIO()
{

	if(error_conf)
	{
		printf("Error al importar el modulo.\n");
		return;
	}

	return;
}

