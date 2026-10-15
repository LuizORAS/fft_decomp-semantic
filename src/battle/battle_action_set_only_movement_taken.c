#include "fft/battle.h"
#include "psx/types.h"

/* Mark the unit as having moved, which can end its turn (battle_action_set_move_act_flags). */
void battle_action_set_only_movement_taken(s32 unit_id) {
    battle_action_set_move_act_flags(unit_id, 1, 0);
}
