#include "fft/battle.h"

/* Formula 0x4C, Choco Cure and Spirit of Life: no evade or hit roll; MA * Y with Magic Attack Up and
 * the zodiac, without Faith (battle_formula_calculate_magical_xa_times_ya), restored as HP, or taken
 * as damage by an undead target. */
void battle_formula_heal_ma_times_y(void) {
    battle_formula_store_ma_and_y();
    battle_formula_calculate_magical_xa_times_ya();
    battle_formula_apply_undead_reversal();
}
