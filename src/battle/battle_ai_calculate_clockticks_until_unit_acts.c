#include "fft/battle.h"
#include "fft/battle_ai.h"
#include "psx/types.h"

/* Clockticks until the unit gets a turn: its Stop CT plus (100 - CT) / Speed
 * rounded up, and no less than its Sleep CT while it sleeps. 0x7fffffff when
 * it never will: Speed 0, crystal, petrified, treasure, or dead without
 * Reraise (from its decision record). Don't Act and Don't Move are not looked
 * at. */
s32 battle_ai_calculate_clockticks_until_unit_acts(battle_stats_t* unit) {
    battle_ai_data_t* ai;
    s32 speed;
    s32 ticks;
    s32 rem;

    ai = &g_battle_ai_data_base;
    speed = unit->attributes[UNIT_ATTRIBUTE_SPEED];
    ticks = 0;
    if (speed == 0) {
        return 0x7FFFFFFF;
    }
    if ((unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CRYSTAL)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CRYSTAL))
        || (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_PETRIFY)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_PETRIFY))
        || (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_TREASURE)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_TREASURE))) {
        return 0x7FFFFFFF;
    }
    if ((ai->unit_decisions[unit->entd_slot].targeting_flags_2 >> 5) & (BATTLE_AI_TARGET_DEAD_WITHOUT_RERAISE >> 5)) {
        return 0x7FFFFFFF;
    }
    if (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_STOP)]
        & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_STOP)) {
        ticks = unit->status_ct[BATTLE_STATUS_CT_INDEX(BATTLE_STATUS_ID_STOP)];
    }
    if (unit->ct < UNIT_TURN_CT_THRESHOLD) {
        rem = UNIT_TURN_CT_THRESHOLD - unit->ct;
        ticks += rem / speed;
        if (rem % speed != 0) {
            ticks += 1;
        }
    }
    if (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_DEAD)]
        & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DEAD)) {
        return ticks;
    }
    if ((unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_SLEEP)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_SLEEP))
        == 0) {
        return ticks;
    }
    if (ticks < unit->status_ct[BATTLE_STATUS_CT_INDEX(BATTLE_STATUS_ID_SLEEP)]) {
        ticks = unit->status_ct[BATTLE_STATUS_CT_INDEX(BATTLE_STATUS_ID_SLEEP)];
    }
    return ticks;
}
