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

cmake_minimum_required(VERSION 3.13)

# Initialize Pico SDK
include(pico_sdk_import.cmake)
        
project(picos C CXX ASM)
pico_sdk_init()

include(micro_python_import.cmake)
include(library/vtterminal/vtterminal.cmake)
include(library/lcd/lcdinit.cmake)

add_executable(picos
    src/main.cpp
    src/modmyport.c
)

target_compile_definitions(micropython_lib PUBLIC
    MICROPY_NLR_SETJMP=1 
)

# Link against Pico SDK
target_link_libraries(picos 
        pico_stdlib
        display_drivers
        usermod_vtterminal
        micropython_lib
        pico_status_led
        pico_rand
        hardware_i2c
        hardware_timer
        hardware_clocks
        )

pico_enable_stdio_usb(picos 1)
pico_enable_stdio_uart(picos 0)

pico_add_extra_outputs(picos)

add_custom_target(flash 
    COMMAND picotool load -xf ${CMAKE_BINARY_DIR}/picos.uf2
    DEPENDS picos
    COMMENT "Flashing program to Pico...")