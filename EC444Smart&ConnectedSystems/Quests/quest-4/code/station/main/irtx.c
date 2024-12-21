#include "irtx.h"
#include "ir.h"
// Variables for my ID, minVal and status plus string fragments
char start = 0x1B;              // START BYTE that UART looks for
extern uint8_t ip_id;
int len_out = 4;


// Mutex (for resources), and Queues (for button)
SemaphoreHandle_t mux = NULL; // 2023: no changes


static void IRAM_ATTR gpio_isr_handler(void* arg){
  uint32_t gpio_num = (uint32_t) arg;
  xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

// Button interrupt init -- 2023: minor changes
void button_init() {
    //zero-initialize the config structure.
    gpio_config_t io_conf = {};
    //interrupt of rising edge
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    //bit mask of the pins, use GPIO4 here
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    //set as input mode
    io_conf.mode = GPIO_MODE_INPUT;
    //enable pull-up mode
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);

    gpio_intr_enable(GPIO_INPUT_IO_1 ); // 2023: retained

    //install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(GPIO_INPUT_IO_1, gpio_isr_handler, (void*) GPIO_INPUT_IO_1);

    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
}

bool button_pressed = false;
// Tasks 
// Button task -- rotate through myIDs -- 2023: no changes
void button_task(){
  uint32_t io_num;
  while(1) {
    if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
      xSemaphoreTake(mux, portMAX_DELAY);
      xSemaphoreGive(mux);
      printf("Button pressed.\n");
      button_pressed = true;
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }

}

// Send task -- sends payload | Start | myID | Start | myID -- 2023: no changes
void send_task(){
  vTaskDelay(100 / portTICK_PERIOD_MS);
  while(1) {
    if(button_pressed){
      if(gpio_get_level(VOTEPIN)){ 
        printf("voted blue! "); 
        //send vote 
       char *data_out = (char *) malloc(len_out);
        xSemaphoreTake(mux, portMAX_DELAY);
        data_out[0] = start;
        data_out[1] = 'B';
        data_out[2] = (char)ip_id;
        data_out[3] = genCheckSum(data_out,len_out-1);
        ESP_LOG_BUFFER_HEXDUMP(TAG_SYSTEM, data_out, len_out, ESP_LOG_INFO);

        uart_write_bytes(UART_NUM_1, data_out, len_out);
        xSemaphoreGive(mux);

        vTaskDelay(10 / portTICK_PERIOD_MS);
      }
      else if(gpio_get_level(VOTEPIN) ==0){ 
        printf("voted red!"); 
        char *data_out = (char *) malloc(len_out);
        xSemaphoreTake(mux, portMAX_DELAY);
        data_out[0] = start;
        data_out[1] = 'R';
        data_out[2] = (char)ip_id;
        data_out[3] = genCheckSum(data_out,len_out-1);
        ESP_LOG_BUFFER_HEXDUMP(TAG_SYSTEM, data_out, len_out, ESP_LOG_INFO);

        uart_write_bytes(UART_NUM_1, data_out, len_out);
        xSemaphoreGive(mux);

        vTaskDelay(10 / portTICK_PERIOD_MS);
      }
      
  }
  button_pressed = false;
  vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}
