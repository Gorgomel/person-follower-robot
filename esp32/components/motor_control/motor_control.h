#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

void motores_init(void);

void stop_motors(void);

//O robô se move para frente em linha reta, sem girar
void forward(void);

//O robô se move para trás em linha reta, mantendo a orientação.
void backward(void);

//O robô desliza lateralmente para a direita, sem rotacionar o corpo.
void right_strafing(void);

//O robô desliza lateralmente para a esquerda, mantendo a orientação.
void left_strafing(void);

//O robô avança em direção diagonal superior direita.
void diagonal_forward_right(void);

//O robô avança em direção diagonal superior esquerda.
void diagonal_forward_left(void);

//O robô recua em direção diagonal inferior direita.
void diagonal_backward_right(void);

//O robô recua em direção diagonal inferior esquerda.
void diagonal_backward_left(void);

//O robô gira sobre o próprio eixo no sentido horário.
void clockwise_rotation(void);

//O robô gira sobre o próprio eixo no sentido anti-horário.
void counterclockwise_rotation(void);

#endif // MOTOR_CONTROL_H
