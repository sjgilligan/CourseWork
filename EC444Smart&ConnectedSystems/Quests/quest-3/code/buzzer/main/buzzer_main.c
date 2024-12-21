/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/dac_cosine.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_check.h"

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

dac_cosine_handle_t chan0_handle;

int buzz = 0;

void buzzer_init(void)
{
    //dac_cosine_handle_t chan0_handle;

    dac_cosine_config_t cos0_cfg = {
        .chan_id = DAC_CHAN_0,
        .freq_hz = 8000, // It will be covered by 8000 in the latter configuration
        .clk_src = DAC_COSINE_CLK_SRC_DEFAULT,
        .offset = 0,
        .phase = DAC_COSINE_PHASE_0,
        .atten = DAC_COSINE_ATTEN_DEFAULT,
        .flags.force_set_freq = false,
    };
	
	

    ESP_ERROR_CHECK(dac_cosine_new_channel(&cos0_cfg, &chan0_handle));

}

void buzzer_task(void){
	static int current = 0;
	while(1){
		if(buzz == 1 && current == 0){
			printf("start\n");
			ESP_ERROR_CHECK(dac_cosine_start(chan0_handle));
			current = 1;
		}
		
		if(buzz == 0 && current == 1){
			ESP_ERROR_CHECK(dac_cosine_stop(chan0_handle));
			printf("stop\n");
			current = 0;
		}	
		vTaskDelay(pdMS_TO_TICKS(100));
	}
}

/*void start_buzz(void){
	vTaskDelay(pdMS_TO_TICKS(5000));
	buzz = 1;
}*/

void app_main(void){
	buzzer_init();
	
	vTaskDelay(pdMS_TO_TICKS(1000));
	
	xTaskCreate(buzzer_task,"buzzer_task", 4096, configMAX_PRIORITIES-1, 5, NULL);
	//xTaskCreate(start_buzz,"start_buzz", 4096, configMAX_PRIORITIES-2, 5, NULL);
	vTaskDelay(pdMS_TO_TICKS(5000));
	buzz = 1;
	vTaskDelay(pdMS_TO_TICKS(2000));
	buzz = 0;
}