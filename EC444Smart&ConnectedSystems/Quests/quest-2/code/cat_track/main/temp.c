#include "temp.h"

#define DEFAULT_VREF    3300        //Use adc2_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES   20          //Multisampling

#define THERMISTORNOMINAL 10000      
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25   
// how many samples to take and average, more takes longer
// but is more 'smooth'
#define NUMSAMPLES 5
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3435

static esp_adc_cal_characteristics_t *adc_chars_temp;

static const adc_channel_t temp_channel = ADC_CHANNEL_0;     	 // GPIO36 Temperature - A4

static const adc_atten_t atten = ADC_ATTEN_DB_11;
static const adc_unit_t unit = ADC_UNIT_1;

float temperature = 0;
// Function to read temperature

void temp_init() {
    // Set the GPIO as a push/pull output
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(temp_channel, atten);

    //Characterize ADC
    adc_chars_temp = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars_temp);
}

void log_temp()
{
    while(1){
        printf("Temp: %3.1f \n", temperature);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void temp() {

    while (1) {

         uint32_t adc_reading = 0;
        //Multisampling
        for (int i = 0; i < NO_OF_SAMPLES; i++) {
            if (unit == ADC_UNIT_1) {
                adc_reading += adc1_get_raw((adc1_channel_t)temp_channel);
            } else {
                int raw;
                adc2_get_raw((adc2_channel_t)temp_channel, ADC_WIDTH_BIT_12, &raw);
                adc_reading += raw;
            }
            vTaskDelay(pdMS_TO_TICKS(100));
        }

        adc_reading /= NO_OF_SAMPLES;
        uint32_t voltage = adc_reading / 4095.0 * 3300;
        float resistance = 10000.0 / (4095.0 / adc_reading -1);

        // Temperature calculation
        float steinhart;
        steinhart = resistance / THERMISTORNOMINAL;     // (R/Ro)
        steinhart = log(steinhart);                  // ln(R/Ro)
        steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
        steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
        steinhart = 1.0 / steinhart;                 // Invert
        steinhart -= 273.15;                         // convert absolute temp to C
        //Convert adc_reading to voltage in mV
        temperature = steinhart;
    }
}