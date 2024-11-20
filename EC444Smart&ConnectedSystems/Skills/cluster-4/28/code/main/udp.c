#include "udp.h"


#include <stdlib.h>
#include <assert.h>



// size_t queue_count(queue_t *queue) {
//   return queue->head - queue->tail;
// }

// void* queue_read(queue_t *queue) {
//   if (queue->head == queue->tail) {
//     return NULL;
//   }
//   void* handle = queue->data[queue->tail % queue->size];
//   queue->tail ++;
//   return handle;
// }

// int queue_write(queue_t *queue, void* handle) {
//   if ((queue->head - queue->tail) == queue->size) {
//     return -1;
//   }
//   printf("Queue write\n");
//   queue->data[queue->head % queue->size] = handle;
//   printf("Queue write 2\n");
//   queue->head ++;
// return 0;
// }

// queue_t payload_queue = {0, 0, 1000, NULL};

QueueHandle_t incoming_queue;

const uint8_t all_addr[4] = ESP_ADDR;
uint8_t live_addr;
char client_payload[20];
uint8_t cat_rank = 0;
extern uint8_t ip_id;

int ip_ids_live[MAX_CAPACITY];
size_t ip_ids_size = 0;

//**AI GENERATED */
int is_unique_ip_id(int ip_id_to_check) {
    for (size_t i = 0; i < ip_ids_size; i++) {
        if (ip_ids_live[i] == ip_id_to_check) {
            return 0; // Not unique
        }
    }
    return 1; // Unique
}
//**AI GENERATED */
// Circular buffer for incoming payloads
//char payload_queue[QUEUE_SIZE][20];


//**AI GENERATED */
void add_ip_id(int ip_id_to_add) {
    if (ip_ids_size < MAX_CAPACITY) {
        if (is_unique_ip_id(ip_id_to_add)) {
            ip_ids_live[ip_ids_size++] = ip_id_to_add;
            ESP_LOGI(TAG, "ip_id %d added", ip_id_to_add);
        } else {
            ESP_LOGW(TAG, "ip_id %d is not unique, not adding", ip_id_to_add);
        }
    } else {
        ESP_LOGW(TAG, "ip_ids array is full, cannot add more ip_ids");
    }
}

//**AI GENERATED */
// Function to return a copy of all_addr that does not contain the ip_id
void get_filtered_addr(uint8_t ip_id, uint8_t *filtered_addr, size_t *filtered_size) {
    size_t j = 0;
    for (size_t i = 0; i < sizeof(all_addr); i++) {
        if (all_addr[i] != ip_id) {
            filtered_addr[j++] = all_addr[i];
        }
    }
    *filtered_size = j;
}

