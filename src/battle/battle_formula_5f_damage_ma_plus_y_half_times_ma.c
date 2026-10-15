#include "fft/battle.h"

/* Formula 0x5F, Nanoflare: the magical evade check, then formula 0x5E's damage
 * (battle_formula_calculate_truth_damage). */
void battle_formula_5f_damage_ma_plus_y_half_times_ma(void) {
    if (battle_formula_calculate_magical_evade() == 0) {
        battle_formula_calculate_truth_damage();
    }
}
