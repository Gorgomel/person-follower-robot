#pragma once

#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    MODE_STOPPED = 0,
    MODE_MANUAL,
    MODE_AUTONOMOUS
} operation_mode_t;

void set_operation_mode(operation_mode_t mode);
operation_mode_t get_operation_mode(void);

#ifdef __cplusplus
}
#endif
