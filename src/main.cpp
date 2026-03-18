#include <stdio.h>
#include <cstdlib>
#include "pico/stdlib.h"
#include "pico/status_led.h"
#include "pico/rand.h"

#include "hardware/timer.h"
#include "hardware/clocks.h"

extern "C" {
    
    #include "py/runtime.h"
    #include "py/gc.h"
    #include "py/stackctrl.h"
    #include "shared/readline/readline.h"
    #include "shared/runtime/pyexec.h"
    #include "picoterm.h"
    #include "picocalc.h"
}

typedef intptr_t mp_int_t;
typedef uintptr_t mp_uint_t;
typedef long mp_off_t;

int64_t alarm_callback(alarm_id_t id, void *user_data) {
    // Put your timeout handler code in here
    return 0;
}


void run_python_script(const char* source_code) {
    size_t heap_size = 1024 * 10;
    // Use uint8_t* or char* to ensure byte-accurate math
    uint8_t* heap = (uint8_t*)malloc(heap_size);
    
    // 1. Initialize GC with start and end addresses
    gc_init(heap, heap + heap_size);
    
    // 2. Set the C-Stack limit to prevent overflows
    mp_stack_set_limit(2048); 
    
    // 3. Initialize the VM core
    mp_init();
    
    // 4. Execute the string
    // NOTE: Use pyexec_str() for raw code strings.
    pyexec_file_if_exists(source_code);
    
    // 5. Clean up when the "sub-program" finishes
    mp_deinit();
    free(heap);
}


void basic_light_test() {
    status_led_init();

    while (true)
    {
        status_led_get_state() ? status_led_set_state(false) : status_led_set_state(true);
        status_led_get_state() ? printf("led ON") : printf("led OFF");
        sleep_ms(1000);
    }
    
}

void repl_test() {

    status_led_init();

    sleep_ms(5000);

    status_led_set_state(true);
    
    printf("\r\nOS Booting...\r\n"); // Basic SDK print test
    static uint32_t heap[4096];

    printf("\r\nGC init...\r\n");
    gc_init(heap, heap + (sizeof(heap) / sizeof(uint32_t)));

    printf("\r\nMP INIT...\r\n");
    mp_init();

    pyexec_friendly_repl();
    printf("\r\nREPL STOPPING...\r\n");

    // 1. Disable all hardware interrupts first
    irq_set_enabled(USBCTRL_IRQ, false); 
    
    // 2. Clear the Python stack/state
    mp_stack_ctrl_init(); 
    
    // 3. NOW it is safe to deinit
    mp_deinit();
    
    // 4. Finally, re-enable interrupts or reset the board
    irq_set_enabled(USBCTRL_IRQ, true);
    printf("\r\nHALT\r\n");
}

/*
void lcd_test() {
    display_init();

    
    while (true) {
        sleep_ms(1000); // so display can fully awake
        uint16_t rand_colour = get_rand_32();
        uint16_t start_x = get_rand_32() % (WIDTH - 10);
        uint16_t start_y = get_rand_32() % (HEIGHT - 10);

        printf("startx: %d  starty: %d \n", start_x, start_y);

        uint32_t width = get_rand_32() % (WIDTH - start_x);
        uint32_t height = get_rand_32() % (HEIGHT - start_y);

        printf("width: %d  height: %d \n", width, height);

        display_solid_rectangle((uint16_t)rand_colour, start_x, start_y, width, height);
        printf("generating random shape \n");
    }
    
}
*/

void terminal_test() {
    picoterm_start();
}

int main()
{
    // need to initialise hardware,
    // then need to startup micropython and execute boot.py
    stdio_init_all();

    sleep_ms(10000); // sleep for 10 seconds so I have time to setup the serial monitor

    picoterm_start();
}

