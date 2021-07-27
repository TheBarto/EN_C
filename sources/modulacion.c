#include "../headers/modulacion_comun.h"

struct Cofiguracion* crear_configuracion_defecto()
{

	struct Cofiguracion* config = (struct Cofiguracion*)malloc(sizeof(*config));
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

	return config;
}

void liberar_elems_struct_configuracion(struct Cofiguracion* config)
{
	config->nMuestrasCaptura = 0;
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

	config->totalElectrovalvulas = 0;
}

void configurar_struct_configuracion(struct Cofiguracion* sConfiguracion, uint8_t* datosConfiguracion,
	char** pelectrovalvulas, uint8_t* totalElectrovalvulas)
{
	sConfiguracion->nMuestrasCaptura = *datosConfiguracion;
	sConfiguracion->sleep = *(datosConfiguracion+1);
	sConfiguracion->sleepTH = *(datosConfiguracion+2);
	sConfiguracion->tEntreNMuestrasCaptura = *(datosConfiguracion+3);
	sConfiguracion->VCC = *(datosConfiguracion+4);
	sConfiguracion->pinMotor = *(datosConfiguracion+5);
	sConfiguracion->pinTH = *(datosConfiguracion+6);
	sConfiguracion->pinVoltajeSensor = *(datosConfiguracion+7);
	sConfiguracion->pinLecturaSensor = *(datosConfiguracion+8);
	sConfiguracion->totalElectrovalvulas = *totalElectrovalvulas;

	sConfiguracion->electrovalvulas = (char **)malloc((*totalElectrovalvulas)*sizeof(*sConfiguracion->electrovalvulas));

	for(uint8_t i = 0; i < *totalElectrovalvulas; i++)
	{
		char* pcharArg = (char *)pelectrovalvulas[i];
		*(sConfiguracion->electrovalvulas+i) = (char *)calloc(SIZE_WORD, sizeof(char));
		char* pcharStruct = (char *)sConfiguracion->electrovalvulas[i];

		while(*pcharArg != 0)
			*pcharStruct++ = *pcharArg++;
	}

	#ifdef DEBUG_MODE
		printf("Valores guardados: \n");
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
		}		
	#endif
}

struct Captura* inicializar_structs_capturas(uint16_t* totalMuestras, uint8_t* suctions, 
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
}

void liberar_structs_capturas(struct Captura* capturas, uint16_t* totalMuestras)
{

	for(uint16_t i = 0; i < *totalMuestras; i++)
	{
		free(capturas[i].ordenValvulas);
		capturas[i].ordenValvulas = NULL;
	}
	free(capturas);
	capturas = NULL;

	return;
}

/* Cambiar el char* por un char**, ya que ahora cada uno tiene su propia carpeta y nombre*/
struct Puro* inicializar_struct_Puro(uint16_t resistencia, char* path, struct Cofiguracion* configuracion,
	uint16_t* totalMuestras, uint8_t* suctions, uint8_t* temperaturasSensor, 
	int16_t* tiemposAnalisisOdor, uint8_t** ordenValvulas, funcion_captura func)
{

	//printf("1.C ordenValvulas: %p, ordenValvulas2: %p\n", (void *)ordenValvulas, (void *)&ordenValvulas[1]);	
	//printf("ordenValvulas[1][0]: %u\n", ordenValvulas[1][0]);

	struct Puro* puro = (struct Puro *)calloc(1, sizeof(struct Puro));

	puro->resistencia = resistencia;

	/*uint8_t* pPuroPath = &puro->path[0];
	while(*path != 0)
	{
		*pPuroPath++ = *path++;
	}*/

	puro->pTotalCapturas = *totalMuestras;
	puro->configuracion = configuracion;
	puro->capturas = inicializar_structs_capturas(totalMuestras, suctions, 
		temperaturasSensor, tiemposAnalisisOdor, ordenValvulas);

	puro->funcion = func;
	puro->f = NULL;

	return puro;
}

struct Puro* inicializar_struct_Puro_struct(uint16_t resistencia, char* path, struct Cofiguracion* configuracion,
	uint16_t* totalMuestras, funcion_captura func, struct Captura* captura)
{
	struct Puro* puro = (struct Puro *)calloc(1, sizeof(struct Puro));

	RESISTENCIA(puro) = resistencia;

	/*uint8_t* pPuroPath = PATH(puro);
	while(*path != 0)
	{
		*pPuroPath++ = *path++;
	}*/

	SCONFIGURACION(puro) = configuracion;
	TOTALCAPTURAS(puro) = *totalMuestras;

	//puro->funcion
	FILE(puro) = NULL;
	FUNCIONCAPTURA(puro) = func;
	SCAPTURAS(puro) = captura;

	return puro;
}

void liberar_struct_Puro(struct Puro* puro)
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
}

struct Regresion* inicializar_struct_Regresion(uint16_t resistencia, uint8_t* path, struct Captura* capturas, 
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
}

