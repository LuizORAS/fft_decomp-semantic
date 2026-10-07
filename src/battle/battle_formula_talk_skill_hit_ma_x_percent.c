#include "fft/battle.h"
#include "psx/types.h"

/* Formula 0x2A, the Talk Skill abilities (Invitation, Persuade, Praise, Threaten, Preach, Solution,
 * Death Sentence, Negotiate, Insult, Mimic Daravon): a sleeping target fails, and so does a monster
 * unless the speaker has Monster Talk; then Finger Guard (battle_formula_apply_finger_guard), the hit
 * chance MA + X with the zodiac, and the talk's effect (battle_formula_apply_talk_skill). No evade
 * check. */
void battle_formula_talk_skill_hit_ma_x_percent(void) {
    battle_formula_force_sleeping_target_miss();
    if ((g_battle_action_target->unit_flags & UNIT_FLAG_MONSTER)
        && !(g_battle_action_attacker->support_abilities[2] & BATTLE_SUPPORT_SET_3_MONSTER_TALK)) {
        battle_formula_force_attack_miss();
    }
    if (g_battle_action_target_data->hit != 0) {
        battle_formula_apply_finger_guard();
        if (g_battle_action_target_data->hit != 0) {
            battle_formula_store_ma_and_x();
            battle_formula_apply_zodiac_compatibility();
            battle_formula_store_hit_chance();
            battle_formula_roll_hit_chance();
            if (g_battle_action_target_data->hit != 0) {
                battle_formula_apply_talk_skill();
            }
        }
    }
}
