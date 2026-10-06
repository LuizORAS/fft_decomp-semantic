#include "fft/battle.h"

/* Formula 0x02, the weapons that cast a spell (Ice Brand, Flame Rod, Ice Rod, Thunder Rod, Flame
 * Whip, Lightning Bow, Holy Lance): formula 0x01's strike, but the 19% roll queues the weapon's
 * spell to follow the hit (battle_formula_queue_weapon_spell) instead of adding a status. */
void battle_formula_weapon_damage_with_proc(void) {
    if (battle_formula_calculate_physical_evade() == 0) {
        if (battle_formula_calculate_weapon_damage() == 0) {
            battle_formula_queue_weapon_spell();
        }
    }
}
