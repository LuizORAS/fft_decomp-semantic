#include "fft/battle_action.h"

/* Formula 0x4C: heal (MA * Y) HP. */
void formula_heal_ma_times_y(void) {
    formula_store_ma_and_y();
    formula_calculate_magical_xa_times_ya();
    formula_apply_undead_reversal();
}
