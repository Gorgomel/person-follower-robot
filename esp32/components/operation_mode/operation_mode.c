#include "operation_mode.h"
#include "esp_log.h"

static const char *TAG = "OP_MODE";

static operation_mode_t current_mode = MODE_STOPPED;

void set_operation_mode(operation_mode_t mode)
{
    if (mode != current_mode) {
        current_mode = mode;
        switch (mode) {
            case MODE_STOPPED:
                ESP_LOGI(TAG, "Mode changed to: STOPPED");
                break;
            case MODE_MANUAL:
                ESP_LOGI(TAG, "Mode changed to: MANUAL");
                break;
            case MODE_AUTONOMOUS:
                ESP_LOGI(TAG, "Mode changed to: AUTONOMOUS");
                break;
            default:
                ESP_LOGW(TAG, "Unknown mode selected");
                break;
        }
    }
}

operation_mode_t get_operation_mode(void)
{
    return current_mode;
}
