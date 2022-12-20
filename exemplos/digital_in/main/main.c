#include "driver/gpio.h"
#include "esp_rom_gpio.h"
#include "sdkconfig.h"

#define BOTAO 15
#define LED 2

void app_main(void)
{
    esp_rom_gpio_pad_select_gpio(BOTAO);
    gpio_set_direction(BOTAO, GPIO_MODE_DEF_INPUT);
    gpio_pullup_dis(BOTAO);
    gpio_pulldown_en(BOTAO);

    esp_rom_gpio_pad_select_gpio(LED);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);

    while (true)
    {
        if (gpio_get_level(BOTAO)){
            gpio_set_level(LED, 1);
        }
        else{
            gpio_set_level(LED, 0);
        }
    }
}