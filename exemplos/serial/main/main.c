#include "driver/uart.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define UART_NUM UART_NUM_0

uint8_t data[128];
int length;

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
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM, 1, 3, 12, 14));
    const int uart_buffer_size = (1024 * 2);
    QueueHandle_t uart_queue;
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM, uart_buffer_size, uart_buffer_size, 10, &uart_queue, 0));

    while(1){
        ESP_ERROR_CHECK(uart_get_buffered_data_len(UART_NUM, (size_t*)&length));
        if (length>0){
            length = uart_read_bytes(UART_NUM, data, length, 100);
            uart_write_bytes(UART_NUM, (const char*)data, length);
        }
        //vTaskDelay(10/portTICK_PERIOD_MS);
    }

}
