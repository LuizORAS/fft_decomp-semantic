#include "fft/battle.h"
#include "psx/types.h"

s32 battle_formula_calculate_physical_accuracy(void) {
    battle_formula_apply_physical_xa_modifiers();
    battle_formula_store_hit_chance();
    battle_formula_use_hp_damage_as_action_hit_percent();
    return g_battle_action_target_data->hit == 0;
}
