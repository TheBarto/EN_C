#ifndef MODULOLECTURAFICHEROS_H
#define MODULOLECTURAFICHEROS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "Stack.h"
#include "Queue.h"
#include "modulacion_comun.h"

#define VALOR_COMA ((uint8_t) 44) //','
#define SEPARADOR_VALVULAS	((uint8_t) 45) // '-'
#define VALOR_CIERRE_PARENTESIS ((uint8_t) 41) //')'
#define VALOR_APERTURA_PARENTESIS ((uint8_t) 40) //'('
#define VALOR_SALTO_LINEA	((uint8_t) 10) //\n
#define VALOR_CIERRE_LLAVE	((uint8_t) 125) //'}'
#define VALOR_APERTURA_LLAVE	((uint8_t) 123) //'{'
#define VALOR_NUMERO_0	((uint8_t) 48) //'0'
#define VALOR_NUMERO_9	((uint8_t) 57) //'9'

#define POSICION_MODULACION				((uint8_t) 0)
#define POSICION_SUCCION				((uint8_t) 1)
#define POSICION_DURACION_ODORANTE		((uint8_t) 2)
#define POSICION_ORDEN_VALVULAS			((uint8_t) 3)
#define POSICION_TEMPERATURA_SENSOR		((uint8_t) 4)
#define POSICION_RAIZ_NOMBRE_FICHERO	((uint8_t) 5)
#define POSICION_NOMBRE_CARPETA			((uint8_t) 6)

#define IDENTIFICADOR_APERTURA_LLAVE	((uint8_t) 0xFF)

#define VALOR_LLAVE_CERRADA	((uint8_t) 0x01)
#define VALOR_NUMERO_LEIDO	((uint8_t) 0x02)
#define VALOR_RESET_LLAVE_CERRADA	((uint8_t) 0x55)


#define SALIDA_OK	((uint8_t) 0)
#define ERROR	((uint8_t) 0xFF)
#define E_MAL_FORMADO	((uint8_t) 1)
#define E_VALORES_INCORRECTOS	((uint8_t) 2)
#define FIN_FICHERO	((uint8_t) 3)

#define TOTAL_DATA_READED_FILE	((uint16_t) 5000)

#define SENSOR_READ_PIN	((uint8_t) 0)
#define SENSOR_HEAT_PIN	((uint8_t) 1)
#define RESISTENCIA_VALUE	((uint8_t) 2)
#define FREC_CAPT_SAMPLES	((uint8_t) 3)
#define SUB_SAMPLES_SAMPLE	((uint8_t) 4)
#define FREC_CAPT_SUB_SAMPLES	((uint8_t) 5)
#define VCC_VALUE	((uint8_t) 6)
#define TH_READ_PIN	((uint8_t) 7)
#define FREC_CAPT_SAMPLES_TH	((uint8_t) 8)
#define MOTOR_CTRL_PIN	((uint8_t) 9)
#define N_ELECTROVALVULAS	((uint8_t) 10)
#define TOTAL_ELECTROVALVULAS	((uint8_t) 11)


//Variable global con el valor que indica el inicio de linea
extern char cabeceraLinea[20];
extern char cabeceraConf[20];

void aperturaFichero(const char* nombre, char* data);

void procesarContenidoFichero(char* contenido);

void obtenerLineaFichero(char* contenido, char* linea);

uint8_t obtenerSecuenciaCapturaDelFichero(const char* data, uint16_t* totalDatosLeidos, struct Captura* captura);

uint8_t procesarInformacion(uint32_t* valor, uint8_t posicionLinea, struct Captura* captura);

void procesarUltimoElementoPila(Stack* stack, uint32_t valor);

void procesarGrupoPila(Stack* stack, uint32_t valor);

void procesarUltimoElementoPila(Stack* stack, uint32_t valor);

void cargarFuncionCapturaOdorantes(uint8_t* modulacion, struct Captura* captura);

uint8_t procesarCadenaAperturaValvulas(uint8_t* cadenaApertura, uint8_t** dstArray, uint8_t* total_dstArray, uint8_t numElemsCadenaApertura);

uint8_t obtenerSecuenciaConfiguracion(const char* data, uint16_t* totalDatosLeidos, struct Configuracion* conf);
#endif
