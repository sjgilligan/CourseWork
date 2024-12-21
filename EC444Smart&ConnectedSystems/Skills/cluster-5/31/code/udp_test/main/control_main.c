#include "wifi_station.h"
#include "udp.h"
#include "move.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


void app_main(void){
	wifi_init_sta();
	example_ledc_init();
	
	xTaskCreate(udp_server_task, "udp_server", 4096, (void*)AF_INET, 1, NULL);
	
}