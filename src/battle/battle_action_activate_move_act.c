#include "fft/battle_action.h"

/* Give the unit its move and its action back. Nothing in the game calls it. */
void battle_action_activate_move_act(battle_stats_t* unit) {
    unit->movement_taken = 0;
    unit->action_taken = 0;
}
