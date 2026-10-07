#include "fft/main.h"

/* Report error category 0 with error_code and clear g_main_system_graphics_error. */
void main_system_handle_pointer_exception(int error_code) {
    main_system_report_error(0, error_code);
    g_main_system_graphics_error = 0;
}
