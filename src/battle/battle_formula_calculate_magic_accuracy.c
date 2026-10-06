#include "fft/battle.h"
#include "psx/types.h"

s32 battle_formula_calculate_magic_accuracy(void) {
    battle_formula_store_ma_and_x();
    battle_formula_apply_ability_element_strengthen();
    battle_formula_apply_magical_xa_modifiers();
    battle_formula_store_hit_chance();
    battle_formula_calculate_faith();
    battle_formula_use_hp_damage_as_action_hit_percent();
    return g_battle_action_target_data->hit == 0;
}
