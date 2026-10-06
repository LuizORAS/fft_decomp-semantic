#include "fft/battle.h"
#include "psx/types.h"

/* Cancel a pending knockback: clear g_current_ability.knockback_flags and the target's KNOCKBACK
 * special effect. An invalid or killed target, a weapon spell that replaces the hit, Poach and Train,
 * a Golem guard and a nullified action cancel it. */
void battle_action_cancel_knockback(void) {
    u8* flag = &g_current_ability.knockback_flags;
    u16* special_effect;
    if ((*flag & BATTLE_KNOCKBACK_PENDING) != 0) {
        special_effect = &g_battle_action_target_data->special_effect;
        *flag = 0;
        *special_effect &= ~BATTLE_ACTION_SPECIAL_EFFECT_KNOCKBACK;
    }
}
