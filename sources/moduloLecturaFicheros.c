#include "../headers/moduloLecturaFicheros.h"

//Inicializamos la variable global con el inicio de linea. Esto se hace así por lo visto
char cabeceraLinea[20] = {"EN_CAPTURE:"};

FILE* aperturaFichero(const char* nombre)
{

	if(!nombre)
	{
		printf("Argumento no valido\n");
		return NULL;
	}


	FILE* file = fopen(nombre, "r");
	if(!file)
	{
		printf("Hubo un problema al leer y no se pudo continuar la ejecucion\n");
	}

	printf("Direccion de memoria: %p\n", (void *)file);

	return file;
}

/*Para obtener una secuencia, solamente tiene que leer una linea. Luego se captura y
 se lee otra linea para otra secuencia */
uint8_t obtenerSecuenciaCapturaDelFicheroOld(FILE* file, uint8_t* modulacion, struct Captura* captura)
{

	if(file == NULL)
	{
		printf("Descriptor de fichero no valido\n");
		return ERROR;
	}

	uint32_t* pvalor = 0;
	uint32_t valor = 0;
	uint16_t posCab = 0;
	uint8_t byte = 0;
	uint8_t lineaEncontrada = 0;
	uint8_t posicionLinea = 0;
	uint8_t llave = 0;
	uint8_t parentesis = 0;
	uint8_t cierreLlaveCercano = 0;
	uint8_t pos_nombre = 0;
	uint8_t nombre[50];
	//Stack* stack = createStack();

	pvalor = &valor;

	uint8_t leido = fread(&byte, 1, 1, file);
	printf("byte leido: %c\n", byte);
	while((leido == 1) && (byte != 0))
	{
		if((cabeceraLinea[posCab] == byte) &
			(lineaEncontrada == 0))
		{
			posCab++;
			if(cabeceraLinea[posCab] == 0)
			{
				lineaEncontrada = 1;
				printf("LINEA ENCONTRADA\n");
			}
		}

		if(lineaEncontrada == 1)
		{	
			switch(byte)
			{
				case VALOR_COMA:
				{
					printf("posicion linea: %d\n", posicionLinea);
					if(posicionLinea == POSICION_ORDEN_VALVULAS)
					{
						/*if(llave == 1)
						{
							llave = 0;
							procesarGrupoPila(stack, valor);
						}
						else
						{
							return E_MAL_FORMADO;
						}

						uint32_t* dataArray = NULL;

						uint32_t totalStackValues = getUInt32ArrayWithStackElements(stack, dataArray);
						procesarInformacion(dataArray, posicionLinea, captura);
						procesarInformacion(&totalStackValues, posicionLinea, captura);*/
					}
					else
					{
						pvalor = &valor;
						if((posicionLinea == POSICION_RAIZ_NOMBRE_FICHERO) || 
							(posicionLinea == POSICION_NOMBRE_CARPETA))
							pvalor = (uint32_t *)nombre;

						if((llave == 0) && (parentesis == 0))
						{
							procesarInformacion(pvalor, posicionLinea, captura);
						}
						else
						{
							return E_MAL_FORMADO;
						}
					}
					posicionLinea++;
					valor = 0;
					break;
				}
				case VALOR_CIERRE_PARENTESIS:
				{
					/*if(posicionLinea == POSICION_ORDEN_VALVULAS)
					{
						parentesis--;
						if(parentesis < 0)
							return E_MAL_FORMADO;

						if(cierreLlaveCercano == VALOR_RESET_LLAVE_CERRADA)
						{
							procesarUltimoElementoPila(stack, valor);
						}
						else
						{
							procesarGrupoPila(stack, valor);
							cierreLlaveCercano = VALOR_RESET_LLAVE_CERRADA;
						}
					}
					else
					{
						return E_MAL_FORMADO;
					}*/
					break;
				}
				case VALOR_APERTURA_PARENTESIS:
				{
					/*if(posicionLinea == POSICION_ORDEN_VALVULAS)
					{	
						parentesis++;
					}
					else
					{
						return E_MAL_FORMADO;
					}*/
					break;
				}
				//Salto de linea
				case VALOR_SALTO_LINEA:
				{
					lineaEncontrada = 0;
					posCab = 0;
					parentesis = 0;
					llave = 0;
					pos_nombre = 0;
					posicionLinea = 0;
					valor = 0;
					break;
				}
				case VALOR_APERTURA_LLAVE:
				{
					/*if(posicionLinea == POSICION_ORDEN_VALVULAS)
					{
						llave++;
						pushDeepCopy(stack, &valor, deepCopyInt32Value);
						valor = IDENTIFICADOR_APERTURA_LLAVE;
						pushDeepCopy(stack, &valor, deepCopyInt32Value);
					}
					else
						return E_MAL_FORMADO;
					break;*/
				}
				case VALOR_CIERRE_LLAVE:
				{
					/*if(posicionLinea == POSICION_ORDEN_VALVULAS)
					{
						llave--;
						if(parentesis < 0)
							return E_MAL_FORMADO;

						if(cierreLlaveCercano == VALOR_LLAVE_CERRADA)
						{
							procesarGrupoPila(stack, valor);
						}

						cierreLlaveCercano = VALOR_LLAVE_CERRADA;
					}
					else
					{
						return E_MAL_FORMADO;
					}
					break;*/
				}
				case SEPARADOR_VALVULAS:
				{
					/*if(cierreLlaveCercano == VALOR_LLAVE_CERRADA)
					{
						procesarGrupoPila(stack, valor);
						cierreLlaveCercano = VALOR_RESET_LLAVE_CERRADA;
					}*/
					
					break;
				}
				default:
				{
					if((posicionLinea == POSICION_RAIZ_NOMBRE_FICHERO) || 
						(posicionLinea == POSICION_NOMBRE_CARPETA))
					{
						nombre[pos_nombre] = byte;
						pos_nombre++;
					}
					else
					{
						if((byte >= VALOR_NUMERO_0) || 
							(byte <= VALOR_NUMERO_9))
						{
							byte-=VALOR_NUMERO_0;
							valor*=10;
							valor+=byte;
						}
						else
						{
							return E_MAL_FORMADO;
						}
					}
					break;
				}
			}
		}

		leido = fread(&byte, 1, 1, file);
		printf("byte leido: %c\n", byte);
	}

	if((leido == 0) && (byte == 0))
	{
		free(captura);
		captura = NULL;
	}

	//deleteDeepCopyStack(stack);
	return SALIDA_OK;
}

