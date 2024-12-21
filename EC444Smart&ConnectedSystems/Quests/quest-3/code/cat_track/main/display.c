//Aymeric Blaizot, Amado Diallo, Sebastian Gilligan, Eve Cruanes 10/25/2024 EC444

#include "display.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "driver/i2c.h"
#include "driver/uart.h"
#include "esp_vfs_dev.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"
#include "esp_sleep.h"

// 14-Segment Display
#define SLAVE_ADDR                         0x70 // alphanumeric address
#define OSC                                0x21 // oscillator cmd
#define HT16K33_BLINK_DISPLAYON            0x01 // Display on cmd
#define HT16K33_BLINK_OFF                  0    // Blink off cmd
#define HT16K33_BLINK_CMD                  0x80 // Blink cmd
#define HT16K33_CMD_BRIGHTNESS             0xE0 // Brightness cmd

// Master I2C
#define I2C_EXAMPLE_MASTER_SCL_IO          22   // gpio number for i2c clk
#define I2C_EXAMPLE_MASTER_SDA_IO          23   // gpio number for i2c data
#define I2C_EXAMPLE_MASTER_NUM             I2C_NUM_0  // i2c port
#define I2C_EXAMPLE_MASTER_TX_BUF_DISABLE  0    // i2c master no buffer needed
#define I2C_EXAMPLE_MASTER_RX_BUF_DISABLE  0    // i2c master no buffer needed
#define I2C_EXAMPLE_MASTER_FREQ_HZ         100000     // i2c master clock freq
#define WRITE_BIT                          I2C_MASTER_WRITE // i2c master write
#define READ_BIT                           I2C_MASTER_READ  // i2c master read
#define ACK_CHECK_EN                       true // i2c master will check ack
#define ACK_CHECK_DIS                      false// i2c master will not check ack
#define ACK_VAL                            0x00 // i2c ack value
#define NACK_VAL                           0xFF // i2c nack value

uint16_t glo_user_buf[100];
uint16_t glo_display_buf[4];
int glo_display_count;

extern int scroll;

// Alphanumeric Functions //////////////////////////////////////////////////////

// Turn on oscillator for alpha display
int alpha_oscillator() {
  int ret;
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, ( SLAVE_ADDR << 1 ) | WRITE_BIT, ACK_CHECK_EN);
  i2c_master_write_byte(cmd, OSC, ACK_CHECK_EN);
  i2c_master_stop(cmd);
  ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
  i2c_cmd_link_delete(cmd);
  vTaskDelay(200 / portTICK_PERIOD_MS);
  return ret;
}

// Set blink rate to off
int no_blink() {
  int ret;
  i2c_cmd_handle_t cmd2 = i2c_cmd_link_create();
  i2c_master_start(cmd2);
  i2c_master_write_byte(cmd2, ( SLAVE_ADDR << 1 ) | WRITE_BIT, ACK_CHECK_EN);
  i2c_master_write_byte(cmd2, HT16K33_BLINK_CMD | HT16K33_BLINK_DISPLAYON | (HT16K33_BLINK_OFF << 1), ACK_CHECK_EN);
  i2c_master_stop(cmd2);
  ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd2, 1000 / portTICK_PERIOD_MS);
  i2c_cmd_link_delete(cmd2);
  vTaskDelay(200 / portTICK_PERIOD_MS);
  return ret;
}

// Set Brightness
int set_brightness_max(uint8_t val) {
  int ret;
  i2c_cmd_handle_t cmd3 = i2c_cmd_link_create();
  i2c_master_start(cmd3);
  i2c_master_write_byte(cmd3, ( SLAVE_ADDR << 1 ) | WRITE_BIT, ACK_CHECK_EN);
  i2c_master_write_byte(cmd3, HT16K33_CMD_BRIGHTNESS | val, ACK_CHECK_EN);
  i2c_master_stop(cmd3);
  ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd3, 1000 / portTICK_PERIOD_MS);
  i2c_cmd_link_delete(cmd3);
  vTaskDelay(200 / portTICK_PERIOD_MS);
  return ret;
}

////////////////////////////////////////////////////////////////////////////////

