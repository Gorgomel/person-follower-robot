# Setup do Ambiente Raspberry Pi Pico no WSL2 com VSCode

Este documento descreve passo a passo como configurar um ambiente de desenvolvimento para o Raspberry Pi Pico utilizando o **WSL2 (Ubuntu)** e o **Visual Studio Code** no Windows. O processo cobre desde a instalação do SDK até a geração do arquivo `.uf2` e acesso pelo explorador de arquivos do Windows.

---

## ✅ Requisitos

- WSL2 (Ubuntu 22.04)
- VSCode com extensão "Dev Containers" instalada
- Docker (opcional para contêiners, mas não usado aqui)
- [Extensão oficial do C/C++ da Microsoft no VSCode](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
- [GCC ARM para cross-compilation](https://packages.ubuntu.com/search?keywords=gcc-arm-none-eabi)

---

## 1. Clonando o SDK do Pico

Abra o terminal do Ubuntu (WSL2):

```bash
mkdir -p ~/pico
cd ~/pico
git clone -b master https://github.com/raspberrypi/pico-sdk.git
cd pico-sdk
git submodule update --init
```

---

## 2. Exportando o caminho do SDK

Adicione ao final do seu `~/.bashrc`:

```bash
echo 'export PICO_SDK_PATH=$HOME/pico/pico-sdk' >> ~/.bashrc
source ~/.bashrc
```

Verifique:

```bash
echo $PICO_SDK_PATH
```

---

## 3. Estrutura do Projeto Pico

Exemplo:

```
~/esp/som_seguidor/pico/
├── CMakeLists.txt
├── src/
│   └── main.c
```

### CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.13)
set(PICO_SDK_PATH "$ENV{PICO_SDK_PATH}")
include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(som_seguidor_pico)

pico_sdk_init()

add_executable(som_seguidor_pico
    src/main.c
)

target_link_libraries(som_seguidor_pico pico_stdlib)

pico_add_extra_outputs(som_seguidor_pico)
```

### src/main.c

```c
#include "pico/stdlib.h"

int main() {
    stdio_init_all();
    while (true) {
        sleep_ms(1000);
    }
}
```

---

## 4. Compilando o Projeto

```bash
cd ~/esp/som_seguidor/pico
mkdir build
cd build
cmake ..
make -j$(nproc)
```

Isso gerará:
```
build/som_seguidor_pico.uf2
```

---

## 5. Gravando no Raspberry Pi Pico

1. Conecte o Pico segurando o botão **BOOTSEL**
2. Ele aparecerá como um disco removível no Windows
3. Copie o `.uf2`:

```bash
cp som_seguidor_pico.uf2 /mnt/c/Users/Brunn/Desktop/RPI-RP2/
```

---

## 6. IntelliSense no VSCode

Crie um arquivo `.vscode/c_cpp_properties.json` no projeto:

```json
{
  "configurations": [
    {
      "name": "WSL-PICO",
      "includePath": [
        "${workspaceFolder}/**",
        "/home/brunoonurb/pico/pico-sdk/**"
      ],
      "defines": [],
      "compilerPath": "/usr/bin/arm-none-eabi-gcc",
      "cStandard": "c11",
      "cppStandard": "c++17",
      "intelliSenseMode": "linux-gcc-arm"
    }
  ],
  "version": 4
}
```

---

## 7. Acessando arquivos pelo Windows

Como o projeto está em `/home/brunoonurb/esp/som_seguidor/pico`, acesse-o pelo Windows via:

```
\\wsl$\Ubuntu-22.04\home\brunoonurb\esp\som_seguidor\pico
```

Cole isso na barra de endereço do Windows Explorer ou use via VSCode: **File > Open Folder > WSL**.

---

## ✅ Pronto!
Agora você pode desenvolver para o Raspberry Pi Pico de forma totalmente integrada com o VSCode e WSL2, com compilação, geração de `.uf2`, IntelliSense e upload manual prontos para uso.

---

Se quiser automatizar ainda mais (por exemplo, copiar o `.uf2` automaticamente quando o Pico for conectado), podemos criar scripts personalizados. Basta pedir!

