/* Aymeric Blaizot, Amado Diallo, Sebastian Gilligan 9/6/2024 EC444

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "esp_mac.h"
#include "sdkconfig.h"
#include "math.h"

//Temp

#define DEFAULT_VREF    3300        //Use adc2_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES   20          //Multisampling

#define THERMISTORNOMINAL 10000      
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25   
// how many samples to take and average, more takes longer
// but is more 'smooth'
#define NUMSAMPLES 5
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3435
// the value of the 'other' resistor
#define SERIESRESISTOR 10000   

#define LIGHT_THESHOLD 200

static esp_adc_cal_characteristics_t *adc_chars_temp;
static esp_adc_cal_characteristics_t *adc_chars_battery;
static esp_adc_cal_characteristics_t *adc_chars_photocell;
static const adc_channel_t temp_channel = ADC_CHANNEL_0;     	 // GPIO36 Temperature - A4
static const adc_channel_t battery_channel = ADC_CHANNEL_3; 	 // GPIO39 Battery - A3
static const adc_channel_t photocell_channel = ADC_CHANNEL_5; 	 // GPIO33 Photocell - 

static const adc_atten_t atten = ADC_ATTEN_DB_11;
static const adc_unit_t unit = ADC_UNIT_1;

static const char *TAG = "example";


#define LED1 13
#define LED2 12
#define LED3 27
//#define LED4 32

#define TILT_INPUT 21
#define GPIO_INPUT_PIN_SEL_TILT    1ULL<<TILT_INPUT // casting GPIO input to bitmap

static uint8_t s_led_state = 0;

static bool direction = true;
#define GPIO_INPUT_IO_1       34
#define ESP_INTR_FLAG_DEFAULT 0
#define GPIO_INPUT_PIN_SEL    1ULL<<GPIO_INPUT_IO_1 // casting GPIO input to bitmap

int flag = 0;     // Global flag for signaling from ISR

// Logged values
float temperature = 0.0; // Global variable for temperature
float light_reading = 0.0; // Global variable for light
float battery = 0.0; // Global variable for battery
bool tilt_reading = false; // Global variable for tilt

int mode = 0; // 0 - green, 1 - blue, 2 - red

// Define button interrupt handler -- just sets the flag on interrupt
static void IRAM_ATTR gpio_isr_handler(void* arg){
  flag = 1;
}

// Intialize the GPIO to detect button press as interrupt
static void button_init() {
  gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_POSEDGE; // interrupt of rising edge
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL; // bit mask of the pins, use GPIO4 here
    io_conf.mode = GPIO_MODE_INPUT;            // set as input mode
    io_conf.pull_up_en = 1;                    // enable resistor pull-up mode on pin
  gpio_config(&io_conf);                       // apply parameters
  gpio_intr_enable(GPIO_INPUT_IO_1 );          // enable interrupts on pin
  gpio_install_isr_service(ESP_INTR_FLAG_LEVEL3);   //install gpio isr service
  gpio_isr_handler_add(GPIO_INPUT_IO_1, gpio_isr_handler, (void*) GPIO_INPUT_IO_1); //hook isr handler for specific gpio pin

}

static void configure_led(uint8_t gpio_num)
{
    ESP_LOGI(TAG, "Example configured to blink GPIO LED!");
    gpio_reset_pin(gpio_num);

    /* Set the GPIO as a push/pull output */
    gpio_set_direction(gpio_num, GPIO_MODE_OUTPUT);
}

void temp_init() {
    // Set the GPIO as a push/pull output
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(temp_channel, atten);

    //Characterize ADC
    adc_chars_temp = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars_temp);
}

void battery_init(){
	adc1_config_channel_atten(battery_channel, atten);
	adc_chars_battery = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars_battery);
}

void photocell_init(){
	adc1_config_channel_atten(photocell_channel, atten);
	adc_chars_photocell = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars_photocell);
}

void led_init() {
    configure_led(LED1);
    configure_led(LED2);
    configure_led(LED3);
   // configure_led(LED4);
}

void tilt_init(){
    gpio_reset_pin(TILT_INPUT);
    gpio_set_direction(TILT_INPUT, GPIO_MODE_INPUT);
}

