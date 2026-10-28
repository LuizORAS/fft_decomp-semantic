#include "fft/thread.h"
#include "fft/world.h"
#include "psx/pad.h"

/* WORLD's version of battle_menu_should_close_thread. On Select, an entry with a select text table
 * also opens its announcement (world_menu_announce_entry_value_thread) in the text slot. */
s32 world_menu_should_close_thread(u32* input) {
    world_menu_entry_t* parameter;
    s32 i;

    if (g_thread_contexts[THREAD_SCREEN_SLOT_TEXT].is_running == 0 && world_menu_is_input_allowed() != 0
        && (*input & PSX_PAD_SELECT)) {
        parameter = (world_menu_entry_t*)world_thread_get_current_parameter_1();
        if (parameter->select_text_table != 0) {
            if (g_world_event_input_suppression_frames == 0) {
                thread_start(THREAD_SCREEN_SLOT_TEXT, world_menu_announce_entry_value_thread);
                thread_set_parameters(THREAD_SCREEN_SLOT_TEXT, (s32)parameter, 0, 0);
                g_menu_input_lock_mode = 1;
            }
            *input = 0;
        }
    }
    if (g_menu_input_lock_mode != 0 || g_world_menu_help_open != 0) {
        *input = 0;
        return 0;
    }
    for (i = 0; i < 17; i++) {
        if (i != g_thread_current_id && world_thread_is_running_asm(i) != 0
            && g_thread_contexts[i].task_id == THREAD_TASK_STOP_REQUEST) {
            *input = 0;
            break;
        }
    }
    if (i == 17 && world_thread_get_current_parameter_3() != 0) {
        return 1;
    }
    return 0;
}
