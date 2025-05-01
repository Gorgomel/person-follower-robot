#include <stdio.h>
#include "motor_control.h"

void app_main(void)
{
    printf("Hello from som_seguidor!\n");
    
    motores_init();
    mover_frente();
    vTaskDelay(pdMS_TO_TICKS(2000));
    parar_motores();
}
