#include "fft/wldcore.h"
#include "psx/pad.h"

/* Open the list of propositions on offer at this town (WLDCORE_MENU_LEVEL_AVAILABLE_PROPOSITIONS):
 * the ones neither in progress nor completed (their variable, EVENT_SCRIPT_VAR_PROPOSITION_FIRST + id),
 * not waiting out the days after a failure (g_main_proposition_states) and offered here
 * (wldcore_proposition_is_offered_at), as rows of TEXT_ID_PROPOSITION_NAME_BASE + id with six
 * visible. When the Bar's menu was hidden it is shown and rebuilt first. With none the barkeeper says
 * so (WLDCORE_TEXT_ID_NO_PROPOSITIONS) and the Bar's menu stays. */
void wldcore_proposition_open_available_list(void) {
    wldcore_point32_t dimensions;
    wldcore_point32_t origin;
    s32 count;
    s32 i;
    s32 index;
    wldcore_window_record_t* record;
    s32 record_index;

    if (g_wldcore_window_render_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 1]
                                            .list_window.frame_render]
            .flags
        & WLDCORE_DISPLAY_OBJECT_HIDDEN) {
        g_wldcore_window_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 1]
                                     .list_window.main_window]
            .flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
        g_wldcore_window_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 1]
                                     .list_window.side_window]
            .flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
        g_wldcore_window_render_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 1]
                                            .list_window.frame_render]
            .flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
        wldcore_bar_build_menu_rows(&g_wldcore_menu_stack_records[g_wldcore_menu_stack_depth].list_window);
        wldcore_window_publish_slot_b806(&g_wldcore_menu_stack_records[g_wldcore_menu_stack_depth].list_window);
        index = g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 1].list_window.main_window;
        record = &g_wldcore_window_records[index];
        record->anim_counter = 0;
        record->frame_index = 0;
        g_wldcore_window_records[index].sequence = WLDCORE_ANIM_SEQUENCE_CURSOR_STILL;
        g_wldcore_window_render_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 1]
                                            .list_window.frame_render]
            .flags |= WLDCORE_DISPLAY_OBJECT_POP_IN;
    }
    count = 0;
    for (i = 0; i < EVENT_SCRIPT_VAR_PROPOSITION_COUNT; i++) {
        if (!(world_script_get_variable(i + EVENT_SCRIPT_VAR_PROPOSITION_FIRST)
                & (WLDCORE_PROPOSITION_VARIABLE_IN_PROGRESS | WLDCORE_PROPOSITION_VARIABLE_COMPLETED))
            && !(g_main_proposition_states[i] & WLDCORE_PROPOSITION_STATE_DAYS_MASK)
            && wldcore_proposition_is_offered_at(i, g_wldcore_map_projection_state.marker.kind + 1) != 0) {
            g_wldcore_list_entry_values[count] = i + TEXT_ID_PROPOSITION_NAME_BASE;
            g_wldcore_list_row_flags[count] = 0;
            count++;
        }
    }
    if (count != 0) {
        world_thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, WLDCORE_TEXT_ID_PROPOSITIONS_INTRO, 0);
        g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].panel.entry_count = count;
        if (count < 6) {
            i = count;
        } else {
            i = 6;
        }
        dimensions.x = 0x96;
        origin.x = 0x4C;
        dimensions.y = i;
        origin.y = 0x50;
        wldcore_list_clamp_cursor_state(WLDCORE_CURSOR_SLOT_AVAILABLE_PROPOSITIONS, count, i);
        wldcore_window_start_panel_render_thread(WLDCORE_THREAD_SLOT_WINDOW_PANEL,
            g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].panel.entry_count, &dimensions, &origin);
    } else {
        wldcore_menu_dispatch_resume_handler();
        world_thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, WLDCORE_TEXT_ID_NO_PROPOSITIONS, 0);
        g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 1].list_window.content_render = 1;
        return;
    }
    record_index = g_wldcore_menu_stack_depth;
    g_wldcore_menu_ordering_table_offset = 9;
    g_wldcore_menu_blocked_buttons = PSX_PAD_CIRCLE | PSX_PAD_CROSS | PSX_PAD_SELECT;
    g_wldcore_menu_stack_records_next[record_index].panel.result = 0;
    g_wldcore_menu_stack_types[record_index + 1] = WLDCORE_MENU_LEVEL_AVAILABLE_PROPOSITIONS;
    g_wldcore_menu_stack_depth = record_index + 1;
}
