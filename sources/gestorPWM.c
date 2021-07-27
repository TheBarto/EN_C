#include "../headers/gestorPWM.h"

int8_t obtenerClavePWM(const char* nombre, char* clavePuerto)
{

	#ifdef DEBUG
		printf("nombre: %s - clavePuerto: %s\n", nombre, clavePuerto);
	#endif

	if(!nombre || !clavePuerto)
	{
		return E_ARG_INVALIDO;
	}

	for(pins_t* pPins = &table[0]; pPins->name; pPins++)
	{
		#ifdef DEBUG
			printf("Valor de pPins: %p\n",pPins);
			printf("Vuelta bucle: %s  -  %s\n", pPins->name, pPins->key);
		#endif

		char* pName = pPins->name;
		char* pArg = nombre;

		//while((*pArg++ == *pName++) && (*pName != 0)). De esta forma queda mas comprimido, pero el ultimo caracter no se comprueba
		while((*pArg == *pName) && (*pName != 0))
		{
			pArg++;
			pName++;
		}

		//Si al buscar hemos llegado a 0 (false), es que loas cadenas son iguales
		if(!*pName && !*pArg)
		{
			#ifdef DEBUG
				printf("Encontrado algo con name :) %s\n",pPins->name);
			#endif
			pName = pPins->name;
		}
		else
		{
			pName = NULL;
		}

		if(pName == NULL)
		{
			pName = pPins->key;
			pArg = nombre;

			while((*pName != 0) && (*pArg == *pName))
			{
				pArg++;
				pName++;
			}

			if(!*pName && !*pArg)
			{
				#ifdef DEBUG
					printf("Encontrado algo con key :) %s\n",pPins->key);
				#endif
				pName = pPins->key;
			}
			else
			{
				pName = NULL;
			}			
		}

		if(pName)
		{
			#ifdef DEBUG
				printf("Copiar en argumento clave, pName: %s\n",pName);
			#endif
			
			pArg = clavePuerto;
			while(*pName!=0)
				*pArg++=*pName++;

			return OK;
		}
	}

	return E_ARG_INVALIDO;
}

int8_t iniciar_puerto_PWM(const char* nombre, uint16_t* frecuencia, uint8_t* polaridad, uint8_t* cicloTrabajo)
{

	if(*frecuencia <= 0)
	{
		printf("Los valores de frecuencia deben ser mayores que 0\n");
		return E_ARG_INVALIDO;
	}

	if(*cicloTrabajo < 0 || *cicloTrabajo > 100)
	{
		printf("Los valores del ciclo de trabajo deben de ser mayores que 0 y menores de 100\n");
		return E_ARG_INVALIDO;		
	}

	if(*polaridad < 0 || *polaridad > 1)
	{
		printf("El valor de la polaridad debe de ser 0 o 1\n");
		return E_ARG_INVALIDO;		
	}

	char clave[20] = {0};
	if(obtenerClavePWM(nombre, &clave[0]) != OK)
	{
		printf("El PWM esta en uso o el nombre es incorrecto\n");
		return E_ARG_INVALIDO;
	}

	//pwm_start
	switch(/*pwm_start(clave, *cicloTrabajo, *frecuencia, *polaridad)*/2)
	{

		case E_ACCESO://BBIO_ACCESS:
		{
			printf("No se pudo acceder al fichero necesario\n");
			return E_ACCESO;
		break;
		}
		case E_SISTEMA_FIC://BBIO_ACCESS:
		{
			printf("Problemas con el sysfs\n");
			return E_SISTEMA_FIC;
		break;
		}
		case E_CAPA_MANEJADOR://BBIO_ACCESS:
		{
			printf("Problemas con el manejador de capas\n");
			return E_CAPA_MANEJADOR;
		break;
		}
		case E_ARG_INVALIDO://BBIO_ACCESS:
		{
			printf("Argumentos inválidos\n");
			return E_ARG_INVALIDO;
		break;
		}
		case E_MEMORIA://BBIO_ACCESS:
		{
			printf("Memoria insuficiente\n");
			return E_MEMORIA;
		break;
		}

		case E_GENERICO://BBIO_ACCESS:
		{
			printf("Error desconocido\n");
			return E_GENERICO;

		break;
		}
	}

	return OK;
}

int8_t parar_puerto_PWM(const char* nombre)
{
	if(!nombre)
	{
		return E_ARG_INVALIDO;
	}

	char clave[8] = {0};

	if(obtenerClavePWM(nombre, &clave[0]) != OK)
	{
		printf("El PWM esta en uso o el nombre es incorrecto\n");
		return 1;		
	}

	switch(2/*pwm_disable(&clave[0])*/)
	{
		case E_SISTEMA_FIC://BBIO_ACCESS:
		{
			printf("Problemas con el sysfs\n");
			return E_SISTEMA_FIC;
		}
		default://BBIO_ACCESS:
		{
			printf("Error desconocido\n");
			return E_GENERICO;
		break;
		}
	}

	return OK;
}

int8_t cambiar_ciclo_trabajo(const char* nombre, uint8_t* cicloTrabajo)
{

	if(*cicloTrabajo < 0 || *cicloTrabajo > 100)
	{
		printf("Los valores del ciclo de trabajo deben de ser mayores que 0 y menores de 100\n");
		return E_ARG_INVALIDO;		
	}

	char clave[8] = {0};

	if(obtenerClavePWM(nombre, &clave[0]) != OK)
	{
		printf("El PWM esta en uso o el nombre es incorrecto\n");
		return E_ARG_INVALIDO;		
	}

	if(2/*wm_set_duty_cicle(clave, *cicloTrabajo) != OK*/)
	{
		printf("Error al cambiar ciclo de trabajo. Arranque el PWM primero\n");
		return E_ACCESO;		
	}

	return OK;
}

int8_t cambiar_frecuencia(const char* nombre, uint8_t* frecuencia)
{

	if(*frecuencia < 0)
	{
		printf("Los valores del ciclo de trabajo deben de ser mayores que 0 y menores de 100\n");
		return E_ARG_INVALIDO;		
	}

	char clave[8] = {0};

	if(obtenerClavePWM(nombre, &clave[0]) != OK)
	{
		printf("El PWM esta en uso o el nombre es incorrecto\n");
		return E_ARG_INVALIDO;		
	}

	switch(2/*pwm_set_frequency(clave, *frecuencia)*/)
	{
		case E_SISTEMA_FIC:
		{
			printf("No se pudo escribir en el archivo de frecuencia\n");
			return E_SISTEMA_FIC;
			break;			
		}
		default:
		{
			printf("Error generico, prueba ha arrancar el PWM primero\n");
			return 3;
			break;			
		}
	}

	return OK;
}
