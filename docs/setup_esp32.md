# 📄 Configuração Completa do Projeto ESP32 com ESP-IDF no WSL2 + VSCode

Este guia descreve como preparar, configurar e compilar o projeto `som_seguidor` com ESP32 usando ESP-IDF 5.4.1 no ambiente **WSL2 (Ubuntu 22.04)** com **VSCode**, incluindo suporte completo ao Bluetooth Classic.

---

## 🧭 Visão Geral

- **Nome do projeto:** `som_seguidor`
- **Plataforma:** ESP32-WROOM-32
- **Ambiente:** WSL2 (Ubuntu 22.04) rodando no Windows 11
- **Editor:** VSCode com extensões: ESP-IDF e Remote - WSL

---

## 📁 Estrutura de Pastas

```
~/esp/som_seguidor/
├── esp32/               # Código principal para ESP32
│   ├── src/             # Código fonte do projeto
│   │   └── main.c
│   ├── components/
│   │   ├── bluetooth/
│   │   │   ├── bluetooth_control.c
│   │   │   ├── bluetooth_control.h
│   │   │   ├── CMakeLists.txt
│   │   │   ├── Kconfig.projbuild
│   │   │   └── README.md
│   │   ├── command_parser/
│   │   │   ├── command_parser.c
│   │   │   ├── command_parser.h
│   │   │   └── CMakeLists.txt
│   │   └── motor_control/
│   │       ├── motor_control.c
│   │       ├── motor_control.h
│   │       ├── CMakeLists.txt
│   │       └── Kconfig.projbuild
│   ├── CMakeLists.txt   # Principal do projeto ESP-IDF
│   └── sdkconfig        # Gerado via menuconfig
├── pico/                # (futuro) Raspberry Pi Pico
├── rpi3/                # (futuro) Raspberry Pi 3
├── shared/              # Recursos comuns aos módulos
└── docs/
    └── setup_esp32.md   # Este documento
```

---

## 🛠️ Ferramentas e Versões

| Ferramenta              | Versão       | Local                        |
|-------------------------|--------------|------------------------------|
| ESP-IDF                 | v5.4.1       | `~/esp/esp-idf`              |
| VSCode                  | Atualizado   | Windows                      |
| Python                  | 3.10.12       | `/usr/bin/python`            |
| Xtensa Toolchain (ESP)  | 14.2.0       | `~/.espressif/tools`         |
| Extensões VSCode        | ESP-IDF, Remote - WSL | VSCode Marketplace |

---

## ⚙️ Instalação do ESP-IDF

```bash
cd ~/esp
git clone -b v5.4.1 https://github.com/espressif/esp-idf.git
cd esp-idf
./install.sh
source export.sh
echo 'source ~/esp/esp-idf/export.sh' >> ~/.bashrc
```

---

## 🔌 Encaminhamento da porta USB no WSL2

Para conectar o ESP32 via USB no WSL:

### No PowerShell (Admin):
```powershell
winget install usbipd-win.usbipd-win
usbipd bind --busid 2-3 --persist
usbipd attach --wsl --busid 2-3
```

### No WSL:
```bash
sudo modprobe cp210x
sudo chmod a+rw /dev/ttyUSB0
```

---

## 🧱 Configuração do Projeto ESP32

### Inicializar projeto

```bash
cd ~/esp/som_seguidor/esp32
idf.py set-target esp32
idf.py menuconfig
```

### No menuconfig:

- **Component config → Bluetooth**:
  - [✓] Enable Bluetooth
  - Host: *Bluedroid – Dual-mode*
  - Controller: *Enabled*
- **Bluedroid Options**:
  - [✓] Classic Bluetooth
  - [✓] SPP (Serial Port Profile)

---

## 🧩 CMakeLists.txt principal (esp32/)

```cmake
cmake_minimum_required(VERSION 3.16)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(som_seguidor)
```

## 📦 src/CMakeLists.txt

```cmake
idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    PRIV_REQUIRES bt nvs_flash log
)
```

---

## 💡 Bluetooth Classic no estilo oficial

Você pode copiar a lógica do arquivo `bt_spp_acceptor_demo_main.c` para sua aplicação ou integrá-lo modularmente via `components/bluetooth`.

### Recomendações:

- Use `esp_bt_controller_enable(ESP_BT_MODE_BTDM)` em vez de `ESP_BT_MODE_CLASSIC_BT` para evitar erros com Dual-mode.
- Ative o NVS com `nvs_flash_init()` antes de qualquer função BT.

---

## 🧪 Compilar, flashar e monitorar

```bash
idf.py fullclean
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor
```

