#include "fft/battle_action.h"
#include "psx/types.h"

/* Cancel a pending knockback: clear g_battle_action_current_ability.knockback_flags and the target's
 * KNOCKBACK special effect. An invalid or killed target, a weapon spell that replaces the hit, Poach
 * and Train, a Golem guard and a nullified action cancel it. */
void battle_action_cancel_knockback(void) {
    battle_action_data_t* special_effect;
    if ((g_battle_action_current_ability.knockback_flags & BATTLE_KNOCKBACK_PENDING) != 0) {
        special_effect = g_battle_action_target_data;
        g_battle_action_current_ability.knockback_flags = 0;
        special_effect->special_effect &= ~BATTLE_ACTION_SPECIAL_EFFECT_KNOCKBACK;
    }
}
