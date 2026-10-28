#include "fft/thread.h"
#include "fft/wldcore.h"

/* Open the Job section's Advancing list (level ACTIVE_PROPOSITIONS): one row per proposition in
 * progress (g_main_save_proposition_count) with its name (text 0xd000 | proposition id), the number of
 * units sent (text 0xb866) and the days left (text 0xb867, assigned minus elapsed days), a cursor
 * window and three column-header windows, and the saved cursor row
 * (g_wldcore_active_propositions_saved_cursor) clamped to the list. WORLD thread 14 shows the Brave
 * Story's prompt, and the Job section's cursor window and list come back if hidden.
 *
 * The 0xb8 frame carries 0x50 bytes of locals the original never reads; `dead_locals` reproduces
 * them.
 *
 * The seeding loop's entry test is spelled `i < count` (the target's signed `blez`) around a
 * do-while that re-reads the u8 count, which gives the hoisted bound copy the target keeps in $a0.
 * The cursor row is folded as base_y - row * -16 + 14 to keep base_y as the first addu operand.
 */
void wldcore_list_open_active_propositions(void) {
    open_point32_t point;
    RECT bounds;
    open_point32_t* base;
    s32 index;
    s32 depth;
    s32 i;
    s32 temp;
    s32 dead_locals[20];

    thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, WLDCORE_TEXT_ID_BRAVE_STORY_INTRO, 0);

    if (g_wldcore_window_render_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 1]
                                            .proposition_list.title_window]
            .flags
        & WLDCORE_DISPLAY_OBJECT_HIDDEN) {
        g_wldcore_window_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 1]
                                     .proposition_list.cursor_window]
            .flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
        g_wldcore_window_render_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 1]
                                            .proposition_list.title_window]
            .flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
        g_wldcore_window_render_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 1]
                                            .proposition_list.title_window]
            .flags |= WLDCORE_DISPLAY_OBJECT_POP_IN;
    }

    index = wldcore_window_append_record_and_reset_color(
        g_wldcore_window_render_object_queue, &g_wldcore_window_render_object_count);
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].proposition_list.cursor_window = index;
    g_wldcore_window_records[index].sequence = WLDCORE_ANIM_SEQUENCE_CURSOR_IDLE;
    g_wldcore_window_records[index].priority = 8;
    g_wldcore_window_records[index].anim_counter = 0;
    g_wldcore_window_records[index].frame_index = 0;

    i = 0;
    index = wldcore_window_append_record_and_reset_color(
        g_wldcore_window_render_object_queue, &g_wldcore_window_render_object_count);
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].proposition_list.title_window = index;
    g_wldcore_window_records[index].sequence = WLDCORE_ANIM_SEQUENCE_PROPOSITION_TITLE_LABEL;
    g_wldcore_window_records[index].priority = 8;
    g_wldcore_window_records[index].anim_counter = 0;
    g_wldcore_window_records[index].frame_index = 0;

    index = wldcore_window_append_record_and_reset_color(
        g_wldcore_window_render_object_queue, &g_wldcore_window_render_object_count);
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].proposition_list.count_window = index;
    g_wldcore_window_records[index].sequence = WLDCORE_ANIM_SEQUENCE_PROPOSITION_COUNT_LABEL;
    g_wldcore_window_records[index].priority = 8;
    g_wldcore_window_records[index].anim_counter = 0;
    g_wldcore_window_records[index].frame_index = 0;

    index = wldcore_window_append_record_and_reset_color(
        g_wldcore_window_render_object_queue, &g_wldcore_window_render_object_count);
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].proposition_list.days_window = index;
    g_wldcore_window_records[index].sequence = WLDCORE_ANIM_SEQUENCE_PROPOSITION_DAYS_LABEL;
    g_wldcore_window_records[index].priority = 8;
    g_wldcore_window_records[index].anim_counter = 0;
    g_wldcore_window_records[index].frame_index = 0;
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].proposition_list.entry_count = 0;
    while (i < g_main_save_proposition_count) {
        g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth]
            .proposition_list
            .entries[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].proposition_list.entry_count] = i;
        i++;
        g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].proposition_list.entry_count++;
    }

    i = 0;
    index = wldcore_window_append_render_record_and_reset_color(
        g_wldcore_window_render_object_queue, &g_wldcore_window_render_object_count);
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].proposition_list.render_index = index;
    bounds.y = 0x40;
    bounds.x = 0;
    bounds.w = 0xD8;
    bounds.h = g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].proposition_list.entry_count * 0x10 + 0x10;
    wldcore_window_build_render_record_image(index, bounds, 2, g_wldcore_window_image_buffer);
    g_wldcore_window_render_records[index].priority = 8;
    g_wldcore_window_render_records[index].flags |= WLDCORE_DISPLAY_OBJECT_POP_IN;
    base = &g_wldcore_window_render_records[index].base;
    base->x = -0x60;
    base->y = -0x28;

    point.x = 8;
    point.y = 8;
    while (i < g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].proposition_list.entry_count) {
        wldcore_menu_display_text_entry(index,
            g_main_active_propositions[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth]
                                           .proposition_list.entries[i]]
                    .proposition_id
                | TEXT_ID_PROPOSITION_NAME_BASE,
            point, g_wldcore_window_image_buffer);
        point.y += 0x10;
        i++;
    }

    point.x = 0x9C;
    point.y = 8;
    i = 0;
    while (i < g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].proposition_list.entry_count) {
        g_text_substitution_values[0]
            = g_main_active_propositions[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth]
                                             .proposition_list.entries[i]]
                  .participant_count;
        wldcore_menu_display_text_entry(index, WLDCORE_TEXT_ID_VALUE, point, g_wldcore_window_image_buffer);
        point.y += 0x10;
        i++;
    }

    point.x = 0xB6;
    point.y = 8;
    i = 0;
    while (i < g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].proposition_list.entry_count) {
        s32 entry = g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].proposition_list.entries[i];
        g_text_substitution_values[0]
            = g_main_active_propositions[entry].assigned_days - g_main_active_propositions[entry].elapsed_days;
        wldcore_menu_display_text_entry(index, WLDCORE_TEXT_ID_TWO_DIGIT_VALUE, point, g_wldcore_window_image_buffer);
        point.y += 0x10;
        i++;
    }

    wldcore_window_load_image_record_to_vram(index, g_wldcore_window_image_buffer);

    depth = g_wldcore_menu_stack_depth;
    /* temp spans two blocks without crossing a call, so global allocation
     * gives the clamp flag and the cursor copy the target's v1. */
    temp = g_wldcore_menu_stack_records_next[depth].proposition_list.entry_count
        < g_wldcore_active_propositions_saved_cursor[0] + 1;
    if (temp) {
        g_wldcore_active_propositions_saved_cursor[0] = 0;
    }
    temp = g_wldcore_active_propositions_saved_cursor[0];
    g_wldcore_menu_stack_records_next[depth].proposition_list.cursor = temp;
    g_wldcore_window_records[g_wldcore_menu_stack_records_next[depth].proposition_list.cursor_window].position.x
        = g_wldcore_window_render_records[index].base.x + 6;
    g_wldcore_window_records[g_wldcore_menu_stack_records_next[depth].proposition_list.cursor_window].position.y
        = g_wldcore_window_render_records[index].base.y - g_wldcore_active_propositions_saved_cursor[0] * -0x10 + 0xE;
    g_wldcore_window_records[g_wldcore_menu_stack_records_next[depth].proposition_list.title_window].position.x
        = g_wldcore_window_render_records[index].base.x + 3;
    g_wldcore_window_records[g_wldcore_menu_stack_records_next[depth].proposition_list.title_window].position.y
        = g_wldcore_window_render_records[index].base.y - 2;
    g_wldcore_window_records[g_wldcore_menu_stack_records_next[depth].proposition_list.count_window].position.x
        = g_wldcore_window_render_records[index].base.x + 0x94;
    g_wldcore_window_records[g_wldcore_menu_stack_records_next[depth].proposition_list.count_window].position.y
        = g_wldcore_window_render_records[index].base.y - 2;
    g_wldcore_window_records[g_wldcore_menu_stack_records_next[depth].proposition_list.days_window].position.x
        = g_wldcore_window_render_records[index].base.x + 0xB2;
    g_wldcore_menu_stack_depth = depth + 1;
    g_wldcore_window_records[g_wldcore_menu_stack_records_next[depth].proposition_list.days_window].position.y
        = g_wldcore_window_render_records[index].base.y - 2;
    g_wldcore_menu_stack_types[depth + 1] = WLDCORE_MENU_LEVEL_ACTIVE_PROPOSITIONS;
}
