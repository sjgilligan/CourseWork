#include "udp.h"
#include "state.h"

#include <stdlib.h>
#include <assert.h>

QueueHandle_t incoming_queue;
QueueHandle_t voter_queue;
QueueHandle_t confirm_queue;

const uint8_t all_addr[4] = ESP_ADDR;
uint8_t live_addr;
char client_payload[20];
uint8_t cat_rank = 0;
extern uint8_t ip_id;
extern char currentVote;
extern individual_states current_ind_state;

int ip_ids_live[MAX_CAPACITY];
size_t ip_ids_size = 0;
extern uint8_t voterID;

extern int leader_id;

uint8_t confirm_led = 0;

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

// Client Task for leader election
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
            ESP_LOGE(CLIENT_TAG, "Unable to create socket: errno %d", errno);
            break;
        }

        // Set timeout
        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout);

        ESP_LOGI(CLIENT_TAG, "Socket created, sending to %s:%d", host_ip, port);

        for (int i = 0; i < 1; i++) {

            int err = sendto(sock, client_payload, strlen(client_payload), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
            if (err < 0) {
                ESP_LOGE(CLIENT_TAG, "Error occurred during sending: errno %d", errno);
                break;
            }
            ESP_LOGI(CLIENT_TAG, "Message sent %s", client_payload);

            struct sockaddr_storage source_addr; // Large enough for both IPv4 or IPv6
            socklen_t socklen = sizeof(source_addr);
            int len = recvfrom(sock, rx_buffer, sizeof(rx_buffer) - 1, 0, (struct sockaddr *)&source_addr, &socklen);

            // Error occurred during receiving
            if (len < 0) {
                ESP_LOGE(CLIENT_TAG, "recvfrom failed: errno %d", errno);
                break;
            }
            // Data received
            else {
                rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string
                ESP_LOGI(CLIENT_TAG, "Received %d bytes from %s:", len, host_ip);
                ESP_LOGI(CLIENT_TAG, "%s", rx_buffer);

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
                        ESP_LOGI(CLIENT_TAG, "ip_id %d", ip_ids_live[i]);
                    }
                }
                char incoming_payload[20];            
                // Parse cat rank from two numbers seperated by a space
                strcpy(incoming_payload, rx_buffer);
                ESP_LOGI(CLIENT_TAG, "CLIENT INCOMING PAYLOAD: %s", incoming_payload);

                // int debug = queue_write(&payload_queue, &incoming_payload);

                if(!(xQueueSend(incoming_queue, (void*)incoming_payload, (TickType_t)0)))
                {
                    ESP_LOGE(CLIENT_TAG,"Failed to send to queue\n");
                }
                else
                {
                    ESP_LOGI(CLIENT_TAG,"Sent to queue\n");
                }
                
            }

            vTaskDelay(20 / portTICK_PERIOD_MS);
        }       

        if (sock != -1) {
            ESP_LOGE(CLIENT_TAG, "Shutting down socket and restarting...");
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
     ESP_LOGE(SERVER_TAG,"Failed to create queue= %p\n", incoming_queue);
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
            ESP_LOGE(SERVER_TAG, "Unable to create socket: errno %d", errno);
            break;
        }
        ESP_LOGI(SERVER_TAG, "Server Socket created");
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
            ESP_LOGE(SERVER_TAG, "Socket unable to bind: errno %d", errno);
        }
        ESP_LOGI(SERVER_TAG, "Socket bound, port %d", ESP_SERVER_PORT);
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
            ESP_LOGI(SERVER_TAG, "Waiting for data");
#if defined(CONFIG_LWIP_NETBUF_RECVINFO) && !defined(CONFIG_EXAMPLE_IPV6)
            int len = recvmsg(sock, &msg, 0);
#else
            int len = recvfrom(sock, rx_buffer, sizeof(rx_buffer) - 1, 0, (struct sockaddr *)&source_addr, &socklen);
