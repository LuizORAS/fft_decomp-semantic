#include "fft/battle_formula.h"

/* Formula 0x4D: absorb (Y)% of max HP, hit (MA+X)%. */
void formula_absorb_hp_y_percent_hit_ma_x_percent(void) {
    if (formula_calculate_physical_evade() != 0) {
        return;
    }
    formula_store_ma_and_x();
    if (formula_calculate_physical_status_accuracy() != 0) {
        return;
    }
    formula_calculate_hp_percent_damage();
    formula_apply_hp_absorption();
}
