#include "move.h"


void example_ledc_init(void)
{
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE,
        .duty_resolution  = LEDC_DUTY_RES,
        .timer_num        = LEDC_TIMER,
        .freq_hz          = LEDC_FREQUENCY,  // Set output frequency at 4 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel_l = {
        .speed_mode     = LEDC_MODE,
        .channel        = LEDC_CHANNEL_L,
        .timer_sel      = LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = LEDC_OUTPUT_IO1,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };

    ledc_channel_config_t ledc_channel_r = {
        .speed_mode     = LEDC_MODE,
        .channel        = LEDC_CHANNEL_R,
        .timer_sel      = LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = LEDC_OUTPUT_IO2,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };

    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_l));
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_r));

    // Direction control pin settings
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

void movement(char direction, uint16_t duty)
{
    switch(direction)
    {
        case 'W':
            gpio_set_level(L_FORWARD, 1);
            gpio_set_level(L_BACKWARD, 0);
            gpio_set_level(R_FORWARD, 0);
            gpio_set_level(R_BACKWARD, 1);

            ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_L, duty));
            ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_L));

            ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_R, duty));
            ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_R));
            break;
        case 'S':
            gpio_set_level(L_FORWARD, 0);
            gpio_set_level(L_BACKWARD, 1);
            gpio_set_level(R_FORWARD, 1);
            gpio_set_level(R_BACKWARD, 0);

            ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_L, duty));
            ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_L));

            ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_R, duty));
            ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_R));
            break;
        case 'A':
            gpio_set_level(L_FORWARD, 1);
            gpio_set_level(L_BACKWARD, 0);
            gpio_set_level(R_FORWARD, 0);
            gpio_set_level(R_BACKWARD, 1);

            ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_L, duty));
            ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_L));

            ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_R, 0));
            ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_R));
            break;
        case 'D':
            gpio_set_level(L_FORWARD, 1);
            gpio_set_level(L_BACKWARD, 0);
            gpio_set_level(R_FORWARD, 0);
            gpio_set_level(R_BACKWARD, 1);

            ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_L, 0));
            ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_L));

            ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_R, duty));
            ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_R));
    }
}
