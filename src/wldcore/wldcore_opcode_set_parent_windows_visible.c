#include "fft/wldcore.h"

/* Script opcode: operand byte 0 sets (non-zero) or clears state flag 0x40
 * and shows or hides both windows of the parent menu level (window flag
 * 0x10), then sets state flag 0x04. */
void wldcore_opcode_set_parent_windows_visible(void) {
    if ((u8)(g_wldcore_opcode_state.instruction >> 8) != 0) {
        g_wldcore_opcode_state.state_flags |= WLDCORE_OPCODE_STATE_PARENT_WINDOWS_VISIBLE;
        g_wldcore_window_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 1]
                                     .window_pair.first_window]
            .flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
        g_wldcore_window_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 1]
                                     .window_pair.second_window]
            .flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
    } else {
        g_wldcore_opcode_state.state_flags &= ~WLDCORE_OPCODE_STATE_PARENT_WINDOWS_VISIBLE;
        g_wldcore_window_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 1]
                                     .window_pair.first_window]
            .flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
        g_wldcore_window_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 1]
                                     .window_pair.second_window]
            .flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
    }
    g_wldcore_opcode_state.state_flags |= WLDCORE_OPCODE_STATE_FETCH;
}