/*Para obtener una secuencia, solamente tiene que leer una linea. Luego se captura y
 se lee otra linea para otra secuencia */
uint8_t obtenerSecuenciaCapturaDelFichero(FILE* file, uint8_t* modulacion, struct Captura* captura)
{

	if(file == NULL)
	{
		printf("Descriptor de fichero no valido\n");
		return ERROR;
	}

	uint32_t valor = 0;
	uint32_t* pvalor = &valor;
	uint16_t posCab = 0;
	uint8_t byte = 0;
	uint8_t lineaEncontrada = 0;
	uint8_t posicionLinea = 0;

	uint8_t pos_nombre = 0;
	uint8_t nombre[50];
	uint8_t orden_valvulas[255];
	uint8_t pos_orden_valvulas = 0;

	uint8_t leido = fread(&byte, 1, 1, file);
	printf("byte leido: %c\n", byte);
	while((leido == 1) && (byte != 0))
	{
		if((cabeceraLinea[posCab] == byte) &
			(lineaEncontrada == 0))
		{
			posCab++;
			if(cabeceraLinea[posCab] == 0)
			{
				lineaEncontrada = 1;
				printf("LINEA ENCONTRADA\n");
			}
		}

		if(lineaEncontrada == 1)
		{	
			switch(byte)
			{
				case VALOR_COMA:
				{
					printf("posicion linea: %d\n", posicionLinea);

					if(posicionLinea == POSICION_ORDEN_VALVULAS)
					{
						procesarCadenaAperturaValvulas(orden_valvulas, &ORDENVALVULAS(captura), &TOTALVALVULAS(captura));

						printf("obtenerSecuenciaCapturaDelFichero > TOTAL VALVULAS: %d\n", TOTALVALVULAS(captura));
						
						for(int i = 0; i < TOTALVALVULAS(captura); i++)
							printf("%d - ", ORDENVALVULAS(captura)[i]);
						printf("\n");						
					}
					else
					{
						if((posicionLinea == POSICION_RAIZ_NOMBRE_FICHERO) || 
						(posicionLinea == POSICION_NOMBRE_CARPETA))
						{
							pvalor = (uint32_t *)nombre;
							printf("Valor de nombre y pvalor: %s - %s\n",nombre, (char *)pvalor);
						}
						else
						{
							pvalor = (uint32_t *)&valor;
						}

						procesarInformacion(pvalor, posicionLinea, captura);
					}

					posicionLinea++;
					valor = 0;
					pos_nombre=0;
					break;
				}
				//Salto de linea
				case VALOR_SALTO_LINEA:
				{
					lineaEncontrada = 0;
					posCab = 0;
					pos_nombre = 0;
					pos_orden_valvulas = 0;
					posicionLinea = 0;
					valor = 0;

					//Ponemos esto aquí para que procese el ultimo elemento de la linea, sino no lo hace
					if((posicionLinea == POSICION_RAIZ_NOMBRE_FICHERO) || 
					(posicionLinea == POSICION_NOMBRE_CARPETA))
					{
						pvalor = (uint32_t *)nombre;
						printf("Valor de nombre y pvalor: %s - %s\n",nombre, (char *)pvalor);
					}
					else
					{
						pvalor = (uint32_t *)&valor;
					}

					procesarInformacion(pvalor, posicionLinea, captura);

					break;
				}

				default:
				{
					if(posicionLinea == POSICION_ORDEN_VALVULAS)
					{
						orden_valvulas[pos_orden_valvulas] = byte;
						pos_orden_valvulas++;
						orden_valvulas[pos_orden_valvulas] = 0;
					}
					else if((posicionLinea == POSICION_RAIZ_NOMBRE_FICHERO) || 
						(posicionLinea == POSICION_NOMBRE_CARPETA))
					{
						nombre[pos_nombre] = byte;
						pos_nombre++;
						nombre[pos_nombre] = 0;
						printf("Valor de nombre en %d: %s\n", posicionLinea, nombre);
					}
					else
					{
						if((byte >= VALOR_NUMERO_0) || 
							(byte <= VALOR_NUMERO_9))
						{
							byte-=VALOR_NUMERO_0;
							valor*=10;
							valor+=byte;
						}
						else
						{
							return E_MAL_FORMADO;
						}
					}
					break;
				}
			}
		}

		leido = fread(&byte, 1, 1, file);
		printf("byte leido: %c\n", byte);
	}

	//Ponemos esto aquí para que procese el ultimo elemento de la linea, sino no lo hace
	if((posicionLinea == POSICION_RAIZ_NOMBRE_FICHERO) || 
	(posicionLinea == POSICION_NOMBRE_CARPETA))
	{
		pvalor = (uint32_t *)nombre;
		printf("Valor de nombre y pvalor: %s - %s\n",nombre, (char *)pvalor);
	}
	else
	{
		pvalor = (uint32_t *)&valor;
	}

	procesarInformacion(pvalor, posicionLinea, captura);	

	if((leido == 0) && (byte == 0))
	{
		free(captura);
		captura = NULL;
	}

	//deleteDeepCopyStack(stack);
	return SALIDA_OK;
}

