//Aymeric Blaizot, Amado Diallo, Sebastian Gilligan, Eve Cruanes 10/25/2024 EC444
#include "buzzer.h"

dac_cosine_handle_t chan0_handle;

extern uint8_t cat_rank;

void buzzer_init(void)
{

    dac_cosine_config_t cos0_cfg = {
        .chan_id = DAC_CHAN_0,
        .freq_hz = 8000, // It will be covered by 8000 in the latter configuration
        .clk_src = DAC_COSINE_CLK_SRC_DEFAULT,
        .offset = 0,
        .phase = DAC_COSINE_PHASE_0,
        .atten = DAC_COSINE_ATTEN_DEFAULT,
        .flags.force_set_freq = false,
    };
	
    ESP_ERROR_CHECK(dac_cosine_new_channel(&cos0_cfg, &chan0_handle));

}

void buzzer_task(void){
	static int current = 0;
	while(1){

		if(cat_rank == 1 && current != 1){
        	ESP_LOGI(BUZZER, "Buzzing on");
			printf("start\n");
			ESP_ERROR_CHECK(dac_cosine_start(chan0_handle));
			current = cat_rank;
        	vTaskDelay(10000 / portTICK_PERIOD_MS);
			cat_rank = 0;
		}
		
		if(cat_rank != 1 && current == 1){
			ESP_LOGI(BUZZER, "Buzzing off");
			ESP_ERROR_CHECK(dac_cosine_stop(chan0_handle));
			printf("stop\n");
			current = cat_rank;
		}	
		vTaskDelay(pdMS_TO_TICKS(100));
	}
}