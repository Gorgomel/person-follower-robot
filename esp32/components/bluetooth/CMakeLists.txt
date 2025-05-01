idf_component_register(
    SRCS "bluetooth_control.c"
    INCLUDE_DIRS "."
    REQUIRES bt
    PRIV_REQUIRES command_parser nvs_flash bt log esp_event
)

