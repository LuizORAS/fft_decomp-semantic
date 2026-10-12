#include "fft/battle_action.h"
#include "fft/battle_formula.h"

/* Miss that no roll decides: accuracy 0, miss type forced failure. */
void formula_force_attack_miss(void) {
    g_action_target_data->hit = 0;
    g_action_target_data->miss_type = ACTION_MISS_TYPE_FORCED_FAILURE;
    /* The accuracy field is cleared as a 16-bit value (sh). */
    g_action_target_data->attack_accuracy = 0;
}
