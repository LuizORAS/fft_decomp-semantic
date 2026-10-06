#include "fft/battle.h"

/* Formula 0x0C, Cure to Cure 4, Moogle and Fairy: no evade or hit roll; XA = MA and YA = Y with the
 * element's Strengthen, Magic Attack Up and the zodiac compatibility
 * (battle_formula_calculate_magical_xa_times_ya), both Faiths, then restored as HP, or taken as
 * damage by an undead target (battle_formula_apply_undead_reversal). */
void battle_formula_magical_heal(void) {
    battle_formula_store_ma_and_y();
    battle_formula_apply_ability_element_strengthen();
    battle_formula_calculate_magical_xa_times_ya();
    battle_formula_calculate_faith();
    battle_formula_apply_undead_reversal();
}
