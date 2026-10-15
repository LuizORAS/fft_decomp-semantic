#include "fft/battle.h"
#include "psx/types.h"

/* Same as battle_formula_calculate_magic_accuracy, without the element's Strengthen. */
s32 battle_formula_calculate_magic_accuracy_without_strengthen(void) {
    battle_formula_store_ma_and_x();
    battle_formula_apply_magical_xa_modifiers();
    battle_formula_store_hit_chance();
    battle_formula_calculate_faith();
    battle_formula_roll_hit_chance();
    return g_battle_action_target_data->hit == 0;
}
