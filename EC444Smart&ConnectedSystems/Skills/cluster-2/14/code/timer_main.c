//Sebastian Gilligan EC444 Based on code provided
/* This is an example of a single timer interrupt using the ESP GPTimer APIs
   The example is pulled from 
   https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/gptimer.html#
   The timer triggers an event every 10s which is sent from the ISR/callback via an event queue
   The Event in queue signals action to turn on LED for 1s
   Runs in latest ESP32 distro as of 2023/09/22
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>             
#include <inttypes.h>           
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"     
#include "esp_log.h"            // for error logging
#include "esp_system.h"         
#include "driver/uart.h"
#include "driver/gptimer.h"     
#include "driver/gpio.h"        
#include "driver/ledc.h"   
#include "driver/i2c.h"

#define ONBOARD   13  // onboard LED
#define GPIO_OUTPUT_PIN_SEL  (1ULL<<ONBOARD)


// 14-Segment Display
#define SLAVE_ADDR                         0x70 // alphanumeric address
#define OSC                                0x21 // oscillator cmd
#define HT16K33_BLINK_DISPLAYON            0x01 // Display on cmd
#define HT16K33_BLINK_OFF                  0    // Blink off cmd
#define HT16K33_BLINK_CMD                  0x80 // Blink cmd
#define HT16K33_CMD_BRIGHTNESS             0xE0 // Brightness cmd

// Master I2C
#define I2C_EXAMPLE_MASTER_SCL_IO          22   // gpio number for i2c clk
#define I2C_EXAMPLE_MASTER_SDA_IO          23   // gpio number for i2c data
#define I2C_EXAMPLE_MASTER_NUM             I2C_NUM_0  // i2c port
#define I2C_EXAMPLE_MASTER_TX_BUF_DISABLE  0    // i2c master no buffer needed
#define I2C_EXAMPLE_MASTER_RX_BUF_DISABLE  0    // i2c master no buffer needed
#define I2C_EXAMPLE_MASTER_FREQ_HZ         100000     // i2c master clock freq
#define WRITE_BIT                          I2C_MASTER_WRITE // i2c master write
#define READ_BIT                           I2C_MASTER_READ  // i2c master read
#define ACK_CHECK_EN                       true // i2c master will check ack
#define ACK_CHECK_DIS                      false// i2c master will not check ack
#define ACK_VAL                            0x00 // i2c ack value
#define NACK_VAL                           0xFF // i2c nack value

#define GPIO_INPUT_IO_1 34
#define ESP_INTR_FLAG_DEFAULT 0
#define GPIO_INPUT_PIN_SEL    1ULL<<GPIO_INPUT_IO_1 // casting GPIO input to bitmap

// Global flag
int flag = 0; //for button

int display = 0; //for timer

int going = 0; //to start timer

uint16_t glo_display_buf[4]; //for i2c display

unsigned short getBinaryValue(char ch) { //USED CHATGPT TO CREATE THIS FUNCTION
    switch (ch) {
        case '!': return 0b0000000000000110;
        case '"': return 0b0000001000100000;
        case '#': return 0b0001001011001110;
        case '$': return 0b0001001011101101;
        case '%': return 0b0000110000100100;
        case '&': return 0b0010001101011101;
        case '\'': return 0b0000010000000000;
        case '(': return 0b0010010000000000;
        case ')': return 0b0000100100000000;
        case '*': return 0b0011111111000000;
        case '+': return 0b0001001011000000;
        case ',': return 0b0000100000000000;
        case '-': return 0b0000000011000000;
        case '.': return 0b0100000000000000;
        case '/': return 0b0000110000000000;
        case '0': return 0b0000110000111111;
        case '1': return 0b0000000000000110;
        case '2': return 0b0000000011011011;
        case '3': return 0b0000000010001111;
        case '4': return 0b0000000011100110;
        case '5': return 0b0010000001101001;
        case '6': return 0b0000000011111101;
        case '7': return 0b0000000000000111;
        case '8': return 0b0000000011111111;
        case '9': return 0b0000000011101111;
        case ':': return 0b0001001000000000;
        case ';': return 0b0000101000000000;
        case '<': return 0b0010010000000000;
        case '=': return 0b0000000011001000;
        case '>': return 0b0000100100000000;
        case '?': return 0b0001000010000011;
        case '@': return 0b0000001010111011;
        case 'A': return 0b0000000011110111;
        case 'B': return 0b0001001010001111;
        case 'C': return 0b0000000000111001;
        case 'D': return 0b0001001000001111;
        case 'E': return 0b0000000011111001;
        case 'F': return 0b0000000001110001;
        case 'G': return 0b0000000010111101;
        case 'H': return 0b0000000011110110;
        case 'I': return 0b0001001000001001;
        case 'J': return 0b0000000000011110;
        case 'K': return 0b0010010001110000;
        case 'L': return 0b0000000000111000;
        case 'M': return 0b0000010100110110;
        case 'N': return 0b0010000100110110;
        case 'O': return 0b0000000000111111;
        case 'P': return 0b0000000011110011;
        case 'Q': return 0b0010000000111111;
        case 'R': return 0b0010000011110011;
        case 'S': return 0b0000000011101101;
        case 'T': return 0b0001001000000001;
        case 'U': return 0b0000000000111110;
        case 'V': return 0b0000110000110000;
        case 'W': return 0b0010100000110110;
        case 'X': return 0b0010110100000000;
        case 'Y': return 0b0001010100000000;
        case 'Z': return 0b0000110000001001;
        case '[': return 0b0000000000111001;
        case '\\': return 0b0010000100000000;
        case ']': return 0b0000000000001111;
        case '^': return 0b0000110000000011;
        case '_': return 0b0000000000001000;
        case '`': return 0b0000000100000000;
        case 'a': return 0b0001000001011000;
        case 'b': return 0b0010000001111000;
        case 'c': return 0b0000000011011000;
        case 'd': return 0b0000100010001110;
        case 'e': return 0b0000100001011000;
        case 'f': return 0b0000000001110001;
        case 'g': return 0b0000010010001110;
        case 'h': return 0b0001000001110000;
        case 'i': return 0b0001000000000000;
        case 'j': return 0b0000000000001110;
        case 'k': return 0b0011011000000000;
        case 'l': return 0b0000000000110000;
        case 'm': return 0b0001000011010100;
        case 'n': return 0b0001000001010000;
        case 'o': return 0b0000000011011100;
        case 'p': return 0b0000000101110000;
        case 'q': return 0b0000010010000110;
        case 'r': return 0b0000000001010000;
        case 's': return 0b0010000010001000;
        case 't': return 0b0000000001111000;
        case 'u': return 0b0000000000011100;
        case 'v': return 0b0010000000000100;
        case 'w': return 0b0010100000010100;
        case 'x': return 0b0010100011000000;
        case 'y': return 0b0010000000001100;
        case 'z': return 0b0000100001001000;
        case '{': return 0b0000100101001001;
        case '|': return 0b0001001000000000;
        case '}': return 0b0010010010001001;
        case '~': return 0b0000010100100000;
        default: return 0; // Return 0 if no match is found
    }
}

static void i2c_example_master_init(){
    // Debug
    printf("\n>> i2c Config\n");
    int err;

    // Port configuration
    int i2c_master_port = I2C_EXAMPLE_MASTER_NUM;

    /// Define I2C configurations
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;                              // Master mode
    conf.sda_io_num = I2C_EXAMPLE_MASTER_SDA_IO;              // Default SDA pin
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;                  // Internal pullup
    conf.scl_io_num = I2C_EXAMPLE_MASTER_SCL_IO;              // Default SCL pin
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;                  // Internal pullup
    conf.master.clk_speed = I2C_EXAMPLE_MASTER_FREQ_HZ;       // CLK frequency
    //conf.clk_flags = 0;                                     // <-- UNCOMMENT IF YOU GET ERRORS (see readme.md)
    err = i2c_param_config(i2c_master_port, &conf);           // Configure
    if (err == ESP_OK) {printf("- parameters: ok\n");}

    // Install I2C driver
    err = i2c_driver_install(i2c_master_port, conf.mode,
                       I2C_EXAMPLE_MASTER_RX_BUF_DISABLE,
                       I2C_EXAMPLE_MASTER_TX_BUF_DISABLE, 0);
    // i2c_set_data_mode(i2c_master_port,I2C_DATA_MODE_LSB_FIRST,I2C_DATA_MODE_LSB_FIRST);
    if (err == ESP_OK) {printf("- initialized: yes\n\n");}

    // Dat in MSB mode
    i2c_set_data_mode(i2c_master_port, I2C_DATA_MODE_MSB_FIRST, I2C_DATA_MODE_MSB_FIRST);
}

// Utility  Functions //////////////////////////////////////////////////////////

// Utility function to test for I2C device address -- not used in deploy
int testConnection(uint8_t devAddr, int32_t timeout) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (devAddr << 1) | I2C_MASTER_WRITE, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    int err = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    return err;
}

// Utility function to scan for i2c device
static void i2c_scanner() {
    int32_t scanTimeout = 1000;
    printf("\n>> I2C scanning ..."  "\n");
    uint8_t count = 0;
    for (uint8_t i = 1; i < 127; i++) {
        // printf("0x%X%s",i,"\n");
        if (testConnection(i, scanTimeout) == ESP_OK) {
            printf( "- Device found at address: 0x%X%s", i, "\n");
            count++;
        }
    }
    if (count == 0)
        printf("- No I2C devices found!" "\n");
    printf("\n");
}

////////////////////////////////////////////////////////////////////////////////

// Alphanumeric Functions //////////////////////////////////////////////////////

// Turn on oscillator for alpha display
int alpha_oscillator() {
  int ret;
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, ( SLAVE_ADDR << 1 ) | WRITE_BIT, ACK_CHECK_EN);
  i2c_master_write_byte(cmd, OSC, ACK_CHECK_EN);
  i2c_master_stop(cmd);
  ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
  i2c_cmd_link_delete(cmd);
  vTaskDelay(200 / portTICK_PERIOD_MS);
  return ret;
}

// Set blink rate to off
int no_blink() {
  int ret;
  i2c_cmd_handle_t cmd2 = i2c_cmd_link_create();
  i2c_master_start(cmd2);
  i2c_master_write_byte(cmd2, ( SLAVE_ADDR << 1 ) | WRITE_BIT, ACK_CHECK_EN);
  i2c_master_write_byte(cmd2, HT16K33_BLINK_CMD | HT16K33_BLINK_DISPLAYON | (HT16K33_BLINK_OFF << 1), ACK_CHECK_EN);
  i2c_master_stop(cmd2);
  ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd2, 1000 / portTICK_PERIOD_MS);
  i2c_cmd_link_delete(cmd2);
  vTaskDelay(200 / portTICK_PERIOD_MS);
  return ret;
}

// Set Brightness
int set_brightness_max(uint8_t val) {
  int ret;
  i2c_cmd_handle_t cmd3 = i2c_cmd_link_create();
  i2c_master_start(cmd3);
  i2c_master_write_byte(cmd3, ( SLAVE_ADDR << 1 ) | WRITE_BIT, ACK_CHECK_EN);
  i2c_master_write_byte(cmd3, HT16K33_CMD_BRIGHTNESS | val, ACK_CHECK_EN);
  i2c_master_stop(cmd3);
  ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd3, 1000 / portTICK_PERIOD_MS);
  i2c_cmd_link_delete(cmd3);
  vTaskDelay(200 / portTICK_PERIOD_MS);
  return ret;
}

static void test_alpha_display() {
    // Debug
    int ret;
    printf(">> Test Alphanumeric Display: \n");

    // Set up routines
    // Turn on alpha oscillator
    ret = alpha_oscillator();
    if(ret == ESP_OK) {printf("- oscillator: ok \n");}
    // Set display blink off
    ret = no_blink();
    if(ret == ESP_OK) {printf("- blink: off \n");}
    ret = set_brightness_max(0xF);
    if(ret == ESP_OK) {printf("- brightness: max \n");}

    // Write to characters to buffer
    uint16_t displaybuffer[8];
    displaybuffer[0] = glo_display_buf[0];
    displaybuffer[1] = glo_display_buf[1];
    displaybuffer[2] = glo_display_buf[2];
    displaybuffer[3] = glo_display_buf[3];

    // Continually writes the same command
    while (1) {

      // Send commands characters to display over I2C
      i2c_cmd_handle_t cmd4 = i2c_cmd_link_create();
      i2c_master_start(cmd4);
      i2c_master_write_byte(cmd4, ( SLAVE_ADDR << 1 ) | WRITE_BIT, ACK_CHECK_EN);
      i2c_master_write_byte(cmd4, (uint8_t)0x00, ACK_CHECK_EN);
      for (uint8_t i=0; i<8; i++) {
        i2c_master_write_byte(cmd4, displaybuffer[i] & 0xFF, ACK_CHECK_EN);
        i2c_master_write_byte(cmd4, displaybuffer[i] >> 8, ACK_CHECK_EN);
      }
      i2c_master_stop(cmd4);
      ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd4, 1000 / portTICK_PERIOD_MS);
      i2c_cmd_link_delete(cmd4);

      // for (int i = 0; i < 8; i++) {
      //     printf("%04x\n", displaybuffer[i]);
      // }

		if(ret == ESP_OK) {
		//printf("- wrote: T.D.C.L. \n\n");
		}
		displaybuffer[0] = glo_display_buf[0];
		displaybuffer[1] = glo_display_buf[1];
		displaybuffer[2] = glo_display_buf[2];
		displaybuffer[3] = glo_display_buf[3];
		

    }


}

// A simple structure for queue elements
typedef struct {
    uint64_t event_count;
} example_queue_element_t;

// Create a FIFO queue for timer-based events
example_queue_element_t ele;
QueueHandle_t timer_queue;

// System log tags -- get logged when things happen, for debugging 
static const char *TAG_TIMER = "ec444: timer";       

// Timer interrupt handler -- callback timer function -- from GPTimer guide example
static bool IRAM_ATTR timer_on_alarm_cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_data) {
    BaseType_t high_task_awoken = pdFALSE;
    QueueHandle_t timer_queue1 = (QueueHandle_t)user_data;    // represents state info passed to callback, if needed
    example_queue_element_t ele = {
          .event_count = edata->count_value                   // Retrieve count value and send to queue
      };
    xQueueSendFromISR(timer_queue1, &ele, &high_task_awoken); // Puts data into queue and alerts other recipients
    return (high_task_awoken == pdTRUE);  		      // pdTRUE indicates data posted successfully
}

// Timer configuration -- from GPTimer guide example
static void alarm_init() {
    gptimer_handle_t gptimer = NULL;
    gptimer_config_t timer_config = {
      .clk_src = GPTIMER_CLK_SRC_DEFAULT,
      .direction = GPTIMER_COUNT_UP,
      .resolution_hz = 1000000, // 1MHz, 1 tick=1us
    };
    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &gptimer)); // instantiates timer
  
    gptimer_event_callbacks_t cbs = { // Set alarm callback
      .on_alarm = timer_on_alarm_cb,  // This is a specific supported callback from callbacks list
    };
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &cbs, timer_queue)); // This registers the callback
    ESP_ERROR_CHECK(gptimer_enable(gptimer));                                      // Enables timer interrupt ISR

    ESP_LOGI(TAG_TIMER, "Start timer, update alarm value dynamically and auto reload"); 
    gptimer_alarm_config_t alarm_config = { // Configure the alarm 
      .reload_count = 0,                    // counter will reload with 0 on alarm event
      .alarm_count = 1*1000000,            // period = 10*1s = 10s
      .flags.auto_reload_on_alarm = true,   // enable auto-reload
    };
    ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer, &alarm_config));  // this enacts the alarm config
    ESP_ERROR_CHECK(gptimer_start(gptimer));                            // this starts the timer
}

// Timer task -- what to do when the timer alarm triggers 
static void timer_evt_task(void *arg) {
	while (1) {
	// Transfer from queue and do something if triggered
		
		if (xQueueReceive(timer_queue, &ele, pdMS_TO_TICKS(2000))) {
			printf("Action! %d\n",display);
			if (going == 1){
				display++;
				if(display == 16){
					display = 0;
				}
				
				glo_display_buf[3] = getBinaryValue((display % 10) + '0');
				
				if (display > 9){
					glo_display_buf[2] = getBinaryValue('1');
				}else{
					glo_display_buf[2] = getBinaryValue('0');
				}
			}else{
				display = -1;
				glo_display_buf[2] = getBinaryValue('0');
				glo_display_buf[3] = getBinaryValue('0');
				vTaskDelay(200 / portTICK_PERIOD_MS);
			}
		}
	}
}

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
			printf("button press\n");
			if(going == 0){
				going = 1;
				display = -1;
				vTaskDelay(200 / portTICK_PERIOD_MS);
			}else{
				display = -1;
				vTaskDelay(200 / portTICK_PERIOD_MS);
			}
			
			flag = 0;
		}
		
		vTaskDelay(200 / portTICK_PERIOD_MS);  // wait a bit
	}
}

void app_main() {
	
	i2c_example_master_init();
	i2c_scanner();
	button_init();
	// Timer queue initialize 
	timer_queue = xQueueCreate(10, sizeof(example_queue_element_t));
	if (!timer_queue) {
		ESP_LOGE(TAG_TIMER, "Creating queue failed");
		return;
	}

	  // Create task to handle timer-based events 
	glo_display_buf[0] = getBinaryValue('0');
	glo_display_buf[1] = getBinaryValue('0');
	glo_display_buf[2] = getBinaryValue('0');
	glo_display_buf[3] = getBinaryValue('0');
	xTaskCreate(timer_evt_task, "timer_evt_task", 4096, NULL, configMAX_PRIORITIES-1, NULL);
	  
	xTaskCreate(test_alpha_display, "test_alpha_display", 4096, NULL, configMAX_PRIORITIES-2, NULL);
	  
	xTaskCreate(button_task, "button_task", 4096, NULL, configMAX_PRIORITIES - 3, NULL); 

	alarm_init();
 
}
