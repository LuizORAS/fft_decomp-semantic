/* Item-list menu thread for the formation item screen: builds the item
 * category list on entry, steps the cursor, and refreshes the stat preview.
 */
#include "fft/world.h"
#include "psx/pad.h"

s32 world_item_run_category_menu(void) {
    s16 x;
    s16 y;
    s32 input;
    s32 selected;
    s32 description;

    if (g_world_item_category_menu_initialized == 0) {
        g_world_item_category_menu_initialized = 1;
        world_script_set_vsync_mode_and_event_speed(2);
        g_world_item_category_previewed_cursor = -1;
        world_item_build_category_list(g_world_formation_selected_unit_index, (s16)(g_world_item_menu_category + 1),
            (s8)(g_world_item_menu_category != 4 ? g_world_item_menu_category + 1 : ITEM_LIST_FILTER_OTHER),
            g_world_menu_entry_ids, ITEM_LIST_MODE_ALL);
        world_menu_get_selection_record(g_world_item_menu_category, &x, &y, g_world_menu_entry_ids);
        world_menu_init_scrollable_list(
            g_world_menu_entry_ids, x, y, g_world_text_section_pointers[TEXT_SECTION_ITEM_NAMES]);
        g_world_menu_script_callbacks[0] = world_menu_get_entry_item_equipped_count;
        g_world_menu_script_callbacks[1] = world_menu_get_entry_item_count;
        g_world_menu_script_callbacks[2] = (s32 (*)(s32))world_item_build_type_icon_source;
        g_world_menu_script_callbacks[3] = world_menu_get_entry_item_icon_source;
        g_world_menu_script_callbacks[4] = world_menu_get_display_value_1;
        g_world_menu_script_callbacks[5] = world_menu_get_display_value_2;
        g_world_menu_script_callbacks[6] = world_menu_get_value_1;
        g_world_menu_script_callbacks[7] = world_menu_get_value_2;
    }
    world_menu_toggle_preview_stats_window(1);
    if (g_world_menu_option_count != 0) {
        description = g_world_menu_entry_ids[g_world_menu_cursor_position] + 0x6800;
    } else {
        description = -1;
    }
    input = g_world_input_primary_repeat;
    g_world_menu_description_text_id = description;
    if (input & PSX_PAD_CROSS) {
        world_menu_set_selection_record(g_world_item_menu_category, g_world_menu_cursor_position,
            g_world_menu_scroll_offset, g_world_menu_entry_ids);
        g_world_item_category_menu_initialized = 0;
        return MENU_ITEM_LIST_STATE_CLOSE;
    }
    selected = world_menu_update_horizontal_selection_and_mark_change(
        5, WORLD_MENU_CURSOR_ITEM_TAB, input, MAIN_SFX_PAGE_SWITCH);
    if (selected != g_world_item_menu_category) {
        world_menu_set_selection_record(g_world_item_menu_category, g_world_menu_cursor_position,
            g_world_menu_scroll_offset, g_world_menu_entry_ids);

        g_world_item_menu_category = selected;
        world_item_build_category_list(g_world_formation_selected_unit_index, (s16)(g_world_item_menu_category + 1),
            (s8)(g_world_item_menu_category != 4 ? g_world_item_menu_category + 1 : ITEM_LIST_FILTER_OTHER),
            g_world_menu_entry_ids, ITEM_LIST_MODE_ALL);
        g_world_item_category_previewed_cursor = -1;
        world_menu_get_selection_record(g_world_item_menu_category, &x, &y, g_world_menu_entry_ids);
        world_menu_init_scrollable_list(
            g_world_menu_entry_ids, x, y, g_world_text_section_pointers[TEXT_SECTION_ITEM_NAMES]);
        world_menu_set_window_scale_step(10);
    } else if (g_world_input_primary_repeat & PSX_PAD_CIRCLE) {
        if (g_world_menu_option_count != 0) {
            g_world_menu_sound_effect_id = MAIN_SFX_CONFIRM;
            world_menu_set_selection_record(
                selected, g_world_menu_cursor_position, g_world_menu_scroll_offset, g_world_menu_entry_ids);
            return MENU_ITEM_LIST_STATE_REARRANGE;
        }
        g_world_menu_sound_effect_id = MAIN_SFX_INVALID;
    } else if (g_world_input_primary_repeat & PSX_PAD_TRIANGLE) {
        g_world_menu_sound_effect_id = MAIN_SFX_CONFIRM;
        if (world_count_item_equipped_by_party(g_world_menu_entry_ids[g_world_menu_cursor_position]) != 0) {
            g_world_item_category_menu_initialized = 0;
            world_menu_set_selection_record(g_world_item_menu_category, g_world_menu_cursor_position,
                g_world_menu_scroll_offset, g_world_menu_entry_ids);
            return MENU_ITEM_LIST_STATE_EQUIPPED_BY;
        }
        g_world_menu_sound_effect_id = MAIN_SFX_INVALID;
    }
    if (g_world_menu_cursor_position != g_world_item_category_previewed_cursor) {
        if (g_world_menu_option_count != 0) {
            selected = g_world_menu_entry_ids[g_world_menu_cursor_position];
        } else {
            selected = 0;
        }
        g_world_item_category_previewed_cursor = g_world_menu_cursor_position;
        world_item_populate_stat_preview(
            selected, &g_world_selected_unit_stat_summary, &g_world_item_preview_stat_detail, 0);
        g_world_status_display_thread_params.redraw_request = 1;
        g_world_preview_stats_thread_params.redraw_request = 1;
    }
    return MENU_ITEM_LIST_STATE_BROWSE;
}
