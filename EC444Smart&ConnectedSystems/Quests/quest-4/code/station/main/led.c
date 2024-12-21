// LED task to light LED based on traffic state -- 2023: no changes
#include "led.h"
#include "state.h"

extern individual_states current_ind_state;
extern uint8_t confirm_led;

void led_init() {
    //zero-initialize the config structure.
    gpio_config_t io_conf = {};
    //disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);
}

void led_task(){
  while(1) {
    switch(current_ind_state){
      case LEADER : // Red
        gpio_set_level(GREENPIN, 0);
        gpio_set_level(REDPIN, 1);
        gpio_set_level(BLUEPIN, 0);
        // printf("Current state: %c\n",status);
        break;
      case FOLLOWER : // Blue
        gpio_set_level(GREENPIN, 0);
        gpio_set_level(REDPIN, 0);
        gpio_set_level(BLUEPIN, 1);
        // printf("Current state: %c\n",status);
        break;
      case SHOULD_BE_LEADER : // Green
        gpio_set_level(GREENPIN, 1);
        gpio_set_level(REDPIN, 0);
        gpio_set_level(BLUEPIN, 0);
        // printf("Current state: %c\n",status);
        break;
      default:
        gpio_set_level(GREENPIN, 0);
        gpio_set_level(REDPIN, 0);
        gpio_set_level(BLUEPIN, 0);
      break;
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void confirm_led_task(){
  while(1) {
    if(confirm_led){
      gpio_set_level(ONBOARD, 1);
      vTaskDelay(5000 / portTICK_PERIOD_MS);
      confirm_led = 0;
      gpio_set_level(ONBOARD, 0);
    }
    else{ 
      gpio_set_level(ONBOARD, 0);
    }
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}