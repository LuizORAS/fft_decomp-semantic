#include "fft/battle.h"
#include "fft/event_require.h"
#include "psx/types.h"

/* Thread body of DisplayConditions: draw numeric values again and run REQUIRE's
 * condition dispatcher, which takes the display id and its operand from the
 * thread parameters. */
void battle_script_execute_display_conditions_instruction(void) {
    g_battle_menu_hide_numeric_values = 0;
    require_condition_dispatch();
}
