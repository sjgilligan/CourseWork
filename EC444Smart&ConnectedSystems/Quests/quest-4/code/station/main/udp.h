#ifndef _udp_h   /* Include guard */
#define _udp_h

#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "freertos/queue.h"


//#include "protocol_examples_common.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>
#define CONFIG_EXAMPLE_IPV4 1

#ifdef CONFIG_EXAMPLE_SOCKET_IP_INPUT_STDIN
#include "addr_from_stdin.h"
#endif
// Rasberry PI IP: 192.168.1.147
#if defined(CONFIG_EXAMPLE_IPV4)
#define ESP_ADDR {132, 142, 110, 118}
#define HOST_IP_ADDR "192.168.1.147" 
#elif defined(CONFIG_EXAMPLE_IPV6)
#define HOST_IP_ADDR CONFIG_EXAMPLE_IPV6_ADDR
#else
#define HOST_IP_ADDR ""
#endif

#define PORT 3333
#define LEADER_PORT 3334
#define ESP_SERVER_PORT 5555
static const char *TAG = "example";
static const char *CLIENT_TAG = "Client_task";
static const char *SERVER_TAG = "Server_task";
static const char *LEADER_TAG = "Leader";
static const char *FOLLOWER_TAG = "Follower";
static const char *FORWARD_TAG = "PI_Forward";
static const char *CONFIRM_TAG = "Confirm";



#define MAX_CAPACITY 4
#define QUEUE_SIZE 10

typedef struct {
    size_t head;
    size_t tail;
    size_t size;
    void** data;
} queue_t;

void udp_client_task(void *pvParameters);
void udp_server_task(void *pvParameters);
void udp_leader_task(void *pvParameters);
void udp_leader_forward_task(void *pvParameters);
void udp_confirm_client_task(void *pvParameters);

size_t queue_count(queue_t *queue);
int queue_write(queue_t *queue, void* handle);
void* queue_read(queue_t *queue);

void add_ip_id(int ip_id_to_add);
int is_unique_ip_id(int ip_id_to_check);




#endif