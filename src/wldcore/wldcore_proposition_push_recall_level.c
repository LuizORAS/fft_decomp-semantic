#include "fft/thread.h"
#include "fft/wldcore.h"

/* Ask whether to call a proposition's units back (WLDCORE_MENU_LEVEL_PROPOSITION_RECALL): the
 * proposition's detail panel (wldcore_proposition_build_detail_panel_image) beside a yes/no panel,
 * over the hidden list, with the barkeeper's question (WLDCORE_TEXT_ID_CALL_BACK_PROMPT).
 *
 * Every level access re-indexes g_wldcore_menu_stack_depth rather than a
 * local copy, which is what keeps the depth in the target's registers. The
 * target reserves 0x30 bytes of frame the function never uses.
 *
 * The level is the type-0x13 proposition confirmation record
 * (see wldcore_proposition_handle_recall_input); wldcore_menu_stack_record_t has no member for it, so its fields are
 * reached through union members at the same offsets
 * (+0x10 proposition, +0x14 choice, +0x18 confirmed, +0x20 second window). */
void wldcore_proposition_push_recall_level(s32 proposition) {
    RECT bounds;
    u8 unused[0x30];
    s32 index;
    s32 depth;
    open_point32_t* base;

    wldcore_proposition_unpack_row(
        g_wldcore_selected_proposition_row, g_main_active_propositions[proposition].proposition_id);
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].proposition_cancel_confirm.proposition = proposition;
    g_wldcore_window_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 1]
                                 .window_pair_render.first_window]
        .flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
    g_wldcore_window_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 1]
                                 .window_pair_render.second_window]
        .flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
    g_wldcore_window_render_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 1]
                                        .window_pair_render.render_index]
        .flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
    thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, WLDCORE_TEXT_ID_CALL_BACK_PROMPT, 0);

    index = wldcore_window_append_record_and_reset_color(
        g_wldcore_window_render_object_queue, &g_wldcore_window_render_object_count);
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].proposition_cancel_confirm.cursor_window = index;
    g_wldcore_window_records[index].sequence = WLDCORE_ANIM_SEQUENCE_CURSOR_IDLE;
    g_wldcore_window_records[index].priority = 8;
    g_wldcore_window_records[index].anim_counter = 0;
    g_wldcore_window_records[index].frame_index = 0;

    index = wldcore_window_append_record_and_reset_color(
        g_wldcore_window_render_object_queue, &g_wldcore_window_render_object_count);
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].proposition_cancel_confirm.frame_window = index;
    g_wldcore_window_records[index].sequence = WLDCORE_ANIM_SEQUENCE_CHOICE_PANEL_HEADER;
    g_wldcore_window_records[index].priority = 8;
    g_wldcore_window_records[index].anim_counter = 0;
    g_wldcore_window_records[index].frame_index = 0;

    index = wldcore_window_append_render_record_and_reset_color(
        g_wldcore_window_render_object_queue, &g_wldcore_window_render_object_count);
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].proposition_cancel_confirm.choice_render = index;
    bounds.x = 0x80;
    bounds.w = 0x28;
    bounds.h = 0x30;
    bounds.y = 0;
    wldcore_window_init_render_record_image(index, bounds, 0, WLDCORE_TEXT_ID_OK_NO, g_wldcore_window_image_buffer);
    base = &g_wldcore_window_render_records[index].base;
    depth = g_wldcore_menu_stack_depth;
    g_wldcore_window_render_records[index].priority = 8;
    base->x = 0x48;
    base->y = 0x20;
    g_wldcore_window_records[g_wldcore_menu_stack_records_next[depth].proposition_cancel_confirm.cursor_window]
        .position.x = g_wldcore_window_render_records[index].base.x + 6;
    g_wldcore_window_records[g_wldcore_menu_stack_records_next[depth].proposition_cancel_confirm.cursor_window]
        .position.y = g_wldcore_window_render_records[index].base.y + 0xE;
    g_wldcore_window_records[g_wldcore_menu_stack_records_next[depth].proposition_cancel_confirm.frame_window]
        .position.x = g_wldcore_window_render_records[index].base.x + 3;
    g_wldcore_window_records[g_wldcore_menu_stack_records_next[depth].proposition_cancel_confirm.frame_window]
        .position.y = g_wldcore_window_render_records[index].base.y - 2;
    wldcore_proposition_build_detail_panel_image(&g_wldcore_menu_stack_records_next[depth], proposition, 0);
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].proposition_cancel_confirm.choice = 0;
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].proposition_cancel_confirm.confirmed = 0;
    g_wldcore_menu_stack_types[g_wldcore_menu_stack_depth + 1] = WLDCORE_MENU_LEVEL_PROPOSITION_RECALL;
    g_wldcore_menu_stack_depth++;
}
