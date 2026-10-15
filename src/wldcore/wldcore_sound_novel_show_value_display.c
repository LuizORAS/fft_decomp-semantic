#include "fft/wldcore.h"

/* Sound novel opcode 28: show the numeric value display in mode operand 0 at (-0x20, 0x54); any mode
 * but the play time shows the record's counter. Opcode 29 takes it down. */
void wldcore_sound_novel_show_value_display(void) {
    s32 mode;

    g_main_system_flags &= ~MAIN_SYSTEM_FLAG_HIDE_CONTEXT_VALUE;
    mode = g_wldcore_sound_novel_record.instruction.bytes.operand_0;
    g_wldcore_sound_novel_record.value_display_mode = mode;
    g_wldcore_context_value_display_mode = mode;
    if (mode != WLDCORE_VALUE_DISPLAY_PLAY_TIME) {
        g_wldcore_displayed_numeric_value = g_wldcore_sound_novel_record.counter;
    }
    g_wldcore_context_value_display_position.x = -0x20;
    g_wldcore_context_value_display_position.y = 0x54;
    g_wldcore_hud_ot_priority = 0xa;
    g_wldcore_sound_novel_record.state_flags
        |= WLDCORE_SOUND_NOVEL_STATE_FETCH | WLDCORE_SOUND_NOVEL_STATE_VALUE_DISPLAY;
}
