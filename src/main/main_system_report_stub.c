#include "fft/main.h"

/* Empty report hook. Callers pass a source number (0 from main_system_handle_error, 1 the effect
 * code, 7 the menu's system functions, 0x11 the dead-unit menu) and a code. */
void main_system_report_stub(s32 category, s32 code) {
}
