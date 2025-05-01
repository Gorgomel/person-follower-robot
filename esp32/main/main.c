#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "motor_control.h"
#include "bluetooth_control.h"

void app_main(void)
{
    printf("Starting som_seguidor robot...\n");

    motores_init();
    bluetooth_control_init();

    // Movimento inicial de teste
    mover_frente();
    vTaskDelay(pdMS_TO_TICKS(2000));
    parar_motores();
}
