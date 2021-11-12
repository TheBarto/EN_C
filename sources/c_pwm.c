/*
 * Copiado de: https://github.com/adafruit/adafruit-beaglebone-io-python/blob/master/source/c_pwm.c
 *
 * Solo he cambiado alguna ruta y elemento para que funcionase correctamente.
 *
 */

/*
Copyright (c) 2013 Adafruit
Author: Justin Cooper

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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#ifdef __unix__
#include <syslog.h>
#endif
#include <fcntl.h>
#include <unistd.h>

#include "../headers/c_pwm.h"

#ifdef BBBVERSION41
#include "../headers/c_pinmux.h"
#endif

#define KEYLEN 7
#define PIN_MODE_LEN 5

int pwm_initialized = 0;

// pwm exports
struct pwm_exp
{
    char key[KEYLEN+1]; /* leave room for terminating NUL byte */
    int period_fd;
    int duty_fd;
    int polarity_fd;
#ifdef BBBVERSION41
    int enable_fd;
#endif
    float duty;
    unsigned long duty_ns;
    unsigned long period_ns;
    struct pwm_exp *next;
};
struct pwm_exp *exported_pwms = NULL;

struct pwm_exp *lookup_exported_pwm(const char *key)
{
    struct pwm_exp *pwm = exported_pwms;

    while (pwm != NULL)
    {
        if (strcmp(pwm->key, key) == 0) {
            return pwm;
        }
        pwm = pwm->next;
    }

#ifdef __unix__
    syslog(LOG_DEBUG, "Adafruit_BBIO: lookup_exported_pwm: couldn't find '%s'", key);
#endif
    return NULL; /* standard for pointers */
}

// Export PWM to the list
void export_pwm(struct pwm_exp *new_pwm)
{
    struct pwm_exp *pwm;

    if (exported_pwms == NULL)
    {
        // create new list
        exported_pwms = new_pwm;
    } else {
        // add to end of existing list
        pwm = exported_pwms;
        while (pwm->next != NULL)
            pwm = pwm->next;
        pwm->next = new_pwm;
    }
}

int is_dmtimer_pin(pwm_t *p) {
    char temp[6];
    strncpy(temp, p->module, 5);
    temp[5] = '\0';

    return (strcmp(temp, "timer") == 0);
}

