#ifndef _button_h   /* Include guard */
#define _button_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>             
#include <inttypes.h>           
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"     
#include "esp_log.h"            // for error logging
#include "esp_system.h"         
#include "driver/uart.h"
#include "driver/gptimer.h"     
#include "driver/gpio.h"        
#include "driver/ledc.h"   
#include "driver/i2c.h"

void IRAM_ATTR gpio_isr_handler(void* arg);
void button_init();
void button_task();

#endif