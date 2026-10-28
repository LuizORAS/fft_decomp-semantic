#include "fft/event_require.h"
#include "fft/menu.h"
#include "fft/script.h"

/* Thread body of DisplayConditions (EVENT_OPCODE_DISPLAY_CONDITIONS): draw numeric values again and
 * run REQUIRE's condition dispatcher, which takes the display id and its operand from the thread
 * parameters. */
void script_display_conditions_thread(void) {
    g_menu_hide_numeric_values = 0;
    require_condition_dispatch();
}
