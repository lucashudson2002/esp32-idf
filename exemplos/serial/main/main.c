#include "driver/uart.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "sdkconfig.h"

#define UART_NUM UART_NUM_0
#define BUF_SIZE (1024)

uint8_t data[7];
QueueHandle_t uart_queue;
static const char* TAG = "Serial";

static void serial(void *pvParameters){
    uart_event_t event;
    int length;

    while (1){
        if (xQueueReceive(uart_queue, (void * )&event, 0xffffffffUL)){
            switch (event.type){
                case UART_DATA:
                    ESP_ERROR_CHECK(uart_get_buffered_data_len(UART_NUM, (size_t*)&length));
                    if (length>=7){
                        length = uart_read_bytes(UART_NUM, data, 7, 0);
                        uart_write_bytes(UART_NUM, (const char*)data, length);
                    }
                    break;
                default:
                    ESP_LOGE(TAG, "Erro na leitura Serial");
            }
        }
    }
}

void app_main(void){
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    ESP_ERROR_CHECK(uart_param_config(UART_NUM, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM, BUF_SIZE*2, BUF_SIZE*2, 7, &uart_queue, 0));

    xTaskCreate(serial, "serial", 2048, NULL, 2, NULL);
}