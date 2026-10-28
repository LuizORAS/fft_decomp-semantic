#include "fft/battle_action.h"
#include "fft/battle_target.h"

/* Store where the strike lands in the strike work (target_new_x, target_new_y,
 * target_new_map_level, which a knockback reads): the Random Fire tile, the target unit's square for
 * a unit target, else the target tile. Also clears skip_empty_tile_effect. */
void battle_target_store_strike_destination(const battle_stats_t* acting, battle_strike_work_t* out) {
    battle_stats_t* target;

    out->skip_empty_tile_effect = 0;
    if (g_battle_action_current_ability.random_fire_flag != 0) {
        out->target_new_x = g_battle_action_current_ability.target_new_x;
        out->target_new_y = g_battle_action_current_ability.target_new_y;
        out->target_new_map_level = g_battle_action_current_ability.target_new_elevation;
    } else if (acting->action_command.targeting_type == BATTLE_TARGETING_TYPE_UNIT) {
        target = &g_battle_unit_stats[acting->action_command.target_id];
        out->target_new_x = target->x;
        out->target_new_y = target->position.y;
        out->target_new_map_level = target->position.higher_elevation;
    } else {
        out->target_new_x = acting->action_command.target_position.vx;
        out->target_new_y = acting->action_command.target_position.vz;
        out->target_new_map_level = acting->action_command.target_position.vy;
    }
}
