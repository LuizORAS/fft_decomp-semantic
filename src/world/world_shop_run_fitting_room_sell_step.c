#include "fft/thread.h"
#include "fft/world.h"

/* Shop step FITTING_ROOM_SELL (Sell, then Stock): run the shop's SELL_LIST and SELL_CONFIRM
 * steps inside the fitting room. On entry it opens the message box, hides the unit browser and banner
 * and clears selection records 5-9. Each frame it runs the remembered sub-step with
 * g_world_shop_menu_step set to it, keeps the step it moves to in g_world_shop_sell_subflow_step and
 * stays at FITTING_ROOM_SELL, until the sell list's exit to MAIN_MENU sends it back to FITTING_ROOM. */
void world_shop_run_fitting_room_sell_step(void) {
    if (g_world_shop_sell_subflow_initialized == 0) {
        if (thread_is_running(THREAD_SCREEN_SLOT_MESSAGE) != 0) {
            return;
        }
        thread_start(THREAD_SCREEN_SLOT_MESSAGE, world_text_message_box_thread);
        g_world_formation_unit_browse_enabled = 0;
        g_world_formation_unit_banner_enabled = 0;
        world_formation_stop_menu_threads();
        g_world_formation_unit_cycle_mode = 0;
        g_world_shop_sell_subflow_step = WORLD_SHOP_STEP_SELL_LIST;
        world_menu_clear_selection_record(5);
        world_menu_clear_selection_record(6);
        world_menu_clear_selection_record(7);
        world_menu_clear_selection_record(8);
        world_menu_clear_selection_record(9);
        g_world_shop_sell_subflow_initialized = 1;
    }
    g_world_shop_menu_step = g_world_shop_sell_subflow_step;
    if (g_world_shop_sell_subflow_step == WORLD_SHOP_STEP_SELL_LIST) {
        world_shop_run_sell_list_step();
        if (g_world_shop_sell_subflow_step != g_world_shop_menu_step) {
            world_menu_reset_selection_results();
        }
    } else if (g_world_shop_sell_subflow_step == WORLD_SHOP_STEP_SELL_CONFIRM) {
        world_shop_run_sell_confirm_step();
    }
    if (g_world_shop_menu_step == WORLD_SHOP_STEP_MAIN_MENU) {
        g_world_shop_menu_step = WORLD_SHOP_STEP_FITTING_ROOM;
        g_world_formation_unit_cycle_mode = 2;
        g_world_shop_sell_subflow_initialized = 0;
        g_world_formation_unit_browse_enabled = 1;
        world_script_set_vsync_mode_and_event_speed(2);
        thread_set_parameters(THREAD_SCREEN_SLOT_MESSAGE, 0x19, -1, 0);
        return;
    }
    g_world_shop_sell_subflow_step = g_world_shop_menu_step;
    g_world_shop_menu_step = WORLD_SHOP_STEP_FITTING_ROOM_SELL;
}
