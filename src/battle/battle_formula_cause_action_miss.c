#include "fft/battle_action.h"

/* Miss decided by the accuracy roll. */
void battle_formula_cause_action_miss(void) {
    g_action_target_data->hit = 0;
    g_action_target_data->miss_type = ACTION_MISS_TYPE_ACCURACY_MISS;
}
