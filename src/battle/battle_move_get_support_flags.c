#include "fft/battle_move.h"
#include "psx/types.h"

/* Post-move benefits of the unit's movement abilities: Move-HP Up, Move-MP Up,
 * Move-Get EXP and Move-Get JP, each with the common benefit bit. */
s32 battle_move_get_support_flags(battle_stats_t* unit) {
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
