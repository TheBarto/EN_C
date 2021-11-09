/*
 * comun.c
 *
 *  Created on: 16 oct. 2021
 *      Author: afuenteo
 */

#include "../headers/comun.h"

uint8_t configuracion_modulo = 0;
uint8_t error_conf = 0;

// Tabla con todos los pines de las BB.
pins_t table[TOTAL_PINES] = {
  { "USR0", "USR0", 53, -1, -1},
  { "USR1", "USR1", 54, -1, -1},
  { "USR2", "USR2", 55, -1, -1},
  { "USR3", "USR3", 56, -1, -1},
  { "DGND", "P8_1", 0, -1, -1},
  { "DGND", "P8_2", 0, -1, -1},
  { "GPIO1_6", "P8_3", 38, -1, -1},
  { "GPIO1_7", "P8_4", 39, -1, -1},
  { "GPIO1_2", "P8_5", 34, -1, -1},
  { "GPIO1_3", "P8_6", 35, -1, -1},
  { "TIMER4", "P8_7", 66, 2, -1},
  { "TIMER7", "P8_8", 67, 2, -1},
  { "TIMER5", "P8_9", 69, 2, -1},
  { "TIMER6", "P8_10", 68, 2, -1},
  { "GPIO1_13", "P8_11", 45, -1, -1},
  { "GPIO1_12", "P8_12", 44, -1, -1},
  { "EHRPWM2B", "P8_13", 23, 4, -1},
  { "GPIO0_26", "P8_14", 26, -1, -1},
  { "GPIO1_15", "P8_15", 47, -1, -1},
  { "GPIO1_14", "P8_16", 46, -1, -1},
  { "GPIO0_27", "P8_17", 27, -1, -1},
  { "GPIO2_1", "P8_18", 65, -1, -1},
  { "EHRPWM2A", "P8_19", 22, 4, -1},
  { "GPIO1_31", "P8_20", 63, -1, -1},
  { "GPIO1_30", "P8_21", 62, -1, -1},
  { "GPIO1_5", "P8_22", 37, -1, -1},
  { "GPIO1_4", "P8_23", 36, -1, -1},
  { "GPIO1_1", "P8_24", 33, -1, -1},
  { "GPIO1_0", "P8_25", 32, -1, -1},
  { "GPIO1_29", "P8_26", 61, -1, -1},
  { "GPIO2_22", "P8_27", 86, -1, -1},
  { "GPIO2_24", "P8_28", 88, -1, -1},
  { "GPIO2_23", "P8_29", 87, -1, -1},
  { "GPIO2_25", "P8_30", 89, -1, -1},
  { "UART5_CTSN", "P8_31", 10, -1, -1},
  { "UART5_RTSN", "P8_32", 11, -1, -1},
  { "UART4_RTSN", "P8_33", 9, -1, -1},
  { "UART3_RTSN", "P8_34", 81, 2, -1},
  { "UART4_CTSN", "P8_35", 8, -1, -1},
  { "UART3_CTSN", "P8_36", 80, 2, -1},
  { "UART5_TXD", "P8_37", 78, -1, -1},
  { "UART5_RXD", "P8_38", 79, -1, -1},
  { "GPIO2_12", "P8_39", 76, -1, -1},
  { "GPIO2_13", "P8_40", 77, -1, -1},
  { "GPIO2_10", "P8_41", 74, -1, -1},
  { "GPIO2_11", "P8_42", 75, -1, -1},
  { "GPIO2_8", "P8_43", 72, -1, -1},
  { "GPIO2_9", "P8_44", 73, -1, -1},
  { "GPIO2_6", "P8_45", 70, 3, -1},
  { "GPIO2_7", "P8_46", 71, 3, -1},
  { "DGND", "P9_1", 0, -1, -1},
  { "DGND", "P9_2", 0, -1, -1},
  { "VDD_3V3", "P9_3", 0, -1, -1},
  { "VDD_3V3", "P9_4", 0, -1, -1},
  { "VDD_5V", "P9_5", 0, -1, -1},
  { "VDD_5V", "P9_6", 0, -1, -1},
  { "SYS_5V", "P9_7", 0, -1, -1},
  { "SYS_5V", "P9_8", 0, -1, -1},
  { "PWR_BUT", "P9_9", 0, -1, -1},
  { "SYS_RESETn", "P9_10", 0, -1, -1},
  { "UART4_RXD", "P9_11", 30, -1, -1},
  { "GPIO1_28", "P9_12", 60, -1, -1},
  { "UART4_TXD", "P9_13", 31, -1, -1},
  { "EHRPWM1A", "P9_14", 50, 6, -1},
  { "GPIO1_16", "P9_15", 48, -1, -1},
  { "EHRPWM1B", "P9_16", 51, 6, -1},
  { "I2C1_SCL", "P9_17", 5, -1, -1},
  { "I2C1_SDA", "P9_18", 4, -1, -1},
  { "I2C2_SCL", "P9_19", 13, -1, -1},
  { "I2C2_SDA", "P9_20", 12, -1, -1},
  { "UART2_TXD", "P9_21", 3, 3, -1},
  { "UART2_RXD", "P9_22", 2, 3, -1},
  { "GPIO1_17", "P9_23", 49, -1, -1},
  { "UART1_TXD", "P9_24", 15, -1, -1},
  { "GPIO3_21", "P9_25", 117, -1, -1},
  { "UART1_RXD", "P9_26", 14, -1, -1},
  { "GPIO3_19", "P9_27", 115, -1, -1},
  { "SPI1_CS0", "P9_28", 113, 4, -1},
  { "SPI1_D0", "P9_29", 111, 1, -1},
  { "SPI1_D1", "P9_30", 112, -1, -1},
  { "SPI1_SCLK", "P9_31", 110, 1, -1},
  { "VDD_ADC", "P9_32", 0, -1, -1},
  { "AIN4", "P9_33", 0, -1, 4},
  { "GNDA_ADC", "P9_34", 0, -1, -1},
  { "AIN6", "P9_35", 0, -1, 6},
  { "AIN5", "P9_36", 0, -1, 5},
  { "AIN2", "P9_37", 0, -1, 2},
  { "AIN3", "P9_38", 0, -1, 3},
  { "AIN0", "P9_39", 0, -1, 0},
  { "AIN1", "P9_40", 0, -1, 1},
  { "CLKOUT2", "P9_41", 20, -1, -1},
  { "GPIO0_7", "P9_42", 7, 0, -1},
  { "DGND", "P9_43", 0, -1, -1},
  { "DGND", "P9_44", 0, -1, -1},
  { "DGND", "P9_45", 0, -1, -1},
  { "DGND", "P9_46", 0, -1, -1},

  // These are for the Blue
    { "GP0_3", "GP0_3",  57, -1, -1},
    { "GP0_4", "GP0_4",  49, -1, -1},
    { "GP0_5", "GP0_5", 116, -1, -1},
    { "GP0_6", "GP0_6", 113, -1, -1},
    { "GP1_3", "GP1_3",  98, -1, -1},
    { "GP1_4", "GP1_4",  97, -1, -1},
    { "RED_LED",   "RED",    66, -1, -1},   // LEDs
    { "GREEN_LED", "GREEN",  67, -1, -1},
    { "BAT25", "BAT25",  27, -1, -1},
    { "BAT50", "BAT50",  11, -1, -1},
    { "BAT75", "BAT75",  61, -1, -1},
    { "BAT100", "BAT100",  10000, -1, -1}, // Placeholder
    { "WIFI", "WIFI",  10001, -1, -1}, // Placeholder

    { "PAUSE", "P8_9",  69, 1, -1},
    { "MODE",  "P8_10", 68, 1, -1},

  // These are for the PocketBeagle
  { "VIN_AC", "P1_1", 0, -1, -1},
  { "GPIO2_23", "P1_2", 87, -1, -1},
  { "USB1_DRVVBUS", "P1_3", 0, -1, -1},
  { "GPIO2_25", "P1_4", 89, -1, -1},
  { "USB1_VBUS_IN", "P1_5", 0, -1, -1},
  { "SPI0_CS0", "P1_6", 5, -1, -1},
  { "VIN-USB", "P1_7", 0, -1, -1},
  { "SPI0_SCLK", "P1_8", 2, 3, -1},
  { "USB1-DN", "P1_9", 0, -1, -1},
  { "SPI0_D0", "P1_10", 3, 3, -1},
  { "USB1-DP", "P1_11", 0, -1, -1},
  { "SPI0_D1", "P1_12", 4, -1, -1},
  { "USB1-ID", "P1_13", 0, -1, -1},
  { "VOUT-3.3V", "P1_14", 0, -1, -1},
  { "GND", "P1_15", 0, -1, -1},
  { "GND", "P1_16", 0, -1, -1},
  { "VREFN", "P1_17", 0, -1, -1},
  { "VREFP", "P1_18", 0, -1, -1},
  { "AIN0", "P1_19", 0, -1, 0},
  { "GPIO0_20", "P1_20", 20, 4, -1},
  { "AIN1", "P1_21", 0, -1, 1},
  { "GND", "P1_22", 0, -1, -1},
  { "AIN2", "P1_23", 0, -1, 2},
  { "VOUT-5V", "P1_24", 0, -1, -1},
  { "AIN3", "P1_25", 0, -1, 3},
  { "I2C2_SDA", "P1_26", 12, 1, -1},
  { "AIN4", "P1_27", 0, -1, 4},
  { "I2C2_SCL", "P1_28", 13, 1, -1},
  { "GPIO3_21", "P1_29", 117, -1, -1},
  { "UART0_TXD", "P1_30", 43, -1, -1},
  { "GPIO3_18", "P1_31", 114, -1, -1},
  { "UART0_RXD", "P1_32", 42, -1, -1},
  { "GPIO3_15", "P1_33", 111, 1, -1},
  { "GPIO0_26", "P1_34", 26, -1, -1},
  { "GPIO2_24", "P1_35", 88, -1, -1},
  { "EHRPWM0A", "P1_36", 110, 1, -1},
  { "EHRPWM1A", "P2_1", 50, 6, -1},
  { "GPIO1_27", "P2_2", 59, -1, -1},
  { "GPIO0_23", "P2_3", 23, 4, -1},
  { "GPIO1_26", "P2_4", 58, -1, -1},
  { "UART4_RXD", "P2_5", 30, -1, -1},
  { "GPIO1_25", "P2_6", 57, -1, -1},
  { "UART4_TXD", "P2_7", 31, -1, -1},
  { "GPIO1_28", "P2_8", 60, -1, -1},
  { "I2C1_SCL", "P2_9", 15, -1, -1},
  { "GPIO1_20", "P2_10", 52, -1, -1},
  { "I2C1_SDA", "P2_11", 14, -1, -1},
  { "POWER_BUTTON", "P2_12", 0, -1, -1},
  { "VOUT-5V", "P2_13", 0, -1, -1},
  { "BAT-VIN", "P2_14", 0, -1, -1},
  { "GND", "P2_15", 0, -1, -1},
  { "BAT-TEMP", "P2_16", 0, -1, -1},
  { "GPIO2_1", "P2_17", 65, -1, -1},
  { "GPIO1_15", "P2_18", 47, -1, -1},
  { "GPIO0_27", "P2_19", 27, -1, -1},
  { "GPIO2_0", "P2_20", 64, -1, -1},
  { "GND", "P2_21", 0, -1, -1},
  { "GPIO1_14", "P2_22", 46, -1, -1},
  { "VOUT-3.3V", "P2_23", 0, -1, -1},
  { "GPIO1_12", "P2_24", 44, -1, -1},
  { "SPI1_CS0", "P2_25", 41, -1, -1},
  { "RESET#", "P2_26", 0, -1, -1},
  { "SPI1_D0", "P2_27", 40, 5, -1},
  { "GPIO3_20", "P2_28", 116, -1, -1},
  { "SPI1_SCLK", "P2_29", 7, -1, -1},
  { "GPIO3_17", "P2_30", 113, -1, -1},
  { "SPI1_CS1", "P2_31", 19, 2, -1},
  { "GPIO3_16", "P2_32", 112, -1, -1},
  { "GPIO1_13", "P2_33", 45, -1, -1},
  { "GPIO3_19", "P2_34", 115, -1, -1},
  { "GPIO2_22", "P2_35", 86, -1, -1},
  { "AIN7", "P2_36", 0, -1, 7},
  { NULL, NULL, 0, 0, 0 }
};