unsigned short getBinaryValue(char ch) { //USED CHATGPT TO CREATE THIS FUNCTION
    switch (ch) {
        case '!': return 0b0000000000000110;
        case '"': return 0b0000001000100000;
        case '#': return 0b0001001011001110;
        case '$': return 0b0001001011101101;
        case '%': return 0b0000110000100100;
        case '&': return 0b0010001101011101;
        case '\'': return 0b0000010000000000;
        case '(': return 0b0010010000000000;
        case ')': return 0b0000100100000000;
        case '*': return 0b0011111111000000;
        case '+': return 0b0001001011000000;
        case ',': return 0b0000100000000000;
        case '-': return 0b0000000011000000;
        case '.': return 0b0100000000000000;
        case '/': return 0b0000110000000000;
        case '0': return 0b0000110000111111;
        case '1': return 0b0000000000000110;
        case '2': return 0b0000000011011011;
        case '3': return 0b0000000010001111;
        case '4': return 0b0000000011100110;
        case '5': return 0b0010000001101001;
        case '6': return 0b0000000011111101;
        case '7': return 0b0000000000000111;
        case '8': return 0b0000000011111111;
        case '9': return 0b0000000011101111;
        case ':': return 0b0001001000000000;
        case ';': return 0b0000101000000000;
        case '<': return 0b0010010000000000;
        case '=': return 0b0000000011001000;
        case '>': return 0b0000100100000000;
        case '?': return 0b0001000010000011;
        case '@': return 0b0000001010111011;
        case 'A': return 0b0000000011110111;
        case 'B': return 0b0001001010001111;
        case 'C': return 0b0000000000111001;
        case 'D': return 0b0001001000001111;
        case 'E': return 0b0000000011111001;
        case 'F': return 0b0000000001110001;
        case 'G': return 0b0000000010111101;
        case 'H': return 0b0000000011110110;
        case 'I': return 0b0001001000001001;
        case 'J': return 0b0000000000011110;
        case 'K': return 0b0010010001110000;
        case 'L': return 0b0000000000111000;
        case 'M': return 0b0000010100110110;
        case 'N': return 0b0010000100110110;
        case 'O': return 0b0000000000111111;
        case 'P': return 0b0000000011110011;
        case 'Q': return 0b0010000000111111;
        case 'R': return 0b0010000011110011;
        case 'S': return 0b0000000011101101;
        case 'T': return 0b0001001000000001;
        case 'U': return 0b0000000000111110;
        case 'V': return 0b0000110000110000;
        case 'W': return 0b0010100000110110;
        case 'X': return 0b0010110100000000;
        case 'Y': return 0b0001010100000000;
        case 'Z': return 0b0000110000001001;
        case '[': return 0b0000000000111001;
        case '\\': return 0b0010000100000000;
        case ']': return 0b0000000000001111;
        case '^': return 0b0000110000000011;
        case '_': return 0b0000000000001000;
        case '`': return 0b0000000100000000;
        case 'a': return 0b0001000001011000;
        case 'b': return 0b0010000001111000;
        case 'c': return 0b0000000011011000;
        case 'd': return 0b0000100010001110;
        case 'e': return 0b0000100001011000;
        case 'f': return 0b0000000001110001;
        case 'g': return 0b0000010010001110;
        case 'h': return 0b0001000001110000;
        case 'i': return 0b0001000000000000;
        case 'j': return 0b0000000000001110;
        case 'k': return 0b0011011000000000;
        case 'l': return 0b0000000000110000;
        case 'm': return 0b0001000011010100;
        case 'n': return 0b0001000001010000;
        case 'o': return 0b0000000011011100;
        case 'p': return 0b0000000101110000;
        case 'q': return 0b0000010010000110;
        case 'r': return 0b0000000001010000;
        case 's': return 0b0010000010001000;
        case 't': return 0b0000000001111000;
        case 'u': return 0b0000000000011100;
        case 'v': return 0b0010000000000100;
        case 'w': return 0b0010100000010100;
        case 'x': return 0b0010100011000000;
        case 'y': return 0b0010000000001100;
        case 'z': return 0b0000100001001000;
        case '{': return 0b0000100101001001;
        case '|': return 0b0001001000000000;
        case '}': return 0b0010010010001001;
        case '~': return 0b0000010100100000;
        default: return 0; // Return 0 if no match is found
    }
}

void test_alpha_display(void *blank) {
    // Debug
    int ret;
    printf(">> Test Alphanumeric Display: \n");

    // Set up routines
    // Turn on alpha oscillator
    ret = alpha_oscillator();
    if(ret == ESP_OK) {printf("- oscillator: ok \n");}
    // Set display blink off
    ret = no_blink();
    if(ret == ESP_OK) {printf("- blink: off \n");}
    ret = set_brightness_max(0xF);
    if(ret == ESP_OK) {printf("- brightness: max \n");}

    // Write to characters to buffer
    uint16_t displaybuffer[8];
    displaybuffer[0] = glo_display_buf[0];
    displaybuffer[1] = glo_display_buf[1];
    displaybuffer[2] = glo_display_buf[2];
    displaybuffer[3] = glo_display_buf[3];

    // Continually writes the same command
    while (1) {

      // Send commands characters to display over I2C
      i2c_cmd_handle_t cmd4 = i2c_cmd_link_create();
      i2c_master_start(cmd4);
      i2c_master_write_byte(cmd4, ( SLAVE_ADDR << 1 ) | WRITE_BIT, ACK_CHECK_EN);
      i2c_master_write_byte(cmd4, (uint8_t)0x00, ACK_CHECK_EN);
      for (uint8_t i=0; i<8; i++) {
        i2c_master_write_byte(cmd4, displaybuffer[i] & 0xFF, ACK_CHECK_EN);
        i2c_master_write_byte(cmd4, displaybuffer[i] >> 8, ACK_CHECK_EN);
      }
      i2c_master_stop(cmd4);
      ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd4, 1000 / portTICK_PERIOD_MS);
      i2c_cmd_link_delete(cmd4);

      // for (int i = 0; i < 8; i++) {
      //     printf("%04x\n", displaybuffer[i]);
      // }

		if(ret == ESP_OK) {
		//printf("- wrote: T.D.C.L. \n\n");
		}
		displaybuffer[0] = glo_display_buf[0];
		displaybuffer[1] = glo_display_buf[1];
		displaybuffer[2] = glo_display_buf[2];
		displaybuffer[3] = glo_display_buf[3];
		

    }


}

void cycle_display(void *blank){
	int i = 0;
	while(1){ //loop forever
  scroll = 1; //Forcing display
		if(scroll && (glo_display_count > 0)){ //if told to scroll and display > 0
			printf("%d\n",glo_display_count);
		
			if(i == glo_display_count){ //once end is reached
				i = -1; //reset i
				scroll = 0; //turn off scroll
				memset(glo_user_buf, 0, sizeof(glo_user_buf)); //clear buffer
			}	
			
			glo_display_buf[0] = glo_user_buf[i];	//assign user buf to 4 seg buffer
			glo_display_buf[1] = glo_user_buf[i + 1];
			glo_display_buf[2] = glo_user_buf[i + 2];
			glo_display_buf[3] = glo_user_buf[i + 3];
		
			i++;
		}
		vTaskDelay(300 / portTICK_PERIOD_MS);
	}
}
