#include <stdio.h>
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED 2

void blink(void *param)
{
  static bool on;
  on = !on;
  
  gpio_set_level(LED, on);
}

void app_main(void)
{
  esp_rom_gpio_pad_select_gpio(LED);
  gpio_set_direction(LED, GPIO_MODE_OUTPUT);

  const esp_timer_create_args_t my_timer_args = {
      .callback = &blink,
      .name = "blink"};
  esp_timer_handle_t timer_handler;
  ESP_ERROR_CHECK(esp_timer_create(&my_timer_args, &timer_handler));
  ESP_ERROR_CHECK(esp_timer_start_periodic(timer_handler, 1000000));
}
