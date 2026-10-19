#include "fft/battle_action.h"
#include "fft/battle_formula.h"

/* Formula 0x51, Choco Esuna, Protect Spirit and Clam Spirit: no evade check; hit chance MA + X with
 * the element's Strengthen, Magic Attack Up and the zodiac, without Faith, then the ability's status. */
void battle_formula_hit_ma_x_percent_no_evade(void) {
    battle_formula_store_ma_and_x();
    battle_formula_apply_ability_element_strengthen();
    battle_formula_apply_magic_attack_up();
    battle_formula_apply_zodiac_compatibility();
    battle_formula_store_hit_chance();
    battle_formula_roll_hit_chance();
    if (g_battle_action_target_data->hit != 0) {
        battle_formula_apply_status_to_action();
    }
}
