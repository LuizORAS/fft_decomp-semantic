#include "fft/battle_action.h"

/* Formula 0x1B: MP damage (Y)% of max MP, hit F(MA+X)%; magic evade. */
void formula_damage_mp_percent(void) {
    if (formula_calculate_magical_evade() == 0) {
        if (formula_calculate_magic_accuracy() == 0) {
            formula_calculate_mp_percent_damage();
        }
    }
}
