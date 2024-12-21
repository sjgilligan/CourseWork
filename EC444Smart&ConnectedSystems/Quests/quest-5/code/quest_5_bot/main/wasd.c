//EC444 Team 4 Eve Cruanes, Sebastian Gillian, Aymeric Blaizot, Amado Diallo
#include "move.h"
#include "wasd.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gptimer.h"     

extern char control;

void wasd_control(char direction, uint16_t duty)
{
    switch(direction)
    {
        case 'w':
            movement(duty, duty, true);
            //printf("Forward\n");
            break;
        case 's':
            movement(duty, duty, false);
            //printf("Backward\n");
            break;
        case 'a':
            movement(0, duty, true);
           // printf("Left\n");
            break;
        case 'd':
            movement(duty, 0, true);
            //printf("Right\n");
            break;
        case 'x':
            movement(0, 0, true);
            //printf("Stop\n");
            break;
        
    }
}

extern gptimer_handle_t gptimer;
float activity_time = 10;
void wasd_task()
{
    wasd_control('x', 512);
    while(1){
    uint64_t count = 0;
    ESP_ERROR_CHECK(gptimer_get_raw_count(gptimer, &count));
    activity_time = count / 1000000.0;
    if(activity_time < 10){
        wasd_control(control, 512);
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);

    }
}
