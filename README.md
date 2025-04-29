# 🤖 Projeto do Robô Seguidor de Pessoas com Tocador de Música

## 📌 Objetivo do Projeto
Este projeto consiste em um robô móvel com rodas omnidirecionais capaz de operar em dois modos:
1. Controle manual via Bluetooth.
2. Navegação autônoma com detecção de pessoa por câmera.

Além disso, o robô é capaz de tocar música durante o deslocamento.

---

## 🧱 Arquitetura Geral

```text
[Sensores Ultrassônicos / IMU] → Raspberry Pi Pico → UART → ESP32 ← UART / Bluetooth ← Raspberry Pi 3 / Smartphone
                                                                  ↓
                                                        Controle de Motores (2 Pontes H)
```

---

## 🗂 Estrutura de Diretórios

```text
som_seguidor/
├── esp32/
│   ├── components/
│   │   ├── motor_control/         ← Controle dos motores
│   │   ├── bt_control/            ← Comandos via Bluetooth
│   │   ├── ultrasonic_receiver/   ← Recebe sensores do Pico via UART
│   │   ├── rpi_comm/              ← Recebe comandos do RPi3 via UART
│   │   └── mode_switch/           ← Alternância entre modos
│   └── main/                      ← Arquivo main.c com lógica central
│
├── pico/
│   ├── ultrasonic_sender.c       ← Leitura de sensores e envio via UART
│   ├── imu_sender.c              ← (Futuro) Envio de dados da IMU
│   └── main.c                    ← Loop principal
│
├── rpi3/
│   ├── autonomous_control.py     ← Detecção de pessoa e envio de comandos
│   ├── image_processing.py       ← Visão computacional com OpenCV
│   └── uart_interface.py         ← Comunicação serial com ESP32
│
└── shared/           ← Estruturas de dados ou mensagens padronizadas
```

---

## 🔁 Modos de Operação

### 🟦 Modo Manual
- Controlado via Bluetooth por celular.
- Interpretado pelo ESP32 e redirecionado aos motores.

### 🔴 Modo Autônomo
- Raspberry Pi 3 detecta uma pessoa usando uma câmera.
- Envia comandos via UART para o ESP32.
- O ESP32 move o robô com base nesses comandos.
- Sensores ultrassônicos garantem desvio de obstáculos.

---

## 📦 Módulos Importantes

| Módulo                | Descrição                             |
|------------------------|------------------------------------------|
| `motor_control`        | Controla 4 motores ligados a 2 pontes H  |
| `bt_control`           | Recebe comandos Bluetooth                |
| `ultrasonic_receiver`  | Recebe dados do Pico via UART           |
| `rpi_comm`             | Recebe comandos do RPi3 via UART        |
| `mode_switch`          | Alterna entre modos com botão/comando   |

---

## 🧪 Protocolo de Comunicação

### UART entre Pico e ESP32
```json
{
  "front": 12.3,
  "left": 10.2,
  "right": 9.8
}
```

### UART entre RPi3 e ESP32
```json
{
  "vx": 0.3,
  "vy": 0.1,
  "omega": 0.05
}
```

---

## 🎯 Próximos Passos

1. Criar estrutura de pastas completa com `CMakeLists.txt` para cada módulo.
2. Implementar `motor_control` e `bt_control` para testes.
3. Criar alternador de modo com botão físico e UART.
4. Implementar detecção com câmera no RPi3.

---