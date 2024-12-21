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
//#define ESP_ADDR {132, 142, 110, 118}
#define HOST_IP_ADDR "192.168.1.147" 
#elif defined(CONFIG_EXAMPLE_IPV6)
#define HOST_IP_ADDR CONFIG_EXAMPLE_IPV6_ADDR
#else
#define HOST_IP_ADDR ""
#endif

void udp_server_task(void *pvParameters);

#endif