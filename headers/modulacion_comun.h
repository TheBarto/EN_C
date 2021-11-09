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

#define DEBUG_MODE

#define SIZE_WORD ((uint8_t) 20)

#define MPURO ((uint8_t) 1)
#define MREGRESION ((uint8_t) 2)
#define MMARTINELLI ((uint8_t) 3)

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

#define NUEVO	((uint8_t) 0)
#define CADENA	((uint8_t) 1)
#define NUMERO	((uint8_t) 2)

#define MAX_ELECTROVALVULAS	((uint8_t) 20)
#define MAX_TAM_NOMBRE	((uint8_t) 6)

struct Configuracion
{
	uint8_t sensorReadPin[MAX_TAM_NOMBRE];
	uint8_t sensorHeatPin[MAX_TAM_NOMBRE];
	uint8_t motorCtrlPin[MAX_TAM_NOMBRE];
	uint8_t frecCaptSamples; //Tiempo total para capturar una muestra (segundos)
	uint8_t frecCaptSamplesTH; //Tiempo total para capturar una muestra (segundos) - quitar¿?
	uint8_t VCC;
	uint8_t THReadPin[MAX_TAM_NOMBRE];
	uint8_t subSamplesForSample; //submuestras que componen una muestra
	uint8_t frecCaptSubSamples; //Tiempo total para capturar las sub muestras que componen la muestra (milisegundos)
	uint8_t totalElectrovalvulas;
	uint8_t electrovalvulas[MAX_ELECTROVALVULAS][MAX_TAM_NOMBRE];
	uint8_t resistencia; //Ponerla como un uint16_t
	uint8_t sleep; //segundos
	uint8_t sleepTH; //segundos
};

#define P_CONF_SENSOR_READ_PIN(x) (x->sensorReadPin)
#define P_CONF_SENSOR_HEAT_PIN(x) (x->sensorHeatPin)
#define P_CONF_MOTOR_CTRL_PIN(x) (x->motorCtrlPin)
#define P_CONF_FREC_CAPTURA_SAMPLES(x) (x->frecCaptSamples)
#define P_CONF_FREC_CAPTURA_SAMPLES_TH(x) (x->frecCaptSamplesTH)
#define P_CONF_VCC(x) (x->VCC)
#define P_CONF_TH_READ_PIN(x) (x->THReadPin)
#define P_CONF_SUB_SAMPLES(x) (x->subSamplesForSample)
#define P_CONF_FREC_SUB_SAMPLES(x) (x->frecCaptSubSamples)
#define P_CONF_TOTAL_VALS(x) (x->totalElectrovalvulas)
#define P_CONF_ELECTROVALVULAS(x) (x->electrovalvulas)
#define P_CONF_RESISTENCIA(x) (x->resistencia)

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

struct Cofiguracion* OLD_crear_configuracion_defecto();
void OLD_liberar_elems_struct_configuracion(struct Cofiguracion* config);

/*
	La estructura captura hace referencia a una captura completa. A UNA secuencia de odorantes,
	que tiene el mismo tiempo de analisis para cada odorante, una lista de odorantes (aire, etanol, metanol, ...),
	una succion para el motor, un tiempo para los odorantes, ...

	Cada lectura de una linea del fichero de texto donde se guardan los parametros de ejecucion tiene
	informacion acerca de una captura. TIENE TODA LA INFORMACION.
*/
typedef void(*funcion_captura)(void* captura, struct Configuracion* config, FILE* file, uint16_t nMuestra);

struct Captura
{
	uint16_t tiempoAnalisisOdor;
	uint16_t valoresCapturaExtra[20];
	uint8_t modulacion; /* 31/07/2021 -> incluimos la modulacion para tener todo junto */
	uint8_t succion;
	uint8_t temperaturaSensor;
	uint8_t totalValvulas;
	uint8_t* ordenValvulas;
	uint8_t path[500];
	uint8_t fileRoot[500]; 
	uint8_t totalValoresCapturaExtraAniadidos;
	funcion_captura funcion;
};

