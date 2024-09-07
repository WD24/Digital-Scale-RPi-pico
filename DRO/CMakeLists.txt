cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(DRO C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})

pico_sdk_init()

add_executable(DRO DRO.c)

# enable usb output, disable uart output
pico_enable_stdio_usb(DRO 1)
pico_enable_stdio_uart(DRO 0)

pico_generate_pio_header (DRO ${CMAKE_CURRENT_LIST_DIR}/Scale.pio)
target_link_libraries(DRO pico_stdlib hardware_pio)

pico_add_extra_outputs(DRO)
