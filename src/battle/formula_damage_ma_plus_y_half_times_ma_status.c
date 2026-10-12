#include "fft/battle_action.h"

/* Formula 0x60: damage ((MA + Y) / 2 * MA) with element, 19% status; no evade. */
void formula_damage_ma_plus_y_half_times_ma_status(void) {
    formula_calculate_truth_damage();
}
