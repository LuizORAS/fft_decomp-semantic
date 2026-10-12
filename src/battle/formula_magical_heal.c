#include "fft/battle_formula.h"

/* Formula 0x0C: heal F(MA * Y) HP (damage to undead). */
void formula_magical_heal(void) {
    formula_store_ma_and_y();
    formula_apply_elemental_strengthen();
    formula_calculate_magical_xa_times_ya();
    formula_calculate_faith();
    formula_apply_undead_reversal();
}
