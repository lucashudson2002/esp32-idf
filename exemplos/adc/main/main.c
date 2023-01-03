#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "sdkconfig.h"

#define POT_CHANNEL ADC1_CHANNEL_0
#define POT_PIN 36

void app_main(void){

    esp_rom_gpio_pad_select_gpio(POT_PIN);
    gpio_set_direction(POT_PIN, GPIO_MODE_INPUT);
    gpio_pulldown_en(POT_PIN);
    gpio_pullup_dis(POT_PIN);

    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(POT_CHANNEL, ADC_ATTEN_DB_11); //150mV a 2450mV

    while(1){
       printf("Potenciometer: %d\n", adc1_get_raw(POT_CHANNEL));
       vTaskDelay(200/portTICK_PERIOD_MS);
    }
}