/* WiFi station Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"
#include "ir.h"
#include "udp.h"
#include "wifi_station.h"
#include "irtx.h"
#include "ir-rx.h"
#include "state.h"
#include "led.h"


extern QueueHandle_t timer_queue;
extern SemaphoreHandle_t mux; // 2023: no changes


void app_main(void)
{
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
    wifi_init_sta();
    xTaskCreate(udp_client_task,"udp_client_task", 4096, configMAX_PRIORITIES-1, 5, NULL);
    xTaskCreate(udp_server_task, "udp_server", 4096, (void*)AF_INET, 5, NULL);
    mux = xSemaphoreCreateMutex();

    // Create task to handle timer-based events -- no changes
    //xTaskCreate(timer_evt_task, "timer_evt_task", 2048, NULL, configMAX_PRIORITIES-5, NULL);

    // Initialize all the things -- no changes
    // rmt_tx_init();
    uart_init();
    led_init();
    //alarm_init();
    button_init();
    pwm_init();
    timer_init();

    // Create tasks for receive, send, set gpio, and button -- 2023 -- no changes

    xTaskCreate(recv_task, "uart_rx_task", 1024*4, NULL, configMAX_PRIORITIES-1, NULL);
    xTaskCreate(send_task, "uart_tx_task", 1024*2, NULL, configMAX_PRIORITIES-9, NULL);
    xTaskCreate(led_task, "set_traffic_task", 1024*2, NULL, configMAX_PRIORITIES-2, NULL);
    xTaskCreate(button_task, "button_task", 1024*2, NULL, configMAX_PRIORITIES-4, NULL);
    xTaskCreate(listen_task, "listen_task", 1024*2, NULL, configMAX_PRIORITIES-5, NULL);
	  xTaskCreate(heartbeat_task, "heartbeat_task", 1024*2, NULL, configMAX_PRIORITIES-6, NULL);
	  xTaskCreate(print_state_task, "print_state_task", 1024*2, NULL, configMAX_PRIORITIES-7, NULL);
    xTaskCreate(udp_leader_task, "udp_leader_task",1024*3, NULL, configMAX_PRIORITIES-8 , NULL);
    xTaskCreate(udp_leader_forward_task, "udp_leader_forward_task",1024*3, NULL, configMAX_PRIORITIES-8 , NULL);
    xTaskCreate(udp_confirm_client_task, "udp_confirm_client_task",1024*3, NULL, configMAX_PRIORITIES-9 , NULL);

    xTaskCreate(confirm_led_task, "confirm_led_task", 1024, NULL, configMAX_PRIORITIES-10, NULL);
    
    while(1){
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }


}
