/* ADC1 Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "math.h"

#define DEFAULT_VREF    1100        //Use adc2_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES   64          //Multisampling

static esp_adc_cal_characteristics_t *adc_chars;
static const adc_channel_t channel = ADC_CHANNEL_6;     //GPIO34 if ADC1, GPIO14 if ADC2
static const adc_atten_t atten = ADC_ATTEN_DB_12;
static const adc_unit_t unit = ADC_UNIT_1;

static void check_efuse(void)
{
    //Check TP is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK) {
        printf("eFuse Two Point: Supported\n");
    } else {
        printf("eFuse Two Point: NOT supported\n");
    }

    //Check Vref is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF) == ESP_OK) {
        printf("eFuse Vref: Supported\n");
    } else {
        printf("eFuse Vref: NOT supported\n");
    }
}

static void print_char_val_type(esp_adc_cal_value_t val_type)
{
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
        printf("Characterized using Two Point Value\n");
    } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
        printf("Characterized using eFuse Vref\n");
    } else {
        printf("Characterized using Default Vref\n");
    }
}
double getDistance(double voltage) {
    // Define the known points
    double voltages[] = {150, 160, 210, 300};
    double distances[] = {15, 30, 45, 60};
    int numPoints = sizeof(voltages) / sizeof(voltages[0]);

    // Handle edge cases
    if (voltage <= voltages[0]) {
        return distances[0];
    }
    if (voltage >= voltages[numPoints - 1]) {
        return distances[numPoints - 1];
    }
    if (voltage == 151) {
        return 0;
    }

    // Perform linear interpolation
    for (int i = 0; i < numPoints - 1; i++) {
        if (voltage >= voltages[i] && voltage <= voltages[i + 1]) {
            double slope = (distances[i + 1] - distances[i]) / (voltages[i + 1] - voltages[i]);
            return distances[i] + slope * (voltage - voltages[i]);
        }
    }

    // If voltage is out of range, return -1 as an error value
    return -1;
}

void app_main(void)
{
    //Check if Two Point or Vref are burned into eFuse
    check_efuse();

    //Configure ADC
    if (unit == ADC_UNIT_1) {
        adc1_config_width(ADC_WIDTH_BIT_12);
        adc1_config_channel_atten(channel, atten);
    } else {
        adc2_config_channel_atten((adc2_channel_t)channel, atten);
    }

    //Characterize ADC
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);
    print_char_val_type(val_type);

    //Continuously sample ADC1
    while (1) {
        uint32_t adc_reading = 0;
        //Multisampling
        for (int i = 0; i < NO_OF_SAMPLES; i++) {
            if (unit == ADC_UNIT_1) {
                adc_reading += adc1_get_raw((adc1_channel_t)channel);
            } else {
                int raw;
                adc2_get_raw((adc2_channel_t)channel, ADC_WIDTH_BIT_12, &raw);
                adc_reading += raw;
                vTaskDelay(10 / portTICK_PERIOD_MS);
            }
        }
        adc_reading /= NO_OF_SAMPLES;
        //Convert adc_reading to voltage in mV
        uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);
		voltage = round((float)((3.3/3.1) * voltage));
        double distance = getDistance( (double)voltage);
        printf("Raw: %ld\tVoltage: %ldmV\t Distance: %.2fcm\n", adc_reading, voltage, distance);
        vTaskDelay(10 / portTICK_PERIOD_MS);

        }
		//float cm = 10650.08 * pow(adc_reading,-0.935) - 10;
		//int present;
               /*
        15 cm 150 mv
        30 cm 160 mv
        45 cm 210 mv
        60 cm 270 mv
        // */
		// if(voltage > 1250){
		// 	present = 1;
		// }else{
		// 	present = 0;
		// }
		
        // printf("Raw: %ld\tVoltage: %ldmV\t Present: %d\n", adc_reading, voltage,present);
        vTaskDelay(pdMS_TO_TICKS(40));
    
}