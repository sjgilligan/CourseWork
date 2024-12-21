#include <string.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "freertos/queue.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#define HEART_BEAT 1*1000
#define LEADER_TIMEOUT 6*1000000
#define ELECTION_TIMEOUT 10*1000000


typedef enum {			// Set of network states
	HAS_LEADER,
	NEED_ELECTION,
    ELECTION,
} network_states;

typedef enum {			// Set of individual states
	LEADER,
	FOLLOWER,
	SHOULD_BE_LEADER,
	NONE,
} individual_states;


void heartbeat_task();

void listen_task();

void timer_init();

void print_state_task();