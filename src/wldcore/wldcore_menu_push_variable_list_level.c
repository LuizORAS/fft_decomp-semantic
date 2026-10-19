#include "fft/wldcore.h"

/* Open the debug script-variable list (level VARIABLE_LIST) that the world menu's debug row
 * requests: one cursor window and one render record, drawn by wldcore_menu_render_debug_variable_list
 * and then positioned from the render record's origin.
 *
 * Both appended indices share one local, which is what holds them in $a2. The window position is
 * written after wldcore_menu_render_debug_variable_list returns, from the render record origin the callee
 * filled in, so the level fields are read back out of the stack record rather than kept in locals; the
 * `- -` keeps the row offset's constant on the multiply chain, which a plain `+` reassociates onto
 * base_y. */
void wldcore_menu_push_variable_list_level(void) {
    s32 index;
    s32 record_index;
    u8 unused_locals[0x10];

    index = wldcore_window_append_record_and_reset_color(
        g_wldcore_window_render_object_queue, &g_wldcore_window_render_object_count);
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].variable_list.window_index = index;
    g_wldcore_window_records[index].sequence = WLDCORE_ANIM_SEQUENCE_CURSOR_IDLE;
    g_wldcore_window_records[index].priority = 3;
    g_wldcore_window_records[index].anim_counter = 0;
    g_wldcore_window_records[index].frame_index = 0;
    index = wldcore_window_append_render_record_and_reset_color(
        g_wldcore_window_render_object_queue, &g_wldcore_window_render_object_count);
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].variable_list.render_index = index;
    g_wldcore_window_render_records[index].flags |= WLDCORE_DISPLAY_OBJECT_POP_IN;
    g_wldcore_window_render_records[index].priority = 3;
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].variable_list.first_variable = 0;
    wldcore_menu_render_debug_variable_list(
        &g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].variable_list);
    record_index = g_wldcore_menu_stack_depth;
    g_wldcore_menu_stack_records_next[record_index].variable_list.cursor = 0;
    g_wldcore_window_records[g_wldcore_menu_stack_records_next[record_index].variable_list.window_index].position.x
        = g_wldcore_window_render_records[g_wldcore_menu_stack_records_next[record_index].variable_list.render_index]
              .base.x
        + 8;
    g_wldcore_menu_stack_depth = record_index + 1;
    g_wldcore_window_records[g_wldcore_menu_stack_records_next[record_index].variable_list.window_index].position.y
        = g_wldcore_window_render_records[g_wldcore_menu_stack_records_next[record_index].variable_list.render_index]
              .base.y
        - -(g_wldcore_menu_stack_records_next[record_index].variable_list.cursor * 0x10 + 0xC);
    g_wldcore_menu_stack_types[record_index + 1] = WLDCORE_MENU_LEVEL_VARIABLE_LIST;
}
