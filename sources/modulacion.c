#include "../headers/modulacion_comun.h"

#define DEBUG_GRADO1	((uint8_t) 0)

/*struct Configuracion* OLD_crear_configuracion_defecto()
{*/

	/*struct Cofiguracion* config = (struct Cofiguracion*)malloc(sizeof(*config));
	config->nMuestrasCaptura = 10;
	config->sleep = 1;
	config->sleepTH = 60;
	config->tEntreNMuestrasCaptura = 10;
	config->VCC = 5;
	config->pinMotor = 0;
	config->pinTH = 0;
	config->pinVoltajeSensor = 0;
	config->pinLecturaSensor = 0;
	config->totalElectrovalvulas = 0;
	config->electrovalvulas = NULL;

	return NULL;//config;*/
/*}*/

/*void OLD_liberar_elems_struct_configuracion(struct Cofiguracion* config)
{*/
	/*config->nMuestrasCaptura = 0;
	config->sleep = 0;
	config->sleepTH = 0;
	config->tEntreNMuestrasCaptura = 0;
	config->VCC = 0;
	config->pinMotor = 0;
	config->pinTH = 0;
	config->pinVoltajeSensor = 0;
	config->pinLecturaSensor = 0;

	for(uint8_t i = 0; i < config->totalElectrovalvulas; i++)
	{
		free(*(config->electrovalvulas+i));
	 *(config->electrovalvulas+i) = NULL;
	}

	free(config->electrovalvulas);
	config->electrovalvulas = NULL;

	config->totalElectrovalvulas = 0;*/
/*}*/

/*void OLD_configurar_struct_configuracion(struct Cofiguracion* sConfiguracion, uint8_t* datosConfiguracion,
		char** pelectrovalvulas, uint8_t* totalElectrovalvulas)
{*/
	/*sConfiguracion->nMuestrasCaptura = *datosConfiguracion;
	sConfiguracion->sleep = *(datosConfiguracion+1);
	sConfiguracion->sleepTH = *(datosConfiguracion+2);
	sConfiguracion->tEntreNMuestrasCaptura = *(datosConfiguracion+3);
	sConfiguracion->VCC = *(datosConfiguracion+4);
	sConfiguracion->pinMotor = *(datosConfiguracion+5);
	sConfiguracion->pinTH = *(datosConfiguracion+6);
	sConfiguracion->pinVoltajeSensor = *(datosConfiguracion+7);
	sConfiguracion->pinLecturaSensor = *(datosConfiguracion+8);
	sConfiguracion->totalElectrovalvulas = *totalElectrovalvulas;

	sConfiguracion->electrovalvulas = (char **)malloc((*totalElectrovalvulas)*sizeof(*sConfiguracion->electrovalvulas));*/

	/*for(uint8_t i = 0; i < *totalElectrovalvulas; i++)
	{
		char* pcharArg = (char *)pelectrovalvulas[i];
	 *(sConfiguracion->electrovalvulas+i) = (char *)calloc(SIZE_WORD, sizeof(char));
		char* pcharStruct = (char *)sConfiguracion->electrovalvulas[i];

		while(*pcharArg != 0)
	 *pcharStruct++ = *pcharArg++;
	}*/

/*#ifdef DEBUG_MODE*/
	/*printf("Valores guardados: \n");
	printf("nMuestrasCaptura: %d\n", sConfiguracion->nMuestrasCaptura);
	printf("sleep: %d\n", sConfiguracion->sleep);
	printf("sleepTH: %d\n", sConfiguracion->sleepTH);
	printf("tEntreNMuestrasCaptura: %d\n", sConfiguracion->tEntreNMuestrasCaptura);
	printf("VCC: %d\n", sConfiguracion->VCC);
	printf("pinMotor: %d\n", sConfiguracion->pinMotor);
	printf("pinTH: %d\n", sConfiguracion->pinTH);
	printf("pinVoltajeSensor: %d\n", sConfiguracion->pinVoltajeSensor);
	printf("pinLecturaSensor: %d\n", sConfiguracion->pinLecturaSensor);
	printf("totalElectrovalvulas: %d\n", sConfiguracion->totalElectrovalvulas);

	for(uint8_t i = 0; i < *totalElectrovalvulas; i++)
	{
		printf("pelectrovalvulas[%d]: %s\n", i, sConfiguracion->electrovalvulas[i]);
	}*/
/*#endif
}*/

