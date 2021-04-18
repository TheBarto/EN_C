#ifndef MODULOLECTURAFICHEROS_H
#define MODULOLECTURAFICHEROS_H

char cabeceraLinea[] = {"EN_CAPTURE"};



void* aperturaFichero(const char* nombre);

void procesarContenidoFichero(char* contenido);

void obtenerLineaFichero(char* contenido, char* linea);

#endif