char ctrl_dir[CTRL_DIR_MAX];
char ocp_dir[OCP_DIR_MAX];
uint8_t direccion_GPIO[120];

BBIO_err load_device_tree(const char *name)
{
    char line[256];
    FILE *file = NULL;
    char slots[100];
#ifdef BBBVERSION41
    snprintf(ctrl_dir, sizeof(ctrl_dir), "/sys/devices/platform/bone_capemgr");
#else
    build_path("/sys/devices", "bone_capemgr", ctrl_dir, sizeof(ctrl_dir));
#endif

    /* Check if the Linux kernel booted with u-boot overlays enabled.
       If enabled, do not load overlays via slots file as the write
       will hang due to kernel bug in cape manager driver. Just return
       BBIO_OK in order to avoid cape manager bug. */
    if(uboot_overlay_enabled()) {
      return OK;
    }

    /* beaglebone blue has complete dtb file and does not need overlays */
    if(beaglebone_blue()) {
      //fprintf(stderr, "common.c: load_device_tree(): beaglebone_blue(): TRUE\n");
      return OK;
    }

    /* pocketbeagle has complete dtb file and does not need overlays */
    if(pocketbeagle()) {
      //fprintf(stderr, "common.c: load_device_tree(): pocketbeagle(): TRUE\n");
      return OK;
    }

    snprintf(slots, sizeof(slots), "%s/slots", ctrl_dir);

    file = fopen(slots, "r+");
    if (!file) {
#ifdef NO_PYTHON
        PyErr_SetFromErrnoWithFilename(PyExc_IOError, slots);
#endif // !NO_PYTHON
        return E_CAPA_MANEJADOR;
    }

    while (fgets(line, sizeof(line), file)) {
        //the device is already loaded, return 1
        if (strstr(line, name)) {
            fclose(file);
            return OK;
        }
    }

    //if the device isn't already loaded, load it, and return
    fprintf(file, "%s", name);
    fclose(file);

    //0.2 second delay
#ifdef __unix__
    nanosleep((struct timespec[]){{0, 200000000L}}, NULL);
#endif

    return OK;
}

