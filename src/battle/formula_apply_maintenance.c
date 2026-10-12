#include "fft/battle_action.h"
#include "fft/battle_formula.h"

/* Maintenance on the target turns a break or steal into a forced failure.
 *
 * Battle Stats 0x91 bit 0x04 is the Maintenance support. */
void formula_apply_maintenance(void) {
    if (g_action_target->support_abilities[2] & BATTLE_SUPPORT_SET_3_MAINTENANCE) {
        g_action_target_data->hit = 0;
        g_action_target_data->miss_type = ACTION_MISS_TYPE_FORCED_FAILURE;
        g_action_target_data->attack_accuracy = 0;
    }
}
