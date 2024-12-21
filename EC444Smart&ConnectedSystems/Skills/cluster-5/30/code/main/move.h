#include "driver/ledc.h"
#include "driver/gpio.h"        // Added in 2023
#include "esp_err.h"

#define LEDC_TIMER              LEDC_TIMER_0
//#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_MODE               LEDC_HIGH_SPEED_MODE
#define LEDC_OUTPUT_IO1         (32) 
#define LEDC_OUTPUT_IO2           (14) 
#define LEDC_CHANNEL_L            LEDC_CHANNEL_0
#define LEDC_CHANNEL_R            LEDC_CHANNEL_1
//#define LEDC_DUTY_RES           LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define LEDC_DUTY_RES           LEDC_TIMER_10_BIT // Set duty resolution to 10 bits
#define LEDC_DUTY               (512) // Set duty to 50%. (2 ** 10) * 50% = 512
#define LEDC_FREQUENCY          (50) // Frequency in Hertz. Set frequency at 50 Hz

#define L_FORWARD               25 // A1
#define L_BACKWARD              21

#define R_FORWARD               15 // A1
#define R_BACKWARD              33

#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<L_FORWARD) | (1ULL<<L_BACKWARD) | (1ULL<<R_FORWARD) | (1ULL<<R_BACKWARD) )

void example_ledc_init(void);

void movement(uint16_t duty_left, uint16_t duty_right, bool direction);

void wasd_control(char direction, uint16_t duty);
