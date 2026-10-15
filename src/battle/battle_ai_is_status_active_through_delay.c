#include "fft/battle.h"

/* Return 1 when unit has status_id and keeps it for delay clockticks (equality counts): Jump until
 * the jump lands (charged_ability_ct), a timed status while its CT lasts unless it is innate, any
 * other status always. */
s32 battle_ai_is_status_active_through_delay(s32 delay, battle_stats_t* unit, s32 status_id) {
    s32 byte_off;

    byte_off = status_id / 8;

    if (!(unit->status_sets.current[byte_off] & (0x80 >> (status_id - byte_off * 8)))) {
        return 0;
    }
    if (status_id == BATTLE_STATUS_ID_JUMP) {
        if (unit->charged_ability_ct < delay) {
            return 0;
        }
    } else {
        if (status_id < BATTLE_STATUS_ID_POISON) {
            return 1;
        }
        if (unit->status_sets.innate[byte_off] & (0x80 >> (status_id - byte_off * 8))) {
            return 1;
        }
        if (unit->status_ct[BATTLE_STATUS_CT_INDEX(status_id)] < delay) {
            return 0;
        }
    }
    return 1;
}