/*struct Captura* OLD_inicializar_structs_capturas(uint16_t* totalMuestras, uint8_t* suctions,
		uint8_t* temperaturasSensor, int16_t* tiemposAnalisisOdor, uint8_t** ordenValvulas)
{
	struct Captura* capturas = (struct Captura *)malloc(*totalMuestras * sizeof(*capturas));

	for(uint16_t i = 0; i < *totalMuestras; i++)
	{

#ifdef DEBUG_MODE
		printf("2 - Valores en vuelta: %d: succion: %u, tempSensor: %d, tiempoAna: %d, totalValvulas: %d\n",
				i, suctions[i], temperaturasSensor[i], tiemposAnalisisOdor[i], ordenValvulas[i][0]);
#else
		//PWM.start(*pinMotor, *succion);
#endif

		capturas[i].succion = suctions[i];
		capturas[i].temperaturaSensor = temperaturasSensor[i];
		capturas[i].tiempoAnalisisOdor = tiemposAnalisisOdor[i];
		capturas[i].totalValvulas = ordenValvulas[i][0];
		capturas[i].ordenValvulas = (uint8_t*)malloc(ordenValvulas[i][0] * sizeof(uint8_t));
		for(uint16_t j = 1; j < (ordenValvulas[i][0]+1); j++)
			capturas[i].ordenValvulas[(j-1)] = ordenValvulas[i][j];
	}

	return capturas;
}*/

/*void OLD_liberar_structs_capturas(struct Captura* capturas, uint16_t* totalMuestras)
{

	for(uint16_t i = 0; i < *totalMuestras; i++)
	{
		free(capturas[i].ordenValvulas);
		capturas[i].ordenValvulas = NULL;
	}
	free(capturas);
	capturas = NULL;

	return;
}*/

/* Cambiar el char* por un char**, ya que ahora cada uno tiene su propia carpeta y nombre*/
/*struct Puro* OLD_inicializar_struct_Puro(uint16_t resistencia, char* path, struct Cofiguracion* configuracion,
		uint16_t* totalMuestras, uint8_t* suctions, uint8_t* temperaturasSensor,
		int16_t* tiemposAnalisisOdor, uint8_t** ordenValvulas, funcion_captura func)
{

	//printf("1.C ordenValvulas: %p, ordenValvulas2: %p\n", (void *)ordenValvulas, (void *)&ordenValvulas[1]);	
	//printf("ordenValvulas[1][0]: %u\n", ordenValvulas[1][0]);

	struct Puro* puro = (struct Puro *)calloc(1, sizeof(struct Puro));

	puro->resistencia = resistencia;*/

	/*uint8_t* pPuroPath = &puro->path[0];
	while(*path != 0)
	{
	 *pPuroPath++ = *path++;
	}*/

	/*puro->pTotalCapturas = *totalMuestras;
	puro->configuracion = configuracion;
	puro->capturas = inicializar_structs_capturas(totalMuestras, suctions, 
			temperaturasSensor, tiemposAnalisisOdor, ordenValvulas);

	puro->funcion = func;
	puro->f = NULL;

	return puro;
}*/

/*struct Puro* OLD_inicializar_struct_Puro_struct(uint16_t resistencia, char* path, struct Cofiguracion* configuracion,
		uint16_t* totalMuestras, funcion_captura func, struct Captura* captura)
{
	struct Puro* puro = (struct Puro *)calloc(1, sizeof(struct Puro));

	RESISTENCIA(puro) = resistencia;*/

	/*uint8_t* pPuroPath = PATH(puro);
	while(*path != 0)
	{
	 *pPuroPath++ = *path++;
	}*/

	/*SCONFIGURACION(puro) = configuracion;
	TOTALCAPTURAS(puro) = *totalMuestras;

	//puro->funcion
	FILE(puro) = NULL;
	FUNCIONCAPTURA(puro) = func;
	SCAPTURAS(puro) = captura;

	return puro;
}*/

/*void OLD_liberar_struct_Puro(struct Puro* puro)
{
	liberar_structs_capturas(SCAPTURAS(puro), &TOTALCAPTURAS(puro));
	liberar_elems_struct_configuracion(SCONFIGURACION(puro));
	free(SCONFIGURACION(puro));
	SCONFIGURACION(puro) = NULL;

	if(FILE(puro))
	{
		free(FILE(puro));
		FILE(puro) = NULL;
	}

	free(puro);
	puro = NULL;

	return;
}*/

