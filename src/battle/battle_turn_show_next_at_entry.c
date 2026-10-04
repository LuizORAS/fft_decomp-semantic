#include "fft/battle.h"
#include "psx/types.h"

/* Show the next AT list entry (Start in the free cursor) and step past it: the cursor on a
 * unit's turn, or the target panels of a charged action; empty entries are skipped. */
void battle_turn_show_next_at_entry(void) {
    s32 id;
    battle_unit_misc_data_t* unit;

    for (;;) {
        id = battle_turn_get_at_entry_unit(g_battle_turn_at_list_index);
        if (id >= 0) {
            unit = battle_unit_get_misc_data_by_battle_id(id & 0xFF);
            if ((id & 0x100) != 0) {
                battle_target_gather_x_y_data_for_attacks(unit);
                battle_target_calculate_for_menu_types(&unit->battle_data->action_actor_id);
                battle_target_set_tile_background_color(7, 3);
            } else {
                battle_target_move_cursor_to_unit(unit);
                battle_target_set_tile_background_color(0, 0);
            }
            break;
        }
        battle_turn_next_at_list_index();
    }
    battle_turn_next_at_list_index();
}