BBIO_err initialize_pwm(void)
{
#ifdef BBBVERSION41  // don't load overlay in 4.1+
    if (!pwm_initialized) {
        strncpy(ocp_dir, "/sys/devices/platform/ocp", sizeof(ocp_dir));
#else
    BBIO_err err;
    if  (!pwm_initialized && load_device_tree("am33xx_pwm")) {
        err = build_path("/sys/devices", "ocp", ocp_dir, sizeof(ocp_dir));
        if (err != OK)
        {
            return E_SISTEMA_FIC;
        }
#endif
        pwm_initialized = 1;
#ifdef __unix__
        syslog(LOG_DEBUG, "Adafruit_BBIO: initialize_pwm: OK");
#endif
        return OK;
    }

#ifdef __unix__
    syslog(LOG_DEBUG, "Adafruit_BBIO: initialize_pwm: OK");
#endif
    return OK;
}

BBIO_err pwm_set_frequency(const char *key, float freq) {
    int len;
    char buffer[100];
    unsigned long period_ns;
    struct pwm_exp *pwm;

    if (freq <= 0.0)
    {
#ifdef __unix__
    	syslog(LOG_ERR, "Adafruit_BBIO: pwm_set_frequency: %s freq %f <= 0.0", key, freq);
#endif
        return E_ARG_INVALIDO;
    }

    pwm = lookup_exported_pwm(key);

    if (pwm == NULL)
    {
#ifdef __unix__
    	syslog(LOG_ERR, "Adafruit_BBIO: pwm_set_frequency: %s couldn't find key", key);
#endif
        return E_GENERICO;
    }

    period_ns = (unsigned long)(1e9 / freq);

    // If we're going to a shorter period, update the
    // duty cycle first, in order to avoid ever setting
    // the period < duty cycle (which would throw error)
    if (period_ns < pwm->period_ns) {
        pwm->period_ns = period_ns;

        // Update duty ns
        pwm->duty_ns = (unsigned long)(period_ns * (pwm->duty / 100.0));
        len = snprintf(buffer, sizeof(buffer), "%lu", pwm->duty_ns);
        lseek(pwm->duty_fd, 0, SEEK_SET); // Seek to beginning of file
        if (write(pwm->duty_fd, buffer, len) < 0) {
#ifdef __unix__
        	syslog(LOG_ERR, "Adafruit_BBIO: pwm_set_frequency: %s couldn't write duty: %i-%s",
                   key, errno, strerror(errno));
#endif
            return E_SISTEMA_FIC;
        }

        // Update period ns
        len = snprintf(buffer, sizeof(buffer), "%lu", period_ns);
        lseek(pwm->period_fd, 0, SEEK_SET); // Seek to beginning of file
        if (write(pwm->period_fd, buffer, len) < 0) {
#ifdef __unix__
        	syslog(LOG_ERR, "Adafruit_BBIO: pwm_set_frequency: %s couldn't write period: %i-%s",
                   key, errno, strerror(errno));
#endif
            return E_SISTEMA_FIC;
        }

    } else if (period_ns > pwm->period_ns) {
        pwm->period_ns = period_ns;
        // Ordinarily update the period first,
        // to avoid the opposite bug - kernel won't
        // let us set duty greater than period

        // Update period ns
        len = snprintf(buffer, sizeof(buffer), "%lu", period_ns);
        lseek(pwm->period_fd, 0, SEEK_SET); // Seek to beginning of file
        if (write(pwm->period_fd, buffer, len) < 0) {
#ifdef __unix__
        	syslog(LOG_ERR, "Adafruit_BBIO: pwm_set_frequency: %s couldn't write period: %i-%s",
                   key, errno, strerror(errno));
#endif
            return E_SISTEMA_FIC;
        }

        // Update duty ns
        pwm->duty_ns = (unsigned long)(period_ns * (pwm->duty / 100.0));
        len = snprintf(buffer, sizeof(buffer), "%lu", pwm->duty_ns);
        lseek(pwm->duty_fd, 0, SEEK_SET); // Seek to beginning of file
        if (write(pwm->duty_fd, buffer, len) < 0) {
#ifdef __unix__
        	syslog(LOG_ERR, "Adafruit_BBIO: pwm_set_frequency: %s couldn't write duty: %i-%s",
                   key, errno, strerror(errno));
#endif
            return E_SISTEMA_FIC;
        }
    } // else do nothing

#ifdef __unix__
    syslog(LOG_DEBUG, "Adafruit_BBIO: pwm_set_frequency: %s %f OK", key, freq);
#endif
    return OK;
}

// Only works before chip is enabled
BBIO_err pwm_set_polarity(const char *key, int polarity) {
    int len;
    char buffer[100]; /* allow room for trailing NUL byte */
    struct pwm_exp *pwm;
#ifdef BBBVERSION41
    int enabled; /* Maintain original state */
#endif

    pwm = lookup_exported_pwm(key);

    if (pwm == NULL) {
#ifdef __unix__
    	syslog(LOG_ERR, "Adafruit_BBIO: pwm_set_polarity: %s couldn't find key", key);
#endif
        return E_GENERICO;
    }

    // polarity can't be changed with enabled.
#ifdef BBBVERSION41
    // Read the current enabled status
    len = 1;
    memset(buffer, 0, 9);  // Initialize buffer
    lseek(pwm->enable_fd, 0, SEEK_SET);
    if (read(pwm->enable_fd, buffer, len) < 0) {
#ifdef __unix__
    	syslog(LOG_ERR, "Adafruit_BBIO: pwm_set_polarity: %s couldn't read enable: %i-%s",
               key, errno, strerror(errno));
#endif
        return E_SISTEMA_FIC;
    }

    // If the PWM is enabled, disable it
    // Can't set the polarity with device enabled
    // It will be reenabled after the parameters are set
    if (buffer[0] == '1') {
        enabled = 1;
        lseek(pwm->enable_fd, 0, SEEK_SET);
        len = snprintf(buffer, sizeof(buffer), "0");
        if (write(pwm->enable_fd, buffer, len) < 0) {
#ifdef __unix__
        	syslog(LOG_ERR, "Adafruit_BBIO: pwm_set_polarity: %s couldn't write enable: %i-%s",
                   key, errno, strerror(errno));
#endif
            return E_SISTEMA_FIC;
        }
    }

    // Treating 0 as normal, 1 as inversed
    // See documentation of sysfs interface at
    // https://www.kernel.org/doc/Documentation/pwm.txt
    if (polarity == 0) {
        len = snprintf(buffer, sizeof(buffer), "normal");
    } else if (polarity == 1) {
        len = snprintf(buffer, sizeof(buffer), "inversed");
    } else {
#ifdef __unix__
        syslog(LOG_ERR, "Adafruit_BBIO: pwm_set_polarity: %s invalid argument value: %i", key, polarity);
#endif
        return E_ARG_INVALIDO;
    }
#else
    len = snprintf(buffer, sizeof(buffer), "%d", polarity);
#endif

    lseek(pwm->polarity_fd, 0, SEEK_SET); // Seek to beginning of file
    if (write(pwm->polarity_fd, buffer, len) < 0) {
#ifdef __unix__
    	syslog(LOG_ERR, "Adafruit_BBIO: pwm_set_polarity: %s couldn't write polarity: %i-%s",
               key, errno, strerror(errno));
#endif
        return E_SISTEMA_FIC;
    }

    /* If we were enabled before, restore state */
#ifdef BBBVERSION41
    if (enabled) {
        lseek(pwm->enable_fd, 0, SEEK_SET);
        len = snprintf(buffer, sizeof(buffer), "1");
        if (write(pwm->enable_fd, buffer, len) < 0) {
#ifdef __unix__
        	syslog(LOG_ERR, "Adafruit_BBIO: pwm_set_polarity: %s couldn't write enable: %i-%s",
                   key, errno, strerror(errno));
#endif
            return E_SISTEMA_FIC;
        }
    }
#endif

#ifdef __unix__
    syslog(LOG_DEBUG, "Adafruit_BBIO: pwm_set_polarity: %s %i OK", key, polarity);
#endif
    return OK;
}

BBIO_err pwm_set_duty_cycle(const char *key, float duty) {
    int len;
    char buffer[100];
    struct pwm_exp *pwm;

    if (duty < 0.0 || duty > 100.0)
        return E_ARG_INVALIDO;

    pwm = lookup_exported_pwm(key);

    if (pwm == NULL) {
#ifdef __unix__
    	syslog(LOG_ERR, "Adafruit_BBIO: pwm_set_duty_cycle: %s couldn't find key", key);
#endif
        return E_GENERICO;
    }

    pwm->duty = duty;
    pwm->duty_ns = (unsigned long)(pwm->period_ns * (duty / 100.0));

    len = snprintf(buffer, sizeof(buffer), "%lu", pwm->duty_ns);
    lseek(pwm->duty_fd, 0, SEEK_SET); // Seek to beginning of file
    if (write(pwm->duty_fd, buffer, len) < 0) {
#ifdef __unix__
    	syslog(LOG_ERR, "Adafruit_BBIO: pwm_set_duty_cycle: %s couldn't write duty: %i-%s",
               key, errno, strerror(errno));
#endif
        return E_SISTEMA_FIC;
    }

#ifdef __unix__
    syslog(LOG_DEBUG, "Adafruit_BBIO: pwm_set_duty_cycle: %s %f OK", key, duty);
#endif
    return OK;
}

BBIO_err pwm_setup(const char *key, __attribute__ ((unused)) float duty, __attribute__ ((unused)) float freq, __attribute__ ((unused)) int polarity)
{
    BBIO_err err;
    struct pwm_exp *new_pwm;

#ifdef BBBVERSION41
    char pwm_dev_path[100]; 	// "/sys/devices/platform/ocp/48300000.epwmss"
    char pwm_addr_path[150];	// "/sys/devices/platform/ocp/48300000.epwmss/48300200.ehrpwm"
    char pwm_chip_path[200]; 	// "/sys/devices/platform/ocp/48300000.epwmss/48300200.ehrpwm/pwm/pwmchip0"
    char pwm_export_path[250]; 	// "/sys/devices/platform/ocp/48300000.epwmss/48300200.ehrpwm/pwm/pwmchip0/export"
    char pwm_path[250]; 	// "/sys/devices/platform/ocp/48300000.epwmss/48300200.ehrpwm/pwm/pwmchip0/pwm2"
    char pwm_path_udev[250]; 	// "/sys/devices/platform/ocp/48300000.epwmss/48300200.ehrpwm/pwm/pwmchip0/pwm-0:2"
    char ecap_path_udev[300];	// "/sys/devices/platform/ocp/48300000.epwmss/48300200.ecap/pwm/pwmchip0/pwm-0:0/"
    char duty_path[300]; 	// "/sys/devices/platform/ocp/48300000.epwmss/48300200.ehrpwm/pwm/pwmchip0/pwm2/duty_cycle"
    char period_path[300];
    char polarity_path[300];
    char enable_path[300];
    char pin_mode[PIN_MODE_LEN]; // "pwm" or "pwm2"

    int e;
    int period_fd, duty_fd, polarity_fd, enable_fd;
    struct stat s;
    FILE *f = NULL;
    pwm_t *p;

    if (!pwm_initialized) {
        err = initialize_pwm();
        if (err != OK) {
#ifdef __unix__
        	syslog(LOG_ERR, "Adafruit_BBIO: pwm_setup: %s couldn't initialize pwm: %i", key, err);
#endif
            return err;
        }
    }

    // Make sure that one of the universal capes is loaded
    if( !uboot_overlay_enabled() // only check kernel overlays if u-boot overlays are not being used
        &&
        !beaglebone_blue() // beaglebone blue has complete dtb file and does not need overlays
        &&
        !pocketbeagle() // pocketbeagle has complete dtb file and does not need overlays
        &&
        !( device_tree_loaded("cape-univ-audio") // from cdsteinkuehler/beaglebone-universal-io
        || device_tree_loaded("cape-univ-emmc")  // ""
        || device_tree_loaded("cape-univ-hdmi")  // ""
        || device_tree_loaded("cape-universal")  // ""
        || device_tree_loaded("cape-universala") // ""
        || device_tree_loaded("cape-universaln") // ""
        || device_tree_loaded("univ-all")        // from latest BeagleBone Debian 8 images
        || device_tree_loaded("univ-bbgw")       // ""
        || device_tree_loaded("univ-emmc")       // ""
        || device_tree_loaded("univ-hdmi")       // ""
        || device_tree_loaded("univ-nhdmi")))    // ""
    {
#ifdef __unix__
        syslog(LOG_ERR, "Adafruit_BBIO: pwm_setup: %s no suitable cape loaded", key);
#endif
        return E_CAPA_MANEJADOR;
    }
    // Do pinmuxing
    if(!strcmp(key, "P9_28")) {
        // ecap2 (P9_28) requires mode pwm2
        // based on bonescript commit 23bf443 by Matthew West
        strncpy(pin_mode, "pwm2", PIN_MODE_LEN);
    } else {
        strncpy(pin_mode, "pwm", PIN_MODE_LEN);
    }
    set_pin_mode(key, pin_mode);

    // Get info for pwm
    err = get_pwm_by_key(key, &p);
    if (err != OK) {
#ifdef __unix__
        syslog(LOG_ERR, "Adafruit_BBIO: pwm_setup: %s couldn't get pwm: %i", key, err);
#endif
        return err;
    }

    int dmtimer_pin = is_dmtimer_pin(p);

    if(!dmtimer_pin) {
        err = build_path(ocp_dir, p->chip, pwm_dev_path, sizeof(pwm_dev_path));
        if (err != OK) {
#ifdef __unix__
            syslog(LOG_ERR, "Adafruit_BBIO: pwm_setup: %s couldn't build pwm_dev_path: %i", key, err);
#endif
            return err;
        }

        err = build_path(pwm_dev_path, p->addr, pwm_addr_path, sizeof(pwm_addr_path));
        if (err != OK) {
#ifdef __unix__
            syslog(LOG_ERR, "Adafruit_BBIO: pwm_setup: %s couldn't build pwm_addr_path: %i", key, err);
#endif
            return err;
        }
    }
    else {
        err = build_path("/sys/devices/platform", p->addr, pwm_addr_path, sizeof(pwm_addr_path));
        if (err != OK) {
#ifdef __unix__
            syslog(LOG_ERR, "Adafruit_BBIO: pwm_setup: %s couldn't build pwm_addr_path, are you sure you've loaded the correct dmtimer device tree overlay?: %i", key, err);
#endif
            return err;
        }
    }

    err = build_path(pwm_addr_path, "pwm/pwmchip", pwm_chip_path, sizeof(pwm_chip_path));
    if (err != OK) {
#ifdef __unix__
        syslog(LOG_ERR, "Adafruit_BBIO: pwm_setup: %s couldn't build pwm_chip_path: %i", key, err);
#endif
        return err;
    }

    snprintf(pwm_path, sizeof(pwm_path), "%s/pwm%d", pwm_chip_path, p->index);
#ifdef __unix__
    syslog(LOG_DEBUG, "Adafruit_BBIO: pwm_start: key: %s, pwm_path: %s", key, pwm_path);
#endif

    //pwm with udev patch
    snprintf(pwm_path_udev, sizeof(pwm_path_udev), "%s/pwm-%c:%d", pwm_chip_path, dmtimer_pin ? pwm_path[47] : pwm_path[66], p->index);
#ifdef __unix__
    syslog(LOG_DEBUG, "Adafruit_BBIO: pwm_start: key: %s, pwm_path_udev: %s", key, pwm_path_udev);
#endif
    //ecap output with udev patch
    snprintf(ecap_path_udev, sizeof(ecap_path_udev), "%s/pwm-%c:%d", pwm_chip_path, dmtimer_pin ? pwm_path[47] : pwm_path[67], p->index);
#ifdef __unix__
    syslog(LOG_DEBUG, "Adafruit_BBIO: pwm_start: key: %s, ecap_path_udev: %s", key, ecap_path_udev);
#endif

    // Export PWM if hasn't already been
    e = stat(pwm_path, &s);
    if (-1 == e) {
        if (ENOENT == errno) { // directory does not exist
            snprintf(pwm_export_path, sizeof(pwm_export_path), "%s/export", pwm_chip_path);
            f = fopen(pwm_export_path, "w");
            if (f == NULL) { // Can't open the export file
#ifdef __unix__
            	syslog(LOG_ERR, "Adafruit_BBIO: pwm_setup: %s couldn't open %s: %i-%s",
                       key, pwm_export_path, errno, strerror(errno));
#endif
                return E_ACCESO;
            }
            fprintf(f, "%d", p->index);
            fclose(f);
            /* sleep to avoid race condition as udev needs the
               opportunity to set group ownership and permission */
#ifdef __unix__
            syslog(LOG_DEBUG, "Adafruit_BBIO: pwm_start: sleep 100 ms after export to avoid udev race condition");
#endif
            usleep(100 * 1000); /* 100 ms */
        } else {
#ifdef __unix__
        	syslog(LOG_ERR, "Adafruit_BBIO: pwm_setup: %s couldn't stat %s: %i-%s",
                   key, pwm_path, errno, strerror(errno));
#endif
            perror("stat");
            return E_GENERICO;
        }
    } else {
        if (S_ISDIR(s.st_mode)) {
            /* It is a directory. Already exported */
        } else {
            /* It's a file. Shouldn't ever happen */
#ifdef __unix__
            syslog(LOG_ERR, "Adafruit_BBIO: pwm_setup: %s %s is not a directory", key, pwm_path);
#endif
            return E_GENERICO;
        }
    }

    e = stat(pwm_path, &s);
    if (-1 == e) {
        if (ENOENT == errno) {
            // Directory still doesn't exist, try the new udev pwm path format in 4.14 kernel
#ifdef __unix__
            syslog(LOG_DEBUG, "Adafruit_BBIO: pwm_setup: key: %s pwm_path: %s doesn't exist", key, pwm_path);
#endif

            e = stat(pwm_path_udev, &s);
            if (-1 == e) {
                if (ENOENT == errno) {
                    // Directory still doesn't exist, try the new udev ecap path format in 4.14 kernel
#ifdef __unix__
                    syslog(LOG_DEBUG, "Adafruit_BBIO: pwm_setup: key: %s pwm_path_udev: %s doesn't exist", key, pwm_path_udev);
#endif
                    e = stat(ecap_path_udev, &s);
                    if (-1 == e) {
                        if (ENOENT == errno) {
                            // Directory still doesn't exist, exit with error
#ifdef __unix__
                            syslog(LOG_DEBUG, "Adafruit_BBIO: pwm_setup: key: %s ecap_path_udev: %s doesn't exist", key, ecap_path_udev);
                            syslog(LOG_ERR, "Adafruit_BBIO: pwm_setup: path for %s doesn't exist", key);
#endif
                            return E_GENERICO;
                        }
                    } else {
                        strncpy(pwm_path, ecap_path_udev, sizeof(pwm_path));
                    }
                }
            } else {
              strncpy(pwm_path, pwm_path_udev, sizeof(pwm_path));
	      usleep(100*1000);
            }
        }
    }
#ifdef __unix__
    syslog(LOG_DEBUG, "Adafruit_BBIO: pwm_setup: pwm_path=%s\n", pwm_path);
#endif

    snprintf(duty_path, sizeof(duty_path), "%s/duty_cycle", pwm_path);
    snprintf(enable_path, sizeof(enable_path), "%s/enable", pwm_path);
#else
    /*Puesto 150 de tamanio en vez de 100 para quitar un aviso.*/
    char fragment[100];
    char pwm_fragment[100];
    char pwm_path[100];
    char duty_path[200];
    char period_path[150];
    char polarity_path[150];
    int period_fd, duty_fd, polarity_fd;

    if (!pwm_initialized) {
        err = initialize_pwm();
        if (err != OK) {
            return err;
        }
    }

    // load tree
    snprintf(fragment, sizeof(fragment), "bone_pwm_%s", key);
    err = load_device_tree(fragment);
    if (err != OK) {
        //error enabling pin for pwm
        return err;
    }

    //creates the fragment in order to build the pwm_test_filename, such as "pwm_test_P9_13"
    snprintf(pwm_fragment, sizeof(pwm_fragment), "pwm_test_%s", key);

    // Initialize the ocp_dir
    err = build_path("/sys/devices", "ocp", ocp_dir, sizeof(ocp_dir));
    if (err != OK) {
        return err;
    }

    //finds and builds the pwm_path, as it can be variable...
    err = build_path(ocp_dir, pwm_fragment, pwm_path, sizeof(pwm_path));
    if (err != OK) {
        return err;
    }

    //create the path for duty
    snprintf(duty_path, sizeof(duty_path), "%s/duty", pwm_path);
#endif

    // create the path for period and polarity
    snprintf(period_path, sizeof(period_path), "%s/period", pwm_path);
    snprintf(polarity_path, sizeof(polarity_path), "%s/polarity", pwm_path);

    //add period and duty fd to pwm list
    if ((period_fd = open(period_path, O_RDWR)) < 0) {
#ifdef __unix__
    	syslog(LOG_ERR, "Adafruit_BBIO: pwm_setup: %s couldn't open %s: %i-%s",
               key, period_path, errno, strerror(errno));
#endif
        return E_SISTEMA_FIC;
    }

    if ((duty_fd = open(duty_path, O_RDWR)) < 0) {
        //error, close already opened period_fd.
        close(period_fd);
#ifdef __unix__
        syslog(LOG_ERR, "Adafruit_BBIO: pwm_setup: %s couldn't open %s: %i-%s",
               key, duty_path, errno, strerror(errno));
#endif
        return E_SISTEMA_FIC;
    }

    if ((polarity_fd = open(polarity_path, O_RDWR)) < 0) {
        //error, close already opened period_fd and duty_fd.
        close(period_fd);
        close(duty_fd);
#ifdef __unix__
        syslog(LOG_ERR, "Adafruit_BBIO: pwm_setup: %s couldn't open %s: %i-%s",
               key, polarity_path, errno, strerror(errno));
#endif
        return E_SISTEMA_FIC;
    }

#ifdef BBBVERSION41
    if ((enable_fd = open(enable_path, O_RDWR)) < 0) {
        // error, close already opened files
        close(period_fd);
        close(duty_fd);
        close(polarity_fd);
#ifdef __unix__
        syslog(LOG_ERR, "Adafruit_BBIO: pwm_setup: %s couldn't open %s: %i-%s",
               key, enable_path, errno, strerror(errno));
#endif
        return E_SISTEMA_FIC;
    }
#endif

    // add to list
    new_pwm = malloc(sizeof(struct pwm_exp));
    if (new_pwm == NULL) {
        close(period_fd);
        close(duty_fd);
        close(polarity_fd);
#ifdef BBBVERSION41
        close(enable_fd);
#endif
#ifdef __unix__
        syslog(LOG_ERR, "Adafruit_BBIO: pwm_setup: %s couldn't malloc pwm_exp: %i-%s",
               key, errno, strerror(errno));
#endif
        return E_MEMORIA; // out of memory
    }

    strncpy(new_pwm->key, key, KEYLEN);  /* can leave string unterminated */
    new_pwm->key[KEYLEN] = '\0'; /* terminate string */
    new_pwm->period_fd = period_fd;
    new_pwm->duty_fd = duty_fd;
    new_pwm->polarity_fd = polarity_fd;

#ifdef BBBVERSION41
    new_pwm->enable_fd = enable_fd;
#endif

    new_pwm->next = NULL;

    export_pwm(new_pwm);

#ifdef __unix__
    syslog(LOG_DEBUG, "Adafruit_BBIO: pwm_setup: %s OK", key);
#endif
    return OK;
}

BBIO_err pwm_start(const char *key, float duty, float freq, int polarity)
{
#ifdef __unix__
    syslog(LOG_DEBUG, "Adafruit_BBIO: pwm_start: %s, %f, %f, %i", key, duty, freq, polarity);
#endif
    //fprintf(stderr, "Adafruit_BBIO: pwm_start: %s, %f, %f, %i\n", key, duty, freq, polarity);

    BBIO_err err;
    char buffer[100];
    ssize_t len;

    struct pwm_exp *pwm = lookup_exported_pwm(key);
    if (pwm == NULL) {
        err = pwm_setup(key, duty, freq, polarity);
        if (err != OK) {
#ifdef __unix__
            syslog(LOG_ERR, "Adafruit_BBIO: pwm_start: %s pwm setup failed: %i", key, err);
#endif
            return err;
        }

        pwm = lookup_exported_pwm(key);
    }

    // If we somehow didn't start successfully
    if (pwm == NULL) {
#ifdef __unix__
        syslog(LOG_ERR, "Adafruit_BBIO: pwm_start: %s pwm is NULL", key);
#endif
        return E_GENERICO;
    }

    // Read out current period_ns from the file, in order for it to behave
    // properly
    memset(buffer, 0, sizeof(buffer));  // Initialize buffer
    lseek(pwm->period_fd, 0, SEEK_SET);
    len = read(pwm->period_fd, buffer, sizeof(buffer));
    if (len < 0) {
#ifdef __unix__
        syslog(LOG_ERR, "Adafruit_BBIO: pwm_start: %s couldn't read period: %i-%s",
               key, errno, strerror(errno));
#endif
        return E_SISTEMA_FIC;
    } else if (len >= (ssize_t)sizeof(buffer)) {
        // If this is the case, there's more in the file.
        // This should never happen, as it would mean that
        // the period is 10^8 seconds
        return E_SISTEMA_FIC;
    }
    // Set the period_ns from the file
    sscanf(buffer, "%lu", &(pwm->period_ns));

    // Read out the current duty_ns from the file, in order for it to
    // behave properly
    memset(buffer, 0, sizeof(buffer));
    lseek(pwm->duty_fd, 0, SEEK_SET);
    len = read(pwm->duty_fd, buffer, sizeof(buffer));
    if (len < 0) {
#ifdef __unix__
    	syslog(LOG_ERR, "Adafruit_BBIO: pwm_start: %s couldn't read duty: %i-%s",
               key, errno, strerror(errno));
#endif
        return E_SISTEMA_FIC;
    } else if (len >= (ssize_t)sizeof(buffer)) {
        // If this is the case, there's more in the file.
        // This should never happen, as it would mean that
        // the period is 10^8 seconds
        return E_SISTEMA_FIC;
    }
    // Set the duty_ns from the file
    sscanf(buffer, "%lu", &(pwm->duty_ns));

    // Initialize pwm->duty to avoid weirdness
    pwm->duty = duty;
#ifdef __unix__
    syslog(LOG_DEBUG, "Adafruit_BBIO: pwm_start: call pwm_set_frequency(key=%s freq=%f)", key, freq);
#endif
    err = pwm_set_frequency(key, freq);
    if (err != OK) {
#ifdef __unix__
        syslog(LOG_ERR, "Adafruit_BBIO: pwm_start: %s couldn't set duty frequency: %i", key, err);
#endif
        return err;
    }

#ifdef __unix__
    syslog(LOG_DEBUG, "Adafruit_BBIO: pwm_start: call pwm_set_duty_cycle(key=%s duty=%f)", key, duty);
#endif
    err = pwm_set_duty_cycle(key, duty);
    if (err != OK) {
#ifdef __unix__
    	syslog(LOG_ERR, "Adafruit_BBIO: pwm_start: %s couldn't set duty cycle: %i", key, err);
#endif
        return err;
    }

#ifdef __unix__
    syslog(LOG_DEBUG, "Adafruit_BBIO: pwm_start: call pwm_set_polarity(key=%s polarity=%d)", key, polarity);
#endif
    err = pwm_set_polarity(key, polarity);
    if (err != OK) {
#ifdef __unix__
    	syslog(LOG_ERR, "Adafruit_BBIO: pwm_start: %s couldn't set polarity: %i", key, err);
#endif
        return err;
    }

#ifdef BBBVERSION41  // Enable the PWM (don't think it's necessary before 4.1+)
    if (pwm == NULL) {
        return E_GENERICO;
    }
    len = snprintf(buffer, sizeof(buffer), "1");
#ifdef __unix__
    syslog(LOG_DEBUG, "Adafruit_BBIO: pwm_start: write 1 to pwm->enable_fd\n");
#endif
    lseek(pwm->enable_fd, 0, SEEK_SET);
    if (write(pwm->enable_fd, buffer, len) < 0) {
#ifdef __unix__
    	syslog(LOG_ERR, "Adafruit_BBIO: pwm_start: %s couldn't write enable: %i-%s",
               key, errno, strerror(errno));
#endif
        return E_SISTEMA_FIC;
    }
#endif

#ifdef __unix__
    syslog(LOG_DEBUG, "Adafruit_BBIO: pwm_start: %s OK", key);
#endif
    return OK;
}


BBIO_err pwm_disable(const char *key)
{
    struct pwm_exp *pwm, *temp, *prev_pwm = NULL;

#ifndef BBBVERSION41
    BBIO_err err;
    char fragment[100];
    snprintf(fragment, sizeof(fragment), "bone_pwm_%s", key);
    err = unload_device_tree(fragment);
    if (err != OK)
        return err;
#endif

    // remove from list
    pwm = exported_pwms;
    while (pwm != NULL)
    {
        if (strcmp(pwm->key, key) == 0)
        {

#ifdef BBBVERSION41
	        char buffer[100];
	        size_t len;

	        // Disable the PWM
	        lseek(pwm->enable_fd, 0, SEEK_SET);
	        len = snprintf(buffer, sizeof(buffer), "0");
	        if (write(pwm->enable_fd, buffer, len) < 0) {
#ifdef __unix__
	        	syslog(LOG_ERR, "Adafruit_BBIO: pwm_disable: %s couldn't write enable: %i-%s",
	                 key, errno, strerror(errno));
#endif
		        return E_SISTEMA_FIC;
	        }

	        // Unexport the PWM
	        // TODO later
#endif

            //close the fd
#ifdef BBBVERSION41
            close(pwm->enable_fd);
#endif
            close(pwm->period_fd);
            close(pwm->duty_fd);
            close(pwm->polarity_fd);

            if (prev_pwm == NULL)
            {
                exported_pwms = pwm->next;
                prev_pwm = pwm;
            } else {
                prev_pwm->next = pwm->next;
            }

            temp = pwm;
            pwm = pwm->next;
            free(temp);
        } else {
            prev_pwm = pwm;
            pwm = pwm->next;
        }
    }

#ifdef __unix__
    syslog(LOG_DEBUG, "Adafruit_BBIO: pwm_disable: %s OK", key);
#endif
    return OK;
}

void pwm_cleanup(void)
{
    while (exported_pwms != NULL) {
        pwm_disable(exported_pwms->key);
    }
}
