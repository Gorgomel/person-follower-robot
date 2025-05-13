```markdown
# 🤖 Projeto do Robô Seguidor de Pessoas com Tocador de Música

## 📌 Objetivo do Projeto
Este projeto consiste em um robô móvel com rodas omnidirecionais capaz de operar em dois modos:
1. Controle manual via Bluetooth.
2. Navegação autônoma com detecção de pessoa por câmera.

Além disso, o robô é capaz de tocar música durante o deslocamento.

---

## 🧱 Arquitetura Geral

```text
[Sensores (Ultrassom/IMU)] → Pico → UART → ESP32 ← UART / Bluetooth ← RPi3 / Smartphone
                                                      ↓
                                            [FreeRTOS com 4 Tarefas]
                                                      ↓
                              ┌────────────┬────────────┬────────────┬────────────┐
                              │ sensor_task│ pid_task   │ bt_task    │ uart_task  │
                              └────────────┴────────────┴────────────┴────────────┘
                                                      ↓
                                            Controle de 4 Motores (2 Pontes H)


---

## 🗂 Estrutura de Diretórios

```text
som_seguidor/
├── esp32/
│   ├── CMakeLists.txt
│   ├── sdkconfig.defaults
│   ├── src/
│   │   ├── main.c               ← Lógica principal do robô
│   │   └── CMakeLists.txt
│   ├── components/
│   │   ├── bluetooth/           ← Baseado no exemplo oficial `bt_spp_acceptor`
│   │   │   ├── bt_spp_acceptor_demo_main.c
│   │   │   ├── CMakeLists.txt
│   │   │   ├── Kconfig.projbuild
│   │   │   ├── README.md
│   │   │   └── ESP32_SSP.md
│   │   └── ... (futuros módulos)
│
├── pico/
│   ├── ultrasonic_sender.c     ← Leitura de sensores e envio via UART
│   ├── imu_sender.c            ← (Futuro) Envio de dados da IMU
│   └── main.c                  ← Loop principal
│
├── rpi3/
│   ├── autonomous_control.py   ← Detecção de pessoa e envio de comandos
│   ├── image_processing.py     ← Visão computacional com OpenCV
│   └── uart_interface.py       ← Comunicação serial com ESP32
│
└── shared/                     ← Estruturas de dados e mensagens padrão
```

---

## 🔁 Modos de Operação

### 🟦 Modo Manual (Bluetooth Classic SPP)
- Comunicação Bluetooth com smartphone (SPP).
- Usa o exemplo `bt_spp_acceptor` como base.
- ESP32 interpreta os comandos e move os motores.

### 🔴 Modo Autônomo
- Raspberry Pi 3 detecta uma pessoa usando uma câmera.
- Envia comandos via UART para o ESP32.
- O ESP32 move o robô com base nesses comandos.
- Sensores ultrassônicos garantem desvio de obstáculos.

---

## 📦 Módulos Importantes

| Módulo                 | Descrição                                     |
|------------------------|-----------------------------------------------|
| `bluetooth`            | Comunicação via Bluetooth Classic (SPP)       |
| `motor_control`        | (Futuro) Controle de 4 motores                |
| `ultrasonic_receiver`  | (Futuro) Leitura dos sensores via UART        |
| `rpi_comm`             | (Futuro) Comunicação com Raspberry Pi 3       |
| `mode_switch`          | (Futuro) Alternância de modo manual/autônomo |

---

## 📡 Protocolo de Comunicação

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

## 🧪 Bluetooth (SPP)

- O módulo `bluetooth` é baseado no exemplo `bt_spp_acceptor` da ESP-IDF.
- Permite receber dados do celular via terminal Bluetooth.
- Utiliza o protocolo RFCOMM para comunicação serial sem fio.

### 📲 Comandos Manuais por Bluetooth

| Comando | Ação                      |
|--------:|---------------------------|
| `1`     | Frente                    |
| `2`     | Ré                        |
| `3`     | Parar                     |
| `4`     | Deslizar para Direita     |
| `5`     | Deslizar para Esquerda    |
| `6`     | Diagonal Frente-Direita   |
| `7`     | Diagonal Frente-Esquerda  |
| `8`     | Diagonal Trás-Direita     |
| `9`     | Diagonal Trás-Esquerda    |
