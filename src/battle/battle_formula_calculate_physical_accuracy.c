#include "fft/battle.h"
#include "psx/types.h"

/* Physical hit chance from the caller's XA and YA: the physical XA modifiers
 * (battle_formula_apply_physical_xa_modifiers), then XA + YA rolled as a percentage
 * (battle_formula_roll_hit_chance). Returns 1 on a miss. */
s32 battle_formula_calculate_physical_accuracy(void) {
    battle_formula_apply_physical_xa_modifiers();
    battle_formula_store_hit_chance();
    battle_formula_roll_hit_chance();
    return g_battle_action_target_data->hit == 0;
}