/*
   Check if board is a BeagleBone Blue
   Refer to GitHub issue for more information:
   https://github.com/adafruit/adafruit-beaglebone-io-python/issues/178
*/
int beaglebone_blue(void) {
#ifdef __unix__
    const char *cmd = "/bin/grep -c 'TI AM335x BeagleBone Blue' /proc/device-tree/model";
    // cache the value to avoid poor performance
    // in functions that are called frequently like
    // gpio_set_value() in source/event_gpio.c
    static int initialized = 0;
    static int retval = 0;
    FILE *file = NULL;

    //fprintf(stderr, "beaglebone_blue(): initialized=[%d] retval=[%d]\n", initialized, retval);
    if(!initialized) {
      initialized = 1;
      //fprintf(stderr, "beaglebone_blue(): not initialized\n");

      file = popen(cmd, "r");
      if (file == NULL) {

         fprintf(stderr, "Adafruit_BBIO: error in beaglebone_blue(): failed to run cmd=%s\n", cmd);
         syslog(LOG_ERR, "Adafruit_BBIO: error in beaglebone_blue(): failed to run cmd=%s\n", cmd);

         return -1;
      }
      if( fgetc(file) == '1' ) {
         retval = 1;
      }
      pclose(file);
    }

    return retval;
#else
    return 0;
#endif
}

