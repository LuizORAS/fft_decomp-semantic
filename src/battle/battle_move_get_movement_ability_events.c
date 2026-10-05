#include "fft/battle.h"
#include "psx/types.h"

/* The post-move events of the unit's movement abilities: Move-HP Up, Move-MP Up, Move-Get-Exp and
 * Move-Get-JP each add their own BATTLE_MOVE_POST_EVENT_* bit and MOVEMENT_BENEFIT.
 * battle_action_init_movement_ability_benefit reads the same bits. */
s32 battle_move_get_movement_ability_events(battle_stats_t* unit) {
    u8 flags = unit->movement_abilities[1];
    s32 post_event_flags = (0 - (unit->movement_abilities[0] & BATTLE_MOVEMENT_SET_1_MOVE_HP_UP))
        & (BATTLE_MOVE_POST_EVENT_MOVEMENT_BENEFIT | BATTLE_MOVE_POST_EVENT_MOVE_HP_UP);
    if (flags & BATTLE_MOVEMENT_SET_2_MOVE_MP_UP)
        post_event_flags |= BATTLE_MOVE_POST_EVENT_MOVEMENT_BENEFIT | BATTLE_MOVE_POST_EVENT_MOVE_MP_UP;
    if (flags & BATTLE_MOVEMENT_SET_2_MOVE_GET_EXP)
        post_event_flags |= BATTLE_MOVE_POST_EVENT_MOVEMENT_BENEFIT | BATTLE_MOVE_POST_EVENT_MOVE_GET_EXP;
    if (flags & BATTLE_MOVEMENT_SET_2_MOVE_GET_JP)
        post_event_flags |= BATTLE_MOVE_POST_EVENT_MOVEMENT_BENEFIT | BATTLE_MOVE_POST_EVENT_MOVE_GET_JP;
    return post_event_flags;
}
