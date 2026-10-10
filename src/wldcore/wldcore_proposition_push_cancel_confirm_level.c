#include "fft/wldcore.h"

/* Pushes the proposition confirmation level (type 0x13) for active
 * proposition `proposition`: unpacks its proposition row, hides the parent
 * level's two windows and render record, shows message 0xb81d, and builds a
 * two-row cursor/frame window pair over render record text 0xb805 beside the
 * proposition detail panel.
 *
 * Every level access re-indexes g_wldcore_menu_stack_depth rather than a
 * local copy, which is what keeps the depth in the target's registers. The
 * target reserves 0x30 bytes of frame the function never uses.
 *
 * Provisional: the level is the type-0x13 proposition confirmation record
 * (see wldcore_proposition_handle_cancel_confirm_input); until wldcore_menu_stack_record_t has a member for it,
 * its words are reached through union members at the same offsets
 * (+0x10 proposition, +0x14 choice, +0x18 confirmed, +0x20 second window). */
void wldcore_proposition_push_cancel_confirm_level(s32 proposition) {
    wldcore_window_render_bounds16_t bounds;
    u8 unused[0x30];
    s32 index;
    s32 depth;
    wldcore_point32_t* base;

    wldcore_unpack_proposition_row(
        g_wldcore_selected_proposition_row, g_main_active_propositions[proposition].proposition_id);
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].participant.proposition = proposition;
    g_wldcore_window_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 1]
                                 .window_pair_render.first_window]
        .flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
    g_wldcore_window_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 1]
                                 .window_pair_render.second_window]
        .flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
    g_wldcore_window_render_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 1]
                                        .window_pair_render.render_index]
        .flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
    world_thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, WLDCORE_TEXT_ID_CALL_BACK_PROMPT, 0);

    index = wldcore_window_append_record_and_reset_color(
        g_wldcore_window_render_object_queue, &g_wldcore_window_render_object_count);
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].window_index = index;
    g_wldcore_window_records[index].sequence = WLDCORE_ANIM_SEQUENCE_CURSOR_IDLE;
    g_wldcore_window_records[index].priority = 8;
    g_wldcore_window_records[index].anim_counter = 0;
    g_wldcore_window_records[index].frame_index = 0;

    index = wldcore_window_append_record_and_reset_color(
        g_wldcore_window_render_object_queue, &g_wldcore_window_render_object_count);
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].list_window.upper_window = index;
    g_wldcore_window_records[index].sequence = WLDCORE_ANIM_SEQUENCE_CHOICE_PANEL_HEADER;
    g_wldcore_window_records[index].priority = 8;
    g_wldcore_window_records[index].anim_counter = 0;
    g_wldcore_window_records[index].frame_index = 0;

    index = wldcore_window_append_render_record_and_reset_color(
        g_wldcore_window_render_object_queue, &g_wldcore_window_render_object_count);
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].window_render.render_index = index;
    bounds.position.x = 0x80;
    bounds.dimensions.x = 0x28;
    bounds.dimensions.y = 0x30;
    bounds.position.y = 0;
    wldcore_window_init_render_record_image(index, bounds, 0, WLDCORE_TEXT_ID_OK_NO, g_wldcore_window_image_buffer);
    base = &g_wldcore_window_render_records[index].base;
    depth = g_wldcore_menu_stack_depth;
    g_wldcore_window_render_records[index].priority = 8;
    base->x = 0x48;
    base->y = 0x20;
    g_wldcore_window_records[g_wldcore_menu_stack_records_next[depth].window_index].position.x
        = g_wldcore_window_render_records[index].base.x + 6;
    g_wldcore_window_records[g_wldcore_menu_stack_records_next[depth].window_index].position.y
        = g_wldcore_window_render_records[index].base.y + 0xE;
    g_wldcore_window_records[g_wldcore_menu_stack_records_next[depth].list_window.upper_window].position.x
        = g_wldcore_window_render_records[index].base.x + 3;
    g_wldcore_window_records[g_wldcore_menu_stack_records_next[depth].list_window.upper_window].position.y
        = g_wldcore_window_render_records[index].base.y - 2;
    wldcore_list_build_proposition_detail_panel_image(
        &g_wldcore_menu_stack_records_next[depth].window_pair_render, proposition, 0);
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].participant.participant = 0;
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].send_unit.cursor_window = 0;
    g_wldcore_menu_stack_types[g_wldcore_menu_stack_depth + 1] = WLDCORE_MENU_LEVEL_PROPOSITION_CANCEL_CONFIRM;
    g_wldcore_menu_stack_depth++;
}
