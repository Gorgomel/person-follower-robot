idf_component_register(
    SRCS "bluetooth_control.c"
    INCLUDE_DIRS "."
    PRIV_REQUIRES nvs_flash bt log esp_event
)
