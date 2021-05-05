#ifndef MODULOLECTURAFICHEROS_H
#define MODULOLECTURAFICHEROS_H

#define VALOR_COMA ((uint8_t) 44) //','
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

#define VALOR_LLAVE_CERRADA	((uint8_t) 0xAA)
#define VALOR_RESET_LLAVE_CERRADA	((uint8_t) 0x55)

char cabeceraLinea[] = {"EN_CAPTURE"};



void* aperturaFichero(const char* nombre);

void procesarContenidoFichero(char* contenido);

void obtenerLineaFichero(char* contenido, char* linea);

#endif