#ifndef COMUN_H
#define COMUN_H

#include <stdint.h>
#include <stdlib.h>


#define OK ((uint8_t) 0)

//¿Poner como máscara?
#define E_ACCESO			((uint8_t) 1)
#define E_SISTEMA_FIC		((uint8_t) 2)
#define E_CAPA_MANEJADOR	((uint8_t) 3)
#define E_ARG_INVALIDO		((uint8_t) 4)
#define E_MEMORIA			((uint8_t) 5)
#define E_GENERICO			((uint8_t) 6)


//#define ERROR				((uint8_t) 1)

typedef struct pins_t
{
	const char* name;
	const char* key;
	uint16_t gpio;
	int8_t pwm_mux_mode;
	int8_t ain;
}pins_t;


// Tabla con todos los pines de las BB.
pins_t table[] = {
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

#endif
