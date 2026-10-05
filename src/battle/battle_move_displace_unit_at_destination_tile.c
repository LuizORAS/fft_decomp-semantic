#include "fft/battle.h"
#include "psx/types.h"

/* Push aside a unit standing on the tile the mover is stepping onto: within 7 units of that tile's
 * centre the occupant moves off it by (7 - distance) * 3 / 2, up to 10 units at the centre
 * (battle_move_displace_unit_along_step_direction). Skipped on the last path step and on a step onto
 * a unit (BATTLE_MOVE_STEP_ON_UNIT).
 *
 * It also chooses the push direction, g_battle_move_displacement_direction, which
 * battle_move_displace_overlapping_unit reuses while the mover leaves: on a straight path the first
 * direction (by index) across it; on a turn the first that is neither the side the mover comes from
 * nor opposite the next step.
 *
 * The case order and the reused `dist` temporary reproduce the target's cross-jumped tails and its s1
 * allocation; spelling the second path index as offset + base reproduces the offset-before-base
 * address add. */
void battle_move_displace_unit_at_destination_tile(battle_unit_misc_data_t* unit, s32 direction) {
    battle_unit_misc_data_t* other;
    s32 dist;
    s32 x;
    s32 y;
    s32 center;
    s32 next;

    if (unit->movement_path_offset == unit->movement_path_count) {
        return;
    }
    other = battle_unit_get_overlapping_misc_data_pointer(unit, unit->movement.bytes.destination_x,
        unit->movement.bytes.destination_y, unit->movement.bytes.destination_z);
    if (unit->movement_path_offset == 0) {
        return;
    }
    if (((unit->movement_path - 1)[unit->movement_path_offset] >> 4) & 1) {
        return;
    }
    if (other == 0) {
        return;
    }
    x = other->movement.bytes.destination_x * 28 + 14;
    y = other->movement.bytes.destination_y * 28 + 14;
    switch (direction) {
    case 2:
        center = unit->movement.bytes.destination_y * 28 + 14;
        dist = center - unit->screen.vz;
        break;
    case 0:
        center = unit->movement.bytes.destination_y * 28 + 14;
        dist = unit->screen.vz - center;
        break;
    case 3:
        center = unit->movement.bytes.destination_x * 28 + 14;
        dist = unit->screen.vx - center;
        break;
    case 1:
        center = unit->movement.bytes.destination_x * 28 + 14;
        dist = center - unit->screen.vx;
        break;
    }
    if (dist >= 8) {
        dist = 7;
    }
    dist -= 7;
    dist = -dist * 3 / 2;
    switch (*(unit->movement_path_offset + unit->movement_path) >> 6) {
    case 0:
        next = 1;
        break;
    case 1:
        next = 3;
        break;
    case 2:
        next = 0;
        break;
    case 3:
        next = 2;
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
