#include "moduloLecturaFicheros.h"

void aperturaFichero(const char* nombre, FILE* file)
{

	if(!nombre)
	{
		printf("Argumento no valido\n");
		return;
	}

	file = NULL;

	file = fopen(nombre, "r");
	if(!file)
	{
		printf("Hubo un problema al leer y no se pudo continuar la ejecucion\n");
	}

	return;
}

/*Para obtener una secuencia, solamente tiene que leer una linea. Luego se captura y
 se lee otra linea para otra secuencia */
void obtenerSecuenciaCapturaDelFichero(FILE* file, uint8_t* modulacion, struct Captura* captura)
{

	if(!file)
	{
		printf("Argumento no valido\n");
		return;
	}

	uint16_t posCab = 0;
	uint8_t byte = 0;
	uint8_t lineaEncontrada = 0;
	uint8_t posicionLinea = 0;
	Stack* stack = createStack();

	uint8_t leido = fread(&byte, 1, 1, file);
	while((leido == 1) && (byte != 0))
	{
		if((cabeceraLinea[posCab] == byte) &
			(lineaEncontrada == 0))
		{
			posCab++;
			if(cabeceraLinea[posCab] == 0)
				lineaEncontrada = 1;
		}

		if(lineaEncontrada == 1)
		{
			switch(byte)
			{
				case VALOR_COMA:
				{
					if(posicionLinea == POSICION_ORDEN_VALVULAS)
					{
						if(llave == 1)
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
						procesarInformacion(&totalStackValues, posicionLinea, captura);
					}
					else
					{					
						if((llave == 0) && (parentesis == 0))
						{
							procesarInformacion(&valor, posicionLinea, captura);
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
					if(posicionLinea == POSICION_ORDEN_VALVULAS)
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
					}
					break;
				}
				case VALOR_APERTURA_PARENTESIS:
				{
					if(posicionLinea == POSICION_ORDEN_VALVULAS)
					{	
						parentesis++;
					}
					else
					{
						return E_MAL_FORMADO;
					}
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
					if(posicionLinea == POSICION_ORDEN_VALVULAS)
					{
						llave++;
						guardarValorPila(IDENTIFICADOR_APERTURA_LLAVE);
						guardarValorPila(valor);
					}
					else
						return E_MAL_FORMADO;
					break;
				}
				case VALOR_CIERRE_LLAVE:
				{
					if(posicionLinea == POSICION_ORDEN_VALVULAS)
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
					break;
				}
				case SEPARADOR_VALVULAS:
				{
					if(cierreLlaveCercano == VALOR_LLAVE_CERRADA)
					{
						procesarGrupoPila(stack, valor);
						cierreLlaveCercano = VALOR_RESET_LLAVE_CERRADA;
					}
					
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
	}

	if((leido == 0) && (byte == 0))
	{
		free(captura);
		captura = NULL;
	}

	deleteDeepCopyStack(stack);
	return captura;
}

void procesarInformacion(uint32_t* valor, uint8_t posicionLinea, struct Captura* captura)
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
				ORDENVALVULAS(captura) = *valor;
			else
				TOTALVALVULAS(captura) = *valor;
			break;
		}				
		case POSICION_TEMPERATURA_SENSOR:
		{
			TEMPERATURASENSOR(captura) = *valor;
			break;
		}
		case POSICION_RAIZ_NOMBRE_FICHERO:
		{
			//raiz nombre comun para ficheros
			uint8_t* raizNombre = FILEROOT(captura);
			while(*path != 0)
				*raizNombre++ = *path++;			
			break;
		}
		case POSICION_NOMBRE_CARPETA:
		{
			//nombre carpeta donde guardar los ficheros
			uint8_t* carpeta = PATH(captura);
			while(*path != 0)
				*raizNombre++ = *path++;				
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
}

void procesarUltimoElementoPila(Stack* stack, uint32_t valor)
{
	uint32_t valorPop = *(uint32_t *)pop(stack);

	for(uint32_t i = 0; i < valor; i++)
	{
		pushDeepCopy(stack, &valorPop, deepCopyInt32Value);
	}

	return;
}

void procesarGrupoPila(Stack* stack, uint32_t valor)
{

	uint32_t valorPop = *(uint32_t *)pop(stack);
	Queue* queue = createQueue();

	while(valorPop != IDENTIFICADOR_APERTURA_LLAVE)
	{
		insertDeepCopy(queue, &valorPop, deepCopyInt32Value);
		valorPop = *(uint32_t *)pop(stack);
	}

	for(uint32_t i = 0; i < valor; i++)
	{
		valorPop = *(uint32_t *)extract(queue);
		pushDeepCopy(stack, &valorPop, deepCopyInt32Value);
		insertDeepCopy(queue, &valorPop, deepCopyInt32Value);
	}

	deleteDeepCopyQueue(queue);
	return;
}