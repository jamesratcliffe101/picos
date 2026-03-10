set(MICROPY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/library/micropython)
set(MICROPY_LIB_DIR ${MICROPY_DIR}/lib)
include(${MICROPY_DIR}/py/py.cmake)


set(MICROPY_SOURCE_QSTR
    ${MICROPY_SOURCE_PY}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/modmyport.c
)

add_library(micropython_lib STATIC
    ${MICROPY_SOURCE_PY}
    ${MICROPY_DIR}/ports/rp2/mpconfigport.h
    ${MICROPY_DIR}/ports/rp2/mphalport.h
    ${MICROPY_DIR}/ports/rp2/mphalport.c
    ${MICROPY_DIR}/shared/runtime/pyexec.c
    ${MICROPY_DIR}/shared/runtime/pyexec.h
    ${MICROPY_DIR}/shared/readline/readline.c
    ${MICROPY_DIR}/shared/readline/readline.h
    ${MICROPY_DIR}/extmod/modplatform.h
    ${MICROPY_DIR}/extmod/moductypes.c
    ${MICROPY_DIR}/extmod/virtpin.h
)

set_target_properties(micropython_lib PROPERTIES LINKER_LANGUAGE C)

target_link_libraries(micropython_lib
    PUBLIC pico_stdlib
)

target_include_directories(micropython_lib PUBLIC 
    ${MICROPY_DIR}/ports/rp2  
    ${MICROPY_DIR}
    ${MICROPY_DIR}/py
    ${CMAKE_CURRENT_BINARY_DIR} # This is where py.cmake puts generated files
)

target_compile_definitions(micropython_lib PRIVATE ${MICROPY_CPPFLAGS})

set(MICROPY_TARGET micropython_lib)
micropy_gather_target_properties(${MICROPY_TARGET})

include(${MICROPY_DIR}/py/mkrules.cmake)
set(MICROPY_SOURCE_QSTR ${MICROPY_SOURCE_QSTR} ${CMAKE_CURRENT_SOURCE_DIR}/src/modmyport.c)