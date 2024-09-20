//Sebastian Gilligan EC444, based on https://learn.adafruit.com/thermistor/using-a-thermistor
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

#define DEFAULT_VREF   3300      

static esp_adc_cal_characteristics_t *adc_chars;
static const adc_channel_t channel = ADC_CHANNEL_0	;     //GPIO34 if ADC1, GPIO14 if ADC2
static const adc_atten_t atten = ADC_ATTEN_DB_11;		//Set attenuation to record higher values
static const adc_unit_t unit = ADC_UNIT_1;				

void app_main(void)
{
	int samples = 20; //Record every 100ms
	
    adc1_config_width(ADC_WIDTH_BIT_12); //Config ADC
    adc1_config_channel_atten(channel, atten);
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t)); //Allocate space for adc conversion
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);

    //Continuously sample ADC1
    while (1) {
        uint32_t adc_reading = 0; //Reset ADC reading value
        //Multisampling
        for (int i = 0; i < samples; i++) { //loop 10 times per second
            adc_reading += adc1_get_raw((adc1_channel_t)channel); //get ADC reading, add to value
			vTaskDelay(pdMS_TO_TICKS(100));
        }
		float average;
		float f_samples = 20.0;
		
        average = adc_reading/f_samples;
		//printf("%f, ", average);
        //Convert adc_reading to voltage in mV
        //uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);
		average = (4095 / average) - 1;
		average = 10000 / average;
		
		//printf("%f, ", average);
		
		
		float steinhart;
		steinhart = average / 10000;
		steinhart = log(steinhart);
		steinhart /= 3435;
		steinhart += 1.0 / (25.0 + 273.15);
		steinhart = 1.0 / steinhart;
		steinhart -= 273.15;
	
        //printf("Raw: %ld\tVoltage: %ldmV\n", adc_reading, voltage);
		printf("Temp: %f\n", steinhart);
        //vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
