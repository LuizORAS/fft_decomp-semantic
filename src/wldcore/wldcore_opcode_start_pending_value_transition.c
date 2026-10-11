#include "fft/wldcore.h"

/* Sound-novel opcode: show the context value display in the mode of operand
 * 0 at (-0x20, 0x54). Any mode but the play time shows the record's counter. */
void wldcore_opcode_start_pending_value_transition(void) {
    s32 mode;

    g_main_system_flags &= ~MAIN_SYSTEM_FLAG_HIDE_CONTEXT_VALUE;
    mode = (u8)(g_wldcore_active_saved_record.instruction >> 8);
    g_wldcore_active_saved_record.value_display_mode = mode;
    g_wldcore_context_value_display_mode = mode;
    if (mode != WLDCORE_VALUE_DISPLAY_PLAY_TIME) {
        g_wldcore_displayed_numeric_value = g_wldcore_active_saved_record.counter;
    }
    g_wldcore_context_value_display_position.x = -0x20;
    g_wldcore_context_value_display_position.y = 0x54;
    g_wldcore_hud_ot_priority = 0xa;
    g_wldcore_active_saved_record.state_flags |= WLDCORE_OPCODE_STATE_FETCH | WLDCORE_OPCODE_STATE_VALUE_DISPLAY;
}
