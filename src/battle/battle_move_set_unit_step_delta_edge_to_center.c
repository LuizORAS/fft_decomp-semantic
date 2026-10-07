#include "fft/battle.h"
#include "psx/types.h"

enum {
    BATTLE_MOVE_STEP_WIDTH = 28,
    BATTLE_MOVE_STEP_HEIGHT_SCALE = 6,
};

/* Same as battle_move_set_unit_step_delta_center_to_edge for the half step from the entry edge
 * to the tile centre: the tilt is reversed, and step_count is the entry edge's corner scale. */
void battle_move_set_unit_step_delta_edge_to_center(
    battle_unit_misc_data_t* unit, const u8* step, const map_tile_t* tile, s32 step_count) {
    VECTOR direction;

    direction.vy
        = (step_count - 1) * (tile->depth_half_height & MAP_TILE_HALF_HEIGHT_MASK) * BATTLE_MOVE_STEP_HEIGHT_SCALE;
    switch (*step >> 6) {
    case 0:
        direction.vx = BATTLE_MOVE_STEP_WIDTH;
        direction.vz = 0;
        break;
    case 1:
        direction.vx = -BATTLE_MOVE_STEP_WIDTH;
        direction.vz = 0;
        break;
    case 2:
        direction.vx = 0;
        direction.vz = -BATTLE_MOVE_STEP_WIDTH;
        break;
    case 3:
        direction.vx = 0;
        direction.vz = BATTLE_MOVE_STEP_WIDTH;
        break;
    }
    VectorNormal(&direction, &unit->velocity);
    unit->velocity.vx = unit->velocity.vx * g_battle_state_vsync_interval;
    unit->velocity.vy = unit->velocity.vy * g_battle_state_vsync_interval;
    unit->velocity.vz = unit->velocity.vz * g_battle_state_vsync_interval;
}
