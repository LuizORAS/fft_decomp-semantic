#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "psx/types.h"

/* Hit chance F(MA+X)%: elemental strengthen and the magical modifiers on MA,
 * zodiac compatibility on both terms, then Faith. Returns 1 on a miss. */
s32 formula_calculate_magic_accuracy(void) {
    formula_store_ma_and_x();
    formula_apply_elemental_strengthen();
    formula_apply_magical_support_status_compatibility();
    formula_store_xa_plus_ya_status_damage();
    formula_calculate_faith();
    formula_use_hp_damage_as_action_hit_percent();
    return g_action_target_data->hit == 0;
}
