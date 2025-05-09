#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gap_bt_api.h"
#include "esp_spp_api.h"

#include "bluetooth_control.h"
#include "command_parser.h"

static const char *TAG = "BT_CONTROL";

#define BT_CMD_MAX_LEN 128

typedef struct {
    char     data[BT_CMD_MAX_LEN];
    uint32_t handle;
} bt_cmd_t;

static QueueHandle_t bt_cmd_queue;

// Task que despacha os comandos recebidos pelo SPP
static void bt_cmd_task(void *pv)
{
    bt_cmd_t cmd;
    for (;;)
    {
        if (xQueueReceive(bt_cmd_queue, &cmd, portMAX_DELAY) == pdTRUE) {
            ESP_LOGI(TAG, "Despachando comando: %s", cmd.data);
            command_parser_handle(cmd.data, cmd.handle);
        }
    }
}

void esp_bt_gap_cb(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param)
{
    ESP_LOGI(TAG, "GAP event: %d", event);
}

void esp_spp_cb(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)
{
    switch (event) {
    case ESP_SPP_INIT_EVT:
        ESP_LOGI(TAG, "ESP_SPP_INIT_EVT");
        break;
    case ESP_SPP_SRV_OPEN_EVT:
        ESP_LOGI(TAG, "Cliente conectado via Bluetooth SPP!");
        break;
    case ESP_SPP_DATA_IND_EVT: {
        bt_cmd_t cmd;
        int len = param->data_ind.len > BT_CMD_MAX_LEN - 1
                  ? BT_CMD_MAX_LEN - 1
                  : param->data_ind.len;
        memcpy(cmd.data, param->data_ind.data, len);
        cmd.data[len] = '\0';
        cmd.handle = param->data_ind.handle;
        if (xQueueSend(bt_cmd_queue, &cmd, 0) != pdTRUE) {
            ESP_LOGW(TAG, "bt_cmd_queue cheia, comando descartado");
        }
        break;
    }
    default:
        ESP_LOGI(TAG, "SPP evento não tratado: %d", event);
        break;
    }
}

void bluetooth_control_init(void)
{
    ESP_LOGI(TAG, "Inicializando Bluetooth...");

    // Inicializa NVS (necessário para Bluedroid)
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Libera memória BLE (usaremos somente Classic)
    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_BLE));

    // Inicializa e habilita o controlador Classic BT
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_bt_controller_init(&bt_cfg));
    ESP_ERROR_CHECK(esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT));

    // Inicializa e habilita o Bluedroid
    esp_bluedroid_config_t bd_cfg = BT_BLUEDROID_INIT_CONFIG_DEFAULT();
    bd_cfg.ssp_en = false;
    ESP_ERROR_CHECK(esp_bluedroid_init_with_cfg(&bd_cfg));
    ESP_ERROR_CHECK(esp_bluedroid_enable());

    // Registra callbacks
    ESP_ERROR_CHECK(esp_bt_gap_register_callback(esp_bt_gap_cb));
    ESP_ERROR_CHECK(esp_spp_register_callback(esp_spp_cb));

    // Configura SPP em modo callback
    esp_spp_cfg_t spp_cfg = {
        .mode = ESP_SPP_MODE_CB,
        .enable_l2cap_ertm = true,
        .tx_buffer_size = 0,
    };
    ESP_ERROR_CHECK(esp_spp_enhanced_init(&spp_cfg));

    // Cria fila e tarefa para despachar comandos Bluetooth
    bt_cmd_queue = xQueueCreate(10, sizeof(bt_cmd_t));
    configASSERT(bt_cmd_queue);
    xTaskCreate(bt_cmd_task, "bt_cmd_task", 2048, NULL, 5, NULL);

    // Define nome do dispositivo e modo de scan
    ESP_ERROR_CHECK(esp_bt_gap_set_device_name("ESP32_SEGUIDOR"));
    ESP_ERROR_CHECK(esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE));

    // PIN fixo para emparelhamento legado
    esp_bt_pin_type_t pin_type = ESP_BT_PIN_TYPE_FIXED;
    esp_bt_pin_code_t pin_code = { '1', '2', '3', '4' };
    ESP_ERROR_CHECK(esp_bt_gap_set_pin(pin_type, 4, pin_code));

    // Inicia servidor SPP
    ESP_ERROR_CHECK(esp_spp_start_srv(ESP_SPP_SEC_NONE, ESP_SPP_ROLE_SLAVE, 0, "SPP_SERVER"));

    ESP_LOGI(TAG, "Bluetooth inicializado com sucesso.");
}
