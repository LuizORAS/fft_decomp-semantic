#include "fft/battle.h"

/* Return 0 when status is Charm, Don't Move or Don't Act and runs out before the unit's next turn
 * (battle_ai_calculate_clockticks_until_unit_acts; equality counts as lasting), else 1. Presence is
 * not tested. */
s32 battle_ai_status_lasts_until_turn(battle_stats_t* unit, s32 status) {
    if (status == BATTLE_STATUS_ID_CHARM || (status - BATTLE_STATUS_ID_DONT_MOVE) < 2U) {
        if (battle_ai_calculate_clockticks_until_unit_acts(unit) > unit->status_ct[BATTLE_STATUS_CT_INDEX(status)]) {
            return 0;
        }
    }
    return 1;
}
