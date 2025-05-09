idf_component_register(
    SRCS "bluetooth_control.c"
    INCLUDE_DIRS "."
    PRIV_REQUIRES
        bt              # pilha Bluetooth Classic
        nvs_flash       # nvs_flash_init()
        log             # ESP_LOG*
        command_parser  # para usar command_parser_handle()
)
