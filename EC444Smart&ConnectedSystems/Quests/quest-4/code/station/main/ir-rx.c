#include "ir-rx.h"
#include "ir.h"
#include "state.h"
#include "udp.h"

char currentVote; 
uint8_t voterID;

extern individual_states current_ind_state;
extern QueueHandle_t voter_queue;

// Variables for my ID, minVal and status plus string fragments
extern char start;              // START BYTE that UART looks for
extern int len_out;


// Receive task -- looks for Start byte then stores received values -- 2023: minor changes
void recv_task(){
  // Buffer for input data
  // Receiver expects message to be sent multiple times
  uint8_t *data_in = (uint8_t *) malloc(BUF_SIZE2);
  while (1) {
    int len_in = uart_read_bytes(UART_NUM_1, data_in, BUF_SIZE2, 100 / portTICK_PERIOD_MS);
    if (len_in > 3) {
      ESP_LOGE(TAG_UART, "Length: %d", len_in);
      int nn = 0;
      //ESP_LOGE(TAG_UART, "Length greater than 10");
      while (data_in[nn] != start) {
        nn++;
      }
      uint8_t copied[len_out];
      memcpy(copied, data_in + nn, len_out * sizeof(uint8_t));
      //printf("before checksum");
      //ESP_LOG_BUFFER_HEXDUMP(TAG_UART, copied, len_out, ESP_LOG_INFO);
      if (checkCheckSum(copied,len_out)) {
		
        printf("after checksum");
        ESP_LOG_BUFFER_HEXDUMP(TAG_UART, copied, len_out, ESP_LOG_INFO);
        //4 bytes, data[0] start data[1] color data[2] id data[3] checksum

        currentVote = data_in[1];
        voterID = (uint8_t) data_in[2];
        printf("Current Vote: %c\n", currentVote);
        printf("Voter ID: %d\n", voterID);
		    uart_flush(UART_NUM_1);
        //if(current_ind_state == LEADER){
          char payload[20];
          sprintf(payload, "%d %c", voterID, currentVote);
          if(!(xQueueSend(voter_queue, (void*)payload, (TickType_t)0)))
          {
              ESP_LOGE(TAG,"IR RX Failed to send to queue\n");
          }
          else
          {
              ESP_LOGI(TAG,"IR RX Sent to queue\n");
          }


        //}
      }
    }
    else{
      // printf("Nothing received.\n");
    }
    //vTaskDelay(5 / portTICK_PERIOD_MS);
  }
  free(data_in);
}

