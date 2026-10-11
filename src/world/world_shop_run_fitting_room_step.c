#include "fft/world.h"
#include "psx/libc.h"
#include "psx/pad.h"
#include "psx/types.h"

/* Shop step FITTING_ROOM. On entry (after any button) it clears the fitting room: no tried-on
 * item, no kept item, a cost of 0. Then the unit grid: Circle or Triangle picks a unit and opens the
 * fitting menu, X goes to the checkout (FITTING_ROOM_CHECKOUT). The fitting menu's rows go to Try it
 * on (FITTING_ROOM_EQUIP), Best fit, Quit fitting (FITTING_ROOM_REMOVE), Sell (then Sell equip or
 * Stock) and Fitting over (the checkout); cancel returns to the unit grid. The cost window shows while
 * a unit is chosen. */
void world_shop_run_fitting_room_step(void) {
    s32 busy;

    if (g_world_shop_fitting_room_initialized == 0) {
        if (g_world_input_newly_pressed == 0) {
            world_shop_run_gil_menu_script();
            return;
        }
        world_formation_init_cursor_and_scroll_state(0);
        memset(g_world_shop_fitting_room_items, 0, 0xC8);
        world_shop_add_fitting_room_cost(-world_shop_add_fitting_room_cost(0));
        g_world_shop_fitting_room_unit_chosen = 0;
        g_world_shop_cost_window_visible = 0;
        world_thread_set_parameters(NATIVE_SCREEN_THREAD_SLOT_MESSAGE, 0x19, -1, 0);
        g_world_shop_fitting_room_initialized = 1;
        return;
    }
    if (g_world_shop_fitting_room_unit_chosen == 0) {
        g_world_shop_background_visible = 0;
        if (g_world_formation_unit_banner_enabled != 0) {
            g_world_menu_description_text_id = (g_world_formation_unit_banner_at_bottom != 0)
                ? MENU_GRID_HELP_BANNER_BOTTOM
                : MENU_GRID_HELP_BANNER_TOP;
        } else {
            g_world_menu_description_text_id = 0;
        }
        busy = world_input_get_lockout_timer();
        busy += world_formation_is_unit_list_scroll_needed();
        if ((g_world_input_primary_repeat & PSX_PAD_CROSS) && busy == 0) {
            g_world_menu_sound_effect_id = MAIN_SFX_CANCEL;
            g_world_formation_unit_banner_enabled = 0;
            g_world_shop_menu_step = WORLD_SHOP_STEP_FITTING_ROOM_CHECKOUT;
        } else if ((g_world_input_primary_repeat & (PSX_PAD_TRIANGLE | PSX_PAD_CIRCLE)) && busy == 0) {
            g_world_formation_unit_cycle_mode = 2;
            g_world_menu_sound_effect_id = MAIN_SFX_CONFIRM;
            g_world_shop_fitting_room_unit_chosen = 1;
            g_world_formation_unit_browse_enabled = 1;
            g_world_formation_unit_banner_at_bottom = 0;
            g_world_shop_cost_window_visible = 1;
            g_world_shop_fitting_room_menu_thread_data[0].selected_index = 0;
            g_world_shop_fitting_room_menu_thread_data[1].selected_index = 0;
            world_menu_clear_selection_records();
        } else {
            world_formation_update_and_draw_unit_grid(1, 1, 0, g_world_input_secondary_repeat,
                g_world_thread_task_active, world_shop_fitting_room_get_unit_marker, 0);
        }
    } else {
        g_world_shop_background_visible = 1;
        g_world_menu_description_text_id = 0;
        if (world_menu_run_thread(NATIVE_SCREEN_THREAD_SLOT_MENU, g_world_shop_fitting_room_menu_thread_data) == 0) {
            if (g_world_menu_selection_results[1] == 0) {
                g_world_shop_menu_step = WORLD_SHOP_STEP_FITTING_ROOM_EQUIP;
            } else if (g_world_menu_selection_results[1] == 1) {
                g_world_shop_menu_step = WORLD_SHOP_STEP_FITTING_ROOM_BEST_FIT;
            } else if (g_world_menu_selection_results[1] == 2) {
                g_world_shop_menu_step = WORLD_SHOP_STEP_FITTING_ROOM_REMOVE;
            } else if (g_world_menu_selection_results[1] == 3) {
                if (g_world_menu_selection_results[2] == 0) {
                    g_world_shop_menu_step = WORLD_SHOP_STEP_FITTING_ROOM_SELL_EQUIPPED;
                } else {
                    g_world_shop_menu_step = WORLD_SHOP_STEP_FITTING_ROOM_SELL;
                }
            } else {
                g_world_shop_fitting_room_unit_chosen = 0;
                world_formation_init_selection_cursor(g_world_formation_selected_unit_index);
                g_world_formation_unit_cycle_mode = 0;
                g_world_formation_unit_browse_enabled = 0;
                g_world_shop_cost_window_visible = 0;
                if (g_world_menu_selection_results[1] == 4) {
                    g_world_formation_unit_banner_enabled = 0;
                    g_world_shop_menu_step = WORLD_SHOP_STEP_FITTING_ROOM_CHECKOUT;
                }
            }
            world_menu_set_cursor_position(WORLD_MENU_CURSOR_REMOVE_SLOT, 0);
            world_menu_set_cursor_position(WORLD_MENU_CURSOR_EQUIP_SLOT, 0);
        }
    }
    if (g_world_shop_cost_window_visible != 0) {
        world_shop_run_fitting_cost_menu_script();
    }
}
