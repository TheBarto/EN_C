#include <stdio.h>
#include <stdlib.h>
#include "headers/moduloLecturaFicheros.h"
#include "headers/modulacion_comun.h"
#include "headers/comun.h"

int main(int arc, char* argv[])
{

	setbuf(stdout, NULL);

	struct Captura captura;
	struct Configuracion conf;
	char data[TOTAL_DATA_READED_FILE];
	uint16_t totalDatosLeidos = 0;

	inicializar_puertos_ADC();
	inicializar_modulo();

	/* Abrimos el fichero de conf y leemos*/
	aperturaFichero(argv[2], data);
	obtenerSecuenciaConfiguracion(data, &totalDatosLeidos, &conf);

	aperturaFichero(argv[1], data);
	totalDatosLeidos = 0;

	uint8_t ret = obtenerSecuenciaCapturaDelFichero(data, &totalDatosLeidos, &captura);
	while(ret == OK)
	{
		captura_secuencias_completas_puro(&captura, &conf);
		ret = obtenerSecuenciaCapturaDelFichero(data, &totalDatosLeidos, &captura);
	}

	return OK;
}
