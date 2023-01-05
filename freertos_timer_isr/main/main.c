#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "esp_timer.h"
#include "driver/gpio.h"

#define LED 2

void blink(TimerHandle_t xTimer)
{
    static bool on;
    on = !on;

    gpio_set_level(LED, on);
}

void app_main(void)
{
    esp_rom_gpio_pad_select_gpio(LED);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);

    TimerHandle_t xTimer = xTimerCreate("blink", pdMS_TO_TICKS(1000),true,NULL, blink);
    xTimerStart(xTimer,0);
}
