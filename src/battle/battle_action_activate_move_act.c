#include "fft/battle.h"

/* Give the unit back its Move and Act (clear movement_taken and action_taken). Nothing on the disc
 * calls it. */
void battle_action_activate_move_act(battle_stats_t* unit) {
    unit->movement_taken = 0;
    unit->action_taken = 0;
}
