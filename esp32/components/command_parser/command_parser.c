#include <string.h>
#include "command_parser.h"
#include "motor_control.h"
#include "esp_log.h"

static const char *TAG = "CMD_PARSER";

esp_err_t command_parser_handle(const char *mensagem, uint32_t handle) {
    if (!mensagem) {
        return ESP_ERR_INVALID_ARG;
    }

    if (strcmp(mensagem, "frente") == 0) {
        mover_frente();
        esp_spp_write(handle, strlen("Indo para frente\n"), (uint8_t *)"Indo para frente\n");
    } else if (strcmp(mensagem, "tras") == 0) {
        mover_tras();
        esp_spp_write(handle, strlen("Recuando\n"), (uint8_t *)"Recuando\n");
    } else if (strcmp(mensagem, "parar") == 0) {
        parar_motores();
        esp_spp_write(handle, strlen("Parando\n"), (uint8_t *)"Parando\n");
    } else {
        esp_spp_write(handle, strlen("Comando não reconhecido\n"), (uint8_t *)"Comando não reconhecido\n");
        ESP_LOGW(TAG, "Comando desconhecido: %s", mensagem);
        return ESP_ERR_NOT_FOUND;
    }

    ESP_LOGI(TAG, "Comando executado: %s", mensagem);
    return ESP_OK;
}
