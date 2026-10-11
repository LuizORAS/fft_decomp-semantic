#include "fft/battle.h"
#include "psx/types.h"

/* Return 1 when unit_index gets a turn and can move before the considered ability's charge lands:
 * the ability charges, the unit's next turn comes within its CT
 * (battle_ai_calculate_clockticks_until_unit_acts) and it can move by then
 * (battle_ai_can_unit_move_within). */
s32 battle_ai_moves_before_charge_lands(s32 unit_index) {
    s32 min_ct;
    u8 ct;

    ct = g_battle_ai_data_base.considered_ability.ct;
    if (ct != 0) {
        min_ct = battle_ai_calculate_clockticks_until_unit_acts(&g_battle_unit_stats[unit_index]);
        if (min_ct != 0x7FFFFFFF) {
            if (ct >= min_ct) {
                return battle_ai_can_unit_move_within(ct, unit_index);
            }
        }
    }
    return 0;
}
