#include "fft/battle.h"
#include "psx/types.h"

/* Same as battle_formula_calculate_physical_accuracy, with Attack Up and Martial Arts in place of
 * the weapon-strike supports (battle_formula_apply_physical_status_xa_modifiers). */
s32 battle_formula_calculate_physical_status_accuracy(void) {
    battle_formula_apply_attack_up_and_martial_arts();
    battle_formula_apply_physical_status_xa_modifiers();
    battle_formula_store_hit_chance();
    battle_formula_roll_hit_chance();
    return g_battle_action_target_data->hit == 0;
}
