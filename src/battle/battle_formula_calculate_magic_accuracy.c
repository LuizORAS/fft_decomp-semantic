#include "fft/battle.h"
#include "psx/types.h"

/* Magical hit chance: XA = MA, YA = X, Strengthen for the ability's element, the magical XA
 * modifiers, then XA + YA scaled by both Faiths (battle_formula_calculate_faith) and rolled as a
 * percentage. Returns 1 on a miss. */
s32 battle_formula_calculate_magic_accuracy(void) {
    battle_formula_store_ma_and_x();
    battle_formula_apply_ability_element_strengthen();
    battle_formula_apply_magical_xa_modifiers();
    battle_formula_store_hit_chance();
    battle_formula_calculate_faith();
    battle_formula_roll_hit_chance();
    return g_battle_action_target_data->hit == 0;
}
