#include "fft/battle.h"
#include "psx/types.h"

/* The physical XA modifiers, in order: the weapon-strike supports
 * (battle_formula_apply_physical_attack_supports), then
 * battle_formula_apply_physical_status_xa_modifiers's steps. */
void battle_formula_apply_physical_xa_modifiers(void) {
    battle_formula_apply_physical_attack_supports();
    battle_formula_apply_attacker_berserk_frog();
    battle_formula_apply_defense_up();
    battle_formula_apply_target_physical_status_xa_modifiers();
    battle_formula_apply_zodiac_compatibility();
}
