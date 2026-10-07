#include "fft/battle.h"

/* Formula 0x50, Secret Fist and monster touches (Eye Gouge, Poison Nail, Black Ink, Zombie Touch,
 * Sleep Touch, Grease Touch, Look of Devil, Beak): the physical evade check and the hit chance MA + X
 * with the physical status modifiers, then the ability's status (battle_formula_apply_status_to_action). */
void battle_formula_hit_ma_x_percent(void) {
    if (battle_formula_calculate_physical_evade() == 0) {
        battle_formula_store_ma_and_x();
        if (battle_formula_calculate_physical_status_accuracy() == 0) {
            battle_formula_apply_status_to_action();
        }
    }
}
