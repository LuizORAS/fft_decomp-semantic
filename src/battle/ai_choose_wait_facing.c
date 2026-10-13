#include "fft/battle_ai.h"

typedef union ai_direction_viability {
    u32 word;
    u8 by_direction[AI_CARDINAL_DIRECTION_COUNT];
} ai_direction_viability_t;

/* Separate locals change the stack slots and coordinate byte stores. */
typedef struct ai_wait_facing_work {
    ai_coords_t target_coords;
    u8 _unused_04[4];
    ai_direction_viability_t viability;
    /* Read as viability.by_direction[AI_DIRECTION_OVERLAP] and never
     * written, so it holds a leftover stack byte (QUIRKS.md). */
    u8 past_viable_directions;
} ai_wait_facing_work_t;

/*
 * Choose a facing hint for an AI-controlled unit's Wait command.
 *
 * Values other than 5 are returned directly. Otherwise prefer the nearest
 * imminent enemy, then any enemy, while rejecting blocked or steeply rising
 * adjacent tiles. The original does not bounds-check the direction lookup, so
 * coincident source and target coordinates can read one byte past the four-byte
 * viability array.
 */
s32 ai_choose_wait_facing(void) {
    ai_data_t* ai = &g_ai_data_base;
    ai_wait_facing_work_t work;
    s32 direction;
    s32 target_id;
    s32 acting_unit_id;
    s32 fallback_direction;
    s32 tried_any_enemy;
    s32 facing;

    if (ai->selected_action.wait_facing_hint != AI_DIRECTION_CHOOSE_AT_WAIT)
        return ai->selected_action.wait_facing_hint;

    ai_copy_unit_coordinates(ai->acting_unit_id, &ai->acting_unit_coords);
    for (direction = 0; direction < AI_CARDINAL_DIRECTION_COUNT; direction++) {
        work.target_coords.fields.x
            = ai->acting_unit_coords.fields.x + g_ai_facing_tile_offsets.by_direction[direction][0];
        work.target_coords.fields.y
            = ai->acting_unit_coords.fields.y + g_ai_facing_tile_offsets.by_direction[direction][1];
        work.target_coords.fields.elevation = 0;
        if (ai_is_facing_tile_blocked(&ai->acting_unit_coords, &work.target_coords) != 0) {
            work.viability.by_direction[direction] = 0;
        } else {
            fallback_direction = direction;
            work.viability.by_direction[direction] = 1;
        }
    }
    if (work.viability.word == 0)
        work.viability.word = 0x01010101;

    for (target_id = 0; target_id < BATTLE_UNIT_SLOT_COUNT; target_id++) {
        ai->targetability.live.unit_targetable[target_id] = 1;
    }
    tried_any_enemy = 0;
    for (;;) {
        acting_unit_id = ai->acting_unit_id;
        target_id = ai_find_nearest_target(AI_NEAREST_IMMINENT_ENEMY);
        if (acting_unit_id == target_id) {
            target_id = ai_find_nearest_target(AI_NEAREST_ENEMY);
            tried_any_enemy = 1;
        }
        ai_copy_unit_coordinates(target_id, &work.target_coords);
        direction = ai_find_direction_of_target(&work.target_coords, &ai->acting_unit_coords);
        if (work.viability.by_direction[direction] != 0)
            return direction;
        if (tried_any_enemy == 0) {
            ai->targetability.live.unit_targetable[target_id] = 0;
            continue;
        }
        facing = ai->acting_unit->position.facing;
        if (work.viability.by_direction[facing] == 0)
            return fallback_direction;
        return facing;
    }
}
