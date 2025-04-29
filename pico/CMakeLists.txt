cmake_minimum_required(VERSION 3.13)

set(PICO_SDK_PATH "/home/brunoonurb/pico/pico-sdk")
include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(som_seguidor_pico)

pico_sdk_init()

add_executable(som_seguidor_pico
    src/main.c
)

target_link_libraries(som_seguidor_pico pico_stdlib)

pico_add_extra_outputs(som_seguidor_pico)