Para sair do monitor: `Ctrl + ]`

---

## 📎 Dicas finais

### Para abrir corretamente no VSCode:
```bash
cd ~/esp/som_seguidor/esp32
code .
```

source ~/esp/esp-idf/export.sh


Assim o VSCode herda todas as variáveis do ambiente ESP-IDF.

## 📦 CMakeLists.txt dos Componentes

### components/bluetooth/CMakeLists.txt
```cmake
idf_component_register(
    SRCS "bluetooth_control.c"
    INCLUDE_DIRS "."
    PRIV_REQUIRES bt command_parser
)
```

### components/command_parser/CMakeLists.txt
```cmake
idf_component_register(
    SRCS "command_parser.c"
    INCLUDE_DIRS "."
    PRIV_REQUIRES motor_control bt
)
```

### components/motor_control/CMakeLists.txt
```cmake
idf_component_register(
    SRCS "motor_control.c"
    INCLUDE_DIRS "."
    PRIV_REQUIRES driver
)
```


Perfeito. Aqui está o conteúdo pronto para ser inserido no final do arquivo `setup_esp32.md`, já formatado em Markdown:

---

## Estrutura de Execução com FreeRTOS

O projeto `som_seguidor` utiliza o sistema operacional de tempo real FreeRTOS como núcleo da arquitetura multitarefa no ESP32. A seguir, detalha-se a estrutura das tarefas, filas e módulos integrados.

### Organização de Tarefas

| Tarefa        | Núcleo | Prioridade | Stack | Descrição                                                                                                                          |
| ------------- | ------ | ---------- | ----- | ---------------------------------------------------------------------------------------------------------------------------------- |
| `uart_task`   | Core 0 | 4          | 4096  | Lê dados da UART recebidos do Raspberry Pi Pico e os insere na fila `uart_raw_queue`.                                              |
| `sensor_task` | Core 1 | 3          | 4096  | Simula ou processa os dados de sensores ultrassônicos e envia o resultado para a fila `sensor_data_queue`.                         |
| `pid_task`    | Core 1 | 3          | 4096  | Realiza o controle de velocidade com base nos dados da fila de sensores. Usa `vTaskDelayUntil()` para garantir frequência estável. |
| `bt_cmd_task` | Core 0 | 5          | 4096  | Recebe comandos via Bluetooth (SPP), interpreta e despacha para os motores. Responsável pela operação manual.                      |

### Estrutura de Filas

* `uart_raw_queue`: fila circular (`QueueHandle_t`) de 4 entradas, usada para transmitir mensagens cruas da UART para o processamento.
* `sensor_data_queue`: fila com buffer para estruturas do tipo `sensor_data_t`, usada para troca de dados entre sensores e o controle PID.
* `bt_cmd_queue`: fila interna do módulo Bluetooth usada para armazenar comandos recebidos via SPP antes do despacho.

### Modo de Operação

Foi definido um `enum modo_t` com três modos de operação:

* `MODE_STOPPED`: estado neutro, motores parados.
* `MODE_AUTONOMOUS`: ativado pelo `pid_task` para controlar a velocidade com base nos sensores.
* `MODE_MANUAL`: ativado via Bluetooth, permite controle direto dos motores com comandos como frente, ré, giros, diagonais, etc.

A função `set_operation_mode()` permite alterar o estado global da operação. Essa variável é protegida por funções getter/setter para controle centralizado.

### Comunicação Serial (UART)

A UART é configurada para receber dados do Raspberry Pi Pico. A comunicação é baseada em interrupções e armazenamento dos dados brutos na fila `uart_raw_queue`, que serão utilizados posteriormente para análises ou decisões de navegação.

### Bluetooth

Utiliza o perfil SPP clássico com callbacks `esp_spp_cb()` e `esp_bt_gap_cb()`. A task `bt_cmd_task` interpreta comandos numéricos (via `atoi`) para movimentação do robô, como frente, ré, strafing, diagonais e rotações.

### Controle de Motores

O módulo `motor_control` abstrai os comandos de movimentação, permitindo chamadas como `forward()`, `backward()`, `left_strafing()`, etc. Cada uma dessas funções escreve diretamente nos GPIOs conectados às pontes H.

### Controle PID

O controle proporcional-integral-derivativo (PID) é simulado com base em dados de sensores e executado periodicamente com `vTaskDelayUntil()` para garantir tempo constante entre iterações, importante para estabilidade do sistema.

### Watchdog

O watchdog por software será integrado posteriormente, quando o sistema estiver completamente estável. Será utilizado para detectar travamentos nas tarefas críticas.
