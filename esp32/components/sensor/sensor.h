#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

typedef struct {
    float distancia_cm;
    float velocidade_mps;
    float aceleracao_mps2;
} sensor_data_t;

extern QueueHandle_t sensor_data_queue;

void sensor_task(void *param);  // simula envio de dados
