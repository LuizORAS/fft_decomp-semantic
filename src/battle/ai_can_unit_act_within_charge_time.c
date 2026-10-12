#include "fft/battle_ai.h"
#include "fft/battle_unit.h"
#include "psx/types.h"

/* 1 when the unit gets a turn by the considered ability's charge deadline
 * and can act by then; 0 for an instant ability. */
s32 ai_can_unit_act_within_charge_time(s32 unit_index) {
    s32 min_ct;
    u8 ct;

    ct = g_ai_data_base.considered_ability.ct;
    if (ct != 0) {
        min_ct = ai_calculate_clockticks_until_unit_acts(&g_battle_unit_stats[unit_index]);
        if (min_ct != 0x7FFFFFFF) {
            if (ct >= min_ct) {
                return ai_is_unit_active_after_delay(ct, unit_index);
            }
        }
    }
    return 0;
}
