#include "fft/battle.h"

/* Formula 0x47, Blood Suck: no evade or hit roll; Y% of the target's max HP drained
 * (battle_formula_apply_hp_absorption), then the ability's status on every use
 * (battle_formula_apply_status). */
void battle_formula_absorb_hp_y_percent_status(void) {
    battle_formula_calculate_hp_percent_damage();
    battle_formula_apply_hp_absorption();
    battle_formula_apply_status();
}
