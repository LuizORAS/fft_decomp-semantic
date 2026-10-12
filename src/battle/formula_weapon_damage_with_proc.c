#include "fft/battle_action.h"

/* Formula 0x02: as 0x01, but the 19% roll queues the weapon's proc ability on
 * the target instead of a status. */
void formula_weapon_damage_with_proc(void) {
    if (formula_calculate_physical_evade() == 0) {
        if (formula_calculate_weapon_damage() == 0) {
            formula_store_reaction_proc_id_and_target();
        }
    }
}
