add_library(picoterm STATIC)

target_sources(picoterm PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/picoterm.c
    ${CMAKE_CURRENT_LIST_DIR}/commands.c
    ${CMAKE_CURRENT_LIST_DIR}/commands.h
    ${CMAKE_CURRENT_LIST_DIR}/songs.c
    ${CMAKE_CURRENT_LIST_DIR}/songs.h
    ${CMAKE_CURRENT_LIST_DIR}/tests.c
    ${CMAKE_CURRENT_LIST_DIR}/tests.h
)

target_link_libraries(picoterm PUBLIC
    hardware_drivers
    micropython_lib
    pico_stdlib
    hardware_spi
    pico_rand
    pico_status_led
    hardware_i2c
)

target_include_directories(picoterm PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)