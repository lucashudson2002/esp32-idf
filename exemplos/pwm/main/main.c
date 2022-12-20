#include "driver/ledc.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LEDC_TIMER LEDC_TIMER_0
#define LEDC_MODE LEDC_LOW_SPEED_MODE
#define LED 2
#define LEDC_CHANNEL LEDC_CHANNEL_0
#define LEDC_DUTY_RES LEDC_TIMER_8_BIT
#define LEDC_FREQUENCY 5000 

static void ledc_init(){
    ledc_timer_config_t ledc_timer={
        .speed_mode = LEDC_MODE,
        .timer_num = LEDC_TIMER,
        .duty_resolution = LEDC_DUTY_RES,
        .freq_hz = LEDC_FREQUENCY,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));
    ledc_channel_config_t ledc_channel={
        .speed_mode = LEDC_MODE,
        .channel = LEDC_CHANNEL,
        .timer_sel = LEDC_TIMER,
        .gpio_num = LED,
        .duty = 0,
        .hpoint = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}

void app_main(void){

    esp_rom_gpio_pad_select_gpio(LED);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);

    ledc_init();

    while(1){
        for (int i=0; i<255; i++){
            ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, i);
            ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
            vTaskDelay(10/portTICK_PERIOD_MS);
        }
        for (int i=255; i>0; i--){
            ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, i);
            ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
            vTaskDelay(10/portTICK_PERIOD_MS);
        }
    }

}
