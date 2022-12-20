#include "driver/gpio.h"
#include "esp_rom_gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"

#define LED 2

void app_main(void){

    esp_rom_gpio_pad_select_gpio(LED);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);
    
    while(1){
        gpio_set_level(LED, 1);
        vTaskDelay(500/portTICK_PERIOD_MS);
        gpio_set_level(LED, 0);
        vTaskDelay(500/portTICK_PERIOD_MS);
    }

}