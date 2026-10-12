#include "fft/battle_action.h"

/* Target affinities to the weapon's elements. */
void battle_formula_modify_elemental_damage(void) {
    battle_formula_modify_damage_for_element(g_action_current_ability.weapon_data.element);
}
