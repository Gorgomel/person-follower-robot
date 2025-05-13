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
├── esp32/                         # Projeto principal do ESP32
│   ├── src/                       # Arquivo principal com app_main()
│   │   └── main.c
│   ├── components/                # Componentes customizados
│   │   ├── bluetooth/             # Módulo Bluetooth SPP
│   │   │   ├── bluetooth_control.c
│   │   │   ├── bluetooth_control.h
│   │   │   ├── CMakeLists.txt
│   │   │   └── Kconfig.projbuild
│   │   ├── command_parser/        # Interpreta comandos recebidos
│   │   │   ├── command_parser.c
│   │   │   ├── command_parser.h
│   │   │   └── CMakeLists.txt
│   │   ├── motor_control/         # Controle das rodas via GPIOs
│   │   │   ├── motor_control.c
│   │   │   ├── motor_control.h
│   │   │   ├── CMakeLists.txt
│   │   │   └── Kconfig.projbuild
│   │   ├── uart_comm/             # Comunicação serial com o Raspberry Pi Pico
│   │   │   ├── uart_comm.c
│   │   │   ├── uart_comm.h
│   │   │   └── CMakeLists.txt
│   │   ├── operation_mode/        # Enum e controle de modo (manual/autônomo)
│   │   │   ├── operation_mode.c
│   │   │   ├── operation_mode.h
│   │   │   └── CMakeLists.txt
│   │   ├── sensor/                # Leitura e simulação de sensores
│   │   │   ├── sensor_task.c
│   │   │   ├── sensor_task.h
│   │   │   └── CMakeLists.txt
│   │   └── pid/                   # Controle PID da velocidade
│   │       ├── pid_control.c
│   │       ├── pid_control.h
│   │       └── CMakeLists.txt
│   ├── CMakeLists.txt             # Projeto raiz ESP-IDF
│   └── sdkconfig                  # Configurações do menuconfig
├── pico/                          # Código do Raspberry Pi Pico (em C)
├── rpi3/                          # Código do Raspberry Pi 3 (em Python)
├── shared/                        # Arquivos compartilhados (ex: uart, mensagens)
├── docs/                          # Documentação técnica
│   └── setup_esp32.md
└── README.md                      # Apresentação geral do repositório
```

## 🛠️ Ferramentas e Versões

| Ferramenta               | Versão       | Local                               |
|--------------------------|--------------|--------------------------------------|
| **ESP-IDF**              | v5.4.1       | `~/esp/esp-idf`                      |
| **VSCode**               | Atualizado   | Windows 11 + WSL2                    |
| **Python**               | 3.10.12      | `/usr/bin/python`                   |
| **Xtensa Toolchain**     | 14.2.0       | `~/.espressif/tools/xtensa-esp-elf/`|
| **Extensões VSCode**     | ESP-IDF, Remote - WSL | VSCode Marketplace       |
| **Sistema Operacional**  | Ubuntu 22.04 (WSL2) | Dentro do Windows 11         |
| **USB Redirecionado**    | `usbipd-win` | Instalado no Windows (via winget)    |

## ⚙️ Instalação do ESP-IDF

```bash
cd ~/esp
git clone -b v5.4.1 https://github.com/espressif/esp-idf.git
cd esp-idf
./install.sh
source export.sh
echo 'source ~/esp/esp-idf/export.sh' >> ~/.bashrc
```

## 🔌 Encaminhamento da porta USB no WSL2

### No Windows (PowerShell como Admin):
```powershell
winget install usbipd-win
usbipd list
usbipd bind --busid 2-3 --persist
usbipd attach --wsl --busid 2-3
```

### No WSL2:
```bash
sudo modprobe cp210x
sudo chmod a+rw /dev/ttyUSB0
```

## 🧱 Configuração do Projeto ESP32

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

## 📦 Organização dos CMakeLists.txt

Cada componente define seu `CMakeLists.txt` com `idf_component_register`, informando fontes (`SRCS`), headers (`INCLUDE_DIRS`) e dependências (`PRIV_REQUIRES`). O projeto principal (`esp32/CMakeLists.txt`) define o alvo geral e carrega os componentes.

## 💡 Bluetooth Classic no estilo oficial

Utiliza o perfil SPP (Serial Port Profile), baseado na demo oficial `bt_spp_acceptor_demo_main.c`, porém com arquitetura modular:

- ✅ `esp_bt_controller_enable(ESP_BT_MODE_BTDM)` para compatibilidade BLE + Classic.
- ✅ `nvs_flash_init()` executado antes do uso do BT.
- ✅ Callbacks `esp_bt_gap_cb` e `esp_spp_cb` registrados.
- ✅ Fila `bt_cmd_queue` usada para envio de comandos via Bluetooth para a task `bt_cmd_task`.

## 🧪 Compilar, flashar e monitorar

```bash
idf.py fullclean
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor
```

> Para sair do monitor: `Ctrl + ]`

## 📎 Dicas finais

```bash
cd ~/esp/som_seguidor/esp32
code .
source ~/esp/esp-idf/export.sh
```

## 🧵 Estrutura de Execução com FreeRTOS

| Tarefa        | Núcleo | Prioridade | Stack | Descrição                                                                                                                          |
| ------------- | ------ | ---------- | ----- | ---------------------------------------------------------------------------------------------------------------------------------- |
| `uart_task`   | Core 0 | 4          | 4096  | Lê dados da UART recebidos do Raspberry Pi Pico e os insere na fila `uart_raw_queue`.                                              |
| `sensor_task` | Core 1 | 3          | 4096  | Simula ou processa os dados de sensores ultrassônicos e envia o resultado para a fila `sensor_data_queue`.                         |
| `pid_task`    | Core 1 | 3          | 4096  | Realiza o controle de velocidade com base nos dados da fila de sensores. Usa `vTaskDelayUntil()` para garantir frequência estável. |
| `bt_cmd_task` | Core 0 | 5          | 4096  | Recebe comandos via Bluetooth (SPP), interpreta e despacha para os motores. Responsável pela operação manual.                      |

### Filas

- `uart_raw_queue`: recebe bytes crus da UART.
- `sensor_data_queue`: recebe structs do tipo `sensor_data_t`.
- `bt_cmd_queue`: recebe comandos Bluetooth para despacho.

### Modo de Operação (`modo_t`)

- `MODE_STOPPED`: robô parado.
- `MODE_AUTONOMOUS`: modo PID baseado em sensores.
- `MODE_MANUAL`: comandos via Bluetooth controlam o robô.

### Resumo dos Módulos

- **UART**: comunicação com Raspberry Pi Pico via fila.
- **Bluetooth**: controle manual via SPP (modo texto ou numérico com `atoi`).
- **Motores**: controlados via `motor_control` com funções encapsuladas.
- **PID**: controle periódico com base na distância.
- **Watchdog**: planejado para fase futura de estabilidade.

---