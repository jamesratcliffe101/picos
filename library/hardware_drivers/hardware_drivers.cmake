add_library(hardware_drivers STATIC)

target_sources(hardware_drivers PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/audio.c
    ${CMAKE_CURRENT_LIST_DIR}/audio.h
    ${CMAKE_CURRENT_LIST_DIR}/clib.c
    ${CMAKE_CURRENT_LIST_DIR}/display.c
    ${CMAKE_CURRENT_LIST_DIR}/display.h
    ${CMAKE_CURRENT_LIST_DIR}/fat32.c
    ${CMAKE_CURRENT_LIST_DIR}/fat32.h
    ${CMAKE_CURRENT_LIST_DIR}/font-5x10.c
    ${CMAKE_CURRENT_LIST_DIR}/font-8x10.c
    ${CMAKE_CURRENT_LIST_DIR}/font.h
    ${CMAKE_CURRENT_LIST_DIR}/keyboard.c
    ${CMAKE_CURRENT_LIST_DIR}/keyboard.h
    ${CMAKE_CURRENT_LIST_DIR}/lcd.c
    ${CMAKE_CURRENT_LIST_DIR}/lcd.h
    ${CMAKE_CURRENT_LIST_DIR}/onboard_led.c
    ${CMAKE_CURRENT_LIST_DIR}/onboard_led.h
    ${CMAKE_CURRENT_LIST_DIR}/picocalc.c
    ${CMAKE_CURRENT_LIST_DIR}/picocalc.h
    ${CMAKE_CURRENT_LIST_DIR}/sdcard.c
    ${CMAKE_CURRENT_LIST_DIR}/sdcard.h
    ${CMAKE_CURRENT_LIST_DIR}/southbridge.c
    ${CMAKE_CURRENT_LIST_DIR}/southbridge.h
)

pico_generate_pio_header(hardware_drivers ${CMAKE_CURRENT_LIST_DIR}/audio.pio)

target_link_libraries(hardware_drivers PUBLIC
    pico_stdlib
    hardware_spi
    pico_status_led
    hardware_i2c
)



target_include_directories(hardware_drivers PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)