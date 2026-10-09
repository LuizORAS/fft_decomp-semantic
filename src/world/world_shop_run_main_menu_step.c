#include "fft/world.h"

void world_shop_run_main_menu_step(void) {
    u8 running;

    if (g_world_shop_main_menu_running == 0) {
        g_world_formation_unit_cycle_mode = 0;
        g_world_menu_cursor_position = 0;
    }
    running = world_menu_run_thread(NATIVE_SCREEN_THREAD_SLOT_MENU, &g_world_shop_main_menu_thread_data);
    g_world_shop_main_menu_running = running;
    if (running == 0) {
        world_menu_clear_selection_records();
        if (g_world_menu_selection_results[0] == 0) {
            g_world_shop_menu_step = WORLD_SHOP_STEP_BUY_LIST;
            g_world_shop_item_category = world_menu_set_cursor_position_2(WORLD_MENU_CURSOR_SHOP_ITEM_TAB, 0);
        } else if (g_world_menu_selection_results[0] == 1) {
            g_world_shop_menu_step = WORLD_SHOP_STEP_SELL_LIST;
        } else if (g_world_menu_selection_results[0] == 2) {
            g_world_shop_menu_step = WORLD_SHOP_STEP_FITTING_ROOM;
            g_world_formation_selected_unit_index = 0;
            world_formation_stage_selected_unit();
            world_thread_set_parameters(NATIVE_SCREEN_THREAD_SLOT_MESSAGE, 0x19, 0xF818, 0);
        } else {
            g_world_shop_menu_step = WORLD_SHOP_STEP_EXIT_FADE;
            world_thread_set_parameters(NATIVE_SCREEN_THREAD_SLOT_MESSAGE, 0x19, 0xF803, 0);
        }
    }
    world_shop_run_obtain_gil_menu();
}
