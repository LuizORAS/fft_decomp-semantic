#include "fft/battle.h"
#include "psx/types.h"

/* Formula 0x41, Galaxy Stop: no evade check; hit chance MA + X without Faith
 * (battle_formula_calculate_magic_accuracy_without_faith); a target of the caster's zodiac sign fails
 * (forced failure), any other takes the ability's status. */
void battle_formula_41_hit_ma_x_percent_other_sign(void) {
    if (battle_formula_calculate_magic_accuracy_without_faith() == 0) {
        if (g_battle_action_target->birthday.fields.zodiac == g_battle_action_attacker->birthday.fields.zodiac) {
            battle_formula_force_attack_miss();
        } else {
            battle_formula_apply_status_to_action();
        }
    }
}