/*struct Regresion* OLD_inicializar_struct_Regresion(uint16_t resistencia, uint8_t* path, struct Captura* capturas,
		struct Cofiguracion* configuracion)
{

	struct Regresion* regresion = (struct Regresion *)malloc(sizeof(regresion));

	regresion->resistencia = resistencia;

	uint8_t* pRegresionPath = &regresion->path[0];
	while(*path != 0)
		*pRegresionPath++ = *path++;

	regresion->configuracion = configuracion;
	regresion->capturas = capturas;

	return regresion;
}*/


// Retornar algun mensaje o algo para error
void inicializar_fichero_puertos(uint8_t* succion, uint8_t* pinMotor, char* path, FILE* f)
{

#ifdef DEBUG_MODE
	printf("Activo PWM del motor\n");
	printf("Path: %s\n", path);
#else
	//PWM.
	iniciar_puerto_PWM((const char *)pinMotor, *succion, FRECUENCIA_DEFECTO, POLARIDAD_DEFECTO);
#endif

	// Comprobamos que el directorio existe, y lo creamos en caso contrario
	struct stat sb = {0};

	if(stat(path, &sb) == -1)
	{
		printf("No creado\n");
		//Bloqueo porque falla
		/*if(mkdir(path, 0755) == 0)
		{
			printf("Creado\n");
		}
		else
		{
			printf("Error, Errno: %s\n",strerror(errno));
		}*/
	}

	// Obtenemos la fecha
	time_t now = time(NULL);

	struct tm* tm = localtime(&now);

	if(!tm)
	{
		return;
	}

	char ruta_archivo[550] = {0};

	// Ponemos el nombre al archivo y lo creamos
	sprintf(&ruta_archivo[0],"%s/capture_%04d_%02d_%02d-%02d_%02d_%02d.dat%c", path, tm->tm_year+1900, 
			tm->tm_mon+1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec,0);

#ifdef DEBUG_MODE
	printf("ruta_archivo: %s\n",ruta_archivo);
#endif



	f = fopen(ruta_archivo,"wb");
	if(f == NULL)
	{
		printf("Problema al abrir fichero: %s\n",ruta_archivo);
		return;
	}

	//Crear hilo aquí para captura de TyH¿?

	return;
}

/* Para cada captura, obtenemos los datos del sensor */
void captura_secuencia_odorantes_completa_puro(void* capt, struct Configuracion* config, FILE* file, uint16_t nMuestra)
{
	float tempTH = 0;
	float humTH = 0;
	float timeUSleep = (P_CONF_FREC_SUB_SAMPLES(config) / P_CONF_SUB_SAMPLES(config));
	float value = 0;

	time_t seconds_ini = time(NULL);
	struct tm* tm_struct = localtime(&seconds_ini);

	for(uint8_t i = 0; i < P_CONF_SUB_SAMPLES(config); i++)
	{
#ifdef DEBUG_MODE
		value = 20.0;
		//printf("Hacemos una lectura - %f del ADC\n",i);
#else
		//ADC.
		value += (leer_valor_ADC(P_CONF_SENSOR_READ_PIN(config))*1800);
#endif
		usleep(timeUSleep);
	}

	value/=P_CONF_SUB_SAMPLES(config);

	float resistencia_interna=((P_CONF_VCC(config)*P_CONF_RESISTENCIA(config))/(value/1000.))-P_CONF_RESISTENCIA(config);

#ifdef DEBUG_MODE
	printf("Resistencia interna: %f\n",resistencia_interna);
#endif

	//printf con los datos
	printf("Datos: %d %.3f %.3f 100 %.3f %.3f %d_%d_%d-%d_%d_%d\n", nMuestra, value, resistencia_interna, tempTH, humTH,
			(tm_struct->tm_year+1900), (tm_struct->tm_mon+1), tm_struct->tm_mday,
			tm_struct->tm_hour, tm_struct->tm_min, tm_struct->tm_sec);

	sleep(P_CONF_FREC_CAPTURA_SAMPLES(config) - (time(NULL)-seconds_ini));

	return;
}

