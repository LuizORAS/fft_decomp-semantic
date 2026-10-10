#include "fft/battle.h"
#include "psx/types.h"

/* Miss that no roll decides: accuracy 0, miss type forced failure. */
void battle_formula_force_attack_miss(void) {
    g_battle_action_target_data->hit = 0;
    g_battle_action_target_data->miss_type = BATTLE_ACTION_MISS_TYPE_FORCED_FAILURE;
    /* The accuracy field is cleared as a halfword (sh). */
    g_battle_action_target_data->attack_accuracy = 0;
}
