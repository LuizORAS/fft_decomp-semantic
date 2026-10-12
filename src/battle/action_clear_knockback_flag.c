#include "fft/battle_action.h"
#include "psx/types.h"

/* Cancel a pending knockback: clear the current ability's knockback flags and the
 * target's KNOCKBACK special effect. */
void action_clear_knockback_flag(void) {
    u8* flag = &g_action_current_ability.knockback_flags;
    action_data_t* target_data;
    if ((*flag & BATTLE_KNOCKBACK_PENDING) != 0) {
        target_data = g_action_target_data;
        *flag = 0;
        target_data->special_effect &= ~ACTION_SPECIAL_EFFECT_KNOCKBACK;
    }
}
