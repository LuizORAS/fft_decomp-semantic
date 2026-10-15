#include "fft/wldcore.h"

/* Open a person's profile (menu level PERSON_DETAIL) for the Persons entry `value` (person * 16 +
 * the profile's variable value): load the chapter data of the person's group of 16, append a render
 * record, hide the Persons list's records and start WORLD thread 14 with text 0xb868 (person 0, the
 * value substituted) or 0xb849.
 *
 * The unused 48-byte local reproduces the target's frame size. */
void wldcore_menu_push_person_detail_level(s32 value) {
    u8 unused[0x30];
    s32 variable;
    s32 index;
    s32 depth;

    variable = value >> 4;
    wldcore_text_load_message_block_if_changed(variable / 16 + 6);
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].detail.value = value;
    index = wldcore_window_append_render_record_and_reset_color(
        g_wldcore_window_render_object_queue, &g_wldcore_window_render_object_count);
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].detail.render_index = index;
    g_wldcore_window_render_records[index].flags
        = (g_wldcore_window_render_records[index].flags & ~WLDCORE_DISPLAY_OBJECT_8BIT_CLUT)
        | WLDCORE_DISPLAY_OBJECT_HIDDEN;
    g_wldcore_window_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 1].list_window.main_window]
        .flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
    g_wldcore_window_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 1].list_window.side_window]
        .flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
    g_wldcore_window_render_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 1]
                                        .list_window.frame_render]
        .flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
    g_wldcore_window_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 1].list_window.upper_window]
        .flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
    g_wldcore_window_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 1].list_window.lower_window]
        .flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
    if (variable == 0) {
        g_world_text_substitution_values[0] = (value & 0xF) + WLDCORE_TEXT_ID_RAMZA_TITLE_FIRST;
        world_thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, WLDCORE_TEXT_ID_RAMZA_INFO, 0);
    } else {
        g_world_text_substitution_values[0] = value + TEXT_ID_PERSON_PROFILE_NAME_BASE;
        world_thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, WLDCORE_TEXT_ID_BRAVE_STORY_ENTRY_INFO, 0);
    }
    depth = g_wldcore_menu_stack_depth;
    g_wldcore_menu_stack_records_next[depth].detail.phase = WLDCORE_DETAIL_PHASE_LOAD;
    g_wldcore_menu_stack_records_next[depth].detail.timer = 0;
    g_wldcore_menu_stack_types[depth + 1] = WLDCORE_MENU_LEVEL_PERSON_DETAIL;
    g_wldcore_menu_stack_depth = depth + 1;
}
