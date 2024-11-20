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

int direction; //0 is up, 1 is down
int light_num; //0 - 7 

void lights(int red, int yellow, int green){ //function to make it easy to light LEDs
	gpio_set_level(13, green);
	gpio_set_level(12, yellow);
	gpio_set_level(27, red);
}

void int_to_lights(int num){ //function to convert integer values to binary lights
	switch(num){
		case 0:
			lights(0,0,0);
			break;
		case 1:
			lights(0,0,1);
			break;
		case 2:
			lights(0,1,0);
			break;
		case 3:
			lights(0,1,1);
			break;
		case 4:
			lights(1,0,0);
			break;
		case 5:
			lights(1,0,1);
			break;
		case 6:
			lights(1,1,0);
			break;
		case 7:
			lights(1,1,1);
			break;
		default:
			break;
	}
}

void init() {				
	direction = 0; //start up
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

static void task_1(){ //Count lights up or down
	while(1){			
		if(direction == 0){
			if(light_num == 7){
				light_num = 0;
			}else{
				light_num++;
			}
		}else if(direction == 1){
			if(light_num == 0){
				light_num = 7;
			}else{
				light_num--;
			}
		}
		int_to_lights(light_num);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

static void task_2(){ //Monitor for button press
	int button_state;
	while(1){			
		button_state = gpio_get_level(34);
		if (button_state == 1){
			direction = 1 - direction;
		}
		printf("button: %d\n",button_state);
		vTaskDelay(200 / portTICK_PERIOD_MS);
	}
}

static void task_3(){ //Signal blue LED 
	while(1){			
		if (direction == 0){
			gpio_set_level(33, 1);
		}else if(direction == 1){
			gpio_set_level(33, 0);
		}
		vTaskDelay(200 / portTICK_PERIOD_MS);
	}
}


void app_main()
{
	init();				// Initialize stuff
	xTaskCreate(task_1, "task_1",1024*2, NULL, configMAX_PRIORITIES-1, NULL);
	xTaskCreate(task_2, "task_2",1024*2, NULL, configMAX_PRIORITIES-2, NULL);
	xTaskCreate(task_3, "task_3",1024*2, NULL, configMAX_PRIORITIES-3, NULL);
}	
