#include "fft/battle.h"
#include "fft/battle_map.h"
#include "fft/battle_menu.h"
#include "psx/types.h"

/* Per-frame body of the two Deep Dungeon mesh game states, which run while the
 * map reloads for a new crystal count: handle the free cursor, update the menus
 * and advance the map load. Once the load has finished, refresh the display of
 * misc units 0-15, clear g_map_deep_dungeon_load_in_progress and return 1;
 * until then return 0. */
s32 map_update_deep_dungeon_and_animation(void) {
    s32 misc_id;

    battle_state_handle_free_cursor_input();
    battle_menu_update_menus_and_event_speed(main_gfx_get_otag(), g_main_input_current_buttons);
    if (map_poll_data_load() != 0) {
        return 0;
    }
    misc_id = 0;
    do {
        battle_unit_misc_data_t* misc = battle_unit_get_misc_data_by_misc_id(misc_id & 0xFFFF);
        misc_id++;
        if (misc != 0) {
            battle_unit_update_display_by_misc_id(misc->unit_id);
        }
    } while (misc_id < 0x10);
    g_map_deep_dungeon_load_in_progress = 0;
    return 1;
}

/* padding */