uint8_t procesarCadenaAperturaValvulas(uint8_t* cadenaApertura, uint8_t** dstArray, uint8_t* total_dstArray)
{

	uint16_t i = 0;
	uint8_t llave = 0;
	uint8_t parentesis = 0;
	uint32_t valor = 0;
	uint8_t cierreLlaveCercano = VALOR_RESET_LLAVE_CERRADA;

	Stack* stack = createStack();

	while(cadenaApertura[i] != 0)
	{
		printf("Valor de cadenaApertura[i]: %c\n", cadenaApertura[i]);
		switch(cadenaApertura[i])
		{
			case VALOR_APERTURA_LLAVE:
				printf("VALOR_APERTURA_LLAVE: %d y total de llaves: %d\n", cadenaApertura[i], llave);
				llave++;
				//pushDeepCopy(stack, &cadenaApertura[i], deepCopyUInt8Value);
			break;

			case VALOR_CIERRE_LLAVE:
				printf("VALOR_CIERRE_LLAVE: %d y total de llaves: %d\n", cadenaApertura[i], llave);
				llave--;
				if(llave < 0)
					return E_MAL_FORMADO;

				//Habria que introducir el valor??

				cierreLlaveCercano = VALOR_LLAVE_CERRADA;
			break;

			case VALOR_APERTURA_PARENTESIS:
				/*Si encontramos un parentesis, tenemos que guardar el elemento que tenemos leido, siempre
				 *que no se haya una llave.*/
				if(cierreLlaveCercano == VALOR_NUMERO_LEIDO)
				{
					pushDeepCopy(stack, &valor, deepCopyUInt8Value);
					cierreLlaveCercano = VALOR_RESET_LLAVE_CERRADA;
				}
				parentesis++;
				valor = 0;
			break;

			case VALOR_CIERRE_PARENTESIS:
				parentesis--;
				if(parentesis < 0)
					return E_MAL_FORMADO;	
					
				if(cierreLlaveCercano == VALOR_LLAVE_CERRADA)
				{
					procesarGrupoPila(stack, valor);
					cierreLlaveCercano = VALOR_RESET_LLAVE_CERRADA;
				}
				else
					procesarUltimoElementoPila(stack, valor);

				cierreLlaveCercano = VALOR_CIERRE_PARENTESIS;
				valor = 0;
			break;

			case SEPARADOR_VALVULAS:

				if(cierreLlaveCercano != VALOR_CIERRE_PARENTESIS)
				{
					printf("SEPARADOR_VALVULAS. Valor :%d \n", valor);
					if(cierreLlaveCercano == VALOR_LLAVE_CERRADA)
						procesarGrupoPila(stack, valor);
					else
						pushDeepCopy(stack, &valor, deepCopyUInt8Value);

					valor = 0; //Reiniciamos tras guardar
				}
				cierreLlaveCercano = VALOR_RESET_LLAVE_CERRADA;
			break;

			default:
				if(cadenaApertura[i] >= '0' && cadenaApertura[i] <= '9')
				{
					valor*=10;
					valor += (cadenaApertura[i]-'0');
					printf("Valor leido: %c y valor actual: %d. Valor cierreLlaveCercano: %d\n", cadenaApertura[i], valor, cierreLlaveCercano);
					if(cierreLlaveCercano == VALOR_RESET_LLAVE_CERRADA)
						cierreLlaveCercano = VALOR_NUMERO_LEIDO;
				}
				else
				{
					return ERROR;
				}
		}
		i++;
	}

	if(cierreLlaveCercano == VALOR_NUMERO_LEIDO)
	{
		//Una vez llegado al final, procesamos el ultimo valor 
		pushDeepCopy(stack, &valor, deepCopyUInt8Value);
		
	}
	else if(cierreLlaveCercano == VALOR_LLAVE_CERRADA)
	{
		procesarGrupoPila(stack, valor);
	}
	cierreLlaveCercano = VALOR_RESET_LLAVE_CERRADA;

	*total_dstArray = getUInt8ArrayWithStackElements(stack, dstArray);
	
	printf("procesarCadenaAperturaValvulas > El total de elementos es: %d\n",*total_dstArray);
	for(int i = 0; i < *total_dstArray; i++)
		printf("%d ", *(dstArray+i));

	printf("\n");

	return OK;
}

