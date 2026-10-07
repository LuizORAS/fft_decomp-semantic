#include "fft/battle.h"
#include "psx/types.h"

/* Formula 0x0A, 38 status spells (Poison, Frog, Slow, Stop, Don't Move, Sleep, Petrify, Silence
 * Song, Dispel Magic, Berserk, Don't Act and others): the magical evade check and hit chance (MA + X
 * with both Faiths), then the spell's status (battle_formula_apply_status_to_action). */
void battle_formula_hit_faith_ma_x_percent(void) {
    if (battle_formula_calculate_magical_evade() == 0) {
        if (battle_formula_calculate_magic_accuracy() == 0) {
            battle_formula_apply_status_to_action();
        }
    }
}
