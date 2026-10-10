#include "fft/main.h"

/* Error hook BATTLE calls, mostly when a unit lookup by id fails: the code is
 * dropped (main_system_handle_malloc_exception is empty) and
 * g_main_system_graphics_error is cleared. */
void main_system_handle_pointer_exception(int error_code) {
    main_system_handle_malloc_exception(0, error_code);
    g_main_system_graphics_error = 0;
}
