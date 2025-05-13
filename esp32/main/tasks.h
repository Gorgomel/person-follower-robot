#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "esp_err.h"
#include "esp_spp_api.h"

#define BT_CMD_MAX_LEN  64

typedef struct {
    char     data[BT_CMD_MAX_LEN];
    uint32_t handle;
} bt_cmd_t;

// Fila Bluetooth
extern QueueHandle_t bt_cmd_queue;

// Tarefa que processa os comandos da fila
void cmd_task(void *param);

void pid_task(void *param);

