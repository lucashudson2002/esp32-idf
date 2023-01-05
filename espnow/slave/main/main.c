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

#define TAG "ESP_NOW"

#define ESP6 {0x10, 0x52, 0x1C, 0x62, 0xD7, 0x9C}
#define ESP7 {0xAC, 0x67, 0xB2, 0x3B, 0xCD, 0xA8}

static const char* PMK_KEY_STR = "RINOBOT_PATO_VS3";
static const char* LMK_KEY_STR = "RINOBOT_PATO_VS3";

uint8_t master[6] = ESP6;

char *mac_to_str(char *buffer, uint8_t *mac)
{
    sprintf(buffer, "%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return buffer;
}

void on_receive(const uint8_t *mac_addr, const uint8_t *data, int data_len)
{
    char buffer[13];
    ESP_LOGI(TAG, "got message from %s", mac_to_str(buffer, (uint8_t *)mac_addr));

    printf("message: %.*s\n", data_len, data);
}

void app_main(void)
{
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
    ESP_ERROR_CHECK(esp_now_register_recv_cb(on_receive));
    ESP_ERROR_CHECK(esp_now_set_pmk((uint8_t *)PMK_KEY_STR));

    esp_now_peer_info_t peer;
    memset(&peer, 0, sizeof(esp_now_peer_info_t));
    peer.channel = 0;
    peer.encrypt = true;
    memcpy(peer.lmk, LMK_KEY_STR, 16);
    memcpy(peer.peer_addr, master, 6);
    ESP_ERROR_CHECK(esp_now_add_peer(&peer) != ESP_OK);
}
