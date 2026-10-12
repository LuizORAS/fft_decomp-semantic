#include "fft/battle_action.h"

/* Formula 0x0A: status, hit F(MA+X)%; magic evade. */
void battle_formula_hit_faith_ma_x_percent(void) {
    if (battle_formula_calculate_magical_evade() == 0) {
        if (battle_formula_calculate_magic_accuracy() == 0) {
            battle_formula_apply_status_to_action();
        }
    }
}
