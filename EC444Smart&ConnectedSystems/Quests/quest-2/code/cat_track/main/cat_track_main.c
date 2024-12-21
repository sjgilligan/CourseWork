/*
Aymeric Blaizot, Amado Diallo, Sebastian Gilligan, Eve Cruanes 10/4/2024 EC444
  Adapted I2C example code to work with the Adafruit ADXL343 accelerometer. Ported and referenced a lot of code from the Adafruit_ADXL343 driver code.
  ----> https://www.adafruit.com/product/4097

  Emily Lam, Aug 2019 for BU EC444
*/
#include <stdio.h>
#include <math.h>
#include "driver/i2c.h"
//#include "./ADXL343.h"
#include "display.h"
#include "accelerometer.h"
#include "button.h"
#include "temp.h"


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
#define NACK_VAL                           0x01 // i2c nack value (Was FF)

// ADXL343
#define ADX_ADDR                         ADXL343_ADDRESS // 0x53

// 14-Segment Display
#define ALPHA_ADDR                         0x70 // alphanumeric address
#define OSC                                0x21 // oscillator cmd
#define HT16K33_BLINK_DISPLAYON            0x01 // Display on cmd
#define HT16K33_BLINK_OFF                  0    // Blink off cmd
#define HT16K33_BLINK_CMD                  0x80 // Blink cmd
#define HT16K33_CMD_BRIGHTNESS             0xE0 // Brightness cmd

extern uint16_t glo_user_buf[16];
extern uint16_t glo_display_buf[4];
extern int glo_display_count;

///////////////////////////////////////////////////////////////

// Function to initiate i2c -- note the MSB declaration!
static void i2c_master_init(){
  // Debug
  //printf("\n>> i2c Config\n");
  int err;

  // Port configuration
  int i2c_master_port = I2C_EXAMPLE_MASTER_NUM;

  /// Define I2C configurations
  i2c_config_t conf;
  conf.mode = I2C_MODE_MASTER;                              // Master mode
  conf.sda_io_num = I2C_EXAMPLE_MASTER_SDA_IO;              // Default SDA pin
  conf.sda_pullup_en = GPIO_PULLUP_ENABLE;                  // Internal pullup
  conf.scl_io_num = I2C_EXAMPLE_MASTER_SCL_IO;              // Default SCL pin
  conf.scl_pullup_en = GPIO_PULLUP_ENABLE;                  // Internal pullup
  conf.master.clk_speed = I2C_EXAMPLE_MASTER_FREQ_HZ;       // CLK frequency
  conf.clk_flags = 0; 
  err = i2c_param_config(i2c_master_port, &conf);           // Configure
  if (err == ESP_OK) {
    //printf("- parameters: ok\n");
    }

  // Install I2C driver
  err = i2c_driver_install(i2c_master_port, conf.mode,
                     I2C_EXAMPLE_MASTER_RX_BUF_DISABLE,
                     I2C_EXAMPLE_MASTER_TX_BUF_DISABLE, 0);
  if (err == ESP_OK) {
    //printf("- initialized: yes\n");
    }

  // Data in MSB mode
  i2c_set_data_mode(i2c_master_port, I2C_DATA_MODE_MSB_FIRST, I2C_DATA_MODE_MSB_FIRST);
}

// Utility  Functions //////////////////////////////////////////////////////////

// Utility function to test for I2C device address -- not used in deploy
int testConnection(uint8_t devAddr, int32_t timeout) {
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, (devAddr << 1) | I2C_MASTER_WRITE, ACK_CHECK_EN);
  i2c_master_stop(cmd);
  int err = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
  i2c_cmd_link_delete(cmd);
  return err;
}

// Utility function to scan for i2c device
static void i2c_scanner() {
  int32_t scanTimeout = 1000;
  //printf("\n>> I2C scanning ..."  "\n");
  uint8_t count = 0;
  for (uint8_t i = 1; i < 127; i++) {
    // printf("0x%X%s",i,"\n");
    if (testConnection(i, scanTimeout) == ESP_OK) {
      //printf( "- Device found at address: 0x%X%s", i, "\n");
      count++;
    }
  }
 // if (count == 0) {printf("- No I2C devices found!" "\n");}
}

extern float temperature;
extern float activity_time;
extern activity_t activity;

static void log_data()
{
    while(1){
        char activity_char[20] = "inactive";
        
        switch(activity)
        {
            case HIGHLY_ACTIVE:
                strcpy(activity_char, "highly_active");
                break;
            case ACTIVE:
                strcpy(activity_char, "active");
                break;
            case INACTIVE:
                strcpy(activity_char, "inactive");
                break;
            default:
                strcpy(activity_char, "inactive");
                break;
        }
        // TIME, TEMP, ACTIVITY
        printf("%f %f %s\n", activity_time, temperature, activity_char);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void app_main() {

  // Routine
  i2c_master_init();
  i2c_scanner();
  timer_init();
  button_init();
  temp_init();


  // Check for ADXL343
  uint8_t deviceID;
  getDeviceID(&deviceID);
  if (deviceID == 0xE5) {
    //printf("\n>> Found ADAXL343\n");
  }

  // Disable interrupts
  writeRegister(ADXL343_REG_INT_ENABLE, 0);

  // Set range
  setRange(ADXL343_RANGE_16_G);

  // Enable measurements
  writeRegister(ADXL343_REG_POWER_CTL, 0x08);
  
  glo_display_buf[0] = getBinaryValue('0');
	glo_display_buf[1] = getBinaryValue('0');
	glo_display_buf[2] = getBinaryValue('0');
	glo_display_buf[3] = getBinaryValue('0');

  // Create task to poll ADXL343
  xTaskCreate(test_adxl343,"test_adxl343", 4096, NULL,  configMAX_PRIORITIES-1, NULL);
  xTaskCreate(test_alpha_display,"test_alpha_display", 4096, NULL, configMAX_PRIORITIES-2, NULL);
  xTaskCreate(cycle_display,"cycle_display", 4096, NULL, configMAX_PRIORITIES-3, NULL);	
  xTaskCreate(get_activity,"get_activity", 4096, configMAX_PRIORITIES-4, 5, NULL);
  xTaskCreate(time_activity,"time_activity", 4096, configMAX_PRIORITIES-5, 5, NULL);
  xTaskCreate(button_task,"button_task", 4096, configMAX_PRIORITIES-6, 5, NULL);
  xTaskCreate(temp,"temp", 4096, configMAX_PRIORITIES-7, 5, NULL);
  xTaskCreate(log_data,"log_data", 4096, configMAX_PRIORITIES-8, 5, NULL);
}