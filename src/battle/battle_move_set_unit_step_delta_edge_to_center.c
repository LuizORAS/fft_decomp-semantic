#include "fft/battle_move.h"
#include "psx/types.h"

/* Same as battle_move_set_unit_step_delta_center_to_edge for the half step from the entry edge
 * to the tile centre: the height part is not negated, so entering over a high side moves the
 * unit down to the centre, and edge_height is the entry side's slope_type value.
 * battle_move_set_float_step_delta_edge_to_center, for a floating unit's steps, is a second copy of
 * this text. */
void battle_move_set_unit_step_delta_edge_to_center(
    battle_unit_t* unit, const battle_move_step_t* step, const map_tile_t* tile, s32 edge_height) {
    VECTOR direction;

    direction.vy = (edge_height - 1) * tile->depth_half_height.half_height * BATTLE_MOVE_STEP_HEIGHT_SCALE;
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