//char message[20];
void udp_client_task(void *pvParameters)
{
    add_ip_id(ip_id); // initialize the ip_ids_live array with the device's own ip_id
    char rx_buffer[128];
    char host_ip[14];

    int addr_family = 0;
    int ip_protocol = 0;
    uint16_t port = 5555;
    uint8_t filtered_addr[sizeof(all_addr)-1];
    size_t filtered_size = 0;

    get_filtered_addr(ip_id, filtered_addr, &filtered_size); // Get rid of the device's own ip_id from the list

    while (1) {

    for(int i = 0; i < filtered_size; i++) {
        sprintf(host_ip, "192.168.1.%d", filtered_addr[i]);


#if defined(CONFIG_EXAMPLE_IPV4)
        struct sockaddr_in dest_addr;
        dest_addr.sin_addr.s_addr = inet_addr(host_ip);
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(port);
        addr_family = AF_INET;
        ip_protocol = IPPROTO_IP;
#elif defined(CONFIG_EXAMPLE_IPV6)
        struct sockaddr_in6 dest_addr = { 0 };
        inet6_aton(HOST_IP_ADDR, &dest_addr.sin6_addr);
        dest_addr.sin6_family = AF_INET6;
        dest_addr.sin6_port = htons(PORT);
        dest_addr.sin6_scope_id = esp_netif_get_netif_impl_index(EXAMPLE_INTERFACE);
        addr_family = AF_INET6;
        ip_protocol = IPPROTO_IPV6;
#elif defined(CONFIG_EXAMPLE_SOCKET_IP_INPUT_STDIN)
        struct sockaddr_storage dest_addr = { 0 };
        ESP_ERROR_CHECK(get_addr_from_stdin(PORT, SOCK_DGRAM, &ip_protocol, &addr_family, &dest_addr));
#endif

        int sock = socket(addr_family, SOCK_DGRAM, ip_protocol);
        if (sock < 0) {
            ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
            break;
        }

        // Set timeout
        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout);

        ESP_LOGI(TAG, "Socket created, sending to %s:%d", host_ip, port);

        for (int i = 0; i < 1; i++) {

            int err = sendto(sock, client_payload, strlen(client_payload), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
            if (err < 0) {
                ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
                break;
            }
            ESP_LOGI(TAG, "Message sent %s", client_payload);

            struct sockaddr_storage source_addr; // Large enough for both IPv4 or IPv6
            socklen_t socklen = sizeof(source_addr);
            int len = recvfrom(sock, rx_buffer, sizeof(rx_buffer) - 1, 0, (struct sockaddr *)&source_addr, &socklen);

            // Error occurred during receiving
            if (len < 0) {
                ESP_LOGE(TAG, "recvfrom failed: errno %d", errno);
                break;
            }
            // Data received
            else {
                rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string
                ESP_LOGI(TAG, "Received %d bytes from %s:", len, host_ip);
                ESP_LOGI(TAG, "%s", rx_buffer);

                 char* token = strtok(rx_buffer, ",");
                if (token != NULL) {
                    char code_letter = atoi(token);
                    token = strtok(NULL, ",");
                    if (token != NULL) {
                        int ip_id_incoming = atoi(token);
                        add_ip_id(ip_id_incoming); // Add the ip_id to the list
                        // Do something with the number
                    }
                    //print ip_ids_live
                    for (size_t i = 0; i < ip_ids_size; i++) {
                        ESP_LOGI(TAG, "ip_id %d", ip_ids_live[i]);
                    }
                }
                char incoming_payload[20];            
                // Parse cat rank from two numbers seperated by a space
                strcpy(incoming_payload, rx_buffer);
                ESP_LOGI(TAG, "CLIENT INCOMING PAYLOAD: %s", incoming_payload);

                // int debug = queue_write(&payload_queue, &incoming_payload);

                if(!(xQueueSend(incoming_queue, (void*)incoming_payload, (TickType_t)0)))
                {
                    ESP_LOGE(TAG,"Failed to send to queue\n");
                }
                else
                {
                    ESP_LOGI(TAG,"Sent to queue\n");
                }
                
            }

            vTaskDelay(20 / portTICK_PERIOD_MS);
        }       

        if (sock != -1) {
            ESP_LOGE(TAG, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);
        }
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}
#define ESP_SERVER_PORT 5555
void udp_server_task(void *pvParameters)
{
    char incoming_payload[20];
    incoming_queue = xQueueCreate(10, sizeof(incoming_payload)); 
    if (incoming_queue == 0)
    {
     ESP_LOGE(TAG,"Failed to create queue= %p\n", incoming_queue);
    }

    char rx_buffer[128];
    char addr_str[128];
    int addr_family = (int)pvParameters;
    int ip_protocol = 0;
    struct sockaddr_in6 dest_addr;
    while (1) {
        if (addr_family == AF_INET) {
            struct sockaddr_in *dest_addr_ip4 = (struct sockaddr_in *)&dest_addr;
            dest_addr_ip4->sin_addr.s_addr = htonl(INADDR_ANY);
            dest_addr_ip4->sin_family = AF_INET;
            dest_addr_ip4->sin_port = htons(ESP_SERVER_PORT);
            ip_protocol = IPPROTO_IP;
        } else if (addr_family == AF_INET6) {
            bzero(&dest_addr.sin6_addr.un, sizeof(dest_addr.sin6_addr.un));
            dest_addr.sin6_family = AF_INET6;
            dest_addr.sin6_port = htons(ESP_SERVER_PORT);
            ip_protocol = IPPROTO_IPV6;
        }
        int sock = socket(addr_family, SOCK_DGRAM, ip_protocol);
        if (sock < 0) {
            ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG, "Server Socket created");
#if defined(CONFIG_LWIP_NETBUF_RECVINFO) && !defined(CONFIG_EXAMPLE_IPV6)
        int enable = 1;
        lwip_setsockopt(sock, IPPROTO_IP, IP_PKTINFO, &enable, sizeof(enable));
#endif
#if defined(CONFIG_EXAMPLE_IPV4) && defined(CONFIG_EXAMPLE_IPV6)
        if (addr_family == AF_INET6) {
            // Note that by default IPV6 binds to both protocols, it is must be disabled
            // if both protocols used at the same time (used in CI)
            int opt = 1;
            setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
            setsockopt(sock, IPPROTO_IPV6, IPV6_V6ONLY, &opt, sizeof(opt));
        }
#endif
        // Set timeout
        struct timeval timeout;
        timeout.tv_sec = 10;
        timeout.tv_usec = 0;
        setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout);
        int err = bind(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
        if (err < 0) {
            ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
        }
        ESP_LOGI(TAG, "Socket bound, port %d", ESP_SERVER_PORT);
        struct sockaddr_storage source_addr; // Large enough for both IPv4 or IPv6
        socklen_t socklen = sizeof(source_addr);
#if defined(CONFIG_LWIP_NETBUF_RECVINFO) && !defined(CONFIG_EXAMPLE_IPV6)
        struct iovec iov;
        struct msghdr msg;
        struct cmsghdr *cmsgtmp;
        u8_t cmsg_buf[CMSG_SPACE(sizeof(struct in_pktinfo))];
        iov.iov_base = rx_buffer;
        iov.iov_len = sizeof(rx_buffer);
        msg.msg_control = cmsg_buf;
        msg.msg_controllen = sizeof(cmsg_buf);
        msg.msg_flags = 0;
        msg.msg_iov = &iov;
        msg.msg_iovlen = 1;
        msg.msg_name = (struct sockaddr *)&source_addr;
        msg.msg_namelen = socklen;
#endif
        while (1) {
            ESP_LOGI(TAG, "Waiting for data");
#if defined(CONFIG_LWIP_NETBUF_RECVINFO) && !defined(CONFIG_EXAMPLE_IPV6)
            int len = recvmsg(sock, &msg, 0);
#else
            int len = recvfrom(sock, rx_buffer, sizeof(rx_buffer) - 1, 0, (struct sockaddr *)&source_addr, &socklen);
#endif
            // Error occurred during receiving
            if (len < 0) {
                ESP_LOGE(TAG, "recvfrom failed: errno %d", errno);
                break;
            }
            // Data received
            else {
                // Get the sender's ip address as string
                if (source_addr.ss_family == PF_INET) {
                    inet_ntoa_r(((struct sockaddr_in *)&source_addr)->sin_addr, addr_str, sizeof(addr_str) - 1);
#if defined(CONFIG_LWIP_NETBUF_RECVINFO) && !defined(CONFIG_EXAMPLE_IPV6)
                    for ( cmsgtmp = CMSG_FIRSTHDR(&msg); cmsgtmp != NULL; cmsgtmp = CMSG_NXTHDR(&msg, cmsgtmp) ) {
                        if ( cmsgtmp->cmsg_level == IPPROTO_IP && cmsgtmp->cmsg_type == IP_PKTINFO ) {
                            struct in_pktinfo *pktinfo;
                            pktinfo = (struct in_pktinfo*)CMSG_DATA(cmsgtmp);
                            ESP_LOGI(TAG, "dest ip: %s", inet_ntoa(pktinfo->ipi_addr));
                        }
                    }
#endif
                } else if (source_addr.ss_family == PF_INET6) {
                    inet6_ntoa_r(((struct sockaddr_in6 *)&source_addr)->sin6_addr, addr_str, sizeof(addr_str) - 1);
                }
                rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string...
                ESP_LOGI(TAG, "Received %d bytes from %s: on Port: %d", len, addr_str, ESP_SERVER_PORT);
                char tx_buffer[100];
               // strcpy(tx_buffer, incoming_payload);
                //strcat(tx_buffer, rx_buffer);


                strcpy(incoming_payload, rx_buffer);
                ESP_LOGI(TAG, "SERVER INCOMING PAYLOAD: %s", incoming_payload);

                // int debug = queue_write(&payload_queue, &incoming_payload);

                if(!(xQueueSend(incoming_queue, (void*)incoming_payload, (TickType_t)0)))
                {
                    ESP_LOGE(TAG,"Failed to send to queue\n");
                }
                else
                {
                    ESP_LOGI(TAG,"Sent to queue\n");
                }

                //  printf("Incoming Payload: %s, queue write %d\n", incoming_payload, debug);      

               // enqueue_payload(incoming_payload);
                
            char* token = strtok(rx_buffer, ",");
                if (token != NULL) {
                    char code_letter = atoi(token);
                    token = strtok(NULL, ",");
                    if (token != NULL) {
                        int ip_id_incoming = atoi(token);
                        add_ip_id(ip_id_incoming); // Add the ip_id to the list
                        // Do something with the number
                         //print ip_ids_live
                        for (size_t i = 0; i < ip_ids_size; i++) {
                            ESP_LOGI(TAG, "ip_id %d", ip_ids_live[i]);
                        }
                    }
                }
                sprintf(tx_buffer,"a,%d",ip_id);


                cat_rank = atoi(rx_buffer);
                int err = sendto(sock, tx_buffer, len+100, 0, (struct sockaddr *)&source_addr, sizeof(source_addr));
                if (err < 0) {
                    ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
                    break;
                }
                ESP_LOGI(TAG, "Server Reply: %s" , tx_buffer);
            }
        }
        if (sock != -1) {
            ESP_LOGE(TAG, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);
        }
    }
    vTaskDelete(NULL);
}