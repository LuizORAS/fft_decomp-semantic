#include "fft/battle_formula.h"

/* Formula 0x23: Draw Out heal (MA * Y) HP with no modifiers (damage to
 * undead); katana break roll. */
void formula_draw_out_heal(void) {
    formula_calculate_katana_break_chance();
    formula_store_ma_and_y();
    formula_store_xa_times_ya_damage();
    formula_apply_undead_reversal();
}
