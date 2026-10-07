#include "fft/battle.h"
#include "psx/types.h"

/* The target's affinities for the weapon's element (battle_formula_apply_element_affinities). */
void battle_formula_apply_weapon_element(void) {
    battle_formula_apply_element_affinities(g_current_ability.weapon_data.element);
}
