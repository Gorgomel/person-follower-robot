#include "motor_control.h"
#include "driver/gpio.h"
#include "esp_log.h"

static const char *TAG = "motor_control";

void motor_control_test(void){
    #if CONFIG_MY_LOG_LEVEL >= 2
        ESP_LOGI(TAG, "Mensagem de log nível INFO");
    #endif
}    

#define IN1 26
#define IN2 27
#define IN3 32
#define IN4 33
#define IN5 16
#define IN6 17
#define IN7 18
#define IN8 19

static void set_motor(int pin, int level){
    gpio_set_level(pin, level);
}

void motores_init(void){
    int pinos[] = {IN1, IN2, IN3, IN4, IN5, IN6, IN7, IN8};
    for(int i = 0 ; i < 8 ; i++){
        gpio_reset_pin(pinos[i]);
        gpio_set_direction(pinos[i], GPIO_MODE_OUTPUT);
    }
    stop_motors();
}

void stop_motors(void){
    int pinos[] = {IN1, IN2, IN3, IN4, IN5, IN6, IN7, IN8};
    for(int i = 0 ; i < 8 ; i++)
        set_motor(pinos[i], 0);

    ESP_LOGI(TAG, "Motores parados");
}

void forward(void){
    set_motor(IN1, 1); set_motor(IN2, 0);
    set_motor(IN3, 1); set_motor(IN4, 0);
    set_motor(IN5, 0); set_motor(IN6, 1);
    set_motor(IN7, 0); set_motor(IN8, 1);
    ESP_LOGI(TAG, "Movendo para frente");
}

void backward(void){
    set_motor(IN1, 0); set_motor(IN2, 1);
    set_motor(IN3, 0); set_motor(IN4, 1);
    set_motor(IN5, 1); set_motor(IN6, 0);
    set_motor(IN7, 1); set_motor(IN8, 0);
    ESP_LOGI(TAG, "Movendo para trás");
}

void right_strafing(void){
    ESP_LOGI(TAG, "");
}

void left_strafing(void){
    ESP_LOGI(TAG, "");
}

void diagonal_forward_right(void){
    ESP_LOGI(TAG, "");
}

void diagonal_forward_left(void){
    ESP_LOGI(TAG, "");
}

void diagonal_backward_right(void){
    ESP_LOGI(TAG, "");
}

void diagonal_backward_left(void){
    ESP_LOGI(TAG, "");
}

void clockwise_rotation(void){
    ESP_LOGI(TAG, "");
}

void counterclockwise_rotation(void){
    ESP_LOGI(TAG,"");
}