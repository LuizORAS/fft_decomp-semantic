#include "fft/battle_action.h"
#include "psx/types.h"

/* Apply an action result's EXP change when executing. Without bit 0x80 the
 * amount is gained: EXP saturates at 255 and the level-up check runs. With it
 * the low seven bits are lost, down to 0. */
void action_handle_steal_exp(unit_stats_t* unit, u8 amount) {
    s32 total;
    u8 capped;
    s16 reduced;

    if (g_action_state != ACTION_STATE_EXECUTE) {
        return;
    }
    if ((amount & 0x80) == 0) {
        total = unit->experience + amount;
        capped = total;
        if (total >= 0x100) {
            capped = 0xFF;
        }
        unit->experience = capped;
        if (main_unit_check_level_up(unit) != 0) {
            if (unit->battle_id == g_battle_acting_unit_id) {
                g_action_current_ability.level_gained_flag = 1;
            }
        }
    } else {
        reduced = unit->experience - (amount & 0x7F);
        if (reduced < 0) {
            reduced = 0;
        }
        unit->experience = reduced;
    }
}
