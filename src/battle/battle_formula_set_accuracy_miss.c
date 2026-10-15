#include "fft/battle.h"
#include "psx/types.h"

/* Mark the target missed by accuracy: no hit, miss type ACCURACY_MISS. */
void battle_formula_set_accuracy_miss(void) {
    g_battle_action_target_data->hit = 0;
    g_battle_action_target_data->miss_type = BATTLE_ACTION_MISS_TYPE_ACCURACY_MISS;
}
