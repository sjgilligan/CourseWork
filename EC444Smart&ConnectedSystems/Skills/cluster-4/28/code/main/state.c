#include "state.h"
#include "udp.h"
#include "driver/gptimer.h"     

extern QueueHandle_t incoming_queue;

network_states current_net_state = NEED_ELECTION;
individual_states current_ind_state = NONE;
extern char client_payload[20];
extern int ip_ids_live[4];
extern size_t ip_ids_size;

int leader_id;
// extern queue_t payload_queue;

char incoming_payload[20];

extern const uint8_t all_addr[4];

extern uint8_t ip_id;

int min(int* nums){
	int lowest = 999999;
	int nums_len = 3;
	
	for(int i = 0; i < nums_len; i++){
		if(nums[i] < lowest && nums[i] > 0){
			lowest = nums[i];
			printf("Looking for min: %d\n", nums[i]);
		}
	}
	
	return lowest;
}

gptimer_handle_t gptimer = NULL;
gptimer_handle_t electiontimer = NULL;

void timer_init() {

    gptimer_config_t timer_config = {
      .clk_src = GPTIMER_CLK_SRC_DEFAULT,
      .direction = GPTIMER_COUNT_UP,
      .resolution_hz = 1000000, // 1MHz, 1 tick=1us
    };
    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &gptimer)); // instantiates timer
    ESP_ERROR_CHECK(gptimer_enable(gptimer));
    ESP_ERROR_CHECK(gptimer_start(gptimer));                            // this starts the timer

	
    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &electiontimer)); // instantiates timer
    ESP_ERROR_CHECK(gptimer_enable(electiontimer));
    ESP_ERROR_CHECK(gptimer_start(electiontimer));                            // this starts the timer
}

void print_state_task(){
	while(1){
		switch(current_ind_state){
			case LEADER:
				printf("IND STATE: LEADER\n");
				break;
			case FOLLOWER:
				printf("IND STATE: FOLLOWER\n");
				break;
			case SHOULD_BE_LEADER:
				printf("IND STATE: SHOULD_BE_LEADER\n");
				break;
			case NONE:
				printf("IND STATE: NONE\n");
				break;
		}
		
		switch(current_net_state){
			case HAS_LEADER:
				printf("NET STATE: HAS_LEADER\n");
				break;
			case NEED_ELECTION:
				printf("NET STATE: NEED_ELECTION\n");
				break;
			case ELECTION:
				printf("NET STATE: ELECTION\n");
				break;
		}
		vTaskDelay(1000/portTICK_PERIOD_MS);
	}
}

void heartbeat_task(){
	int wait_for_hb = 0;
	while(1){
		if(current_ind_state == LEADER){
			sprintf(client_payload,"%c,%d\n",'h',ip_id);
			//printf("Heartbeat sent\n");
		}
		
		if(current_ind_state == FOLLOWER){
			//printf("looking for heartbeat\n");
			
			uint64_t count;

			 ESP_ERROR_CHECK(gptimer_get_raw_count(gptimer, &count));
			 
			//printf("count: %lld",count/1000000);

			if(count > LEADER_TIMEOUT){
				wait_for_hb++;
				printf("Heartbeat missed!\n");
			}
			
			if(wait_for_hb == 3){
				//clear ip_ids_live
				ip_ids_live[0] = 0;
				ip_ids_live[1] = 0;
				ip_ids_live[2] = 0;
				ip_ids_live[3] = 0;

				ip_ids_size = 0;
				
				add_ip_id(ip_id);

				sprintf(client_payload,"%c,%d\n",'e',ip_id);
				current_net_state = NEED_ELECTION;
				printf("Heartbeat missed 3 times, election!\n");
				
				
				//START TIMER
				
				ESP_ERROR_CHECK(gptimer_get_raw_count(electiontimer, &count));
				
				if(count > ELECTION_TIMEOUT){
					printf("Election timeout! I am a lonely leader\n");
					current_net_state = HAS_LEADER;
					current_ind_state = LEADER;
				}
				
		
				wait_for_hb = 0;
				
			}
		}
		
		vTaskDelay(10/portTICK_PERIOD_MS);
	}
}
		

void listen_task(){
	// //modifies current_net_state
	sprintf(client_payload,"%c,%d\n",'e',ip_id);
	while(1){
	while(1){

		//printf("listen_task\n");
		// char * temp;
		// temp = (char *)queue_read(&payload_queue);

		if(incoming_queue == NULL){
			vTaskDelay(20/portTICK_PERIOD_MS);
			break;
		}

		if(!(xQueueReceive(incoming_queue, &(incoming_payload), (TickType_t)5))){
			vTaskDelay(10/portTICK_PERIOD_MS);
			break;
		}
		ESP_LOGI(TAG, "Queue RX: %s", incoming_payload);

		if(incoming_payload[0] == 'v'){ //new leader signal
			current_net_state = HAS_LEADER;
			current_ind_state = FOLLOWER;
			
			printf("New Leader received! Leader ID: %d\n",leader_id);
		}
		
		if(incoming_payload[0] == 'h'){ //heartbeat signal
	
			
			ESP_ERROR_CHECK(gptimer_set_raw_count(gptimer, 0));
			
			current_net_state = HAS_LEADER;
			current_ind_state = FOLLOWER;
			
			int h_leader_id = 999999;
			
			char* token = strtok(incoming_payload, ",");
            if (token != NULL) {
                char code_letter = atoi(token);
                token = strtok(NULL, ",");
                if (token != NULL) {
                    h_leader_id = atoi(token);
                }
            }
			
			leader_id = h_leader_id;
			printf("Heartbeat receieved from %d\n",leader_id);
			
			if(ip_id < h_leader_id){
				current_ind_state = SHOULD_BE_LEADER;
				
				sprintf(client_payload,"%c,%d\n",'e',ip_id);
				current_net_state = NEED_ELECTION;
				printf("I should be leader!\n");
			}
			
			
		}
		
		if(incoming_payload[0] == 'e'){ //need election signal
			
			printf("election received!\n");
			
			sprintf(client_payload,"%c,%d\n",'a',ip_id);
			
			printf("election answer sent!\n");
			
			
		}
		
		if(incoming_payload[0] == 'a'){
			gptimer_set_raw_count(electiontimer, 0);
			if(ip_id == min(ip_ids_live)){
				sprintf(client_payload,"%c,%d\n",'v',ip_id);
				current_net_state = HAS_LEADER;
				current_ind_state = LEADER;
				printf("I am now leader!\n");
			}else{
				printf("I am following %d\n", min(ip_ids_live));
			}
			
		}
			
		vTaskDelay(10/portTICK_PERIOD_MS);
	}
	}
}


// void fsm_task(){
	// while(1){
		// if(current_net_state == NEED_ELECTION){
			// int ids_len;
			// message = 'e';
			// transmit_id(id); //answer message
			
			// // suspend.listen_task();
			// // get_ids(all_ids);
			// //min_id = min(all_ids);
			// // resume.listen_task();
			
			// if(id == 192){
				// // current_ind_state = LEADER;
				// // transmit_sig("new_leader");
				// // current_net_state = HAS_LEADER;
				
				// // while(current_net_state == HAS_LEADER){
				// // 	send_heartbeat(id, min_id, current_ind_state);
				// // 	sleep(HEART_BEAT);
				// // }
				
			// }else{
				// current_ind_state = FOLLOWER;
			// }
			
			
			
	// // 		}
	// // `
				
		// }
		// vTaskDelay(1000 / portTICK_PERIOD_MS);
		// }
	// }
	