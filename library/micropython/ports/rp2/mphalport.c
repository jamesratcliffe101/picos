#include <unistd.h>
#include <stdio.h>
#include <setjmp.h>
#include "py/builtin.h"
#include "py/compile.h"
#include "py/gc.h"
#include "py/mperrno.h"
#include "py/mpconfig.h"
#include "pico/stdlib.h"


// Receive single character
int mp_hal_stdin_rx_chr(void) {
    // this method is a little harder as we sometimes want to listen to inputs from the builtin keyboard, and other times from
    // an external input. again, getchar_timeout_us() might be best if we can pipe the builtin keyboard output through it
    // worth a dig around. OK yeah I think should really take advantage of pico/stdio!
    for (;;) {
        int c = getchar_timeout_us(100);
        if (c != PICO_ERROR_TIMEOUT) {
            return c;
        }
        // Small sleep to keep the CPU from 100% usage while waiting
        sleep_ms(1);
    }
}

// Send string of given length
void mp_hal_stdout_tx_strn(const char *str, size_t len) {
    // for this to properly work in PICOS, we need to pipe the data the the correct location
    // If PICOS says we want python to get sent via USB to an external command line, then we run printf() 
    // If PICOS says we want to pipe the output and input though our custom picoterm (terminal), then we make picoterm calls
    // actually, we need to make stdio calls, then picoterm can pico those up, and PICOS can manage them, so actually printf() is 
    // probably best

    for (size_t i = 0; i < len; i++) {
        putchar(str[i]);
    }
}

void mp_hal_stdout_tx_str(const char *str, size_t len) {
    for (size_t i = 0; i < len; i++) {
        putchar(str[i]);
    }
    printf(str);
}

uint32_t mp_hal_ticks_ms(void) {
    return to_ms_since_boot(get_absolute_time());
}

// 2. The "Cooked" Output Function (Handles \n -> \r\n)
void mp_hal_stdout_tx_strn_cooked(const char *str, size_t len) {
    for (size_t i = 0; i < len; i++) {
        if (str[i] == '\n') {
            putchar('\r');
        }
        putchar(str[i]);
    }
}

void gc_collect(void) {
// 1. Start the GC collection process
    gc_collect_start();

    // 2. Capture CPU registers by pushing them onto the stack
    jmp_buf dummy;
    setjmp(dummy); 

    // 3. Trace the stack
    // 'MP_STATE_THREAD(stack_top)' was set during your mp_stack_ctrl_init()
    uintptr_t sp = (uintptr_t)__builtin_frame_address(0);
    gc_collect_root((void**)sp, ((uintptr_t)MP_STATE_THREAD(stack_top) - sp) / sizeof(uintptr_t));

    // 4. End the collection
    gc_collect_end();
}

void nlr_jump_fail(void *val) {
    printf("MicroPython Fatal Error: NLR Jump Fail\n");
    while (1); // Halt the CPU
}

// There is no filesystem so stat'ing returns nothing.
mp_import_stat_t mp_import_stat(const char *path) {
    return MP_IMPORT_STAT_NO_EXIST;
}

// There is no filesystem so opening a file raises an exception.
mp_lexer_t *mp_lexer_new_from_file(qstr filename) {
    mp_raise_OSError(MP_ENOENT);
}
