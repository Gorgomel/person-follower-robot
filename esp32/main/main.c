#include <stdio.h>
#include <stdint.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "tasks.h"
#include "sensor.h"
#include "uart_comm.h"
#include "operation_mode.h"
#include "motor_control.h"
#include "bluetooth_control.h"
#include "command_parser.h"

#include "esp_log.h"

static const char *TAG = "MAIN";

void app_main(void)
{
    ESP_LOGI(TAG, "System initialization complete. Current mode: %d", get_operation_mode());

    // Define modo inicial
    set_operation_mode(MODE_STOPPED);

    // Criação das filas
    uart_raw_queue = xQueueCreate(4, UART_RAW_MAX_LEN);
    if (!uart_raw_queue) {
        ESP_LOGE(TAG, "Failed to create uart_raw_queue");
        return;
    }

    sensor_data_queue = xQueueCreate(1, sizeof(sensor_data_t));
    if (!sensor_data_queue) {
        ESP_LOGE(TAG, "Failed to create sensor_data_queue");
        return;
    }

    vTaskDelay(pdMS_TO_TICKS(1000));
    
    // Inicializações
    uart_comm_init();
    motor_control_init();
    bluetooth_control_init();

    // Criação das tarefas
    xTaskCreatePinnedToCore(uart_task, "uart_task", 4096, NULL, 4, NULL, 0);         // Core 0
    xTaskCreatePinnedToCore(sensor_task, "sensor_task", 4096, NULL, 3, NULL, 1);     // Core 1
    xTaskCreatePinnedToCore(pid_task, "pid_task", 4096, NULL, 3, NULL, 1);           // Core 1

    ESP_LOGI(TAG, "System initialization complete. Current mode: %d", get_operation_mode());
}