/*
   PocketBeagle has everything built-in to the main dtb
*/
int pocketbeagle(void) {
#ifdef __unix__
    const char *cmd = "/bin/grep -c 'TI AM335x PocketBeagle' /proc/device-tree/model";
    char pocketbeagle;
    FILE *file = NULL;

    file = popen(cmd, "r");
    if (file == NULL) {

       fprintf(stderr, "error: pocketbeagle() failed to run cmd=%s\n", cmd);
       syslog(LOG_ERR, "Adafruit_BBIO: error: pocketbeagle() failed to run cmd=%s\n", cmd);
       return -1;
    }
    pocketbeagle = fgetc(file);
    pclose(file);

    if(pocketbeagle == '1') {
      return 1;
    } else {
      return 0;
    }
#else
   return 0;
#endif
}

#ifdef __unix__
BBIO_err build_path(const char *partial_path, const char *prefix, char *full_path, size_t full_path_len)
{
    glob_t results;
    size_t len = strlen(partial_path) + strlen(prefix) + 5;
    char *pattern = malloc(len);
    snprintf(pattern, len, "%s/%s*", partial_path, prefix);

/*  int glob(const char *pattern, int flags,
                int (*errfunc) (const char *epath, int eerrno),
                glob_t *pglob); */
    int err = glob(pattern, 0, NULL, &results);
    free(pattern);
    if (err != OK) {
        globfree(&results);
        if (err == GLOB_NOSPACE)
            return E_MEMORIA;
        else
            return E_GENERICO;
    }

    // We will return the first match
    strncpy(full_path, results.gl_pathv[0], full_path_len);

    // Free memory
    globfree(&results);

    return OK;
}
#else
BBIO_err build_path(const char *partial_path, const char *prefix, char *full_path, size_t full_path_len)
{
	return OK;
}
#endif

