#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "psx/types.h"

/* Hit chance (XA+YA)% with Attack Up, Martial Arts, Berserk or Frog, Defense
 * Up, the target's statuses and zodiac compatibility on XA. Returns 1 on a
 * miss. */
s32 formula_calculate_physical_status_accuracy(void) {
    formula_apply_attack_up_and_martial_arts();
    formula_apply_physical_status_support_compatibility();
    formula_store_xa_plus_ya_status_damage();
    formula_use_hp_damage_as_action_hit_percent();
    return g_action_target_data->hit == 0;
}
