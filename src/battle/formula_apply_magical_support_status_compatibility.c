#include "fft/battle_formula.h"

/* Magical modifiers on XA, in order: Magic Attack Up, Magic Defense Up, the
 * target's statuses (Shell, Frog or Chicken), zodiac compatibility. */
void formula_apply_magical_support_status_compatibility(void) {
    formula_apply_magic_attack_up();
    formula_apply_magic_defense_up();
    formula_apply_target_magical_status_xa_modifiers();
    formula_apply_zodiac_compatibility();
}
