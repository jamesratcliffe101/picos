# Use MicroPython as a static library (target: micropython_lib) instead of building a standalone firmware executable.
set(MICROPY_AS_LIBRARY ON)
set(USER_C_MODULES ${CMAKE_CURRENT_SOURCE_DIR}/src)
include(library/micropython/ports/rp2/micropython.cmake)