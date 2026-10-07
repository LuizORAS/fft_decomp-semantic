#include "fft/battle.h"

/* Formula 0x5E, Triple Thunder, Triple Flame and Dark Whisper: the magical evade check, then XA = MA
 * and YA = (MA + Y) / 2 as magical damage without Faith (battle_formula_calculate_truth_damage).
 * While executing, the ability strikes X + 1 times (battle_action_init_current_ability_strike_data). */
void battle_formula_5e_damage_ma_plus_y_half_times_ma(void) {
    if (battle_formula_calculate_magical_evade() == 0) {
        battle_formula_calculate_truth_damage();
    }
}
