#include "fft/battle.h"
#include "psx/types.h"

/* Show the next AT list entry and advance the AT list cursor past it.
 *
 * Empty entries are skipped. A charged action moves the cursor to its target and
 * marks the tiles it will hit; a unit's turn moves the cursor to the unit and
 * removes the tile marks. The free cursor's Start button and the status-menu
 * request call it. The loop has no exit when every entry is empty. */
void battle_action_is_at_list_unit_charging_ability(void) {
    s32 id;
    battle_unit_misc_data_t* unit;

    for (;;) {
        id = battle_action_calculate_at_list_and_get_specific_unit_id(g_battle_action_at_list_id);
        if (id >= 0) {
            unit = battle_unit_get_misc_data_by_battle_id(id & 0xFF);
            if ((id & 0x100) != 0) {
                battle_target_gather_x_y_data_for_attacks(unit);
                battle_target_calculate_for_menu_types((u8*)&unit->battle_data->action_command);
                battle_target_set_tile_background_color(BATTLE_TILE_MARK_SHOW_TARGETS, 3);
            } else {
                battle_target_move_cursor_to_unit(unit);
                battle_target_set_tile_background_color(BATTLE_TILE_MARK_NONE, 0);
            }
            break;
        }
        battle_action_increment_at_list_id();
    }
    battle_action_increment_at_list_id();
}