/* Le pasamos una estructura de modulacion, con las diferentes capturas indicadas. Cada
captura contiene un conjunto de apertura y cierre de valvulas, y tiempos de captura 
diferentes. */
/*ESTA ES CORRECTA, FALTA PERFILARLA - CAMBIAR EL NOMBRE TAMBIEN*/
void captura_secuencias_completas_puro(struct Captura* captura, struct Configuracion* config)
{

	uint16_t nMuestras = 0;
	char nombreFichero[1500];

	time_t tm = time(NULL);
	struct tm* tm_struct = localtime(&tm);

	sprintf(&nombreFichero[0], "%s/%s-%d_%d_%d-%d_%d_%d.dat%c",P_CAPT_PATH(captura), P_CAPT_FILE_ROOT(captura),
			tm_struct->tm_year+1900, tm_struct->tm_mon+1, tm_struct->tm_mday, tm_struct->tm_hour,
			tm_struct->tm_min, tm_struct->tm_sec, 0);

	FILE* file = fopen(nombreFichero, "w");

#ifdef DEBUG_GRADO1
	printf("nombre fichero:) %s\n", nombreFichero);
#endif

	if(!file)
	{
		printf("Fallo al abrir el fichero %s, finalizamos la ejecucion:)\n", nombreFichero);
		return;
	}

	/* Antes de hacer nada, comprobamos que no se quieran
	 * abrir mas valvulas de las declaradas*/
	if(P_CAPT_TOTAL_VALVULAS(captura) > P_CONF_TOTAL_VALS(config))
	{
		printf("Se quieren abrir más electrovalvulas de las declaradas\n");
		return;
	}

	/* Con esto inicializamos el puerto PWM del motor*/
#ifdef DEBUG_MODE
	if(P_CAPT_SUCCION(captura) != 0xFF)
		printf("Activo PWM del motor\n");
	else
		printf("No activo PWM del motor\n");
#else
	//PWM.
	if(P_CAPT_SUCCION(captura) != 0xFF)
		iniciar_puerto_PWM((const char *)P_CONF_MOTOR_CTRL_PIN(config), P_CAPT_SUCCION(captura), FRECUENCIA_DEFECTO, POLARIDAD_DEFECTO);
#endif

#ifdef DEBUG_MODE
	printf("Activo PWM de la temperatura del sensor\n");
#else
	/* Inicializamos el puerto del sensor */
	iniciar_puerto_PWM((const char *)P_CONF_SENSOR_HEAT_PIN(config), P_CAPT_TEMPERATURA_SENSOR(captura), FRECUENCIA_DEFECTO, POLARIDAD_DEFECTO);
#endif

	int i = 0;
	//void inicializar_fichero_puertos(uint8_t* succion, uint8_t* pinMotor, char* path, FILE* f)
	while((i < P_CAPT_TOTAL_VALVULAS(captura)) || 
		(P_CAPT_TOTAL_VALVULAS(captura) == 0xFF))
	{
		/* Si existen electrovalvulas, comprobamos que la que se desee abrir
		 * exista, y que si hay 3 valvulas (0, 1 y 2) no se quiera abrir la
		 * numero 8.*/
		if(P_CAPT_TOTAL_VALVULAS(captura) != 0xFF)
		{
			if(P_CAPT_ORDEN_VALVULAS(captura)[i] < P_CONF_TOTAL_VALS(config))
			{
				abrir_electrovalvula(P_CONF_ELECTROVALVULAS(config), P_CAPT_ORDEN_VALVULAS(captura)[i]);
			}
			else
			{
				printf("La valvula que quiere abrirse no está declarada.\n");
				printf("Hay %d valvulas indicadas y se quiere abrir la numero %d\n",
						P_CONF_TOTAL_VALS(config), P_CAPT_ORDEN_VALVULAS(captura)[i]);
				cerrar_electrovalvulas(P_CONF_ELECTROVALVULAS(config), P_CONF_TOTAL_VALS(config));
				return;
			}
		}


#ifdef DEBUG_GRADO1
		printf("captura_secuencia_odorantes_completa_puro. TOTALVALVULAS(captura): %d\n",P_CAPT_TOTAL_VALVULAS(captura));
#endif

		int j = 0;
		while((j < P_CAPT_TIEMPO_ANALISIS_ODOR(captura)) || 
			(P_CAPT_TIEMPO_ANALISIS_ODOR(captura) == 0xFF))
		{
			P_CAPT_FUNCION(captura)(captura, config, file, nMuestras);
			nMuestras++;
			j++;
		}

		if(P_CAPT_TOTAL_VALVULAS(captura) != 0xFF)
			cerrar_electrovalvulas(P_CONF_ELECTROVALVULAS(config),
					P_CONF_TOTAL_VALS(config));

		i++;
	}

	cierreDescriptoresAbiertos(config, file);

	return;	
}


