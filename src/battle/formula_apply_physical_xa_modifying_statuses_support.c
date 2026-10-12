#include "fft/battle_formula.h"

/* Physical modifiers on XA, in order: attacker supports (Two Hands, Attack Up,
 * Martial Arts), Berserk or Frog, Defense Up, the target's statuses, zodiac
 * compatibility. */
void formula_apply_physical_xa_modifying_statuses_support(void) {
    formula_apply_physical_attack_supports();
    formula_apply_attacker_berserk_frog();
    formula_apply_defense_up();
    formula_apply_target_xa_affecting_statuses();
    formula_apply_zodiac_compatibility();
}
