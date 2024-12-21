//Aymeric Blaizot, Amado Diallo, Sebastian Gilligan, Eve Cruanes 10/25/2024 EC444

#include "accelerometer.h"

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


float activity_time = 0;
float inactive_time = 0;
float active_time = 0;
float highly_active_time = 0;

float inactive_time_d = 0;
float active_time_d = 0;
float highly_active_time_d = 0;


cat_activity_t cat;
// ADXL343 Functions ///////////////////////////////////////////////////////////

// Get Device ID
int getDeviceID(uint8_t *data) {
  int ret;
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, ( ADX_ADDR << 1 ) | WRITE_BIT, ACK_CHECK_EN);
  i2c_master_write_byte(cmd, ADXL343_REG_DEVID, ACK_CHECK_EN);
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, ( ADX_ADDR << 1 ) | READ_BIT, ACK_CHECK_EN);
  i2c_master_read_byte(cmd, data, ACK_CHECK_DIS);
  i2c_master_stop(cmd);
  ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
  i2c_cmd_link_delete(cmd);
  return ret;
}

// Write one byte to register
int writeRegister(uint8_t reg, uint8_t data) {
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, ( ADX_ADDR << 1 ) | WRITE_BIT, ACK_CHECK_EN);
  i2c_master_write_byte(cmd, reg, ACK_CHECK_EN);
  i2c_master_write_byte(cmd, data, ACK_CHECK_EN);
  i2c_master_stop(cmd);
  i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
  i2c_cmd_link_delete(cmd);
  vTaskDelay(20 / portTICK_PERIOD_MS);
  return 1;
  // YOUR CODE HERE
}

// Read register
uint8_t readRegister(uint8_t reg) {
  uint8_t data;
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, ( ADX_ADDR << 1 ) | WRITE_BIT, ACK_CHECK_EN);
  i2c_master_write_byte(cmd, reg, ACK_CHECK_EN);
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, ( ADX_ADDR << 1 ) | READ_BIT, ACK_CHECK_EN);
  i2c_master_read_byte(cmd, &data, ACK_CHECK_DIS);
  i2c_master_stop(cmd);
  i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
  i2c_cmd_link_delete(cmd);
  vTaskDelay(20 / portTICK_PERIOD_MS);

  return data;
  // YOUR CODE HERE
}

// read 16 bits (2 bytes)
int16_t read16(uint8_t reg) {
  uint8_t data1;
  uint8_t data2;
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, ( ADX_ADDR << 1 ) | WRITE_BIT, ACK_CHECK_EN);
  i2c_master_write_byte(cmd, reg, ACK_CHECK_EN);
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, ( ADX_ADDR << 1 ) | READ_BIT, ACK_CHECK_EN);
  i2c_master_read_byte(cmd, &data1, ACK_CHECK_DIS);
  i2c_master_read_byte(cmd, &data2, ACK_CHECK_DIS);
  i2c_master_stop(cmd);
  i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
  i2c_cmd_link_delete(cmd);
  vTaskDelay(20 / portTICK_PERIOD_MS);

  return (int16_t)(data2 << 8 | data1); // *AI Generated: Github copilot* 
  // YOUR CODE HERE
}

void setRange(range_t range) {
  /* Red the data format register to preserve bits */
  uint8_t format = readRegister(ADXL343_REG_DATA_FORMAT);

  /* Update the data rate */
  format &= ~0x0F;
  format |= range;

  /* Make sure that the FULL-RES bit is enabled for range scaling */
  format |= 0x08;

  /* Write the register back to the IC */
  writeRegister(ADXL343_REG_DATA_FORMAT, format);

}

range_t getRange(void) {
  /* Red the data format register to preserve bits */
  return (range_t)(readRegister(ADXL343_REG_DATA_FORMAT) & 0x03);
}

dataRate_t getDataRate(void) {
  return (dataRate_t)(readRegister(ADXL343_REG_BW_RATE) & 0x0F);
}

////////////////////////////////////////////////////////////////////////////////

// function to get acceleration
void getAccel(float * xp, float *yp, float *zp) {
  *xp = read16(ADXL343_REG_DATAX0) * ADXL343_MG2G_MULTIPLIER * SENSORS_GRAVITY_STANDARD;
  *yp = read16(ADXL343_REG_DATAY0) * ADXL343_MG2G_MULTIPLIER * SENSORS_GRAVITY_STANDARD;
  *zp = read16(ADXL343_REG_DATAZ0) * ADXL343_MG2G_MULTIPLIER * SENSORS_GRAVITY_STANDARD;
// printf("X: %.2f \nY: %.2f \nZ: %.2f\n", *xp, *yp, *zp);
}

// function to print roll and pitch
void calcRP(float x, float y, float z){
  // YOUR CODE HERE

  // *AI Generated: Github copilot* 
  float roll = atan2(y, z) * 180 / M_PI;
  float pitch = atan2((- x), sqrt(y * y + z * z)) * 180 / M_PI;

  

  //printf("roll: %.2f \t pitch: %.2f \n", roll, pitch);
}
float xVal, yVal, zVal = 0;

