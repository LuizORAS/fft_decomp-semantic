#include "fft/battle_ai.h"
#include "fft/battle_status.h"
#include "psx/types.h"

/* Clockticks until the unit gets a turn: its Stop CT plus (100 - CT) / Speed
 * rounded up, and no less than its Sleep CT while it sleeps. 0x7fffffff when
 * it never will: Speed 0, crystal, petrified, treasure, or dead without
 * Reraise (from its decision record). Don't Act and Don't Move are not looked
 * at. */
s32 ai_calculate_clockticks_until_unit_acts(const unit_stats_t* unit) {
    ai_data_t* ai;
    s32 speed;
    s32 ticks;
    s32 rem;

    ai = &g_ai_data_base;
    speed = unit->attributes[UNIT_ATTRIBUTE_SPEED];
    ticks = 0;
    if (speed == 0) {
        return 0x7FFFFFFF;
    }
    if ((unit->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_CRYSTAL)] & STATUS_BYTE_MASK(STATUS_ID_CRYSTAL))
        || (unit->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_PETRIFY)] & STATUS_BYTE_MASK(STATUS_ID_PETRIFY))
        || (unit->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_TREASURE)] & STATUS_BYTE_MASK(STATUS_ID_TREASURE))) {
        return 0x7FFFFFFF;
    }
    if ((ai->unit_decisions[unit->entd_slot].targeting_flags_2 >> 5) & (AI_TARGET_DEAD_WITHOUT_RERAISE >> 5)) {
        return 0x7FFFFFFF;
    }
    if (unit->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_STOP)] & STATUS_BYTE_MASK(STATUS_ID_STOP)) {
        ticks = unit->status_ct[STATUS_CT_INDEX(STATUS_ID_STOP)];
    }
    if (unit->ct < UNIT_TURN_CT_THRESHOLD) {
        rem = UNIT_TURN_CT_THRESHOLD - unit->ct;
        ticks += rem / speed;
        if (rem % speed != 0) {
            ticks += 1;
        }
    }
    if (unit->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_DEAD)] & STATUS_BYTE_MASK(STATUS_ID_DEAD)) {
        return ticks;
    }
    if ((unit->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_SLEEP)] & STATUS_BYTE_MASK(STATUS_ID_SLEEP)) == 0) {
        return ticks;
    }
    if (ticks < unit->status_ct[STATUS_CT_INDEX(STATUS_ID_SLEEP)]) {
        ticks = unit->status_ct[STATUS_CT_INDEX(STATUS_ID_SLEEP)];
    }
    return ticks;
}
