#include "fft/battle_ai.h"
#include "fft/battle_unit.h"
#include "psx/types.h"

/* 1 when the unit can act limit clockticks from now: present, Speed above 0,
 * not crystal, petrified or treasure, with no Stop, Don't Move or Sleep that
 * lasts that long, and alive, or dead with Reraise and due a turn by then. */
s32 ai_is_unit_active_after_delay(s32 limit, s32 unit_id) {
    unit_stats_t* unit = &g_battle_unit_stats[unit_id];

    if (unit->entd_slot == BATTLE_ENTD_SLOT_NONE) {
        return 0;
    }
    if (unit->attributes[UNIT_ATTRIBUTE_SPEED] == 0) {
        return 0;
    }
    if ((unit->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_CRYSTAL)] & STATUS_BYTE_MASK(STATUS_ID_CRYSTAL))
        || (unit->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_PETRIFY)] & STATUS_BYTE_MASK(STATUS_ID_PETRIFY))
        || (unit->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_TREASURE)] & STATUS_BYTE_MASK(STATUS_ID_TREASURE))) {
        return 0;
    }
    if (ai_is_status_active_through_delay(limit, unit, STATUS_ID_STOP) != 0) {
        return 0;
    }
    if (ai_is_status_active_through_delay(limit, unit, STATUS_ID_DONT_MOVE) != 0) {
        return 0;
    }
    if (ai_is_status_active_through_delay(limit, unit, STATUS_ID_SLEEP) != 0) {
        return 0;
    }
    if ((unit->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_DEAD)] & STATUS_BYTE_MASK(STATUS_ID_DEAD)) == 0) {
        return 1;
    }
    if ((unit->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_RERAISE)] & STATUS_BYTE_MASK(STATUS_ID_RERAISE)) == 0
        || limit < ai_calculate_clockticks_until_unit_acts(unit)) {
        return 0;
    }
    return 1;
}
