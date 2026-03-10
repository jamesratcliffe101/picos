add_library(hardware_drivers STATIC)

target_sources(hardware_drivers PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/font.h
    ${CMAKE_CURRENT_LIST_DIR}/font-5x10.c
    ${CMAKE_CURRENT_LIST_DIR}/font-8x10.c
    ${CMAKE_CURRENT_LIST_DIR}/keyboard.c
    ${CMAKE_CURRENT_LIST_DIR}/keyboard.h
    ${CMAKE_CURRENT_LIST_DIR}/lcd.c
    ${CMAKE_CURRENT_LIST_DIR}/lcd.h
    ${CMAKE_CURRENT_LIST_DIR}/onboard_led.c
    ${CMAKE_CURRENT_LIST_DIR}/onboard_led.h
    ${CMAKE_CURRENT_LIST_DIR}/southbridge.c
    ${CMAKE_CURRENT_LIST_DIR}/southbridge.h
)

target_link_libraries(hardware_drivers PUBLIC
    pico_stdlib
    hardware_spi
    pico_status_led
    hardware_i2c
)

target_include_directories(hardware_drivers PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)