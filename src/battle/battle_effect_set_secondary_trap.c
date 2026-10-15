#include "fft/battle.h"

/* Show the trap effect on a unit that has just sprung a trap: battle_move_start_next_post_move_event
 * starts it for every trap, whatever its kind. */
void battle_effect_set_secondary_trap(battle_unit_misc_data_t* unit) {
    battle_effect_set_secondary(unit, BATTLE_EFFECT_SECONDARY_ANIMATION_TRAP);
}
