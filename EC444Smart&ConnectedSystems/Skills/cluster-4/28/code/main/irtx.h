/* Infrared IR/UART TX or RX example
   Created November 2019 -- Emily Lam
   Updated April 13, 2023 -- ESP IDF v 5.0 port

   *Not reliable to run both TX and RX parts at the same time* 

   PWM Pulse          -- pin 26 -- A0
   UART Transmitter   -- pin 25 -- A1
   UART Receiver      -- pin 34 -- A2

   Hardware interrupt -- pin 4 -- A5
   ID Indicator       -- pin 13 -- Onboard LED

   Red LED            -- pin 33
   Green LED          -- pin 32
   Blue LED           -- Pin 14

   Features:
   - Sends UART payload -- | START | myColor | myID | Checksum? |
   - Outputs 38kHz using PWM for IR transmission
   - Onboard LED blinks device ID (myID)
   - Button press to change device ID
   - RGB LED shows traffic light state (red, green, yellow)
   - Timer controls traffic light state (r - 10s, g - 10s, y - 10s)
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>             // Added in 2023..
#include <inttypes.h>           // Added in 2023
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"     // Added in 2023
#include "esp_log.h"
#include "esp_system.h"         // Added in 2023
#include "driver/rmt_tx.h"      // Modified in 2023
#include "soc/rmt_reg.h"        // Not needed?
#include "driver/uart.h"
//#include "driver/periph_ctrl.h"
#include "driver/gptimer.h"     // Added in 2023
//#include "clk_tree.h"           // Added in 2023
#include "driver/gpio.h"        // Added in 2023
#include "driver/mcpwm_prelude.h"// Added in 2023
#include "driver/ledc.h"        // Added in 2023

// MCPWM defintions -- 2023: modified
#define MCPWM_TIMER_RESOLUTION_HZ 10000000 // 10MHz, 1 tick = 0.1us
#define MCPWM_FREQ_HZ             38000    // 38KHz PWM -- 1/38kHz = 26.3us
#define MCPWM_FREQ_PERIOD         263      // 263 ticks = 263 * 0.1us = 26.3us
#define MCPWM_GPIO_NUM            25

// LEDC definitions -- 2023: modified 
// NOT USED / altnernative to MCPWM above
#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_OUTPUT_IO          25
#define LEDC_CHANNEL            LEDC_CHANNEL_0
#define LEDC_DUTY_RES           6     // Set duty resolution to 6 bits
#define LEDC_DUTY               32    // Set duty to 50%. ((2^6) - 1) * 50% = 32
#define LEDC_FREQUENCY          38000 // Frequency in Hertz. 38kHz

// UART definitions -- 2023: no changes
#define UART_TX_GPIO_NUM 26 // A0
#define UART_RX_GPIO_NUM 34 // A2
#define BUF_SIZE (1024)
#define BUF_SIZE2 (32)

// Hardware interrupt definitions -- 2023: no changes
#define GPIO_INPUT_IO_1       4
#define ESP_INTR_FLAG_DEFAULT 0
#define GPIO_INPUT_PIN_SEL    1ULL<<GPIO_INPUT_IO_1

// LED Output pins definitions -- 2023: minor changes
#define BLUEPIN   14
#define GREENPIN  32
#define REDPIN    15
#define ONBOARD   13
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<BLUEPIN) | (1ULL<<GREENPIN) | (1ULL<<REDPIN) | (1ULL<<ONBOARD) )

// Default ID/color
#define ID 3
#define COLOR 'R'


// Mutex (for resources), and Queues (for button)
static QueueHandle_t gpio_evt_queue = NULL; // 2023: Changed
// static xQueueHandle_t timer_queue; -- 2023: removed

// A simple structure to pass "events" to main task -- 2023: modified
typedef struct {
    uint64_t event_count;
} example_queue_element_t;

// Create a FIFO queue for timer-based events -- Modified


// System tags for diagnostics -- 2023: modified
static const char *TAG_SYSTEM = "ec444: system";       // For debug logs
static const char *TAG_TIMER = "ec444: timer";         // For timer logs
static const char *TAG_UART = "ec444: uart";           // For UART logs 

void pwm_init();
void uart_init();
void alarm_init();
void button_init();
//void led_init();

void button_task();
void send_task();
void recv_task();
//void led_task();
void id_task();
void timer_evt_task();