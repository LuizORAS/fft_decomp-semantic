#include "fft/battle_action.h"

/* Formula 0x4E: damage (MA * Y). */
void formula_damage_ma_times_y(void) {
    if (formula_calculate_magical_evade() == 0) {
        formula_store_ma_and_y();
        formula_calculate_truth_magical_damage();
    }
}
