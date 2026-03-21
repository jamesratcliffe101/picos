# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")
set(PICO_STACK_SIZE 0x2000 CACHE STRING "Stack size")

cmake_minimum_required(VERSION 3.13)

# Initialize Pico SDK
include(pico_sdk_import.cmake)

# MicroPython's patched libraries must be set BEFORE pico_sdk_init() so the
# SDK picks them up instead of its own bundled copies.
set(_MICROPY_VENDOR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/library/micropython/lib)
set(PICO_TINYUSB_PATH ${_MICROPY_VENDOR_DIR}/tinyusb)
set(PICO_LWIP_PATH ${_MICROPY_VENDOR_DIR}/lwip)
set(PICO_BTSTACK_PATH ${_MICROPY_VENDOR_DIR}/btstack)

project(picos C CXX ASM)
pico_sdk_init()

string(APPEND CMAKE_C_FLAGS " -I${PICO_SDK_PATH}/src/rp2_common/hardware_flash/include")
string(APPEND CMAKE_CXX_FLAGS " -I${PICO_SDK_PATH}/src/rp2_common/hardware_flash/include")

include(micro_python_import.cmake)
include(library/hardware_drivers/hardware_drivers.cmake)
include(library/picoterm/picoterm.cmake)
#include(library/lcd/lcdinit.cmake)

add_executable(picos
    src/main.cpp
)

target_compile_definitions(micropython_lib PUBLIC
    MICROPY_NLR_SETJMP=1
    MICROPY_HW_ENABLE_USBDEV=0
)

# Link against Pico SDK
target_link_libraries(picos 
        pico_stdlib
        micropython_lib
        pico_status_led
        pico_rand
        pico_multicore
        hardware_i2c
        hardware_timer
        hardware_clocks
        hardware_drivers
        picoterm
        )

pico_enable_stdio_usb(picos 1)
pico_enable_stdio_uart(picos 0)

pico_add_extra_outputs(picos)

add_custom_target(flash 
    COMMAND picotool load -xf ${CMAKE_BINARY_DIR}/picos.uf2
    DEPENDS picos
    COMMENT "Flashing program to Pico...")