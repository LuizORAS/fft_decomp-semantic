#include "fft/battle.h"
#include "psx/types.h"

/* Start the unit's next movement step from the current tile centre.
 *
 * Variant of battle_move_start_fly_step without the rider dismount, storing the
 * facing directly rather than through animation 0x12: the step byte's top
 * two bits pick the direction, which sets the facing and step phase; its low
 * two bits extend the destination tile coordinate. The switch has no default, so `facing` is unset
 * for a direction outside 0-3 (none exists), and game state 0x2d leaves the unit's facing unchanged. */
void battle_move_start_unit_step(battle_unit_misc_data_t* unit, const map_tile_t* from, const map_tile_t* unused_to) {
    u8* step;
    s32 step_count;
    s16 facing;
    u8 next;

    step_count = unit->current_edge_height;
    step = &unit->current_path_step.value;
    switch (unit->current_path_step.value >> 6) {
    case BATTLE_MOVE_STEP_DIRECTION_EAST:
        facing = BATTLE_FACING_EAST;
        unit->step_phase = BATTLE_MOVE_STEP_PHASE_WALK_TO_EDGE_EAST;
        next = unit->map_x + 1;
        unit->movement.bytes.destination_x = next + (unit->current_path_step.value & 3);
        break;
    case BATTLE_MOVE_STEP_DIRECTION_WEST:
        facing = BATTLE_FACING_WEST;
        unit->step_phase = BATTLE_MOVE_STEP_PHASE_WALK_TO_EDGE_WEST;
        next = unit->map_x - 1;
        unit->movement.bytes.destination_x = next - (unit->current_path_step.value & 3);
        break;
    case BATTLE_MOVE_STEP_DIRECTION_SOUTH:
        facing = BATTLE_FACING_SOUTH;
        unit->step_phase = BATTLE_MOVE_STEP_PHASE_WALK_TO_EDGE_SOUTH;
        next = unit->map_y - 1;
        unit->movement.bytes.destination_y = next - (unit->current_path_step.value & 3);
        break;
    case BATTLE_MOVE_STEP_DIRECTION_NORTH:
        facing = BATTLE_FACING_NORTH;
        unit->step_phase = BATTLE_MOVE_STEP_PHASE_WALK_TO_EDGE_NORTH;
        next = unit->map_y + 1;
        unit->movement.bytes.destination_y = next + (unit->current_path_step.value & 3);
        break;
    }
    if (g_battle_game_state != BATTLE_GAME_STATE_ACTION_EXECUTE) {
        unit->facing = facing;
        battle_unit_set_idle_animation_for_movement(unit);
    }
    battle_move_set_unit_step_delta_center_to_edge(unit, step, from, step_count);
    battle_move_interpolate_partial(unit, unit->walk_speed.word);
    unit->movement.bytes.destination_z = (*step >> 5) & 1;
}
