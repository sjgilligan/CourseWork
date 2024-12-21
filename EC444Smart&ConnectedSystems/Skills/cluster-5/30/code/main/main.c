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
#include  "move.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"



/* Warning:
 * For ESP32, ESP32S2, ESP32S3, ESP32C3, ESP32C2, ESP32C6, ESP32H2, ESP32P4 targets,
 * when LEDC_DUTY_RES selects the maximum duty resolution (i.e. value equal to SOC_LEDC_TIMER_BIT_WIDTH),
 * 100% duty cycle is not reachable (duty cannot be set to (2 ** SOC_LEDC_TIMER_BIT_WIDTH)).
 */


void app_main(void)
{
    // Set the LEDC peripheral configuration
    example_ledc_init();

        
    while(1){
        // Set the LEDC peripheral configuration
        // Set duty to 50%
        uint16_t duty = 400;

        movement(0, 0, true);
        printf("Stop\n");
        vTaskDelay(2000 / portTICK_PERIOD_MS);

        movement(duty, duty, true);
        printf("Forward\n");
        vTaskDelay(500 / portTICK_PERIOD_MS);

        movement(0, 0, true);
        printf("Stop\n");
        vTaskDelay(2000 / portTICK_PERIOD_MS);

        movement(0, 512, true);
        printf("Right\n");
        vTaskDelay(500 / portTICK_PERIOD_MS);

        movement(0, 0, true);
        printf("Stop\n");
        vTaskDelay(2000 / portTICK_PERIOD_MS);

        movement(512, 0, true);
        printf("Left\n");
        vTaskDelay(500 / portTICK_PERIOD_MS);

        movement(0, 0, true);
        printf("Stop\n");
        vTaskDelay(2000 / portTICK_PERIOD_MS);

        movement(duty, duty, false);
        printf("Backward\n");
        vTaskDelay(500 / portTICK_PERIOD_MS);
        // movement(0, 0, true);
        // printf("Stop\n");
        // vTaskDelay(500 / portTICK_PERIOD_MS);
         
        // Update duty to apply the new value

    }
    
}