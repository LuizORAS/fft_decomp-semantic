#include "fft/world.h"
#include "psx/types.h"

/* Shop step RENAME_NAME_ENTRY: the name keyboard for the chosen monster. The entered name is
 * stored at once (renaming costs nothing) and the office menu returns (0xF821). */
void world_shop_run_rename_name_entry_step(void) {
    if (g_world_shop_rename_name_entry_initialized == 0) {
        g_world_formation_unit_banner_enabled = 0;
        world_menu_toggle_unit_status_thread(0);
        g_world_shop_rename_name_entry_initialized = 1;
    }
    world_formation_update_and_draw_unit_grid(
        1, 0, 0, 0, g_world_menu_input_lock_mode, world_shop_rename_unit_get_unit_marker, 0);
    g_world_shop_entered_unit_name = world_name_run_entry_keyboard_frame(
        g_world_formation_unit_pointers[g_world_formation_selected_unit_index]->name, 0);
    if (g_world_shop_entered_unit_name != 0) {
        g_world_shop_menu_step = WORLD_SHOP_STEP_SOLDIER_OFFICE_REOPEN;
        world_formation_set_unit_name(g_world_formation_selected_unit_index, g_world_shop_entered_unit_name);
        world_thread_start(NATIVE_SCREEN_THREAD_SLOT_MESSAGE, world_text_message_box_thread);
        world_thread_set_parameters(NATIVE_SCREEN_THREAD_SLOT_MESSAGE, 0x19, 0xF821, 0);
        g_world_shop_background_visible = 1;
        g_world_shop_rename_name_entry_initialized = 0;
    }
}
