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
void obtenerSecuenciaCapturaDelFichero(FILE* file, uint8_t* modulacion)
{

	if(!file)
	{
		printf("Argumento no valido\n");
		return;
	}

	struct Captura* captura = (struct Captura *)calloc(sizeof(*captura));

	uint16_t posCab = 0;
	uint8_t byte = 0;
	uint8_t lineaEncontrada = 0;

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
				case ',':
				{
					if((llave == 0) && (parentesis == 0))
					{
						procesarInformacion(valor, posicionLinea, captura);
					}
					else if(llave == 0)
					{
						añadirValorGrupo(valor);
					}
					else
					{
						return E_MAL_FORMADO;
					}
					posicionLinea++;
					valor = 0;
					break;
				}
				case ')':
				{
					if(posicionLinea == 3)
					{
						parentesis--;
						if(parentesis == 0)
							return E_MAL_FORMADO;
						repetirUltimoConjunto(valor);
					}
					else
					{
						return E_MAL_FORMADO;
					}
					break;
				}
				case '(':
				{
					if(posicionLinea == 3)
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
				case 10:
				{
					lineaEncontrada = 0;
					posCab = 0;
					parentesis = 0;
					llave = 0;
					pos_nombre=0;
					posicionLinea = 0;
					valor = 0;
					break;
				}
				case '{':
				{
					if(posicionLinea == 3)
						llave++;
					else
						return E_MAL_FORMADO;
					break;
				}
				case '}':
				{
					if(posicionLinea == 3)
					{
						llave--;
						if(parentesis == 0)
							return E_MAL_FORMADO;
					}
					else
					{
						return E_MAL_FORMADO;
					}
					break;
				}
				default:
				{
					if((posicionLinea == 5) || 
						(posicionLinea == 6))
					{
						nombre[pos_nombre] = byte;
						pos_nombre++;
					}
					else
					{
						if((byte >= '0') || 
							(byte <= '9'))
						{
							byte-='0';
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

	return captura;
}

void procesarInformacion(uint32_t valor, uint8_t posicionLinea, struct Captura* captura)
{
	if(!captura)
	{
		return ERROR;
	}

	switch(posicionLinea)
	{
		case 0:
		{
			if((valor < 0) || (valor > 3))
			{
				return E_VALORES_INCORRECTOS;
			}
			else
			{

			}
			break;
		}
		case 1:
		{
			SUCCION(captura) = valor;
			break;
		}
		case 2:
		{
			TIEMPOANALISISODOR(captura) = valor;
			break;
		}
		case 3:
		{
			ORDENVALVULAS(captura) = valor;
			TOTALVALVULAS(captura) = valor;
			break;
		}				
		case 4:
		{
			TEMPERATURASENSOR(captura) = valor;
			break;
		}
		case 5:
		{
			//raiz nombre comun para ficheros
			uint8_t* raizNombre = FILEROOT(captura);
			while(*path != 0)
				*raizNombre++ = *path++;			
			break;
		}
		case 6:
		{
			//nombre carpeta donde guardar los ficheros
			uint8_t* carpeta = PATH(captura);
			while(*path != 0)
				*raizNombre++ = *path++;				
			break;
		}
		default:
		{
			return ERROR;
		}
	}
}