#include "fft/battle_menu.h"
#include "fft/menu.h"

/* Put the row the active-turn unit last chose back into a menu entry.
 *
 * Only the first 12 menu ids of g_battle_menu_command_maps are tracked. A stored
 * row that one of the eight remap rows lists for this menu is first replaced by
 * that row's replacement. WORLD compiles this text too. */
void menu_restore_selection_for_selected_unit(s32 menu_id) {
    battle_menu_unit_selection_record_t* record;
    s32 i;
    s32 row;
    s32 candidate;
    battle_menu_selection_remap_t* remap;

    for (i = 0; i < 12; i++) {
        if (g_battle_menu_command_maps[i].menu_id == menu_id) {
            break;
        }
    }
    if (i == 12) {
        return;
    }
    record = &g_battle_menu_unit_selection_records[g_menu_selected_unit.battle_id];
    for (row = 0, remap = g_battle_menu_selection_remaps; row < 8; remap++, row++) {
        if (menu_id == g_battle_menu_selection_remaps[row].menu_id) {
            /* Keep the signed byte-table index used by the target. */
            for (candidate = 2; candidate < 5; candidate++) {
                if (record->command_index == remap->matches[candidate - 2]) {
                    record->command_index = g_battle_menu_selection_remaps[row].replacement;
                    break;
                }
            }
        }
    }
    g_menu_thread_menu_data[menu_id].selected_index = record->command_index;
}