uint8_t procesarInformacion(uint32_t* valor, uint8_t posicionLinea, struct Captura* captura)
{
	if(!captura)
	{
		return ERROR;
	}

	switch(posicionLinea)
	{
		case POSICION_MODULACION:
		{
			if((*valor < 0) || (*valor > 3))
			{
				return E_VALORES_INCORRECTOS;
			}
			else
			{

			}
			break;
		}
		case POSICION_SUCCION:
		{
			SUCCION(captura) = *valor;
			break;
		}
		case POSICION_DURACION_ODORANTE:
		{
			TIEMPOANALISISODOR(captura) = *valor;
			break;
		}
		case POSICION_ORDEN_VALVULAS:
		{
			if(!ORDENVALVULAS(captura))
				ORDENVALVULAS(captura) = (uint8_t*)valor;
			else
				TOTALVALVULAS(captura) = (uint8_t)*valor;
			break;
		}				
		case POSICION_TEMPERATURA_SENSOR:
		{
			TEMPERATURASENSOR(captura) = *valor;
			break;
		}
		case POSICION_RAIZ_NOMBRE_FICHERO:
		{
			//Importante hacer el casting para obtener los tamanios adecuados del puntero
			char *pvalor = (char *)valor;
			printf("POSICION_RAIZ_NOMBRE_FICHERO - Valor: %s, %p\n", (char *)valor, valor);
			//raiz nombre comun para ficheros
			uint8_t* raizNombre = FILEROOT(captura);

			while(*pvalor != 0)
			{
				printf("Caracter valor: %c\n",*pvalor);
				*raizNombre++ = *pvalor++;
			}
			*raizNombre = 0;

			printf("Valor raiz nombre fichero: %s\n", FILEROOT(captura));
			break;
		}
		case POSICION_NOMBRE_CARPETA:
		{
			//Importante hacer el casting para obtener los tamanios adecuados del puntero
			char *pvalor = (char *)valor;		
			printf("POSICION_NOMBRE_CARPETA - Valor: %s\n", (char*)valor);
			//nombre carpeta donde guardar los ficheros
			uint8_t* carpeta = PATH(captura);
			while(*pvalor != 0)
			{
				printf("Caracter valor: %c\n",*pvalor);
				*carpeta++ = *pvalor++;
			}
			*carpeta = 0;	

			printf("Valor nombre carpeta: %s\n", PATH(captura));		
			break;
		}
		default:
		{
			if(TOTALVALORESEXTRA(captura) < 20)
			{
				GUARDARVALORESEXTRA(captura, *valor);
			}
			else
			{
				return ERROR;
			}
		}
	}

	return SALIDA_OK;
}

