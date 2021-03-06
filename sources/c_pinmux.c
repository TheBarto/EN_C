/*
 * c_pinmux.c
 *
 *  Created on: 16 oct. 2021
 *      Author: afuenteo.
 *
 *      Codigo copiado de https://github.com/adafruit/adafruit-beaglebone-io-python/blob/master/source/c_pinmux.c. No realizado por mi.
 *
 *      Realizado peque?os cambios en defines y variables para adaptarlo a mis constantes.
 */


#include <stdio.h>
#include <string.h>
//#include <syslog.h>

#include "../headers/c_pinmux.h"

uint8_t set_pin_mode(const char *key, const char *mode)
{
	// char ocp_dir[] defined in common.h
	char path[100]; // "/sys/devices/platform/ocp/ocp:P#_##_pinmux/state"
	char pinmux_dir[20]; // "ocp:P#_##_pinmux"
	char pin[6]; //"P#_##"
	FILE *f = NULL;

	// pin mode can not be set for the built-in USRn LEDs
	if (strncmp(key, "USR", 3) == 0) {
		return OK;
	}

	if (strlen(key) == 4)	// Key P#_# format, must inject '0' to be P#_0#
		snprintf(pin, sizeof(pin), "%.3s0%c", key,key[3]);
	else	//copy string
		snprintf(pin, sizeof(pin), "%s", key);

#ifndef BBBVERSION41
	 uint8_t err = build_path("/sys/devices/platform", "ocp", ocp_dir, sizeof(ocp_dir));
	if (err != OK) {
		return err;
	}
#else
	strncpy(ocp_dir, "/sys/devices/platform/ocp", sizeof(ocp_dir));
#endif

	snprintf(pinmux_dir, sizeof(pinmux_dir), "ocp:%s_pinmux", pin);
	snprintf(path, sizeof(path), "%s/%s/state", ocp_dir, pinmux_dir);

	/* beaglebone blue has complete dtb file and does not need overlays */
	if(beaglebone_blue()) {
		fprintf(stderr, "DEBUG: Adafruit_BBIO: set_pin_mode() :: Pinmux file: %s, mode: %s", path, mode);
		fprintf(stderr, "DEBUG: Adafruit_BBIO: set_pin_mode(): beaglebone_blue() is TRUE; return BBIO_OK\n");
		return OK;
	}


	f = fopen(path, "w");
	if (NULL == f) {
		return E_ACCESO;
	}
	//syslog(LOG_DEBUG, "Adafruit_BBIO: set_pin_mode() :: Pinmux file %s access OK", path);
	fprintf(f, "%s", mode);
	fclose(f);
	//syslog(LOG_DEBUG, "Adafruit_BBIO: set_pin_mode() :: Set pinmux mode to %s for %s", mode, pin);
	return OK;
}
