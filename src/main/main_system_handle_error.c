#include "fft/main.h"

/* The error hook of the file loads, the heap, the SMD loads and the battle sprite code. */
void main_system_handle_error(int error_code) {
    main_noop_800449f8(0, error_code);
    g_main_system_graphics_error = 0;
}
