#include "fft/battle.h"
#include "psx/types.h"

/* Pushes a unit sharing the mover's destination tile aside, mirroring
 * battle_move_displace_overlapping_unit but keyed on the pending step's
 * destination tile instead of the unit's current map square.  The trailing
 * loop only advances the shared direction counter g_battle_move_displacement_direction; it has no other
 * effect.
 *
 * The case order and the reused `dist` temporary reproduce the target's
 * cross-jumped tails and its s1 allocation; spelling the second path index as
 * offset + base reproduces the offset-before-base address add. */
void battle_move_displace_unit_at_destination_tile(battle_unit_misc_data_t* unit, s32 direction) {
    battle_unit_misc_data_t* other;
    s32 dist;
    s32 x;
    s32 y;
    s32 center;
    s32 next;

    if (unit->movement_path_offset == unit->path.count) {
        return;
    }
    other = battle_unit_get_overlapping_misc_data_pointer(unit, unit->movement.bytes.destination_x,
        unit->movement.bytes.destination_y, unit->movement.bytes.destination_z);
    if (unit->movement_path_offset == 0) {
        return;
    }
    if (((unit->path.data.steps - 1)[unit->movement_path_offset].value >> 4) & 1) {
        return;
    }
    if (other == 0) {
        return;
    }
    x = other->movement.bytes.destination_x * 28 + 14;
    y = other->movement.bytes.destination_y * 28 + 14;
    switch (direction) {
    case BATTLE_MOVE_DIRECTION_NORTH:
        center = unit->movement.bytes.destination_y * 28 + 14;
        dist = center - unit->screen.vz;
        break;
    case BATTLE_MOVE_DIRECTION_SOUTH:
        center = unit->movement.bytes.destination_y * 28 + 14;
        dist = unit->screen.vz - center;
        break;
    case BATTLE_MOVE_DIRECTION_WEST:
        center = unit->movement.bytes.destination_x * 28 + 14;
        dist = unit->screen.vx - center;
        break;
    case BATTLE_MOVE_DIRECTION_EAST:
        center = unit->movement.bytes.destination_x * 28 + 14;
        dist = center - unit->screen.vx;
        break;
    }
    if (dist >= 8) {
        dist = 7;
    }
    dist -= 7;
    dist = -dist * 3 / 2;
    switch ((unit->movement_path_offset + unit->path.data.steps)->value >> 6) {
    case BATTLE_MOVE_STEP_DIRECTION_EAST:
        next = BATTLE_MOVE_DIRECTION_EAST;
        break;
    case BATTLE_MOVE_STEP_DIRECTION_WEST:
        next = BATTLE_MOVE_DIRECTION_WEST;
        break;
    case BATTLE_MOVE_STEP_DIRECTION_SOUTH:
        next = BATTLE_MOVE_DIRECTION_SOUTH;
        break;
    case BATTLE_MOVE_STEP_DIRECTION_NORTH:
        next = BATTLE_MOVE_DIRECTION_NORTH;
        break;
    }
    for (g_battle_move_displacement_direction = 0; g_battle_move_displacement_direction < 4;
        g_battle_move_displacement_direction++) {
        if (direction == next) {
            if ((g_battle_move_displacement_direction != next)
                && (g_battle_move_displacement_direction != (next ^ 2))) {
                break;
            }
        } else {
            if ((g_battle_move_displacement_direction != (direction ^ 2))
                && (g_battle_move_displacement_direction != (next ^ 2))) {
                break;
            }
        }
    }
    battle_move_displace_unit_along_step_direction(other, x, y, dist);
}