void procesarUltimoElementoPila(Stack* stack, uint32_t valor)
{

	printf("procesarUltimoElementoPila > nº repeticiones: %d\n", valor);

	uint32_t valorPop = *(uint32_t *)pop(stack);

	printf("procesarUltimoElementoPila > valorPop: %d\n", valorPop);

	for(uint32_t i = 0; i < valor; i++)
	{
		printf("procesarUltimoElementoPila > introduccion pila: %d\n", i);
		pushDeepCopy(stack, &valorPop, deepCopyInt32Value);
	}

	return;
}

void procesarGrupoPila(Stack* stack, uint32_t valor)
{

	uint32_t valorPop = *(uint32_t *)pop(stack);
	LinkedList* list = createLinkedList();

	while(valorPop != IDENTIFICADOR_APERTURA_LLAVE)
	{
		printf("procesarGrupoPila > valor leido de la pila: %d\n", valorPop);
		appendElemIndexDeepCopy(list, &valorPop, 0, deepCopyInt32Value);
		valorPop = *(uint32_t *)pop(stack);
	}

	for(uint32_t i = 0; i < valor; i++)
	{
		for(uint32_t j = 0; j < valor; j++)
		{
			valorPop = *(uint32_t *)getLinkedListElem(list, j);
			pushDeepCopy(stack, &valorPop, deepCopyInt32Value);
		}
	}

	deleteLinkedList(list);
	return;
}