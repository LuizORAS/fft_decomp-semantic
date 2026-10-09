#include "fft/wldcore.h"

/* Declared with an s32 value here: the target passes the entry value
 * without narrowing it. */

/* Opens the detail view for proposition entry `value`: shows message
 * 0xb879 + value with a chance from its save-data state byte (10, 20 or 40
 * percent), otherwise message 0xb80c with substitution value 0xd000 + value;
 * unpacks the proposition row, builds a render record with text 0x8800 +
 * value and pushes it as a type-9 level, hiding the parent level's windows.
 *
 * The target stores base_x/base_y through one pointer to the pair. */
void wldcore_proposition_push_detail_level(s32 value) {
    s32 chance;
    s32 index;
    s32 depth;
    u8 state;
    wldcore_window_render_bounds16_t bounds;
    wldcore_point32_t* base;

    state = g_main_proposition_states[value];
    chance = (state & WLDCORE_PROPOSITION_STATE_FAILURE) ? 0x14 : 0;
    if (state & WLDCORE_PROPOSITION_STATE_BAD_FAILURE) {
        chance = 0x28;
    }
    if (!(state & (WLDCORE_PROPOSITION_STATE_FAILURE | WLDCORE_PROPOSITION_STATE_BAD_FAILURE))) {
        chance = 0xA;
    }
    if ((rand() * 100 >> 15) < chance) {
        world_thread_set_parameters(WORLD_THREAD_SLOT_WLDCORE_MESSAGE, 0x19, value + 0xB879, 0);
    } else {
        g_world_text_substitution_values[0] = value + 0xD000;
        world_thread_set_parameters(WORLD_THREAD_SLOT_WLDCORE_MESSAGE, 0x19, 0xB80C, 0);
    }
    wldcore_unpack_proposition_row(g_wldcore_selected_proposition_row, value);
    index = wldcore_window_append_render_record_and_reset_color(
        g_wldcore_window_render_object_queue, &g_wldcore_window_render_object_count);
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].window_render.render_index = index;
    bounds.position.y = 0x60;
    bounds.dimensions.x = 0xF4;
    bounds.dimensions.y = 0x80;
    bounds.position.x = 0;
    wldcore_window_init_render_record_image(index, bounds, 0, value + 0x8800, g_wldcore_window_image_buffer);
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
