#include "fft/battle_action.h"

/* Formula 0x0D: heal (Y)% of max HP, hit F(MA+X)%. */
void formula_heal_y_percent_faith(void) {
    formula_store_ma_and_x();
    formula_apply_elemental_strengthen();
    if (formula_calculate_friendly_magic_accuracy() != 0) {
        return;
    }
    if (formula_apply_status_and_check_undead() == 0) {
        return;
    }
    formula_calculate_hp_percent_damage();
    formula_apply_undead_reversal();
}
