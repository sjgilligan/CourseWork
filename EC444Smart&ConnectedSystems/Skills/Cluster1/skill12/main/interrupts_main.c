#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"

#define GPIO_INPUT_IO_1 34
#define ESP_INTR_FLAG_DEFAULT 0
#define GPIO_INPUT_PIN_SEL    1ULL<<GPIO_INPUT_IO_1 // casting GPIO input to bitmap

int flag = 0;     // Global flag for signaling from ISR
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

void lights_init() {	
	light_num = 0;
	gpio_reset_pin(13); //green
	gpio_reset_pin(12); //yellow
	gpio_reset_pin(27); //red
	gpio_reset_pin(33); //blue
	gpio_set_direction(13, GPIO_MODE_OUTPUT);
	gpio_set_direction(12, GPIO_MODE_OUTPUT);
	gpio_set_direction(27, GPIO_MODE_OUTPUT);
	gpio_set_direction(33, GPIO_MODE_OUTPUT);
}

// Define button interrupt handler -- just sets the flag on interrupt
static void IRAM_ATTR gpio_isr_handler(void* arg){
	flag = 1;
}	

// Intialize the GPIO to detect button press as interrupt
static void button_init() {
	gpio_config_t io_conf;
		io_conf.intr_type = GPIO_INTR_POSEDGE; // interrupt of rising edge
		io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL; // bit mask of the pins, use GPIO4 here
		io_conf.mode = GPIO_MODE_INPUT;            // set as input mode
		io_conf.pull_up_en = 1;                    // enable resistor pull-up mode on pin
	gpio_config(&io_conf);                       // apply parameters
	gpio_intr_enable(GPIO_INPUT_IO_1 );          // enable interrupts on pin
	gpio_install_isr_service(ESP_INTR_FLAG_LEVEL3);   //install gpio isr service
	gpio_isr_handler_add(GPIO_INPUT_IO_1, gpio_isr_handler, (void*) GPIO_INPUT_IO_1); //hook isr handler for specific gpio pin
}

// Define button task that runs forever
void button_task(){
	while(1) {                               // loop forever in this task
		if(flag) {
			printf("Button pressed.\n");
			flag = 0;                            // set the flag to false
			if(light_num < 4){
				light_num++;
			}else if(light_num == 4){
				light_num = 0;
			}
		}
		int_to_lights(light_num);
		printf("%d\n",light_num);
		
		vTaskDelay(100 / portTICK_PERIOD_MS);  // wait a bit
	}
}

void app_main(void)
{
	button_init();     // Initialize button config 
	lights_init();
	xTaskCreate(button_task, "button_task", 1024*2, NULL, configMAX_PRIORITIES - 1, NULL); // Create task
	printf("Everything initialized. Waiting for button presses...\n"); // console message that it has begun
	while(1) {                               // loop forever in this task
		vTaskDelay(100 / portTICK_PERIOD_MS);  // wait a bit
	}
}
