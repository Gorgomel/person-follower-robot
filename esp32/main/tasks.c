#include "tasks.h"
#include "command_parser.h"
#include "motor_control.h"
#include "esp_log.h"
#include "sensor.h"

static const char *TAG = "CMD_TASK";

// Definição da fila (compartilhada com o Bluetooth)
QueueHandle_t bt_cmd_queue;

void cmd_task(void *param)
{
    bt_cmd_t cmd;
    bool obstacle_detected = false;  // stub — para integrar depois com sensores

    for (;;) {
        // Espera por novo comando vindo do Bluetooth
        if (xQueueReceive(bt_cmd_queue, &cmd, pdMS_TO_TICKS(100))) {
            if (obstacle_detected) {
                ESP_LOGW(TAG, "Obstáculo detectado — comando ignorado");
                // stop_motors();
            } else {
                if (command_parser_handle(cmd.data, cmd.handle) != ESP_OK) {
                    ESP_LOGW(TAG, "Falha ao tratar comando: %s", cmd.data);
                }
            }
        }

        vTaskDelay(pdMS_TO_TICKS(10));  // Alívio de CPU
    }
}

void pid_task(void *param)
{
    sensor_data_t dado;
    TickType_t xLastWakeTime = xTaskGetTickCount();

    const TickType_t intervalo = pdMS_TO_TICKS(100);  // 100 ms
    const float setpoint = 0.8f;  // velocidade desejada

    while (1) {
        if (xQueuePeek(sensor_data_queue, &dado, 0)) {
            float erro = setpoint - dado.velocidade_mps;
            float controle = erro * 1.5f;  // PID proporcional simples
            ESP_LOGI("PID", "Erro: %.2f, Controle: %.2f", erro, controle);
        }
        vTaskDelayUntil(&xLastWakeTime, intervalo);
    }
}