#ifndef COMUN_H
#define COMUN_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#ifdef __unix__
#include <glob.h>
#include <unistd.h>
#else
#include <windows.h>
#endif
#include <string.h>
//#include <time.h>
#include <errno.h>


#define OK ((uint8_t) 0)
#define CTRL_DIR_MAX ((uint8_t) 50)
#define OCP_DIR_MAX  ((uint8_t) 50)
#define TOTAL_PINES	((uint8_t) 189)

#define ARRAY_SIZE(a)  (sizeof(a) / sizeof(a[0]))

typedef uint8_t BBIO_err;

//¿Poner como máscara?
#define E_ACCESO			((uint8_t) 1)
#define E_SISTEMA_FIC		((uint8_t) 2)
#define E_CAPA_MANEJADOR	((uint8_t) 3)
#define E_ARG_INVALIDO		((uint8_t) 4)
#define E_MEMORIA			((uint8_t) 5)
#define E_GENERICO			((uint8_t) 6)


//#define ERROR				((uint8_t) 1)
extern uint8_t configuracion_modulo;
extern uint8_t modulo_GPIO;
extern uint8_t error_conf;
extern uint8_t direccion_GPIO[120];

extern char ctrl_dir[CTRL_DIR_MAX];
extern char ocp_dir[OCP_DIR_MAX];

typedef struct pins_t
{
	const char* name;
	const char* key;
	uint16_t gpio;
	int8_t pwm_mux_mode;
	int8_t ain;
}pins_t;

extern pins_t table[TOTAL_PINES];

typedef struct pwm_t {
  const char *module;
  const int sysfs;
  const int index;
  const int muxmode;
  const char *path;
  const char *name;
  const char *chip;
  const char *addr;
  const char *key;  // Pin name eg P9_21
} pwm_t;

int uboot_overlay_enabled(void);

int beaglebone_blue(void);
int pocketbeagle(void);

BBIO_err build_path(const char *partial_path, const char *prefix, char *full_path, size_t full_path_len);
BBIO_err load_device_tree(const char *name);
BBIO_err unload_device_tree(const char *name);

#endif