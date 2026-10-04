#include "fft/main.h"

/* Report error_code through main_system_report_error_2 (category 0) and clear
 * g_main_system_graphics_error. */
void main_system_handle_animation_exception(int error_code) {
    main_system_report_error_2(0, error_code);
    g_main_system_graphics_error = 0;
}
