#include "fft/battle.h"

/* The physical XA modifiers without the attacker's supports, in order: the attacker's Berserk and
 * Frog, the target's Defense Up, its Protect, Sleep, Charging, Chicken and Frog
 * (battle_formula_apply_target_physical_status_xa_modifiers), and the zodiac compatibility. */
void battle_formula_apply_physical_status_xa_modifiers(void) {
    battle_formula_apply_attacker_berserk_frog();
    battle_formula_apply_defense_up();
    battle_formula_apply_target_physical_status_xa_modifiers();
    battle_formula_apply_zodiac_compatibility();
}
