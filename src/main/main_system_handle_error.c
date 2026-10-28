#include "fft/main.h"

/* Error hook of the file loads, the sound file opens and BATTLE's sprite and unit-slot code:
 * report error_code through main_system_report_error_2 (source 0) and clear
 * g_main_system_graphics_error. */
void main_system_handle_error(int error_code) {
    main_system_report_error_2(0, error_code);
    g_main_system_graphics_error = 0;
}
