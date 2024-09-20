// Sebastian Gilligan EC444 Based on example code given
/* From design pattern Serial IO Example */
#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"

#include <string.h>
#include "driver/uart.h"
#include "esp_vfs_dev.h"
// This is associated with VFS -- virtual file system interface and abstraction -- see the docs

static const uint8_t LED_BUILTIN = 13;
#define BUILTIN_LED  LED_BUILTIN // backward compatibility

void app_main()
{
    /* Install UART driver for interrupt-driven reads and writes */
    ESP_ERROR_CHECK( uart_driver_install(UART_NUM_0,
      256, 0, 0, NULL, 0) );

    /* Tell VFS to use UART driver */
    esp_vfs_dev_uart_use_driver(UART_NUM_0);
	
	gpio_reset_pin(LED_BUILTIN);
	gpio_set_direction(LED_BUILTIN, GPIO_MODE_OUTPUT);

    int mode = 0;
	int led_state = 0;
	
	printf("Welcome, 's' to switch modes\n");
	printf("Toggle LED Mode\n");
	
    while(1) {
      // gets()
      char buf[5];
      gets(buf);
      if (buf[0] != '\0') {
		switch(mode){
			case 0:
				if(buf[0] == 's'){
					mode++;
					printf("Echo Mode\n");
				}else if (buf[0] == 't'){
					led_state = 1 - led_state; //switch between 1 and 0
					gpio_set_level(13, led_state);
					printf("Read:  %s\n",buf);
				}
				break;
			case 1:
				if(buf[0] == 's'){
					mode++;
					printf("Echo Hex to Dec Mode\n");
				}else{
					printf("Echo: %s\n",buf);
				}
				break;
			case 2:
				if(buf[0] == 's'){
					mode = 0;
					printf("Toggle LED Mode\n");
				}else{
					printf("Hex: %x\n",atoi(buf));
				}
				break;
				
			default:
				break;
		}
      }
      vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}
