#include <string.h>
#include "command_parser.h"
#include "motor_control.h"
#include "esp_log.h"

static const char *TAG = "CMD_PARSER";

esp_err_t command_parser_handle(const char *mensagem, uint32_t handle) {
    if (!mensagem) {
        return ESP_ERR_INVALID_ARG;
    }

    //Mover para frente
    if (mensagem[0] == '1') {
        forward();
        esp_spp_write(handle, strlen("Indo para frente\n"), (uint8_t *)"Indo para frente\n");
    } 
    
    //Movendo para tras 
    else if (mensagem[0] == '2') {
        backward();
        esp_spp_write(handle, strlen("Recuando\n"), (uint8_t *)"Recuando\n");
    } 
    
    else if (mensagem[0] == '3') {
        stop_motors();
        esp_spp_write(handle, strlen("Parando\n"), (uint8_t *)"Parando\n");
    } 
    
    else {
        esp_spp_write(handle, strlen("Comando não reconhecido\n"), (uint8_t *)"Comando não reconhecido\n");
        ESP_LOGW(TAG, "Comando desconhecido: %s", mensagem);
        return ESP_ERR_NOT_FOUND;
    }

    ESP_LOGI(TAG, "Comando executado: %s", mensagem);
    return ESP_OK;
}
