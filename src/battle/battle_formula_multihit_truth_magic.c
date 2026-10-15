#include "fft/battle.h"

/* Formula 0x1E, Rafa's Truth abilities (Heaven Thunder, Asura, Diamond Sword, Hydragon Pit, Space
 * Storage, Sky Demon) and Holy Bracelet: the magical evade check, then XA = MA and YA = (MA + Y) / 2
 * as magical damage without Faith (battle_formula_calculate_truth_damage). While executing, the
 * ability strikes 1 to X times at random (battle_action_init_current_ability_strike_data). */
void battle_formula_multihit_truth_magic(void) {
    if (battle_formula_calculate_magical_evade() == 0) {
        battle_formula_calculate_truth_damage();
    }
}
