#include <stdint.h>

// 1. Define the Machine Architecture
#define MICROPY_EMIT_THUMB          (1)
#define MICROPY_EMIT_INLINE_THUMB   (1)
#define MICROPY_ARCH_ARMV7M         (1)

// Python internal features.
#define MICROPY_ENABLE_GC                       (1)
#define MICROPY_HELPER_REPL                     (1)
#define MICROPY_REPL_INFO                       (1)



#define MICROPY_USE_READLINE_HISTORY            (0)
#define MICROPY_READLINE_HISTORY_SIZE           (8)
#define MICROPY_HELPER_LEXER_UNIX               (1)
#define MICROPY_ENABLE_SOURCE_LINE              (1)
#define MICROPY_REPL_EVENT_DRIVEN               (0)
#define MICROPY_ERROR_REPORTING                 (MICROPY_ERROR_REPORTING_TERSE)
#define MICROPY_FLOAT_IMPL                      (MICROPY_FLOAT_IMPL_FLOAT)
#define MICROPY_PY_SYS (1)
#define MICROPY_PY_SYS_EXIT (1)

// Fine control over Python builtins, classes, modules, etc.
#define MICROPY_PY_ASYNC_AWAIT                  (0)
#define MICROPY_PY_BUILTINS_SET                 (0)
#define MICROPY_PY_ATTRTUPLE                    (0)
#define MICROPY_PY_COLLECTIONS                  (0)
#define MICROPY_PY_MATH                         (0)
#define MICROPY_PY_IO                           (0)
#define MICROPY_PY_STRUCT                       (0)

#define MICROPY_ENABLE_DYNLIB (0)
#define MICROPY_PY_BUILTINS_HELP (0)



// Type definitions for the specific machine.

typedef long mp_off_t;

// We need to provide a declaration/definition of alloca().
#include <alloca.h>

// Define the port's name and hardware.
#define MICROPY_HW_BOARD_NAME "example-board"
#define MICROPY_HW_MCU_NAME   "unknown-cpu"

#define MP_STATE_PORT MP_STATE_VM