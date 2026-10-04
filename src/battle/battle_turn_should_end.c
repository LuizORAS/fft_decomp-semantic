#include "fft/battle.h"

/* Return 1 when the unit can neither move (moved, or Don't Move) nor act (acted, or
 * battle_formula_can_unit_evade returns nonzero) and has done one of them. */
s32 battle_turn_should_end(battle_stats_t* unit) {
    /* Pin: unpinned, GCC builds the result in $a2 and copies it to $v0. */
    register s32 result __asm__("$2");
    u8 no_move;
    u8 no_action;
    u8 taken;

    result = battle_formula_can_unit_evade(unit);
    no_move = unit->movement_taken
        | (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_DONT_MOVE)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DONT_MOVE));
    no_action = unit->action_taken | result;
    taken = unit->movement_taken | unit->action_taken;
    result = 0;
    if (no_move != 0 && no_action != 0) {
        result = taken != 0;
    }
    return result;
}
