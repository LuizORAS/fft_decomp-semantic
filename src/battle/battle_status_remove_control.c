#include "fft/battle.h"
#include "psx/types.h"

void battle_status_remove_control(battle_stats_t* unit) {
    if ((unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CONFUSION)]
            & (BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CONFUSION)
                | BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_BLOOD_SUCK)))
        || (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_BERSERK)]
            & (BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_BERSERK) | BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CHICKEN)))
        || (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CHARM)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CHARM))) {
        unit->team_flags = unit->team_flags & 0xF7;
    } else {
        unit->team_flags = unit->team_flags | (unit->initial_team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED);
    }
    if ((unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CHARM)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CHARM))
        == 0) {
        unit->team_flags = (unit->team_flags & 0xCF) | (unit->initial_team_flags & BATTLE_TEAM_MASK);
    }
}
