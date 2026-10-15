#include "fft/battle.h"
#include "psx/types.h"

/* Formula 0x29, Steal Heart, Allure and Nose Bracelet: no evade check; hit chance MA + X with the
 * zodiac; a hit fails (forced failure) when the target's monster, female and male flags equal the
 * caster's, so the same sex or two monsters, and otherwise adds the ability's status
 * (battle_formula_apply_status_to_action). */
void battle_formula_opposite_sex_hit_ma_x_percent(void) {
    battle_formula_store_ma_and_x();
    battle_formula_apply_zodiac_compatibility();
    battle_formula_store_hit_chance();
    battle_formula_roll_hit_chance();
    if (g_battle_action_target_data->hit != 0) {
        if ((g_battle_action_target->unit_flags & (UNIT_FLAG_MONSTER | UNIT_FLAG_FEMALE | UNIT_FLAG_MALE))
            == (g_battle_action_attacker->unit_flags & (UNIT_FLAG_MONSTER | UNIT_FLAG_FEMALE | UNIT_FLAG_MALE))) {
            battle_formula_force_attack_miss();
        } else {
            battle_formula_apply_status_to_action();
        }
    }
}
