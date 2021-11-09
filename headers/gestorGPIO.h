/*
 * gestorGPIO.h
 *
 *  Created on: 12 oct. 2021
 *      Author: afuenteo
 */

#include "../headers/comun.h"
#include "../headers/event_gpio.h"
#include "../headers/c_pinmux.h"

#ifndef HEADERS_GESTORGPIO_H_
#define HEADERS_GESTORGPIO_H_

#define RETRASO_GPIO_DEFECTO	((uint8_t) 0)
#define PULL_UP_DOWN_DEFECTO	((uint8_t) PUD_OFF)
#define INICIAL_DEFECTO			((uint8_t) 0)


uint8_t obtener_numero_gpio(const char* nombre, uint16_t* gpio);

void iniciar_GPIO(const char* canal, uint8_t direccion, uint8_t pull_up_down, uint16_t espera, uint8_t inicial);
void limpiar_GPIO();
void salida_GPIO(const char* canal, uint8_t value);
uint32_t entrada_GPIO(const char* canal);
/*
void anadir_deteccion_evento_GPIO();
void eliminar_deteccion_evento_GPIO();
void anadir_llamada_funcion_evento_GPIO();
void espera_cambio_GPIO();
void funcion_GPIO();
 */
void fijar_aviso_GPIO();

#endif /* HEADERS_GESTORGPIO_H_ */
