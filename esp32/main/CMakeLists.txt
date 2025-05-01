idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."           # para poder incluir "motor_control.h"
    PRIV_REQUIRES motor_control
)
