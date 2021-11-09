#include <stdio.h>
#include <stdlib.h>
#include "headers/moduloLecturaFicheros.h"

//#define BUSCADORES

#ifdef BUSCADORES
#include "headers/gestorPWM.h"
#include "headers/gestorADC.h"
#include "headers/gestorGPIO.h"
#endif

#define VRESISTENCIA ((uint16_t) 2200)
#define VTOTALMUESTRAS ((uint16_t) 2)
#define VVALVULASPORCAPTURA ((uint8_t) 5)

int main(int arc, char* argv[])
{

	setbuf(stdout, NULL);

	//Pruebas buscadores de puertos
#ifdef BUSCADORES

	char clave[15] = {0};

	//PWM
	printf(">>>> PWM <<<<\n");

	obtenerClavePWM("P9_21", &clave[0]);
	printf("> P9_21 - %s\n", clave);

	obtenerClavePWM("UART2_TXD", &clave[0]);
	printf("> UART2_TXD - %s\n", clave);

	obtenerClavePWM("P8_37", &clave[0]);
	printf("> P8_37 - %s\n", clave);

	obtenerClavePWM("UART5_TXD", &clave[0]);
	printf("> UART5_TXD - %s\n", clave);

	obtenerClavePWM("P8_9", &clave[0]);
	printf("> P8_9 - %s\n", clave);

	obtenerClavePWM("TIMER5", &clave[0]);
	printf("> TIMER5 - %s\n", clave);

	//ADC
	printf(">>>> ADC <<<<\n");
	uint8_t adc = 0;

	obtener_numero_ADC("AIN1", &adc);
	printf("> AIN1 - %d\n", adc);
	obtener_numero_ADC("P9_40", &adc);
	printf("> P9_40 - %d\n", adc);

	obtener_numero_ADC("AIN2", &adc);
	printf("> AIN2 - %d\n", adc);
	obtener_numero_ADC("P9_37", &adc);
	printf("> P9_37 - %d\n", adc);

	obtener_numero_ADC("AIN6", &adc);
	printf("> AIN6 - %d\n", adc);
	obtener_numero_ADC("P9_35", &adc);
	printf("> P9_35 - %d\n", adc);

	//GPIO
	printf(">>>> GPIO <<<<\n");
	uint16_t gpio = 0;

	obtener_numero_gpio("GPIO1_14", &gpio);
	printf("> GPIO1_14 - %d\n", gpio);
	obtener_numero_gpio("P8_16", &gpio);
	printf("> P8_16 - %d\n", gpio);

	obtener_numero_gpio("GPIO1_4", &gpio);
	printf("> GPIO1_4 - %d\n", gpio);
	obtener_numero_gpio("P8_23", &gpio);
	printf("> P8_23 - %d\n", gpio);

	obtener_numero_gpio("GPIO3_19", &gpio);
	printf("> GPIO3_19 - %d\n", gpio);
	obtener_numero_gpio("P9_27", &gpio);
	printf("> P9_27 - %d\n", gpio);

	return 1;
#endif

	//Pruebas de la lectura de ficheros

	char name1[20] = "pruebaConf.txt";// "pruebaLectura.txt";
	char name2[30] = "pruebaLecturaPuro.txt";
	//uint8_t modulacion = 0; //MPURO;
	struct Captura captura;
	struct Configuracion conf;
	char data[TOTAL_DATA_READED_FILE];

	aperturaFichero(name1, data);
	//printf("Direccion de memoria: %p\n", (void *)file);
	uint16_t totalDatosLeidos = 0;

	obtenerSecuenciaConfiguracion(data, &totalDatosLeidos, &conf);
	struct Configuracion* pconf = &conf;


	printf("El resultado 1 obtenido:\n");

	printf("MODULACION Y VALOR\n");
	printf("Pin lectura sensor: %s\n", pconf->sensorReadPin);
	printf("Pin calentamiento sensor: %s\n", pconf->sensorHeatPin);
	printf("Pin control motor: %s\n", pconf->motorCtrlPin);
	printf("Frecuencia captura muestras: %d\n", pconf->frecCaptSamples);
	printf("Frecuencia captura muestras TH: %d\n", pconf->frecCaptSamplesTH);

	printf("VCC: %d\n", pconf->VCC);
	printf("Pin lectura TH: %s\n", pconf->THReadPin);
	printf("Sub muestras por muestras: %d\n", pconf->subSamplesForSample);
	printf("Frecuencia captura sub muestras por muestras: %d\n", pconf->frecCaptSubSamples);
	printf("Resistencia: %d\n", pconf->resistencia);

	printf("Electrovalvulas: %d\n", pconf->totalElectrovalvulas);
	for(int i = 0; i < pconf->totalElectrovalvulas; i++)
		printf("%s - ", pconf->electrovalvulas[i]);
	printf("\n");

	aperturaFichero(name2, data);
	totalDatosLeidos = 0;
	obtenerSecuenciaCapturaDelFichero(data, &totalDatosLeidos, &captura);
	struct Captura* pcaptura = &captura;

	printf("\nEl resultado 2 obtenido:\n");

	printf("MODULACION Y VALOR\n");
	printf("SUCCION: %d\n", P_CAPT_SUCCION(pcaptura));
	printf("TEMPERATURA SENSOR: %d\n", P_CAPT_TEMPERATURA_SENSOR(pcaptura));
	printf("TIEMPO ANALISIS ODOR: %d\n", P_CAPT_TIEMPO_ANALISIS_ODOR(pcaptura));
	printf("PATH: %s\n", P_CAPT_PATH(pcaptura));
	printf("FILEROOT: %s\n", P_CAPT_FILE_ROOT(pcaptura));

	printf("TOTAL VALVULAS: %d\n", P_CAPT_TOTAL_VALVULAS(pcaptura));
	
	for(int i = 0; i < P_CAPT_TOTAL_VALVULAS(pcaptura); i++)
		printf("%d - ", P_CAPT_ORDEN_VALVULAS(pcaptura)[i]);
	printf("\n");

	printf("Tendencia: %d\n", pcaptura->valoresCapturaExtra[0]);
	printf("HeatSensor: %d\n", pcaptura->valoresCapturaExtra[1]);

	printf("\n\nComenzamos captura de prueba\n\n");

	captura_secuencias_completas_puro(&captura, &conf);

	/*obtenerSecuenciaCapturaDelFichero(data, &totalDatosLeidos, &captura);

	printf("El resultado 2 obtenido:\n");

	printf("MODULACION Y VALOR\n");
	printf("SUCCION: %d\n", SUCCION(pcaptura));
	printf("TEMPERATURA SENSOR: %d\n", TEMPERATURASENSOR(pcaptura));
	printf("TIEMPO ANALISIS ODOR: %d\n", TIEMPOANALISISODOR(pcaptura));
	printf("PATH: %s\n", PATH(pcaptura));
	printf("FILEROOT: %s\n", FILEROOT(pcaptura));

	printf("TOTAL VALVULAS: %d\n", TOTALVALVULAS(pcaptura));

	for(int i = 0; i < TOTALVALVULAS(pcaptura); i++)
		printf("%d - ", ORDENVALVULAS(pcaptura)[i]);
	printf("\n");*/
	//printf("TIEMPO ANALISIS ODOR: %d\n", );
	//printf("TOTAL VALVULAS: %d\n");	

	//El directorio, o ruta global, o a partir de donde se encuentra el executable,
	//sino dara error -> IMPORTANTE
	/*char path[500];
	sprintf(&path[0], "Pruebas%c",0);

	uint16_t totalMuestras = VTOTALMUESTRAS;

	uint8_t suctions[totalMuestras];
	uint8_t temperaturasSensor[totalMuestras];
	int16_t tiemposAnalisisOdor[totalMuestras];

	uint8_t** ordenValvulas;

	ordenValvulas = (uint8_t**)malloc(VTOTALMUESTRAS*sizeof(uint8_t*));

	for(uint8_t i = 0; i < VTOTALMUESTRAS; i++)
	{
		*(ordenValvulas+i) = (uint8_t*)malloc((VVALVULASPORCAPTURA+1)*sizeof(uint8_t));
	}


	for(uint8_t i = 0; i < VTOTALMUESTRAS; i++)
	{
		*(suctions+i) = i;
		*(temperaturasSensor+i) = i;
		*(tiemposAnalisisOdor+i) = i;

		ordenValvulas[i][0] = VVALVULASPORCAPTURA;

		//printf("Valores en vuelta: %d: succion: %u, tempSensor: %d, tiempoAna: %d, totalValvulas: %d\n",
		//	i, suctions[i], temperaturasSensor[i], tiemposAnalisisOdor[i], ordenValvulas[i][0]);

		//printf("1.0 ordenValvulas: %p, ordenValvulas[%d]: %p\n", (void *)ordenValvulas, i, (void*)ordenValvulas[i]);

		for(uint8_t j = 1; j < (VVALVULASPORCAPTURA+1); j++)
		{
			ordenValvulas[i][j] = (j-1);
			//printf("1.01 ordenValvulas: %p, ordenValvulas[%d][%d]: %p\n", (void *)ordenValvulas, i, j, (void*)&ordenValvulas[i][j]);
		}
	}*/

	//uint8_t **pOrdenValvulas = (uint8_t**)ordenValvulas;

	/*struct Cofiguracion* pConf = crear_configuracion_defecto();

	uint8_t totalElectrovalvulas = 20;

	uint8_t datosConfiguracion[10] = {10, 1, 60, 10, 5, 32, 30, 31, 40, 0};*/

	/*char puertosGPIO[20][10] = {"GPIO_1", "GPIO_2", "GPIO_3", "GPIO_4", "GPIO_5", 
		"GPIO_6", "GPIO_7", "GPIO_8", "GPIO_9", "GPIO_10", "GPIO_11", "GPIO_12", "GPIO_13", 
		"GPIO_14", "GPIO_15","GPIO_16", "GPIO_17", "GPIO_18", "GPIO_19", "GPIO_20"};*/

	/*char** puertosGPIO = (char **)malloc(20*sizeof(*puertosGPIO));

	for(uint8_t i = 0; i < 20; i++)
	{
		puertosGPIO[i] = (char *)malloc(10*sizeof(*puertosGPIO[i]));
		sprintf(&puertosGPIO[i][0], "GPIO_%02d%c", (i+1), 0);

		//printf("%s\n",puertosGPIO[i]);
	}*/
	
	/*for(uint8_t i = 0; i < 20; i++)
	{
		printf("puertosGPIO ARRAY ESTATICO: %p\n", (void *)puertosGPIO[i]);
		for(int j = 0; j < 10; j++)
			printf("puertosGPIO,%d,%d: %p\n", i,j,(void *)&puertosGPIO[i][j]);
	}*/

	/*for(uint8_t i = 0; i < VTOTALMUESTRAS; i++)
	{
		printf("ordenValvulas ARRAY DINAMICO: %p\n", (void *)ordenValvulas[i]);
		for(int j = 0; j < (VVALVULASPORCAPTURA+1); j++)
			printf("ordenValvulas,%d,%d: %p\n", i,j,(void *)&ordenValvulas[i][j]);
	}*/

	/*char clave[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	char nombre[20];
	sprintf(nombre, "AIN74%c", 0);*/

	//obtenerClavePWM(nombre, clave);
	
	/*configurar_struct_configuracion(pConf, datosConfiguracion, puertosGPIO, &totalElectrovalvulas);

	struct Puro* pMPuro = inicializar_struct_Puro(VRESISTENCIA, path, pConf, &totalMuestras, suctions,
		temperaturasSensor, tiemposAnalisisOdor, ordenValvulas, &captura_secuencias_completas_puro);
	
	uint8_t tipo = 0;
	captura_muestras_datos(&tipo, (void*)pMPuro);

	liberar_struct_Puro(pMPuro);

	for(uint8_t i = 0; i < 20; i++)
	{
		free(puertosGPIO[i]);
		puertosGPIO[i] = NULL;
	}

	free(puertosGPIO);
	puertosGPIO = NULL;

	for(uint8_t i = 0; i < VTOTALMUESTRAS; i++)
	{
		free(*(ordenValvulas+i));
		*(ordenValvulas+i) = NULL;
	}

	free(ordenValvulas);
	ordenValvulas = NULL;*/
}
