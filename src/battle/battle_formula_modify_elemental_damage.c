#include "fft/battle.h"
#include "psx/types.h"

void battle_formula_modify_elemental_damage(void) {
    battle_formula_modify_damage_for_element(g_battle_action_current_ability.weapon_data.element);
}
