#include "fft/battle.h"
#include "psx/types.h"

enum {
    BATTLE_MOVE_STEP_WIDTH = 28,
    BATTLE_MOVE_STEP_HEIGHT_SCALE = 6,
};

/* Set the unit's per-frame velocity for the first half of a step, from the tile
 * centre to its exit edge.
 *
 * The direction vector is 28 along the axis the step byte's top two bits select
 * and (step_count - 1) * the tile's slope half-height * 6 in height, negated
 * because height is negative upward. step_count is not a count: it is the
 * slope_type height of the side crossed (0 low, 1 sloping, 2 high), so a low side
 * lies below the centre and a high side above it. VectorNormal reduces the vector
 * to unit length (ONE) and g_main_system_animation_speed scales it; the caller
 * applies the walk speed with battle_move_interpolate_partial. */
void battle_move_set_unit_step_delta_center_to_edge(
    battle_unit_misc_data_t* unit, const u8* step, const map_tile_t* tile, s32 step_count) {
    VECTOR direction;

    direction.vy = -(
        (step_count - 1) * (tile->depth_half_height.value & MAP_TILE_HALF_HEIGHT_MASK) * BATTLE_MOVE_STEP_HEIGHT_SCALE);
    switch (*step >> 6) {
    case BATTLE_MOVE_STEP_DIRECTION_EAST:
        direction.vx = BATTLE_MOVE_STEP_WIDTH;
        direction.vz = 0;
        break;
    case BATTLE_MOVE_STEP_DIRECTION_WEST:
        direction.vx = -BATTLE_MOVE_STEP_WIDTH;
        direction.vz = 0;
        break;
    case BATTLE_MOVE_STEP_DIRECTION_SOUTH:
        direction.vx = 0;
        direction.vz = -BATTLE_MOVE_STEP_WIDTH;
        break;
    case BATTLE_MOVE_STEP_DIRECTION_NORTH:
        direction.vx = 0;
        direction.vz = BATTLE_MOVE_STEP_WIDTH;
        break;
    }
    VectorNormal(&direction, &unit->velocity);
    unit->velocity.vx = unit->velocity.vx * g_main_system_animation_speed;
    unit->velocity.vy = unit->velocity.vy * g_main_system_animation_speed;
    unit->velocity.vz = unit->velocity.vz * g_main_system_animation_speed;
}
