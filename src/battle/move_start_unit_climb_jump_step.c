#include "fft/battle_move.h"
#include "psx/types.h"

/* Start a unit's jump across one or more tiles onto the destination tile.
 *
 * The landing height is the top of a unit standing on the destination (per
 * battle_unit_calculate_height_data) or, when the tile is empty or that unit
 * is flagged 0x80, the tile height plus its slope scaled by destination_edge_height. The
 * vertical speed and frame count come from the rise over the unit's current
 * screen height plus a clearance of two half-heights per tile of jump length.
 * The step byte's top two bits give the direction, as in
 * move_start_unit_jump_step. `unused_10` reproduces an unreferenced
 * 16-byte frame slot below the height data. */
void move_start_unit_climb_jump_step(battle_unit_t* unit, const map_tile_t* from, const map_tile_t* to) {
    VECTOR unused_10;
    battle_unit_height_data_t height_data;
    move_step_t* step;
    s32 height;
    s32 speed;
    s32 time;
    u8 next;

    step = &unit->current_path_step;
    if (unit->standing_on_battle_id_plus_one != 0
        && (battle_unit_calculate_height_data(&height_data, unit->standing_on_battle_id_plus_one - 1),
            (height_data.unit_flags & BATTLE_UNIT_HEIGHT_UNAVAILABLE) == 0)) {
        height = height_data.total_height;
    } else {
        height = to->height * 2 + to->depth_half_height.half_height * unit->destination_edge_height + 2;
    }
    /* Grouping each constant with the gravity term keeps the target's order
     * (scale, then multiply by gravity); the flat spelling scales gravity first. */
    speed
        = SquareRoot12((height - -unit->world_coords.vy / 6 + (step->length + 1) * 2) * (6 * g_move_jump_gravity) * 2);
    time = speed / g_move_jump_gravity;
    unit->velocity.vy = -speed;
    time += SquareRoot12((step->length + 1) * (12 * g_move_jump_gravity) * 2) / g_move_jump_gravity;
    switch (step->direction) {
    case MOVE_STEP_DIRECTION_EAST:
        unit->step_phase = MOVE_STEP_PHASE_JUMP_START_EAST;
        battle_unit_store_animation_facing(BATTLE_UNIT_ANIMATION_JUMP, BATTLE_FACING_EAST, unit);
        next = unit->map_x + 1;
        unit->movement.destination_x = next + step->length;
        unit->velocity.vx = (MAP_TILE_SIZE * ONE / 2) / time;
        unit->velocity.vz = 0;
        break;
    case MOVE_STEP_DIRECTION_WEST:
        unit->step_phase = MOVE_STEP_PHASE_JUMP_START_WEST;
        battle_unit_store_animation_facing(BATTLE_UNIT_ANIMATION_JUMP, BATTLE_FACING_WEST, unit);
        next = unit->map_x - 1;
        unit->movement.destination_x = next - step->length;
        unit->velocity.vx = -(MAP_TILE_SIZE * ONE / 2) / time;
        unit->velocity.vz = 0;
        break;
    case MOVE_STEP_DIRECTION_SOUTH:
        unit->step_phase = MOVE_STEP_PHASE_JUMP_START_SOUTH;
        battle_unit_store_animation_facing(BATTLE_UNIT_ANIMATION_JUMP, BATTLE_FACING_SOUTH, unit);
        next = unit->map_y - 1;
        unit->movement.destination_y = next - step->length;
        unit->velocity.vz = -(MAP_TILE_SIZE * ONE / 2) / time;
        unit->velocity.vx = 0;
        break;
    case MOVE_STEP_DIRECTION_NORTH:
        unit->step_phase = MOVE_STEP_PHASE_JUMP_START_NORTH;
        battle_unit_store_animation_facing(BATTLE_UNIT_ANIMATION_JUMP, BATTLE_FACING_NORTH, unit);
        next = unit->map_y + 1;
        unit->movement.destination_y = next + step->length;
        unit->velocity.vz = (MAP_TILE_SIZE * ONE / 2) / time;
        unit->velocity.vx = 0;
        break;
    }
    unit->movement.destination_z = step->higher;
}