void OLD_captura_secuencia_odorantes(struct Captura* captura, struct Cofiguracion* config)
{

	/*inicializar_fichero_puertos(&SUCCION(captura), &PINMOTOR(SCONFIGURACION(puro)),
			(char*)PATH(captura), FILE(puro));*/

	printf("captura_secuencia_odorantes_completa_puro. TOTALVALVULAS(captura): %d\n",P_CAPT_TOTAL_VALVULAS(captura));
	for(int j = 0; j < P_CAPT_TOTAL_VALVULAS(captura); j++)
	{
		/*abrir_electrovalvulas((char **)ELECTROVALVULASTOTAL(SCONFIGURACION(puro)),
				ORDENVALVULAS(captura), j);*/

		//captura_secuencia_odorantes_completa_puro(captura, SCONFIGURACION(puro), RESISTENCIA(puro));

		/*cerrar_electrovalvulas((char **)ELECTROVALVULASTOTAL(SCONFIGURACION(puro)),
				ORDENVALVULAS(captura), j);*/
	}
	//cierre(stru, MPURO, FILE(puro));

	return;
}

void cierreDescriptoresAbiertos(struct Configuracion* conf, FILE* file)
{

	//Si es distinto de NULL liberamos
	if(file)
	{
		fclose(file);
		file = NULL;
	}

	if(P_CONF_MOTOR_CTRL_PIN(conf)[0] != 0)
	{
#ifdef DEBUG_MODE
		printf("Paro PWM del motor\n");
		printf("Limpio PWM\n");
#else
		//PWM.
		parar_puerto_PWM((const char *)P_CONF_MOTOR_CTRL_PIN(conf));
		parar_puerto_PWM((const char *)P_CONF_SENSOR_HEAT_PIN(conf));
		//PWM.cleanup()
#endif
	}

	cerrar_electrovalvulas(P_CONF_ELECTROVALVULAS(conf),
			P_CONF_TOTAL_VALS(conf));

	return;
}

/* Nos centramos en abrir una electrovalvula al momento */
void abrir_electrovalvula(uint8_t p_electrovalvulas[MAX_ELECTROVALVULAS][MAX_TAM_NOMBRE],
		uint8_t electrovalvula)
{
#ifdef DEBUG_MODE
	printf("Abrimos la valvula: %s\n", p_electrovalvulas[electrovalvula]);
#else
	//GPIO.
	salida_GPIO((const char *)p_electrovalvulas[electrovalvula], HIGH);
#endif
}

void cerrar_electrovalvulas(uint8_t p_electrovalvulas[MAX_ELECTROVALVULAS][MAX_TAM_NOMBRE],
		uint8_t total_electro)
{

	if(total_electro > 0)
	{
		for(uint8_t i = 0; i < total_electro; i++)
		{
#ifdef DEBUG_MODE
			printf("Cerramos la valvula: %s\n", p_electrovalvulas[i]);
#else
			//GPIO.
			salida_GPIO((const char *)p_electrovalvulas[i], LOW);
#endif
		}
	}
}

//Esta función tiene como objetivo activar los puertos GPIO en caso de ser necesarios
void activar_puertos_GPIO(uint8_t** GPIOs, uint8_t* total_GPIOs)
{

#ifdef DEBUG_MODE
	for(uint8_t i = 0; i < *total_GPIOs; i++)
		printf("Activamos puerto GPIO: %s\n", *(GPIOs+i));
#else
	//GPIO.
	for(uint8_t i = 0; i < *total_GPIOs; i++)
		iniciar_GPIO((const char *)*(GPIOs+i), OUTPUT, PULL_UP_DOWN_DEFECTO, RETRASO_GPIO_DEFECTO, INICIAL_DEFECTO);

#endif
}

/*void captura_muestras_datos(uint8_t* tipo, void* structModulacion)
{
	switch(*tipo)
	{
		case MPURO:
		{
			struct Puro* puro = (struct Puro*) structModulacion;

			FUNCIONCAPTURA(puro, (void *)puro);
			break;
		}
		case MREGRESION:
		{

			break;
		}
		case MMARTINELLI:
		{

			break;
		}
		default:
		{

		}
	}
}*/
