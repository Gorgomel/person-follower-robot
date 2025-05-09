// esp32/src/tasks.c
#include "tasks.h"
#include "command_parser.h"
#include "motor_control.h"
#include "esp_log.h"

static const char *TAG = "MOVEMENT";

// A fila é definida aqui (e extern em tasks.h)
QueueHandle_t bt_cmd_queue;

void movement_task(void *param)
{
    bt_cmd_t cmd;
    // stub de sensores (ainda não ligados ao Pico):
    bool obstacle_detected = false;

    for (;;) {
        // espera até 100 ms por um comando Bluetooth
        if (xQueueReceive(bt_cmd_queue, &cmd, pdMS_TO_TICKS(100))) {
            // se os sensores detectarem obstáculo, podemos ignorar certos comandos:
            if (obstacle_detected) {
                // parar_motores();  // exemplo de ação de segurança
                ESP_LOGW(TAG, "Obstáculo detectado — comando ignorado");
            } else {
                // trata o comando e escreve a resposta via SPP:
                if (command_parser_handle(cmd.data, cmd.handle) != ESP_OK) {
                    ESP_LOGW(TAG, "Falha ao tratar comando: %s", cmd.data);
                }
            }
        }
        // aqui poderia ler sensores a cada ciclo...
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
