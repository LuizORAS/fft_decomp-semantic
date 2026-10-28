#include "fft/battle_action.h"

/* Return 1 when the unit can neither move (moved, or Don't Move) nor act (acted, or
 * battle_unit_get_action_block returns nonzero) and has done one of them. */
s32 battle_turn_should_end(const battle_stats_t* unit) {
    s32 cannot_act;
    u8 no_move;
    u8 no_action;
    u8 taken;

    cannot_act = battle_unit_get_action_block(unit);
    no_move = unit->movement_taken
        | (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_DONT_MOVE)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DONT_MOVE));
    no_action = unit->action_taken | cannot_act;
    taken = unit->movement_taken | unit->action_taken;
    if (no_move != 0 && no_action != 0 && taken != 0) {
        return 1;
    }
    return 0;
}