// Retornar algun mensaje o algo para error
void inicializar_fichero_puertos(uint8_t* succion, uint8_t* pinMotor, char* path, FILE* f)
{

	#ifdef DEBUG_MODE
		printf("Activo PWM del motor\n");
		printf("Path: %s\n", path);
	#else
		//PWM.start(*pinMotor, *succion);
	#endif	

	// Comprobamos que el directorio existe, y lo creamos en caso contrario
	struct stat sb = {0};

	if(stat(path, &sb) == -1)
	{
		printf("No creado\n");
		if(mkdir(path, 0755) == 0)
		{
			printf("Creado\n");
		}
		else
		{
			printf("Error, Errno: %s\n",strerror(errno));
		}
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
	#else
		//PWM.start(*pinMotor, *succion);
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
void captura_secuencia_odorantes_completa_puro(struct Captura* captura, struct Cofiguracion* config, uint16_t resistencia)
{

	printf("captura_secuencia_odorantes_completa_puro. TIEMPOANALISISODOR(captura): %d\n",TIEMPOANALISISODOR(captura));
	for(int i = 0; i < TIEMPOANALISISODOR(captura); i++)
	{
		time_t seconds_ini = time(NULL);

		uint8_t value = 0;

		//Lectura del ADC -> ¿incorrecta la lectura?
		#ifdef DEBUG_MODE
			printf("Hacemos una inicial lectura del ADC\n");
		#else
			//ADC.read(Puro.sensorPin2600)
		#endif		
		

		for(uint8_t i = 0; i < NMUESTRASCAPTURA(config); i++)
		{
			#ifdef DEBUG_MODE
				printf("Hacemos una lectura - %d del ADC\n",i);
			#else
				//value += ADC.read(Puro.sensorPin2600)*1800;
			#endif				
			
			usleep(TNMUESTRASCAPTURA(config));
		}

		value/=NMUESTRASCAPTURA(config);

		uint16_t resistencia_interna=((VCC(config)*resistencia)/(value/1000.))-resistencia;

		#ifdef DEBUG_MODE
			printf("Resistencia interna: %d\n",resistencia_interna);
		#endif		

		time_t seconds_end = time(NULL)-seconds_ini;

		usleep(SLEEP(config) - seconds_end);
	}

	return;
}

/* Le pasamos una estructura de modulacion, con las diferentes capturas indicadas. Cada
captura contiene un conjunto de apertura y cierre de valvulas, y tiempos de captura 
diferentes. */
void captura_secuencias_completas_puro(void* stru)
{
	
	struct Puro* puro = (struct Puro *)stru;	

	for(int i = 0; i < TOTALCAPTURAS(puro); i++)
	{

		struct Captura* captura = &SCAPTURASPOSI(puro,i);
		inicializar_fichero_puertos(&SUCCION(captura), &PINMOTOR(SCONFIGURACION(puro)), 
			(char*)PATH(captura), FILE(puro));

		printf("captura_secuencia_odorantes_completa_puro. TOTALVALVULAS(captura): %d\n",TOTALVALVULAS(captura));
		for(int j = 0; j < TOTALVALVULAS(captura); j++)
		{
			abrir_electrovalvulas((char **)ELECTROVALVULASTOTAL(SCONFIGURACION(puro)), 
				ORDENVALVULAS(captura), j);
			
			captura_secuencia_odorantes_completa_puro(captura, SCONFIGURACION(puro), RESISTENCIA(puro));
			
			cerrar_electrovalvulas((char **)ELECTROVALVULASTOTAL(SCONFIGURACION(puro)), 
				ORDENVALVULAS(captura), j);		
		}
		cierre(stru, MPURO, FILE(puro));
	}

	return;	
}

void cierre(void* structModulacion, uint8_t tipo, FILE* file)
{

	//Si es distinto de NULL liberamos
	if(file)
	{
		fclose(file);
		file = NULL;
	}

	#ifdef DEBUG_MODE
		printf("Paro PWM del motor\n");
		printf("Limpio PWM\n");
	#else
		//PWM.stop(Modulacion.motorPin)
    	//PWM.stop(heatPin)
		//PWM.cleanup()
	#endif

	//cerrar_electrovalvulas();

    //fclose(FILE(structModulacion));
}

void abrir_electrovalvulas(char** p_total_electrovalvulas, 
	uint8_t* p_electrovalvulas_seleccionadas, 
	uint8_t electrovalvula)
{

	printf("Abrimos la valvula: %s\n", p_total_electrovalvulas[p_electrovalvulas_seleccionadas[electrovalvula]]);
	#ifdef DEBUG_MODE
		printf("Abrimos la valvula: %s\n", p_total_electrovalvulas[p_electrovalvulas_seleccionadas[electrovalvula]]);
	#else
		//for(uint8_t i = 0; i < total_electrovalvulas; i++)
	    	//GPIO.output(Modulacion.electrovalvulas[number_elect-1], GPIO.HIGH)
    #endif
}

void cerrar_electrovalvulas(char** p_total_electrovalvulas, 
	uint8_t* p_electrovalvulas_seleccionadas, 
	uint8_t electrovalvula)
{

	#ifdef DEBUG_MODE
		printf("Cerramos la valvula: %s\n", p_total_electrovalvulas[p_electrovalvulas_seleccionadas[electrovalvula]]);
	#else
		//for(uint8_t i = 0; i < total_electrovalvulas; i++)	
    		//GPIO.output(Modulacion.electrovalvulas[number_elect-1], GPIO.LOW)
    #endif    	
}

//Esta función tiene como objetivo activar los puertos GPIO en caso de ser necesarios
void activar_puertos_GPIO(uint8_t** GPIOs, uint8_t* total_GPIOs)
{

	#ifdef DEBUG_MODE
		for(uint8_t i = 0; i < *total_GPIOs; i++)
	   		printf("Activamos puerto GPIO: %s\n", *(GPIOs+i));
	#else	
		//for(uint8_t i = 0; i < *total_GPIOs; i++)
    		//GPIO.setup(*(GPIOs+i),GPIO.OUT);
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
