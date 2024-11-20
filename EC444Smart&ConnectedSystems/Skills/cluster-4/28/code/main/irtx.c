#include "irtx.h"

char start = 0x1B;              // START BYTE that UART looks for
char myID = (char) ID;
char myColor = (char) COLOR;
int len_out = 4;

example_queue_element_t ele;
QueueHandle_t timer_queue;
SemaphoreHandle_t mux = NULL; // 2023: no changes


// Button interrupt handler -- add to queue -- 2023: no changes
static void IRAM_ATTR gpio_isr_handler(void* arg){
  uint32_t gpio_num = (uint32_t) arg;
  xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

// Timmer interrupt handler -- Callback timer function -- 2023: modified
// Note we enabled time for auto-reload
static bool IRAM_ATTR timer_on_alarm_cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_data) {
    BaseType_t high_task_awoken = pdFALSE;
    QueueHandle_t timer_queue1 = (QueueHandle_t)user_data;
    // Retrieve count value and send to queue
    example_queue_element_t ele = {
        .event_count = edata->count_value
    };
    xQueueSendFromISR(timer_queue1, &ele, &high_task_awoken);
    return (high_task_awoken == pdTRUE);
}

// Utilities ///////////////////////////////////////////////////////////////////

// Checksum -- 2023: no changes
char genCheckSum(char *p, int len) {
  char temp = 0;
  for (int i = 0; i < len; i++){
    temp = temp^p[i];
  }
  // printf("%X\n",temp);  // Diagnostic

  return temp;
}
bool checkCheckSum(uint8_t *p, int len) {
  char temp = (char) 0;
  bool isValid;
  for (int i = 0; i < len-1; i++){
    temp = temp^p[i];
  }
  // printf("Check: %02X ", temp); // Diagnostic
  if (temp == p[len-1]) {
    isValid = true; }
  else {
    isValid = false; }
  return isValid;
}


// MCPWM Initialize -- 2023: this is to create 38kHz carrier
void pwm_init() {

  // Create timer
  mcpwm_timer_handle_t pwm_timer = NULL;
  mcpwm_timer_config_t pwm_timer_config = {
      .group_id = 0,
      .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,
      .resolution_hz = MCPWM_TIMER_RESOLUTION_HZ,
      .period_ticks = MCPWM_FREQ_PERIOD,
      .count_mode = MCPWM_TIMER_COUNT_MODE_UP,
  };
  ESP_ERROR_CHECK(mcpwm_new_timer(&pwm_timer_config, &pwm_timer));

  // Create operator
  mcpwm_oper_handle_t oper = NULL;
  mcpwm_operator_config_t operator_config = {
      .group_id = 0, // operator must be in the same group to the timer
  };
  ESP_ERROR_CHECK(mcpwm_new_operator(&operator_config, &oper));

  // Connect timer and operator
  ESP_ERROR_CHECK(mcpwm_operator_connect_timer(oper, pwm_timer));

  // Create comparator from the operator
  mcpwm_cmpr_handle_t comparator = NULL;
  mcpwm_comparator_config_t comparator_config = {
      .flags.update_cmp_on_tez = true,
  };
  ESP_ERROR_CHECK(mcpwm_new_comparator(oper, &comparator_config, &comparator));

  // Create generator from the operator
  mcpwm_gen_handle_t generator = NULL;
  mcpwm_generator_config_t generator_config = {
      .gen_gpio_num = MCPWM_GPIO_NUM,
  };
  ESP_ERROR_CHECK(mcpwm_new_generator(oper, &generator_config, &generator));

  // set the initial compare value, so that the duty cycle is 50%
  ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator,132));
  // CANNOT FIGURE OUT HOW MANY TICKS TO COMPARE TO TO GET 50%

  // Set generator action on timer and compare event
  // go high on counter empty
  ESP_ERROR_CHECK(mcpwm_generator_set_action_on_timer_event(generator,
                  MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH)));
  // go low on compare threshold
  ESP_ERROR_CHECK(mcpwm_generator_set_action_on_compare_event(generator,
                  MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, comparator, MCPWM_GEN_ACTION_LOW)));

  // Enable and start timer
  ESP_ERROR_CHECK(mcpwm_timer_enable(pwm_timer));
  ESP_ERROR_CHECK(mcpwm_timer_start_stop(pwm_timer, MCPWM_TIMER_START_NO_STOP));

}

