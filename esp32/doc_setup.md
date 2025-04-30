Claro! Aqui está um texto de **documentação técnica completa**, explicando tudo o que foi feito e configurado até agora. Essa descrição é ideal para ser compartilhada com outro ambiente, outra sessão do ChatGPT ou com membros do time.

---

# 📄 **Documentação de Configuração do Projeto ESP32 com ESP-IDF no WSL + VSCode**

Este documento descreve **todo o ambiente configurado**, incluindo ferramentas utilizadas, organização de pastas, e passos seguidos para deixar o ESP-IDF funcionando perfeitamente com suporte ao ESP32 dentro do WSL + VSCode.

---

## 🧭 Visão Geral do Projeto

- **Nome do projeto:** `som_seguidor`
- **Plataformas envolvidas:**
  - ESP32 (controlador principal)
  - Raspberry Pi Pico
  - Raspberry Pi 3
- **Modo de desenvolvimento:** via **WSL 2 (Ubuntu 22.04)** no Windows 11, usando **VSCode com extensões ESP-IDF e Remote - WSL**

---

## 📁 Estrutura do Diretório

```
~/esp/som_seguidor/
├── esp32/      ← Código do ESP32 com ESP-IDF
│   ├── main/
│   │   └── main.c
│   ├── CMakeLists.txt
│   └── sdkconfig
├── pico/       ← Código separado para Raspberry Pi Pico (ainda não tratado aqui)
├── rpi3/       ← Código da Raspberry Pi 3 (ainda não tratado aqui)
└── shared/     ← Diretório comum a todos os módulos
```

---

## 🛠️ Ambiente de Desenvolvimento

### ✅ Sistema Base

- **Windows 11**
- **WSL 2 ativado**
- **Distribuição Linux utilizada:** `Ubuntu-22.04`
- Pasta principal do projeto localizada em:  
  `/home/brunoonurb/esp/som_seguidor/`

---

### ✅ Ferramentas Instaladas

| Ferramenta        | Versão      | Local               |
|-------------------|-------------|---------------------|
| ESP-IDF           | v5.4.1      | `~/esp/esp-idf`     |
| VSCode            | Atualizado  | Windows             |
| Extensão ESP-IDF  | ✅ Instalado | Marketplace VSCode  |
| Extensão Remote - WSL | ✅       | Marketplace VSCode  |
| Python            | 3.10.12     | `/usr/bin/python`   |
| Toolchain Xtensa  | v14.2.0     | `~/.espressif/tools`|

---

## ⚙️ Configurações Realizadas

### ✅ Instalação e Configuração do ESP-IDF no WSL

1. Clonado o repositório oficial:
   ```bash
   cd ~/esp
   git clone -b v5.4.1 https://github.com/espressif/esp-idf.git
   ```

2. Instalado o ambiente:
   ```bash
   cd esp-idf
   ./install.sh
   ```

3. Exportado o ambiente com:
   ```bash
   source export.sh
   ```

4. Adicionado ao `.bashrc` para automatizar:
   ```bash
   echo 'source ~/esp/esp-idf/export.sh' >> ~/.bashrc
   ```

---

### ✅ Inicialização do Projeto ESP32

Criado o projeto manualmente com:

- `main.c` básico:
  ```c
  #include <stdio.h>

  void app_main(void)
  {
      printf("ESP32 inicializado com sucesso!\n");
  }
  ```

- `main/CMakeLists.txt`:
  ```cmake
  idf_component_register(SRCS "main.c"
                          INCLUDE_DIRS ".")
  ```

- `esp32/CMakeLists.txt`:
  ```cmake
  cmake_minimum_required(VERSION 3.16)
  include($ENV{IDF_PATH}/tools/cmake/project.cmake)
  project(som_seguidor)
  ```

---

### ✅ Build, Flash e Monitor

Com o terminal WSL ativado no diretório `esp32/`, os comandos utilizados foram:

```bash
idf.py fullclean
idf.py build
idf.py -p /dev/ttyUSB0 flash
idf.py -p /dev/ttyUSB0 monitor
```

> OBS: O dispositivo ESP32 é identificado como `/dev/ttyUSB0` no Linux.

---

## 📂 Acesso pelo Windows

Para abrir a pasta no **explorador de arquivos do Windows**:

```bash
explorer.exe .
```

Ou acesse diretamente:

```
\\wsl$\Ubuntu-22.04\home\brunoonurb\esp\som_seguidor\esp32
```

---

## ✅ Status Atual

- Ambiente ESP-IDF funcional no WSL
- Projeto `esp32` compila com sucesso
- `idf.py build` concluído sem erros
- `app_main` básico presente
- VSCode acessando projeto dentro do WSL com extensão ativa
- Pronto para desenvolvimento dos componentes personalizados (`bt_control`, `motor_control`, etc.)

---




# Documentação da Configuração do Ambiente ESP-IDF no WSL2

Este documento reúne todas as etapas e ajustes realizados com sucesso para rodar, compilar, flashar e testar Bluetooth num ESP32-WROOM usando o ESP-IDF dentro do WSL2/Ubuntu no Windows 11.

