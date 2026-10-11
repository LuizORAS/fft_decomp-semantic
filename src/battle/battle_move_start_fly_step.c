#include "fft/battle.h"
#include "psx/types.h"

/* Start the unit's next movement step, dismounting any rider first.
 *
 * Variant of battle_move_start_unit_step that sets the facing through
 * battle_unit_store_animation_facing with animation 0x12: the step byte's top
 * two bits pick the direction, facing and edge offset, and its low two bits
 * extend the destination tile coordinate. A carried rider (mount state MOUNT)
 * takes animation 0x32 with the mount's facing. The caller also passes the
 * current and destination tiles, which this variant never reads. */
void battle_move_start_fly_step(
    battle_unit_misc_data_t* unit, const map_tile_t* unused_current_tile, const map_tile_t* unused_destination_tile) {
    battle_move_step_t current_step;
    s32 facing;
    battle_move_step_t* step;
    battle_unit_misc_data_t* rider;
    u8 next;

    battle_unit_dismount_rider(unit);
    step = &unit->current_path_step;
    /* The byte copy prevents a wider direction load. */
    current_step = unit->current_path_step;
    switch (current_step.bits.direction) {
    case BATTLE_MOVE_STEP_DIRECTION_EAST:
        facing = BATTLE_FACING_EAST;
        unit->step_phase = BATTLE_MOVE_STEP_PHASE_FLY_TO_EDGE_EAST;
        next = unit->map_x + 1;
        unit->movement.bytes.destination_x = next + unit->current_path_step.bits.length;
        break;
    case BATTLE_MOVE_STEP_DIRECTION_WEST:
        facing = BATTLE_FACING_WEST;
        unit->step_phase = BATTLE_MOVE_STEP_PHASE_FLY_TO_EDGE_WEST;
        next = unit->map_x - 1;
        unit->movement.bytes.destination_x = next - unit->current_path_step.bits.length;
        break;
    case BATTLE_MOVE_STEP_DIRECTION_SOUTH:
        facing = BATTLE_FACING_SOUTH;
        unit->step_phase = BATTLE_MOVE_STEP_PHASE_FLY_TO_EDGE_SOUTH;
        next = unit->map_y - 1;
        unit->movement.bytes.destination_y = next - unit->current_path_step.bits.length;
        break;
    case BATTLE_MOVE_STEP_DIRECTION_NORTH:
        facing = BATTLE_FACING_NORTH;
        unit->step_phase = BATTLE_MOVE_STEP_PHASE_FLY_TO_EDGE_NORTH;
        next = unit->map_y + 1;
        unit->movement.bytes.destination_y = next + unit->current_path_step.bits.length;
        break;
    }

    battle_unit_store_animation_facing(BATTLE_UNIT_ANIMATION_FLY, facing, unit);
    if (unit->mount_state == BATTLE_MISC_MOUNT_STATE_MOUNT) {
        rider = battle_unit_get_misc_data_by_misc_id(unit->mount_partner_misc_id);
        if (rider != 0) {
            battle_unit_store_animation_facing(BATTLE_UNIT_ANIMATION_RIDING, unit->facing, rider);
        }
    }
    battle_move_set_velocity_for_contiguous_clamped_steps(unit, &unit->path, step);
    battle_move_interpolate_partial(unit, unit->walk_speed);
    unit->movement.bytes.destination_z = step->bits.higher;
}
