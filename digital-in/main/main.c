#include "driver/gpio.h"
#include "esp_rom_gpio.h"
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

#define BOTAO 21
#define LED 2

bool state = 0;
bool led = 0;

void app_main(void)
{
    esp_rom_gpio_pad_select_gpio(BOTAO);
    gpio_set_direction(BOTAO, GPIO_MODE_INPUT);
    gpio_pullup_dis(BOTAO);
    gpio_pulldown_en(BOTAO);

    esp_rom_gpio_pad_select_gpio(LED);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);

    while(true){
        if(!gpio_get_level(BOTAO))
            state = 1;
    
        if(gpio_get_level(BOTAO) && state)
        {
            state = 0;
            led = !led;
            gpio_set_level(LED, led);
        }
    }
}