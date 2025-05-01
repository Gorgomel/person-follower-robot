#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

// Inicialização
void motores_init(void);
void parar_motores(void);

// Movimentos principais
void mover_frente(void);
void mover_tras(void);
void girar_esquerda(void);
void girar_direita(void);

// Movimento lateral
void andar_lado_esq(void);
void andar_lado_dir(void);

// Diagonais
void mover_diag_esq_frente(void);
void mover_diag_dir_frente(void);

// Teste
void motor_control_test(void);

#endif // MOTOR_CONTROL_H
