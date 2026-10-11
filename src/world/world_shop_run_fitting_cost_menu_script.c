#include "fft/world.h"

/* Draw the fitting room's cost window: run g_world_shop_fitting_cost_menu_script with its
 * three menu value callbacks installed (gil, fitting cost, gil minus cost); the script gets no input
 * while a menu is busy. */
void world_shop_run_fitting_cost_menu_script(void) {
    g_world_menu_script_callbacks[0] = world_shop_add_gil;
    g_world_menu_script_callbacks[1] = world_shop_add_fitting_room_cost;
    g_world_menu_script_callbacks[2] = world_shop_get_gil_minus_fitting_room_cost;
    world_menu_run_script_with_palette_mode(
        g_world_shop_fitting_cost_menu_script, g_world_input_primary_repeat, world_menu_is_busy(0));
}
