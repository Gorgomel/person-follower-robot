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

Se desejar, posso salvar este conteúdo como um `README.md` ou `doc_setup.md` dentro do seu projeto para referência futura. Deseja isso?