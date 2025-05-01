idf_component_register(
    SRCS "command_parser.c"
    INCLUDE_DIRS "."
    REQUIRES bt        # esp_spp_api.h, esp_bt_main.h
    PRIV_REQUIRES motor_control
)
