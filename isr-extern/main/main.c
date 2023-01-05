#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#define BOTAO 21
#define LED 2

bool led = 0;

static void IRAM_ATTR blink (void *args){
    led = !led;
    gpio_set_level(LED, led);
}

void app_main(void){
    esp_rom_gpio_pad_select_gpio(LED);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);

    esp_rom_gpio_pad_select_gpio(BOTAO);
    gpio_set_direction(BOTAO, GPIO_MODE_INPUT);
    gpio_pulldown_en(BOTAO);
    gpio_pullup_dis(BOTAO);

    gpio_set_intr_type(BOTAO, GPIO_INTR_POSEDGE);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(BOTAO, blink, NULL);
}