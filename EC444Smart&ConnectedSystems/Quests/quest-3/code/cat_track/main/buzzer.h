#ifndef _buzzer_h   /* Include guard */
#define _buzzer_h

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/dac_cosine.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_check.h"
#include "esp_system.h"         
#include "esp_log.h"


#if CONFIG_IDF_TARGET_ESP32
#define EXAMPLE_DAC_CHAN0_ADC_CHAN          ADC_CHANNEL_8   // GPIO25, same as DAC channel 0
#define EXAMPLE_DAC_CHAN1_ADC_CHAN          ADC_CHANNEL_9   // GPIO26, same as DAC channel 1
#define EXAMPLE_ADC_WIDTH                   ADC_WIDTH_BIT_12
#elif CONFIG_IDF_TARGET_ESP32S2
#define EXAMPLE_DAC_CHAN0_ADC_CHAN          ADC_CHANNEL_6   // GPIO17, same as DAC channel 0
#define EXAMPLE_DAC_CHAN1_ADC_CHAN          ADC_CHANNEL_7   // GPIO18, same as DAC channel 1
#define EXAMPLE_ADC_WIDTH                   ADC_WIDTH_BIT_13
#endif
#define EXAMPLE_ADC_ATTEN                   ADC_ATTEN_DB_12

void buzzer_init(void);
void buzzer_task(void);
static const char *BUZZER = "Buzzer";

#endif
