#include "fft/battle_action.h"
#include "fft/battle_formula.h"

/* Formula 0x2A: talk skill, hit (MA+X)%: gil (Level * SP), CT to 0, +/-Brave
 * or Faith (Y), or status by ability. Misses a sleeping target and, without
 * Monster Talk, a monster; Finger Guard can cancel it. */
void formula_talk_skill_hit_ma_x_percent(void) {
    formula_force_sleeping_target_miss();
    if ((g_action_target->unit_flags & UNIT_FLAG_MONSTER)
        && !(g_action_attacker->support_abilities[2] & BATTLE_SUPPORT_SET_3_MONSTER_TALK)) {
        formula_force_attack_miss();
    }
    if (g_action_target_data->hit != 0) {
        formula_apply_finger_guard();
        if (g_action_target_data->hit != 0) {
            formula_store_ma_and_x();
            formula_apply_zodiac_compatibility();
            formula_store_xa_plus_ya_status_damage();
            formula_use_hp_damage_as_action_hit_percent();
            if (g_action_target_data->hit != 0) {
                formula_apply_talk_skill();
            }
        }
    }
}
