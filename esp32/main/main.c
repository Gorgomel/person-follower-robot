// esp32/src/main.c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tasks.h"
#include "motor_control.h"
#include "bluetooth_control.h"
#include "esp_log.h"

static const char *TAG = "APP";

void app_main(void)
{
    ESP_LOGI(TAG, "Iniciando som_seguidor...");

    // 1) inicializa GPIO dos motores
    motores_init();

    // 2) cria a fila de comandos Bluetooth
    bt_cmd_queue = xQueueCreate(10, sizeof(bt_cmd_t));
    if (bt_cmd_queue == NULL) {
        ESP_LOGE(TAG, "Não foi possível criar bt_cmd_queue");
        return;
    }

    // 3) cria a tarefa de movimento (prioridade 5)
    xTaskCreate(movement_task, "movement_task", 4096, NULL, 5, NULL);

    // 4) inicializa o Bluetooth (callback vai usar bt_cmd_queue)
    bluetooth_control_init();
}
