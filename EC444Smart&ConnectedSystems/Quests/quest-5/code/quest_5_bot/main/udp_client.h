//EC444 Team 4 Eve Cruanes, Sebastian Gillian, Aymeric Blaizot, Amado Diallo
#include <stdio.h>
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
//#include "protocol_examples_common.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
//EC444 Team 4 Eve Cruanes, Sebastian Gillian, Aymeric Blaizot, Amado Diallo
#include "lwip/sys.h"
#include <lwip/netdb.h>

//#include "parseNodeString.c"

#ifdef CONFIG_EXAMPLE_SOCKET_IP_INPUT_STDIN
#include "addr_from_stdin.h"
#endif

#define CONFIG_EXAMPLE_IPV4 1

#if defined(CONFIG_EXAMPLE_IPV4)
#define HOST_IP_ADDR "192.168.0.167"
#elif defined(CONFIG_EXAMPLE_IPV6)
#define HOST_IP_ADDR CONFIG_EXAMPLE_IPV6_ADDR
#else
#define HOST_IP_ADDR "192.168.0.167"
#endif

#define PORT 41234

typedef struct {
    int id;
    float x;
    float z;
    float theta;
    char status[10];
} RobotData;

int parseRobotData(const char *input, RobotData *data);
void udp_client_task(void *pvParameters);