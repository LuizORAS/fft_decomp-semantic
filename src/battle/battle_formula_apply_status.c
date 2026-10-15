#include "fft/battle.h"
#include "psx/types.h"

/* Add the ability's status as a side effect of a hit (battle_formula_apply_status_to_action),
 * restoring the hit flag, accuracy and miss type afterwards, so a status that cannot land does not
 * turn the hit into a miss. */
void battle_formula_apply_status(void) {
    u8 hit, miss_type;
    u16 accuracy;

    miss_type = g_battle_action_target_data->miss_type;
    /* The accuracy field is read and written as a halfword (lhu/sh). */
    accuracy = g_battle_action_target_data->attack_accuracy;
    hit = g_battle_action_target_data->hit;
    battle_formula_apply_status_to_action();
    g_battle_action_target_data->hit = hit;
    g_battle_action_target_data->attack_accuracy = accuracy;
    g_battle_action_target_data->miss_type = miss_type;
}
