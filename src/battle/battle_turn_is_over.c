#include "fft/battle.h"
#include "psx/types.h"

/* Return 1 when the unit's turn is over: it has no ENTD slot or no longer has the turn, or a
 * status ends it (CT frozen, dead or asleep), which also clears has_turn. */
s32 battle_turn_is_over(s32 unit_id) {
    battle_stats_t* unit = &g_battle_unit_stats[unit_id];
    if (unit->entd_slot == BATTLE_ENTD_SLOT_NONE) {
        return 1;
    }
    if (!(battle_turn_get_status_flags(unit) & BATTLE_TURN_STATUS_BLOCKS_TURN_MASK)) {
        return unit->has_turn == 0;
    }
    unit->has_turn = 0;
    return 1;
}
