// Sebastian Gilligan EC444 Skill06
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "sdkconfig.h"

void lights(int green, int red, int blue, int yellow){
	gpio_set_level(13, green);
	gpio_set_level(12, red);
	gpio_set_level(27, blue);
	gpio_set_level(33, yellow);
	
	vTaskDelay(1000 / portTICK_PERIOD_MS);
}

void app_main(void){
	gpio_reset_pin(13);
	gpio_reset_pin(12);
	gpio_reset_pin(27); //good 4
	gpio_reset_pin(33);
	gpio_set_direction(13, GPIO_MODE_OUTPUT);
	gpio_set_direction(12, GPIO_MODE_OUTPUT);
	gpio_set_direction(27, GPIO_MODE_OUTPUT);
	gpio_set_direction(33, GPIO_MODE_OUTPUT);

    while (1) {
		lights(0,0,0,0);
		
		lights(0,0,0,1);
		
		lights(0,0,1,1);
		
		lights(0,1,1,1);
		
		lights(1,1,1,1);
		
		lights(1,1,1,0);
		
		lights(1,1,0,1);
		
		lights(1,1,0,0);
		
		lights(1,0,1,1);
		
		lights(1,0,1,0);
		
		lights(1,0,0,1);
		
		lights(1,0,0,0);
		
		lights(0,1,1,1);
		
		lights(0,1,1,0);
		
		lights(0,1,0,1);
		
		lights(0,1,0,0);
		
		lights(0,0,1,1);
		
		lights(0,0,1,0);
		
		lights(0,0,0,1);
		
		lights(0,0,0,0);
    }
}
