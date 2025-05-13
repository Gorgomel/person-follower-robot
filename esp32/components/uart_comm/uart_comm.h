#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#define UART_RAW_MAX_LEN 64

extern QueueHandle_t uart_raw_queue;

#ifdef __cplusplus
extern "C" {
#endif

void uart_comm_init(void);
void uart_task(void *param);

#ifdef __cplusplus
}
#endif
