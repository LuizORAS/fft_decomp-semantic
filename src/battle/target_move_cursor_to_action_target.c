#include "fft/battle.h"
#include "fft/battle_target.h"

/* Move the cursor to the target of the unit's stored action: the target tile, or
 * the tile of the target unit. */
void target_move_cursor_to_action_target(const battle_unit_t* unit) {
    battle_stats_t* bd;

    bd = unit->battle_data;
    if (bd->action_command.targeting_type == BATTLE_TARGETING_TYPE_TILE) {
        g_battle_cursor_x = bd->action_command.target_position.vx;
        g_battle_cursor_z = unit->battle_data->action_command.target_position.vy;
        g_battle_cursor_y = unit->battle_data->action_command.target_position.vz;
        return;
    }
    target_move_cursor_to_unit(battle_unit_get_by_battle_id(bd->action_command.target_id));
}
