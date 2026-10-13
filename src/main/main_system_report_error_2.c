#include "fft/main.h"

/* Second empty error hook, with the same (source, code) arguments as main_system_report_error.
 * Callers pass source 0 (main_system_handle_error), 1 (effect code), 7 (the menus' system
 * functions) or 0x11 (the crystal pickup menu). */
void main_system_report_error_2(s32 category, s32 code) {
}
