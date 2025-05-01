#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include "esp_err.h"
#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"
#include "esp_spp_api.h"


// Função pública para tratar comandos recebidos via Bluetooth
esp_err_t command_parser_handle(const char *mensagem, uint32_t handle);

#endif // COMMAND_PARSER_H
