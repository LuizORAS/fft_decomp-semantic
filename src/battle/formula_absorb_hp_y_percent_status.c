#include "fft/battle_formula.h"

/* Formula 0x47: absorb (Y)% of max HP, 100% status. */
void formula_absorb_hp_y_percent_status(void) {
    formula_calculate_hp_percent_damage();
    formula_apply_hp_absorption();
    formula_apply_status();
}
