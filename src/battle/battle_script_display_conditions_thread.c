#include "fft/battle.h"
#include "fft/event_require.h"
#include "psx/types.h"

/* Thread body of DisplayConditions (EVENT_OPCODE_DISPLAY_CONDITIONS): draw numeric values again and
 * run REQUIRE's condition dispatcher, which takes the display id and its operand from the thread
 * parameters. */
void battle_script_display_conditions_thread(void) {
    g_battle_menu_hide_numeric_values = 0;
    require_condition_dispatch();
}
