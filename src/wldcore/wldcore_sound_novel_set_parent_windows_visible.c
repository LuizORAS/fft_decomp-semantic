#include "fft/wldcore.h"

/* Sound novel opcode 12: show (nonzero operand) or hide both windows of the menu level under the
 * reader, keeping the choice in WLDCORE_SOUND_NOVEL_STATE_PARENT_WINDOWS_VISIBLE. */
void wldcore_sound_novel_set_parent_windows_visible(void) {
    if (g_wldcore_sound_novel_state.instruction.bytes.operand_0 != 0) {
        g_wldcore_sound_novel_state.state_flags |= WLDCORE_SOUND_NOVEL_STATE_PARENT_WINDOWS_VISIBLE;
        g_wldcore_window_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 1]
                                     .window_pair.first_window]
            .flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
        g_wldcore_window_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 1]
                                     .window_pair.second_window]
            .flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
    } else {
        g_wldcore_sound_novel_state.state_flags &= ~WLDCORE_SOUND_NOVEL_STATE_PARENT_WINDOWS_VISIBLE;
        g_wldcore_window_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 1]
                                     .window_pair.first_window]
            .flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
        g_wldcore_window_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 1]
                                     .window_pair.second_window]
            .flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
    }
    g_wldcore_sound_novel_state.state_flags |= WLDCORE_SOUND_NOVEL_STATE_FETCH;
}