BBIO_err unload_device_tree(const char *name)
{
    FILE *file = NULL;
    char slots[100];
#ifdef BBBVERSION41
    snprintf(ctrl_dir, sizeof(ctrl_dir), "/sys/devices/platform/bone_capemgr");
#else
    build_path("/sys/devices", "bone_capemgr", ctrl_dir, sizeof(ctrl_dir));
#endif
    char line[256];
    char *slot_line;

    /* beaglebone blue has complete dtb file and does not need overlays */
    if(beaglebone_blue()) {
      //fprintf(stderr, "common.c: unload_device_tree(): beaglebone_blue(): TRUE\n");
      return OK;
    }

    /* pocketbeagle has complete dtb file and does not need overlays */
    if(pocketbeagle()) {
      //fprintf(stderr, "common.c: unload_device_tree(): pocketbeagle(): TRUE\n");
      return OK;
    }

    snprintf(slots, sizeof(slots), "%s/slots", ctrl_dir);
    file = fopen(slots, "r+");
    if (!file) {
#ifdef PYTHON
        PyErr_SetFromErrnoWithFilename(PyExc_IOError, slots);
#endif // !NO_PYTHON
        return E_SISTEMA_FIC;
    }

    while (fgets(line, sizeof(line), file)) {
        //the device is loaded, let's unload it
        if (strstr(line, name)) {
            slot_line = strtok(line, ":");
            //remove leading spaces
            while(*slot_line == ' ')
                slot_line++;

            fprintf(file, "-%s", slot_line);
            fclose(file);
            return OK;
        }
    }

    //not loaded, close file
    fclose(file);

    return OK;
}

/*
   Refer to: http://elinux.org/Beagleboard:BeagleBoneBlack_Debian#U-Boot_Overlays
   Robert C. Nelson maintains the BeagleBoard.org Debian images and
   suggested adding this check to see if u-boot overlays are enabled.
   If u-boot overlays are enabled, then device tree overlays should not
   be loaded with the cape manager by writing to the slots file.  There
   is currently a kernel bug that causes the write to hang.
*/
int uboot_overlay_enabled(void) {
#ifdef __unix__
    const char *cmd = "/bin/grep -c bone_capemgr.uboot_capemgr_enabled=1 /proc/cmdline";
    char uboot_overlay;
    FILE *file = NULL;


    file = popen(cmd, "r");

    if (file == NULL) {
       fprintf(stderr, "error: uboot_overlay_enabled() failed to run cmd=%s\n", cmd);
       syslog(LOG_ERR, "Adafruit_BBIO: error: uboot_overlay_enabled() failed to run cmd=%s\n", cmd);

       return -1;
    }
    uboot_overlay = fgetc(file);
    pclose(file);

    if(uboot_overlay == '1') {

      syslog(LOG_DEBUG, "Adafruit_BBIO: uboot_overlay_enabled() is true\n");

      return 1;
    } else {

      syslog(LOG_DEBUG, "Adafruit_BBIO: uboot_overlay_enabled() is false\n");

      return 0;
    }
#else
    return 0;
#endif
}
