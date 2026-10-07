#include "fft/battle.h"
#include "psx/types.h"

/* Keep a unit that shares the mover's tile pushed aside while the mover leaves it: the push
 * (battle_move_displace_unit_along_step_direction, in the direction
 * battle_move_displace_unit_at_destination_tile chose) shrinks from 10 units at the centre to 0 once
 * the mover is 7 units past it. Skipped on the first step and when the mover came onto this tile by
 * a step onto a unit (BATTLE_MOVE_STEP_ON_UNIT).
 *
 * The case order and per-case centre temporaries reproduce the target's cross-jumped tails; indexing
 * from movement_path - 2 reproduces its offset-before-base address add. */
void battle_move_displace_overlapping_unit(battle_unit_misc_data_t* unit, s32 direction) {
    battle_unit_misc_data_t* other;
    s32 dist;
    s32 x;
    s32 y;
    s32 center;

    other = battle_unit_get_overlapping_misc_data_pointer(unit, unit->map_x, unit->map_y, unit->map_z);
    if (unit->movement_path_offset < 2) {
        return;
    }
    if (((unit->movement_path - 2)[unit->movement_path_offset] >> 4) & 1) {
        return;
    }
    if (other == 0) {
        return;
    }
    x = other->map_x * 28 + 14;
    y = other->map_y * 28 + 14;
    switch (direction) {
    case 2:
        center = unit->map_y * 28 + 14;
        dist = unit->screen.vz - center;
        break;
    case 0:
        center = unit->map_y * 28 + 14;
        dist = center - unit->screen.vz;
        break;
    case 3:
        center = unit->map_x * 28 + 14;
        dist = center - unit->screen.vx;
        break;
    case 1:
        center = unit->map_x * 28 + 14;
        dist = unit->screen.vx - center;
        break;
    }
    if (dist >= 8) {
        dist = 7;
    }
    dist -= 7;
    battle_move_displace_unit_along_step_direction(other, x, y, -dist * 3 / 2);
}
