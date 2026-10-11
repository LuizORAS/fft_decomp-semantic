#include "fft/battle.h"
#include "psx/types.h"

/* Start the unit's next movement step from the current tile centre.
 *
 * The step byte's top two bits pick the direction, which sets the facing and
 * edge offset; its low two bits extend the destination tile coordinate. The
 * second tile argument is unused. */
void battle_move_start_float_step(battle_unit_misc_data_t* unit, const map_tile_t* from, const map_tile_t* to) {
    battle_move_step_t* step;
    s32 edge_height;
    s16 facing;
    u8 next;

    battle_unit_dismount_rider_and_update_display(unit);
    edge_height = unit->current_edge_height;
    step = &unit->current_path_step;
    switch (unit->current_path_step.value >> 6) {
    case BATTLE_MOVE_STEP_DIRECTION_EAST:
        facing = BATTLE_FACING_EAST;
        unit->step_phase = BATTLE_MOVE_STEP_PHASE_FLOAT_TO_EDGE_EAST;
        next = unit->map_x + 1;
        unit->movement.bytes.destination_x = next + (unit->current_path_step.value & 3);
        break;
    case BATTLE_MOVE_STEP_DIRECTION_WEST:
        facing = BATTLE_FACING_WEST;
        unit->step_phase = BATTLE_MOVE_STEP_PHASE_FLOAT_TO_EDGE_WEST;
        next = unit->map_x - 1;
        unit->movement.bytes.destination_x = next - (unit->current_path_step.value & 3);
        break;
    case BATTLE_MOVE_STEP_DIRECTION_SOUTH:
        facing = BATTLE_FACING_SOUTH;
        unit->step_phase = BATTLE_MOVE_STEP_PHASE_FLOAT_TO_EDGE_SOUTH;
        next = unit->map_y - 1;
        unit->movement.bytes.destination_y = next - (unit->current_path_step.value & 3);
        break;
    case BATTLE_MOVE_STEP_DIRECTION_NORTH:
        facing = BATTLE_FACING_NORTH;
        unit->step_phase = BATTLE_MOVE_STEP_PHASE_FLOAT_TO_EDGE_NORTH;
        next = unit->map_y + 1;
        unit->movement.bytes.destination_y = next + (unit->current_path_step.value & 3);
        break;
    }
    unit->facing = facing;
    battle_unit_set_idle_animation_for_movement(unit);
    battle_move_set_float_step_delta_center_to_edge(unit, step, from, edge_height);
    battle_move_interpolate_partial(unit, unit->walk_speed);
    unit->movement.bytes.destination_z = (step->value >> 5) & 1;
}
