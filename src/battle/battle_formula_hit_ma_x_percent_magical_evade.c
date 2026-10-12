#include "fft/battle.h"

/* Formula 0x3D: status, hit (MA+X)%; magic evade. */
void battle_formula_hit_ma_x_percent_magical_evade(void) {
    if (battle_formula_calculate_magical_evade() == 0) {
        if (battle_formula_calculate_magic_accuracy_without_faith() == 0) {
            battle_formula_apply_status_to_action();
        }
    }
}
