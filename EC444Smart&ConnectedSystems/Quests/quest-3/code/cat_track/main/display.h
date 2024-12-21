#ifndef _display_h   /* Include guard */
#define _display_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "driver/i2c.h"
#include "driver/uart.h"
#include "esp_vfs_dev.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"
#include "esp_sleep.h"

int alpha_oscillator();
int no_blink();
int set_brightness_max(uint8_t val);
unsigned short getBinaryValue(char ch);

void test_alpha_display(void *blank);
void cycle_display(void *blank);

#endif