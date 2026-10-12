#include "fft/battle_action.h"
#include "fft/unit.h"

/* Formula 0x29: opposite sex only, hit (MA+X)%. */
void formula_opposite_sex_hit_ma_x_percent(void) {
    formula_store_ma_and_x();
    formula_apply_zodiac_compatibility();
    formula_store_xa_plus_ya_status_damage();
    formula_use_hp_damage_as_action_hit_percent();
    if (g_action_target_data->hit != 0) {
        if ((g_action_target->unit_flags & (UNIT_FLAG_MONSTER | UNIT_FLAG_FEMALE | UNIT_FLAG_MALE))
            == (g_action_attacker->unit_flags & (UNIT_FLAG_MONSTER | UNIT_FLAG_FEMALE | UNIT_FLAG_MALE))) {
            formula_force_attack_miss();
        } else {
            formula_apply_status_to_action();
        }
    }
}
