#include "fft/battle.h"
#include "psx/types.h"

/* Magical damage that ignores Faith, from the caller's XA and YA: Strengthen for the ability's
 * element, the magical XA modifiers, XA * YA with weather and element, then absorption and the
 * ability's status (battle_formula_apply_elemental_absorption_and_status). Draw Out, Truth and other
 * Faith-free formulas use it. */
void battle_formula_calculate_magical_damage_without_faith(void) {
    battle_formula_apply_ability_element_strengthen();
    battle_formula_apply_magical_xa_modifiers();
    if (battle_formula_calculate_elemental_xa_times_ya() == 0) {
        battle_formula_apply_elemental_absorption_and_status();
    }
}
