// pico/src/main.c
#include "pico/stdlib.h"
#include "hardware/uart.h"

int main() {
    stdio_init_all();        // para debugging via USB CDC
    uart_init(uart0, 115200);
    gpio_set_function(0, GPIO_FUNC_UART);  // TX
    gpio_set_function(1, GPIO_FUNC_UART);  // RX (não usado aqui)

    while (true) {
        uart_puts(uart0, "Hello from Pico!\r\n");
        sleep_ms(1000);
    }
}
