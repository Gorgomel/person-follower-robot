#include "sensor.h"
#include "freertos/task.h"
#include "esp_log.h"

#define SENSOR_TASK_DELAY_MS 100

QueueHandle_t sensor_data_queue = NULL;
static const char *TAG = "SENSOR_TASK";

void sensor_task(void *param)
{
    sensor_data_t dado;

    while (1) {
        // Simulação de dados (podem ser aleatórios ou fixos)
        dado.distancia_cm = 100.0f;
        dado.velocidade_mps = 0.5f;
        dado.aceleracao_mps2 = 0.02f;

        xQueueOverwrite(sensor_data_queue, &dado);  // sobrescreve o último valor

        ESP_LOGI(TAG, "Simulado: dist=%.2fcm vel=%.2fm/s", dado.distancia_cm, dado.velocidade_mps);
        vTaskDelay(pdMS_TO_TICKS(SENSOR_TASK_DELAY_MS));
    }
}
