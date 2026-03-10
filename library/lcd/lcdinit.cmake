add_library(display_drivers STATIC)

target_sources(display_drivers PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/lcdspi.c
    ${CMAKE_CURRENT_LIST_DIR}/lcdspi.h
)

target_link_libraries(display_drivers PUBLIC
    pico_stdlib
    pico_multicore
    hardware_spi
)

target_include_directories(display_drivers PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)