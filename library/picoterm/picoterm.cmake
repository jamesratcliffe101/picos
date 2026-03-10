# Create an INTERFACE library for our C module.
add_library(picoterm INTERFACE)

# Add our source files to the lib
target_sources(picoterm INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/display.c
)

# Add the current directory as an include directory.
target_include_directories(picoterm INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)
