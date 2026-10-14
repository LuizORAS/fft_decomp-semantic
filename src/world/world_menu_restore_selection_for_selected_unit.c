#include "fft/world.h"
#include "psx/types.h"

/* Target 0x800f302c. When menu_index is in the 12-entry lookup table, remaps
 * the stored unit's row value through the matching remap record and copies
 * it into the menu entry's 0x38 field. */
void world_menu_restore_selection_for_selected_unit(s32 menu_index) {
    s32 i;
    s32 k;
    s32 m;
    menu_unit_selection_record_t* row;
    menu_selection_remap_t* remap;

    for (i = 0; i < 12; i++) {
        if (g_world_menu_command_maps[i].menu_id == menu_index) {
            break;
        }
    }
    if (i == 12) {
        return;
    }
    row = &g_world_menu_unit_selection_rows[g_world_active_unit_data.battle_id];
    for (k = 0, remap = g_world_menu_value_remap_table; k < 8; remap++, k++) {
        if (menu_index == g_world_menu_value_remap_table[k].menu_id) {
            /* Keep the signed index bound used by the target. */
            for (m = 2; m < 5; m++) {
                if (row->command_index == remap->matches[m - 2]) {
                    row->command_index = g_world_menu_value_remap_table[k].replacement;
                    break;
                }
            }
        }
    }
    g_menu_thread_menu_data[menu_index].selected_index = row->command_index;
}
