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
