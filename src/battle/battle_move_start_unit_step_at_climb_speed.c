#include "fft/battle.h"
#include "psx/types.h"

/* Start the unit's next movement step with animation 0x23.
 *
 * Variant of battle_move_start_unit_step that sets the facing through
 * battle_unit_store_animation_facing: the step byte's top two bits pick the
 * direction, facing and edge offset, and its low two bits extend the
 * destination tile coordinate. A carried rider (mount state MOUNT) takes
 * animation 0x32 with the mount's facing. The interpolation scale is
 * step_speed, the 0x2000 climb scale, rather than the walk speed. */
void battle_move_start_unit_step_at_climb_speed(
    battle_unit_misc_data_t* unit, const map_tile_t* from, const map_tile_t* unused_to) {
    u8* step;
    s32 step_count;
    battle_unit_misc_data_t* rider;

    step_count = unit->current_edge_height;
    step = &unit->current_path_step.value;
    switch (unit->current_path_step.value >> 6) {
    case BATTLE_MOVE_STEP_DIRECTION_EAST:
        unit->step_phase = BATTLE_MOVE_STEP_PHASE_CLIMB_TO_EDGE_EAST;
        battle_unit_store_animation_facing(BATTLE_UNIT_ANIMATION_CLIMB, BATTLE_FACING_EAST, unit);
        unit->movement.bytes.destination_x = (u8)(unit->map_x + 1) + (unit->current_path_step.value & 3);
        break;
    case BATTLE_MOVE_STEP_DIRECTION_WEST:
        unit->step_phase = BATTLE_MOVE_STEP_PHASE_CLIMB_TO_EDGE_WEST;
        battle_unit_store_animation_facing(BATTLE_UNIT_ANIMATION_CLIMB, BATTLE_FACING_WEST, unit);
        unit->movement.bytes.destination_x = (u8)(unit->map_x - 1) - (unit->current_path_step.value & 3);
        break;
    case BATTLE_MOVE_STEP_DIRECTION_SOUTH:
        unit->step_phase = BATTLE_MOVE_STEP_PHASE_CLIMB_TO_EDGE_SOUTH;
        battle_unit_store_animation_facing(BATTLE_UNIT_ANIMATION_CLIMB, BATTLE_FACING_SOUTH, unit);
        unit->movement.bytes.destination_y = (u8)(unit->map_y - 1) - (unit->current_path_step.value & 3);
        break;
    case BATTLE_MOVE_STEP_DIRECTION_NORTH:
        unit->step_phase = BATTLE_MOVE_STEP_PHASE_CLIMB_TO_EDGE_NORTH;
        battle_unit_store_animation_facing(BATTLE_UNIT_ANIMATION_CLIMB, BATTLE_FACING_NORTH, unit);
        unit->movement.bytes.destination_y = (u8)(unit->map_y + 1) + (unit->current_path_step.value & 3);
        break;
    }
    if (unit->mount_state == BATTLE_MISC_MOUNT_STATE_MOUNT) {
        rider = battle_unit_get_misc_data_by_misc_id(unit->mount_partner_misc_id);
        if (rider != 0) {
            battle_unit_store_animation_facing(BATTLE_UNIT_ANIMATION_RIDING, unit->facing, rider);
        }
    }
    battle_move_set_unit_step_delta_center_to_edge(unit, step, from, step_count);
    battle_move_interpolate_partial(unit, unit->step_speed);
    unit->movement.bytes.destination_z = (*step >> 5) & 1;
}
