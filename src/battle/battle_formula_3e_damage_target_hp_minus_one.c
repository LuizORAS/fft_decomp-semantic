#include "fft/battle.h"
#include "psx/types.h"

/* Formula 0x3E: damage of the target's current HP - 1 (battle_formula_calculate_damage_leaving_one_hp)
 * with no evade check or hit roll, unlike formula 0x17. No retail ability, weapon or item uses it. */
void battle_formula_3e_damage_target_hp_minus_one(void) {
    battle_formula_calculate_damage_leaving_one_hp();
}
