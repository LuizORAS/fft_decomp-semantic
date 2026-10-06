#include "fft/battle.h"
#include "psx/types.h"

void battle_formula_calculate_truth_magical_damage(void) {
    battle_formula_apply_elemental_strengthen();
    battle_formula_apply_magical_xa_modifiers();
    if (battle_formula_calculate_elemental_xa_times_ya() == 0) {
        battle_formula_apply_elemental_absorption_and_status();
    }
}