/* LEDC Initialize -- 2023: modified -- use either MCPWM or LEDC (this one is alternative PWM generator)
 static void pwm_init() {
   // Prepare and then apply the LEDC PWM timer configuration
   ledc_timer_config_t ledc_timer = {
       .speed_mode       = LEDC_MODE,
       .timer_num        = LEDC_TIMER,
       .duty_resolution  = LEDC_DUTY_RES,
       .freq_hz          = LEDC_FREQUENCY,  // Set output frequency at 5 kHz
       .clk_cfg          = LEDC_AUTO_CLK
   };
   ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

   // Prepare and then apply the LEDC PWM channel configuration
   ledc_channel_config_t ledc_channel = {
       .speed_mode     = LEDC_MODE,
       .channel        = LEDC_CHANNEL,
       .timer_sel      = LEDC_TIMER,
       .intr_type      = LEDC_INTR_DISABLE,
       .gpio_num       = LEDC_OUTPUT_IO,
       .duty           = LEDC_DUTY, // Set duty to 50%
       .hpoint         = 0
   };
   ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));

 }
*/

// Configure UART -- 2023: minor changes
void uart_init() {
  // Basic configs
  const uart_config_t uart_config = {
      .baud_rate = 1200, // Slow BAUD rate
      .data_bits = UART_DATA_8_BITS,
      .parity    = UART_PARITY_DISABLE,
      .stop_bits = UART_STOP_BITS_1,
      .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
      .source_clk = UART_SCLK_DEFAULT
  };
  uart_param_config(UART_NUM_1, &uart_config);

  // Set UART pins using UART0 default pins
  uart_set_pin(UART_NUM_1, UART_TX_GPIO_NUM, UART_RX_GPIO_NUM, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

  // Reverse receive logic line
  uart_set_line_inverse(UART_NUM_1,UART_SIGNAL_RXD_INV);

  // Install UART driver
  uart_driver_install(UART_NUM_1, BUF_SIZE * 2, 0, 0, NULL, 0);
}

// // GPIO init for LEDs -- 2023: modified
// void led_init() {
//     //zero-initialize the config structure.
//     gpio_config_t io_conf = {};
//     //disable interrupt
//     io_conf.intr_type = GPIO_INTR_DISABLE;
//     //set as output mode
//     io_conf.mode = GPIO_MODE_OUTPUT;
//     //bit mask of the pins that you want to set,e.g.GPIO18/19
//     io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
//     //disable pull-down mode
//     io_conf.pull_down_en = 0;
//     //disable pull-up mode
//     io_conf.pull_up_en = 0;
//     //configure GPIO with the given settings
//     gpio_config(&io_conf);
// }

// Configure timer -- 2023: Modified
void alarm_init() {

    gptimer_handle_t gptimer = NULL;
    gptimer_config_t timer_config = {
      .clk_src = GPTIMER_CLK_SRC_DEFAULT,
      .direction = GPTIMER_COUNT_UP,
      .resolution_hz = 1000000, // 1MHz, 1 tick=1us
    };
    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &gptimer));

    // Set alarm callback
    gptimer_event_callbacks_t cbs = {
        .on_alarm = timer_on_alarm_cb,
    };
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &cbs, timer_queue));

    // Enable timer
    ESP_ERROR_CHECK(gptimer_enable(gptimer));

    ESP_LOGI(TAG_TIMER, "Start timer, update alarm value dynamically and auto reload");
    gptimer_alarm_config_t alarm_config = {
      .reload_count = 0, // counter will reload with 0 on alarm event
      .alarm_count = 10*300000, // period = 10*1s = 10s
      .flags.auto_reload_on_alarm = true, // enable auto-reload
    };
    ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer, &alarm_config));
    ESP_ERROR_CHECK(gptimer_start(gptimer));

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
      if (myID == 3) {
        myID = 1;
      }
      else {
        myID++;
      }
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
      for(int i = 0; i < 20; i++){
        char *data_out = (char *) malloc(len_out);
        xSemaphoreTake(mux, portMAX_DELAY);
        data_out[0] = start;
        data_out[1] = (char) myColor;
        data_out[2] = (char) myID;
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

// Receive task -- looks for Start byte then stores received values -- 2023: minor changes
void recv_task(){
  // Buffer for input data
  // Receiver expects message to be sent multiple times
  uint8_t *data_in = (uint8_t *) malloc(BUF_SIZE2);
  while (1) {
    int len_in = uart_read_bytes(UART_NUM_1, data_in, BUF_SIZE2, 100 / portTICK_PERIOD_MS);
    ESP_LOGE(TAG_UART, "Length: %d", len_in);
    if (len_in > 10) {
      int nn = 0;
      ESP_LOGE(TAG_UART, "Length greater than 10");
      while (data_in[nn] != start) {
        nn++;
      }
      uint8_t copied[len_out];
      memcpy(copied, data_in + nn, len_out * sizeof(uint8_t));
      //printf("before checksum");
      ESP_LOG_BUFFER_HEXDUMP(TAG_UART, copied, len_out, ESP_LOG_INFO);
      if (checkCheckSum(copied,len_out)) {
        printf("after checksum");
        ESP_LOG_BUFFER_HEXDUMP(TAG_UART, copied, len_out, ESP_LOG_INFO);
	uart_flush(UART_NUM_1);
      }
    }
    else{
      // printf("Nothing received.\n");
    }
  vTaskDelay(5 / portTICK_PERIOD_MS);
  }
  free(data_in);
}

// LED task to light LED based on traffic state -- 2023: no changes
// void led_task(){
//   while(1) {
//     switch((int)myColor){
//       case 'R' : // Red
//         gpio_set_level(GREENPIN, 0);
//         gpio_set_level(REDPIN, 1);
//         gpio_set_level(BLUEPIN, 0);
//         // printf("Current state: %c\n",status);
//         break;
//       case 'B' : // Yellow
//         gpio_set_level(GREENPIN, 0);
//         gpio_set_level(REDPIN, 0);
//         gpio_set_level(BLUEPIN, 1);
//         // printf("Current state: %c\n",status);
//         break;
//       case 'G' : // Green
//         gpio_set_level(GREENPIN, 1);
//         gpio_set_level(REDPIN, 0);
//         gpio_set_level(BLUEPIN, 0);
//         // printf("Current state: %c\n",status);
//         break;
//       default:
//         gpio_set_level(GREENPIN, 0);
//         gpio_set_level(REDPIN, 0);
//         gpio_set_level(BLUEPIN, 0);
//       break;
//     }
//     vTaskDelay(100 / portTICK_PERIOD_MS);
//   }
// }

// LED task to blink onboard LED based on ID -- 2023: no changes
void id_task(){
  while(1) {
    for (int i = 0; i < (int) myID; i++) {
      gpio_set_level(ONBOARD,1);
      vTaskDelay(200 / portTICK_PERIOD_MS);
      gpio_set_level(ONBOARD,0);
      vTaskDelay(200 / portTICK_PERIOD_MS);
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// Timer task -- R (10 seconds), G (10 seconds), Y (10 seconds) -- 2023: modified
void timer_evt_task(void *arg) {
  while (1) {
    // Transfer from queue and do something if triggered
    if (xQueueReceive(timer_queue, &ele, pdMS_TO_TICKS(2000))) {
      printf("Changing color\n");

      if (myColor == 'R') {
        myColor = 'G';
      }
      else if (myColor == 'G') {
        myColor = 'B';
      }
      else if (myColor == 'B') {
        myColor = 'O';
      }
      else if (myColor == 'O')
      {
        myColor = 'R';
      }
      printf("Color: %c\n", myColor);

    }
  }
}