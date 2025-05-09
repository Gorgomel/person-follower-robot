// esp32/src/tasks.h
#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "esp_err.h"
#include "esp_spp_api.h"  // para uint32_t handle

#define BT_CMD_MAX_LEN  64

typedef struct {
    char     data[BT_CMD_MAX_LEN];
    uint32_t handle;
} bt_cmd_t;

// Esta fila será criada em main e usada no callback e na tarefa:
extern QueueHandle_t bt_cmd_queue;

// Tarefa que processa comandos Bluetooth (e futuramente sensores)
void movement_task(void *param);
