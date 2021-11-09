#include "../headers/moduloLecturaFicheros.h"

//Inicializamos la variable global con el inicio de linea. Esto se hace asÃ­ por lo visto
char cabeceraLinea[20] = { "EN_CAPTURE:" };
char cabeceraConf[20] = { "EN_CONFIG:" };

void aperturaFichero(const char* nombre, char* data) {

	if (!nombre) {
		printf("Argumento no valido\n");
		return;
	}

	FILE* file = fopen(nombre, "r");
	if (!file) {
		printf("Hubo un problema al leer y no se pudo continuar la ejecucion\n");
	}

	//Leemos los datos, y si no hay un salto de linea, lo añadimos
	uint8_t leido = fread(&data[0], 1, TOTAL_DATA_READED_FILE, file);
	if ((data[leido - 1] != '\n') && (leido < TOTAL_DATA_READED_FILE - 1)) {
		data[leido] = '\n';
		data[leido+1] = 0;
		leido++;
	}

#ifdef DEBUG_GRADO1
	printf("Direccion de memoria: %p\n", (void *) file);
#endif

	fclose(file);
	file = NULL;

	return;
}

/*Para obtener una secuencia, solamente tiene que leer una linea. Luego se captura y
 se lee otra linea para otra secuencia */
uint8_t obtenerSecuenciaCapturaDelFicheroOld(FILE* file, uint8_t* modulacion,
		struct Captura* captura) {

	if (file == NULL) {
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
	//uint8_t cierreLlaveCercano = 0;
	uint8_t pos_nombre = 0;
	uint8_t nombre[50];
	//Stack* stack = createStack();

	pvalor = &valor;

	uint8_t leido = fread(&byte, 1, 1, file);
	printf("byte leido: %c\n", byte);
	while ((leido == 1) && (byte != 0)) {
		if ((cabeceraLinea[posCab] == byte) & (lineaEncontrada == 0)) {
			posCab++;
			if (cabeceraLinea[posCab] == 0) {
				lineaEncontrada = 1;
				printf("LINEA ENCONTRADA\n");
			}
		}

		if (lineaEncontrada == 1) {
			switch (byte) {
			case VALOR_COMA: {
				printf("posicion linea: %d\n", posicionLinea);
				if (posicionLinea == POSICION_ORDEN_VALVULAS) {
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
				} else {
					pvalor = &valor;
					if ((posicionLinea == POSICION_RAIZ_NOMBRE_FICHERO)
							|| (posicionLinea == POSICION_NOMBRE_CARPETA))
						pvalor = (uint32_t *) nombre;

					if ((llave == 0) && (parentesis == 0)) {
						procesarInformacion(pvalor, posicionLinea, captura);
					} else {
						return E_MAL_FORMADO;
					}
				}
				posicionLinea++;
				valor = 0;
				break;
			}
			case VALOR_CIERRE_PARENTESIS: {
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
			case VALOR_APERTURA_PARENTESIS: {
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
			case VALOR_SALTO_LINEA: {
				lineaEncontrada = 0;
				posCab = 0;
				parentesis = 0;
				llave = 0;
				pos_nombre = 0;
				posicionLinea = 0;
				valor = 0;
				break;
			}
			case VALOR_APERTURA_LLAVE: {
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
			case VALOR_CIERRE_LLAVE: {
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
			case SEPARADOR_VALVULAS: {
				/*if(cierreLlaveCercano == VALOR_LLAVE_CERRADA)
				 {
				 procesarGrupoPila(stack, valor);
				 cierreLlaveCercano = VALOR_RESET_LLAVE_CERRADA;
				 }*/

				break;
			}
			default: {
				if ((posicionLinea == POSICION_RAIZ_NOMBRE_FICHERO)
						|| (posicionLinea == POSICION_NOMBRE_CARPETA)) {
					nombre[pos_nombre] = byte;
					pos_nombre++;
				} else {
					if ((byte >= VALOR_NUMERO_0) || (byte <= VALOR_NUMERO_9)) {
						byte -= VALOR_NUMERO_0;
						valor *= 10;
						valor += byte;
					} else {
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

	if ((leido == 0) && (byte == 0)) {
		free(captura);
		captura = NULL;
	}

	//deleteDeepCopyStack(stack);
	return SALIDA_OK;
}

/*Para obtener una secuencia, solamente tiene que leer una linea. Luego se captura y
 se lee otra linea para otra secuencia */
uint8_t obtenerSecuenciaCapturaDelFichero(const char* data,
		uint16_t* totalDatosLeidos,
		struct Captura* captura) {

	P_CAPT_TOTAL_VALORES_EXTRA(captura) = 0;

	if (data == NULL) {
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

	byte = data[*totalDatosLeidos];
#ifdef DEBUG_GRADO2
	printf("byte leido: %c\n", byte);
#endif
	while ((byte != 0) && (byte != VALOR_SALTO_LINEA)) {
		if(lineaEncontrada == 0)
		{
			if ((cabeceraLinea[posCab] == byte)) {
				posCab++;
				if (cabeceraLinea[posCab] == 0) {
					lineaEncontrada = 1;
#ifdef DEBUG_GRADO2
					printf("LINEA ENCONTRADA\n");
#endif
					(*totalDatosLeidos)++;
					byte = data[*totalDatosLeidos];
#ifdef DEBUG_GRADO1
					printf("byte leido: %c\n", byte);
#endif
				}
			}
			else
			{
				posCab = 0;
			}
		}

		if (lineaEncontrada == 1) {
			switch (byte) {
			case VALOR_COMA: {
#ifdef DEBUG_GRADO1
				printf("posicion linea: %d\n", posicionLinea);
#endif

				if (posicionLinea == POSICION_ORDEN_VALVULAS) {
					procesarCadenaAperturaValvulas(orden_valvulas,
							&P_CAPT_ORDEN_VALVULAS(captura), &P_CAPT_TOTAL_VALVULAS(captura),
							pos_orden_valvulas);

#ifdef DEBUG_GRADO2
					printf("obtenerSecuenciaCapturaDelFichero > TOTAL VALVULAS: %d\n",
							P_CAPT_TOTAL_VALVULAS(captura));

					for (int i = 0; i < P_CAPT_TOTAL_VALVULAS(captura); i++)
						printf("%d - ", P_CAPT_ORDEN_VALVULAS(captura)[i]);
					printf("\n");
#endif
				} else {
					if ((posicionLinea == POSICION_RAIZ_NOMBRE_FICHERO)
							|| (posicionLinea == POSICION_NOMBRE_CARPETA)) {
						pvalor = (uint32_t *) nombre;
#ifdef DEBUG_GRADO2
						printf("Valor de nombre y pvalor: %s - %s\n", nombre, (char *) pvalor);
#endif
					} else {
						pvalor = (uint32_t *) &valor;
					}

					procesarInformacion(pvalor, posicionLinea, captura);
				}

				posicionLinea++;
				valor = 0;
				pos_nombre = 0;
				break;
			}
				//Salto de linea
			case VALOR_SALTO_LINEA: {
				lineaEncontrada = 0;
				posCab = 0;
				pos_nombre = 0;
				pos_orden_valvulas = 0;
				posicionLinea = 0;
				valor = 0;

				//Ponemos esto aquÃ­ para que procese el ultimo elemento de la linea, sino no lo hace
				if ((posicionLinea == POSICION_RAIZ_NOMBRE_FICHERO)
						|| (posicionLinea == POSICION_NOMBRE_CARPETA)) {
					pvalor = (uint32_t *) nombre;
#ifdef DEBUG_GRADO2
					printf("Valor de nombre y pvalor: %s - %s\n", nombre, (char *) pvalor);
#endif
				} else {
					pvalor = (uint32_t *) &valor;
				}

				procesarInformacion(pvalor, posicionLinea, captura);

				/*Sumo uno mas para no leer el salto de linea
				 * la siguiente vez que leamos los datos*/
				(*totalDatosLeidos)++;

				break;
			}

			default: {
				if (posicionLinea == POSICION_ORDEN_VALVULAS) {
					orden_valvulas[pos_orden_valvulas] = byte;
					pos_orden_valvulas++;
					orden_valvulas[pos_orden_valvulas] = 0;
				} else if ((posicionLinea == POSICION_RAIZ_NOMBRE_FICHERO)
						|| (posicionLinea == POSICION_NOMBRE_CARPETA)) {
					nombre[pos_nombre] = byte;
					pos_nombre++;
					nombre[pos_nombre] = 0;
#ifdef DEBUG_GRADO1
					printf("Valor de nombre en %d: %s\n", posicionLinea, nombre);
#endif
				} else {
					if ((byte >= VALOR_NUMERO_0) && (byte <= VALOR_NUMERO_9)) {
						byte -= VALOR_NUMERO_0;
						valor *= 10;
						valor += byte;
					} else {
						return E_MAL_FORMADO;
					}
				}
				break;
			}
			}
		}

		(*totalDatosLeidos)++;
		byte = data[*totalDatosLeidos];
#ifdef DEBUG_GRADO2
		printf("byte leido: %c\n", byte);
#endif
	}

	//Ponemos esto aqui para que procese el ultimo elemento de la linea, sino no lo hace
	/*Prblema. Si no hay salto de linea, no lo procesa, y lo hay lo procesara dos veces*/
	if ((posicionLinea == POSICION_RAIZ_NOMBRE_FICHERO)
			|| (posicionLinea == POSICION_NOMBRE_CARPETA)) {
		pvalor = (uint32_t *) nombre;
#ifdef DEBUG_GRADO2
		printf("Valor de nombre y pvalor: %s - %s\n", nombre, (char *) pvalor);
#endif
	} else {
		pvalor = (uint32_t *) &valor;
	}

	procesarInformacion(pvalor, posicionLinea, captura);

	/*Sumo uno mas para no leer el salto de linea
	 * la siguiente vez que leamos los datos*/
	(*totalDatosLeidos)++;

	if (/*(byte == 10) && */(byte == 0)) {
		free(captura);
		captura = NULL;
	}

	//deleteDeepCopyStack(stack);
	return SALIDA_OK;
}

uint8_t procesarCadenaAperturaValvulasOld(uint8_t* cadenaApertura,
		uint8_t** dstArray, uint8_t* total_dstArray) {

	uint16_t i = 0;
	uint8_t llave = 0;
	uint8_t parentesis = 0;
	uint32_t valor = 0;
	uint8_t cierreLlaveCercano = VALOR_RESET_LLAVE_CERRADA;

	Stack* stack = createStack();

	while (cadenaApertura[i] != 0) {
		printf("Valor de cadenaApertura[i]: %c\n", cadenaApertura[i]);
		switch (cadenaApertura[i]) {
		case VALOR_APERTURA_LLAVE:
			printf("VALOR_APERTURA_LLAVE: %d y total de llaves: %d\n",
					cadenaApertura[i], llave);
			llave++;
			//pushDeepCopy(stack, &cadenaApertura[i], deepCopyUInt8Value);
			break;

		case VALOR_CIERRE_LLAVE:
			printf("VALOR_CIERRE_LLAVE: %d y total de llaves: %d\n",
					cadenaApertura[i], llave);
			llave--;
			if (llave < 0)
				return E_MAL_FORMADO;

			//Habria que introducir el valor??

			cierreLlaveCercano = VALOR_LLAVE_CERRADA;
			break;

		case VALOR_APERTURA_PARENTESIS:
			/*Si encontramos un parentesis, tenemos que guardar el elemento que tenemos leido, siempre
			 *que no se haya una llave.*/
			if (cierreLlaveCercano == VALOR_NUMERO_LEIDO) {
				pushDeepCopy(stack, &valor, deepCopyUInt8Value);
				cierreLlaveCercano = VALOR_RESET_LLAVE_CERRADA;
			}
			parentesis++;
			valor = 0;
			break;

		case VALOR_CIERRE_PARENTESIS:
			parentesis--;
			if (parentesis < 0)
				return E_MAL_FORMADO;

			if (cierreLlaveCercano == VALOR_LLAVE_CERRADA) {
				procesarGrupoPila(stack, valor);
				cierreLlaveCercano = VALOR_RESET_LLAVE_CERRADA;
			} else
				procesarUltimoElementoPila(stack, valor);

			cierreLlaveCercano = VALOR_CIERRE_PARENTESIS;
			valor = 0;
			break;

		case SEPARADOR_VALVULAS:

			if (cierreLlaveCercano != VALOR_CIERRE_PARENTESIS) {
				printf("SEPARADOR_VALVULAS. Valor :%d \n", valor);
				if (cierreLlaveCercano == VALOR_LLAVE_CERRADA)
					procesarGrupoPila(stack, valor);
				else
					pushDeepCopy(stack, &valor, deepCopyUInt8Value);

				valor = 0; //Reiniciamos tras guardar
			}
			cierreLlaveCercano = VALOR_RESET_LLAVE_CERRADA;
			break;

		default:
			if (cadenaApertura[i] >= '0' && cadenaApertura[i] <= '9') {
				valor *= 10;
				valor += (cadenaApertura[i] - '0');
				printf(
						"Valor leido: %c y valor actual: %d. Valor cierreLlaveCercano: %d\n",
						cadenaApertura[i], valor, cierreLlaveCercano);
				if (cierreLlaveCercano == VALOR_RESET_LLAVE_CERRADA)
					cierreLlaveCercano = VALOR_NUMERO_LEIDO;
			} else {
				return ERROR;
			}
		}
		i++;
	}

	if (cierreLlaveCercano == VALOR_NUMERO_LEIDO) {
		//Una vez llegado al final, procesamos el ultimo valor 
		pushDeepCopy(stack, &valor, deepCopyUInt8Value);

	} else if (cierreLlaveCercano == VALOR_LLAVE_CERRADA) {
		procesarGrupoPila(stack, valor);
	}
	cierreLlaveCercano = VALOR_RESET_LLAVE_CERRADA;

	*total_dstArray = getUInt8ArrayWithStackElements(stack, dstArray);

	printf("procesarCadenaAperturaValvulas > El total de elementos es: %d\n",
			*total_dstArray);
	for (int i = 0; i < *total_dstArray; i++)
		printf("%d ", *(dstArray + i));

	printf("\n");

	return OKK ;
}

uint8_t procesarCadenaAperturaValvulas(uint8_t* cadenaApertura,
		uint8_t** dstArray, uint8_t* total_dstArray,
		uint8_t numElemsCadenaApertura) {

	int16_t i = numElemsCadenaApertura - 1;
	uint8_t llave = 0;
	uint8_t parentesis = 0;
	int32_t valor = 0;
	uint8_t valorPrevio = 0x00;
	uint8_t repeticionesGrupo[0xFF];
	uint8_t posRepGrupo = 0;

	Stack* stack = createStack();

	while (i >= 0) {
#ifdef DEBUG_GRADO2
		printf("Valor de cadenaApertura[i]: %c\n", cadenaApertura[i]);
#endif
		switch (cadenaApertura[i]) {
		case VALOR_APERTURA_LLAVE:
#ifdef DEBUG_GRADO2
			printf("VALOR_APERTURA_LLAVE: %d y total de llaves: %d\n", cadenaApertura[i], llave);
#endif
			llave++;
			valorPrevio = '{';

			if (valor != -1) {
				pushDeepCopy(stack, &valor, deepCopyUInt8Value);
				valor = -1;
			}

			posRepGrupo--;
			//Tratamos los elementos hasta llegar al cierre
			procesarGrupoPila(stack, repeticionesGrupo[posRepGrupo]);
			break;

		case VALOR_CIERRE_LLAVE:
#ifdef DEBUG_GRADO2
			printf("VALOR_CIERRE_LLAVE: %d y total de llaves: %d\n", cadenaApertura[i], llave);
#endif
			llave--;
			if (valorPrevio == '(')
				repeticionesGrupo[posRepGrupo] = valor;
			else
				repeticionesGrupo[posRepGrupo] = 1;
			posRepGrupo++;
			valor = 0;
			valorPrevio = 0x00;
			//Introducimos el valor de cierre para indicar que hemos llegado al final del grupo
			pushDeepCopy(stack, &cadenaApertura[i], deepCopyUInt8Value);
			valorPrevio = '}';
			break;

		case VALOR_APERTURA_PARENTESIS:
			parentesis++;
			valorPrevio = '(';
			break;

		case VALOR_CIERRE_PARENTESIS:
			parentesis--;
			valor = 0;
			break;

		case SEPARADOR_VALVULAS:

			if ((valorPrevio != VALOR_APERTURA_LLAVE)
					&& (valorPrevio != VALOR_APERTURA_PARENTESIS)) {
				pushDeepCopy(stack, &valor, deepCopyUInt8Value);
			}

			if (valorPrevio == VALOR_APERTURA_PARENTESIS) {
				procesarUltimoElementoPila(stack, valor);
			}
			valorPrevio = 0x00;
			valor = 0;
			break;

		default:
			if (cadenaApertura[i] >= '0' && cadenaApertura[i] <= '9') {
				valor *= 10;
				valor += (cadenaApertura[i] - '0');
#ifdef DEBUG_GRADO2
				printf("Valor leido: %c y valor actual: %d\n", cadenaApertura[i], valor);
#endif
			} else {
				return ERROR;
			}
		}
		i--;
	}

	*total_dstArray = getUInt8ArrayWithStackElements(stack, dstArray);

#ifdef DEBUG_GRADO2
	printf("procesarCadenaAperturaValvulas > El total de elementos es: %d\n", *total_dstArray);
	for (int i = 0; i < *total_dstArray; i++)
		printf("%d ", *(dstArray + i));

	printf("\n");
#endif

	return OKK ;
}

uint8_t procesarInformacion(uint32_t* valor, uint8_t posicionLinea,
		struct Captura* captura) {
	if (!captura) {
		return ERROR;
	}

	switch (posicionLinea) {
	case POSICION_MODULACION: {
		if ((*valor < 0) || (*valor > 3)) {
			return E_VALORES_INCORRECTOS;
		} else {
			P_CAPT_MODULACION(captura) = *valor;
			cargarFuncionCapturaOdorantes((uint8_t*)valor, captura);
		}
		break;
	}
	case POSICION_SUCCION: {
		P_CAPT_SUCCION(captura) = *valor;
		break;
	}
	case POSICION_DURACION_ODORANTE: {
		P_CAPT_TIEMPO_ANALISIS_ODOR(captura) = *valor;
		break;
	}
	case POSICION_ORDEN_VALVULAS: {
		if (!P_CAPT_ORDEN_VALVULAS(captura))
			P_CAPT_ORDEN_VALVULAS(captura) = (uint8_t*) valor;
		else
			P_CAPT_TOTAL_VALVULAS(captura) = (uint8_t) *valor;
		break;
	}
	case POSICION_TEMPERATURA_SENSOR: {
		P_CAPT_TEMPERATURA_SENSOR(captura) = *valor;
		break;
	}
	case POSICION_RAIZ_NOMBRE_FICHERO: {
		//Importante hacer el casting para obtener los tamanios adecuados del puntero
		char *pvalor = (char *) valor;
#ifdef DEBUG_GRADO2
		printf("POSICION_RAIZ_NOMBRE_FICHERO - Valor: %s, %p\n", (char *) valor, valor);
#endif
		//raiz nombre comun para ficheros
		uint8_t* raizNombre = P_CAPT_FILE_ROOT(captura);

		while (*pvalor != 0) {
#ifdef DEBUG_GRADO2
			printf("Caracter valor: %c\n", *pvalor);
#endif
			*raizNombre++ = *pvalor++;
		}
		*raizNombre = 0;

#ifdef DEBUG_GRADO2
		printf("Valor raiz nombre fichero: %s\n", P_CAPT_FILE_ROOT(captura));
#endif
		break;
	}
	case POSICION_NOMBRE_CARPETA: {
		//Importante hacer el casting para obtener los tamanios adecuados del puntero
		char *pvalor = (char *) valor;
#ifdef DEBUG_GRADO2
		printf("POSICION_NOMBRE_CARPETA - Valor: %s\n", (char*) valor);
#endif
		//nombre carpeta donde guardar los ficheros
		uint8_t* carpeta = P_CAPT_PATH(captura);
		while (*pvalor != 0) {
#ifdef DEBUG_GRADO2
			printf("Caracter valor: %c\n", *pvalor);
#endif
			*carpeta++ = *pvalor++;
		}
		*carpeta = 0;

#ifdef DEBUG_GRADO2
		printf("Valor nombre carpeta: %s\n", P_CAPT_PATH(captura));
#endif
		break;
	}
	default: {
		if (P_CAPT_TOTAL_VALORES_EXTRA(captura) < 20) {
			P_CAPT_GUARDAR_VALORES_EXTRA(captura, *valor);
			P_CAPT_TOTAL_VALORES_EXTRA(captura)++;
		} else {
			return ERROR;
		}
	}
	}

	return SALIDA_OK;
}

void procesarUltimoElementoPila(Stack* stack, uint32_t valor) {

#ifdef DEBUG_GRADO1
	printf("procesarUltimoElementoPila > nº repeticiones: %d\n", valor);
#endif
	uint32_t valorPop = *(uint32_t *) pop(stack);

#ifdef DEBUG_GRADO2
	printf("procesarUltimoElementoPila > valorPop: %d\n", valorPop);
#endif

	for (uint32_t i = 0; i < valor; i++) {
#ifdef DEBUG_GRADO2
		printf("procesarUltimoElementoPila > introduccion pila: %d\n", i);
#endif
		pushDeepCopy(stack, &valorPop, deepCopyInt32Value);
	}

	return;
}

void procesarGrupoPila(Stack* stack, uint32_t valor) {

	uint8_t valorPop = *(uint8_t *) pop(stack);
	LinkedList* list = createLinkedList();

	while (valorPop != VALOR_CIERRE_LLAVE) {
#ifdef DEBUG_GRADO2
		printf("procesarGrupoPila > valor leido de la pila: %d\n", valorPop);
#endif
		appendElemDeepCopy(list, &valorPop, deepCopyUInt8Value);
		valorPop = *(uint8_t *) pop(stack);
	}

	for (uint32_t i = 0; i < valor; i++) {
		for (int32_t j = (getNElems(list) - 1); j >= 0; j--) {
			valorPop = *(uint8_t *) getLinkedListElem(list, j);
#ifdef DEBUG_GRADO2
			printf("procesarGrupoPila > valor pop al final (lista): %d\n", valorPop);
#endif
			pushDeepCopy(stack, &valorPop, deepCopyUInt8Value);
		}
	}

	deleteLinkedList(list);
	return;
}

void cargarFuncionCapturaOdorantes(uint8_t* modulacion, struct Captura* captura) {

	switch (*modulacion) {
	case MPURO:
		captura->funcion = &captura_secuencia_odorantes_completa_puro;
		break;
	case MREGRESION:
		//captura->funcion = NULL;
		break;
	case MMARTINELLI:
		//captura->funcion = NULL;
		break;
	}
}

/****** - TENEMOS LAS FUNCIONES DE IMPLEMENTACION DE LECTURA PARA CONFIGURACION - *******/

/*Para obtener una secuencia, solamente tiene que leer una linea. Luego se captura y
 se lee otra linea para otra secuencia */
uint8_t obtenerSecuenciaConfiguracion(const char* data,
		uint16_t* totalDatosLeidos, struct Configuracion* conf) {

	if (data == NULL) {
		printf("Descriptor de fichero no valido\n");
		return ERROR;
	}

	uint16_t posCab = 0;
	uint8_t byte = 0;
	uint8_t posicionLinea = 0;

	uint8_t lineaEncontrada = 0;
	uint8_t* ptr_data = NULL;
	uint8_t pos = 0;
	uint8_t posVal = 1;
	uint8_t valor_leido = 0;
	uint8_t pos_read = 0;

	byte = data[*totalDatosLeidos];
#ifdef DEBUG_GRADO2
	printf("byte leido: %c\n", byte);
#endif
	while ((byte != 0) && (byte != VALOR_SALTO_LINEA)) {
		if ((cabeceraConf[posCab] == byte) && (lineaEncontrada == 0)) {
			posCab++;
			if (cabeceraConf[posCab] == 0) {
				lineaEncontrada = 1;
#ifdef DEBUG_GRADO1
				printf("LINEA ENCONTRADA\n");
#endif
				(*totalDatosLeidos)++;
				byte = data[*totalDatosLeidos];
#ifdef DEBUG_GRADO2
				printf("byte leido: %c\n", byte);
#endif

				ptr_data = &conf->sensorReadPin[0];
				ptr_data[5] = 0;
			}
		}

		if (lineaEncontrada == 1) {
			switch (byte) {
			case VALOR_COMA: {
				pos++;
				switch (pos) {
				case SENSOR_HEAT_PIN:
					ptr_data = &conf->sensorHeatPin[0];
					conf->sensorHeatPin[5] = 0;
					break;
				case MOTOR_CTRL_PIN:
					ptr_data = &conf->motorCtrlPin[0];
					conf->motorCtrlPin[5] = 0;
					break;
				case FREC_CAPT_SAMPLES:
					ptr_data = &conf->frecCaptSamples;
					break;
				case FREC_CAPT_SAMPLES_TH:
					ptr_data = &conf->frecCaptSamplesTH;
					break;
				case VCC_VALUE:
					ptr_data = &conf->VCC;
					break;
				case TH_READ_PIN:
					ptr_data = &conf->THReadPin[0];
					conf->THReadPin[5] = 0;
					break;
				case SUB_SAMPLES_SAMPLE:
					ptr_data = &conf->subSamplesForSample;
					break;
				case FREC_CAPT_SUB_SAMPLES:
					ptr_data = &conf->frecCaptSubSamples;
					break;
				case N_ELECTROVALVULAS:
					ptr_data = &conf->electrovalvulas[0][0];
					ptr_data[5] = 0;
					break;
				case RESISTENCIA_VALUE:
					ptr_data = &conf->resistencia;
					break;
				}
				pos_read = 0;
				*ptr_data = 0;
				valor_leido = NUEVO;
#ifdef DEBUG_GRADO1
				printf("posicion linea: %d\n", posicionLinea);
#endif

				posicionLinea++;
				break;
			}
				//Salto de linea
			case SEPARADOR_VALVULAS: {
				ptr_data[pos_read] = 0;
				ptr_data = &conf->electrovalvulas[posVal][0];
				posVal++;
				pos_read = 0;

				break;
			}

			default: {
				if (((valor_leido == NUEVO) && (byte == 'P'))
						|| (valor_leido == CADENA)) {
					ptr_data[pos_read] = byte;
					valor_leido = CADENA;
				} else if (((valor_leido == NUEVO)
						&& ((byte >= '0') && (byte <= '9')))
						|| (valor_leido == NUMERO)) {
					(*ptr_data) *= 10;
					(*ptr_data) += (byte - '0');
					valor_leido = NUMERO;
				} else {
					//ERROR
				}
				pos_read++;
				break;
			}
			}
		}

		(*totalDatosLeidos)++;
		byte = data[*totalDatosLeidos];
#ifdef DEBUG_GRADO2
		printf("byte leido: %c\n", byte);
#endif
	}

	/*Sumo uno mas para no leer el salto de linea
	 * la siguiente vez que leamos los datos*/
	(*totalDatosLeidos)++;
	conf->totalElectrovalvulas = posVal;
	ptr_data[pos_read] = 0;

	return SALIDA_OK;
}
