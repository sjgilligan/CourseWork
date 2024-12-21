#ifndef _accelerometer_h   /* Include guard */
#define _accelerometer_h

#include <stdio.h>
#include <math.h>
#include "driver/i2c.h"
#include "./ADXL343.h"
#include "display.h"
#include "driver/gptimer.h"     


int getDeviceID(uint8_t *data);
int writeRegister(uint8_t reg, uint8_t data);
uint8_t readRegister(uint8_t reg);
int16_t read16(uint8_t reg);
void setRange(range_t range);
range_t getRange(void) ;
dataRate_t getDataRate(void);
void getAccel(float * xp, float *yp, float *zp);
void calcRP(float x, float y, float z);
void test_adxl343();
void get_activity();
void time_activity();
void timer_init();
void udpate_activity();

#define BUFFER_SIZE 600 
// *AI GENERATED CODE*
typedef struct {
    double highly_active_time[BUFFER_SIZE];
    double active_time[BUFFER_SIZE];
    double inactive_time[BUFFER_SIZE];
    int head;
    int tail;
    double total_highly_active_time;
    double total_active_time;
    double total_inactive_time;
} cat_activity_t;

#endif