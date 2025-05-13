#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

#include "driver/uart.h"
#include "esp_log.h"

#include "uart_comm.h"

QueueHandle_t uart_raw_queue = NULL;

#define UART_NUM        UART_NUM_1
#define UART_RX_PIN     4       // <-- Mude conforme o seu hardware
#define UART_TX_PIN     -1      // Não utilizado
#define UART_BAUDRATE   115200
#define BUF_SIZE        128

static const char *TAG = "UART_COMM";

void uart_comm_init(void)
{
    const uart_config_t uart_config = {
        .baud_rate = UART_BAUDRATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM, BUF_SIZE * 2, 0, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(UART_NUM, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM, UART_TX_PIN, UART_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    ESP_LOGI(TAG, "UART inicializada com RX no GPIO %d", UART_RX_PIN);
}

void uart_task(void *param)
{
    uint8_t data[BUF_SIZE];

    while (1) {
        int len = uart_read_bytes(UART_NUM, data, BUF_SIZE - 1, pdMS_TO_TICKS(100));
        if (len > 0) {
            data[len] = '\0';  // Garante string válida
            ESP_LOGI(TAG, "Recebido: %s", (char *) data);
            // Aqui futuramente: parse dos dados e envio para fila
        }
    }
}
