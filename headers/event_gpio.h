
/* Copiado de: https://github.com/adafruit/adafruit-beaglebone-io-python/blob/master/source/event_gpio.h
 * El software no esta desarrollado por mi. Solo tiene unos pequeños cambios de rutas y constantes.*/

/*
Copyright (c) 2013 Adafruit

Original RPi.GPIO Author Ben Croston
Modified for BBIO Author Justin Cooper

This file incorporates work covered by the following copyright and
permission notice, all modified code adopts the original license:

Copyright (c) 2013 Ben Croston

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#ifndef EVENT_GPIO_H
#define EVENT_GPIO_H

//#include "adafruit/bbio/error.h"
#include "../headers/comun.h"

#define NO_EDGE      ((uint8_t) 0)
#define RISING_EDGE  ((uint8_t) 1)
#define FALLING_EDGE ((uint8_t) 2)
#define BOTH_EDGE    ((uint8_t) 3)

#define INPUT  ((uint8_t) 0)
#define OUTPUT ((uint8_t) 1)
#define ALT0   ((uint8_t) 4)

#define HIGH ((uint8_t) 1)
#define LOW  ((uint8_t) 0)

#define MAX_FILENAME ((uint8_t) 60)

#define USR_LED_GPIO_MIN ((uint8_t) 53)
#define USR_LED_GPIO_MAX ((uint8_t) 56)
#define USR_LED_RED      ((uint8_t) 66)
#define USR_LED_GREEN    ((uint8_t) 67)
#define BAT25   ((uint8_t) 27)
#define BAT50   ((uint8_t) 11)
#define BAT75   ((uint8_t) 61)
#define BAT100  ((uint16_t) 10000)       // Placeholder until I find the real number
#define WIFI    ((uint16_t) 10001)       // Ditto

#define PUD_OFF  ((uint8_t) 0)
#define PUD_DOWN ((uint8_t) 1)
#define PUD_UP   ((uint8_t) 2)

BBIO_err gpio_export(unsigned int gpio);
BBIO_err gpio_unexport(unsigned int gpio);
void exports_cleanup(void);
BBIO_err gpio_set_direction(unsigned int gpio, unsigned int in_flag);
BBIO_err gpio_get_direction(unsigned int gpio, unsigned int *value);
BBIO_err gpio_set_value(unsigned int gpio, unsigned int value);
BBIO_err gpio_get_value(unsigned int gpio, unsigned int *value);

int add_edge_detect(unsigned int gpio, unsigned int edge);
void remove_edge_detect(unsigned int gpio);
int add_edge_callback(unsigned int gpio, void (*func)(unsigned int gpio));
int event_detected(unsigned int gpio);
int gpio_initial(unsigned int gpio);
int gpio_event_add(unsigned int gpio);
int gpio_event_remove(unsigned int gpio);
int gpio_is_evented(unsigned int gpio);
int event_initialise(void);
void event_cleanup(void);
int blocking_wait_for_edge(unsigned int gpio, unsigned int edge, int timeout);

#endif /* EVENT_GPIO_H */
