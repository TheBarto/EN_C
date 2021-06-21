#include <stdio.h>
#include <stdlib.h>
#include "moduloLecturaFicheros.h"

#define VRESISTENCIA ((uint16_t) 2200)
#define VTOTALMUESTRAS ((uint16_t) 2)
#define VVALVULASPORCAPTURA ((uint8_t) 5)

int main(int arc, char* argv[])
{


	//Pruebas de la lectura de ficheros

	char name[20] = "pruebaLectura.txt";
	FILE file;
	uint8_t modulacion = 0; //MPURO;
	struct Captura captura;

	aperturaFichero(name, &file);
	obtenerSecuenciaCapturaDelFichero(&file, &modulacion, &captura);


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