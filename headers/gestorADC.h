/*
 * gestorADC.h
 *
 *  Created on: 12 oct. 2021
 *      Author: afuenteo
 */

#ifndef HEADERS_GESTORADC_H_
#define HEADERS_GESTORADC_H_

#include "comun.h"
#include "c_adc.h"


uint8_t obtener_numero_ADC(const char* nombre, uint8_t* adc);
void inicializar_puertos_ADC();
float leer_valor_ADC();
float leer_valor_crudo_ADC();

#endif /* HEADERS_GESTORADC_H_ */
