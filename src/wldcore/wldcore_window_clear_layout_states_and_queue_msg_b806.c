#include "fft/wldcore.h"

/* Restore the bar list's palettes and cursor animation, then show its welcome text. */
void wldcore_window_clear_layout_states_and_queue_msg_b806(wldcore_menu_stack_record_t* level) {
    s32 index;
    s32 second;
    s32 slot;
    /* The original frame reserves eight unused bytes. */
    s32 pad[2];

    index = level->list_window.main_window;
    second = level->list_window.side_window;
    g_wldcore_window_render_records[level->list_window.frame_render].palette = 0;
    g_wldcore_window_records[second].palette = 0;
    g_wldcore_window_records[index].palette = 0;
    slot = level->list_window.main_window;
    g_wldcore_window_records[slot].sequence = WLDCORE_ANIM_SEQUENCE_CURSOR_IDLE;
    slot = level->list_window.main_window;
    g_wldcore_window_records[slot].anim_counter = 0;
    g_wldcore_window_records[slot].frame_index = 0;
    world_thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, WLDCORE_TEXT_ID_BAR_WELCOME, 0);
}
