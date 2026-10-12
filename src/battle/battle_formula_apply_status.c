#include "fft/battle_action.h"
#include "psx/types.h"

/* Add the ability's status as a side effect of a hit: the hit flag, accuracy
 * and miss type are kept when the status does not apply. */
void battle_formula_apply_status(void) {
    u8 hit, miss_type;
    u16 accuracy;

    miss_type = g_action_target_data->miss_type;
    /* The accuracy field is read and written as a 16-bit value (lhu/sh). */
    accuracy = g_action_target_data->attack_accuracy;
    hit = g_action_target_data->hit;
    battle_formula_apply_status_to_action();
    g_action_target_data->hit = hit;
    g_action_target_data->attack_accuracy = accuracy;
    g_action_target_data->miss_type = miss_type;
}
