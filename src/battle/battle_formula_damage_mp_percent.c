#include "fft/battle.h"

/* Formula 0x1B, Magic Ruin: the magical evade check and hit chance (MA + X with both Faiths), then
 * MP damage of Y% of the target's max MP. */
void battle_formula_damage_mp_percent(void) {
    if (battle_formula_calculate_magical_evade() == 0) {
        if (battle_formula_calculate_magic_accuracy() == 0) {
            battle_formula_calculate_mp_percent_damage();
        }
    }
}
