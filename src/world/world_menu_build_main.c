#include "fft/world.h"
#include "psx/types.h"

/* Menu thread of the main command menu: requests a redraw, runs the icon
 * selection loop and stores the chosen row in byte 0 of the active unit's
 * menu record. */
void world_menu_build_main(void) {
    world_menu_entry_t* entry;
    battle_menu_record_t* row;
    s32 unit_id;

    entry = world_thread_get_current_parameter_1();
    unit_id = g_world_active_unit_data.battle_id;
    row = &g_world_menu_unit_selection_rows[unit_id];
    g_world_thread_contexts[g_world_thread_current_id].function_parameter_2 = 1; /* redraw request */
    world_menu_run_icon_selection_loop();
    row->bytes[0] = entry->selected_index;
    world_thread_exit_current();
}
