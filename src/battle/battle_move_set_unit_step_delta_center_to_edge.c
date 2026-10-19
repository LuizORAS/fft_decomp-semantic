#include "fft/battle_move.h"
#include "psx/types.h"

/* Set the unit's velocity for the half step from the tile centre to its exit edge: a vector of
 * 28 along the step's direction (path byte bits 6-7) and (edge_height - 1) * the tile's slope
 * half-height * 6 in height, negated since height is negative upward, so it rises toward a high side
 * and falls toward a low one, reduced to unit length (VectorNormal) and times the VSync interval.
 * edge_height is not a count: it is the crossed side's 2-bit slope_type value (0 low, 1 sloping,
 * 2 high). The caller scales it by the walk or step speed (battle_move_interpolate_partial).
 * battle_move_set_float_step_delta_center_to_edge, for a floating unit's steps, is a second copy of
 * this text. */
void battle_move_set_unit_step_delta_center_to_edge(
    battle_unit_t* unit, const battle_move_step_t* step, const map_tile_t* tile, s32 edge_height) {
    VECTOR direction;

    direction.vy = -((edge_height - 1) * tile->depth_half_height.half_height * (MAP_HEIGHT_STEP / 2));
    switch (step->direction) {
    case BATTLE_MOVE_STEP_DIRECTION_EAST:
        direction.vx = BATTLE_MAP_TILE_SIZE;
        direction.vz = 0;
        break;
    case BATTLE_MOVE_STEP_DIRECTION_WEST:
        direction.vx = -BATTLE_MAP_TILE_SIZE;
        direction.vz = 0;
        break;
    case BATTLE_MOVE_STEP_DIRECTION_SOUTH:
        direction.vx = 0;
        direction.vz = -BATTLE_MAP_TILE_SIZE;
        break;
    case BATTLE_MOVE_STEP_DIRECTION_NORTH:
        direction.vx = 0;
        direction.vz = BATTLE_MAP_TILE_SIZE;
        break;
    }
    VectorNormal(&direction, &unit->velocity);
    unit->velocity.vx = unit->velocity.vx * g_main_system_vsync_interval;
    unit->velocity.vy = unit->velocity.vy * g_main_system_vsync_interval;
    unit->velocity.vz = unit->velocity.vz * g_main_system_vsync_interval;
}
