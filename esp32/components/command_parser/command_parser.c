#include <string.h>
#include <stdlib.h>
#include "command_parser.h"
#include "motor_control.h"
#include "esp_log.h"
#include "esp_spp_api.h"

static const char *TAG = "CMD_PARSER";

esp_err_t command_parser_handle(const char *mensagem, uint32_t handle) {
    if (!mensagem) return ESP_ERR_INVALID_ARG;

    int comando = atoi(mensagem);

    switch (comando) {
        case 1:
            forward();
            esp_spp_write(handle, strlen("Frente\n"), (uint8_t *)"Frente\n");
            break;
        case 2:
            backward();
            esp_spp_write(handle, strlen("Ré\n"), (uint8_t *)"Ré\n");
            break;
        case 3:
            stop_motors();
            esp_spp_write(handle, strlen("Parando\n"), (uint8_t *)"Parando\n");
            break;
        case 4:
            right_strafing();
            esp_spp_write(handle, strlen("Direita\n"), (uint8_t *)"Direita\n");
            break;
        case 5:
            left_strafing();
            esp_spp_write(handle, strlen("Esquerda\n"), (uint8_t *)"Esquerda\n");
            break;
        case 6:
            diagonal_forward_right();
            esp_spp_write(handle, strlen("Diagonal frente-direita\n"), (uint8_t *)"Diagonal frente-direita\n");
            break;
        case 7:
            diagonal_forward_left();
            esp_spp_write(handle, strlen("Diagonal frente-esquerda\n"), (uint8_t *)"Diagonal frente-esquerda\n");
            break;
        case 8:
            diagonal_backward_right();
            esp_spp_write(handle, strlen("Diagonal trás-direita\n"), (uint8_t *)"Diagonal trás-direita\n");
            break;
        case 9:
            diagonal_backward_left();
            esp_spp_write(handle, strlen("Diagonal trás-esquerda\n"), (uint8_t *)"Diagonal trás-esquerda\n");
            break;
        case 10:
            clockwise_rotation();
            esp_spp_write(handle, strlen("Girando horário\n"), (uint8_t *)"Girando horário\n");
            break;
        case 11:
            counterclockwise_rotation();
            esp_spp_write(handle, strlen("Girando anti-horário\n"), (uint8_t *)"Girando anti-horário\n");
            break;
        default:
            esp_spp_write(handle, strlen("Comando não reconhecido\n"), (uint8_t *)"Comando não reconhecido\n");
            ESP_LOGW(TAG, "Comando desconhecido: %s", mensagem);
            return ESP_ERR_NOT_FOUND;
    }

    ESP_LOGI(TAG, "Comando executado: %s", mensagem);
    return ESP_OK;
}