gptimer_handle_t gptimer = NULL;
void timer_init() {

    gptimer_config_t timer_config = {
      .clk_src = GPTIMER_CLK_SRC_DEFAULT,
      .direction = GPTIMER_COUNT_UP,
      .resolution_hz = 1000000, // 1MHz, 1 tick=1us
    };
    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &gptimer)); // instantiates timer
    ESP_ERROR_CHECK(gptimer_enable(gptimer));
    ESP_ERROR_CHECK(gptimer_start(gptimer));                            // this starts the timer
}
activity_t activity;
void get_activity(){
  while (1){
    vTaskDelay(100 / portTICK_PERIOD_MS);

    // *AI Generated: Github copilot*
    float total = sqrt(xVal * xVal + yVal * yVal + zVal * zVal) / 9.81;
    // Rolling average of last 10 total values 
    static float totalValues[10] = {0};
    static int index = 0;
    totalValues[index] = total;
    index = (index + 1) % 10;
    float totalSum = 0;
    for (int i = 0; i < 10; i++) {
      totalSum += totalValues[i];
    }
    totalSum /= 10;
    printf("Total g: %.2f\n", totalSum);
    // Thresholds for activity levels: 1.75 g for highly active, 1.25 g for active
    if (totalSum > 1.6) {
      activity = HIGHLY_ACTIVE;
      // printf("Highly Active\n");
    } else if (totalSum > 1.15) {
      activity = ACTIVE;
      // printf("Active\n");
    } else {
      activity = INACTIVE;
      // printf("Inactive\n");
    }
  }
}
void time_activity(){
  activity_t last_activity = INACTIVE;
  while (1){
    vTaskDelay(500 / portTICK_PERIOD_MS);
    uint64_t count = 0;

    vTaskDelay(10 / portTICK_PERIOD_MS);

    switch(activity){
      case HIGHLY_ACTIVE:
        highly_active_time_d = activity_time;
        break;
      case ACTIVE:
        active_time_d = activity_time;
        break;
      case INACTIVE:
        inactive_time_d = activity_time;
        break;
    }
    if (activity != last_activity){
      switch (last_activity)
      {
      case HIGHLY_ACTIVE:
        highly_active_time += activity_time;
        break;
      case ACTIVE:
        active_time += activity_time;
        break;
      case INACTIVE:
        inactive_time += activity_time;
        break;
      }
      last_activity = activity;
      //ESP_ERROR_CHECK(gptimer_set_raw_count(gptimer, 0));
     // activity_time = 0; // Reset activity_time when activity changes
      highly_active_time_d =0;
      active_time_d = 0;
      inactive_time_d = 0;
    }
    printf("Activity Time: %.2f\n", activity_time); 
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}


void udpate_activity() {
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  while(1){
    uint64_t count = 0;
    ESP_ERROR_CHECK(gptimer_get_raw_count(gptimer, &count));
    activity_time = count / 1000000.0;
    // ** AI GENERATED CODE **
    int head = cat.head;
    int tail = cat.tail;

    // Update the circular buffer
    if (activity == HIGHLY_ACTIVE) {
      cat.highly_active_time[head] = activity_time;
      cat.active_time[head] = 0;
      cat.inactive_time[head] = 0;
    } else if (activity == ACTIVE) {
      cat.active_time[head] = activity_time;
      cat.highly_active_time[head] = 0;
      cat.inactive_time[head] = 0;
    } else {
      cat.active_time[head] = 0;
      cat.highly_active_time[head] = 0;
      cat.inactive_time[head] = activity_time;
    }

    // Update the total times
    cat.total_highly_active_time += cat.highly_active_time[head];
    cat.total_active_time += cat.active_time[head];
    cat.total_inactive_time += cat.inactive_time[head];

    // Move the head and tail of the circular buffer
    head = (head + 1) % BUFFER_SIZE;
    if (head == tail) {
        // Remove the oldest entry
        cat.total_highly_active_time -= cat.highly_active_time[tail];
        cat.total_active_time -= cat.active_time[tail];
        cat.total_inactive_time -= cat.inactive_time[tail];
        tail = (tail + 1) % BUFFER_SIZE;
        printf("Removing oldest entry %f %f %f\n", cat.highly_active_time[tail], cat.active_time[tail], cat.inactive_time[tail]);
    }
    cat.head = head;
    cat.tail = tail;
    ESP_ERROR_CHECK(gptimer_set_raw_count(gptimer, 0));
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}


// Task to continuously poll acceleration and calculate roll and pitch
void test_adxl343() {
  // printf("\n>> Polling ADAXL343\n");
  while (1) {
    getAccel(&xVal, &yVal, &zVal);
    calcRP(xVal, yVal, zVal);
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

