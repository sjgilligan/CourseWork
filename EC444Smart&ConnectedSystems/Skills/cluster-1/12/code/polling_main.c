//Sebastian Gilligan EC444, Based off sample code given

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "string.h"
#include "driver/gpio.h"
#include "esp_vfs_dev.h"

int light_num; //0 - 4 

void lights(int blue, int red, int yellow, int green){ //function to make it easy to light LEDs
	gpio_set_level(13, green);
	gpio_set_level(12, yellow);
	gpio_set_level(27, red);
	gpio_set_level(33, blue);
}

void int_to_lights(int num){ //function to convert integer values to binary lights
	switch(num){
		case 0:
			lights(0,0,0,0);
			break;
		case 1:
			lights(0,0,0,1);
			break;
		case 2:
			lights(0,0,1,1);
			break;
		case 3:
			lights(0,1,1,1);
			break;
		case 4:
			lights(1,1,1,1);
			break;
		default:
			break;
	}
}

void init() {				
	light_num = 0;
	
	gpio_reset_pin(13); //green
	gpio_reset_pin(12); //yellow
	gpio_reset_pin(27); //red
	gpio_reset_pin(33); //blue
	gpio_set_direction(13, GPIO_MODE_OUTPUT);
	gpio_set_direction(12, GPIO_MODE_OUTPUT);
	gpio_set_direction(27, GPIO_MODE_OUTPUT);
	gpio_set_direction(33, GPIO_MODE_OUTPUT);
	
	gpio_reset_pin(34); //input pin
	gpio_set_direction(34, GPIO_MODE_INPUT);
	
	ESP_ERROR_CHECK( uart_driver_install(UART_NUM_0, 256, 0, 0, NULL, 0) ); //UART
	  
	esp_vfs_dev_uart_use_driver(UART_NUM_0);
	
}



static void button_task(){ //Monitor for button press
	int button_state;
	while(1){			
		button_state = gpio_get_level(34);
		if (button_state == 1){
			if(light_num < 4){
				light_num++;
			}else if(light_num == 4){
				light_num = 0;
			}
		}
		int_to_lights(light_num);
		printf("button: %d\n",button_state);
		printf("%d\n",light_num);
		vTaskDelay(200 / portTICK_PERIOD_MS);
	}
}

void app_main()
{
	init();				// Initialize stuff
	xTaskCreate(button_task, "button_task",1024*2, NULL, configMAX_PRIORITIES-1, NULL);
}	