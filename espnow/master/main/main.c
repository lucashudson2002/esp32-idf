#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_now.h"
#include "esp_mac.h"
#include "driver/gpio.h"

#define LED 2

#define TAG "ESP_NOW"

#define ESP6 {0x10, 0x52, 0x1C, 0x62, 0xD7, 0x9C}
#define ESP7 {0xAC, 0x67, 0xB2, 0x3B, 0xCD, 0xA8}

static const char* PMK_KEY_STR = "RINOBOT_PATO_VS3";
static const char* LMK_KEY_STR = "RINOBOT_PATO_VS3";

uint8_t slave[6] = ESP7;

char *mac_to_str(char *buffer, uint8_t *mac)
{
    sprintf(buffer, "%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return buffer;
}

void on_sent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    char buffer[13];
    switch (status)
    {
        case ESP_NOW_SEND_SUCCESS:
            gpio_set_level(LED, 1);
            ESP_LOGI(TAG, "message sent to %s", mac_to_str(buffer,(uint8_t *) mac_addr));
            break;
        case ESP_NOW_SEND_FAIL:
            gpio_set_level(LED, 0);
            ESP_LOGE(TAG, "message sent to %s failed", mac_to_str(buffer,(uint8_t *) mac_addr));
            break;
    }
}

void app_main(void)
{
    esp_rom_gpio_pad_select_gpio(LED);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);

    uint8_t my_mac[6];
    esp_efuse_mac_get_default(my_mac);
    char my_mac_str[13];
    ESP_LOGI(TAG, "My mac %s", mac_to_str(my_mac_str, my_mac));

    nvs_flash_init();
    esp_netif_init();
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_ERROR_CHECK(esp_now_init());
    ESP_ERROR_CHECK(esp_now_register_send_cb(on_sent));
    ESP_ERROR_CHECK(esp_now_set_pmk((uint8_t *)PMK_KEY_STR));

    esp_now_peer_info_t peer;
    memset(&peer, 0, sizeof(esp_now_peer_info_t));
    peer.channel = 0;
    peer.encrypt = true;
    memcpy(peer.lmk, LMK_KEY_STR, 16);
    memcpy(peer.peer_addr, slave, 6);
    ESP_ERROR_CHECK(esp_now_add_peer(&peer) != ESP_OK);

    char send_buffer[250];
    int i=0;
    while(true)
    {
        sprintf(send_buffer, "Hello from %s message %d", my_mac_str, i++);
        ESP_ERROR_CHECK(esp_now_send(slave, (uint8_t *)send_buffer, strlen(send_buffer)));
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

}
