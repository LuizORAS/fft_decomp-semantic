#include "fft/battle.h"
#include "psx/types.h"

/* Start a knockback: copy the destination held in the attacker's record (target_new_x,
 * target_new_y, target_new_map_level) into the target's movement destination and set up its
 * one-step move (battle_move_init_knockback). Called for BATTLE_ACTION_SPECIAL_EFFECT_KNOCKBACK. */
void battle_move_start_knockback(battle_unit_misc_data_t* attacker, battle_unit_misc_data_t* target) {
    target->movement.bytes.destination_x = attacker->target_new_x;
    target->movement.bytes.destination_y = attacker->target_new_y;
    target->movement.bytes.destination_z = attacker->target_new_map_level;
    battle_move_init_knockback(target);
}