static void lights()
{
    while(1){
        if(mode == 0){
            gpio_set_level(LED1, 1); //green
            gpio_set_level(LED2, 0); //blue
            gpio_set_level(LED3, 0); //red
        }else if(mode == 1){
            gpio_set_level(LED1, 0); //green
            gpio_set_level(LED2, 0); //red
            while(mode == 1){
                gpio_set_level(LED2, 1);
                vTaskDelay(2000 / portTICK_PERIOD_MS);
                gpio_set_level(LED2, 0);
                vTaskDelay(2000 / portTICK_PERIOD_MS);
            }
        }else if(mode == 2){
            gpio_set_level(LED1, 0); //green
            gpio_set_level(LED2, 0); //blue
            gpio_set_level(LED3, 1); //red
        }
            vTaskDelay(20 / portTICK_PERIOD_MS);
    }
}
// Counting up and down with the LEDs
static void temp() {

    while (1) {

         uint32_t adc_reading = 0;
        //Multisampling
        for (int i = 0; i < NO_OF_SAMPLES; i++) {
            if (unit == ADC_UNIT_1) {
                adc_reading += adc1_get_raw((adc1_channel_t)temp_channel);
            } else {
                int raw;
                adc2_get_raw((adc2_channel_t)temp_channel, ADC_WIDTH_BIT_12, &raw);
                adc_reading += raw;
            }
            vTaskDelay(pdMS_TO_TICKS(100));
        }

        adc_reading /= NO_OF_SAMPLES;
        uint32_t voltage = adc_reading / 4095.0 * 3300;
        float resistance = 10000.0 / (4095.0 / adc_reading -1);

        // Temperature calculation
        float steinhart;
        steinhart = resistance / THERMISTORNOMINAL;     // (R/Ro)
        steinhart = log(steinhart);                  // ln(R/Ro)
        steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
        steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
        steinhart = 1.0 / steinhart;                 // Invert
        steinhart -= 273.15;                         // convert absolute temp to C
        //Convert adc_reading to voltage in mV
        temperature = steinhart;
    }
}

static void light()			
{
	while(1){
		uint32_t adc_reading = 0;
		float volts = 0.0;
		float amps = 0.0;
		float lux = 0.0;
		
		adc_reading = adc1_get_raw((adc1_channel_t)photocell_channel);
		volts = adc_reading * DEFAULT_VREF / 4095.0;
		amps = volts / SERIESRESISTOR;
		lux = amps * 2000.0;
		light_reading = lux-290;
		
		vTaskDelay(200 / portTICK_PERIOD_MS);
           
	}
}

static void battery_level()
{
        while(1){
			uint32_t adc_reading = 0;
			float volts = 0.0;
			adc_reading = adc1_get_raw((adc1_channel_t)battery_channel);
			volts = adc_reading * DEFAULT_VREF / 4095.0;
			battery = volts / 1000.0;
			
			
            vTaskDelay(200 / portTICK_PERIOD_MS);
        }
}

static void tilt()            
{
              
        while(1){
            if(gpio_get_level(TILT_INPUT)==1){
                tilt_reading = !tilt_reading;
            }
            vTaskDelay(200 / portTICK_PERIOD_MS);
        }
}

static void status()
{
    while(1) {                               // loop forever in this task
		if(flag) {
			
			flag = 0;                            // set the flag to false
            printf("Button Pressed\n");
			
			if(mode == 0){
				printf("Ready to be swallowed.\n");
				while(light_reading > LIGHT_THESHOLD){
                    vTaskDelay(100 / portTICK_PERIOD_MS);
					//do nothing
				}
				mode = 1; //go to blue
                printf("In Body.\n");

				while(light_reading < LIGHT_THESHOLD){
                    vTaskDelay(100 / portTICK_PERIOD_MS);
				}
				mode = 2; //go to red
                printf("Leaving Body.\n");
				while(1){
                    vTaskDelay(100 / portTICK_PERIOD_MS);
				}
			}
		}
		vTaskDelay(100 / portTICK_PERIOD_MS);  // wait a bit
	}
}

static void log_data()
{
    int time = 0;
    while(1){
        char tilt_log[20] = "horizontal";
        if(tilt_reading){
            strcpy(tilt_log, "vertical");
        }
        printf("Time: %d s, Temp: %3.1f C, Light: %4.2f Lux, Battery: %3.1f V, Tilt: %s\n", time, temperature, light_reading, battery, tilt_log);
        time += 2;
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}


void app_main(void)
{
    button_init();          // Initialize Button
    led_init();				// Initialize LEDs
    temp_init();            // Initialize Temperature Sensor
	battery_init();         // Initialize Battery Monitor
	photocell_init();       // Initialize Light Sensor
    tilt_init();            // Initialize Tilt Sensor
	
    //Setting up tasks
    xTaskCreate(temp, "temp",1024*2, NULL, configMAX_PRIORITIES-1, NULL);
    xTaskCreate(light, "light",1024*2, NULL, configMAX_PRIORITIES-2, NULL);
    xTaskCreate(battery_level, "battery_level",1024*2, NULL, configMAX_PRIORITIES-3, NULL);
    xTaskCreate(tilt, "tilt",1024*2, NULL, configMAX_PRIORITIES-4, NULL);
    xTaskCreate(status, "status",1024*2, NULL, configMAX_PRIORITIES-5, NULL);
    xTaskCreate(log_data, "log_data",1024*2, NULL, configMAX_PRIORITIES-6, NULL);
    xTaskCreate(lights, "lights",1024*2, NULL, configMAX_PRIORITIES-7, NULL);
  

}
