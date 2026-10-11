#include "fft/main.h"

/* Error hook BATTLE calls, mostly when a unit lookup by id fails (code 12): report source 0 with
 * error_code through main_system_report_error, which is empty, and clear
 * g_main_system_graphics_error. */
void main_system_handle_pointer_exception(int error_code) {
    main_system_report_error(0, error_code);
    g_main_system_graphics_error = 0;
}
