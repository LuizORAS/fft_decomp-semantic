#include "fft/battle_formula.h"

/* Formula 0x1E: damage ((MA + Y) / 2 * MA) with element; magic evade; 19%
 * status. Strikes rnd(1..X) times
 * (action_init_current_ability_strike_data). */
void formula_multihit_truth_magic(void) {
    if (formula_calculate_magical_evade() == 0) {
        formula_calculate_truth_damage();
    }
}
