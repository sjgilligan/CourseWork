//EC444 Team 4 Eve Cruanes, Sebastian Gillian, Aymeric Blaizot, Amado Diallo
/* ADC1 Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "range.h"

#define DEFAULT_VREF    1100        //Use adc2_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES   64          //Multisampling

static esp_adc_cal_characteristics_t *adc_chars;
static const adc_channel_t channel = ADC_CHANNEL_6;     //GPIO34 if ADC1, GPIO14 if ADC2
static const adc_atten_t atten = ADC_ATTEN_DB_12;
static const adc_unit_t unit = ADC_UNIT_1;

float cm;

bool detection; 

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

void ir_range(void)
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
            }
        }
        adc_reading /= NO_OF_SAMPLES;
        //Convert adc_reading to voltage in mV
        uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);
		voltage = round((float)((3.3/3.1) * voltage));
		//float cm = 10650.08 * pow(adc_reading,-0.935) - 10;
		//for sharp IR range finder:

        if(voltage < 2800 && voltage > 1600){//most cases
			cm = -0.007 * voltage + 39.6;
            detection = true; 
            printf("OBJECT DETECTED, DISTANCE: %f \t %d mV\n", cm, (uint16_t)voltage); 
            vTaskDelay(pdMS_TO_TICKS(200));
		}else{
            cm = 25;
            detection = false; 
			//printf("No signal\n");
        }
       // printf("Raw: %ld\tVoltage: %ldmV\t Range: %fcm\n", adc_reading, voltage,cm);


		// if(voltage > 2800){
		// 	cm = 999999;//no signal;
		// 	printf("No signal\n");
		// }else if(voltage < 2800 && voltage > 1400){//most cases
		// 	cm = -0.007 * voltage + 39.6;
		// 	printf("Raw: %ld\tVoltage: %ldmV\t Range: %fcm\n", adc_reading, voltage,cm);
		// }else if(voltage < 900){
		// 	cm = 99999;
		// 	printf("No signal\n");
		// }else{
		// 	cm = 0;
		// 	printf("No signal\n");
		// }
		
        
        vTaskDelay(pdMS_TO_TICKS(40));
    }
}