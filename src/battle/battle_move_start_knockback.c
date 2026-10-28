#include "fft/battle_move.h"

/* Start a knockback: copy the destination held in the attacker's record (target_new_x,
 * target_new_y, target_new_map_level) into the target's movement destination and set up its
 * one-step move (battle_move_init_knockback). Called for BATTLE_ACTION_SPECIAL_EFFECT_KNOCKBACK. */
void battle_move_start_knockback(const battle_unit_t* attacker, battle_unit_t* target) {
    target->movement.destination_x = attacker->strike.target_new_x;
    target->movement.destination_y = attacker->strike.target_new_y;
    target->movement.destination_z = attacker->strike.target_new_map_level;
    battle_move_init_knockback(target);
}
