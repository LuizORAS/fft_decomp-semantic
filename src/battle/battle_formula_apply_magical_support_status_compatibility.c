#include "fft/battle.h"

/* Magical modifiers on XA, in order: Magic Attack Up, Magic Defense Up, the
 * target's statuses (Shell, Frog or Chicken), zodiac compatibility. */
void battle_formula_apply_magical_support_status_compatibility(void) {
    battle_formula_apply_magic_attack_up();
    battle_formula_apply_magic_defense_up();
    battle_formula_apply_target_magical_status_xa_modifiers();
    battle_formula_apply_zodiac_compatibility();
}
