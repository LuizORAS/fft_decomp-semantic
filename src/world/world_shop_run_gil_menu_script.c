#include "fft/world.h"

/* Draw the shop's gil window: run g_world_shop_gil_menu_script with world_shop_add_gil as
 * menu value callback 0, then restore the previous callback. Most shop steps call it every frame; twin
 * of world_shop_run_soldier_office_fee_menu_script. */
void world_shop_run_gil_menu_script(void) {
    menu_callback_t* callback_slot;
    s32 (*previous_callback)(s32);

    callback_slot = g_world_menu_script_callbacks;
    previous_callback = callback_slot->value;
    callback_slot->value = world_shop_add_gil;
    world_menu_run_script_with_palette_mode(&g_world_shop_gil_menu_script, 0, 0);
    callback_slot->value = previous_callback;
}
