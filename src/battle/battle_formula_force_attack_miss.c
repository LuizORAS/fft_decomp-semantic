#include "fft/battle.h"
#include "psx/types.h"

/* Mark the action a forced failure on the target: no hit, miss type FORCED_FAILURE, accuracy 0. */
void battle_formula_force_attack_miss(void) {
    g_battle_action_target_data->hit = 0;
    g_battle_action_target_data->miss_type = BATTLE_ACTION_MISS_TYPE_FORCED_FAILURE;
    /* The accuracy field is cleared as a halfword (sh). */
    g_battle_action_target_data->attack_accuracy = 0;
}
