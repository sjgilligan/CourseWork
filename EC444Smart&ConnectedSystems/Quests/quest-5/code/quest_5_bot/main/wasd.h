#include "move.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void wasd_control(char direction, uint16_t duty);
void wasd_task();