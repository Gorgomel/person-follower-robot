idf_component_register(
    SRCS
        "main.c"
        "tasks.c"        # inclua aqui todas as .c da sua aplicação
    INCLUDE_DIRS
        "."              # para encontrar tasks.h
    PRIV_REQUIRES
        motor_control    # seu componente de motor
        bt_control       # seu componente de bluetooth
        command_parser   # se você delega parsing lá
)
