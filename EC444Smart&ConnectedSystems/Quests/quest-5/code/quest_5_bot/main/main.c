//EC444 Team 4 Eve Cruanes, Sebastian Gillian, Aymeric Blaizot, Amado Diallo
/* tt-motor-ledc adapted from LEDC from ESP32 examples/peripherals
   20224-11-09

   Edits reflect tested settings (f=50Hz, duty cycle 0--1023). Drives
   one motor with single speed at 50% duty cycle without direction
   control.
   ============   
   LEDC (LED Controller) basic example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "move.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "wifi_station.h"
#include "udp.h"
#include "range.h"
#include "protocol_examples_common.h"
#include "butterfly_move.h"
#include "udp_client.h"
#include "velocity.h"
//#include "protocol_examples_common.h"



/* Warning:
 * For ESP32, ESP32S2, ESP32S3, ESP32C3, ESP32C2, ESP32C6, ESP32H2, ESP32P4 targets,
 * when LEDC_DUTY_RES selects the maximum duty resolution (i.e. value equal to SOC_LEDC_TIMER_BIT_WIDTH),
 * 100% duty cycle is not reachable (duty cannot be set to (2 ** SOC_LEDC_TIMER_BIT_WIDTH)).
 */


void app_main(void)
{
    //Set the LEDC peripheral configuration
    example_ledc_init();
	//ESP_ERROR_CHECK(nvs_flash_init()); //udp_client
	//ESP_ERROR_CHECK(esp_netif_init());
	//ESP_ERROR_CHECK(esp_event_loop_create_default());
	//ESP_ERROR_CHECK(example_connect());
        
        // Set the LEDC peripheral configuration
        // Set duty to 50%
		

		
	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) { //udp_server
	ESP_ERROR_CHECK(nvs_flash_erase());
	ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret); 
	
	wifi_init_sta();
	
	xTaskCreate(udp_server_task, "udp_server", 4096,(void*)AF_INET, 5, NULL); //server to receive 

	
	xTaskCreate(ir_range, "ir_range", 1024*4, NULL, configMAX_PRIORITIES-2, NULL); //Get IR range data
	
	xTaskCreate(udp_client_task, "udp_client", 4096, NULL, 5, NULL); //udp client
	
	xTaskCreate(wasd_task, "wasd_task", 4096, configMAX_PRIORITIES-4, 5, NULL);

	xTaskCreate(butterfly_move_task, "butterfly_task", 4096, configMAX_PRIORITIES-6, 5, NULL);
	xTaskCreate(get_velocity,"get_velocity",4096, configMAX_PRIORITIES-7, 5, NULL);
	
	while(1){
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
        // movement(0, 0, true);
        // printf("Stop\n");
        // vTaskDelay(500 / portTICK_PERIOD_MS);
         
        // Update duty to apply the new value

    //}
    
}