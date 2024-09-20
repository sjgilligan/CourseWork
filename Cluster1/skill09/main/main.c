//Sebastian Gilligan EC444, based on sample code provided
#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

#define DEFAULT_VREF   3300      

static esp_adc_cal_characteristics_t *adc_chars;
static const adc_channel_t channel = ADC_CHANNEL_6;     //GPIO34 if ADC1, GPIO14 if ADC2
static const adc_atten_t atten = ADC_ATTEN_DB_11;		//Set attenuation to record higher values
static const adc_unit_t unit = ADC_UNIT_1;				

void app_main(void)
{
	int samples_per_second = 10; //Record every 100ms
	
    adc1_config_width(ADC_WIDTH_BIT_12); //Config ADC
    adc1_config_channel_atten(channel, atten);
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t)); //Allocate space for adc conversion
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);

    //Continuously sample ADC1
    while (1) {
        uint32_t adc_reading = 0; //Reset ADC reading value
        //Multisampling
        for (int i = 0; i < samples_per_second; i++) { //loop 10 times per second
            adc_reading += adc1_get_raw((adc1_channel_t)channel); //get ADC reading, add to value
			vTaskDelay(pdMS_TO_TICKS(100));
        }
        adc_reading /= samples_per_second;
        //Convert adc_reading to voltage in mV
        uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);
        printf("Raw: %ld\tVoltage: %ldmV\n", adc_reading, voltage);
        //vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
