add_library(usermod_myport INTERFACE)

target_sources(usermod_myport INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/modmyport.c
)

target_link_libraries(usermod INTERFACE usermod_myport)