#endif
            // Error occurred during receiving
            if (len < 0) {
                ESP_LOGE(SERVER_TAG, "recvfrom failed: errno %d", errno);
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
                ESP_LOGI(SERVER_TAG, "Received %d bytes from %s: on Port: %d", len, addr_str, ESP_SERVER_PORT);
                char tx_buffer[100];
               // strcpy(tx_buffer, incoming_payload);
                //strcat(tx_buffer, rx_buffer);


                strcpy(incoming_payload, rx_buffer);
                ESP_LOGI(SERVER_TAG, "SERVER INCOMING PAYLOAD: %s", incoming_payload);

                // int debug = queue_write(&payload_queue, &incoming_payload);

                if(incoming_payload[0]== '1'){
                    if(!(xQueueSend(voter_queue, (void*)incoming_payload, (TickType_t)0)))
                    {
                        ESP_LOGE(FORWARD_TAG,"Failed to send to voter queue\n");
                    }
                    else
                    {
                        char* token = strtok(rx_buffer, " ");
                        int voter_id_number = 0;
                        if (token != NULL) {
                            voter_id_number = atoi(token);
                            token = strtok(NULL, " ");
                        }
                        char confim_payload[10];
                        sprintf(confim_payload,"c,%d",voter_id_number);

                        if(!(xQueueSend(confirm_queue, (void*)confim_payload, (TickType_t)0)))
                        {
                            ESP_LOGE(FORWARD_TAG,"Failed to send to confirm queue\n");
                        }

                        strcpy(tx_buffer, confim_payload);
                        
                        ESP_LOGI(FORWARD_TAG,"Sent to confirm queue\n");
                    }
                }else if (incoming_payload[0]== 'c'){
                    ESP_LOGI(FOLLOWER_TAG, "Received confirmation");
                    confirm_led = 1;
				}
                else if(!(xQueueSend(incoming_queue, (void*)incoming_payload, (TickType_t)0)))
                {
                    ESP_LOGE(SERVER_TAG,"Failed to send to queue\n");
                }
                else
                {
                    ESP_LOGI(SERVER_TAG,"Sent to queue\n");
                    sprintf(tx_buffer,"a,%d",ip_id);
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
                            ESP_LOGI(SERVER_TAG, "ip_id %d", ip_ids_live[i]);
                        }
                    }
                }
                }             
    

                int err = sendto(sock, tx_buffer, len+100, 0, (struct sockaddr *)&source_addr, sizeof(source_addr));
                if (err < 0) {
                    ESP_LOGE(SERVER_TAG, "Error occurred during sending: errno %d", errno);
                    break;
                }
                ESP_LOGI(SERVER_TAG, "Server Reply: %s" , tx_buffer);
            }
        }
        if (sock != -1) {
            ESP_LOGE(SERVER_TAG, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);
        }
    }
    vTaskDelete(NULL);
}

// Client Task for leader to forward votes to Rasberry Pi
void udp_leader_forward_task(void *pvParameters)
{
    char voter_payload[10];
    voter_queue = xQueueCreate(10, sizeof(voter_payload)); 
    if (voter_queue == 0)
    {
     ESP_LOGE(FORWARD_TAG,"Failed to create queue= %p\n", voter_queue);
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
            dest_addr_ip4->sin_port = htons(LEADER_PORT);
            ip_protocol = IPPROTO_IP;
        } else if (addr_family == AF_INET6) {
            bzero(&dest_addr.sin6_addr.un, sizeof(dest_addr.sin6_addr.un));
            dest_addr.sin6_family = AF_INET6;
            dest_addr.sin6_port = htons(LEADER_PORT);
            ip_protocol = IPPROTO_IPV6;
        }
        int sock = socket(addr_family, SOCK_DGRAM, ip_protocol);
        if (sock < 0) {
            ESP_LOGE(FORWARD_TAG, "Unable to create socket: errno %d", errno);
            break;
        }
        ESP_LOGI(FORWARD_TAG, "Server Socket created");
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
            ESP_LOGE(FORWARD_TAG, "Socket unable to bind: errno %d", errno);
        }
        ESP_LOGI(FORWARD_TAG, "Socket bound, port %d", LEADER_PORT);
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
            ESP_LOGI(FORWARD_TAG, "Waiting for data");
#if defined(CONFIG_LWIP_NETBUF_RECVINFO) && !defined(CONFIG_EXAMPLE_IPV6)
            int len = recvmsg(sock, &msg, 0);
#else
            int len = recvfrom(sock, rx_buffer, sizeof(rx_buffer) - 1, 0, (struct sockaddr *)&source_addr, &socklen);
