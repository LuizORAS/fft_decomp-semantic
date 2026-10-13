#include "fft/wldcore.h"
#include "psx/types.h"

/* Open the status help over a unit's status panels (level STATUS_HELP), which Select starts in a
 * participant's detail and in the send-unit status: a cursor window on the first field of
 * g_wldcore_status_help_fields, drawn at OT offset 1 (the previous offset is kept in the level).
 *
 * Writing the window columns as fields of g_wldcore_window_records (not separate stride arrays) keeps
 * the scheduler from sinking the record's window_index store below them. */
void wldcore_menu_push_status_help_level(void) {
    s32 window;

    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].status_help.saved_state
        = g_wldcore_menu_ordering_table_offset;
    g_wldcore_menu_ordering_table_offset = 1;
    window = wldcore_window_append_record_and_reset_color(
        g_wldcore_window_render_object_queue, &g_wldcore_window_render_object_count);
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].status_help.window_index = window;
    g_wldcore_window_records[window].sequence = WLDCORE_ANIM_SEQUENCE_STATUS_HELP_CURSOR;
    g_wldcore_window_records[window].priority = 2;
    g_wldcore_window_records[window].anim_counter = 0;
    g_wldcore_window_records[window].frame_index = 0;
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].status_help.field = 0;
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].status_help.cached_field_kind = 1;
    wldcore_window_move_status_help_cursor(&g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].status_help);
    g_wldcore_menu_stack_types[g_wldcore_menu_stack_depth + 1] = WLDCORE_MENU_LEVEL_STATUS_HELP;
    g_wldcore_menu_stack_depth++;
}
