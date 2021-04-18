#ifndef MODULACION_COMUN_H
#define MODULACION_COMUN_H

//#include "tipos.h"
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>

//#include "comun.h"

//#define DEBUG_MODE 1

#define SIZE_WORD ((uint8_t) 20)

#define MPURO ((uint8_t) 0)
#define MREGRESION ((uint8_t) 1)
#define MMARTINELLI ((uint8_t) 2)

#define NMUESTRASCAPTURA(x) (x->nMuestrasCaptura)
#define SLEEP(x) (x->sleep)
#define SLEEPTH(x) (x->sleepTH)
#define TNMUESTRASCAPTURA(x) (x->tEntreNMuestrasCaptura)
#define VCC(x) (x->VCC)
#define PINMOTOR(x) (x->pinMotor)
#define PINTH(x) (x->pinTH)
#define PINVOLTAJESENSOR(x) (x->pinVoltajeSensor)
#define PINLECTURASENSOR(x) (x->pinLecturaSensor)
#define ELECTROVALVULASTOTAL(x) (x->electrovalvulas)

#define ASIG_NMUESTRASCAPTURA(x, y) (x->nMuestrasCaptura = y)
#define ASIG_SLEEP(x, y) (x->sleep = y)
#define ASIG_SLEEPTH(x, y) (x->sleepTH = y)
#define ASIG_TNMUESTRASCAPTURA(x, y) (x->tEntreNMuestrasCaptura = y)
#define ASIG_VCC(x, y) (x->VCC = y)
#define ASIG_PINMOTOR(x, y) (x->pinMotor = y)
#define ASIG_PINTH(x, y) (x->pinTH = y)
#define ASIG_PINVOLTAJESENSOR(x, y) (x->pinVoltajeSensor = y)
#define ASIG_PINLECTURASENSOR(x, y) (x->pinLecturaSensor = y)

struct Cofiguracion
{
	uint8_t nMuestrasCaptura; // total de sub-capturas que haran una captura completa
	uint8_t sleep; //segundos
	uint8_t sleepTH; //segundos
	uint8_t tEntreNMuestrasCaptura; //milisegundos
	uint8_t VCC;
	uint8_t pinMotor;
	uint8_t pinTH;
	uint8_t pinVoltajeSensor;
	uint8_t pinLecturaSensor;
	char** electrovalvulas;//[20][10];
	uint8_t totalElectrovalvulas;
};

#define INI_CONFIGURACION_STRUCT(pconfiguracion_struct) \
	NMUESTRASCAPTURA(pconfiguracion_struct) = 10; \
	SLEEP(pconfiguracion_struct) = 1; \
	SLEEPTH(pconfiguracion_struct) = 60; \
	TNMUESTRASCAPTURA(pconfiguracion_struct) = 10; \
	VCC(pconfiguracion_struct) = 5; \
	PINMOTOR(pconfiguracion_struct) = 0; \
	PINTH(pconfiguracion_struct) = 0; \
	PINVOLTAJESENSOR(pconfiguracion_struct) = 0; \
	PINLECTURASENSOR(pconfiguracion_struct) = 0; \

struct Cofiguracion* crear_configuracion_defecto();
void liberar_elems_struct_configuracion(struct Cofiguracion* config);

/*
	La estructura captura hace referencia a una captura completa. A UNA secuencia de odorantes,
	que tiene el mismo tiempo de analisis para cada odorante, una lista de odorantes (aire, etanol, metanol, ...),
	una succion para el motor, un tiempo para los odorantes, ...
*/
struct Captura
{
	uint8_t succion;
	uint8_t temperaturaSensor;
	uint16_t tiempoAnalisisOdor;
	uint8_t totalValvulas;
	uint8_t* ordenValvulas;
};

#define SUCCION(x) (x->succion)
#define TEMPERATURASENSOR(x) (x->temperaturaSensor)
#define TIEMPOANALISISODOR(x) (x->tiempoAnalisisOdor)
#define TOTALVALVULAS(x) (x->totalValvulas)
#define ORDENVALVULAS(x) (x->ordenValvulas)

#define TOTALVALVULAS2(x) (x.totalValvulas)

void configurar_struct_configuracion(struct Cofiguracion* sConfiguracion, uint8_t* datosConfiguracion, 
	char** pelectrovalvulas, uint8_t* totalElectrovalvulas);
struct Captura* inicializar_structs_capturas(uint16_t* totalMuestras, uint8_t* suctions,
	uint8_t* temperaturasSensor, int16_t* tiemposAnalisisOdor, uint8_t** ordenValvulas);
void liberar_structs_capturas(struct Captura* capturas, uint16_t* totalMuestras);

/*
	La estructura Puro hace referencia al total de las multiples secuencias de captura con esta modulacion. 
	Cada una de las secuencias se lleva a cabo con una struct captura (UN FICHERO). La idea es programar varias secuencias
	en diferentes capturas y analizarlas despues. Las capturas se diferencian por las comas(,) de separacion
*/

typedef void (*funcion_captura)(void*);

struct Puro{
	struct Cofiguracion* configuracion;
	uint16_t resistencia;
	uint8_t path[500];
	uint16_t pTotalCapturas;
	struct Captura* capturas;
	FILE* f;
	funcion_captura funcion;
};

#define SCONFIGURACION(x) (x->configuracion)
#define RESISTENCIA(x) (x->resistencia)
#define PATH(x) (x->path)
#define SCAPTURAS(x) (x->capturas)
#define SCAPTURASPOSI(x, i) (x->capturas[i])
#define FILE(x) (x->f)
#define FUNCIONCAPTURA(x, y) (x->funcion(y))
#define TOTALCAPTURAS(x) (x->pTotalCapturas)

struct Puro* inicializar_struct_Puro(uint16_t resistencia, char* path, struct Cofiguracion* configuracion,
	uint16_t* totalMuestras, uint8_t* suctions, uint8_t* temperaturasSensor, 
	int16_t* tiemposAnalisisOdor, uint8_t** ordenValvulas, funcion_captura func);

void captura_secuencia_odorantes_completa_puro(struct Captura* captura, struct Cofiguracion* config, uint16_t resistencia);
void captura_secuencias_completas_puro(void* stru);

void liberar_struct_Puro(struct Puro* pMPuro);

#define TENDENCIA(x) (x->tendencia)
#define VENTANA(x) (x->ventana)

struct Regresion{
	struct Cofiguracion* configuracion;
	uint16_t resistencia;
	uint16_t tendencia;
	uint16_t* ventana;
	uint8_t path[500];
	struct Captura* capturas;
	FILE* f;
};

struct Regresion* inicializar_struct_Regresion(uint16_t resistencia, uint8_t* path, struct Captura* capturas, 
	struct Cofiguracion* configuracion);


void inicializar_fichero_puertos(uint8_t* succion, uint8_t* pinMotor, char* path, FILE* f);
void cierre(void* structModulacion, uint8_t tipo, FILE* f);
void abrir_electrovalvulas(char** p_total_electrovalvulas, 
	uint8_t* p_electrovalvulas_seleccionadas, 
	uint8_t electrovalvula);
void cerrar_electrovalvulas(char** p_total_electrovalvulas, 
	uint8_t* p_electrovalvulas_seleccionadas, 
	uint8_t electrovalvula);
void activar_puertos_GPIO(uint8_t** GPIOs, uint8_t* total_GPIOs);
void captura_muestras_datos(uint8_t* tipo, void* structModulacion);


#endif