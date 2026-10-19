#include "fft/battle_action.h"
#include "fft/battle_unit.h"
#include "psx/types.h"

/* Record the highest EXP award of the current action (in the primary action only). A hit that changed
 * the target is worth 10, and a KO 20 divided by the target's KO count, which already counts this one
 * (20, then 10, then 6); the target's level minus the attacker's is added, the result is at least 1,
 * Gained EXP Up doubles it, and it stops at 99. */
void battle_unit_update_attacker_earned_experience(s32 outcome) {
    s16 base;
    s32 signed_exp;
    s32 divisor;
    battle_stats_t* attacker;
    s16 exp;
    s32 sum;

    if ((g_battle_action_context == BATTLE_ACTION_CONTEXT_PRIMARY) && (outcome != 0)) {
        if (outcome == 1) {
            base = 10;
        } else {
            divisor = g_battle_action_target->ko_count;
            base = 20;
            if (divisor != 0) {
                base = 20 / divisor;
            }
        }
        attacker = g_battle_action_attacker;
        sum = (base + g_battle_action_target->level) - attacker->level;
        exp = sum;
        if (exp <= 0) {
            exp = 1;
        }
        if (attacker->ability_flags.support[1] & BATTLE_SUPPORT_SET_2_GAINED_EXP_UP) {
            exp = exp * 2;
        }
        if (exp >= 100) {
            exp = 99;
        }
        signed_exp = exp << 16;
        if (g_battle_action_current_ability.earned_experience < (signed_exp >> 16)) {
            g_battle_action_current_ability.earned_experience = exp;
        }
    }
}
