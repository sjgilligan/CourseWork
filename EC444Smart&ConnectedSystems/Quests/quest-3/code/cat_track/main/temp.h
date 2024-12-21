#ifndef _temp_h   /* Include guard */
#define _temp_h


#include <stdio.h>
#include <math.h>
#include "driver/gpio.h"
#include "esp_log.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "esp_mac.h"
#include "sdkconfig.h"
#include "math.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void temp_init();
void temp();
void log_temp();

#endif