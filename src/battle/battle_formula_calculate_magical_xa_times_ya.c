#include "fft/battle.h"

/* XA * YA for a healing formula: Magic Attack Up and the zodiac compatibility on XA (no target
 * defenses), stored as HP damage for the caller to turn into healing. */
void battle_formula_calculate_magical_xa_times_ya(void) {
    battle_formula_apply_magic_attack_up();
    battle_formula_apply_zodiac_compatibility();
    battle_formula_store_xa_times_ya_damage();
}
