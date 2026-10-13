#include "fft/world.h"
#include "psx/pad.h"
#include "psx/types.h"

/* The formation's List menu: the inventory by category (world_item_run_category_menu), with its
 * rearrange, action, sort order and "equipped by" modes, until the category menu closes (return 0).
 * Each frame it draws the Triangle button (enabled for an item the party has equipped), the list
 * window, the category tabs and, for the item under the cursor, its hand icons, HP/MP bonus window
 * and status icons.
 *
 * `busy = 0;` is written after the pressable-button call. The target
 * interleaves `move s0,zero` into that call's argument setup (after
 * `li a2,0x3c`, before `lui a3`); written before the call it is instead
 * emitted first, and reorg then steals it into the preceding `bnez` delay slot
 * and retargets the branch past its own slot instruction. `busy` is not an
 * argument to that call -- it is only read by the script call below -- so the
 * later position is the natural one. */
s32 world_formation_run_item_list_menu(void) {
    s32 input;
    s32 disabled;
    s32 busy;
    s32 unused[6]; /* never referenced; reproduces the 0x48 frame */

    if (g_world_item_submenu_initialized == 0) {
        g_world_item_submenu_initialized = 1;
        g_world_item_menu_category = world_menu_init_cursor_position(WORLD_MENU_CURSOR_ITEM_TAB, 0);
        world_thread_suspend(WORLD_THREAD_SLOT_UPPER_MENU);
        world_menu_request_thread_stop(NATIVE_SCREEN_THREAD_SLOT_UNIT_STATUS);
        world_menu_request_thread_stop(NATIVE_SCREEN_THREAD_SLOT_NUMERIC_EDITOR);
        world_menu_request_thread_stop(WORLD_THREAD_SLOT_EQUIP_ABILITY_PANEL);
        g_world_item_menu_mode = MENU_ITEM_LIST_STATE_BROWSE;
        g_world_formation_unit_banner_enabled = 0;
        g_world_preview_stats_thread_params.x = 0;
        g_world_preview_stats_thread_params.y = -0x49;
    }
    g_world_formation_cursor_ot_override = 9;
    if (g_world_item_menu_mode == MENU_ITEM_LIST_STATE_CLOSE) {
        world_menu_toggle_preview_stats_window(0);
        world_script_set_vsync_mode_and_event_speed(0);
        g_world_item_submenu_initialized = 0;
        g_world_formation_unit_banner_enabled = 1;
        world_thread_resume(WORLD_THREAD_SLOT_UPPER_MENU);
        g_world_preview_stats_thread_params.x = 2;
        g_world_preview_stats_thread_params.y = 2;
        g_world_formation_unit_cycle_mode = 2;
        g_world_menu_sound_effect_id = MAIN_SFX_CANCEL;
        return 0;
    }
    if (g_world_item_menu_mode == MENU_ITEM_LIST_STATE_BROWSE) {
        g_world_item_menu_mode = world_item_run_category_menu();
    } else if (g_world_item_menu_mode == MENU_ITEM_LIST_STATE_REARRANGE) {
        g_world_item_menu_mode = world_item_run_rearrange_mode();
    } else if (g_world_item_menu_mode == MENU_ITEM_LIST_STATE_ACTION) {
        g_world_formation_cursor_ot_override = 0x3C;
        g_world_item_menu_mode = world_item_run_action_menu();
    } else if (g_world_item_menu_mode == MENU_ITEM_LIST_STATE_SORT_ORDER) {
        g_world_item_menu_mode = world_item_run_sort_order_menu();
    } else if (g_world_item_menu_mode == MENU_ITEM_LIST_STATE_EQUIPPED_BY) {
        g_world_item_menu_mode = world_item_show_units_equipping_selected();
        return 1;
    }
    if (g_world_item_menu_mode == MENU_ITEM_LIST_STATE_EQUIPPED_BY) {
        return 1;
    }
    input = 0;
    if (g_world_item_menu_mode < 2) {
        input = g_world_input_primary_repeat;
        g_world_menu_prompt_active = 0;
    } else {
        g_world_menu_prompt_active = 1;
    }
    disabled = 0;
    if (world_menu_is_screen_busy() != 0 || g_world_item_menu_mode != MENU_ITEM_LIST_STATE_BROWSE
        || world_count_item_equipped_by_party(g_world_menu_entry_ids[g_world_menu_cursor_position]) == 0) {
        disabled = 1;
    }
    world_menu_draw_pressable_button(MENU_PRESSABLE_BUTTON_ITEM_LIST_TRIANGLE, 0xB8, 0x3C,
        g_world_input_primary_repeat & PSX_PAD_TRIANGLE, disabled, MAIN_SFX_CONFIRM);
    busy = 0;
    if (world_thread_is_task_active(0) != 0 || world_thread_is_running(NATIVE_SCREEN_THREAD_SLOT_MENU_CHILD) != 0) {
        busy = 1;
    }
    world_menu_run_script_with_palette_mode(g_world_item_list_window_script, input, busy);
    world_menu_draw_item_category_tabs(0, g_world_item_menu_category, busy);
    if (g_world_menu_option_count != 0) {
        if (g_world_menu_entry_ids[g_world_menu_cursor_position] < ITEM_ID_THROWABLE_FIRST) {
            world_item_draw_weapon_hand_icons(g_world_menu_entry_ids[g_world_menu_cursor_position]);
        }
        if (g_world_menu_entry_ids[g_world_menu_cursor_position] >= ITEM_ID_HEADGEAR_FIRST
            && g_world_menu_entry_ids[g_world_menu_cursor_position] < ITEM_ID_ACCESSORY_FIRST) {
            world_menu_run_script_with_palette_mode(g_world_item_hp_mp_bonus_window_script, 0, 0);
        }
        world_item_build_status_list_polygons(g_world_menu_entry_ids[g_world_menu_cursor_position]);
    }
    return 1;
}
