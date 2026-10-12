#include "fft/battle_action.h"

/* Formula 0x1E: damage ((MA + Y) / 2 * MA) with element; magic evade; 19%
 * status. Strikes rnd(1..X) times
 * (battle_action_init_current_ability_strike_data). */
void battle_formula_multihit_truth_magic(void) {
    if (battle_formula_calculate_magical_evade() == 0) {
        battle_formula_calculate_truth_damage();
    }
}