#endif
            // Error occurred during receiving
            if (len < 0) {
                ESP_LOGE(FORWARD_TAG, "recvfrom failed: errno %d", errno);
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
                ESP_LOGI(FORWARD_TAG, "Received %d bytes from %s: on Port: %d", len, addr_str, LEADER_PORT);
                char tx_buffer[100];
               // strcpy(tx_buffer, incoming_payload);
                //strcat(tx_buffer, rx_buffer);


                strcpy(voter_payload, rx_buffer);
                ESP_LOGI(FORWARD_TAG, "SERVER VOTER INCOMING PAYLOAD: %s", voter_payload);

                // int debug = queue_write(&payload_queue, &incoming_payload);

                if(!(xQueueSend(voter_queue, (void*)voter_payload, (TickType_t)0)))
                {
                    ESP_LOGE(FORWARD_TAG,"Failed to send to queue\n");
                }
                else
                {
                    ESP_LOGI(FORWARD_TAG,"Sent to queue\n");
                }

                //  printf("Incoming Payload: %s, queue write %d\n", incoming_payload, debug);      

               // enqueue_payload(incoming_payload);
                
				char* token = strtok(rx_buffer, " ");
				int voter_id_number = 0;
                if (token != NULL) {
                    voter_id_number = atoi(token);
                    token = strtok(NULL, " ");
                }
				
                sprintf(tx_buffer,"c,%d",voter_id_number);


                int err = sendto(sock, tx_buffer, len+100, 0, (struct sockaddr *)&source_addr, sizeof(source_addr));
                if (err < 0) {
                    ESP_LOGE(FORWARD_TAG, "Error occurred during sending: errno %d", errno);
                    break;
                }
                ESP_LOGI(FORWARD_TAG, "Server Reply: %s" , tx_buffer);
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

// Client Task forward votes to Leader from IR_RX
void udp_leader_task(void *pvParameters)
{
    char rx_buffer[128];
    char host_ip[] = HOST_IP_ADDR;
    int addr_family = 0;
    int ip_protocol = 0;

    while (1) {
    if(current_ind_state == LEADER){
#if defined(CONFIG_EXAMPLE_IPV4)
        struct sockaddr_in dest_addr;
        dest_addr.sin_addr.s_addr = inet_addr(HOST_IP_ADDR);
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(PORT);
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
            ESP_LOGE(LEADER_TAG, "Unable to create socket: errno %d", errno);
            break;
        }

        // Set timeout
        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout);

        ESP_LOGI(LEADER_TAG, "Socket created, sending to %s:%d", HOST_IP_ADDR, PORT);

        while (current_ind_state == LEADER) {
			//if(currentVote != 'o'){
                char payload[20];
                //sprintf(payload, "%d %c", voterID, currentVote);
				
				if(voter_queue == NULL){
					vTaskDelay(20/portTICK_PERIOD_MS);
					break;
				}

				while(!(xQueueReceive(voter_queue, &(payload), (TickType_t)5))){
					vTaskDelay(10/portTICK_PERIOD_MS);
				}
                
				ESP_LOGI(TAG, "Queue Voter: %s", payload);

                int err = sendto(sock, payload, strlen(payload), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
                if (err < 0) {
                    ESP_LOGE(LEADER_TAG, "Error occurred during sending: errno %d", errno);
                    break;
                }

                char confirm_payload[10];

                char* token = strtok(payload, " ");
                int voter_id_number = 0;
                if (token != NULL) {
                    voter_id_number = atoi(token);
                    token = strtok(NULL, " ");
                }
                sprintf(confirm_payload,"c,%d",voter_id_number);

                if(!(voter_id_number == 0)){
                    if(!(xQueueSend(confirm_queue, (void*)confirm_payload, (TickType_t)0)))
                    {
                        ESP_LOGE(FORWARD_TAG,"Failed to send to confirm queue\n");
                    }
                }
                ESP_LOGI(LEADER_TAG, "Message sent to Rasberry Pi %s", payload);

                struct sockaddr_storage source_addr; // Large enough for both IPv4 or IPv6
                socklen_t socklen = sizeof(source_addr);
                int len = recvfrom(sock, rx_buffer, sizeof(rx_buffer) - 1, 0, (struct sockaddr *)&source_addr, &socklen);

                // Error occurred during receiving
                if (len < 0) {
                    ESP_LOGE(LEADER_TAG, "recvfrom failed: errno %d", errno);
                    break;
                }
                // Data received
                else {
                    rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string
                    ESP_LOGI(LEADER_TAG, "Received %d bytes from %s:", len, host_ip);
                    ESP_LOGI(LEADER_TAG, "%s", rx_buffer);
                    // Parse cat rank from two numbers seperated by a space
                    
                    //cat_rank = atoi(rx_buffer);
                }
                //currentVote = 'o';
               // vTaskDelay(500 / portTICK_PERIOD_MS);
            
            //}else{
				
				
				
              //  vTaskDelay(500 / portTICK_PERIOD_MS);
            //}
        }    

        if (sock != -1) {
            ESP_LOGE(TAG, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);
        }
    } else {
		
		
        bool broke =  false;
		while (current_ind_state != LEADER) {
			char payload[20];
                //sprintf(payload, "%d %c", voterID, currentVote);	
			if(voter_queue == NULL){
				vTaskDelay(20/portTICK_PERIOD_MS);
				break;
			}

			while(!(xQueueReceive(voter_queue, &(payload), (TickType_t)5))){
				vTaskDelay(10/portTICK_PERIOD_MS);
                if(current_ind_state == LEADER){
                    broke = true;
                    break;
                }
			}
            if(broke){
                break;
            }
			
			sprintf(host_ip, "192.168.1.%d", leader_id);
		
#if defined(CONFIG_EXAMPLE_IPV4)
        struct sockaddr_in dest_addr;
        dest_addr.sin_addr.s_addr = inet_addr(host_ip);
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(ESP_SERVER_PORT);
        addr_family = AF_INET;
        ip_protocol = IPPROTO_IP;
#elif defined(CONFIG_EXAMPLE_IPV6)
        struct sockaddr_in6 dest_addr = { 0 };
        inet6_aton(host_ip, &dest_addr.sin6_addr);
        dest_addr.sin6_family = AF_INET6;
        dest_addr.sin6_port = htons(ESP_SERVER_PORT);
        dest_addr.sin6_scope_id = esp_netif_get_netif_impl_index(EXAMPLE_INTERFACE);
        addr_family = AF_INET6;
        ip_protocol = IPPROTO_IPV6;
#elif defined(CONFIG_EXAMPLE_SOCKET_IP_INPUT_STDIN)
        struct sockaddr_storage dest_addr = { 0 };
        ESP_ERROR_CHECK(get_addr_from_stdin(PORT, SOCK_DGRAM, &ip_protocol, &addr_family, &dest_addr));
#endif


        int sock = socket(addr_family, SOCK_DGRAM, ip_protocol);
        if (sock < 0) {
            ESP_LOGE(FOLLOWER_TAG, "Unable to create socket: errno %d", errno);
            break;
        }

        // Set timeout
        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout);

        ESP_LOGI(FOLLOWER_TAG, "Socket created, sending to %s:%d", host_ip, ESP_SERVER_PORT);
		
                
                

                int err = sendto(sock, payload, strlen(payload), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
                if (err < 0) {
                    ESP_LOGE(FOLLOWER_TAG, "Error occurred during sending: errno %d", errno);
                    break;
                }
                ESP_LOGI(FOLLOWER_TAG, "Message sent to Leader %s", payload);

                struct sockaddr_storage source_addr; // Large enough for both IPv4 or IPv6
                socklen_t socklen = sizeof(source_addr);
                int len = recvfrom(sock, rx_buffer, sizeof(rx_buffer) - 1, 0, (struct sockaddr *)&source_addr, &socklen);

                // Error occurred during receiving
                if (len < 0) {
                    ESP_LOGE(FOLLOWER_TAG, "recvfrom failed: errno %d", errno);
                    break;
                }
                // Data received
                else {
                    rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string
                    ESP_LOGI(FOLLOWER_TAG, "Received %d bytes from %s:", len, host_ip);
                    ESP_LOGI(FOLLOWER_TAG, "%s", rx_buffer);
					
					char* token = strtok(rx_buffer, ",");
					char confirmation = 'o';
					if (token != NULL) {
						confirmation = atoi(token);
						token = strtok(NULL, ",");
					}
                    if(confirmation == 'c'){
                        confirm_led = 1;
                    }
					
					
                    // Parse cat rank from two numbers seperated by a space
                    
                    //cat_rank = atoi(rx_buffer);
                }
               vTaskDelay(100 / portTICK_PERIOD_MS);
            
		
		
        }
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    }
    vTaskDelete(NULL);
}

// Client Task so that Leader can send confirmation to the voter
void udp_confirm_client_task(void *pvParameters)
{
    char rx_buffer[128];
    char host_ip[14];
    char confim_payload[10];
    confirm_queue = xQueueCreate(100, 100); 

    int addr_family = 0;
    int ip_protocol = 0;
    uint16_t port = 5555;
    int voter_id_number = 0;


while(1){
    while (1) {
        if(confirm_queue == NULL){
            vTaskDelay(20/portTICK_PERIOD_MS);
            break;
        }

        while(!(xQueueReceive(confirm_queue, &(confim_payload), (TickType_t)5))){
            vTaskDelay(20/portTICK_PERIOD_MS);
        }
        printf("Confirm payload from queue: %s",confim_payload);
        char* token = strtok(confim_payload, ",");
        if (token != NULL) {
            token = strtok(NULL, ",");
            voter_id_number = atoi(token);
            token = strtok(NULL, " ");
        }

		sprintf(host_ip, "192.168.1.%d",voter_id_number);

        #if defined(CONFIG_EXAMPLE_IPV4)
        struct sockaddr_in dest_addr;
        dest_addr.sin_addr.s_addr = inet_addr(host_ip);
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(ESP_SERVER_PORT);
        addr_family = AF_INET;
        ip_protocol = IPPROTO_IP;
#elif defined(CONFIG_EXAMPLE_IPV6)
        struct sockaddr_in6 dest_addr = { 0 };
        inet6_aton(host_ip, &dest_addr.sin6_addr);
        dest_addr.sin6_family = AF_INET6;
        dest_addr.sin6_port = htons(ESP_SERVER_PORT);
        dest_addr.sin6_scope_id = esp_netif_get_netif_impl_index(EXAMPLE_INTERFACE);
        addr_family = AF_INET6;
        ip_protocol = IPPROTO_IPV6;
#elif defined(CONFIG_EXAMPLE_SOCKET_IP_INPUT_STDIN)
        struct sockaddr_storage dest_addr = { 0 };
        ESP_ERROR_CHECK(get_addr_from_stdin(PORT, SOCK_DGRAM, &ip_protocol, &addr_family, &dest_addr));
#endif


        int sock = socket(addr_family, SOCK_DGRAM, ip_protocol);
        if (sock < 0) {
            ESP_LOGE(CONFIRM_TAG, "Unable to create socket: errno %d", errno);
            break;
        }

        // Set timeout
        struct timeval timeout;
        timeout.tv_sec = 2;
        timeout.tv_usec = 0;
        setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout);

        ESP_LOGI(CONFIRM_TAG, "Socket created, sending to %s:%d", host_ip, ESP_SERVER_PORT);              

                int err = sendto(sock, confim_payload, strlen(confim_payload), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
                if (err < 0) {
                    ESP_LOGE(CONFIRM_TAG, "Error occurred during sending: errno %d", errno);
                    break;
                }
                ESP_LOGI(CONFIRM_TAG, "Confirmation sent to Voter %s", confim_payload);

                struct sockaddr_storage source_addr; // Large enough for both IPv4 or IPv6
                socklen_t socklen = sizeof(source_addr);
                int len = recvfrom(sock, rx_buffer, sizeof(rx_buffer) - 1, 0, (struct sockaddr *)&source_addr, &socklen);

                // Error occurred during receiving
                if (len < 0) {
                    ESP_LOGE(CONFIRM_TAG, "recvfrom failed: errno %d", errno);
                    break;
                }
                // Data received
                else {
                    rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string
                    ESP_LOGI(CONFIRM_TAG, "Received %d bytes from %s:", len, host_ip);
                    ESP_LOGI(CONFIRM_TAG, "%s", rx_buffer);
                }

             vTaskDelay(100 / portTICK_PERIOD_MS);
        if (sock != -1) {
            ESP_LOGE(CONFIRM_TAG, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);
        }
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
}
    vTaskDelete(NULL);
}