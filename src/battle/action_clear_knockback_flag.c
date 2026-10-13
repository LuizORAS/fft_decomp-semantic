#include "fft/battle_action.h"
#include "psx/types.h"

/* Cancel a pending knockback: clear the current ability's knockback flags and the
 * target's KNOCKBACK special effect. */
void action_clear_knockback_flag(void) {
    action_data_t* target_data;
    if ((g_action_current_ability.knockback_flags & BATTLE_KNOCKBACK_PENDING) != 0) {
        target_data = g_action_target_data;
        g_action_current_ability.knockback_flags = 0;
        target_data->special_effect &= ~ACTION_SPECIAL_EFFECT_KNOCKBACK;
    }
}
