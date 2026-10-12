#include "fft/battle_formula.h"

/* The physical modifiers on XA without the attacker supports: Berserk or Frog,
 * Defense Up, the target's statuses, zodiac compatibility. */
void formula_apply_physical_status_support_compatibility(void) {
    formula_apply_attacker_berserk_frog();
    formula_apply_defense_up();
    formula_apply_target_xa_affecting_statuses();
    formula_apply_zodiac_compatibility();
}
