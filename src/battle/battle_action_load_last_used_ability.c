#include "fft/battle.h"
#include "psx/types.h"

/* Nothing in the game calls it. */
s16 battle_action_load_last_used_ability(void) {
    return g_battle_reaction_unit_last_ability_id;
}
