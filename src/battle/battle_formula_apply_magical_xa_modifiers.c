#include "fft/battle.h"

/* The magical XA modifiers, in order: Magic Attack Up, Magic Defense Up, the target's Shell, Frog and
 * Chicken (battle_formula_apply_target_magical_status_xa_modifiers) and the zodiac compatibility. */
void battle_formula_apply_magical_xa_modifiers(void) {
    battle_formula_apply_magic_attack_up();
    battle_formula_apply_magic_defense_up();
    battle_formula_apply_target_magical_status_xa_modifiers();
    battle_formula_apply_zodiac_compatibility();
}
