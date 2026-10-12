#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "psx/types.h"

/* Hit chance (XA+YA)% with every physical modifier on XA, Two Hands included.
 * Returns 1 on a miss. */
s32 formula_calculate_physical_accuracy(void) {
    formula_apply_physical_xa_modifying_statuses_support();
    formula_store_xa_plus_ya_status_damage();
    formula_use_hp_damage_as_action_hit_percent();
    return g_action_target_data->hit == 0;
}
