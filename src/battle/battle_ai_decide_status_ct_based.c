#include "fft/battle.h"
#include "psx/types.h"

s32 battle_ai_decide_status_ct_based(s32 limit, s32 unit_id) {
    battle_stats_t* unit = &g_battle_unit_stats[unit_id];

    if (unit->entd_slot == BATTLE_ENTD_SLOT_NONE) {
        return 0;
    }
    if (unit->attributes[UNIT_ATTRIBUTE_SPEED] == 0) {
        return 0;
    }
    if ((unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CRYSTAL)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CRYSTAL))
        || (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_PETRIFY)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_PETRIFY))
        || (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_TREASURE)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_TREASURE))) {
        return 0;
    }
    if (battle_ai_is_status_active_through_delay(limit, unit, BATTLE_STATUS_ID_STOP) != 0) {
        return 0;
    }
    if (battle_ai_is_status_active_through_delay(limit, unit, BATTLE_STATUS_ID_DONT_MOVE) != 0) {
        return 0;
    }
    if (battle_ai_is_status_active_through_delay(limit, unit, BATTLE_STATUS_ID_SLEEP) != 0) {
        return 0;
    }
    if ((unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_DEAD)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DEAD))
        == 0) {
        return 1;
    }
    if ((unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_RERAISE)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_RERAISE))
            == 0
        || limit < battle_ai_calculate_clockticks_until_unit_acts(unit)) {
        return 0;
    }
    return 1;
}
