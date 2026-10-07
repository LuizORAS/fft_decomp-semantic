#include "fft/battle.h"
#include "psx/types.h"

/* Formula 0x60: formula 0x5E's damage (battle_formula_calculate_truth_damage) with no evade check;
 * no retail ability, weapon or item uses it. */
void battle_formula_60_damage_ma_plus_y_half_times_ma(void) {
    battle_formula_calculate_truth_damage();
}
