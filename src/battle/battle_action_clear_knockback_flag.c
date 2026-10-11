#include "fft/battle.h"
#include "psx/types.h"

/* Cancel a pending knockback: clear the current ability's knockback flags and the
 * target's KNOCKBACK special effect. */
void battle_action_clear_knockback_flag(void) {
    u8* flag = &g_battle_action_current_ability.knockback_flags;
    battle_action_data_t* target_data;
    if ((*flag & BATTLE_KNOCKBACK_PENDING) != 0) {
        target_data = g_battle_action_target_data;
        *flag = 0;
        target_data->special_effect &= ~BATTLE_ACTION_SPECIAL_EFFECT_KNOCKBACK;
    }
}
