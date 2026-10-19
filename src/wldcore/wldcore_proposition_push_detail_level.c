#include "fft/thread.h"
#include "fft/wldcore.h"

/* Open a proposition's detail (WLDCORE_MENU_LEVEL_PROPOSITION_DETAIL): its description
 * (TEXT_ID_WORLD_MESSAGE_BASE + id) in a render record over the Bar's hidden windows, and a line from
 * the barkeeper: a hint about it (WLDCORE_TEXT_ID_PROPOSITION_HINT_FIRST + id) with a chance of 10 in
 * 100, 20 after it failed and 40 after it failed badly (g_main_proposition_states), otherwise the
 * plain introduction. The row is unpacked into g_wldcore_selected_proposition_row.
 *
 * Declared with an s32 value here: the target passes the entry value without
 * narrowing it. The target stores base_x/base_y through one pointer to the pair. */
void wldcore_proposition_push_detail_level(s32 value) {
    s32 chance;
    s32 index;
    s32 depth;
    u8 state;
    RECT bounds;
    open_point32_t* base;

    state = g_main_proposition_states[value];
    chance = (state & WLDCORE_PROPOSITION_STATE_FAILURE) ? 0x14 : 0;
    if (state & WLDCORE_PROPOSITION_STATE_BAD_FAILURE) {
        chance = 0x28;
    }
    if (!(state & (WLDCORE_PROPOSITION_STATE_FAILURE | WLDCORE_PROPOSITION_STATE_BAD_FAILURE))) {
        chance = 0xA;
    }
    if ((rand() * 100 >> 15) < chance) {
        thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, value + WLDCORE_TEXT_ID_PROPOSITION_HINT_FIRST, 0);
    } else {
        g_text_substitution_values[0] = value + TEXT_ID_PROPOSITION_NAME_BASE;
        thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, WLDCORE_TEXT_ID_PROPOSITION_ABOUT, 0);
    }
    wldcore_proposition_unpack_row(g_wldcore_selected_proposition_row, value);
    index = wldcore_window_append_render_record_and_reset_color(
        g_wldcore_window_render_object_queue, &g_wldcore_window_render_object_count);
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].window_render.render_index = index;
    bounds.y = 0x60;
    bounds.w = 0xF4;
    bounds.h = 0x80;
    bounds.x = 0;
    wldcore_window_init_render_record_image(
        index, bounds, 0, value + TEXT_ID_WORLD_MESSAGE_BASE, g_wldcore_window_image_buffer);
    base = &g_wldcore_window_render_records[index].base;
    g_wldcore_window_render_records[index].priority = 9;
    depth = g_wldcore_menu_stack_depth;
    base->x = -0x7A;
    base->y = -0x28;
    g_wldcore_window_records[g_wldcore_menu_stack_records_next[depth - 1].window_pair_render.first_window].flags
        |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
    g_wldcore_window_records[g_wldcore_menu_stack_records_next[depth - 1].window_pair_render.second_window].flags
        |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
    g_wldcore_window_render_records[g_wldcore_menu_stack_records_next[depth - 1].window_pair_render.render_index].flags
        |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
    g_wldcore_menu_stack_depth = depth + 1;
    g_wldcore_menu_stack_types_next[depth] = WLDCORE_MENU_LEVEL_PROPOSITION_DETAIL;
}