#define P_CAPT_MODULACION(x) (x->modulacion)
#define P_CAPT_SUCCION(x) (x->succion)
#define P_CAPT_TEMPERATURA_SENSOR(x) (x->temperaturaSensor)
#define P_CAPT_TIEMPO_ANALISIS_ODOR(x) (x->tiempoAnalisisOdor)
#define P_CAPT_TOTAL_VALVULAS(x) (x->totalValvulas)
#define P_CAPT_ORDEN_VALVULAS(x) (x->ordenValvulas)
#define P_CAPT_PATH(x) (x->path)
#define P_CAPT_FILE_ROOT(x) (x->fileRoot)
#define P_CAPT_GUARDAR_VALORES_EXTRA(x, y) (x->valoresCapturaExtra[x->totalValoresCapturaExtraAniadidos] = y)
#define P_CAPT_TOTAL_VALORES_EXTRA(x) (x->totalValoresCapturaExtraAniadidos)
#define P_CAPT_FUNCION(x)	(x->funcion)

#define TOTALVALVULAS2(x) (x.totalValvulas)

void OLD_configurar_struct_configuracion(struct Cofiguracion* sConfiguracion, uint8_t* datosConfiguracion,
	char** pelectrovalvulas, uint8_t* totalElectrovalvulas);
struct Captura* OLD_inicializar_structs_capturas(uint16_t* totalMuestras, uint8_t* suctions,
		uint8_t* temperaturasSensor, int16_t* tiemposAnalisisOdor, uint8_t** ordenValvulas);
void OLD_liberar_structs_capturas(struct Captura* capturas, uint16_t* totalMuestras);

/*
	La estructura Puro hace referencia al total de las multiples secuencias de captura con esta modulacion. 
	Cada una de las secuencias se lleva a cabo con una struct captura (UN FICHERO). La idea es programar varias secuencias
	en diferentes capturas y analizarlas despues. Las capturas se diferencian por las comas(,) de separacion
*/

//typedef void (*funcion_captura)(void*);

struct Puro{
	struct Cofiguracion* configuracion;
	uint16_t resistencia;
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
#define FUNCIONCAPTURA(x) (x->funcion)
#define TOTALCAPTURAS(x) (x->pTotalCapturas)

struct Puro* OLD_inicializar_struct_Puro(uint16_t resistencia, char* path, struct Cofiguracion* configuracion,
	uint16_t* totalMuestras, uint8_t* suctions, uint8_t* temperaturasSensor, 
	int16_t* tiemposAnalisisOdor, uint8_t** ordenValvulas, funcion_captura func);

struct Puro* OLD_inicializar_struct_Puro_struct(uint16_t resistencia, char* path, struct Cofiguracion* configuracion,
	uint16_t* totalMuestras, funcion_captura func, struct Captura* captura);

void captura_secuencia_odorantes_completa_puro(void* capt, struct Configuracion* config, FILE* file, uint16_t nMuestra);
void captura_secuencias_completas_puro(struct Captura* captura, struct Configuracion* config);

void OLD_captura_secuencia_odorantes(struct Captura* captura, struct Cofiguracion* config);

void OLD_liberar_struct_Puro(struct Puro* pMPuro);

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

struct Regresion* OLD_inicializar_struct_Regresion(uint16_t resistencia, uint8_t* path, struct Captura* capturas,
	struct Cofiguracion* configuracion);


void inicializar_fichero_puertos(uint8_t* succion, uint8_t* pinMotor, char* path, FILE* f);
void cierre(void* structModulacion, uint8_t tipo, FILE* f);
void abrir_electrovalvulas(char** p_total_electrovalvulas, 
	uint8_t* p_electrovalvulas_seleccionadas, 
	uint8_t electrovalvula);

void abrir_electrovalvula(uint8_t p_electrovalvulas[MAX_ELECTROVALVULAS][MAX_TAM_NOMBRE],
		uint8_t electrovalvula);

void cerrar_electrovalvulas(uint8_t p_electrovalvulas[MAX_ELECTROVALVULAS][MAX_TAM_NOMBRE],
		uint8_t total_electro);

void cierreDescriptoresAbiertos(struct Configuracion* conf, FILE* file);

void activar_puertos_GPIO(uint8_t** GPIOs, uint8_t* total_GPIOs);
void captura_muestras_datos(uint8_t* tipo, void* structModulacion);


#endif
