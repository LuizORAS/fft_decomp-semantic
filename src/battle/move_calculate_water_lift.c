#include "fft/battle_move.h"

/* How far above the floor of a water tile the unit stands, in half-height steps: the whole
 * depth with Float or ON_WATER movement, one level less with IN_WATER movement, else 0. */
s32 move_calculate_water_lift(battle_unit_t* unit, map_tile_t* tile) {
    s32 movement_flags;

    if (unit->battle_data != 0) {
        movement_flags = move_get_effective_flags(unit->battle_data);
    } else {
        movement_flags = 0;
    }
    if (tile->depth_half_height.depth) {
        if ((movement_flags & BATTLE_EFFECTIVE_MOVEMENT_FLOAT)
            || (movement_flags & BATTLE_EFFECTIVE_MOVEMENT_ON_WATER)) {
            return tile->depth_half_height.depth * 2;
        }
        if (movement_flags & BATTLE_EFFECTIVE_MOVEMENT_IN_WATER) {
            return (tile->depth_half_height.depth - 1) * 2;
        }
    }
    return 0;
}
