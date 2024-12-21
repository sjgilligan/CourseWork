#include "button.h"
#include "display.h"
#include "accelerometer.h"

#define GPIO_INPUT_IO_1 34
#define ESP_INTR_FLAG_DEFAULT 0
#define GPIO_INPUT_PIN_SEL    1ULL<<GPIO_INPUT_IO_1 // casting GPIO input to bitmap

extern uint16_t glo_user_buf[100];
extern int glo_display_count;
int scroll = 0;

extern activity_t activity;
extern float activity_time;

int flag = 0;

void IRAM_ATTR gpio_isr_handler(void* arg){
	flag = 1;
}	

// Intialize the GPIO to detect button press as interrupt
void button_init() {
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

void floatToString(float num, char* buffer) {
    sprintf(buffer, "%d", (int)floor(num));
}

// Define button task that runs forever
void button_task(){ //USED SOME CHATGPT TO REMIND MYSELF HOW TO DO STRINGS IN C
	//DISPLAY: "OTTO [activity] [activty_time] seconds"
	//Tell me the length of that string
	while(1) {                               // loop forever in this task
		if(flag) {
			printf("button press\n");
			
			char temp_buffer[100];
			char activity_time_buf[2];
			int length = 0;
			
			strcpy(temp_buffer, "OTTO ");
			length += 5;
			
			switch (activity) {
				case INACTIVE:
					strcat(temp_buffer, "INACTIVE ");
					length += 9; // "INACTIVE " (8 letters + 1 space)
					break;
				case ACTIVE:
					strcat(temp_buffer, "ACTIVE ");
					length += 7; // "ACTIVE " (6 letters + 1 space)
					break;
				case HIGHLY_ACTIVE:
					strcat(temp_buffer, "HIGHLY ACTIVE ");
					length += 14; // "HIGHLY ACTIVE " (13 letters + 1 space)
					break;
			}
			
			floatToString(activity_time, activity_time_buf);
			strcat(temp_buffer, activity_time_buf); // Append the activity_time string
			length += strlen(activity_time_buf);    // Add the length of the activity_time

			// Append " secs"
			strcat(temp_buffer, " SECS");
			length += 5; // " secs" (1 space + 4 letters)
			
			printf("%s\n", temp_buffer);

			// Convert the entire string to binary and store in glo_user_buf
			for (int i = 0; i < length; i++) {
				glo_user_buf[i] = getBinaryValue(temp_buffer[i]);
			}
			
			glo_display_count = length;
			flag = 0;
			scroll = 1; //tell display to go
		}
		
		vTaskDelay(200 / portTICK_PERIOD_MS);  // wait a bit
	}
}