---

## 1. Ambiente WSL2 + VSCode  
- **Distribuição:** Ubuntu 22.04 rodando sobre WSL2.  
- **Editor:** Visual Studio Code com a extensão **Espressif IDF**, apontada para a distro WSL.  

---

## 2. Instalação e configuração do ESP-IDF  
- Clonagem do repositório ESP-IDF na pasta `~/esp/esp-idf`.  
- Execução de `./install.sh` e `./export.sh` (ou configuração manual de variáveis de ambiente).  
- Verificado o target com:
  ```bash
  idf.py set-target esp32
  ```

---

## 3. Encaminhamento da porta USB Serial (USBIPD-WIN)  
Para que o WSL enxergue o conversor USB-UART (CP210x/CH340/FTDI):

1. **No Windows (PowerShell Admin):**  
   ```powershell
   winget install --id=usbipd-win.usbipd-win
   usbipd bind   --busid 2-3 --persist    # 2-3 é o BUSID do CP210x (COM3)
   ```
2. **Cada vez que plugar o ESP32:**  
   ```powershell
   usbipd attach --wsl --busid 2-3
   ```
3. **No WSL/Ubuntu:**  
   ```bash
   sudo modprobe cp210x                 # carrega driver, se necessário  
   dmesg | grep -i cp210x               # confirma attachment  
   ls /dev/ttyUSB* /dev/ttyACM*         # deve listar /dev/ttyUSB0
   ```

---

## 4. Permissões de acesso à porta serial  
```bash
sudo usermod -aG dialout $USER         # adiciona ao grupo dialout
# reinicie a sessão WSL
sudo chmod a+rw /dev/ttyUSB0           # libera leitura/escrita
```

---

## 5. Fluxo de build / flash / monitor  
Dentro da pasta do projeto (ex.: `~/esp/som_seguidor/esp32`):
```bash
idf.py -p /dev/ttyUSB0 build flash monitor
```
- `-p /dev/ttyUSB0` → porta serial correta  
- `build` → compila o firmware  
- `flash` → grava no ESP32  
- `monitor` → abre o terminal serial (115200 bps)

---

## 6. Configuração Bluetooth no menuconfig  
Execute:
```bash
idf.py menuconfig
```
Em **Component config → Bluetooth**:
1. **[ * ] Bluetooth** (habilita o módulo Bluedroid)  
2. **Host:** “Bluedroid – Dual-mode”  
3. **Controller:** “Enabled”  
4. Em **Bluedroid Options**:
   - **[ * ] Classic Bluetooth**  
   - **[ * ] Bluetooth Low Energy**  
   - (deixe os perfis desmarcados se não precisar; o importante é ativar o stack)

---

## 7. Ajuste em `main.c` para Dual-Mode  
No `main/main.c`, substitua:
```c
ESP_ERROR_CHECK(esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT));
```
por:
```c
ESP_ERROR_CHECK(esp_bt_controller_enable(ESP_BT_MODE_BTDM));
```
Isso evita o erro `ESP_ERR_INVALID_ARG` e alinha o código ao menuconfig Dual-mode.

---

## 8. Exemplo mínimo de `app_main()`  
```c
void app_main(void)
{
    // ... inicialização NVS e controller ...
    ESP_ERROR_CHECK(esp_bt_controller_enable(ESP_BT_MODE_BTDM));
    ESP_ERROR_CHECK(esp_bluedroid_init());
    ESP_ERROR_CHECK(esp_bluedroid_enable());
    ESP_ERROR_CHECK(esp_bt_gap_register_callback(bt_app_gap_cb));
    ESP_ERROR_CHECK(esp_bt_dev_set_device_name("ESP32_BT_TEST"));
    ESP_ERROR_CHECK(esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE));
    ESP_LOGI(TAG, "Bluetooth discoverable como 'ESP32_BT_TEST'");
    // Laço simples de teste
    for (int i = 0; i < 5; ++i) {
        ESP_LOGI(TAG, "Loop de teste: %d", i);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    ESP_LOGI(TAG, "Teste finalizado");
}
```

---

## 9. Saindo do monitor serial  
- **Ctrl + ]** (padrão do `idf.py monitor`)  
- Se estiver no Windows Terminal, experimente **Ctrl + Shift +]**  
- Como último recurso, **Ctrl + C** duas vezes (pode exigir reiniciar o terminal)

---

## ℹ️ Dica Final – Como abrir o VSCode com ESP-IDF corretamente

Para garantir que o VSCode funcione com o ESP-IDF corretamente:

1. Abra o terminal WSL2 (Ubuntu)
2. Vá até a pasta do projeto:
   ```bash
   cd ~/esp/som_seguidor/esp32
   ```
3. Execute:
   ```bash
   code .
   ```

Dessa forma, o VSCode herdará as variáveis de ambiente (como `PATH` e `IDF_PATH`) do ESP-IDF e evitará problemas com compiladores errados.
