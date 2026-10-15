#include "fft/battle.h"

typedef struct battle_ai_wait_facing_work {
    battle_ai_coords_t target_coords;
    u8 _unused_04[4];
    u8 viable_directions[4];
    /* Read as viable_directions[BATTLE_AI_DIRECTION_OVERLAP] and never
     * written, so it holds a leftover stack byte (QUIRKS.md). */
    u8 past_viable_directions;
} battle_ai_wait_facing_work_t;

/* Return the facing for the acting unit's Wait: the selected action's hint, unless it is
 * BATTLE_AI_DIRECTION_CHOOSE_AT_WAIT. Then the unit faces the nearest imminent enemy, trying the next
 * one while that direction is not viable, and last the nearest enemy. A direction is viable when the
 * tile next to the unit there does not block it (battle_ai_is_facing_tile_blocked); all are when none
 * is. Failing that it keeps its facing when viable, else takes the last viable direction. An enemy on
 * its own tile reads past the four directions (QUIRKS.md). */
s32 battle_ai_choose_wait_facing(void) {
    battle_ai_data_t* ai = &g_battle_ai_data_base;
    battle_ai_wait_facing_work_t work;
    s32 direction;
    s32 target_id;
    s32 acting_unit_id;
    s32 fallback_direction;
    s32 tried_any_enemy;
    s32 facing;

    if (ai->selected_action.wait_facing_hint != BATTLE_AI_DIRECTION_CHOOSE_AT_WAIT)
        return ai->selected_action.wait_facing_hint;

    battle_ai_transfer_unit_coordinates(ai->acting_unit_id, &ai->acting_unit_coords);
    for (direction = 0; direction < 4; direction++) {
        work.target_coords.bytes.x
            = ai->acting_unit_coords.bytes.x + g_battle_ai_facing_tile_offsets.by_direction[direction][0];
        work.target_coords.bytes.y
            = ai->acting_unit_coords.bytes.y + g_battle_ai_facing_tile_offsets.by_direction[direction][1];
        work.target_coords.bytes.elevation = 0;
        if (battle_ai_is_facing_tile_blocked(&ai->acting_unit_coords, &work.target_coords) != 0) {
            work.viable_directions[direction] = 0;
        } else {
            fallback_direction = direction;
            work.viable_directions[direction] = 1;
        }
    }
    if (*(u32*)work.viable_directions == 0)
        *(u32*)work.viable_directions = 0x01010101;

    for (target_id = 0; target_id < BATTLE_UNIT_SLOT_COUNT; target_id++) {
        ai->targetability.live.unit_targetable[target_id] = 1;
    }
    tried_any_enemy = 0;
    for (;;) {
        acting_unit_id = ai->acting_unit_id;
        target_id = battle_ai_find_nearest_target(BATTLE_AI_NEAREST_IMMINENT_ENEMY);
        if (acting_unit_id == target_id) {
            target_id = battle_ai_find_nearest_target(BATTLE_AI_NEAREST_ENEMY);
            tried_any_enemy = 1;
        }
        battle_ai_transfer_unit_coordinates(target_id, &work.target_coords);
        direction = battle_ai_find_direction_of_target((s8*)&work.target_coords, (s8*)&ai->acting_unit_coords);
        if (work.viable_directions[direction] != 0)
            return direction;
        if (tried_any_enemy == 0) {
            ai->targetability.live.unit_targetable[target_id] = 0;
            continue;
        }
        facing = ai->acting_unit->position.bits.facing;
        if (work.viable_directions[facing] == 0)
            return fallback_direction;
        return facing;
    }
}
