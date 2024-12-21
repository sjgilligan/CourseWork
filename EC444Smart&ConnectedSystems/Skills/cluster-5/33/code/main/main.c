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

// void actuate(float output)
// {

  // if(output < 0)
  // {
    // int duty = output * -1;
    // movement(output, output, true);
    // printf("backward %d\n", duty);
  // }
  // if(output > 0){
    // int duty = output * 1;
    // movement(duty, duty, false);
    // printf("forward %d\n", duty);

  // }

  // printf("Actuating with output: %f\n", output);
// }

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

	float previous_error = 0;
	float integral = 0;

	float setpoint = 25;
	float dt = 0.2;

	float Kp = 0.5;
	float Ki = 0.1;
	float Kd = 0.1;
	
	gpio_reset_pin(13);
	gpio_reset_pin(12);
	gpio_reset_pin(27); //good 4
	gpio_reset_pin(33);
	
	gpio_set_direction(13, GPIO_MODE_OUTPUT);
	gpio_set_direction(12, GPIO_MODE_OUTPUT);
	gpio_set_direction(27, GPIO_MODE_OUTPUT);
	gpio_set_direction(33, GPIO_MODE_OUTPUT);
	
	gpio_set_level(13,1);
	gpio_set_level(12,1);
	gpio_set_level(27,1);
	
	vTaskDelay(pdMS_TO_TICKS(1000));

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
		float cm;
		if(voltage > 2800){
			cm = 999999;//no signal;
			//printf("No signal\n");
		}else if(voltage < 2800 && voltage > 1400){//most cases
			cm = -0.007 * voltage + 39.6;
			//printf("Raw: %ld\tVoltage: %ldmV\t Range: %fcm\n", adc_reading, voltage,cm);
		}else if(voltage < 900){
			cm = 99999;
			//printf("No signal\n");
		}else{
			cm = 0;
			//printf("No signal\n");
		}
		
		
		float error = setpoint - cm;
		integral = integral + error * dt;
		float derivative = (error - previous_error) / dt;
		float output = Kp * error + Ki * integral + Kd * derivative;
		previous_error = error;
		//printf("cm: %f\n", cm);
		printf("Error: %f\tIntegral %f\tderivative %f \n", error, integral, derivative);
		//actuate(output);

		
		if(error < -1){
			gpio_set_level(13,1);
			gpio_set_level(12,0);
			gpio_set_level(27,0);
		}else if(error > 1){
			gpio_set_level(13,0);
			gpio_set_level(12,0);
			gpio_set_level(27,1);
		}else{
			gpio_set_level(13,0);
			gpio_set_level(12,1);
			gpio_set_level(27,0);
		}
        
        vTaskDelay(pdMS_TO_TICKS(40));
    }
}